import sys  
sys.path.append('utils/')
sys.path.append('develop/')

from args import *   # get all CLI arguments 
from methods import *
from Cell import MossyFiber, GranuleCell
import sqlite3         # For getting the connectivity, gid, 
from neuron import h, coreneuron
import random 
import socket  # To get node_names and update in the datanase (not necessary though)


# Random seed for reproducibility
random.seed(42) 

# parse the CLI arguments
args, unknown = parser.parse_known_args()    # parser namespace is from args
run_type = args.run_type    #["coreneuron_gpu", "coreneuron_cpu",  "neuron_serial", "neuron_parallel"]
tstop=args.tstop
trial=args.trial   
 
  
# Setting for coreneuron (cpu or gpu) 
if run_type in {"coreneuron_gpu", "coreneuron_cpu"}:
    from neuron import coreneuron
    coreneuron.enable = True
    coreneuron.file_mode = coreneuron_file_modedend(1)
    coreneuron.cell_permute = coreneuron_permute
    if run_type == "coreneuron_gpu":
        coreneuron.gpu = True

###################################################################### 
# 1. Start the parallel context
######################################################################  
h.nrnmpi_init()
pc = h.ParallelContext()
rank = int(pc.id())
nhost = int(pc.nhost())
#pc.nhost() → total number of MPI ranks (ranks across all nodes) aware of this during slurm
hostname = socket.gethostname()   # this would be used later


# Solver efficiency settings
h.cvode.cache_efficient(1)
h.CVode().use_fast_imem(1) 


###################################################################### 
# 2. Clear worker table (only rank 0) 
######################################################################
 
# Open DB in read-only immutable mode for non-zero ranks:
if rank == 0:
    conn = sqlite3.connect('connectivity/mf_grc.db')
else:
    conn = sqlite3.connect('file:connectivity/mf_grc.db?mode=ro&immutable=1', uri=True) 
cursor = conn.cursor()  

if rank == 0:  # only on the rank 0  clear the worker table as it has to be updated
    cursor.execute("DELETE FROM worker")
    conn.commit()

pc.barrier() 

###################################################################### 
#  3. Hostname → worker_host_id mapping (collectively across all ranks) 
###################################################################### 
## Populate the workers table
hostnames = pc.py_alltoall([hostname])  # Collective MPI
unique_hostnames = sorted(set(hostnames))
host_id_map = {name: idx for idx, name in enumerate(unique_hostnames)}
worker_host_id = host_id_map[hostname]

if rank == 0:   # But update only on rank 0 to avoid race conditioning
    for name, wid in host_id_map.items():
        cursor.execute(
            "INSERT INTO worker (worker_host_id, worker_name) VALUES (?, ?)",
            (wid, name)
        )
    conn.commit() 
pc.barrier()
 
###################################################################### 
# 4. Load GIDs from database/round-robin and update mpi-rank
######################################################################

cursor.execute("SELECT gid FROM cell")
gids = [row[0] for row in cursor.fetchall()]

#  Assign gids via round robin
# this assignment would be changed later based on cell location?
assigned_gids = [gid for gid in gids if gid % nhost == rank]
len_assigned_gids=len(assigned_gids)


# Update assigned gids with mpi_rank and host_id in indivial ranks
for gid in assigned_gids:
    cursor.execute(
        "UPDATE cell SET mpi_rank_id = ?, worker_host_id = ? WHERE gid = ?",
        (rank, worker_host_id, gid)
    )
    
# Get the cell type of the assigned_gids for cell creation
cell_types = [
    cursor.execute("SELECT cell_type FROM cell WHERE gid = ?", (gid,)).fetchone()[0]
    for gid in assigned_gids
]
 
pc.barrier()
###################################################################### 
#  Create units for each cell_type in a particular node (rank)
######################################################################
 
 
cell_list = []
 
for cell_type,gid in zip(cell_types,assigned_gids): 
    
    if cell_type == "mf": 
        cell = MossyFiber(gid)     # From Cell.py
    elif cell_type == "grc": 
        cell = GranuleCell(gid) 
    else:
        raise ValueError(f"Unknown cell type: {cell_type}")
 
    cell_list.append(cell)  # why is this needed?

    # Register this cell in the parallel context
    pc.set_gid2node(cell.gid, rank)  # register the cell (gid) to mpi
    nc = cell.connect2target(None)  # connect the spikes to the target cell, no target just spike detector
    pc.cell(cell.gid, nc)     #this gid is associated; very improtant for spike recording of gids as well 

