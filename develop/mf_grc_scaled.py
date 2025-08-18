# coding only for mf_grc network
# run this code from 
import os
import sys   
import glob
sys.path.append('../utils/')
sys.path.append('../develop/')

from args import  *      # get all CLI arguments 
from methods import print_status

from Cell import MossyFiber, GranuleCell
import sqlite3         # For getting the connectivity, gid, 
from neuron import h, coreneuron, load_mechanisms 
#import random 
import socket  # To get node_names and update in the datanase (not necessary though)
import pandas as pd
import matplotlib.pyplot as plt

#load_mechanisms('src/x86_64/')
 
#load_mechanisms('/media/oli/Research/Gitrepo/GranularLayerModel/src/')


db_file='../connectivity/mf_grc.db'    # Database file
db_mode='file:'+db_file+'?mode=ro&immutable=1' 
rank_database_path='../run_databases'  # Do not put ending back slashes
results_path="../results"

# Random seed for reproducibility
seed_value=42 
#mf_rate=50


# parse the CLI arguments
args, unknown = parser.parse_known_args()    # parser namespace is from args
run_type = args.runtype    #["coreneuron_gpu", "coreneuron_cpu",  "neuron_serial", "neuron_parallel"]
tstop=args.tstop
trial=args.trial   
 
#h.cvode.cache_efficient(1)

# Setting for coreneuron (cpu or gpu) 
# Please add additional coreneuron options for efficiency
if run_type in {"coreneuron_gpu", "coreneuron_cpu"}:
    from neuron import coreneuron
    coreneuron.enable = True  
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
#h.cvode.cache_efficient(1)
#h.CVode().use_fast_imem(1)  

# Weights for synaptic weight change 
wE = 5e-3  # pS
#wI = 1e-3  # pS
  
###################################################################### 
# 2. Clear worker table (only rank 0) 
######################################################################
 
# Open DB in read-only immutable mode for non-zero ranks:
if rank == 0:
    conn = sqlite3.connect(db_file)
else:
    conn = sqlite3.connect(db_mode, uri=True) 
cursor = conn.cursor()  

if rank == 0:  # only on the rank 0  clear the worker table as it has to be updated

    print_status('Clearing worker table at rank 0 for every new run')
    cursor.execute("DELETE FROM worker") 
    conn.commit()
    
    print_status('Clearing rank* databased for every new run')
    # clear all the tables if it exists; they are not cleared by the rank0 at the end
    # for the test purpose we keep them up
    os.system('rm '+rank_database_path + '/rank*')  # clear all rank databases
      
 
###################################################################### 
#  3. Hostname → worker_host_id mapping (collectively across all ranks) 
###################################################################### 
## Populate the workers table
hostnames = pc.py_allgather(hostname)  # Collective MPI
pc.barrier()
#print(hostnames)
#print(type(hostnames))

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

    cursor.close()
    conn.close()  
 
###################################################################### 
# 4. Load GIDs from database/round-robin and update mpi-rank
###################################################################### 

conn = sqlite3.connect(db_mode, uri=True)
cursor = conn.cursor() 
cursor.execute("SELECT gid,cell_type FROM cell")
gids, cell_types = zip(*cursor.fetchall())
 
#  Assign gids via round robin
# this assignment would be changed later based on cell location? 
assigned_gids, assigned_cell_types = zip(
    *[(gid, cell_type) for gid, cell_type in zip(gids, cell_types) if gid % nhost == rank]
) if gids else ([], [])

gid_tuple = tuple(assigned_gids)    # tuple of the list
len_assigned_gids=len(assigned_gids)
#print(assigned_gids)  # Working properly 
cursor.close()
conn.close()

# Each rank makes a new database and stores the gid seperately. 
# and rank 0 finally reads all of them and updates on the database and deleted the table
# not decided the second line

db_rank_name=  f"{rank_database_path}/rank{rank}.db"
conn = sqlite3.connect(db_rank_name)
cursor=conn.cursor() 

strn = f'''
    CREATE TABLE IF NOT EXISTS gid_run_info (
        gid INTEGER PRIMARY KEY,
        mpi_rank_id INTEGER NOT NULL,
        worker_host_id INTEGER NOT NULL 
    )
    ''' 
cursor.execute(strn)  # create a new database
 
