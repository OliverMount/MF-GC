# Version 2 based on connectivity based GID assignment

###################################################################### 
# 0. Import modules, Set paths, CLI and a few methods
######################################################################  

import os
import sys   
import glob
import time
sys.path.append('../utils/')
sys.path.append('../develop/')

from args import  *      # get all CLI arguments 
from methods import print_status, print_timings,elapsed_time_string, equalize_sublists, remove_overlapping_gids, silently_remove_files, make_synapse_dict

from neuron import h  #, load_mechanisms 
from CellDevelop import MossyFiber, GranuleCell
import sqlite3         # For getting the connectivity, gid, 

#import random 
import socket  # To get node_names and update in the datanase (not necessary though)
import pandas as pd
import matplotlib.pyplot as plt
 
timing_log = {}
start_time = time.time()  # All timings are with reference to this file

#db_file='/mnt/data/mf_grc_scaled.db'    # This is bind with the container
#db_file='../connectivity/mf_grc.db'    # Database file
db_file='../connectivity/mf_grc_scaled_80.db'
#db_file='../connectivity/mf_grc_scaled_280.db'  # 1M GRC 
#db_mode='file:'+db_file+'?mode=ro&immutable=1' 

connectivity_folder='../connectivity/'
rank_database_path='../run_databases'  # Do not put ending back slashes
results_path="../results"


# Random seed for reproducibility
seed_value=42 
#mf_rate=50 

# parse the CLI arguments
args, unknown = parser.parse_known_args()    # parser namespace is from args (if possible make it explicit)
run_type = args.runtype    #["coreneuron_gpu", "coreneuron_cpu",  "neuron_serial", "neuron_parallel"]
tstop=args.tstop
trial=args.trial   
fig_needed=args.fig_needed 
#h.cvode.cache_efficient(1)

# Setting for coreneuron (cpu or gpu) 
# Please add additional coreneuron options for efficiency
if run_type in {"coreneuron_gpu", "coreneuron_cpu"}:
    from neuron import coreneuron
    coreneuron.enable = True  
    if run_type == "coreneuron_gpu":
        coreneuron.gpu = True 
        
 
# Keep this function here        
def mark_time(label, rank):
    now = time.time() 
    elapsed = now - start_time
    timing_log[label] = elapsed
    print(f"[Rank {rank} : Timing] {label} ==> +{elapsed:.3f} sec ("+elapsed_time_string(start_time, now)+ ") ... OK! \n") 
     
    
def fetch_in_batches(cursor,assigned_gids,kind='synapse',chunk_size=10000):
    res = [] 
        
    for i in range(0, len(assigned_gids), chunk_size):
        chunk = assigned_gids[i:i+chunk_size]
        placeholders = ','.join('?' for _ in chunk)  
        
        if kind=='synapse': 
            query = f"SELECT * FROM synapse WHERE target_gid IN ({placeholders}) ORDER BY target_gid ASC,source_gid ASC"
            cursor.execute(query, chunk)
            #res.extend(cursor.fetchall())
            res.extend(list(row) for row in cursor.fetchall())
        elif kind=="cell_type":
            query = f"SELECT cell_type FROM cell WHERE gid IN ({placeholders})"
            cursor.execute(query, chunk)
            res.extend([row[0] for row in cursor.fetchall()])
        else:
            raise ValueError(f'{kind} not a proper fetch_in_batches argument')  
          
    return res
     
###################################################################### 
# 1. Start the parallel context
######################################################################  

h.nrnmpi_init()
pc = h.ParallelContext()
rank = int(pc.id())
nhost = int(pc.nhost())
#pc.nhost() → total number of MPI ranks (ranks across all nodes) aware of this during slurm
hostname = socket.gethostname()   # this would be used later

print(f"[Rank {rank} : Database file] ==> {db_file} \n")
mark_time("START", rank)   # Time marker for beginning
 
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
# 2. Connectivivity based GID assignement at rank 0, update worker table
######################################################################
#print(db_file)
 