###################################################################### 
#  Synapses
######################################################################
# 1. Get the synpase for the assigned_gid on a rank
# Right now binary synapse; extend it to include synaptic weight if any
# by selecting the weight as well from the synapse table 
# Convert assigned_gids to a tuple for use in SQL IN clause
# If gid_tuple is empty then there is no synapse from these gids
gid_tuple = tuple(assigned_gids)


if gid_tuple:     # To make sure at least one element for the IN clause
    placeholder = ','.join('?' for _ in gid_tuple)
    query = f"SELECT * FROM synapse WHERE source_gid IN ({placeholder})"
    cursor.execute(query, gid_tuple)
    synapses = cursor.fetchall()  # list of tuples
    
    
    # 2. Connect the synapses only if the gid_tuple is not empty 
    nclist = []
    for source_gid,target_gid,weight,syntype,target_dendid,target_x,delay in synapses:  # weight not needed for binary 
        if pc.gid_exists(target_gid):
            target_cell = pc.gid2cell(target_gid)  # Get the cell 
            syn = target_cell.add_synapses(dendid=target_dendid, x=target_x, syntype=syntype) # made synapse 
            nc = pc.gid_connect(source_gid, syn)
            nc.delay = delay
            nc.weight[0] = weight  
        else:
            pass # log it; it should be present in the table
        
        nclist.append(nc)  # MUST STORE NCs?? 

#synapses = [                      # list of tuples (source_gid, target_gid)
#    result for gid in assigned_gids
#    if (result := cursor.execute(
#        "SELECT * FROM synapse WHERE source_gid = ?", (gid,)  # all rows from the synapse table
#    ).fetchone()) is not None   # possible that some gids maynot have targers
#] 
  
###################################################################### 
# Gap junctions
###################################################################### 
   
# 1. Get the gap junction info for the assigned_gid on a rank
# Good practice rather than select * so that we know what we unpack?
if gid_tuple:
    placeholder = ','.join('?' for _ in gid_tuple)
    query = f"SELECT sid, source_gid, source_sec_id, source_dend_loc, target_gid, target_sec_id, target_dend_loc, g FROM gap WHERE source_gid IN ({placeholder})"
    cursor.execute(query, gid_tuple)
    gaps = cursor.fetchall()  # Each row: (sid, source_gid, source_sec_id, source_dend_loc, target_gid, target_sec_id, target_dend_loc, g)

    # 2. Connect gap junctions 
    for sid, source_gid, source_sec_id, source_x, target_gid, target_sec_id, target_x, _ in gaps:
        if pc.gid_exists(source_gid):  
            source_sec = pc.gid2cell(source_gid).dend[source_sec_id]
            source_seg = source_sec(source_x)
            pc.source_var(source_seg._ref_v, sid, sec=source_sec)
    
        if pc.gid_exists(target_gid):  
            target_cell = pc.gid2cell(target_gid)
            gap = target_cell.add_gap_junction(target_sec_id, target_x, pc=pc)
            pc.target_var(gap, gap._ref_vgap, sid)
    
    pc.setup_transfer() 

#What if create_cell() doesn’t use the DB at all? If you're absolutely sure that:
#create_cell() and all subfunctions don't touch cursor or conn, and
#The only thing left to do is NEURON cell instantiation,
#then technically you could close the connection right after step 5.
#But this is risky and less maintainable because:
#Future changes to create_cell() might add DB calls.
#It's common practice to close the DB only after all logic (including NEURON setup) is done.
# Best Practice: Keep the close at the end. It's clean, robust, and safe.

 
# Close the db finally 
cursor.close()
conn.close()


###############################################################################
# Run the simulation
###############################################################################
h.celsius = 34
h.dt = 0.025
h.stdinit()


# the following vectors are dynamically resized as the spike occurs
tvec = h.Vector()
idvec = h.Vector()
#int(tstop/h.dt)*len_assigned_gids

pc.spike_record(-1, tvec, idvec)   # -1 for all gids in that node
pc.set_maxstep(10) 
pc.psolve(tstop)
pc.barrier() 

###############################################################################
# Storing all the spikes as a csv file  (could ne also stored as sqlite3)
###############################################################################

fname = f"spk_{trial}.csv"

if rank == 0:
    f = open(fname, "w")
    f.close()
    
for r in range(nhost):
    if r == rank:
        f = open(fname, "a")
        for i in range(len(tvec)):
            f.write("%g,%d,%d\n" % (tvec.x[i], int(idvec.x[i]),rank))
        f.close()
    pc.barrier() 
    
###############################################################################
# Storing the membrane potential of the soma 
###############################################################################

fname = f"mem_pot_{trial}.csv"



 
