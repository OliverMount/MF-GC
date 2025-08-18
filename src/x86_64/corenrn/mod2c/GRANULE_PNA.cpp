/*********************************************************
Model Name      : GRANULE_PNA
Filename        : GRANULE_PNA.mod
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

/**
 * \dir
 * \brief Solver for a system of linear equations : Crout matrix decomposition
 *
 * \file
 * \brief Implementation of Crout matrix decomposition (LU decomposition) followed by
 * Forward/Backward substitution: Implementation details : (Legacy code) nrn / scopmath / crout.c
 */

#include <Eigen/Core>
#include <cmath>

#if defined(CORENEURON_ENABLE_GPU) && !defined(DISABLE_OPENACC)
#include "coreneuron/utils/offload.hpp"
#endif

namespace nmodl {
namespace crout {

/**
 * \brief Crout matrix decomposition : in-place LU Decomposition of matrix a.
 *
 * Implementation details : (Legacy code) nrn / scopmath / crout.c
 *
 * Returns: 0 if no error; -1 if matrix is singular or ill-conditioned
 */
#if defined(CORENEURON_ENABLE_GPU) && !defined(DISABLE_OPENACC)
nrn_pragma_acc(routine seq)
nrn_pragma_omp(declare target)
#endif
template <typename T>
EIGEN_DEVICE_FUNC inline int Crout(int n, T* const a, int* const perm, double* const rowmax) {
    // roundoff is the minimal value for a pivot element without its being considered too close to
    // zero
    double roundoff = 1.e-20;
    int i, j, k, r, pivot, irow, save_i = 0, krow;
    T sum, equil_1, equil_2;

    /* Initialize permutation and rowmax vectors */

    for (i = 0; i < n; i++) {
        perm[i] = i;
        k = 0;
        for (j = 1; j < n; j++) {
            if (std::fabs(a[i * n + j]) > std::fabs(a[i * n + k])) {
                k = j;
            }
        }
        rowmax[i] = a[i * n + k];
    }

    /* Loop over rows and columns r */

    for (r = 0; r < n; r++) {
        /*
         * Operate on rth column.  This produces the lower triangular matrix
         * of terms needed to transform the constant vector.
         */

        for (i = r; i < n; i++) {
            sum = 0.0;
            irow = perm[i];
            for (k = 0; k < r; k++) {
                krow = perm[k];
                sum += a[irow * n + k] * a[krow * n + r];
            }
            a[irow * n + r] -= sum;
        }

        /* Find row containing the pivot in the rth column */

        pivot = perm[r];
        equil_1 = std::fabs(a[pivot * n + r] / rowmax[pivot]);
        for (i = r + 1; i < n; i++) {
            irow = perm[i];
            equil_2 = std::fabs(a[irow * n + r] / rowmax[irow]);
            if (equil_2 > equil_1) {
                /* make irow the new pivot row */

                pivot = irow;
                save_i = i;
                equil_1 = equil_2;
            }
        }

        /* Interchange entries in permutation vector if necessary */

        if (pivot != perm[r]) {
            perm[save_i] = perm[r];
            perm[r] = pivot;
        }

        /* Check that pivot element is not too small */

        if (std::fabs(a[pivot * n + r]) < roundoff) {
            return -1;
        }

        /*
         * Operate on row in rth position.  This produces the upper
         * triangular matrix whose diagonal elements are assumed to be unity.
         * This matrix is used in the back substitution algorithm.
         */

        for (j = r + 1; j < n; j++) {
            sum = 0.0;
            for (k = 0; k < r; k++) {
                krow = perm[k];
                sum += a[pivot * n + k] * a[krow * n + j];
            }
            a[pivot * n + j] = (a[pivot * n + j] - sum) / a[pivot * n + r];
        }
    }
    return 0;
}
#if defined(CORENEURON_ENABLE_GPU) && !defined(DISABLE_OPENACC)
nrn_pragma_omp(end declare target)
#endif

/**
 * \brief Crout matrix decomposition : Forward/Backward substitution.
 *
 * Implementation details : (Legacy code) nrn / scopmath / crout.c
 *
 * Returns: no return variable
 */
#define y_(arg) p[y[arg]]
#define b_(arg) b[arg]
#if defined(CORENEURON_ENABLE_GPU) && !defined(DISABLE_OPENACC)
nrn_pragma_acc(routine seq)
nrn_pragma_omp(declare target)
#endif
template <typename T>
EIGEN_DEVICE_FUNC inline int solveCrout(int n,
                                        T const* const a,
                                        T const* const b,
                                        T* const p,
                                        int const* const perm,
                                        int const* const y = nullptr) {
    int i, j, pivot;
    T sum;

    /* Perform forward substitution with pivoting */
    if (y) {
        for (i = 0; i < n; i++) {
            pivot = perm[i];
            sum = 0.0;
            for (j = 0; j < i; j++) {
                sum += a[pivot * n + j] * (y_(j));
            }
            y_(i) = (b_(pivot) - sum) / a[pivot * n + i];
        }

        /*
         * Note that the y vector is already in the correct order for back
         * substitution.  Perform back substitution, pivoting the matrix but not
         * the y vector.  There is no need to divide by the diagonal element as
         * this is assumed to be unity.
         */

        for (i = n - 1; i >= 0; i--) {
            pivot = perm[i];
            sum = 0.0;
            for (j = i + 1; j < n; j++) {
                sum += a[pivot * n + j] * (y_(j));
            }
            y_(i) -= sum;
        }
    } else {
        for (i = 0; i < n; i++) {
            pivot = perm[i];
            sum = 0.0;
            for (j = 0; j < i; j++) {
                sum += a[pivot * n + j] * (p[j]);
            }
            p[i] = (b_(pivot) - sum) / a[pivot * n + i];
        }

        /*
         * Note that the y vector is already in the correct order for back
         * substitution.  Perform back substitution, pivoting the matrix but not
         * the y vector.  There is no need to divide by the diagonal element as
         * this is assumed to be unity.
         */

        for (i = n - 1; i >= 0; i--) {
            pivot = perm[i];
            sum = 0.0;
            for (j = i + 1; j < n; j++) {
                sum += a[pivot * n + j] * (p[j]);
            }
            p[i] -= sum;
        }
    }
    return 0;
}
#if defined(CORENEURON_ENABLE_GPU) && !defined(DISABLE_OPENACC)
nrn_pragma_omp(end declare target)
#endif

#undef y_
#undef b_

}  // namespace crout
}  // namespace nmodl