# Open DB in read-only immutable mode for non-zero ranks:
if rank == 0:  
    with sqlite3.connect(db_file) as conn:
        cursor = conn.cursor()
        
        ###############################
        # Clear the previous worker table  
        ###############################  
        print_status(f'Clearing worker table at rank {rank} for every new run \n')
        cursor.execute("DELETE FROM worker") 
        conn.commit()  
        ###############################
        # Update the new worker table  
        ###############################  
        for name, wid in host_id_map.items():
            cursor.execute(
                "INSERT INTO worker (worker_host_id, worker_name) VALUES (?, ?)",
                (wid, name)
            )
        conn.commit()     
        ###############################
        # Test purpose; gid  rank worker_host_id databases   
        ###############################  
        #print_status('Clearing rank* database for every new run')
        # clear all the tables if it exists; they are not cleared by the rank0 at the end
        # for the test purpose we keep them up
        
        silently_remove_files(rank_database_path,'mpi*','gid*','synapse*')
         
        mark_time("GID assignment",rank) 
        fname=os.path.join(rank_database_path, 'gid.db')  # this is the database of gids
        
        if nhost > 1:  
           
            ###############################
            # Read all synapse table once 
            ############################### 
            print(f'[Rank{rank} : Reading] : Reading synapse table for gid assignment \n')
            
            
            query = "SELECT COUNT(*) FROM synapse;"
            cursor.execute(query)
            nrows=cursor.fetchone()[0] 
            rows_per_rank = (nrows + nhost - 1) // nhost
            
            # Split the synapse (connectivity) table in to nhost parts
            syn=[]   # list of length nhost
            for i in range(nhost):
                offset = i * rows_per_rank 
                cursor.execute(f""" 
                    SELECT source_gid,target_gid  FROM synapse
                    ORDER BY target_gid ASC, source_gid ASC 
                    LIMIT {rows_per_rank} OFFSET {offset}
                """)
                synapses=cursor.fetchall()
                #print(synapses[0:10])
                #print(synapses[-10:-1])
                #print(len(synapses))
                syn.append(list({num for tup in synapses for num in tup}))  # This is where combining all source and target in to one
            
            
            cursor.close()
            
            # Remove overlappping elements    
            gids_cleaned_for_overlapping=remove_overlapping_gids(syn)   
            
            # Equalize the GID assignment (not very optimal though)   
            equalized_gids=equalize_sublists(gids_cleaned_for_overlapping)    
            
            # Push gids per rank to into the gid database
            with sqlite3.connect(fname) as gid_conn: 
                gid_cursor = gid_conn.cursor()   
                for idx,rank_gids in enumerate(equalized_gids):  
                    table_name = f'rank{idx}'
                    gid_cursor.execute(f''' CREATE TABLE IF NOT EXISTS {table_name} ( gids INTEGER NOT NULL, PRIMARY KEY (gids) ) ''')
                    gids = [(value,) for value in rank_gids]
                    gid_cursor.executemany( f'INSERT INTO {table_name} (gids) VALUES (?)', gids)
                gid_conn.commit()
                gid_cursor.close() 
            
        else:
            print_status(f'[Rank{rank}] : No extra synapse database as nhost is {nhost}') 
            # Store the gids here for rank 0
            with sqlite3.connect(fname) as gid_conn: 
                gid_cursor = gid_conn.cursor()   
                table_name = f'rank{rank}'
                gid_cursor.execute(f''' CREATE TABLE IF NOT EXISTS {table_name} ( gids INTEGER NOT NULL, PRIMARY KEY (gids) ) ''')
                 
                cursor.execute("SELECT gid FROM cell")  # previous cursor
                data =  [(row[0],) for row in cursor.fetchall()]
                 
                gid_cursor.executemany( f'INSERT INTO {table_name} (gids) VALUES (?)', data)
                gid_conn.commit()
                gid_cursor.close()  
                
    ###############################
    # Synapse headers common for all
    ###############################  
    mark_time("Synapse headers",rank) 
    
    for ran in range(nhost):
        print(f'[Rank{rank} : Table headers ] : Creating extra synapse table headers for ==> Rank{ran}')
        db_temp_path = os.path.join(rank_database_path, f'synapse_rank{ran}.db')
        if os.path.exists(db_temp_path):
            os.remove(db_temp_path)  # Delete existing file
                 
        with sqlite3.connect(db_temp_path) as temp_conn:
            temp_cursor = temp_conn.cursor() 
            temp_cursor.execute('''CREATE TABLE synapse (
                    source_gid INTEGER NOT NULL,
                    target_gid INTEGER NOT NULL,
                    weight INTEGER NOT NULL DEFAULT 1,
                    delay REAL NOT NULL DEFAULT 0,
                    syn_type INTEGER NOT NULL DEFAULT 1,
                    syn_dynamics INTEGER NOT NULL DEFAULT 0,
                    target_seg_id INTEGER NOT NULL,
                    dend_syn_loc REAL NOT NULL DEFAULT 0.5,
                    need_single_syn INTEGER NOT NULL CHECK (need_single_syn IN (0, 1)) DEFAULT 1,
                    syn_id INTEGER NOT NULL DEFAULT 0,
                    PRIMARY KEY (source_gid, target_gid) )''')   
            temp_cursor.close() 
               
pc.barrier()   # Other wait until rank0 is done with 
###################################################################### 
# 4. Load GIDs from database 
###################################################################### 
mark_time("Split db storing",rank)  
 
table_name = f'rank{rank}'
gid_db=os.path.join(rank_database_path, 'gid.db') 
 
with sqlite3.connect(gid_db, uri=True) as conn:
    cursor = conn.cursor()   
    cursor.execute(f'SELECT gids from {table_name}')
    assigned_gids=[row[0] for row in cursor.fetchall()]
    cursor.close()
    
gid_tuple= tuple(assigned_gids) 
len_assigned_gids=len(gid_tuple)
print(f"[Rank{rank}] : No. of cells assigned ==> {len_assigned_gids}")  # Working properly 
  