# push the gid info into the database
for g in assigned_gids:
    cursor.execute(
        "INSERT INTO gid_run_info (gid, mpi_rank_id,worker_host_id) VALUES (?, ?,?)",
        (g, rank,worker_host_id )
    )
conn.commit()  
cursor.close()
conn.close() 
 
###############################################################################
#  Create units for each cell_type in a particular node (rank)
###############################################################################
 
# following cells needs to create synapses
synapse_needed_cells=['grc']


cells = {}
nclist = []
grc_count=0
mf_count=0
 
for cell_type,gid in zip(assigned_cell_types,assigned_gids): 
    cell_seed = seed_value + gid  
    
    if cell_type == "mf": 
        cell = MossyFiber(gid,seed=cell_seed,mode='random',tstop=tstop, rate_needed=5)     # From Cell.py
        mf_count+=1
    elif cell_type == "grc": 
        cell = GranuleCell(gid,seed=cell_seed, tstop=tstop) 
        grc_count+=1
    else:
        raise ValueError(f"Unknown cell type: {cell_type} for MF-GRC network")
 
    cells[gid]=cell  # why is this needed?

    # Register this cell in the parallel context
    pc.set_gid2node(cell.gid, rank)  # register the cell (gid) to mpi
    nc = cell.connect2target(None)  # connect the spikes to the target cell, no target just spike detector
    pc.cell(cell.gid, nc)     #this gid is associated; very improtant for spike recording of gids as well; prerequisite for gid_connect()

    if cell_type in synapse_needed_cells: 
        # Make a list of synaptic objects here given the db
        cell.determine_synaptic_connections(db_file)   # this will update the syn_id

pc.barrier() # # Synchronize all processes; ensures all cells are created before continuing

###############################################################################
#  Establish synaptic links based on the each cell created synapses
###############################################################################
  
conn = sqlite3.connect(db_mode, uri=True)
cursor = conn.cursor() 

if gid_tuple:     # To make sure at least one element for the IN clause
    placeholder = ','.join('?' for _ in gid_tuple)
    # Query and order by source and target gid
    query = f"SELECT source_gid, target_gid,weight, delay, syn_id FROM synapse WHERE target_gid IN ({placeholder})"
    cursor.execute(query, gid_tuple)
    synapses = cursor.fetchall()  # list of tuples
    
    for source_gid, target_gid,weight, delay, syn_id in synapses:   
        nc = pc.gid_connect(source_gid, cells[target_gid].synapses[syn_id])
        nc.delay = max(delay,0.2)  # this is crucial 0 delay may affect the synapse
        nc.weight[0] = weight*wE  # For efficient storage all weight are 1 in the database

        nclist.append(nc)
    
# Close the db finally 
cursor.close()
conn.close()
  
###############################################################################
# Run the simulation
###############################################################################
h.celsius = 34
h.dt = 0.025
h.finitialize()


# the following vectors are dynamically resized as the spike occurs
tvec = h.Vector()
idvec = h.Vector() 
#int(tstop/h.dt)*len_assigned_gids

pc.spike_record(-1, tvec, idvec)   # -1 for all gids in that node
pc.set_maxstep(10) 
pc.psolve(tstop)
pc.barrier() 

 
###############################################################################
# Storing all the spikes as a csv file  (could ne also stored as sqlite3)  saving per rank
###############################################################################
 
# Create results directory if not exists
if rank == 0 and not os.path.exists(results_path):
    os.makedirs(results_path)
pc.barrier()  # Ensure directory exists before others write

# Save spikes recorded by this rank
rank_file = f"{results_path}/spikes_rank{rank}.csv"
with open(rank_file, "w") as f:
    for i in range(len(tvec)):
        f.write(f"{tvec[i]},{int(idvec[i])},{rank}\n")

pc.barrier()  # Wait until all ranks are done writing

###############################################################################
# Merge all spike files into a single CSV on rank 0 and clean up
###############################################################################
if rank == 0:
    merged_file = f"{results_path}/all_spikes.csv"
    with open(merged_file, "w") as outfile:
        for rf in sorted(glob.glob(f"{results_path}/spikes_rank*.csv")):
            with open(rf, "r") as infile:
                outfile.writelines(infile)

    # Delete per-rank spike files
    for rf in glob.glob(f"{results_path}/spikes_rank*.csv"):
        os.remove(rf)

    print(f"[Rank 0] Merged spikes into {merged_file} and deleted per-rank files.")
    print(f"[Rank 0] No. of mf: {mf_count}; No. of grc: {grc_count}")
  
