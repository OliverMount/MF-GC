import sys
import os
import numpy as np
import gzip
import pickle
import matplotlib.pyplot as plt
import sqlite3
import networkx as nx  # For importing the data as a Digraph
import re


import compress_pickle

# Database should have
#gid → mpi_rank_id → hostname (physical node)


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
 
conn = sqlite3.connect('mf_grc.db')   # This will create a database or access the one if it does exists
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
    cell_name TEXT,
    cell_type TEXT CHECK(cell_type IN ('mf', 'grc')),   --this can be avoided for multiple cell types
    x INTEGER NOT NULL,
    y INTEGER NOT NULL,
    z INTEGER NOT NULL,
    mpi_rank_id INTEGER NOT NULL,
    worker_host_id INTEGER NOT NULL,
    FOREIGN KEY(worker_host_id) REFERENCES worker(worker_host_id)
)  
''')


# Create edges table to include the gid 
# Edges table with foreign key constraints
# Create edges table (the header)
cursor.execute('''
CREATE TABLE synapse (
    source_gid INTEGER NOT NULL,
    target_gid INTEGER NOT NULL,
    weight INTEGER NOT NULL DEFAULT 1,   -- for binary it is 1
    syn_type INTEGER NOT NULL DEFAULT 1,    -- 1 for excitatory  0 for inhibitory synapse
    syn_dynamics INTEGER NOT NULL DEFAULT 0,  -- 0: linear, 1: nonlinear
    target_dend_id REAL NOT NULL,
    dend_syn_loc REAL NOT NULL DEFAULT 0.5,   -- default at the center 
    PRIMARY KEY (source_gid, target_gid),
    FOREIGN KEY(source_gid) REFERENCES cell(gid),
    FOREIGN KEY(target_gid) REFERENCES cell(gid),
    FOREIGN KEY (syn_type) REFERENCES synapse_type(syn_type),
    FOREIGN KEY (syn_dynamics) REFERENCES synapse_dynamics(syn_dynamics)
)
''')


# gap connection a globally unique identifier sid, which is crucial for NEURON's pc.source_var() / pc.target_var() pairing.
# Create a gap junction table
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

# Concatenate, mf nodes first
ordered_cells = mf_cells + grc_cells   # 5023 units

# Prepare units_data with explicit gid assignment
cells_data = [
    (
        gid, 
        cell_id, 
        mf_grc.nodes[cell_id]['cell_type'], 
        *mf_grc.nodes[cell_id]['xyz'],
        0,    # Zero mpi rank as initialization
        0     #  initialization of worker node
    )
    for gid, cell_id in enumerate(ordered_cells)   # here is assigning gid not autoincrement 
]
 

cursor.executemany('''
    INSERT INTO cell (gid, cell_name, cell_type, x, y, z,mpi_rank_id,worker_host_id)
    VALUES (?, ?, ?, ?, ?,?,?,?)
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
     1,   # 1 for excitatory and 0 for inhibitory
     0,  # always connect to the zeroth dendrite
     0.5 # location of the dendritic synapse on the target
     )   
    for source, target in edges_list
    if source in unit_to_gid and target in unit_to_gid
} 
)
 


#------- Insert Edges Data--------  
cursor.executemany('''
    INSERT INTO synapse (source_gid, target_gid,weight,syn_type, target_dend_id,dend_syn_loc)
    VALUES (?, ?,?,?,?,?)
''', edges_data)
 
conn.commit()  # Save changes permanently

# For source-target lookups (for faster access)
# The index is "behind the scenes." for faster access 

# For the nodes table gid is already the primary key for which the sqlite will automatically
# assign the index


# for the edges table
cursor.execute('CREATE INDEX idx_edges_source_gid ON synapse(source_gid)')  # This is not visible outside
cursor.execute('CREATE INDEX idx_edges_target_gid ON synapse(target_gid)')
conn.commit()  # Save index definitions


## For inserting into gap table(where is this data from??) 
# Prepare the gap list data  linked with cell morphology method()
# this about extracting from the morphology data

gap_list = [
    # Example tuples: (gid1, sec1, loc1, gid2, sec2, loc2, g)
    (10, 2, 0.4, 5, 3, 0.6, 3e-4),
    (5, 3, 0.6, 10, 2, 0.4, 3e-4),  # Duplicate of above, should be filtered
    (20, 1, 0.5, 15, 2, 0.5, 3e-4),
    # ...more
]


# Assuming gap_list is defined
gap_entries = set()

for gid1, sec1, loc1, gid2, sec2, loc2, g in gap_list:
    if gid1 > gid2:
        gid1, gid2 = gid2, gid1
        sec1, sec2 = sec2, sec1
        loc1, loc2 = loc2, loc1
    gap_entries.add((gid1, sec1, loc1, gid2, sec2, loc2, g))

gap_data = list(gap_entries)

cursor.executemany('''
    INSERT OR IGNORE INTO gap (
        source_gid, source_sec_id, source_dend_loc,
        target_gid, target_sec_id, target_dend_loc,
        g
    ) VALUES (?, ?, ?, ?, ?, ?, ?)
''', gap_data)

conn.commit()

