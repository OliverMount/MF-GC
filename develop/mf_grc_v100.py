# coding only for mf_grc network
# run this code from 
import os
import sys   
import glob
import time
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

timing_log = {}
start_time = time.time()  # All timings are with reference to this file

#db_file='/mnt/data/mf_grc_scaled.db'    # This is bind with the container
#db_file='../connectivity/mf_grc.db'    # Database file
#db_file='../connectivity/mf_grc_scaled_80.db'
db_file='../connectivity/mf_grc_scaled_280.db'
db_mode='file:'+db_file+'?mode=ro&immutable=1'
db_rank_folder='../connectivity/'
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

def mark_time(label, rank):
    now = time.time()
    elapsed = now - start_time
    timing_log[label] = elapsed
    print(f"[Rank{rank} : Timing] {label} @ +{elapsed:.3f} sec")

def print_timings(rank):
    print(f"\n--- Timing Summary for Rank {rank} ---")
    keys = list(timing_log.keys())
    for i in range(1, len(keys)):
        duration = timing_log[keys[i]] - timing_log[keys[i - 1]]
        print(f"[Rank{rank} : Timing] {keys[i - 1]} -->  {keys[i]}: {duration:.3f} sec")
    print("--------------------------------------\n")

###################################################################### 
# 1. Start the parallel context
######################################################################  

h.nrnmpi_init()
pc = h.ParallelContext()
rank = int(pc.id())
nhost = int(pc.nhost())
#pc.nhost() → total number of MPI ranks (ranks across all nodes) aware of this during slurm
hostname = socket.gethostname()   # this would be used later

print(f"[Rank{rank}] : db_file_used {db_file}",end='\n')
mark_time("START", rank)   # Time marker for beginning
print()
 
## Populate the workers table
hostnames = pc.py_allgather(hostname)  # Collective MPI
pc.barrier()
#print(hostnames)
#print(type(hostnames))

unique_hostnames = sorted(set(hostnames))
host_id_map = {name: idx for idx, name in enumerate(unique_hostnames)}
worker_host_id = host_id_map[hostname]
 
# Solver efficiency settings
#h.cvode.cache_efficient(1)
#h.CVode().use_fast_imem(1)  

# Weights for synaptic weight change 
wE = 5e-3  # pS
#wI = 1e-3  # pS
  
###################################################################### 
# 2. Clear, update  worker table (only rank 0) 
######################################################################
#print(db_file)

# Open DB in read-only immutable mode for non-zero ranks:
if rank == 0:
    mark_time("Update worker table", rank)
    print()
    with sqlite3.connect(db_file) as conn:
        cursor = conn.cursor()
        
        print_status('Clearing worker table at rank 0 for every new run')
        cursor.execute("DELETE FROM worker") 
        conn.commit()
         
        for name, wid in host_id_map.items():
            cursor.execute(
                "INSERT INTO worker (worker_host_id, worker_name) VALUES (?, ?)",
                (wid, name)
            )
        conn.commit()    
        
        #print_status('Clearing rank* database for every new run')
        # clear all the tables if it exists; they are not cleared by the rank0 at the end
        # for the test purpose we keep them up
        os.system('rm '+rank_database_path + '/rank*')  # clear all rank databases

        # If there are more ranks; create seperate synapse table for each rank at rank0 
        # This creates template in each rank
        if nhost > 1:
            for ran in range(nhost):
                print_status(f'[Rank{rank}] : Creating extra synapse table headers for ==> Rank{ran}')
                db_temp_path = os.path.join(db_rank_folder, f'synapse_rank{ran}.db')
                if os.path.exists(db_temp_path):
                    os.remove(db_temp_path)  # Delete existing file
                         
                with sqlite3.connect(db_temp_path) as temp_conn:
                    temp_cursor = temp_conn.cursor()
                    
                    temp_cursor.execute('''
                    CREATE TABLE synapse (
                        source_gid INTEGER NOT NULL,
                        target_gid INTEGER NOT NULL,
                        weight INTEGER NOT NULL DEFAULT 1,   -- for binary it is 1
                        delay REAL NOT NULL DEFAULT 0,    
                        syn_type INTEGER NOT NULL DEFAULT 1,    -- 1 for excitatory  0 for inhibitory synapse
                        syn_dynamics INTEGER NOT NULL DEFAULT 0,  -- 0: linear, 1: nonlinear
                        target_dend_id INTEGER NOT NULL,
                        dend_syn_loc REAL NOT NULL DEFAULT 0.5,   -- default at the center 
                        need_single_syn INTEGER NOT NULL CHECK (need_single_syn IN (0, 1)) DEFAULT 1,  -- Boolean
                        syn_id INTEGER NOT NULL DEFAULT 0,          -- identifies the synaptic location within the cell
                        PRIMARY KEY (source_gid, target_gid) 
                    )
                    ''')
        else:
            print_status(f'[Rank{rank}] : No extra synapse database as nhost is {nhost}')
        
        cursor.close()
    