/**
 * \dir
 * \brief Newton solver implementations
 *
 * \file
 * \brief Implementation of Newton method for solving system of non-linear equations
 */

#include <Eigen/Dense>
#include <Eigen/LU>

namespace nmodl {
/// newton solver implementations
namespace newton {

/**
 * @defgroup solver Solver Implementation
 * @brief Solver implementation details
 *
 * Implementation of Newton method for solving system of non-linear equations using Eigen
 *   - newton::newton_solver with user, e.g. SymPy, provided Jacobian
 *
 * @{
 */

static constexpr int MAX_ITER = 50;
static constexpr double EPS = 1e-13;

template <int N>
EIGEN_DEVICE_FUNC bool is_converged(const Eigen::Matrix<double, N, 1>& X,
                                    const Eigen::Matrix<double, N, N>& J,
                                    const Eigen::Matrix<double, N, 1>& F,
                                    double eps) {
    bool converged = true;
    double square_eps = eps * eps;
    for (Eigen::Index i = 0; i < N; ++i) {
        double square_error = 0.0;
        for (Eigen::Index j = 0; j < N; ++j) {
            double JX = J(i, j) * X(j);
            square_error += JX * JX;
        }

        if (F(i) * F(i) > square_eps * square_error) {
            converged = false;
// The NVHPC is buggy and wont allow us to short-circuit.
#ifndef __NVCOMPILER
            return converged;
#endif
        }
    }
    return converged;
}

/**
 * \brief Newton method with user-provided Jacobian
 *
 * Newton method with user-provided Jacobian: given initial vector X and a
 * functor that calculates `F(X)`, `J(X)` where `J(X)` is the Jacobian of `F(X)`,
 * solves for \f$F(X) = 0\f$, starting with initial value of `X` by iterating:
 *
 *  \f[
 *     X_{n+1} = X_n - J(X_n)^{-1} F(X_n)
 *  \f]
 * when \f$|F|^2 < eps^2\f$, solution has converged.
 *
 * @return number of iterations (-1 if failed to converge)
 */
template <int N, typename FUNC>
EIGEN_DEVICE_FUNC int newton_solver(Eigen::Matrix<double, N, 1>& X,
                                    FUNC functor,
                                    double eps = EPS,
                                    int max_iter = MAX_ITER) {
    // If finite differences are needed, this is stores the stepwidth.
    Eigen::Matrix<double, N, 1> dX;
    // Vector to store result of function F(X):
    Eigen::Matrix<double, N, 1> F;
    // Matrix to store Jacobian of F(X):
    Eigen::Matrix<double, N, N> J;
    // Solver iteration count:
    int iter = -1;
    while (++iter < max_iter) {
        // calculate F, J from X using user-supplied functor
        functor(X, dX, F, J);
        if (is_converged(X, J, F, eps)) {
            return iter;
        }
        // In Eigen the default storage order is ColMajor.
        // Crout's implementation requires matrices stored in RowMajor order (C-style arrays).
        // Therefore, the transposeInPlace is critical such that the data() method to give the rows
        // instead of the columns.
        if (!J.IsRowMajor) {
            J.transposeInPlace();
        }
        Eigen::Matrix<int, N, 1> pivot;
        Eigen::Matrix<double, N, 1> rowmax;
        // Check if J is singular
        if (nmodl::crout::Crout<double>(N, J.data(), pivot.data(), rowmax.data()) < 0) {
            return -1;
        }
        Eigen::Matrix<double, N, 1> X_solve;
        nmodl::crout::solveCrout<double>(N, J.data(), F.data(), X_solve.data(), pivot.data());
        X -= X_solve;
    }
    // If we fail to converge after max_iter iterations, return -1
    return -1;
}

/**
 * Newton method template specializations for \f$N <= 4\f$ Use explicit inverse
 * of `F` instead of LU decomposition. This is faster, as there is no pivoting
 * and therefore no branches, but it is not numerically safe for \f$N > 4\f$.
 */

template <typename FUNC, int N>
EIGEN_DEVICE_FUNC int newton_solver_small_N(Eigen::Matrix<double, N, 1>& X,
                                            FUNC functor,
                                            double eps,
                                            int max_iter) {
    bool invertible;
    Eigen::Matrix<double, N, 1> F;
    Eigen::Matrix<double, N, 1> dX;
    Eigen::Matrix<double, N, N> J, J_inv;
    int iter = -1;
    while (++iter < max_iter) {
        functor(X, dX, F, J);
        if (is_converged(X, J, F, eps)) {
            return iter;
        }
        // The inverse can be called from within OpenACC regions without any issue, as opposed to
        // Eigen::PartialPivLU.
        J.computeInverseWithCheck(J_inv, invertible);
        if (invertible) {
            X -= J_inv * F;
        } else {
            return -1;
        }
    }
    return -1;
}

template <typename FUNC>
EIGEN_DEVICE_FUNC int newton_solver(Eigen::Matrix<double, 1, 1>& X,
                                    FUNC functor,
                                    double eps = EPS,
                                    int max_iter = MAX_ITER) {
    return newton_solver_small_N<FUNC, 1>(X, functor, eps, max_iter);
}

template <typename FUNC>
EIGEN_DEVICE_FUNC int newton_solver(Eigen::Matrix<double, 2, 1>& X,
                                    FUNC functor,
                                    double eps = EPS,
                                    int max_iter = MAX_ITER) {
    return newton_solver_small_N<FUNC, 2>(X, functor, eps, max_iter);
}

template <typename FUNC>
EIGEN_DEVICE_FUNC int newton_solver(Eigen::Matrix<double, 3, 1>& X,
                                    FUNC functor,
                                    double eps = EPS,
                                    int max_iter = MAX_ITER) {
    return newton_solver_small_N<FUNC, 3>(X, functor, eps, max_iter);
}

template <typename FUNC>
EIGEN_DEVICE_FUNC int newton_solver(Eigen::Matrix<double, 4, 1>& X,
                                    FUNC functor,
                                    double eps = EPS,
                                    int max_iter = MAX_ITER) {
    return newton_solver_small_N<FUNC, 4>(X, functor, eps, max_iter);
}

/** @} */  // end of solver

}  // namespace newton
}  // namespace nmodl



