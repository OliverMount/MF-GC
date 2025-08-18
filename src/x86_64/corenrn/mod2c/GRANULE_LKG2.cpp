/*********************************************************
Model Name      : GRANULE_TONICCL
Filename        : GRANULE_LKG2.mod
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
        "GRANULE_TONICCL",
        "gbar_GRANULE_TONICCL",
        "Q10_diff_GRANULE_TONICCL",
        "fix_celsius_GRANULE_TONICCL",
        "egaba_GRANULE_TONICCL",
        "ibias_GRANULE_TONICCL",
        0,
        "il_GRANULE_TONICCL",
        "g_GRANULE_TONICCL",
        "gbar_Q10_GRANULE_TONICCL",
        0,
        0,
        0
    };


    /** all global variables */
    struct GRANULE_TONICCL_Store {
        int reset{};
        int mech_type{};
    };
    static_assert(std::is_trivially_copy_constructible_v<GRANULE_TONICCL_Store>);
    static_assert(std::is_trivially_move_constructible_v<GRANULE_TONICCL_Store>);
    static_assert(std::is_trivially_copy_assignable_v<GRANULE_TONICCL_Store>);
    static_assert(std::is_trivially_move_assignable_v<GRANULE_TONICCL_Store>);
    static_assert(std::is_trivially_destructible_v<GRANULE_TONICCL_Store>);
    static GRANULE_TONICCL_Store GRANULE_TONICCL_global;


    /** all mechanism instance variables and global variables */
    struct GRANULE_TONICCL_Instance  {
        const double* gbar{};
        const double* Q10_diff{};
        const double* fix_celsius{};
        const double* egaba{};
        const double* ibias{};
        double* il{};
        double* g{};
        double* gbar_Q10{};
        double* v_unused{};
        double* g_unused{};
        GRANULE_TONICCL_Store* global{&GRANULE_TONICCL_global};
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
        return -1;
    }


    static inline int first_random_var_index() {
        return -1;
    }


    static inline int float_variables_size() {
        return 10;
    }


    static inline int int_variables_size() {
        return 0;
    }


    static inline int get_mech_type() {
        return GRANULE_TONICCL_global.mech_type;
    }


    static inline Memb_list* get_memb_list(NrnThread* nt) {
        if (!nt->_ml_list) {
            return nullptr;
        }
        return nt->_ml_list[get_mech_type()];
    }


    static inline void* mem_alloc(size_t num, size_t size, size_t alignment = 16) {
        void* ptr;
        cudaMallocManaged(&ptr, num*size);
        cudaMemset(ptr, 0, num*size);
        return ptr;
    }


    static inline void mem_free(void* ptr) {
        cudaFree(ptr);
    }


    static inline void coreneuron_abort() {
        printf("Error : Issue while running OpenACC kernel \n");
        assert(0==1);
    }

    // Allocate instance structure
    static void nrn_private_constructor_GRANULE_TONICCL(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new GRANULE_TONICCL_Instance{};
        assert(inst->global == &GRANULE_TONICCL_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(GRANULE_TONICCL_Store);
    }

    static inline void copy_instance_to_device(NrnThread* nt, Memb_list* ml, GRANULE_TONICCL_Instance const* inst);
    static inline void delete_instance_from_device(GRANULE_TONICCL_Instance* inst);
    // Deallocate the instance structure
    static void nrn_private_destructor_GRANULE_TONICCL(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<GRANULE_TONICCL_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GRANULE_TONICCL_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GRANULE_TONICCL_Store));
        delete_instance_from_device(inst);
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<GRANULE_TONICCL_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GRANULE_TONICCL_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GRANULE_TONICCL_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gbar = ml->data+0*pnodecount;
        inst->Q10_diff = ml->data+1*pnodecount;
        inst->fix_celsius = ml->data+2*pnodecount;
        inst->egaba = ml->data+3*pnodecount;
        inst->ibias = ml->data+4*pnodecount;
        inst->il = ml->data+5*pnodecount;
        inst->g = ml->data+6*pnodecount;
        inst->gbar_Q10 = ml->data+7*pnodecount;
        inst->v_unused = ml->data+8*pnodecount;
        inst->g_unused = ml->data+9*pnodecount;
        copy_instance_to_device(nt, ml, inst);
    }

    static inline void copy_instance_to_device(NrnThread* nt, Memb_list* ml, GRANULE_TONICCL_Instance const* inst) {
        if (!nt->compute_gpu) {
            return;
        }
        auto tmp = *inst;
        auto* d_inst = cnrn_target_is_present(inst);
        if (!d_inst) {
            d_inst = cnrn_target_copyin(inst);
        }
        tmp.global = cnrn_target_deviceptr(tmp.global);
        tmp.gbar = cnrn_target_deviceptr(tmp.gbar);
        tmp.Q10_diff = cnrn_target_deviceptr(tmp.Q10_diff);
        tmp.fix_celsius = cnrn_target_deviceptr(tmp.fix_celsius);
        tmp.egaba = cnrn_target_deviceptr(tmp.egaba);
        tmp.ibias = cnrn_target_deviceptr(tmp.ibias);
        tmp.il = cnrn_target_deviceptr(tmp.il);
        tmp.g = cnrn_target_deviceptr(tmp.g);
        tmp.gbar_Q10 = cnrn_target_deviceptr(tmp.gbar_Q10);
        tmp.v_unused = cnrn_target_deviceptr(tmp.v_unused);
        tmp.g_unused = cnrn_target_deviceptr(tmp.g_unused);
        cnrn_target_memcpy_to_device(d_inst, &tmp);
        auto* d_ml = cnrn_target_deviceptr(ml);
        void* d_inst_void = d_inst;
        cnrn_target_memcpy_to_device(&(d_ml->instance), &d_inst_void);
    }

    static inline void delete_instance_from_device(GRANULE_TONICCL_Instance* inst) {
        if (cnrn_target_is_present(inst)) {
            cnrn_target_delete(inst);
        }
    }



    static void nrn_alloc_GRANULE_TONICCL(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_GRANULE_TONICCL(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GRANULE_TONICCL_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_GRANULE_TONICCL(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GRANULE_TONICCL_Instance*>(ml->instance);

        #endif
    }


    /** initialize channel */
    void nrn_init_GRANULE_TONICCL(NrnThread* nt, Memb_list* ml, int type) {
        nrn_pragma_acc(data present(nt, ml) if(nt->compute_gpu))
        {
            int nodecount = ml->nodecount;
            int pnodecount = ml->_nodecount_padded;
            const int* node_index = ml->nodeindices;
            double* data = ml->data;
            const double* voltage = nt->_actual_v;
            Datum* indexes = ml->pdata;
            ThreadDatum* thread = ml->_thread;

            setup_instance(nt, ml);
            auto* const inst = static_cast<GRANULE_TONICCL_Instance*>(ml->instance);

            if (nt->compute_gpu) {
                nrn_pragma_acc(update device (GRANULE_TONICCL_global))
                nrn_pragma_omp(target update to(GRANULE_TONICCL_global))
            }
            if (_nrn_skip_initmodel == 0) {
                nrn_pragma_acc(parallel loop present(inst, node_index, data, voltage, indexes, thread) async(nt->stream_id) if(nt->compute_gpu))
                nrn_pragma_omp(target teams distribute parallel for if(nt->compute_gpu))
                for (int id = 0; id < nodecount; id++) {
                    int node_id = node_index[id];
                    double v = voltage[node_id];
                    #if NRN_PRCELLSTATE
                    inst->v_unused[id] = v;
                    #endif
                    inst->gbar_Q10[id] = inst->gbar[id];
                    inst->g[id] = inst->gbar_Q10[id];
                }
            }
        }
    }


    inline double nrn_current_GRANULE_TONICCL(int id, int pnodecount, GRANULE_TONICCL_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->il[id] = inst->g[id] * (v - inst->egaba[id]) + inst->ibias[id];
        current += inst->il[id];
        return current;
    }


    /** update current */
    void nrn_cur_GRANULE_TONICCL(NrnThread* nt, Memb_list* ml, int type) {
        nrn_pragma_acc(data present(nt, ml) if(nt->compute_gpu))
        {
            int nodecount = ml->nodecount;
            int pnodecount = ml->_nodecount_padded;
            const int* node_index = ml->nodeindices;
            double* data = ml->data;
            const double* voltage = nt->_actual_v;
            double* vec_rhs = nt->_actual_rhs;
            double* vec_d = nt->_actual_d;
            Datum* indexes = ml->pdata;
            ThreadDatum* thread = ml->_thread;
            auto* const inst = static_cast<GRANULE_TONICCL_Instance*>(ml->instance);

            nrn_pragma_acc(parallel loop present(inst, node_index, data, voltage, indexes, thread, vec_rhs, vec_d) async(nt->stream_id) if(nt->compute_gpu))
            nrn_pragma_omp(target teams distribute parallel for if(nt->compute_gpu))
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                double g = nrn_current_GRANULE_TONICCL(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
                double rhs = nrn_current_GRANULE_TONICCL(id, pnodecount, inst, data, indexes, thread, nt, v);
                g = (g-rhs)/0.001;
                #if NRN_PRCELLSTATE
                inst->g_unused[id] = g;
                #endif
                vec_rhs[node_id] -= rhs;
                vec_d[node_id] += g;
            }
        }
    }


    /** update state */
    void nrn_state_GRANULE_TONICCL(NrnThread* nt, Memb_list* ml, int type) {
        nrn_pragma_acc(data present(nt, ml) if(nt->compute_gpu))
        {
            int nodecount = ml->nodecount;
            int pnodecount = ml->_nodecount_padded;
            const int* node_index = ml->nodeindices;
            double* data = ml->data;
            const double* voltage = nt->_actual_v;
            Datum* indexes = ml->pdata;
            ThreadDatum* thread = ml->_thread;
            auto* const inst = static_cast<GRANULE_TONICCL_Instance*>(ml->instance);

            nrn_pragma_acc(parallel loop present(inst, node_index, data, voltage, indexes, thread) async(nt->stream_id) if(nt->compute_gpu))
            nrn_pragma_omp(target teams distribute parallel for if(nt->compute_gpu))
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
            }
        }
    }


    /** register channel with the simulator */
    void _GRANULE_LKG2_reg() {

        int mech_type = nrn_get_mechtype("GRANULE_TONICCL");
        GRANULE_TONICCL_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_GRANULE_TONICCL, nrn_cur_GRANULE_TONICCL, nullptr, nrn_state_GRANULE_TONICCL, nrn_init_GRANULE_TONICCL, nrn_private_constructor_GRANULE_TONICCL, nrn_private_destructor_GRANULE_TONICCL, first_pointer_var_index(), 1);

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
