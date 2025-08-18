from neuron import h
h.nrnmpi_init()       # initialize MPI, This is necessary
pc = h.ParallelContext()
print('I am {} of {}'.format(pc.id(), pc.nhost()))
h.quit()              # necessary to avoid a warning message on parallel exit on some systems



#mpiexec -n 4 python testmpi.py