namespace coreneuron {
    #ifndef NRN_PRCELLSTATE
    #define NRN_PRCELLSTATE 0
    #endif


    /** channel information */
    static const char *mechanism_info[] = {
        "7.7.0",
        "GRANULE_PNA",
        "Aalpha_m_GRANULE_PNA",
        "Kalpha_m_GRANULE_PNA",
        "V0alpha_m_GRANULE_PNA",
        "Abeta_m_GRANULE_PNA",
        "Kbeta_m_GRANULE_PNA",
        "V0beta_m_GRANULE_PNA",
        "V0_minf_GRANULE_PNA",
        "B_minf_GRANULE_PNA",
        "gbar_GRANULE_PNA",
        "Q10_diff_GRANULE_PNA",
        "Q10_channel_GRANULE_PNA",
        "fix_celsius_GRANULE_PNA",
        0,
        "ina_GRANULE_PNA",
        "ic_GRANULE_PNA",
        "m_inf_GRANULE_PNA",
        "tau_m_GRANULE_PNA",
        "g_GRANULE_PNA",
        "gbar_Q10_GRANULE_PNA",
        0,
        "m_GRANULE_PNA",
        0,
        0
    };


    /** all global variables */
    struct GRANULE_PNA_Store {
        int na_type{};
        double m0{};
        int reset{};
        int mech_type{};
        int slist1[1]{18};
        int dlist1[1]{20};
        double usetable{1};
        double tmin_rate{};
        double mfac_rate{};
        double t_m_inf[13001]{};
        double t_tau_m[13001]{};
    };
    static_assert(std::is_trivially_copy_constructible_v<GRANULE_PNA_Store>);
    static_assert(std::is_trivially_move_constructible_v<GRANULE_PNA_Store>);
    static_assert(std::is_trivially_copy_assignable_v<GRANULE_PNA_Store>);
    static_assert(std::is_trivially_move_assignable_v<GRANULE_PNA_Store>);
    static_assert(std::is_trivially_destructible_v<GRANULE_PNA_Store>);
    static GRANULE_PNA_Store GRANULE_PNA_global;


