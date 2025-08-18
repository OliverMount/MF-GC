import sqlite3
from neuron import h
import matplotlib.pyplot as plt

h.load_file('stdrun.hoc')

# 1. Create a small SQLite database with two cells and one connection
conn = sqlite3.connect(":memory:")
cursor = conn.cursor()

# Create tables
cursor.execute('''
CREATE TABLE nodes (
    gid INTEGER PRIMARY KEY,
    cell_type TEXT
)
''')
cursor.execute('''
CREATE TABLE edges (
    source_gid INTEGER,
    target_gid INTEGER
)
''')

# Insert two cells: gids 0 and 1
cursor.executemany('INSERT INTO nodes (gid, cell_type) VALUES (?, ?)', [
    (0, 'ballstick'),
    (1, 'ballstick')
])
# Insert one connection: 0 -> 1
cursor.execute('INSERT INTO edges (source_gid, target_gid) VALUES (?, ?)', (0, 1))
conn.commit()

# 2. Read cells and edges from the database
cursor.execute('SELECT gid FROM nodes')
gids = [row[0] for row in cursor.fetchall()]

cursor.execute('SELECT source_gid, target_gid FROM edges')
edges = cursor.fetchall()

# 3. Build the NEURON network using gids
pc = h.ParallelContext()
cells = {}

# Soma body only
class BallStick:
    def __init__(self, gid):
        self.soma = h.Section(name='soma')
        self.soma.L = self.soma.diam = 20
        self.soma.insert('hh')  # Add Hodgkin-Huxley mechanism for spiking
        self.syn = h.ExpSyn(self.soma(0.5))
        pc.set_gid2node(gid, pc.id())
        pc.cell(gid, h.NetCon(self.soma(0.5)._ref_v, None, sec=self.soma))  # Register cell

# Create cells
for gid in gids:
    cells[gid] = BallStick(gid)

# Connect cells according to the edges table
netcons = []
for src_gid, tgt_gid in edges:
    src_cell = cells[src_gid]
    tgt_cell = cells[tgt_gid]
    nc = h.NetCon(src_cell.soma(0.5)._ref_v, tgt_cell.syn, sec=src_cell.soma)
    nc.threshold = 0  # Default threshold for spike detection
    nc.weight[0] = 0.1
    netcons.append(nc)

# Stimulate cell 0 with a train of current pulses to generate multiple spikes
stim = h.NetStim()
stim.interval = 30  # ms between spikes
stim.number = 5     # number of spikes
stim.start = 5      # ms, when to start
stim.noise = 3     # deterministic

# Connect NetStim to cell 0 via a NetCon and IClamp-like current injection
syn_stim = h.ExpSyn(cells[0].soma(0.5))
nc_stim = h.NetCon(stim, syn_stim)
nc_stim.weight[0] = 0.02  # Strong enough to cause a spike

# Record membrane potentials from both cells
v0 = h.Vector().record(cells[0].soma(0.5)._ref_v)
v1 = h.Vector().record(cells[1].soma(0.5)._ref_v)
t = h.Vector().record(h._ref_t)

# Run the simulation
h.finitialize(-65)
h.tstop = 125
h.run()

plt.plot(t, v0, label='Cell 0 (presynaptic)')
plt.plot(t, v1, label='Cell 1 (postsynaptic)')
plt.xlabel('Time (ms)')
plt.ylabel('Vm (mV)')
plt.legend()
plt.show()