# collect cell type for that gid    
with sqlite3.connect(db_file, uri=True) as conn:
    cursor = conn.cursor()   
    assigned_cell_types=fetch_in_batches(cursor,assigned_gids,kind="cell_type")
    synapses = fetch_in_batches(cursor, assigned_gids,kind='synapse') 
    
    synapses_table_name='synapse'
    cursor.execute(f"PRAGMA table_info({synapses_table_name})")
    synapse_table_column_names=[names[1] for names in cursor.fetchall()]
    syn_id_idx=synapse_table_column_names.index('syn_id')
    
    cursor.close() 
   
# Make synapse as a dict  with target as key
synapses_dict=make_synapse_dict(synapses)

# Synapse table based on the gid
synapse_db=os.path.join(rank_database_path, f'synapse_rank{rank}.db') 
with sqlite3.connect(synapse_db) as conn:
    cursor = conn.cursor() 
    # copy synapse data into it each rank
    cursor.executemany("INSERT INTO synapse VALUES (?,?,?,?,?,?,?,?,?,?)",synapses) 
    conn.commit() 
    cursor.close() 
    print(f'[Rank{rank}] : DB path is {synapse_db} ...OK!')

# Each rank makes a new database and stores the gid seperately. 
# and rank 0 finally reads all of them and updates on the database and deleted the table
# not decided the second line

mpi_rank_db=  f"{rank_database_path}/mpi_rank{rank}.db"  # This is for mpi_rank_id and worker_host_id
with sqlite3.connect(mpi_rank_db) as conn:
    cursor = conn.cursor() 
    strn = '''CREATE TABLE IF NOT EXISTS gid_run_info ( 
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
#  Create cells for each cell_type in a particular node (rank)
###############################################################################
 
  
# following cells needs to create synapses
synapse_needed_cells=['grc'] 

cells = {}
nclist = []
grc_count=0
mf_count=0
 
mark_time("Creating cells ",rank) 

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

    # feeding here the target gids synapse construction
    if cell_type in synapse_needed_cells:
        # Assuming synapses_dict is not empty
        syn_id=cell.determine_synaptic_connections(synapses_dict[gid],synapse_table_column_names)   # this will update the cell.synapses list
        
        # Update the syn id in the synaptic dictionary  in place
        for syn, syn_id_val in zip(synapses_dict[gid], syn_id):
            syn[syn_id_idx] = syn_id_val  # Replace in place
         
        
pc.barrier() # # Synchronize all processes; ensures all cells are created before continuing

###############################################################################
#  Establish synaptic links based on the each cell created synapses
###############################################################################
mark_time("Establish synaptic links",rank) 

target_gids=synapses_dict.keys()  # list of target gids in that rank
   
for tgt_gid in target_gids:  
    syn=synapses_dict[tgt_gid]   # rows of the synapse information for that target gid
    for source_gid, target_gid,weight,delay,_,_,_,_,_,syn_id in syn:  # target_gid and tgt_gid must be the same
        nc = pc.gid_connect(source_gid, cells[target_gid].synapses[syn_id])  
        nc.delay = max(delay,0.2)  # this is crucial 0 delay may affect the synapse
        nc.weight[0] = weight*wE  # For efficient storage all weight are 1 in the database
         
        nclist.append(nc)
         
pc.barrier()  
  
###############################################################################
# Run the simulation
###############################################################################
h.celsius = 37
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
  
    
    if fig_needed:
    
        with sqlite3.connect(db_file, uri=True) as conn:
            cursor = conn.cursor()
            cursor.execute("SELECT gid,cell_type FROM cell")
            rows = cursor.fetchall()
              
              
        mf_gids = [gid for gid, cell_type in rows if cell_type == 'mf']
        grc_gids = [gid for gid, cell_type in rows if cell_type == 'grc']
         
        #Load spikes CSV
        df = pd.read_csv(merged_file, header=None, names=['time', 'gid', 'rank'])
         
         
        #Reindex GIDs to start from 0 and go up
        def reindex_gids(df, gid_list):
            gid_map = {gid: i for i, gid in enumerate(sorted(gid_list))}
            return df.assign(gid=df['gid'].map(gid_map))
         
          
        #Split data
        mf_spikes = df[df['gid'].isin(mf_gids)]
        grc_spikes = df[df['gid'].isin(grc_gids)] 
         
        #Apply reindexing
        mf_spikes_reindexed = reindex_gids(mf_spikes, mf_gids)
        grc_spikes_reindexed = reindex_gids(grc_spikes, grc_gids)
         
        #Function to make raster plot
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
             
              
        #Plot and save
        plot_raster(mf_spikes_reindexed, "Mossy Fiber Raster Plot", f"{results_path}/mf_raster.png",col="red")
        plot_raster(grc_spikes_reindexed, "Granule Cell Raster Plot", f"{results_path}/grc_raster.png",col="green")
          
pc.barrier() 
pc.done() 
# Print the timing summary for a rank in the out file
print_timings(rank,timing_log)

# Properly finalize MPI before exiting
h.quit()