    /** all mechanism instance variables and global variables */
    struct GRANULE_PNA_Instance  {
        const double* Aalpha_m{};
        const double* Kalpha_m{};
        const double* V0alpha_m{};
        const double* Abeta_m{};
        const double* Kbeta_m{};
        const double* V0beta_m{};
        const double* V0_minf{};
        const double* B_minf{};
        const double* gbar{};
        const double* Q10_diff{};
        const double* Q10_channel{};
        const double* fix_celsius{};
        double* ina{};
        double* ic{};
        double* m_inf{};
        double* tau_m{};
        double* g{};
        double* gbar_Q10{};
        double* m{};
        double* ena{};
        double* Dm{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ena{};
        double* ion_ina{};
        double* ion_dinadv{};
        GRANULE_PNA_Store* global{&GRANULE_PNA_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"usetable_GRANULE_PNA", &GRANULE_PNA_global.usetable},
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
        return 23;
    }


    static inline int int_variables_size() {
        return 3;
    }


    static inline int get_mech_type() {
        return GRANULE_PNA_global.mech_type;
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
    static void nrn_private_constructor_GRANULE_PNA(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new GRANULE_PNA_Instance{};
        assert(inst->global == &GRANULE_PNA_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(GRANULE_PNA_Store);
    }

    static inline void copy_instance_to_device(NrnThread* nt, Memb_list* ml, GRANULE_PNA_Instance const* inst);
    static inline void delete_instance_from_device(GRANULE_PNA_Instance* inst);
    // Deallocate the instance structure
    static void nrn_private_destructor_GRANULE_PNA(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<GRANULE_PNA_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GRANULE_PNA_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GRANULE_PNA_Store));
        delete_instance_from_device(inst);
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<GRANULE_PNA_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GRANULE_PNA_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GRANULE_PNA_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->Aalpha_m = ml->data+0*pnodecount;
        inst->Kalpha_m = ml->data+1*pnodecount;
        inst->V0alpha_m = ml->data+2*pnodecount;
        inst->Abeta_m = ml->data+3*pnodecount;
        inst->Kbeta_m = ml->data+4*pnodecount;
        inst->V0beta_m = ml->data+5*pnodecount;
        inst->V0_minf = ml->data+6*pnodecount;
        inst->B_minf = ml->data+7*pnodecount;
        inst->gbar = ml->data+8*pnodecount;
        inst->Q10_diff = ml->data+9*pnodecount;
        inst->Q10_channel = ml->data+10*pnodecount;
        inst->fix_celsius = ml->data+11*pnodecount;
        inst->ina = ml->data+12*pnodecount;
        inst->ic = ml->data+13*pnodecount;
        inst->m_inf = ml->data+14*pnodecount;
        inst->tau_m = ml->data+15*pnodecount;
        inst->g = ml->data+16*pnodecount;
        inst->gbar_Q10 = ml->data+17*pnodecount;
        inst->m = ml->data+18*pnodecount;
        inst->ena = ml->data+19*pnodecount;
        inst->Dm = ml->data+20*pnodecount;
        inst->v_unused = ml->data+21*pnodecount;
        inst->g_unused = ml->data+22*pnodecount;
        inst->ion_ena = nt->_data;
        inst->ion_ina = nt->_data;
        inst->ion_dinadv = nt->_data;
        copy_instance_to_device(nt, ml, inst);
    }

