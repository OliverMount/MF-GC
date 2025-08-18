/*********************************************************
Model Name      : GRANULE_Nmda_leak
Filename        : GRANULE_Nmda_leak.mod
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


    /** constants used in nmodl from UNITS */
    static const double PI = 0x1.921fb54442d18p+1;
    #ifndef NRN_PRCELLSTATE
    #define NRN_PRCELLSTATE 0
    #endif


    /** channel information */
    static const char *mechanism_info[] = {
        "7.7.0",
        "GRANULE_Nmda_leak",
        "gmax_factor_GRANULE_Nmda_leak",
        "gmax_GRANULE_Nmda_leak",
        "Erev_GRANULE_Nmda_leak",
        "Q10_diff_GRANULE_Nmda_leak",
        "v0_block_GRANULE_Nmda_leak",
        "k_block_GRANULE_Nmda_leak",
        "fix_celsius_GRANULE_Nmda_leak",
        0,
        "i_GRANULE_Nmda_leak",
        "ic_GRANULE_Nmda_leak",
        "g_GRANULE_Nmda_leak",
        "MgBlock_GRANULE_Nmda_leak",
        0,
        0,
        0
    };


    /** all global variables */
    struct GRANULE_Nmda_leak_Store {
        int reset{};
        int mech_type{};
        double surf{2.9926e-06};
        double O{0.01};
        double Or{52};
        double usetable{1};
        double tmin_rates{};
        double mfac_rates{};
        double t_MgBlock[151]{};
    };
    static_assert(std::is_trivially_copy_constructible_v<GRANULE_Nmda_leak_Store>);
    static_assert(std::is_trivially_move_constructible_v<GRANULE_Nmda_leak_Store>);
    static_assert(std::is_trivially_copy_assignable_v<GRANULE_Nmda_leak_Store>);
    static_assert(std::is_trivially_move_assignable_v<GRANULE_Nmda_leak_Store>);
    static_assert(std::is_trivially_destructible_v<GRANULE_Nmda_leak_Store>);
    static GRANULE_Nmda_leak_Store GRANULE_Nmda_leak_global;


    /** all mechanism instance variables and global variables */
    struct GRANULE_Nmda_leak_Instance  {
        const double* gmax_factor{};
        const double* gmax{};
        const double* Erev{};
        const double* Q10_diff{};
        const double* v0_block{};
        const double* k_block{};
        const double* fix_celsius{};
        double* i{};
        double* ic{};
        double* g{};
        double* MgBlock{};
        double* gbar_Q10{};
        double* v_unused{};
        double* g_unused{};
        GRANULE_Nmda_leak_Store* global{&GRANULE_Nmda_leak_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"surf_GRANULE_Nmda_leak", &GRANULE_Nmda_leak_global.surf},
        {"O_GRANULE_Nmda_leak", &GRANULE_Nmda_leak_global.O},
        {"Or_GRANULE_Nmda_leak", &GRANULE_Nmda_leak_global.Or},
        {"usetable_GRANULE_Nmda_leak", &GRANULE_Nmda_leak_global.usetable},
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
        return 14;
    }


    static inline int int_variables_size() {
        return 0;
    }


    static inline int get_mech_type() {
        return GRANULE_Nmda_leak_global.mech_type;
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
    static void nrn_private_constructor_GRANULE_Nmda_leak(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new GRANULE_Nmda_leak_Instance{};
        assert(inst->global == &GRANULE_Nmda_leak_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(GRANULE_Nmda_leak_Store);
    }

    static inline void copy_instance_to_device(NrnThread* nt, Memb_list* ml, GRANULE_Nmda_leak_Instance const* inst);
    static inline void delete_instance_from_device(GRANULE_Nmda_leak_Instance* inst);
    // Deallocate the instance structure
    static void nrn_private_destructor_GRANULE_Nmda_leak(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<GRANULE_Nmda_leak_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GRANULE_Nmda_leak_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GRANULE_Nmda_leak_Store));
        delete_instance_from_device(inst);
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<GRANULE_Nmda_leak_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GRANULE_Nmda_leak_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GRANULE_Nmda_leak_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gmax_factor = ml->data+0*pnodecount;
        inst->gmax = ml->data+1*pnodecount;
        inst->Erev = ml->data+2*pnodecount;
        inst->Q10_diff = ml->data+3*pnodecount;
        inst->v0_block = ml->data+4*pnodecount;
        inst->k_block = ml->data+5*pnodecount;
        inst->fix_celsius = ml->data+6*pnodecount;
        inst->i = ml->data+7*pnodecount;
        inst->ic = ml->data+8*pnodecount;
        inst->g = ml->data+9*pnodecount;
        inst->MgBlock = ml->data+10*pnodecount;
        inst->gbar_Q10 = ml->data+11*pnodecount;
        inst->v_unused = ml->data+12*pnodecount;
        inst->g_unused = ml->data+13*pnodecount;
        copy_instance_to_device(nt, ml, inst);
    }

    static inline void copy_instance_to_device(NrnThread* nt, Memb_list* ml, GRANULE_Nmda_leak_Instance const* inst) {
        if (!nt->compute_gpu) {
            return;
        }
        auto tmp = *inst;
        auto* d_inst = cnrn_target_is_present(inst);
        if (!d_inst) {
            d_inst = cnrn_target_copyin(inst);
        }
        tmp.global = cnrn_target_deviceptr(tmp.global);
        tmp.gmax_factor = cnrn_target_deviceptr(tmp.gmax_factor);
        tmp.gmax = cnrn_target_deviceptr(tmp.gmax);
        tmp.Erev = cnrn_target_deviceptr(tmp.Erev);
        tmp.Q10_diff = cnrn_target_deviceptr(tmp.Q10_diff);
        tmp.v0_block = cnrn_target_deviceptr(tmp.v0_block);
        tmp.k_block = cnrn_target_deviceptr(tmp.k_block);
        tmp.fix_celsius = cnrn_target_deviceptr(tmp.fix_celsius);
        tmp.i = cnrn_target_deviceptr(tmp.i);
        tmp.ic = cnrn_target_deviceptr(tmp.ic);
        tmp.g = cnrn_target_deviceptr(tmp.g);
        tmp.MgBlock = cnrn_target_deviceptr(tmp.MgBlock);
        tmp.gbar_Q10 = cnrn_target_deviceptr(tmp.gbar_Q10);
        tmp.v_unused = cnrn_target_deviceptr(tmp.v_unused);
        tmp.g_unused = cnrn_target_deviceptr(tmp.g_unused);
        cnrn_target_memcpy_to_device(d_inst, &tmp);
        auto* d_ml = cnrn_target_deviceptr(ml);
        void* d_inst_void = d_inst;
        cnrn_target_memcpy_to_device(&(d_ml->instance), &d_inst_void);
    }

    static inline void delete_instance_from_device(GRANULE_Nmda_leak_Instance* inst) {
        if (cnrn_target_is_present(inst)) {
            cnrn_target_delete(inst);
        }
    }



    static void nrn_alloc_GRANULE_Nmda_leak(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_GRANULE_Nmda_leak(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GRANULE_Nmda_leak_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_GRANULE_Nmda_leak(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GRANULE_Nmda_leak_Instance*>(ml->instance);

        #endif
    }


    inline static int rates_GRANULE_Nmda_leak(int id, int pnodecount, GRANULE_Nmda_leak_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);


    inline int f_rates_GRANULE_Nmda_leak(int id, int pnodecount, GRANULE_Nmda_leak_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        int ret_f_rates = 0;
        inst->MgBlock[id] = 1.0 / (1.0 + exp( -(_lv - inst->v0_block[id]) / inst->k_block[id]));
        return ret_f_rates;
    }


    void update_table_rates_GRANULE_Nmda_leak(int id, int pnodecount, GRANULE_Nmda_leak_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        if (inst->global->usetable == 0) {
            return;
        }
        static bool make_table = true;
        static double save_v0_block;
        static double save_k_block;
        if (save_v0_block != inst->v0_block[id]) {
            make_table = true;
        }
        if (save_k_block != inst->k_block[id]) {
            make_table = true;
        }
        if (make_table) {
            make_table = false;
            inst->global->tmin_rates =  -120.0;
            double tmax = 30.0;
            double dx = (tmax-inst->global->tmin_rates) / 150.;
            inst->global->mfac_rates = 1./dx;
            double x = inst->global->tmin_rates;
            for (std::size_t i = 0; i < 151; x += dx, i++) {
                f_rates_GRANULE_Nmda_leak(id, pnodecount, inst, data, indexes, thread, nt, v, x);
                inst->global->t_MgBlock[i] = inst->MgBlock[id];
            }
            save_v0_block = inst->v0_block[id];
            save_k_block = inst->k_block[id];
        }
    }


    inline static int rates_GRANULE_Nmda_leak(int id, int pnodecount, GRANULE_Nmda_leak_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv){
        if (inst->global->usetable == 0) {
            f_rates_GRANULE_Nmda_leak(id, pnodecount, inst, data, indexes, thread, nt, v, _lv);
            return 0;
        }
        double xi = inst->global->mfac_rates * (_lv - inst->global->tmin_rates);
        if (isnan(xi)) {
            inst->MgBlock[id] = xi;
            return 0;
        }
        if (xi <= 0. || xi >= 150.) {
            int index = (xi <= 0.) ? 0 : 150;
            inst->MgBlock[id] = inst->global->t_MgBlock[index];
            return 0;
        }
        int i = int(xi);
        double theta = xi - double(i);
        inst->MgBlock[id] = inst->global->t_MgBlock[i] + theta*(inst->global->t_MgBlock[i+1]-inst->global->t_MgBlock[i]);
        return 0;
    }


    /** initialize channel */
    void nrn_init_GRANULE_Nmda_leak(NrnThread* nt, Memb_list* ml, int type) {
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
            auto* const inst = static_cast<GRANULE_Nmda_leak_Instance*>(ml->instance);

            if (nt->compute_gpu) {
                nrn_pragma_acc(update device (GRANULE_Nmda_leak_global))
                nrn_pragma_omp(target update to(GRANULE_Nmda_leak_global))
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
                    rates_GRANULE_Nmda_leak(id, pnodecount, inst, data, indexes, thread, nt, v, v);
                    inst->gbar_Q10[id] = pow(inst->Q10_diff[id], ((inst->fix_celsius[id] - 30.0) / 10.0));
                }
            }
        }
    }


    inline double nrn_current_GRANULE_Nmda_leak(int id, int pnodecount, GRANULE_Nmda_leak_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        rates_GRANULE_Nmda_leak(id, pnodecount, inst, data, indexes, thread, nt, v, v);
        inst->g[id] = inst->gmax[id] / inst->global->surf * inst->gbar_Q10[id] * inst->global->O * inst->global->Or * inst->gmax_factor[id] * inst->MgBlock[id];
        inst->i[id] = inst->g[id] * (v - inst->Erev[id]);
        inst->ic[id] = inst->i[id];
        current += inst->i[id];
        return current;
    }


    /** update current */
    void nrn_cur_GRANULE_Nmda_leak(NrnThread* nt, Memb_list* ml, int type) {
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
            auto* const inst = static_cast<GRANULE_Nmda_leak_Instance*>(ml->instance);

            nrn_pragma_acc(parallel loop present(inst, node_index, data, voltage, indexes, thread, vec_rhs, vec_d) async(nt->stream_id) if(nt->compute_gpu))
            nrn_pragma_omp(target teams distribute parallel for if(nt->compute_gpu))
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                double g = nrn_current_GRANULE_Nmda_leak(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
                double rhs = nrn_current_GRANULE_Nmda_leak(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_GRANULE_Nmda_leak(NrnThread* nt, Memb_list* ml, int type) {
        nrn_pragma_acc(data present(nt, ml) if(nt->compute_gpu))
        {
            int nodecount = ml->nodecount;
            int pnodecount = ml->_nodecount_padded;
            const int* node_index = ml->nodeindices;
            double* data = ml->data;
            const double* voltage = nt->_actual_v;
            Datum* indexes = ml->pdata;
            ThreadDatum* thread = ml->_thread;
            auto* const inst = static_cast<GRANULE_Nmda_leak_Instance*>(ml->instance);

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


    static void check_table_thread_GRANULE_Nmda_leak (int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, int tml_id) {
        setup_instance(nt, ml);
        auto* const inst = static_cast<GRANULE_Nmda_leak_Instance*>(ml->instance);
        double v = 0;
        update_table_rates_GRANULE_Nmda_leak(id, pnodecount, inst, data, indexes, thread, nt, v);
    }


    /** register channel with the simulator */
    void _GRANULE_Nmda_leak_reg() {

        int mech_type = nrn_get_mechtype("GRANULE_Nmda_leak");
        GRANULE_Nmda_leak_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_GRANULE_Nmda_leak, nrn_cur_GRANULE_Nmda_leak, nullptr, nrn_state_GRANULE_Nmda_leak, nrn_init_GRANULE_Nmda_leak, nrn_private_constructor_GRANULE_Nmda_leak, nrn_private_destructor_GRANULE_Nmda_leak, first_pointer_var_index(), 1);

        _nrn_thread_table_reg(mech_type, check_table_thread_GRANULE_Nmda_leak);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
