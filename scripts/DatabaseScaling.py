 
import os 
import gzip
import pickle 
import sqlite3 

 
scaling = 2    # factor by which to scale the exising database

def load(networkx_path):
    with gzip.open(networkx_path, 'rb') as f:
        G = pickle.load(f)
    return G


os.chdir('/media/oli/Research/Gitrepo/GranularLayerModel/connectivity/')

# Load the binary graph 
data_path='/media/oli/Research/GitrepoO/cb2_project_analysis/graphs'


# binary mf_grc graph

data_name='graph_mf_grc_binary_210519.gz'  # provided
mf_grc = load(os.path.join(data_path,data_name))  # G is of DiGraph python class
 
# Some checks
mf_grc.nodes['mf_181__1'] 
mf_grc.nodes['grc_1651']
list(mf_grc.successors('mf_181__1'))   # From mf_181_1 to nodes m
list(mf_grc.predecessors('mf_181__1'))  # There is no predecessors of mf_181_1  From somewhere to the node
  
# get the nodes and edges
cells= list(mf_grc.nodes)  # list of nodes (Cells) in the graph
n_cells=len(cells)  # Number of nodes  5023 
print(n_cells)

edges_list = list(mf_grc.edges)  # Source and target 


## SQL constrains

# If Table A has a foreign key reference to Table B, then:
# You must insert into Table B first, so that the referenced row exists.
# Otherwise, SQL will raise an integrity error (e.g., FOREIGN KEY constraint failed).

# 1. Cell (A) --> Worker (B)
# 2. synapse (A)→ cell (B)
# 3. gap (A) → cell (B)

# So the order of fill should be worker, cell, synapse, gap

#------------------------------------------------------------------------------
# Database creation from the anatomical DiGraph mf--> grc
#------------------------------------------------------------------------------
 
conn = sqlite3.connect(f'mf_grc_scaled_{scaling}.db')   # This will create a database or access the one if it does exists
cursor = conn.cursor()   # cursor is a iterator used for executing commands or accessing data

# Nodes table with spatial coordinates
# Create table seperate table (which is a normalization procedure)


# Enable foreign key constraints (looks critical!, it is not enabled by default)
# This ensures that you cannot insert an edge referencing a non-existent node.
cursor.execute('PRAGMA foreign_keys = ON') 


# To view the index but not necessary
#cursor.execute("PRAGMA index_list('nodes')")
#print(cursor.fetchall())


# Create nodes table  (just creating the header)
cursor.execute('''
CREATE TABLE cell (
    gid INTEGER PRIMARY KEY, 
    cell_type TEXT CHECK(cell_type IN ('mf', 'grc')),   --this can be avoided for multiple cell types 
    mpi_rank_id INTEGER NOT NULL,
    worker_host_id INTEGER NOT NULL,
    FOREIGN KEY(worker_host_id) REFERENCES worker(worker_host_id)
)  
''')


# Create edges table to include the gid 
# Edges table with foreign key constraints
# Create edges table (the header) 

# gap connection a globally unique identifier sid, which is crucial for NEURON's pc.source_var() / pc.target_var() pairing.
# Create a gap junction table
cursor.execute('''
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
    PRIMARY KEY (source_gid, target_gid),
    FOREIGN KEY(source_gid) REFERENCES cell(gid),
    FOREIGN KEY(target_gid) REFERENCES cell(gid),
    FOREIGN KEY (syn_type) REFERENCES synapse_type(syn_type),
    FOREIGN KEY (syn_dynamics) REFERENCES synapse_dynamics(syn_dynamics)
)
''')


cursor.execute('''
CREATE TABLE gap (
    sid INTEGER PRIMARY KEY AUTOINCREMENT,  -- 

    source_gid INTEGER NOT NULL,
    source_sec_id INTEGER NOT NULL,
    source_dend_loc REAL NOT NULL,

    target_gid INTEGER NOT NULL,
    target_sec_id INTEGER NOT NULL,
    target_dend_loc REAL NOT NULL,

    g REAL NOT NULL DEFAULT 3e-4,

    FOREIGN KEY (source_gid) REFERENCES cell(gid),
    FOREIGN KEY (target_gid) REFERENCES cell(gid),

    CHECK (source_gid < target_gid)  -- make sure when we populate the gap 
) 
''')
 