    static inline void copy_instance_to_device(NrnThread* nt, Memb_list* ml, GRANULE_PNA_Instance const* inst) {
        if (!nt->compute_gpu) {
            return;
        }
        auto tmp = *inst;
        auto* d_inst = cnrn_target_is_present(inst);
        if (!d_inst) {
            d_inst = cnrn_target_copyin(inst);
        }
        tmp.global = cnrn_target_deviceptr(tmp.global);
        tmp.Aalpha_m = cnrn_target_deviceptr(tmp.Aalpha_m);
        tmp.Kalpha_m = cnrn_target_deviceptr(tmp.Kalpha_m);
        tmp.V0alpha_m = cnrn_target_deviceptr(tmp.V0alpha_m);
        tmp.Abeta_m = cnrn_target_deviceptr(tmp.Abeta_m);
        tmp.Kbeta_m = cnrn_target_deviceptr(tmp.Kbeta_m);
        tmp.V0beta_m = cnrn_target_deviceptr(tmp.V0beta_m);
        tmp.V0_minf = cnrn_target_deviceptr(tmp.V0_minf);
        tmp.B_minf = cnrn_target_deviceptr(tmp.B_minf);
        tmp.gbar = cnrn_target_deviceptr(tmp.gbar);
        tmp.Q10_diff = cnrn_target_deviceptr(tmp.Q10_diff);
        tmp.Q10_channel = cnrn_target_deviceptr(tmp.Q10_channel);
        tmp.fix_celsius = cnrn_target_deviceptr(tmp.fix_celsius);
        tmp.ina = cnrn_target_deviceptr(tmp.ina);
        tmp.ic = cnrn_target_deviceptr(tmp.ic);
        tmp.m_inf = cnrn_target_deviceptr(tmp.m_inf);
        tmp.tau_m = cnrn_target_deviceptr(tmp.tau_m);
        tmp.g = cnrn_target_deviceptr(tmp.g);
        tmp.gbar_Q10 = cnrn_target_deviceptr(tmp.gbar_Q10);
        tmp.m = cnrn_target_deviceptr(tmp.m);
        tmp.ena = cnrn_target_deviceptr(tmp.ena);
        tmp.Dm = cnrn_target_deviceptr(tmp.Dm);
        tmp.v_unused = cnrn_target_deviceptr(tmp.v_unused);
        tmp.g_unused = cnrn_target_deviceptr(tmp.g_unused);
        tmp.ion_ena = cnrn_target_deviceptr(tmp.ion_ena);
        tmp.ion_ina = cnrn_target_deviceptr(tmp.ion_ina);
        tmp.ion_dinadv = cnrn_target_deviceptr(tmp.ion_dinadv);
        cnrn_target_memcpy_to_device(d_inst, &tmp);
        auto* d_ml = cnrn_target_deviceptr(ml);
        void* d_inst_void = d_inst;
        cnrn_target_memcpy_to_device(&(d_ml->instance), &d_inst_void);
    }

    static inline void delete_instance_from_device(GRANULE_PNA_Instance* inst) {
        if (cnrn_target_is_present(inst)) {
            cnrn_target_delete(inst);
        }
    }



