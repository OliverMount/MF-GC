#import numpy as np
#import matplotlib.pyplot as plt
import sqlite3
import sys
import numpy as np
sys.path.append("src/mod")
sys.path.append("utils/")
from neuron import h, load_mechanisms 
import random
import csv

from methods import print_status
#load_mechanisms('/media/oli/Research/Gitrepo/GranularLayerModel/src/')
#load_mechanisms('/media/oli/Research/Gitrepo/GranularLayerModel/src/')
#load_mechanisms('/media/oli/Research/GitrepoO/core_granular_layer/src/')
 

h.load_file('stdrun.hoc')
 

# Parent cell
# gid should be registered seperately
# simply soma  with two dendrites


class CerebellumCell:
    def __init__(self, gid=None,seed=42,tstop=100,celsius=37):
        self.gid = gid
        self.seed=seed   
        self.sections = []  # list of sections useful for synapses
        self.synapses = []
        self.gap_junctions = []
        self.stimulus = None
        self.recording = {}   # Dictionary of with key t and v 
        self.spiketime = h.Vector()   # For recording 
        
        self.tstop = tstop           # Default run time
        self.fix_celsius = celsius       # Default temperature
          
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
    
    def connect2target(self, target, thresh=-20, delay=0.1):
        nc = h.NetCon(self.soma(0.5)._ref_v, target, sec=self.soma)
        nc.threshold = thresh
        nc.delay = delay
        return nc    
    # This will read the database and determine number of synapses needed     
    def determine_synaptic_connections(self,db_file,rank=None):   # pass the rank as well
        
        if self.sections: # If at least one section (soma by default exits)
            conn = sqlite3.connect(db_file, uri=True) 
            cursor = conn.cursor() 
            
            # logic for determining the synapse number 
            query = """SELECT source_gid, target_gid, weight, delay, syn_type, syn_dynamics, 
            target_dend_id, dend_syn_loc, need_single_syn, syn_id FROM synapse 
            WHERE target_gid = ? 
            ORDER by source_gid asc"""
            cursor.execute(query,(self.gid,))
            synapses = cursor.fetchall()  # list of tuples
             
            #print(synapses)
               
            # Get column names in order (so that we can refer to the index without hardcoding it)
            synapse_table_col_names = [desc[0] for desc in cursor.description]
            
            # Get the index of the synpase table columns to avoid hard coding
            source_gid_idx=synapse_table_col_names.index('source_gid')
            target_dend_id_idx = synapse_table_col_names.index('target_dend_id')
            dend_syn_loc_idx = synapse_table_col_names.index('dend_syn_loc')  
            need_single_syn_idx = synapse_table_col_names.index('need_single_syn')
            #syn_id_idx = synapse_table_col_names.index('syn_id')
            
            # Every time open and close it
            cursor.close()  
            conn.close()  # Do not close before cursor.description
             
            # log these variables
            #self.no_exhaustive_synapse=len(synapses)
            #self.no_different_synapses_requested=sum(1 for k in synapses if not k[-1]) 
            #self.no_same_synapse=1   # this will be always one  (do we need this?)
            #self.no_synapse_needed=self.no_same_synapse + self.no_different_synapses_requested
            
            if synapses:   # if synapse is not empty 
                #print("Inside the synapses")
                self.synapse_available={}  # dictionary of synapses available based on the database 
                
                # For each section determine the segment and assign dendrite x to the closest location
                for idx,sec in enumerate(self.sections):
                    self.synapse_available[idx]= [seg.x for seg in sec]    # idx is the id for targeting section
                
                # update synapse (locally in the cell) before assigning synaptic ids 
                segments=self.assign_synapse_to_closest_segment(synapses,target_dend_id_idx,dend_syn_loc_idx) 
                #print(segments)
                # the below line strictly needs the database order is there a way to deal with it ?
                synapses=  [syn[:dend_syn_loc_idx] + (seg,) + syn[(dend_syn_loc_idx+1):] for syn, seg in zip(synapses,segments)] 
                #print(synapses)
                 
                syn_id=self._assign_syn_ids(synapses,target_dend_id_idx,dend_syn_loc_idx,need_single_syn_idx)  # This assigns synaptic id based on single or multiple synapse
                #print("syn_id at a cell is ", (syn_id,self.gid))
                source_gid=[syn[source_gid_idx] for syn in synapses]   # 0 is the source id
                syn_id_source_gid= [(sy,so) for sy,so in zip(syn_id,source_gid)]
                
                #print("syn_id_source_gid", syn_id_source_gid)
                
                conn = sqlite3.connect(db_file) 
                cursor = conn.cursor() 
                
                # Add the synapse id for the  gid (target here)
                update_query = """UPDATE synapse  
                SET syn_id = ? 
                WHERE source_gid = ?"""
                cursor.executemany(update_query,syn_id_source_gid)  
                
                # Query again to create synapses in that order
                # Create list of synapses, that will be stored within the cell
                unique_syn_id=set(syn_id)
                for k in unique_syn_id:   # only for the unique ids
                    #print("Inisde the loop",(k,self.gid))
                    cursor.execute(
                        "SELECT syn_type, syn_dynamics, target_dend_id, dend_syn_loc FROM synapse WHERE syn_id = ? and target_gid = ?;",
                        (k,self.gid)
                    )
                    row = cursor.fetchone()  # fetchone as we need to have only one synapse per pattern  
                    #print("Row is ", row)
                    if row:
                        syn_type, syn_dynamics, target_dend_id, dend_syn_loc = row
                        self.synapses.append(
                            self.add_synapse(target_dend_id, dend_syn_loc, syn_type, syn_dynamics)
                        )
                # Close the db finally 
                cursor.close()
                conn.close()   
 
            else: # If no synapse exits then just pass or log it 
                print_status(f"ZERO synapse!!! at {self.gid}") 
        else:
            raise NotImplementedError("Cell sections cannot be empty; at least it should have soma")    
        # this function does not return anything; self.synapses contains the synaptic connection objects 
    # This will make sure all synapses are in the proper segment
    def assign_synapse_to_closest_segment(self,synapses,target_dend_id_idx,dend_syn_loc_idx):# this will make sure of self.synapse_available dictionary (keys are sections)
        # with the above idx hardcoding is avoided
        result = []
        for tup in synapses:
            key = tup[target_dend_id_idx]
            if key in self.synapse_available:
                candidates = self.synapse_available[key]
                closest = min(candidates, key=lambda x: abs(x - tup[dend_syn_loc_idx]))
                result.append(closest)
            else:
                result.append(tup[dend_syn_loc_idx])  # fallback: keep original if key not found
        return tuple(result)    # return only the segments 
    # Make it private function 
    def _assign_syn_ids(self,synapses,target_dend_id_idx,dend_syn_loc_idx,need_single_syn_idx):
        # with the above idx there is not need for hardcoding
        pattern_to_id = {}
        current_id = 0
        result = []
        for tup in synapses:
            if tup[need_single_syn_idx] == 0:  # this is the need_single_syn from the synapse database
                # Always assign a new unique ID for each occurrence where tup[8]==0
                result.append(current_id)
                current_id += 1
            else:
                pattern = (tup[target_dend_id_idx], tup[dend_syn_loc_idx], tup[need_single_syn_idx])
                if pattern not in pattern_to_id:
                    pattern_to_id[pattern] = current_id
                    current_id += 1
                result.append(pattern_to_id[pattern])
        return result 
        
    # right now supporting only linear synapse
    def add_synapse(self, section, loc=0.5, syn_type=1, syn_dynamics=0): 
        # make section mandatory argument
         
        if syn_dynamics == 0:     # Linear synapse
            syn = h.Exp2Syn(self.sections[section](loc)) # Create a synapse object # 'Exp2Syn'  
            # Excitatory or inhibitory
            if syn_type==1: 
                syn.tau1 = 0.28 
                syn.tau2= 1.23
                syn.e=0
            elif syn_type==0:
                syn.tau1 = 1.8
                syn.tau2 = 8.5
                syn.e=-65
            else:
                raise NotImplementedError(f"Unknown Synapse '{syn_type}'.")
                
        elif syn_dynamics == 1:  # non-linear synapse dynamics
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
    def extract_numeric_parameters(self, section=None, filename_prefix="cell_parameters"):
        
    
        if section is None:
            if hasattr(self, "soma"):
                section = self.soma
            else:
                raise ValueError("No section provided and no soma exists in cell.")
    
        param_list = []
    
        # Geometry
        param_list.extend([
            {"name": "L", "value": section.L, "symbol": "L", "type": "Geometry (µm)"},
            {"name": "diam", "value": section.diam, "symbol": "d", "type": "Geometry (µm)"},
            {"name": "nseg", "value": section.nseg, "symbol": "n_seg", "type": "Geometry"},
            {"name": "Ra", "value": section.Ra, "symbol": "R_a", "type": "Axial Resistance (Ω·cm)"},
            {"name": "cm", "value": section.cm, "symbol": "C_m", "type": "Membrane Capacitance (µF/cm²)"}
        ])
    
        # Reversal potentials
        for ion in ['na', 'k', 'ca', 'cl']:
            try:
                erev = getattr(section(0.5), f"e{ion}")
                param_list.append({
                    "name": f"e{ion}", "value": erev,
                    "symbol": f"E_{{{ion}}}", "type": f"Reversal Potential (mV)"
                })
            except Exception:
                continue
    
        # Density mechanisms
        psec = section.psection()
        for mech, params in psec.get("density_mechs", {}).items():
            for param, val in params.items():
                param_list.append({
                    "name": f"{param}_{mech}", "value": val,
                    "symbol": param, "type": f"Mechanism: {mech}"
                })
    
        # Point processes
        for seg in section:
            for obj in seg.point_processes():
                mech_name = obj.hname()
                for attr in dir(obj):
                    if attr.startswith("_") or attr in ["loc", "hname", "has_loc"]:
                        continue
                    try:
                        val = getattr(obj, attr)
                        if isinstance(val, (float, int)):
                            param_list.append({
                                "name": f"{attr}_{mech_name}",
                                "value": val,
                                "symbol": attr,
                                "type": f"Point Process: {mech_name}"
                            })
                    except Exception:
                        continue
    
        # --- CSV ---
        csv_file = filename_prefix + ".csv"
        with open(csv_file, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=["name", "value", "symbol", "type"])
            writer.writeheader()
            writer.writerows(param_list)
    
        # --- Markdown ---
        md_file = filename_prefix + ".md"
        with open(md_file, "w") as f:
            f.write("| Parameter Name | Value | Symbol | Type |\n")
            f.write("|----------------|-------|--------|------|\n")
            for p in param_list:
                f.write(f"| {p['name']} | {p['value']} | {p['symbol']} | {p['type']} |\n")
    
        # --- LaTeX ---
        tex_file = filename_prefix + ".tex"
        with open(tex_file, "w") as f:
            f.write("\\begin{tabular}{|l|r|l|l|}\n\\hline\n")
            f.write("Parameter Name & Value & Symbol & Type \\\\\n\\hline\n")
            for p in param_list:
                val = f"{p['value']:.6g}" if isinstance(p['value'], float) else p['value']
                f.write(f"{p['name']} & {val} & ${p['symbol']}$ & {p['type']} \\\\\n")
            f.write("\\hline\n\\end{tabular}\n")
    
        print(f"Saved to:\n- {csv_file}\n- {md_file}\n- {tex_file}")
 

