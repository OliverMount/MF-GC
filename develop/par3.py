from neuron import h, gui
from neuron.parallel import ParallelNetManager

pc = h.ParallelContext()
pnm = ParallelNetManager(pc)

rank = int(pc.id())

def create_stim():
    stim = h.NetStim()
    stim.number = 1
    stim.start = 5.0
    return stim

# Automatically assign and register cell
gid = rank
cell = create_stim()
pnm.register_cell(gid, cell)

# Record spikes
pnm.spike_record(gid)

# Run simulation
h.stdinit()
pc.psolve(20)

# Gather and print spikes
pc.barrier()
spikes = pnm.gatherspikes()
if rank == 0:
    print("Gathered spikes from all ranks:", spikes)

pc.done()