pc.barrier() 
###################################################################### 
# 4. Load GIDs from database/round-robin and update mpi-rank
###################################################################### 
mark_time("GID assignment",rank)
print()
with sqlite3.connect(db_mode, uri=True) as conn:
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
    print(f"[rank{rank}] : No. of cells assigned ==> {len_assigned_gids}")  # Working properly
    
    # Each rank gets its own synapse data from the big synapse database
    # The functions prevents query size overflow  
    #def fetch_synapses_in_chunks(cursor, assigned_gids, chunk_size=10000):
    #    all_synapses = []
    #    for i in range(0, len(assigned_gids), chunk_size):
    #        chunk = assigned_gids[i:i+chunk_size]
    #        placeholders = ','.join('?' for _ in chunk)
    #        query = f"SELECT * FROM synapse WHERE target_gid IN ({placeholders}) ORDER BY target_gid ASC,source_gid ASC"
    #        cursor.execute(query, chunk)
    #        all_synapses.extend(cursor.fetchall())
    #    return all_synapses
    
    #synapses = fetch_synapses_in_chunks(cursor, assigned_gids)
    if nhost > 1:
        def fetch_synapses_in_chunks_method(cursor, assigned_gids, chunk_size=10000):
            all_synapses = []
            for i in range(0, len(assigned_gids), chunk_size):
                chunk = assigned_gids[i:i+chunk_size]
                placeholders = ','.join('?' for _ in chunk)
                query = f"SELECT * FROM synapse WHERE target_gid IN ({placeholders}) ORDER BY target_gid ASC,source_gid ASC"
                cursor.execute(query, chunk)
                all_synapses.extend(cursor.fetchall())
            return all_synapses

        synapses = fetch_synapses_in_chunks_method(cursor, assigned_gids)
        #placeholders = ','.join('?' for _ in assigned_gids)
        #query = f"SELECT * FROM synapse WHERE target_gid IN ({placeholders}) ORDER BY target_gid ASC,source_gid ASC"
        #cursor.execute(query, assigned_gids)
        #synapses = cursor.fetchall()  # Rankwise syanapse
    
    cursor.close()

# push synapse data onto individial database if the nhost is not 1
if nhost>1:
    mark_time("Synapse data per rank",rank)
    print()
    with sqlite3.connect(os.path.join(db_rank_folder, f'synapse_rank{rank}.db')) as conn:
        cursor = conn.cursor()
        print_status(f"[Rank{rank}] : DB path is "+os.path.join(db_rank_folder, f'synapse_rank{rank}.db'))
        # copy synapse data into it each rank
        cursor.executemany("INSERT INTO synapse VALUES (?,?,?,?,?,?,?,?,?,?)",synapses) 
        conn.commit() 
        cursor.close()


# Each rank makes a new database and stores the gid seperately. 
# and rank 0 finally reads all of them and updates on the database and deleted the table
# not decided the second line
db_rank_name=  f"{rank_database_path}/rank{rank}.db"  # This is for mpi_rank_id and worker_host_id
with sqlite3.connect(db_rank_name) as conn:
    cursor = conn.cursor()

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
###############################################################################
#  Create units for each cell_type in a particular node (rank)
###############################################################################
# this is for updating the syn_id (which is each target id what is the number of synapses) 
if nhost>1:
    db_syn_file=os.path.join(db_rank_folder, f'synapse_rank{rank}.db') 
else:
    db_syn_file=db_file
    print_status('Only one rank. Thus reading from orignal file')

# following cells needs to create synapses
synapse_needed_cells=['grc']

cells = {}
nclist = []
grc_count=0
mf_count=0
 
mark_time("Creating cells ",rank)
print()

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
 
    cells[gid]=cell  # This is very much needed for establishing synaptic connections

    # Register this cell in the parallel context
    pc.set_gid2node(cell.gid, rank)  # register the cell (gid) to mpi
    nc = cell.connect2target(None)  # connect the spikes to the target cell, no target just spike detector
    pc.cell(cell.gid, nc)     #this gid is associated; very improtant for spike recording of gids as well; prerequisite for gid_connect()

    if cell_type in synapse_needed_cells:
        cell.determine_synaptic_connections(db_syn_file)   # this will update the syn_id