### Worker table

cursor.execute('''
CREATE TABLE worker (
    worker_host_id INTEGER PRIMARY KEY,     -- integer ID for the host
    worker_name TEXT NOT NULL               -- actual hostname (e.g., 'olaf-09')
) 
''')
 
# Insert default worker host first (becuase of the foreign key integrity)
cursor.execute('''
    INSERT INTO worker (worker_host_id, worker_name)
    VALUES (0, 'default_host')
''')


##### Synapse type info table 
cursor.execute('''
CREATE TABLE synapse_type (
    syn_type INTEGER PRIMARY KEY,   -- 1: Excitatory, 0: Inhibitory
    description TEXT NOT NULL
) 
''')
 
cursor.execute('''
INSERT INTO synapse_type (syn_type, description) VALUES
    (1, 'Excitatory'),
    (0, 'Inhibitory')  
''')


##### Synapse dynamics info table 

cursor.execute('''
CREATE TABLE synapse_dynamics (
    syn_dynamics INTEGER PRIMARY KEY,   -- 0: Linear, 1: Nonlinear
    description TEXT NOT NULL)

''')

cursor.execute('''
INSERT INTO synapse_dynamics (syn_dynamics, description) VALUES
    (0, 'Linear'),
    (1, 'Nonlinear')
''')

 
#------- PREPARE the Nodes Data--------
## Inserting data in to node  from the graph
# First creating the node_data as list of tuples 

# Separate and sort nodes by cell_type
# This order is still ok?  mf first and then the grc
mf_cells = [n for n in cells if mf_grc.nodes[n]['cell_type'] == 'mf']  # 1070 units
grc_cells = [n for n in cells if mf_grc.nodes[n]['cell_type'] == 'grc']  # 3953 units


# grc_cells_ to remove
# Remove these grc cells 

# Concatenate, mf nodes first
ordered_cells_temp = mf_cells + grc_cells   # 5023 units

gids_to_remove=[1096,1353, 2773, 3445, 3615, 3725, 3992, 4019, 4360, 4435, 4505, 4506, 4513, 4532, 4550, 4610, 4687, 4721, 4754, 4786, 4817, 4833, 4957, 4964, 4981, 4984, 4988, 4993]
 

ordered_cells=[ordcell for idx,ordcell in enumerate(ordered_cells_temp) if idx not in gids_to_remove]


# Prepare units_data with explicit gid assignment
cells_data = [
    (
        gid,  
        mf_grc.nodes[cell_id]['cell_type'],  
        0,    # Zero mpi rank as initialization
        0     #  initialization of worker node
    )
    for gid, cell_id in enumerate(ordered_cells)   # here is assigning gid not autoincrement 
]
 

cursor.executemany('''
    INSERT INTO cell (gid,  cell_type,   mpi_rank_id,worker_host_id)
    VALUES (?, ?, ?, ?)
''', cells_data)



# For single data insertion use the format below
# =============================================================================
# for node_id in nodes:
#     attrs = G.nodes[node_id]
#     cursor.execute('''
#         INSERT INTO nodes (node_id, cell_type, x, y, z)
#         VALUES (?, ?, ?, ?, ?)
#     ''', (
#         node_id,
#         attrs['cell_type'],
#         *attrs['xyz']  # Unpack (x,y,z) tuple
#     ))
# =============================================================================
 

#------- PREPARE the Edges Data-------- 
unit_to_gid = {unit_id: gid for gid, unit_id in enumerate(ordered_cells)}