pc.barrier() 
pc.done()


###############################################################################
# Plotting the results
###############################################################################
 
if rank == 0:
    conn = sqlite3.connect(db_mode, uri=True)
    cursor = conn.cursor() 
    cursor.execute("SELECT gid,cell_type FROM cell")
    rows = cursor.fetchall()
     
     
    mf_gids = [gid for gid, cell_type in rows if cell_type == 'mf']
    grc_gids = [gid for gid, cell_type in rows if cell_type == 'grc']
    
    # Load spikes CSV
    df = pd.read_csv(merged_file, header=None, names=['time', 'gid', 'rank'])
    
    
    # Reindex GIDs to start from 0 and go up
    def reindex_gids(df, gid_list):
        gid_map = {gid: i for i, gid in enumerate(sorted(gid_list))}
        return df.assign(gid=df['gid'].map(gid_map))
    
     
    # Split data
    mf_spikes = df[df['gid'].isin(mf_gids)]
    grc_spikes = df[df['gid'].isin(grc_gids)] 
    
    # Apply reindexing
    mf_spikes_reindexed = reindex_gids(mf_spikes, mf_gids)
    grc_spikes_reindexed = reindex_gids(grc_spikes, grc_gids)
    
    # Function to make raster plot
    def plot_raster(spikes_df, title, filename,col):
        plt.figure(figsize=(12, 6))
        plt.scatter(spikes_df['time'],  spikes_df['gid'] ,  color=col,s=1)
        plt.xlabel("Time (ms)")
        plt.ylabel("GID")
        plt.title(title)
        plt.tight_layout()
        plt.savefig(filename, dpi=300)
        plt.close()
        print(f"Saved: {filename}")
        
         
    # Plot and save
    plot_raster(mf_spikes_reindexed, "Mossy Fiber Raster Plot", f"{results_path}/mf_raster.png",col="red")
    plot_raster(grc_spikes_reindexed, "Granule Cell Raster Plot", f"{results_path}/grc_raster.png",col="green")
     
    cursor.close()
    conn.close() 
    
     

# Function to make raster plot
def plot_raster(spikes_df, title, filename, col):
    plt.figure(figsize=(12, 6))
    plt.scatter(spikes_df['time'], spikes_df['gid'], color=col, s=1)
    plt.xlabel("Time (ms)")
    plt.ylabel("Neuron index (reindexed)")
    plt.title(title)
    plt.tight_layout()
    plt.savefig(filename, dpi=300)
    plt.close()
    print(f"Saved: {filename}")


#No. of mf: 1070; No. of grc: 3925
###############################################################################
# Storing the membrane potential of the soma 
###############################################################################

#fname = f"mem_pot_{trial}.csv" 

# n_trials = 1000

# for trial in range(n_trials):
#     print(f"[Rank {rank}] Running trial {trial}")

#     h.finitialize()
#     tvec = h.Vector()
#     idvec = h.Vector()
#     pc.spike_record(-1, tvec, idvec)

#     pc.psolve(tstop)
#     pc.barrier()

#     # Save per-rank spike file
#     trial_file = f"results/spikes_rank{rank}_trial{trial}.csv"
#     with open(trial_file, "w") as f:
#         for i in range(len(tvec)):
#             f.write(f"{tvec[i]},{int(idvec[i])},{rank}\n")

#     pc.barrier()

#     # Merge on rank 0
#     if rank == 1:
#         merged_file = f"results/all_spikes_trial{trial}.csv"
#         with open(merged_file, "w") as outfile:
#             for rf in sorted(glob.glob(f"results/spikes_rank*_trial{trial}.csv")):
#                 with open(rf, "r") as infile:
#                     outfile.writelines(infile)

#         # Delete per-rank files
#         for rf in glob.glob(f"results/spikes_rank*_trial{trial}.csv"):
#             os.remove(rf)

#         print(f"[Rank 0] Trial {trial}: Merged and cleaned up.")

# Properly finalize MPI before exiting
h.quit()

