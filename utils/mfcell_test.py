from neuron import h, load_mechanisms 
import matplotlib.pyplot as plt
import random 
 
#load_mechanisms('/media/oli/Research/Gitrepo/GranularLayerModel/src/')

h.load_file("stdlib.hoc")  # Required for VecStim
h.load_file("stdrun.hoc")  # Good practice for standard run protocols
 

# Assuming you already have this modified MossyFiber class:
# from your previous message or module
# from mymodule import MossyFiber


class MossyFiber:
    def __init__(self, gid, seed=42, t_offset=5, noise=0.075, number=5, rate=20, 
                 tstop=100, mode="poisson", custom_spike_times=None):
        self.gid = gid
        self.has_soma = False
        self.tstop = tstop
        self.mode = mode
        self.v = None  # Will be NetStim or VecStim

        random.seed(seed)

        if mode == "poisson":
            interval = 1000 / rate
            duration = tstop - t_offset
            max_possible_spikes = int(duration // interval)

            if max_possible_spikes < 1:
                raise ValueError(f"Rate too low: Cannot generate 1 spike in {duration} ms.")

            self.v = h.NetStim()
            self.v.start = t_offset
            self.v.interval = interval
            self.v.noise = noise
            self.v.number = number
            self.v.noiseFromRandom123(seed, 0, 0)

        elif mode == "random":
            # Use VecStim for manually specified or randomly generated spike times
            self.spike_times = h.Vector()

            if custom_spike_times is None:
                # Generate N random spike times uniformly between t_offset and tstop
                self.spike_times.from_python(
                    sorted(random.uniform(t_offset, tstop) for _ in range(number))
                )
            else:
                self.spike_times.from_python(sorted(custom_spike_times))

            self.v = h.VecStim()
            self.v.play(self.spike_times)

        else:
            raise ValueError(f"Unknown mode: {mode}. Use 'poisson' or 'random'.")

    def connect2target(self, target, thresh=0.5, delay=0.01):
        nc = h.NetCon(self.v, target)
        nc.threshold = thresh
        nc.delay = delay
        return nc

# Create one MossyFiber with random spikes
mf = MossyFiber(gid=0, number=15, tstop=100, mode="random", seed=12)

# Set up recording
tvec = h.Vector()
spikevec = h.Vector()

nc = mf.connect2target(None)
nc.record(tvec)

# Simulation settings
h.dt = 0.025
h.finitialize()
h.continuerun(100)

# Convert to Python lists for plotting
spike_times = list(tvec)

# Plot
plt.figure(figsize=(8, 2))
plt.eventplot([spike_times], orientation='horizontal', colors='black', linewidths=1.5)
plt.xlabel("Time (ms)")
plt.yticks([0], ['GID 0'])
plt.title("Spike Raster Plot (Single MossyFiber)")
plt.tight_layout()
#plt.savefig("mf_single_spikes.png", dpi=300)
plt.show()