cursor.close()
conn.close() 
######################### upto here is updated one ##########################
# For spatial queries (if ndeded)
# cursor.execute('CREATE INDEX idx_nodes_z ON nodes(z)') 



# Function to create sqlite3 db from a Graph
# Load the necessary modules for the function
# Have to make is more generalized for any DiGraph
def create_db_from_graph(G,db_name): 
    
    if not isinstance(G, nx.DiGraph):
        raise TypeError("Argument G must be DiGraph from networkx module")
 
    
    if os.path.exists(db_name):
        conn = sqlite3.connect(db_name)   # This will create a database or access the one if it does exists
        cursor = conn.cursor()   # cursor is a iterator used for executing commands or accessing data


#------------------------------------------------------------------------------
# Database creation from the anatomical DiGraph  grc--> pc
#------------------------------------------------------------------------------
 
data_name='grc_pc.gz' # I made in the Cleanup_data.py
grc_pc = load(os.path.join(data_path,data_name))  # GrcPC is of class DiGraph  

 
# get the nodes and edges
nodes= list(grc_pc.nodes)  # list of nodes in the graph  grc front and pcs back
n_nodes=len(nodes)  # Number of nodes  611 (grc  + pc)
print(n_nodes)   
edges_list = list(grc_pc.edges)  
  

# Sort the nodes accoring to the number grc first, pc next
def extract_number(s, prefix):
    """Extract integer after a given prefix, or return None if not matching."""
    match = re.match(rf'{prefix}(\d+)$', s)
    return int(match.group(1)) if match else None

def sort_key(s):
    # grc_<number>
    n = extract_number(s, 'grc_')
    if n is not None:
        return (0, n)
    # pc_<number>
    n = extract_number(s, 'pc_')
    if n is not None:
        return (1, n)
    # pc_dendrite_<number>
    n = extract_number(s, 'pc_dendrite_')
    if n is not None:
        return (2, n)
    # All others
    return (3, s)

  
# Sort the list
nodes = sorted(nodes, key=sort_key) 
#print(nodes)
 


conn = sqlite3.connect('grc_pc.db')   # This will create a database or access the one if it does exists
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
CREATE TABLE nodes (
    gid INTEGER PRIMARY KEY,                    -- gid is the primary key
    node_id TEXT,                              -- e.g., 'grc_668' or 'pc_34'
    cell_type TEXT CHECK(cell_type IN ('grc', 'pc')),
    x INTEGER NOT NULL,                                    -- X coordinate
    y INTEGER NOT NULL,                                    -- Y coordinate
    z INTEGER NOT NULL                                     -- Z coordinate
)
''')


# Create edges table to include the gid 
# Edges table with foreign key constraints
# Create edges table (the header)
cursor.execute('''
CREATE TABLE edges (
    source_gid INTEGER NOT NULL,
    target_gid INTEGER NOT NULL,
    PRIMARY KEY (source_gid, target_gid),      -- this is unique
    FOREIGN KEY(source_gid) REFERENCES nodes(gid),
    FOREIGN KEY(target_gid) REFERENCES nodes(gid)
)
''')
 
#------- PREPARE the Nodes Data--------
## Inserting data in to node  from the graph
# First creating the node_data as list of tuples 

# Separate and sort nodes by cell_type
# This order is still ok?  mf first and then the grc
grc_nodes = [n for n in nodes if grc_pc.nodes[n]['cell_type'] == 'grc']  #521
pc_nodes = [n for n in nodes if grc_pc.nodes[n]['cell_type'] == 'pc']  #90

# Concatenate, mf nodes first
ordered_nodes =    grc_nodes + pc_nodes   # 611 nodes

# Prepare node_data with explicit gid assignment
node_data = [
    (
        gid, 
        node_id, 
        grc_pc.nodes[node_id]['cell_type'], 
        *grc_pc.nodes[node_id]['xyz']
    )
    for gid, node_id in enumerate(ordered_nodes)  # this would create gid
]
 

# multple insertion into table all at once
cursor.executemany('''
    INSERT INTO nodes (gid, node_id, cell_type, x, y, z)
    VALUES (?, ?, ?, ?, ?,?)
''', node_data)
 

#------- PREPARE the Edges Data-------- 
node_to_gid = {node_id: gid for gid, node_id in enumerate(ordered_nodes)}

#  to ensure uniqueness
edges_set = {
    (node_to_gid[source], node_to_gid[target])
    for source, target in edges_list
    if source in node_to_gid and target in node_to_gid
} 
 
edges_data = list(edges_set)  # this will be inserted into the table


#------- Insert Edges Data-------- 

cursor.executemany('''
    INSERT INTO edges (source_gid, target_gid)
    VALUES (?, ?)
''', edges_data)
 
conn.commit()  # Save changes permanently

# For source-target lookups (for faster access)
# The index is "behind the scenes." for faster access 

# For the nodes table gid is already the primary key for which the sqlite will automatically
# assign the index


# for the edges table
cursor.execute('CREATE INDEX idx_edges_source_gid ON edges(source_gid)')  # This is not visible outside
cursor.execute('CREATE INDEX idx_edges_target_gid ON edges(target_gid)')
conn.commit()  # Save index definitions
