if False:
    
    class CerebellumCell1:
        def __init__(self, gid=None,seed=42):
            self.gid = gid
            self.seed=42
            self.soma = None
            self.sections = []  # list of sections
            self.synapses = []
            self.gap_junctions = []
            self.stimulus = None
            self.recording = {}   # Dictionary of with key t and v 
            self.spiketime = h.Vector()   # For recording 
            self.tstop = 100
    
            self.create_sections()  # creates soma and adds to sections
    
        def create_sections(self):
            if self.soma is None:
                self.create_morphology()
            else:
                self.sections.append(self.soma)
    
        def create_morphology(self):
            self.soma = h.Section(name='soma')
            self.soma.L = 30
            self.soma.diam = 30
            self.soma.insert('hh')
            self.sections.append(self.soma)
    
        def insert_mechanisms(self):
            for sec in self.sections:
                sec.Ra = 100
                sec.cm = 1.0
    
        def setup_recording(self):
            self.recording['t'] = h.Vector().record(h._ref_t)
            if self.soma:
                self.recording['v'] = h.Vector().record(self.soma(0.5)._ref_v)
    
        def initialize_mechanisms_and_recording(self):
            self.insert_mechanisms()
            self.setup_recording()
    
        def add_synapse(self, section=None, loc=0.5, syn_type='ExpSyn', tau=2.0):
            section = section if section else self.soma
            if syn_type == 'ExpSyn':
                syn = h.ExpSyn(section(loc))
                syn.tau = tau
            else:
                raise NotImplementedError(f"Synapse '{syn_type}' not supported.")
            self.synapses.append(syn)
             
        def add_current_clamp(self, delay=10, dur=50, amp=0.1):
            stim = h.IClamp(self.soma(0.5))
            stim.delay = delay
            stim.dur = dur
            stim.amp = amp
            self.stimulus = stim
    
        def add_gap_junction(self, section=None, loc=0.5, g=300e-6):
            section = section if section else self.soma
            seg = section(loc)
            if hasattr(self, 'pc') and self.pc:
                gj = h.gap(seg)
            else:
                gj = h.gapp(seg)
            gj.g = g
            self.gap_junctions.append(gj)
     
        def get_recordings(self):
            return {k: list(vec) for k, vec in self.recording.items()} 
        
        def action_potential_count(self,section,threshold):
            self.AP = h.APCount(section)
            self.AP.thresh = threshold
            #self.AP.record()
     
      
        def create_sections(self):
            # dendrite section is also hardcoded; How to automize?
            self.soma = h.Section(name="soma", cell=self)
            self.dend = [h.Section(name=f"dend[{i}]", cell=self) for i in range(12)]
            self.allsec = h.SectionList()
            self.allsec.append(self.soma)
    
            for sec in self.dend:
                self.allsec.append(sec)
                 
            
        def set_geometry(self):
                    
            self.seed = 42
            self.alph = h.Random(self.eseed)
            dprob = self.alph.uniform(-9.76 * 0.2, 9.76 * 0.2) 
             
            self.soma.L = 9.76
            self.soma.diam = 9.76 + dprob
            self.soma.nseg = 1  # only one segment; every 
            self.soma.Ra = 100
            self.soma.cm = 1
    
            # Optional variability in diameter
            rng = h.Random(self.seed )
            rng.uniform(-0.2 * self.soma.diam, 0.2 * self.soma.diam)
            self.soma.diam += rng.repick()
    
            
            
     
        # Insert mechanisms
        def set_biophysics(self):
            
            for seg in self.soma:
                seg.ena = 87.39
                seg.eca = 129.33
                seg.ek = -88
                seg.gbar_GRANULE_PNA *= 1.25 
            
            for mech in self.mechanisms:
                #print(f"Mechanism: {mech}")
                self.soma.insert(mech)   
                # set the corresponding fix_celsius parameter on soma(0.5)
                attr_name = f"fix_celsius_{mech.upper()}"
                try:
                    setattr(self.soma(0.5), attr_name, self.fix_celsius)
                except AttributeError:
                    print(f"Mechanism {mech} does not have {attr_name} parameter")
                    
            
            syn = h.GrCAMPAplusNMDA(0.5, sec=self.soma)
            self.ampa.append(syn)
    
            gaba_syn = h.GrCGABAexp(0.5, sec=self.soma)
            self.configure_gaba(self.soma(0.5),gaba_syn)  # confiure in a segment
            self.gaba.append(gaba_syn)   
            
        def set_topology(self):
            # This is hard-coded as well; probably need to have a good logic for auto configuration
            # from a table in sqlite database?
            pass
                 
        def configure_condition(self):
            for seg in self.soma: 
                seg.ecl = seg.ek + 7
                seg.gbar_GRANULE_TONICCL = 0.00011639755259452132
                seg.ibias_GRANULE_TONICCL = -0.00566
                seg.gbar_GRANULE_KIR *= 1.5
     
        # Configure gaba on a segment
        def configure_gaba(self,seg,gaba_syn): 
            ecl = seg.ek + (7 if "adult" in self.condition else 17)
            gaba_syn.egaba = ecl
        
            if self.condition == "adult_wt":
                gaba_syn.tau1 = 0.2
                gaba_syn.tau2 = 9
                gaba_syn.tau3 = 81
                gaba_syn.c2 = 0.88319
        
            elif self.condition == "young_wt":
                gaba_syn.tau1 = 0.2
                gaba_syn.tau2 = 4.71
                gaba_syn.tau3 = 22.2
                gaba_syn.c2 = 0.64512
        
            elif self.condition == "adult_ko":
                gaba_syn.tau1 = 0.2
                gaba_syn.tau2 = 6.8
                gaba_syn.tau3 = 95
                gaba_syn.c2 = 0.86
        
            elif self.condition == "young_ko":
                gaba_syn.tau1 = 0.2
                gaba_syn.tau2 = 4.5
                gaba_syn.tau3 = 33
                gaba_syn.c2 = 0.58
    
             
        # Get the action potential count (may not be necessary; this is very limited rather  use pc.SpikeCount)
        def get_AP_count(self,section,threshold):
            super.action_potential_count(section=section,thresh=threshold)  # defines AP
            return int(self.AP.n)
        
         
    import numpy as np
    import matplotlib.pyplot as plt 
    from neuron import h 
    h.load_file('stdrun.hoc')
    
     
    class MossyFiber:
        def __init__(self, gid, seed=42, t_offset=2, noise=0.075, number=5,
                      spike_times=None, rate=None):
            self.gid = gid
            self.has_soma = False
     
            self.v = h.NetStim()   # Use NetStim for Poisson-like or regular spiking
            self.v.start = t_offset
            self.v.interval = 1000/rate if rate else 20  # ms
            self.v.noise = noise
            self.v.number = number  ## maximum number of spikes generated
            self.v.noiseFromRandom123(seed, 0, 0)
    
            self.spike_detector = self.connect2target(None)   # Spike detector
         
            # Internal recording mechanism
            self.spike_times = h.Vector()
            self.spike_detector.record(self.spike_times)
    
        def connect2target(self, target, thresh=0.5, delay=0.01):
            nc = h.NetCon(self.v, target)
            nc.threshold = thresh
            nc.delay = delay
            return nc
        
        
    ## For Poisson spike  
    rate = 20  # Hz
    mf = MossyFiber(gid=0, seed=41, t_offset=5, noise=1, number=100, rate=rate)
    
    h.tstop = 100   # Simulate for 10 seconds
    h.run()
    
    plt.vlines(list(mf.spike_times), 0, 1)
    plt.xlabel('Time (ms)')
    plt.yticks([])
    plt.title('Poisson Mossy Fiber Spike Raster')
    plt.show()
    
     
    
    class GolgiCell(CerebellumCell):
        def create_morphology(self):
            super().create_morphology()
      
    
        def insert_mechanisms(self):
            super().insert_mechanisms()
            # Add Golgicell-specific ion channels here
        
    
    class PurkinjeCell(CerebellumCell):
        def create_morphology(self):
            super().create_morphology()
      
    
        def insert_mechanisms(self):
            super().insert_mechanisms()
            # Add Purkinje-specific ion channels here
    
      
         
    
    class ClimbingFiber:
        
        def __init__(self):
            pass
        
        def properties(self):
            pass
        
        
    class ParallelFiber:
        
        def __init__(self):
            pass
        
        def properties(self):
            pass
    
     
    
    
     
    #GRANULE_LKG1: A generic leak conductance, representing non-specific background ion flow.
    #GRANULE_TONICCL: A tonic chloride conductance, often used to model persistent GABAergic inhibition via chloride channels.
    #GRANULE_NA: The fast voltage-gated sodium channel, responsible for action potential initiation.
    #GRANULE_NAR: The persistent (resurgent) sodium channel, which supports sustained or repetitive firing.
    
    #GRANULE_PNA: The persistent sodium current, a non-inactivating sodium conductance contributing to subthreshold depolarization and repetitive firing.
    #GRANULE_KV: A delayed rectifier potassium channel, responsible for repolarizing the membrane after action potentials.
    
    #GRANULE_KA: The A-type potassium channel, a fast transient K+ current that regulates excitability and firing patterns.
    
    #GRANULE_KIR: The inward rectifier potassium channel, stabilizes the resting membrane potential.
    
    #GRANULE_KCA: The calcium-activated potassium channel, links intracellular calcium levels to membrane repolarization.
    
    #GRANULE_KM: The M-type potassium channel, a slowly activating and deactivating current that modulates excitability.
    
    #GRANULE_CA: A voltage-gated calcium channel, mediates calcium influx during depolarization.
    
    #GRANULE_CALC: Likely represents calcium buffering or calcium dynamics within the cell.
    
    #GRANULE_Nmda_leak_linear: A linearized NMDA receptor-mediated current, representing a simplified model of NMDA receptor conductance.
    #It's a simplified NMDA receptor model.
    
    #The “linear” suggests the voltage-dependence and Mg
    #+ block of NMDA have been removed or approximated for efficiency (e.g., for use in large networks).
    #✔️ So yes, it's a linearized approximation of NMDA current.
    
    
            
        
    
        
    # =============================================================================
    #     def create_dend(self):  #
    #     
    #         if not hasattr(self,"dend"):  # will this work?  
    #              
    #             self.dend0=h.Section(name='dend',cell=self)
    #             self.dend0.nseg = 1
    #             self.dend0.diam = 3.18
    #             self.dend0.L = 701.9
    #             self.dend0.Ra = 123.0
    #             self.dend0.insert('pas')
    #             self.dend0.g_pas = 0.0001667
    #             self.dend0.e_pas = -60.0 
    #             
    #             self.dend1=h.Section(name='dend',cell=self)
    #     
    #             self.dend1.nseg = 5
    #             self.dend1.diam = 2.0
    #             self.dend1.L = 549.1
    #             self.dend1.Ra = 123.0
    #             self.dend1.insert('pas')
    #             self.dend1.g_pas = 0.0001667
    #             self.dend1.e_pas = -60.0
    #             
    #             # Connect them to soma
    #             self.dend0.connect(self.soma(0))
    #             self.dend1.connect(self.soma(1))  
    #             
    #             self.sections.extend([self.dend0,self.dend1])
    # =============================================================================
    
    # example 
    
    lst=[(0, 1070, 1, 0.0, 1, 0, 0, 0.5, 1, 0),
     (1, 1070, 1, 0.0, 1, 0, 0, 0.5, 0, 0),
     (2, 1070, 1, 0.0, 1, 0, 0, 0.5, 1, 0),
     (3, 1070, 1, 0.0, 1, 0, 0, 0.5, 0, 0)]
    
    
    
    def update_tuple_values(lst, d):
        updated_lst = []
        for tup in lst:
            key =  tup[6] 
            if key in d:
                candidates = d[key]
                closest = min(candidates, key=lambda x: abs(x - tup[7]))
                tup = tup[:7] + (closest,) + tup[8:]
            updated_lst.append(tup)
        return updated_lst
    
    def obtain_synaptic_segment(lst, d):
        result = []
        for tup in lst:
            key = tup[6]
            if key in d:
                candidates = d[key]
                closest = min(candidates, key=lambda x: abs(x - tup[7]))
                result.append(closest)
            else:
                result.append(tup[7])  # fallback: keep original if key not found
        return tuple(result)
    
    
     
    
    lst = [
        (0, 1070, 1, 0.0, 1, 0, 0, 0.5, 1, 0),
        (1, 1070, 1, 0.0, 1, 0, 0, 0.25, 0, 0),
        (2, 1070, 1, 0.0, 1, 0, 0, 0.32, 0, 0),
        (3, 1070, 1, 0.0, 1, 0, 1, 0.32, 0, 0),
        (3, 1070, 1, 0.0, 1, 0, 1, 0.9, 1, 0)
    ]
    
    d = {0: [0.25, 0.5, 0.75], 1: [0.6,0.3],2: [0.33, 0.66]}
    
    obtain_synaptic_segment(lst, d)
     
    
