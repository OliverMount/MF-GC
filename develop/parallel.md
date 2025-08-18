1. The call to `h.nrnmpi_init()` must happen before any use of the ParallelContext class – which forms a key part of any NEURON parallel simulation.

✅ h.nrnmpi_init()
Low-level initialization function.

Used to manually initialize NEURON’s MPI (Message Passing Interface) subsystem.

Required only when:

You're doing MPI-based parallel computing without using ParallelContext, or

You’re controlling the simulation environment more directly and want finer control.

When to use:
Rarely used in modern NEURON scripts.

If you're using ParallelContext, it automatically handles MPI initialization, so you typically don’t need to call h.nrnmpi_init() explicitly.



✅ ParallelContext (from neuron.h.ParallelContext())
Core parallel interface in NEURON for distributed simulations using MPI.

Wraps lower-level MPI operations and provides high-level functions for:

GID (global identifier) management

Spike exchange

Communication and synchronization between ranks

Running distributed simulations (e.g., pc.psolve(), pc.set_gid2node(), etc.)

Common methods in ParallelContext:
Method	Purpose
set_gid2node(gid, node)	Assigns a global identifier to a node/rank
create_cell(gid)	Declares that the rank owns the cell with that gid
connect() / nc_append()	Set up NetCon-based spike connections
spike_record()	Record spikes of given gids
psolve(tstop)	Advance the simulation
barrier(), allreduce()	MPI sync/comm methods
round_robin()	Assign cells to ranks evenly
prun(), pcontinue()	Simulation run controls


✅ ParallelNetManager
Higher-level helper class built on top of ParallelContext.

Simplifies large-scale, parallel network construction and simulation by abstracting:

Cell creation and GID assignment

Connection setup

Spike recording and gathering

Spike output writing

Load balancing (via round_robin())

Common methods:
Method	Purpose
register_cell(gid, cell)	Register a cell object with its GID
create_cell()	Construct a cell and register it
spike_record()	Automatically record cell spikes
gatherspikes()	Collect spikes from all ranks
splitcell()	Partition cell computation across ranks
want_all_spikes()	Forces collection of all spike events

When to use:
Great for large network models where you want:

Cleaner code 
Automated management of GIDs and connectivity

Easier handling of spike data


Feature	            ParallelContext	ParallelNetManager
GID assignment	    Manual	        Automatic
Cell registration   Manual	         register_cell handles it
Spike recording	    pc.spike_record(gid, vec)	spike_record(gid) simplifies it
Spike gathering	    Manual			gatherspikes() collects across ranks
Simplicity	    Medium			High



## CoreNEURON setting ##   This has be the there for all CoreNeuron
 h.cvode.cache_efficient(1)





2. ParallelNetManager class has the following methods
create_cell · gatherspikes · gid_exists · nc_append · pcontinue · pinit · prstat · prun · psolve · register_cell · round_robin · set_gid2node · spike_record · splitcell · want_all_spikes
