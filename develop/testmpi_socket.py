from neuron import h
import socket

h.nrnmpi_init()
pc = h.ParallelContext()

rank = pc.id()
nhost = pc.nhost()
hostname = socket.gethostname() 

print(f"MPI rank {rank} of {nhost} is running on node {hostname}")

h.quit() 