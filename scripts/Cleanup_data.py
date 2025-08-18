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


def load(networkx_path):
    with gzip.open(networkx_path, 'rb') as f:
        G = pickle.load(f)
    return G


# Load the binary graph 
data_path='/media/oli/Research/GitrepoO/cb2_project_analysis/graphs' 
data_name='graph_grc_pc_synapse_210429_coalesced.gz'   # cell bodies inside the data-set
G_local = load(os.path.join(data_path,data_name))   
data_name="graph_pfs_pc_synapse_210429_coalesced.gz"
G_remote = load(os.path.join(data_path,data_name)) 


# print cell types in the graphs
cell_types = set()
for n in G_local.nodes():
    if 'cell_type' not in G_local.nodes[n]:
        continue
    cell_types.add(G_local.nodes[n]['cell_type'])
print(f'Local: {cell_types}')
#Local: {'pc', 'dendrite_fragment', 'grc'}  for grc -> pc connectivity

cell_types = set()
for n in G_remote.nodes():
    if 'cell_type' not in G_remote.nodes[n]:
        continue
    cell_types.add(G_remote.nodes[n]['cell_type'])
print(f'Remote: {cell_types}')   
# Remote: {'cf', 'pc', 'dendrite_fragment', 'pf'}  pf -> pc connectivity; Not sure if it is needed

# Filter and get cell lists
local_grcs = []   # both in local and in remote
remote_grcs = []  # only in remote
all_pcs = set()   # both in local and in remote
cf=[]   # Only in remote
  

def process_graph(G):
    for n in G.nodes():
        if 'cell_type' not in G.nodes[n]:
            continue
        cell_type = G.nodes[n]['cell_type']  # It has only three ['dendrite_fragment', 'grc', 'pc']
        if cell_type == 'grc':
            local_grcs.append(n)
        elif cell_type == 'pf':
            remote_grcs.append(n)
        elif cell_type == 'pc' or cell_type == 'dendrite_fragment':
            all_pcs.add(n)
        elif cell_type == 'cf':
            cf.append(n)

process_graph(G_local)
print("The length of pcs or dendrite_fragment in G_local is",len(all_pcs))  #97
print("The length of grcs in G_local is",len(local_grcs))  # 521
print("The length of pf in G_local is",len(remote_grcs))   # 0 
print("The length of cf in G_local is",len(cf))   # 0 

#local_grcs = []
#remote_grcs = []
#all_pcs = set()
process_graph(G_remote)

print("The length of pcs or dendrite_fragment in G_remote is",len(all_pcs))  #116
print("The length of grcs in G_remote is",len(local_grcs)) # 0, no grcs with cell body
print("The length of pf in G_remote is",len(remote_grcs))   # 4485
print("The length of cf in G_remote is",len(cf))   # 1 

# count the pcs only with pc_<a number>
count = sum(1 for name in all_pcs if re.fullmatch(r'pc_\d+', name))
print(count)  #91 just the pc no dendritic segment


# Check that there are no duplicated synapses
n = 0
syn_locs = set()
syn_origin = {}
def check_duplication(G, cell_list):
    for nid in cell_list:
        for nbr, datadict in G.adj[nid].items():
            for syn in datadict:
                syn_attrs = datadict[syn]
                syn_loc = datadict[syn]['syn_loc']
                assert syn_loc not in syn_locs
                syn_locs.add(syn_loc)
                syn_origin[syn_loc] = nid

check_duplication(G_local, local_grcs)
check_duplication(G_remote, remote_grcs)



data_name='db_pf_contacts_201224.gz'   # pf -> PC
pf_pc_touches = compress_pickle.load(os.path.join(data_path,data_name))

data_name='grc_pc_touches_210429.gz'  # Grc -> PC, this data set can be used to construct binary connectivity
grc_pc_touches = compress_pickle.load(os.path.join(data_path,data_name))

## GRC --> PC

# filter out newer neurons not available in the touch db or vice versa
touchdb_grc = set(grc_pc_touches.keys()) & set(local_grcs)
print("Length of touchdb_grc", len(touchdb_grc) )  # 521  (all the local_grcs)
assert(len(touchdb_grc) > 0)


