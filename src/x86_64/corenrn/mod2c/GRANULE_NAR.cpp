/*********************************************************
Model Name      : GRANULE_NAR
Filename        : GRANULE_NAR.mod
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
        "GRANULE_NAR",
        "Aalpha_s_GRANULE_NAR",
        "V0alpha_s_GRANULE_NAR",
        "Kalpha_s_GRANULE_NAR",
        "Shiftalpha_s_GRANULE_NAR",
        "Abeta_s_GRANULE_NAR",
        "V0beta_s_GRANULE_NAR",
        "Kbeta_s_GRANULE_NAR",
        "Shiftbeta_s_GRANULE_NAR",
        "Aalpha_f_GRANULE_NAR",
        "V0alpha_f_GRANULE_NAR",
        "Kalpha_f_GRANULE_NAR",
        "Abeta_f_GRANULE_NAR",
        "V0beta_f_GRANULE_NAR",
        "Kbeta_f_GRANULE_NAR",
        "gbar_GRANULE_NAR",
        "Q10_diff_GRANULE_NAR",
        "Q10_channel_GRANULE_NAR",
        "fix_celsius_GRANULE_NAR",
        0,
        "ic_GRANULE_NAR",
        "g_GRANULE_NAR",
        "s_inf_GRANULE_NAR",
        "tau_s_GRANULE_NAR",
        "f_inf_GRANULE_NAR",
        "tau_f_GRANULE_NAR",
        "gbar_Q10_GRANULE_NAR",
        0,
        "s_GRANULE_NAR",
        "f_GRANULE_NAR",
        0,
        0
    };


    /** all global variables */
    struct GRANULE_NAR_Store {
        int na_type{};
        double s0{};
        double f0{};
        int reset{};
        int mech_type{};
        int slist1[2]{25, 26};
        int dlist1[2]{28, 29};
        double usetable{1};
        double tmin_rate{};
        double mfac_rate{};
        double t_tau_s[13001]{};
        double t_s_inf[13001]{};
        double t_tau_f[13001]{};
        double t_f_inf[13001]{};
    };
    static_assert(std::is_trivially_copy_constructible_v<GRANULE_NAR_Store>);
    static_assert(std::is_trivially_move_constructible_v<GRANULE_NAR_Store>);
    static_assert(std::is_trivially_copy_assignable_v<GRANULE_NAR_Store>);
    static_assert(std::is_trivially_move_assignable_v<GRANULE_NAR_Store>);
    static_assert(std::is_trivially_destructible_v<GRANULE_NAR_Store>);
    static GRANULE_NAR_Store GRANULE_NAR_global;


    /** all mechanism instance variables and global variables */
    struct GRANULE_NAR_Instance  {
        const double* Aalpha_s{};
        const double* V0alpha_s{};
        const double* Kalpha_s{};
        const double* Shiftalpha_s{};
        const double* Abeta_s{};
        const double* V0beta_s{};
        const double* Kbeta_s{};
        const double* Shiftbeta_s{};
        const double* Aalpha_f{};
        const double* V0alpha_f{};
        const double* Kalpha_f{};
        const double* Abeta_f{};
        const double* V0beta_f{};
        const double* Kbeta_f{};
        const double* gbar{};
        const double* Q10_diff{};
        const double* Q10_channel{};
        const double* fix_celsius{};
        double* ic{};
        double* g{};
        double* s_inf{};
        double* tau_s{};
        double* f_inf{};
        double* tau_f{};
        double* gbar_Q10{};
        double* s{};
        double* f{};
        double* ena{};
        double* Ds{};
        double* Df{};
        double* ina{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ena{};
        double* ion_ina{};
        double* ion_dinadv{};
        GRANULE_NAR_Store* global{&GRANULE_NAR_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"usetable_GRANULE_NAR", &GRANULE_NAR_global.usetable},
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
        return 33;
    }


    static inline int int_variables_size() {
        return 3;
    }


    static inline int get_mech_type() {
        return GRANULE_NAR_global.mech_type;
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
    static void nrn_private_constructor_GRANULE_NAR(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new GRANULE_NAR_Instance{};
        assert(inst->global == &GRANULE_NAR_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(GRANULE_NAR_Store);
    }

    static inline void copy_instance_to_device(NrnThread* nt, Memb_list* ml, GRANULE_NAR_Instance const* inst);
    static inline void delete_instance_from_device(GRANULE_NAR_Instance* inst);
    // Deallocate the instance structure
    static void nrn_private_destructor_GRANULE_NAR(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<GRANULE_NAR_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GRANULE_NAR_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GRANULE_NAR_Store));
        delete_instance_from_device(inst);
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<GRANULE_NAR_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GRANULE_NAR_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GRANULE_NAR_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->Aalpha_s = ml->data+0*pnodecount;
        inst->V0alpha_s = ml->data+1*pnodecount;
        inst->Kalpha_s = ml->data+2*pnodecount;
        inst->Shiftalpha_s = ml->data+3*pnodecount;
        inst->Abeta_s = ml->data+4*pnodecount;
        inst->V0beta_s = ml->data+5*pnodecount;
        inst->Kbeta_s = ml->data+6*pnodecount;
        inst->Shiftbeta_s = ml->data+7*pnodecount;
        inst->Aalpha_f = ml->data+8*pnodecount;
        inst->V0alpha_f = ml->data+9*pnodecount;
        inst->Kalpha_f = ml->data+10*pnodecount;
        inst->Abeta_f = ml->data+11*pnodecount;
        inst->V0beta_f = ml->data+12*pnodecount;
        inst->Kbeta_f = ml->data+13*pnodecount;
        inst->gbar = ml->data+14*pnodecount;
        inst->Q10_diff = ml->data+15*pnodecount;
        inst->Q10_channel = ml->data+16*pnodecount;
        inst->fix_celsius = ml->data+17*pnodecount;
        inst->ic = ml->data+18*pnodecount;
        inst->g = ml->data+19*pnodecount;
        inst->s_inf = ml->data+20*pnodecount;
        inst->tau_s = ml->data+21*pnodecount;
        inst->f_inf = ml->data+22*pnodecount;
        inst->tau_f = ml->data+23*pnodecount;
        inst->gbar_Q10 = ml->data+24*pnodecount;
        inst->s = ml->data+25*pnodecount;
        inst->f = ml->data+26*pnodecount;
        inst->ena = ml->data+27*pnodecount;
        inst->Ds = ml->data+28*pnodecount;
        inst->Df = ml->data+29*pnodecount;
        inst->ina = ml->data+30*pnodecount;
        inst->v_unused = ml->data+31*pnodecount;
        inst->g_unused = ml->data+32*pnodecount;
        inst->ion_ena = nt->_data;
        inst->ion_ina = nt->_data;
        inst->ion_dinadv = nt->_data;
        copy_instance_to_device(nt, ml, inst);
    }

    static inline void copy_instance_to_device(NrnThread* nt, Memb_list* ml, GRANULE_NAR_Instance const* inst) {
        if (!nt->compute_gpu) {
            return;
        }
        auto tmp = *inst;
        auto* d_inst = cnrn_target_is_present(inst);
        if (!d_inst) {
            d_inst = cnrn_target_copyin(inst);
        }
        tmp.global = cnrn_target_deviceptr(tmp.global);
        tmp.Aalpha_s = cnrn_target_deviceptr(tmp.Aalpha_s);
        tmp.V0alpha_s = cnrn_target_deviceptr(tmp.V0alpha_s);
        tmp.Kalpha_s = cnrn_target_deviceptr(tmp.Kalpha_s);
        tmp.Shiftalpha_s = cnrn_target_deviceptr(tmp.Shiftalpha_s);
        tmp.Abeta_s = cnrn_target_deviceptr(tmp.Abeta_s);
        tmp.V0beta_s = cnrn_target_deviceptr(tmp.V0beta_s);
        tmp.Kbeta_s = cnrn_target_deviceptr(tmp.Kbeta_s);
        tmp.Shiftbeta_s = cnrn_target_deviceptr(tmp.Shiftbeta_s);
        tmp.Aalpha_f = cnrn_target_deviceptr(tmp.Aalpha_f);
        tmp.V0alpha_f = cnrn_target_deviceptr(tmp.V0alpha_f);
        tmp.Kalpha_f = cnrn_target_deviceptr(tmp.Kalpha_f);
        tmp.Abeta_f = cnrn_target_deviceptr(tmp.Abeta_f);
        tmp.V0beta_f = cnrn_target_deviceptr(tmp.V0beta_f);
        tmp.Kbeta_f = cnrn_target_deviceptr(tmp.Kbeta_f);
        tmp.gbar = cnrn_target_deviceptr(tmp.gbar);
        tmp.Q10_diff = cnrn_target_deviceptr(tmp.Q10_diff);
        tmp.Q10_channel = cnrn_target_deviceptr(tmp.Q10_channel);
        tmp.fix_celsius = cnrn_target_deviceptr(tmp.fix_celsius);
        tmp.ic = cnrn_target_deviceptr(tmp.ic);
        tmp.g = cnrn_target_deviceptr(tmp.g);
        tmp.s_inf = cnrn_target_deviceptr(tmp.s_inf);
        tmp.tau_s = cnrn_target_deviceptr(tmp.tau_s);
        tmp.f_inf = cnrn_target_deviceptr(tmp.f_inf);
        tmp.tau_f = cnrn_target_deviceptr(tmp.tau_f);
        tmp.gbar_Q10 = cnrn_target_deviceptr(tmp.gbar_Q10);
        tmp.s = cnrn_target_deviceptr(tmp.s);
        tmp.f = cnrn_target_deviceptr(tmp.f);
        tmp.ena = cnrn_target_deviceptr(tmp.ena);
        tmp.Ds = cnrn_target_deviceptr(tmp.Ds);
        tmp.Df = cnrn_target_deviceptr(tmp.Df);
        tmp.ina = cnrn_target_deviceptr(tmp.ina);
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

    static inline void delete_instance_from_device(GRANULE_NAR_Instance* inst) {
        if (cnrn_target_is_present(inst)) {
            cnrn_target_delete(inst);
        }
    }



    static void nrn_alloc_GRANULE_NAR(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_GRANULE_NAR(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GRANULE_NAR_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_GRANULE_NAR(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GRANULE_NAR_Instance*>(ml->instance);

        #endif
    }


    inline static double alp_s_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);
    inline static double bet_s_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);
    inline static double alp_f_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);
    inline static double bet_f_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);
    inline static int rate_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv);


    struct functor_GRANULE_NAR_0 {
        NrnThread* nt;
        GRANULE_NAR_Instance* inst;
        int id;
        int pnodecount;
        double v;
        const Datum* indexes;
        double* data;
        ThreadDatum* thread;
        double old_s, old_f;

        void initialize() {
            rate_GRANULE_NAR(id, pnodecount, inst, data, indexes, thread, nt, v, v);
            old_s = inst->s[id];
            old_f = inst->f[id];
        }

        functor_GRANULE_NAR_0(NrnThread* nt, GRANULE_NAR_Instance* inst, int id, int pnodecount, double v, const Datum* indexes, double* data, ThreadDatum* thread)
            : nt(nt), inst(inst), id(id), pnodecount(pnodecount), v(v), indexes(indexes), data(data), thread(thread)
        {}
        void operator()(const Eigen::Matrix<double, 2, 1>& nmodl_eigen_xm, Eigen::Matrix<double, 2, 1>& nmodl_eigen_dxm, Eigen::Matrix<double, 2, 1>& nmodl_eigen_fm, Eigen::Matrix<double, 2, 2>& nmodl_eigen_jm) const {
            const double* nmodl_eigen_x = nmodl_eigen_xm.data();
            double* nmodl_eigen_dx = nmodl_eigen_dxm.data();
            double* nmodl_eigen_j = nmodl_eigen_jm.data();
            double* nmodl_eigen_f = nmodl_eigen_fm.data();
            nmodl_eigen_dx[0] = std::max(1e-6, 0.02*std::fabs(nmodl_eigen_x[0]));
            nmodl_eigen_dx[1] = std::max(1e-6, 0.02*std::fabs(nmodl_eigen_x[1]));
            nmodl_eigen_f[static_cast<int>(0)] = (nt->_dt * ( -nmodl_eigen_x[static_cast<int>(0)] + inst->s_inf[id]) + inst->tau_s[id] * ( -nmodl_eigen_x[static_cast<int>(0)] + old_s)) / (nt->_dt * inst->tau_s[id]);
            nmodl_eigen_j[static_cast<int>(0)] = ( -nt->_dt - inst->tau_s[id]) / (nt->_dt * inst->tau_s[id]);
            nmodl_eigen_j[static_cast<int>(2)] = 0.0;
            nmodl_eigen_f[static_cast<int>(1)] = (nt->_dt * ( -nmodl_eigen_x[static_cast<int>(1)] + inst->f_inf[id]) + inst->tau_f[id] * ( -nmodl_eigen_x[static_cast<int>(1)] + old_f)) / (nt->_dt * inst->tau_f[id]);
            nmodl_eigen_j[static_cast<int>(1)] = 0.0;
            nmodl_eigen_j[static_cast<int>(3)] = ( -nt->_dt - inst->tau_f[id]) / (nt->_dt * inst->tau_f[id]);
        }

        void finalize() {
        }
    };


    inline int f_rate_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        int ret_f_rate = 0;
        double a_s, b_s, a_f, b_f, alp_s_in_0, bet_s_in_0, alp_f_in_0, bet_f_in_0;
        {
            double Q10, _lv_in_0;
            _lv_in_0 = _lv;
            Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 20.0) / 10.0));
            alp_s_in_0 = Q10 * (inst->Shiftalpha_s[id] + inst->Aalpha_s[id] * ((_lv_in_0 + inst->V0alpha_s[id]) / 1.0) / (exp((_lv_in_0 + inst->V0alpha_s[id]) / inst->Kalpha_s[id]) - 1.0));
        }
        a_s = alp_s_in_0;
        {
            double Q10, x1, _lv_in_1;
            _lv_in_1 = _lv;
            Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 20.0) / 10.0));
            x1 = (_lv_in_1 + inst->V0beta_s[id]) / inst->Kbeta_s[id];
            if (x1 > 200.0) {
                x1 = 200.0;
            }
            bet_s_in_0 = Q10 * (inst->Shiftbeta_s[id] + inst->Abeta_s[id] * ((_lv_in_1 + inst->V0beta_s[id]) / 1.0) / (exp(x1) - 1.0));
        }
        b_s = bet_s_in_0;
        inst->s_inf[id] = a_s / (a_s + b_s);
        inst->tau_s[id] = 1.0 / (a_s + b_s);
        {
            double Q10, _lv_in_2;
            _lv_in_2 = _lv;
            Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 20.0) / 10.0));
            alp_f_in_0 = Q10 * inst->Aalpha_f[id] * exp((_lv_in_2 - inst->V0alpha_f[id]) / inst->Kalpha_f[id]);
        }
        a_f = alp_f_in_0;
        {
            double Q10, _lv_in_3;
            _lv_in_3 = _lv;
            Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 20.0) / 10.0));
            bet_f_in_0 = Q10 * inst->Abeta_f[id] * exp((_lv_in_3 - inst->V0beta_f[id]) / inst->Kbeta_f[id]);
        }
        b_f = bet_f_in_0;
        inst->f_inf[id] = a_f / (a_f + b_f);
        inst->tau_f[id] = 1.0 / (a_f + b_f);
        return ret_f_rate;
    }


    void update_table_rate_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        if (inst->global->usetable == 0) {
            return;
        }
        static bool make_table = true;
        static double save_fix_celsius;
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
                f_rate_GRANULE_NAR(id, pnodecount, inst, data, indexes, thread, nt, v, x);
                inst->global->t_s_inf[i] = inst->s_inf[id];
                inst->global->t_tau_s[i] = inst->tau_s[id];
                inst->global->t_f_inf[i] = inst->f_inf[id];
                inst->global->t_tau_f[i] = inst->tau_f[id];
            }
            save_fix_celsius = inst->fix_celsius[id];
        }
    }


    inline static int rate_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv){
        if (inst->global->usetable == 0) {
            f_rate_GRANULE_NAR(id, pnodecount, inst, data, indexes, thread, nt, v, _lv);
            return 0;
        }
        double xi = inst->global->mfac_rate * (_lv - inst->global->tmin_rate);
        if (isnan(xi)) {
            inst->s_inf[id] = xi;
            inst->tau_s[id] = xi;
            inst->f_inf[id] = xi;
            inst->tau_f[id] = xi;
            return 0;
        }
        if (xi <= 0. || xi >= 13000.) {
            int index = (xi <= 0.) ? 0 : 13000;
            inst->s_inf[id] = inst->global->t_s_inf[index];
            inst->tau_s[id] = inst->global->t_tau_s[index];
            inst->f_inf[id] = inst->global->t_f_inf[index];
            inst->tau_f[id] = inst->global->t_tau_f[index];
            return 0;
        }
        int i = int(xi);
        double theta = xi - double(i);
        inst->s_inf[id] = inst->global->t_s_inf[i] + theta*(inst->global->t_s_inf[i+1]-inst->global->t_s_inf[i]);
        inst->tau_s[id] = inst->global->t_tau_s[i] + theta*(inst->global->t_tau_s[i+1]-inst->global->t_tau_s[i]);
        inst->f_inf[id] = inst->global->t_f_inf[i] + theta*(inst->global->t_f_inf[i+1]-inst->global->t_f_inf[i]);
        inst->tau_f[id] = inst->global->t_tau_f[i] + theta*(inst->global->t_tau_f[i+1]-inst->global->t_tau_f[i]);
        return 0;
    }


    inline double alp_s_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        double ret_alp_s = 0.0;
        double Q10;
        Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 20.0) / 10.0));
        ret_alp_s = Q10 * (inst->Shiftalpha_s[id] + inst->Aalpha_s[id] * ((_lv + inst->V0alpha_s[id]) / 1.0) / (exp((_lv + inst->V0alpha_s[id]) / inst->Kalpha_s[id]) - 1.0));
        return ret_alp_s;
    }


    inline double bet_s_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        double ret_bet_s = 0.0;
        double Q10, x1;
        Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 20.0) / 10.0));
        x1 = (_lv + inst->V0beta_s[id]) / inst->Kbeta_s[id];
        if (x1 > 200.0) {
            x1 = 200.0;
        }
        ret_bet_s = Q10 * (inst->Shiftbeta_s[id] + inst->Abeta_s[id] * ((_lv + inst->V0beta_s[id]) / 1.0) / (exp(x1) - 1.0));
        return ret_bet_s;
    }


    inline double alp_f_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        double ret_alp_f = 0.0;
        double Q10;
        Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 20.0) / 10.0));
        ret_alp_f = Q10 * inst->Aalpha_f[id] * exp((_lv - inst->V0alpha_f[id]) / inst->Kalpha_f[id]);
        return ret_alp_f;
    }


    inline double bet_f_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double _lv) {
        double ret_bet_f = 0.0;
        double Q10;
        Q10 = pow(inst->Q10_channel[id], ((inst->fix_celsius[id] - 20.0) / 10.0));
        ret_bet_f = Q10 * inst->Abeta_f[id] * exp((_lv - inst->V0beta_f[id]) / inst->Kbeta_f[id]);
        return ret_bet_f;
    }


    /** initialize channel */
    void nrn_init_GRANULE_NAR(NrnThread* nt, Memb_list* ml, int type) {
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
            auto* const inst = static_cast<GRANULE_NAR_Instance*>(ml->instance);

            if (nt->compute_gpu) {
                nrn_pragma_acc(update device (GRANULE_NAR_global))
                nrn_pragma_omp(target update to(GRANULE_NAR_global))
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
                    inst->s[id] = inst->global->s0;
                    inst->f[id] = inst->global->f0;
                    inst->gbar_Q10[id] = inst->gbar[id] * (pow(inst->Q10_diff[id], ((inst->fix_celsius[id] - 30.0) / 10.0)));
                    rate_GRANULE_NAR(id, pnodecount, inst, data, indexes, thread, nt, v, v);
                    inst->s[id] = inst->s_inf[id];
                    inst->f[id] = inst->f_inf[id];
                }
            }
        }
    }


    inline double nrn_current_GRANULE_NAR(int id, int pnodecount, GRANULE_NAR_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->g[id] = inst->gbar_Q10[id] * inst->s[id] * inst->f[id];
        inst->ina[id] = inst->g[id] * (v - inst->ena[id]);
        inst->ic[id] = inst->ina[id];
        current += inst->ina[id];
        return current;
    }


    /** update current */
    void nrn_cur_GRANULE_NAR(NrnThread* nt, Memb_list* ml, int type) {
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
            auto* const inst = static_cast<GRANULE_NAR_Instance*>(ml->instance);

            nrn_pragma_acc(parallel loop present(inst, node_index, data, voltage, indexes, thread, vec_rhs, vec_d) async(nt->stream_id) if(nt->compute_gpu))
            nrn_pragma_omp(target teams distribute parallel for if(nt->compute_gpu))
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
                double g = nrn_current_GRANULE_NAR(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
                double dina = inst->ina[id];
                double rhs = nrn_current_GRANULE_NAR(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_GRANULE_NAR(NrnThread* nt, Memb_list* ml, int type) {
        nrn_pragma_acc(data present(nt, ml) if(nt->compute_gpu))
        {
            int nodecount = ml->nodecount;
            int pnodecount = ml->_nodecount_padded;
            const int* node_index = ml->nodeindices;
            double* data = ml->data;
            const double* voltage = nt->_actual_v;
            Datum* indexes = ml->pdata;
            ThreadDatum* thread = ml->_thread;
            auto* const inst = static_cast<GRANULE_NAR_Instance*>(ml->instance);

            nrn_pragma_acc(parallel loop present(inst, node_index, data, voltage, indexes, thread) async(nt->stream_id) if(nt->compute_gpu))
            nrn_pragma_omp(target teams distribute parallel for if(nt->compute_gpu))
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
                
                Eigen::Matrix<double, 2, 1> nmodl_eigen_xm;
                double* nmodl_eigen_x = nmodl_eigen_xm.data();
                nmodl_eigen_x[static_cast<int>(0)] = inst->s[id];
                nmodl_eigen_x[static_cast<int>(1)] = inst->f[id];
                // call newton solver
                functor_GRANULE_NAR_0 newton_functor(nt, inst, id, pnodecount, v, indexes, data, thread);
                newton_functor.initialize();
                int newton_iterations = nmodl::newton::newton_solver(nmodl_eigen_xm, newton_functor);
                if (newton_iterations < 0) assert(false && "Newton solver did not converge!");
                inst->s[id] = nmodl_eigen_x[static_cast<int>(0)];
                inst->f[id] = nmodl_eigen_x[static_cast<int>(1)];
                newton_functor.initialize(); // TODO mimic calling F again.
                newton_functor.finalize();

            }
        }
    }


    static void check_table_thread_GRANULE_NAR (int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, int tml_id) {
        setup_instance(nt, ml);
        auto* const inst = static_cast<GRANULE_NAR_Instance*>(ml->instance);
        double v = 0;
        update_table_rate_GRANULE_NAR(id, pnodecount, inst, data, indexes, thread, nt, v);
    }


    /** register channel with the simulator */
    void _GRANULE_NAR_reg() {

        int mech_type = nrn_get_mechtype("GRANULE_NAR");
        GRANULE_NAR_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_GRANULE_NAR, nrn_cur_GRANULE_NAR, nullptr, nrn_state_GRANULE_NAR, nrn_init_GRANULE_NAR, nrn_private_constructor_GRANULE_NAR, nrn_private_destructor_GRANULE_NAR, first_pointer_var_index(), 1);
        GRANULE_NAR_global.na_type = nrn_get_mechtype("na_ion");

        _nrn_thread_table_reg(mech_type, check_table_thread_GRANULE_NAR);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "na_ion");
        hoc_register_dparam_semantics(mech_type, 1, "na_ion");
        hoc_register_dparam_semantics(mech_type, 2, "na_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