class GranuleCell(CerebellumCell):
    def __init__(self, gid,seed=42,tstop=100,celsius=37):  
        super().__init__(gid,seed,tstop=tstop,celsius=celsius)  # creates soma with default morphology 
        
        self.mechanisms=self.get_mechanisms() 
        self.set_soma_geometry()  # soma is created in the parent
        self.set_soma_biophysics()
        
    # Mechanism specific for Granule cell
    def get_mechanisms(self):
        return [
            "GRANULE_LKG1", "GRANULE_TONICCL", "GRANULE_NA", "GRANULE_NAR",
            "GRANULE_PNA", "GRANULE_KV", "GRANULE_KA", "GRANULE_KIR",
            "GRANULE_KCA", "GRANULE_KM", "GRANULE_CA", "GRANULE_CALC",
            "GRANULE_Nmda_leak_linear" 
            ,"pas"
        ]
    
    def set_soma_geometry(self): 
        
        self.alph = h.Random(self.seed)  # seed in inttilaized when the cell is created
        dprob = self.alph.uniform(-9.76 * 0.2, 9.76 * 0.2) 
          
        L = 9.76
        diam = 9.76 + dprob
        nseg = 1  # only one segment; every 
        Ra = 100
        cm = 1

        # Optional variability in diameter
        rng = h.Random(self.seed)
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
        #self.soma.insert('hh')  
        for mech in self.mechanisms:
            #print(f"Mechanism: {mech}")
            self.soma.insert(mech)   
            # set the corresponding fix_celsius parameter on soma(0.5)
            attr_name = f"fix_celsius_{mech.upper()}"
            try:
                setattr(self.soma(0.5), attr_name, self.fix_celsius)
            except AttributeError:  # log it 
                pass
                #print(f"Mechanism {mech} does not have {attr_name} parameter")
                
        for seg in self.soma:
            seg.ena = 87.39
            seg.eca = 129.33
            seg.ek = -88
            seg.gbar_GRANULE_PNA *= 1.25 
            
            # for passive 
            seg.g_pas=0.0047     # Adjust this 
            seg.e_pas=-80.53   # based on real measurements of inhibition 
        

    def get_rate(self): 
        
        tstop = self.tstop

        num_spikes = len(self.spiketime)
        duration_sec = tstop / 1000.0  # Convert ms to seconds

        if duration_sec == 0:
            return 0.0 
         
        return num_spikes / duration_sec  # Rate= spikes/sec

        
            
