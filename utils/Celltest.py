import numpy as np
import matplotlib.pyplot as plt
import sqlite3
import sys
sys.path.append("src/mod")
from neuron import h, load_mechanisms 
load_mechanisms('/media/oli/Research/GitrepoO/core_granular_layer/src/')

h.load_file('stdrun.hoc')

# Parent cell
# gid should be registered seperately
# simply soma  with two dendrites 

class CerebellumCell:
    def __init__(self, gid=None,seed=42):
        self.gid = gid
        self.seed=seed   
        self.sections = []  # list of sections useful for synapses
        self.synapses = []
        self.gap_junctions = []
        self.stimulus = None
        self.recording = {}   # Dictionary of with key t and v 
        self.spiketime = h.Vector()   # For recording 
        
        self.tstop = 100            # Default run time
        self.fix_celsius = 37       # Default temperature
          
        # Default one some (with hh) and two dendtrites (pas)
        self.create_soma()  # creates soma and adds to sections
        #self.create_dend()  # no dendries
        
        
    def create_soma(self):  # creates soma and adds to sections
        if not hasattr(self,"soma"):
            self.soma = h.Section(name='soma',cell=self)
            self.soma.L = 30
            self.soma.diam = 30 
            self.sections.append(self.soma) 
    
    def set_soma_properties(self,para):  # para should be dictionary of parameters
        if not hasattr(self, "soma"):
            raise AttributeError("Soma has not been created yet.")
            
        for key, value in para.items():
            setattr(self.soma, key, value) 
    
    def connect2target(self, target, thresh=-30, delay=0.1):
        nc = h.NetCon(self.soma(0.5)._ref_v, target, sec=self.soma)
        nc.threshold = thresh
        nc.delay = delay
        return nc    
         
    # This will read the database and determine number of synapses needed     
    def determine_synaptic_connections(self,db_file): 
        
        if not self.sections: # If at least one section (soma by default exits)
            conn = sqlite3.connect(db_file, uri=True) 
            cursor = conn.cursor() 
            
            # logic for determining the synapse number 
            query = """SELECT source_gid, target_gid, weight, delay, syn_type, syn_dynamics, 
            target_dend_id, dend_syn_loc, need_single_syn, syn_id FROM synapse 
            WHERE target_gid = ? 
            ORDER by source_gid asc"""
            cursor.execute(query,(self.gid,))
            synapses = cursor.fetchall()  # list of tuples
              
            # log these variables
            self.no_exhaustive_synapse=len(synapses)
            self.no_different_synapses_requested=sum(1 for k in synapses if not k[-1]) 
            self.no_same_synapse=1   # this will be always one  (do we need this?)
            self.no_synapse_needed=self.no_same_synapse + self.no_different_synapses_requested
            
            if not synapses:   # if synapse is not empty 
                self.synapse_available={}  # dictionary of synapses available based on the database 
                
                # For each section determine the segment and assign dendrite x to the closest location
                for idx,sec in enumerate(self.sections):
                    self.synapse_available[idx]= [seg.x for seg in sec]    # idx is the id for targeting section
                    
                # Determine here how many synapses needed based on the synapses list
                # counting number of zeros in the last column of synapse
       
                # Check for same dendrite id and loc and need_single synapse
                syn_id=self._assign_syn_ids(synapses)
                source_gid=[syn[0] for syn in synapses]   # 0 is the source id
                syn_id_source_gid= [(sy,so) for sy,so in zip(syn_id,source_gid)]
                
                # Add the synapse id for the  gid (target here)
                update_query = """UPDATE synapse  
                SET syn_id = ? 
                WHERE source_gid = ?"""
                cursor.executemany(update_query,syn_id_source_gid)  
                
                # Query again to create synapses in that order
                # Create list of synapses, that will be stored within the cell
                for k in np.unique(syn_id):   # only for the unique ids
                    cursor.execute(
                        "SELECT syn_type, syn_dynamics, target_dend_id, dend_syn_loc FROM synapse WHERE last_column = ? LIMIT 1;",
                        (k,)
                    )
                    row = cursor.fetchone()
                    if row:
                        syn_type, syn_dynamics, target_dend_id, dend_syn_loc = row
                        self.synapses.append(
                            self.add_synapse(target_dend_id, dend_syn_loc, syn_type, syn_dynamics)
                        )
 
            else: # If no synapse exits then just pass or log it 
                pass        
                     
                    
        else:
            raise NotImplementedError("Cell sections cannot be empty; at least it should have soma")
            
            
        # Close the db finally 
        cursor.close()
        conn.close()  
        
        # this function does not return anything; self.synapses contains the synaptic connection objects 
        
    # Make it private function
    def _assign_syn_ids(self,lst):
        pattern_to_id = {}
        current_id = 0
        result = []
        for tup in lst:
            pattern = (tup[6], tup[7], tup[8])  #target_dend_id, dend_syn_loc, requsted same synapse
            if pattern not in pattern_to_id:
                pattern_to_id[pattern] = current_id
                current_id += 1
            result.append(pattern_to_id[pattern])
        return tuple(result) 
        
    # right now supporting only linear synapse
    def add_synapse(self, section, loc=0.5, syn_type=0, syn_dynamics=0): 
        # make section mandatory argument
         
        if syn_dynamics == 0:  # 'Exp2Syn'  
            # Create a synapse object
            syn = h.Exp2Syn(self.sections[section](loc)) 
            # Excitatory or inhibitory
            if syn_type==0: 
                syn.tau1 = 0.28 
                syn.tau2= 1.23
                syn.e=0
            elif syn_type==1:
                syn.tau1 = 1.8
                syn.tau2 = 8.5
                syn.e=-65
            else:
                raise NotImplementedError(f"Unknow Synapse '{syn_type}'.")
                
        elif syn_dynamics == 1:  # Implement non-linear synapse
            pass
        else:
            raise NotImplementedError(f"Synapse dynamics'{syn_dynamics}' not supported.")  
            
        return syn  # this will be used by gc.gid2connect(gid,syn)  
    
    def describe_cell(self, summary_type='dict'): 
        
        if hasattr(self,"soma"):
        
            if summary_type == 'dict':
                summary = {
                    'gid': self.gid,
                    'soma': {
                        'L': self.soma.L,
                        'diam': self.soma.diam
                    },
                    'num_dendrites': len([sec for sec in self.sections if sec != self.soma]),
                    'sections': []
                }
    
                for sec in self.sections:
                    sec_type = "soma" if sec == self.soma else "dendrite"
                    info = sec.psection()
    
                    density_mechs = list(info.get('density_mechs', {}).keys())
                    point_procs = [pp.__class__.__name__ for pp in info.get('point_processes', [])]
                    ions = list(info.get('ions', {}).keys())
    
                    sec_summary = {
                        'name': sec.name(),
                        'type': sec_type,
                        'density_mechanisms': density_mechs,
                        'point_processes': point_procs,
                        'ion_mechanisms': ions
                    }
    
                    summary['sections'].append(sec_summary)
    
                return summary
            else:
                print("Cell Description:")
                print(f"gid = {self.gid}")
                print(f"  Soma: present (L={self.soma.L}, diam={self.soma.diam})")
    
                dendrites = [sec for sec in self.sections if sec != self.soma]
                print(f"  Number of dendrites: {len(dendrites)}\n")
    
                print("Mechanisms per section:")
                for sec in self.sections:
                    sec_type = "soma" if sec == self.soma else "dendrite"
                    print(f"  Section '{sec.name()}' ({sec_type}):")
    
                    info = sec.psection()
    
                    density_mechs = info.get('density_mechs', {})
                    print(f"    Density mechanisms: {list(density_mechs.keys()) if density_mechs else 'None'}")
    
                    point_procs = info.get('point_processes', {})
                    pp_list = [pp.__class__.__name__ for pp in point_procs]
                    print(f"    Point processes: {pp_list if point_procs else 'None'}")
    
                    ions = info.get('ions', {})
                    print(f"    Ion mechanisms: {list(ions.keys()) if ions else 'None'}")
                    print()
        else:
            print("++ Simple cell with NO soma!!")