pc.barrier() # # Synchronize all processes; ensures all cells are created before continuing

###############################################################################
#  Establish synaptic links based on the each cell created synapses
###############################################################################
mark_time("Establish synaptic links",rank)
print()
with sqlite3.connect(db_syn_file) as conn:
    cursor = conn.cursor()

    if gid_tuple:     # To make sure at least one element for the IN clause

        # Each rank gets its own synapse data from the big synapse database
        # The functions prevents query size overflow  
        def fetch_synapses_in_chunks(cursor, assigned_gids, chunk_size=10000):
            all_synapses = []
            for i in range(0, len(assigned_gids), chunk_size):
                chunk = assigned_gids[i:i+chunk_size]
                placeholders = ','.join('?' for _ in chunk)
                query = f"SELECT source_gid, target_gid,weight, delay, syn_id FROM synapse WHERE target_gid IN ({placeholders})"
                cursor.execute(query, chunk)
                all_synapses.extend(cursor.fetchall())
            return all_synapses

        synapses = fetch_synapses_in_chunks(cursor, assigned_gids)

        #placeholder = ','.join('?' for _ in gid_tuple)
        # Query and order by source and target gid
        #query = f"SELECT source_gid, target_gid,weight, delay, syn_id FROM synapse WHERE target_gid IN ({placeholder})"
        #cursor.execute(query, gid_tuple)
        #synapses = cursor.fetchall()  # list of tuples
        
        for source_gid, target_gid,weight, delay, syn_id in synapses:   
            nc = pc.gid_connect(source_gid, cells[target_gid].synapses[syn_id])
            nc.delay = max(delay,0.2)  # this is crucial 0 delay may affect the synapse
            nc.weight[0] = weight*wE  # For efficient storage all weight are 1 in the database
    
            nclist.append(nc)
cursor.close()
         
# Close the db finally 
  
###############################################################################
# Run the simulation
###############################################################################
h.celsius = 34
h.dt = 0.025
h.finitialize()

mark_time("Solving Equations", rank)
print()

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
mark_time("Saving the spikes ",rank)
print()
rank_file = f"{results_path}/spikes_rank{rank}.csv"
with open(rank_file, "w") as f:
    for i in range(len(tvec)):
        f.write(f"{tvec[i]},{int(idvec[i])},{rank}\n")

pc.barrier()  # Wait until all ranks are done writing

###############################################################################
# Merge all spike files into a single CSV on rank 0 and clean up
###############################################################################
if rank == 0:
    mark_time("Merging spikes from all the rank",rank)
    print()
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
 
#if rank == 0:
#    with sqlite3.connect(db_mode, uri=True) as conn:
#        cursor = conn.cursor()
#        cursor.execute("SELECT gid,cell_type FROM cell")
#        rows = cursor.fetchall()
         
         
#    mf_gids = [gid for gid, cell_type in rows if cell_type == 'mf']
#    grc_gids = [gid for gid, cell_type in rows if cell_type == 'grc']
    
    # Load spikes CSV
#    df = pd.read_csv(merged_file, header=None, names=['time', 'gid', 'rank'])
    
    
    # Reindex GIDs to start from 0 and go up
#    def reindex_gids(df, gid_list):
#        gid_map = {gid: i for i, gid in enumerate(sorted(gid_list))}
#        return df.assign(gid=df['gid'].map(gid_map))
    
     
    # Split data
#    mf_spikes = df[df['gid'].isin(mf_gids)]
#    grc_spikes = df[df['gid'].isin(grc_gids)] 
    
    # Apply reindexing
#    mf_spikes_reindexed = reindex_gids(mf_spikes, mf_gids)
#    grc_spikes_reindexed = reindex_gids(grc_spikes, grc_gids)
    
    # Function to make raster plot
#    def plot_raster(spikes_df, title, filename,col):
#        plt.figure(figsize=(12, 6))
#        plt.scatter(spikes_df['time'],  spikes_df['gid'] ,  color=col,s=1)
#        plt.xlabel("Time (ms)")
#        plt.ylabel("GID")
#        plt.title(title)
#        plt.tight_layout()
#        plt.savefig(filename, dpi=300)
#        plt.close()
#        print(f"Saved: {filename}")
        
         
    # Plot and save
#    plot_raster(mf_spikes_reindexed, "Mossy Fiber Raster Plot", f"{results_path}/mf_raster.png",col="red")
#    plot_raster(grc_spikes_reindexed, "Granule Cell Raster Plot", f"{results_path}/grc_raster.png",col="green")

  

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

# Print the timing summary for a rank in the out file
print_timings(rank)

# Properly finalize MPI before exiting
h.quit()
