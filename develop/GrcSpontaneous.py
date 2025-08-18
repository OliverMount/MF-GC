# Tune the spontaneity of the GRC to < 1 Hz typically 0.5 to 0.8 hz
# simulate for 10 sec and see on the average 5 to 8 spikes occur in 10 sec
 
import os
import sys   
import glob
import random

sys.path.append('../utils/')
sys.path.append('../develop/')

from args import  *      # get all CLI arguments 
from methods import print_status

from Cell import MossyFiber, GranuleCell
import sqlite3         # For getting the connectivity, gid, 
from neuron import h, coreneuron, load_mechanisms 
 
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

results_path="../results"

# Random seed for reproducibility

seed_values = [random.randint(10, 50) for _ in range(5)]
#seed_values =[11,12,13,14,15]
#seed_values =[16,22,25,29,34]
print(seed_values)
 

# No coreneuron for time being
delay=0

tstop=10000  # 10 sec of simulation

h.nrnmpi_init()
pc = h.ParallelContext()
rank = int(pc.id())
nhost = int(pc.nhost())

# Create 4  mossy fibers
MF1=MossyFiber(0,tstop=tstop,mode="random",number=57,seed=seed_values[0])  # number is the no. of spikes per tstop (50000/10000)
MF2=MossyFiber(1,tstop=tstop,mode="random",number=60,seed=seed_values[1])
MF3=MossyFiber(2,tstop=tstop,mode="random",number=72,seed=seed_values[2])
MF4=MossyFiber(3,tstop=tstop,mode="random",number=68,seed=seed_values[3])
#print(seed_values)
 
# Spike detectors
pc.set_gid2node(MF1.gid,rank)
nc=MF1.connect2target(None)
pc.cell(MF1.gid, nc)      

pc.set_gid2node(MF2.gid,rank)
nc=MF2.connect2target(None)
pc.cell(MF2.gid, nc) 

pc.set_gid2node(MF3.gid,rank)
nc= MF3.connect2target(None)
pc.cell(MF3.gid, nc) 

pc.set_gid2node(MF4.gid,rank)
nc=MF4.connect2target(None)
pc.cell(MF4.gid, nc)     
#### Create 1 GRC and 4 MF

# Create one GRC 
GRC=GranuleCell(4,tstop=tstop,seed=seed_values[4]) 

pc.set_gid2node(GRC.gid, rank)
pc.cell(GRC.gid, GRC.connect2target(None)) 

# Synapse
syn=GRC.add_synapse(section=0)
GRC.synapses.append(syn)  # this may not be necessary 

for mf in [MF1, MF2, MF3, MF4]:
    nc = pc.gid_connect(mf.gid, syn)
    nc.delay = max(delay, 0.2)
    nc.weight[0] = 0.01  # Try a stronger weight
 

h.celsius = 34
h.dt = 0.025
h.finitialize()


# the following vectors are dynamically resized as the spike occurs
tvec = h.Vector()
idvec = h.Vector() 
#int(tstop/h.dt)*len_assigned_gids 
vvec = h.Vector()
vvec.record(GRC.soma(0.5)._ref_v) 
pc.spike_record(-1, tvec, idvec)   # -1 for all gids in that node
pc.set_maxstep(10) 
pc.psolve(tstop)
pc.barrier()  

# Store the spike times
 
# Create results directory if not exists
if rank == 0 and not os.path.exists(results_path):
    os.makedirs(results_path)
pc.barrier()  # Ensure directory exists before others write

# Save spikes recorded by this rank
file = f"{results_path}/grc_test.csv"
with open(file, "w") as f:
    for i in range(len(tvec)):
        f.write(f"{tvec[i]},{int(idvec[i])}\n")

pc.barrier()  # Wait until all ranks are done writing
 
 
# Plotting the rasters
# =============================================================================
# if rank == 0: 
#       
#     # Load spikes CSV
#     df = pd.read_csv(file, header=None, names=['time', 'gid' ]) 
#     # Function to make raster plot
#     def plot_raster(spikes_df, title, filename,col):
#         plt.figure(figsize=(12, 6))
#         plt.scatter(spikes_df['time'],  spikes_df['gid'] ,  color=col,s=1)
#         plt.xlabel("Time (ms)")
#         plt.ylabel("GID")
#         plt.title(title)
#         plt.tight_layout()
#         plt.savefig(filename, dpi=300)
#         #plt.show()
#         print(f"Saved: {filename}")
#          
#     # Plot and save
#     plot_raster(df, "Grc spkies", f"{results_path}/grc_test.png",col="green") 
#     
#     #print(f"vvec size: {vvec.size()}")  # should be > 0
# =============================================================================
    
grc_spikes = sum(1 for i in range(len(tvec)) if int(idvec[i]) == GRC.gid)
rate = grc_spikes / (tstop / 1000.0)
#print(f"GRC spike rate: {rate:.2f} Hz")

#grc_spikes = sum(1 for i in range(len(tvec)) if int(idvec[i]) == GRC.gid)
#rate = grc_spikes / (tstop / 1000.0)

if rank == 0:
    print(f"{rate:.3f}")  # Print only the number (for parsing)

# =============================================================================
#     v = np.array([vvec.x[i] for i in range(int(vvec.size()))])
#     t = np.arange(len(v)) * h.dt 
#     
#     plt.figure(figsize=(10, 4))
#     plt.plot(t, v)
#     plt.title("GRC Membrane Potential")
#     plt.xlabel("Time (ms)")
#     plt.ylabel("Vm (mV)")
#     plt.tight_layout()
#     plt.savefig(f"{results_path}/grc_voltage.png", dpi=300)
#     plt.show()
# =============================================================================

# Properly finalize MPI before exiting
pc.done()
h.quit()