class GranuleCell(CerebellumCell):
    def __init__(self, gid,seed=42):  
        super().__init__(gid,seed)  # creates soma with default morphology 
        
        self.mechanisms=self.get_mechanisms() 
        self.set_soma_geometry()
        self.set_soma_biophysics()
        
    # Mechanism specific for Granule cell
    def get_mechanisms(self):
        return [
            "GRANULE_LKG1", "GRANULE_TONICCL", "GRANULE_NA", "GRANULE_NAR",
            "GRANULE_PNA", "GRANULE_KV", "GRANULE_KA", "GRANULE_KIR",
            "GRANULE_KCA", "GRANULE_KM", "GRANULE_CA", "GRANULE_CALC",
            "GRANULE_Nmda_leak_linear"
        ]
    
    def set_soma_geometry(self):
                
        self.seed = 42
        self.alph = h.Random(self.seed)
        dprob = self.alph.uniform(-9.76 * 0.2, 9.76 * 0.2) 
          
        L = 9.76
        diam = 9.76 + dprob
        nseg = 1  # only one segment; every 
        Ra = 100
        cm = 1

        # Optional variability in diameter
        rng = h.Random(self.seed )
        rng.uniform(-0.2 * self.soma.diam, 0.2 * self.soma.diam)
        diam += rng.repick()
         
        prop={'L' : L,
              'diam' : diam,
              'nseg' : nseg,
              'Ra' : Ra,
              'cm' : cm
              }
         
        self.set_soma_properties(prop)  # this will set the 
        
    # Insert mechanisms
    def set_soma_biophysics(self): 
        
        for mech in self.mechanisms:
            #print(f"Mechanism: {mech}")
            self.soma.insert(mech)   
            # set the corresponding fix_celsius parameter on soma(0.5)
            attr_name = f"fix_celsius_{mech.upper()}"
            try:
                setattr(self.soma(0.5), attr_name, self.fix_celsius)
            except AttributeError:
                print(f"Mechanism {mech} does not have {attr_name} parameter")
                
        for seg in self.soma:
            seg.ena = 87.39
            seg.eca = 129.33
            seg.ek = -88
            seg.gbar_GRANULE_PNA *= 1.25 


 
class MossyFiber:
    def __init__(self, gid, seed=42, t_offset=2, noise=0.075, number=5,
                  spike_times=None, rate=None):
        self.gid = gid
        self.has_soma = False
 
        self.v = h.NetStim()   # Use NetStim for Poisson-like or regular spiking
        self.v.start = t_offset
        self.v.interval = 1000.0 / rate if rate else 1.5  # ms
        self.v.noise = noise
        self.v.number = number
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