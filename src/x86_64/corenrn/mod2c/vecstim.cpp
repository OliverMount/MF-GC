/*********************************************************
Model Name      : VecStim
Filename        : vecstim.mod
NMODL Version   : 7.7.0
Vectorized      : true
Threadsafe      : true
Created         : Thu Jul  3 15:36:17 2025
Simulator       : CoreNEURON
Backend         : C++-OpenAcc (api-compatibility)
NMODL Compiler  : 9.0.0a0.post1449 []
*********************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coreneuron/utils/offload.hpp>
#include <cuda_runtime_api.h>

#include <coreneuron/gpu/nrn_acc_manager.hpp>
#include <coreneuron/mechanism/mech/mod2c_core_thread.hpp>
#include <coreneuron/mechanism/register_mech.hpp>
#include <coreneuron/nrnconf.h>
#include <coreneuron/nrniv/nrniv_decl.h>
#include <coreneuron/sim/multicore.hpp>
#include <coreneuron/sim/scopmath/newton_thread.hpp>
#include <coreneuron/utils/ivocvect.hpp>
#include <coreneuron/utils/nrnoc_aux.hpp>
#include <coreneuron/utils/randoms/nrnran123.h>


namespace coreneuron {
    #ifndef NRN_PRCELLSTATE
    #define NRN_PRCELLSTATE 0
    #endif


    /** channel information */
    static const char *mechanism_info[] = {
        "7.7.0",
        "VecStim",
        0,
        0,
        0,
        "ptr",
        0
    };


    /** all global variables */
    struct VecStim_Store {
        int point_type{};
        int reset{};
        int mech_type{};
    };
    static_assert(std::is_trivially_copy_constructible_v<VecStim_Store>);
    static_assert(std::is_trivially_move_constructible_v<VecStim_Store>);
    static_assert(std::is_trivially_copy_assignable_v<VecStim_Store>);
    static_assert(std::is_trivially_move_assignable_v<VecStim_Store>);
    static_assert(std::is_trivially_destructible_v<VecStim_Store>);
    static VecStim_Store VecStim_global;


    /** all mechanism instance variables and global variables */
    struct VecStim_Instance  {
        double* index{};
        double* etime{};
        double* v_unused{};
        double* tsave{};
        const double* node_area{};
        void** point_process{};
        void** ptr{};
        void** tqitem{};
        VecStim_Store* global{&VecStim_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {nullptr, nullptr}
    };


    /** connect global (array) variables to hoc -- */
    static DoubVec hoc_vector_double[] = {
        {nullptr, nullptr, 0}
    };


    static inline int first_pointer_var_index() {
        return 2;
    }


    static inline int first_random_var_index() {
        return -1;
    }


    static inline int num_net_receive_args() {
        return 1;
    }


    static inline int float_variables_size() {
        return 4;
    }


    static inline int int_variables_size() {
        return 4;
    }


    static inline int get_mech_type() {
        return VecStim_global.mech_type;
    }


    static inline Memb_list* get_memb_list(NrnThread* nt) {
        if (!nt->_ml_list) {
            return nullptr;
        }
        return nt->_ml_list[get_mech_type()];
    }


    static inline void* mem_alloc(size_t num, size_t size, size_t alignment = 64) {
        size_t aligned_size = ((num*size + alignment - 1) / alignment) * alignment;
        void* ptr = aligned_alloc(alignment, aligned_size);
        memset(ptr, 0, aligned_size);
        return ptr;
    }


    static inline void mem_free(void* ptr) {
        free(ptr);
    }


    static inline void coreneuron_abort() {
        printf("Error : Issue while running OpenACC kernel \n");
        assert(0==1);
    }

    // Allocate instance structure
    static void nrn_private_constructor_VecStim(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new VecStim_Instance{};
        assert(inst->global == &VecStim_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(VecStim_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_VecStim(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<VecStim_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &VecStim_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(VecStim_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<VecStim_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &VecStim_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(VecStim_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->index = ml->data+0*pnodecount;
        inst->etime = ml->data+1*pnodecount;
        inst->v_unused = ml->data+2*pnodecount;
        inst->tsave = ml->data+3*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = nt->_vdata;
        inst->ptr = nt->_vdata;
        inst->tqitem = nt->_vdata;
    }



    static void nrn_alloc_VecStim(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_VecStim(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<VecStim_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_VecStim(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<VecStim_Instance*>(ml->instance);

        // VERBATIM
        #if !NRNBBCORE
        	void* vv = (void*)(inst->ptr[indexes[2*pnodecount + id]]);  
                if (vv) {
        		hoc_obj_unref(*vector_pobj(vv));
        	}
        #endif
        // ENDVERBATIM

        #endif
    }


    inline static int element_VecStim(int id, int pnodecount, VecStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline static int play_VecStim(int id, int pnodecount, VecStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


// VERBATIM
static void bbcore_write(double* xarray, int* iarray, int* xoffset, int* ioffset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
  int i, dsize, *ia;
  double *xa, *dv;
  dsize = 0;
  if (nt->_vdata[indexes[2*pnodecount + id]]) {
    dsize = vector_capacity(nt->_vdata[indexes[2*pnodecount + id]]);
  }
  if (iarray) {
    void* vec = nt->_vdata[indexes[2*pnodecount + id]];
    ia = iarray + *ioffset;
    xa = xarray + *xoffset;
    ia[0] = dsize;
    if (dsize) {
      dv = vector_vec(vec);
      for (i = 0; i < dsize; ++i) {
         xa[i] = dv[i];
      }
    }
  }
  *ioffset += 1;
  *xoffset += dsize;
}
static void bbcore_read(double* xarray, int* iarray, int* xoffset, int* ioffset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
  int dsize, i, *ia;
  double *xa, *dv;
  xa = xarray + *xoffset;
  ia = iarray + *ioffset;
  dsize = ia[0];
  if (!nt->_vdata[indexes[2*pnodecount + id]]) {
    nt->_vdata[indexes[2*pnodecount + id]] = vector_new1(dsize);
  }
  assert(dsize == vector_capacity(nt->_vdata[indexes[2*pnodecount + id]]));
  dv = vector_vec(nt->_vdata[indexes[2*pnodecount + id]]);
  for (i = 0; i < dsize; ++i) {
    dv[i] = xa[i];
  }
  *xoffset += dsize;
  *ioffset += 1;
}
// ENDVERBATIM


namespace coreneuron {


    inline int element_VecStim(int id, int pnodecount, VecStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_element = 0;
        // VERBATIM
        	
          { void* vv; int i, size; double* px;
        	i = (int)inst->index[id];
        	if (i >= 0) {
        		vv = (void*)(inst->ptr[indexes[2*pnodecount + id]]);
        		if (vv) {
        			size = vector_capacity(vv);
        			px = vector_vec(vv);
        			if (i < size) {
        				inst->etime[id] = px[i];
        				inst->index[id] += 1.;
        			}else{
        				inst->index[id] = -1.;
        			}
        		}else{
        			inst->index[id] = -1.;
        		}
        	}
          }
        // ENDVERBATIM

        return ret_element;
    }


    inline int play_VecStim(int id, int pnodecount, VecStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_play = 0;
        // VERBATIM
        #if !NRNBBCORE
          {
        	void** pv;
        	void* ptmp = NULL;
        	if (ifarg(1)) {
        		ptmp = vector_arg(1);
        		hoc_obj_ref(*vector_pobj(ptmp));
        	}
        	pv = (void**)(&inst->ptr[indexes[2*pnodecount + id]]);
        	if (*pv) {
        		hoc_obj_unref(*vector_pobj(*pv));
        	}
        	*pv = ptmp;
          }
        #endif
        // ENDVERBATIM

        return ret_play;
    }


    static inline void net_receive_VecStim(Point_process* pnt, int weight_index, double flag) {
        int tid = pnt->_tid;
        int id = pnt->_i_instance;
        double v = 0;
        NrnThread* nt = nrn_threads + tid;
        Memb_list* ml = nt->_ml_list[pnt->_type];
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        double* data = ml->data;
        double* weights = nt->weights;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<VecStim_Instance*>(ml->instance);

        double t = nt->_t;
        inst->tsave[id] = t;
        {
            if (flag == 1.0) {
                net_event(pnt, t);
                {
                    // VERBATIM
                    	
                      { void* vv; int i, size; double* px;
                    	i = (int)inst->index[id];
                    	if (i >= 0) {
                    		vv = (void*)(inst->ptr[indexes[2*pnodecount + id]]);
                    		if (vv) {
                    			size = vector_capacity(vv);
                    			px = vector_vec(vv);
                    			if (i < size) {
                    				inst->etime[id] = px[i];
                    				inst->index[id] += 1.;
                    			}else{
                    				inst->index[id] = -1.;
                    			}
                    		}else{
                    			inst->index[id] = -1.;
                    		}
                    	}
                      }
                    // ENDVERBATIM

                }
                if (inst->index[id] > 0.0) {
                    artcell_net_send(&inst->tqitem[indexes[3*pnodecount + id]], weight_index, pnt, nt->_t+inst->etime[id] - t, 1.0);
                }
            }
        }
    }


    /** initialize channel */
    void nrn_init_VecStim(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<VecStim_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            for (int id = 0; id < nodecount; id++) {
                inst->tsave[id] = -1e20;
                double v = 0.0;
                inst->index[id] = 0.0;
                {
                    // VERBATIM
                    	
                      { void* vv; int i, size; double* px;
                    	i = (int)inst->index[id];
                    	if (i >= 0) {
                    		vv = (void*)(inst->ptr[indexes[2*pnodecount + id]]);
                    		if (vv) {
                    			size = vector_capacity(vv);
                    			px = vector_vec(vv);
                    			if (i < size) {
                    				inst->etime[id] = px[i];
                    				inst->index[id] += 1.;
                    			}else{
                    				inst->index[id] = -1.;
                    			}
                    		}else{
                    			inst->index[id] = -1.;
                    		}
                    	}
                      }
                    // ENDVERBATIM

                }
                if (inst->index[id] > 0.0) {
                    artcell_net_send(&inst->tqitem[indexes[3*pnodecount + id]], 0, (Point_process*)inst->point_process[indexes[1*pnodecount + id]], nt->_t+inst->etime[id] - nt->_t, 1.0);
                }
            }
        }
    }


    /** register channel with the simulator */
    void _vecstim_reg() {

        int mech_type = nrn_get_mechtype("VecStim");
        VecStim_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_VecStim, nullptr, nullptr, nullptr, nrn_init_VecStim, nrn_private_constructor_VecStim, nrn_private_destructor_VecStim, first_pointer_var_index(), nullptr, nrn_destructor_VecStim, 1);

        hoc_reg_bbcore_read(mech_type, bbcore_read);
        hoc_reg_bbcore_write(mech_type, bbcore_write);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_dparam_semantics(mech_type, 2, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 3, "netsend");
        add_nrn_has_net_event(mech_type);
        add_nrn_artcell(mech_type, 3);
        set_pnt_receive(mech_type, net_receive_VecStim, nullptr, num_net_receive_args());
        hoc_register_net_send_buffering(mech_type);
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