    static void nrn_alloc_GRANULE_PNA(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_GRANULE_PNA(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GRANULE_PNA_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_GRANULE_PNA(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GRANULE_PNA_Instance*>(ml->instance);

        #endif
    }


    inline static double alp_m_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);
    inline static double bet_m_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);
    inline static double linoid_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lx, double _ly);
    inline static int rate_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);


    struct functor_GRANULE_PNA_0 {
        NrnThread* nt;
        GRANULE_PNA_Instance* inst;
        int id;
        int pnodecount;
        double v;
        const Datum* indexes;
        double* data;
        ThreadDatum* thread;
        double old_m;

        void initialize() {
            rate_GRANULE_PNA(id, pnodecount, inst, data, indexes, thread, nt, v, v);
            old_m = inst->m[id];
        }

        functor_GRANULE_PNA_0(NrnThread* nt, GRANULE_PNA_Instance* inst, int id, int pnodecount, double v, const Datum* indexes, double* data, ThreadDatum* thread)
            : nt(nt), inst(inst), id(id), pnodecount(pnodecount), v(v), indexes(indexes), data(data), thread(thread)
        {}
        void operator()(const Eigen::Matrix<double, 1, 1>& nmodl_eigen_xm, Eigen::Matrix<double, 1, 1>& nmodl_eigen_dxm, Eigen::Matrix<double, 1, 1>& nmodl_eigen_fm, Eigen::Matrix<double, 1, 1>& nmodl_eigen_jm) const {
            const double* nmodl_eigen_x = nmodl_eigen_xm.data();
            double* nmodl_eigen_dx = nmodl_eigen_dxm.data();
            double* nmodl_eigen_j = nmodl_eigen_jm.data();
            double* nmodl_eigen_f = nmodl_eigen_fm.data();
            nmodl_eigen_dx[0] = std::max(1e-6, 0.02*std::fabs(nmodl_eigen_x[0]));
            nmodl_eigen_f[static_cast<int>(0)] = (nt->_dt * ( -nmodl_eigen_x[static_cast<int>(0)] + inst->m_inf[id]) + inst->tau_m[id] * ( -nmodl_eigen_x[static_cast<int>(0)] + old_m)) / (nt->_dt * inst->tau_m[id]);
            nmodl_eigen_j[static_cast<int>(0)] = ( -nt->_dt - inst->tau_m[id]) / (nt->_dt * inst->tau_m[id]);
        }

        void finalize() {
        }
    };


    inline int f_rate_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        int ret_f_rate = 0;
        double a_m, b_m, alp_m_in_0, bet_m_in_0;
        {
            double Q10, linoid_in_0, _lv_in_0;
            _lv_in_0 = _lv;
            Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 30.0) / 10.0));
            {
                double _lx_in_0, _ly_in_0;
                _lx_in_0 = _lv_in_0 - inst->V0alpha_m[id];
                _ly_in_0 = inst->Kalpha_m[id];
                if (fabs(_lx_in_0 / _ly_in_0) < 1e-6) {
                    linoid_in_0 = _ly_in_0 * (1.0 - _lx_in_0 / _ly_in_0 / 2.0);
                } else {
                    linoid_in_0 = _lx_in_0 / (exp(_lx_in_0 / _ly_in_0) - 1.0);
                }
            }
            alp_m_in_0 = Q10 * inst->Aalpha_m[id] * linoid_in_0;
        }
        a_m = alp_m_in_0;
        {
            double Q10, linoid_in_1, _lv_in_1;
            _lv_in_1 = _lv;
            Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 30.0) / 10.0));
            {
                double _lx_in_1, _ly_in_1;
                _lx_in_1 = _lv_in_1 - inst->V0beta_m[id];
                _ly_in_1 = inst->Kbeta_m[id];
                if (fabs(_lx_in_1 / _ly_in_1) < 1e-6) {
                    linoid_in_1 = _ly_in_1 * (1.0 - _lx_in_1 / _ly_in_1 / 2.0);
                } else {
                    linoid_in_1 = _lx_in_1 / (exp(_lx_in_1 / _ly_in_1) - 1.0);
                }
            }
            bet_m_in_0 = Q10 * inst->Abeta_m[id] * linoid_in_1;
        }
        b_m = bet_m_in_0;
        inst->m_inf[id] = 1.0 / (1.0 + exp( -(_lv - inst->V0_minf[id]) / inst->B_minf[id]));
        inst->tau_m[id] = 5.0 / (a_m + b_m);
        return ret_f_rate;
    }


    void update_table_rate_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        if (inst->global->usetable == 0) {
            return;
        }
        static bool make_table = true;
        static double save_Aalpha_m;
        static double save_Kalpha_m;
        static double save_V0alpha_m;
        static double save_Abeta_m;
        static double save_Kbeta_m;
        static double save_V0beta_m;
        static double save_fix_celsius;
        if (save_Aalpha_m != inst->Aalpha_m[id]) {
            make_table = true;
        }
        if (save_Kalpha_m != inst->Kalpha_m[id]) {
            make_table = true;
        }
        if (save_V0alpha_m != inst->V0alpha_m[id]) {
            make_table = true;
        }
        if (save_Abeta_m != inst->Abeta_m[id]) {
            make_table = true;
        }
        if (save_Kbeta_m != inst->Kbeta_m[id]) {
            make_table = true;
        }
        if (save_V0beta_m != inst->V0beta_m[id]) {
            make_table = true;
        }
        if (save_fix_celsius != inst->fix_celsius[id]) {
            make_table = true;
        }
        if (make_table) {
            make_table = false;
            inst->global->tmin_rate =  -100.0;
            double tmax = 30.0;
            double dx = (tmax-inst->global->tmin_rate) / 13000.;
            inst->global->mfac_rate = 1./dx;
            double x = inst->global->tmin_rate;
            for (std::size_t i = 0; i < 13001; x += dx, i++) {
                f_rate_GRANULE_PNA(id, pnodecount, inst, data, indexes, thread, nt, v, x);
                inst->global->t_m_inf[i] = inst->m_inf[id];
                inst->global->t_tau_m[i] = inst->tau_m[id];
            }
            save_Aalpha_m = inst->Aalpha_m[id];
            save_Kalpha_m = inst->Kalpha_m[id];
            save_V0alpha_m = inst->V0alpha_m[id];
            save_Abeta_m = inst->Abeta_m[id];
            save_Kbeta_m = inst->Kbeta_m[id];
            save_V0beta_m = inst->V0beta_m[id];
            save_fix_celsius = inst->fix_celsius[id];
        }
    }


    inline static int rate_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv){
        if (inst->global->usetable == 0) {
            f_rate_GRANULE_PNA(id, pnodecount, inst, data, indexes, thread, nt, v, _lv);
            return 0;
        }
        double xi = inst->global->mfac_rate * (_lv - inst->global->tmin_rate);
        if (isnan(xi)) {
            inst->m_inf[id] = xi;
            inst->tau_m[id] = xi;
            return 0;
        }
        if (xi <= 0. || xi >= 13000.) {
            int index = (xi <= 0.) ? 0 : 13000;
            inst->m_inf[id] = inst->global->t_m_inf[index];
            inst->tau_m[id] = inst->global->t_tau_m[index];
            return 0;
        }
        int i = int(xi);
        double theta = xi - double(i);
        inst->m_inf[id] = inst->global->t_m_inf[i] + theta*(inst->global->t_m_inf[i+1]-inst->global->t_m_inf[i]);
        inst->tau_m[id] = inst->global->t_tau_m[i] + theta*(inst->global->t_tau_m[i+1]-inst->global->t_tau_m[i]);
        return 0;
    }


    inline double alp_m_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        double ret_alp_m = 0.0;
        double Q10, linoid_in_0;
        Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 30.0) / 10.0));
        {
            double _lx_in_0, _ly_in_0;
            _lx_in_0 = _lv - inst->V0alpha_m[id];
            _ly_in_0 = inst->Kalpha_m[id];
            if (fabs(_lx_in_0 / _ly_in_0) < 1e-6) {
                linoid_in_0 = _ly_in_0 * (1.0 - _lx_in_0 / _ly_in_0 / 2.0);
            } else {
                linoid_in_0 = _lx_in_0 / (exp(_lx_in_0 / _ly_in_0) - 1.0);
            }
        }
        ret_alp_m = Q10 * inst->Aalpha_m[id] * linoid_in_0;
        return ret_alp_m;
    }


    inline double bet_m_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        double ret_bet_m = 0.0;
        double Q10, linoid_in_1;
        Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 30.0) / 10.0));
        {
            double _lx_in_1, _ly_in_1;
            _lx_in_1 = _lv - inst->V0beta_m[id];
            _ly_in_1 = inst->Kbeta_m[id];
            if (fabs(_lx_in_1 / _ly_in_1) < 1e-6) {
                linoid_in_1 = _ly_in_1 * (1.0 - _lx_in_1 / _ly_in_1 / 2.0);
            } else {
                linoid_in_1 = _lx_in_1 / (exp(_lx_in_1 / _ly_in_1) - 1.0);
            }
        }
        ret_bet_m = Q10 * inst->Abeta_m[id] * linoid_in_1;
        return ret_bet_m;
    }


    inline double linoid_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lx, double _ly) {
        double ret_linoid = 0.0;
        if (fabs(_lx / _ly) < 1e-6) {
            ret_linoid = _ly * (1.0 - _lx / _ly / 2.0);
        } else {
            ret_linoid = _lx / (exp(_lx / _ly) - 1.0);
        }
        return ret_linoid;
    }


    /** initialize channel */
    void nrn_init_GRANULE_PNA(NrnThread* nt, Memb_list* ml, int type) {
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
            auto* const inst = static_cast<GRANULE_PNA_Instance*>(ml->instance);

            if (nt->compute_gpu) {
                nrn_pragma_acc(update device (GRANULE_PNA_global))
                nrn_pragma_omp(target update to(GRANULE_PNA_global))
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
                    inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
                    inst->m[id] = inst->global->m0;
                    inst->gbar_Q10[id] = inst->gbar[id] * (pow(inst->Q10_diff[id], ((inst->fix_celsius[id] - 30.0) / 10.0)));
                    rate_GRANULE_PNA(id, pnodecount, inst, data, indexes, thread, nt, v, v);
                    inst->m[id] = inst->m_inf[id];
                }
            }
        }
    }


    inline double nrn_current_GRANULE_PNA(int id, int pnodecount, GRANULE_PNA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->g[id] = inst->gbar_Q10[id] * inst->m[id];
        inst->ina[id] = inst->g[id] * (v - inst->ena[id]);
        inst->ic[id] = inst->ina[id];
        current += inst->ina[id];
        return current;
    }


    /** update current */
    void nrn_cur_GRANULE_PNA(NrnThread* nt, Memb_list* ml, int type) {
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
            auto* const inst = static_cast<GRANULE_PNA_Instance*>(ml->instance);

            nrn_pragma_acc(parallel loop present(inst, node_index, data, voltage, indexes, thread, vec_rhs, vec_d) async(nt->stream_id) if(nt->compute_gpu))
            nrn_pragma_omp(target teams distribute parallel for if(nt->compute_gpu))
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
                double g = nrn_current_GRANULE_PNA(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
                double dina = inst->ina[id];
                double rhs = nrn_current_GRANULE_PNA(id, pnodecount, inst, data, indexes, thread, nt, v);
                g = (g-rhs)/0.001;
                inst->ion_dinadv[indexes[2*pnodecount + id]] += (dina-inst->ina[id])/0.001;
                inst->ion_ina[indexes[1*pnodecount + id]] += inst->ina[id];
                #if NRN_PRCELLSTATE
                inst->g_unused[id] = g;
                #endif
                vec_rhs[node_id] -= rhs;
                vec_d[node_id] += g;
            }
        }
    }


    /** update state */
    void nrn_state_GRANULE_PNA(NrnThread* nt, Memb_list* ml, int type) {
        nrn_pragma_acc(data present(nt, ml) if(nt->compute_gpu))
        {
            int nodecount = ml->nodecount;
            int pnodecount = ml->_nodecount_padded;
            const int* node_index = ml->nodeindices;
            double* data = ml->data;
            const double* voltage = nt->_actual_v;
            Datum* indexes = ml->pdata;
            ThreadDatum* thread = ml->_thread;
            auto* const inst = static_cast<GRANULE_PNA_Instance*>(ml->instance);

            nrn_pragma_acc(parallel loop present(inst, node_index, data, voltage, indexes, thread) async(nt->stream_id) if(nt->compute_gpu))
            nrn_pragma_omp(target teams distribute parallel for if(nt->compute_gpu))
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
                
                Eigen::Matrix<double, 1, 1> nmodl_eigen_xm;
                double* nmodl_eigen_x = nmodl_eigen_xm.data();
                nmodl_eigen_x[static_cast<int>(0)] = inst->m[id];
                // call newton solver
                functor_GRANULE_PNA_0 newton_functor(nt, inst, id, pnodecount, v, indexes, data, thread);
                newton_functor.initialize();
                int newton_iterations = nmodl::newton::newton_solver(nmodl_eigen_xm, newton_functor);
                if (newton_iterations < 0) assert(false && "Newton solver did not converge!");
                inst->m[id] = nmodl_eigen_x[static_cast<int>(0)];
                newton_functor.initialize(); // TODO mimic calling F again.
                newton_functor.finalize();

            }
        }
    }


    static void check_table_thread_GRANULE_PNA (int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, int tml_id) {
        setup_instance(nt, ml);
        auto* const inst = static_cast<GRANULE_PNA_Instance*>(ml->instance);
        double v = 0;
        update_table_rate_GRANULE_PNA(id, pnodecount, inst, data, indexes, thread, nt, v);
    }


    /** register channel with the simulator */
    void _GRANULE_PNA_reg() {

        int mech_type = nrn_get_mechtype("GRANULE_PNA");
        GRANULE_PNA_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_GRANULE_PNA, nrn_cur_GRANULE_PNA, nullptr, nrn_state_GRANULE_PNA, nrn_init_GRANULE_PNA, nrn_private_constructor_GRANULE_PNA, nrn_private_destructor_GRANULE_PNA, first_pointer_var_index(), 1);
        GRANULE_PNA_global.na_type = nrn_get_mechtype("na_ion");

        _nrn_thread_table_reg(mech_type, check_table_thread_GRANULE_PNA);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "na_ion");
        hoc_register_dparam_semantics(mech_type, 1, "na_ion");
        hoc_register_dparam_semantics(mech_type, 2, "na_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