# =============================================================================
# class MossyFiber:
#     def __init__(self, gid, seed=42, t_offset=2, noise=0.075, number=5, rate=None):
#         self.gid = gid
#         self.has_soma = False
#  
#         self.v = h.NetStim()   # Use NetStim for Poisson-like or regular spiking
#         self.v.start = t_offset
#         self.v.interval = 1000.0 / rate if rate else 20  # ms
#         self.v.noise = noise
#         self.v.number = number
#         self.v.noiseFromRandom123(seed, 0, 0)
# 
#         #self.spike_detector = self.connect2target(None)   # Spike detector
#      
#         # Internal recording mechanism
#         #self.spike_times = h.Vector()
#         #self.spike_detector.record(self.spike_times)
# 
#     def connect2target(self, target, thresh=0.5, delay=0.01):
#         nc = h.NetCon(self.v, target)
#         nc.threshold = thresh
#         nc.delay = delay
#         return nc
# =============================================================================
    
#A=GranuleCell(1070)
#A.determine_synaptic_connections('connectivity/mf_grc.db')  


class MossyFiber1:
    def __init__(self, gid, seed=42, t_offset=5, noise=0.075, number=5, rate=20, tstop=100):
        self.gid = gid
        self.has_soma = False 
        self.tstop=tstop 
       
        interval = 1000 / rate  
        duration = tstop - t_offset
        max_possible_spikes = int(duration // interval)
        #print(max_possible_spikes)
        if max_possible_spikes < 1:
            raise ValueError(f"Rate too low: Cannot generate 1 spike in {duration} ms. (required rate ≥ {1000.0 / duration:.2f} Hz)")
   
        self.v = h.NetStim()
        self.v.start = t_offset
        self.v.interval = interval
        self.v.noise = noise
        self.v.number = number
        self.v.noiseFromRandom123(seed, 0, 0)

    def connect2target(self, target, thresh=0.5, delay=0.01):
        nc = h.NetCon(self.v, target)
        nc.threshold = thresh
        nc.delay = delay
        return nc  
     

class MossyFiber:
    def __init__(self, gid, seed=42, t_offset=5, noise=0.075, number=1, rate_needed=20, 
                 tstop=500, mode="random", custom_spike_times=None):
        self.gid = gid
        self.has_soma = False
        self.tstop = tstop
        self.mode = mode
        self.seed=seed 
        self.spike_times=h.Vector()  # will be used in the custom mode

        if mode == "poisson":
            interval = 1000 / rate_needed
            duration = tstop - t_offset
            max_possible_spikes = int(duration // interval)

            if max_possible_spikes < 1:
                raise ValueError(f"Rate too low: Cannot generate 1 spike in {duration} ms.")
 
            self.v = h.NetStim()
            self.v.start = t_offset
            self.v.interval = interval
            self.v.noise = noise
            self.v.number = number
            self.v.noiseFromRandom123(self.seed, 0, 0)

        elif mode == "random":    
            
            # Create a separate random number generator for this MossyFiber
            rng = np.random.RandomState(self.seed)
            
            # Generate N random spike times uniformly between t_offset and tstop 
            duration = tstop - t_offset
            max_possible_spikes = int(np.ceil(rate_needed * (duration/1000)))   # Interval in sec
            #print(max_possible_spikes)
            min_possible_rate=((1*1000)/tstop)    # spike in the given duration

            if max_possible_spikes < 1:
                raise ValueError(f"Rate is too low: Cannot generate 1 spike in {duration} ms (for which rate should be strickly greater than {min_possible_rate} Hz")
              
            spike_times = sorted(rng.uniform(t_offset, tstop, max_possible_spikes))
            self.spike_times.from_python(spike_times)
            self.v=h.VecStim() 
            self.v.play(self.spike_times) 
 
        elif mode == "custom": 
            if custom_spike_times is not None:
                self.spike_times.from_python(sorted(custom_spike_times))
                self.v = h.VecStim()
                self.v.play(self.spike_times)
                
            else:
                raise ValueError(f"custom_spike_times cannot be empty or None for custom mode")
             
        else:
            raise ValueError(f"Unknown mode: {mode}. Use 'poisson','random' or 'custom'. ")

    def connect2target(self, target, thresh=0.5, delay=0.01):
        nc = h.NetCon(self.v, target)  #  Here the NetCon first parameter is a point process unlike membrance potential
        nc.threshold = thresh
        nc.delay = delay
        return nc 
