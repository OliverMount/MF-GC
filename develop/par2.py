from neuron import h, gui
pc = h.ParallelContext()

rank = int(pc.id())
nhost = int(pc.nhost())

# Create a NetStim-based artificial cell
gid = rank  # One cell per rank
cell = h.NetStim()
cell.number = 1
cell.start = 5.0  # ms

# Register the GID and associate it with the cell
pc.set_gid2node(gid, rank)
nc = h.NetCon(cell, None)
pc.cell(gid, nc)

# Record spikes
spikevec = h.Vector()
idvec = h.Vector()
pc.spike_record(gid, spikevec, idvec)

# Initialize simulation
h.stdinit()

# 🔧 Set minimum delay > 0
pc.set_maxstep(0.1)

# Run the simulation
pc.psolve(20)

# Print recorded spikes
print(f"Rank {rank} recorded spikes: {list(spikevec)}")

pc.barrier()
pc.done()