#  to ensure uniqueness
edges_data = list({
    (unit_to_gid[source], 
     unit_to_gid[target],
     1,  #  1 is the  connectivity weights for the binary graph
     0, # default no delay
     1,   # 1 for excitatory and 0 for inhibitory
     0,   # 0 for linear synapse
     0,  # always connect to the soma of the grc for time being (0 for soma)
     0.5, # location of the dendritic synapse on the target
     1, # Single synapse needed
     0   # By fault the first syanpse in the cell list; dynamically will be updated
     )   
    for source, target in edges_list
    if source in unit_to_gid and target in unit_to_gid
} 
)
 
# Remember that when we build the synapse database it is all about reducing entering the  database values





#------- Insert Edges Data--------  
cursor.executemany('''
    INSERT INTO synapse (source_gid, target_gid,weight,delay,syn_type, syn_dynamics,target_dend_id,dend_syn_loc,need_single_syn,syn_id)
    VALUES (?, ?,?,?,?,?,?,?,?,?)
''', edges_data)
 
conn.commit()  # Save changes permanently


# Display from edges table
#cursor.execute('SELECT * FROM synapse order by  target_gid asc LIMIT 10')  # Just 5 entries
#rows = cursor.fetchall()
#for row in rows:
#    print(row) 
     

# For source-target lookups (for faster access)
# The index is "behind the scenes." for faster access 

# For the nodes table gid is already the primary key for which the sqlite will automatically
# assign the index


# for the edges table
cursor.execute('CREATE INDEX idx_edges_source_gid ON synapse(source_gid)')  # This is not visible outside
cursor.execute('CREATE INDEX idx_edges_target_gid ON synapse(target_gid)')


# These gids are obtained from the synapse table after finding the orphaned connection
#gids_to_remove=[1096,1353, 2773, 3445, 3615, 3725, 3992, 4019, 4360, 4435, 4505, 4506, 4513, 4532, 4550, 4610, 4687, 4721, 4754, 4786, 4817, 4833, 4957, 4964, 4981, 4984, 4988, 4993]
 

#placeholders = ','.join(['?'] * len(gids_to_remove))
#stmt = f"DELETE FROM cell WHERE gid IN ({placeholders})" 
#cursor.execute(stmt, gids_to_remove) 


conn.commit()  # Save index definitions  

    

## This is for scaling purpose

original_n_cells = len(ordered_cells)
total_cells = original_n_cells * scaling

# New cell data
scaled_cells_data = []
for block in range(scaling):
    gid_offset = block * original_n_cells
    for i, cell_tuple in enumerate(cells_data):
        # cell_tuple: (gid, cell_name, cell_type, x, y, z, mpi_rank_id, worker_host_id)
        old_gid = cell_tuple[0]
        new_gid = gid_offset + old_gid
         
        # Copy the rest of the fields as-is, except gid and cell_name
        scaled_cells_data.append((
            new_gid,  cell_tuple[1], cell_tuple[2], cell_tuple[3]
        ))

# New synapse data
scaled_edges_data = []
for block in range(scaling):
    gid_offset = block * original_n_cells
    for edge_tuple in edges_data:
        # edge_tuple: (source_gid, target_gid, ...)
        new_source = gid_offset + edge_tuple[0]
        new_target = gid_offset + edge_tuple[1]
        # Copy the rest of the fields as-is
        scaled_edges_data.append((
            new_source, new_target, *edge_tuple[2:]
        ))


cursor.execute('DELETE FROM synapse')
cursor.execute('DELETE FROM cell')

conn.commit()


cursor.executemany('''
    INSERT INTO cell (gid,  cell_type,  mpi_rank_id, worker_host_id)
    VALUES (?, ?, ?, ?)
''', scaled_cells_data)

cursor.executemany('''
    INSERT INTO synapse (source_gid, target_gid, weight, delay, syn_type, syn_dynamics, target_dend_id, dend_syn_loc, need_single_syn, syn_id)
    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
''', scaled_edges_data)


conn.commit()

#os.system('mv mf_grc_scaled.db /media/oli/Data/')
#os.system('scp mf_grc_scaled_2.db oliver@172.12.200.52:/home/oliver/Desktop') 