G_local.nodes['grc_1178']  # <- This contains the local Grc location 

touchdb_pcs = set()
for k,v in grc_pc_touches.items():  # This contains the PC locations 
    touchdb_pcs |= (set(v.keys()))
touchdb_pcs &= all_pcs
assert(len(touchdb_pcs) > 0)

# the local Grc location and PC locations should be store in the graph for creating data

# make a binary "touch" networkx graph 
G = nx.DiGraph()
distance_threshold = 160

for grc in touchdb_grc:
    grc_node_details=G_local.nodes[grc]   # location information dictionary  # ex:grc_1715
    G.add_node(grc,**grc_node_details)  
    for pc, (dist, location) in grc_pc_touches[grc].items():
        if pc not in touchdb_pcs or pc not in G_local.nodes:  # check in Glocal as well
            continue 
        pc_node_details=G_local.nodes[pc]    # ex: pc_39
        G.add_node(pc,**pc_node_details)   
        if dist < distance_threshold:
            G.add_edge(grc, pc)
            
assert(len(G.edges) > 0)
G_touch_grc = G  
 

# make a binary "synapse" networkx graph
## This is used to make the database for Grc--> PC
G = nx.DiGraph()

for grc in touchdb_grc:
    grc_node_details=G_local.nodes[grc]   # location information dictionary  #grc_1715
    G.add_node(grc,**grc_node_details)  
    for pc in G_local.adj[grc]:  # 'AdjacencyView' class
        if pc not in touchdb_pcs:
            continue
        pc_node_details=G_local.nodes[pc]    # ex: pc_39
        G.add_node(pc,**pc_node_details)   
        #G.add_node(pc)
        G.add_edge(grc, pc)

assert(len(G.edges) > 0)
G_binary_grc = G     # This is grc --> Purkinji cell  
 
# Save as gzipped pickle file
# This will be loaded for making the sqlite2 database
with gzip.open("grc_pc.gz", "wb") as f:
    pickle.dump(G_binary_grc, f)
    
    
os.system('mv grc_pc.gz /media/oli/Research/GitrepoO/cb2_project_analysis/graphs/')
    

# Make sure all the nodes have the information embeddeded  ( yes embedded)
required_attrs = {'cell_type', 'xyz', 'tags'}
missing_attrs = []

for node in G.nodes:
    node_data = G.nodes[node]
    # Check if all required attributes exist
    if not required_attrs.issubset(node_data.keys()):
        missing_attrs.append((node, node_data))

if not missing_attrs:
    print("All nodes have cell_type, xyz, and tags attributes")
else:
    print(f"{len(missing_attrs)} nodes missing attributes:")
    for node, attrs in missing_attrs[:5]:  # Show first 5 problematic nodes
        print(f"  - {node}: Missing {required_attrs - attrs.keys()}")


# THIS part have not yet done; Check if this is needed
## PF --> PC 

# filter out newer neurons not available in the touch db or vice versa
touchdb_pfs = set(pf_pc_touches.keys()) & set(remote_grcs)
assert(len(remote_grcs) > 0)

touchdb_pcs = set()
for k,v in pf_pc_touches.items():
    touchdb_pcs |= (set(v.keys()))
touchdb_pcs &= all_pcs
assert(len(touchdb_pcs) > 0)

# make a binary "touch" networkx graph

G = nx.DiGraph()
distance_threshold = 160

for pfs in touchdb_pfs:
    G.add_node(pfs)
    for pc, (dist, _) in pf_pc_touches[pfs].items():
        if pc not in touchdb_pcs:
            continue
        G.add_node(pc)
        if dist < distance_threshold:
            G.add_edge(pfs, pc)
            
assert(len(G.edges) > 0)
G_touch_pfs = G


# make a binary "synapse" networkx graph
G = nx.DiGraph()

for pfs in touchdb_pfs:
    G.add_node(pfs)
    for pc in G_remote.adj[pfs]:
        if pc not in touchdb_pcs:
            continue
        G.add_node(pc)
        G.add_edge(pfs, pc)

assert(len(G.edges) > 0)
G_binary_pfs = G     # This is pf --> Purkinji cell  
