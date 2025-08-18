/* Created by Language version: 7.7.0 */
/* VECTORIZED */
#define NRN_VECTORIZED 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mech_api.h"
#undef PI
#define nil 0
#define _pval pval
// clang-format off
#include "md1redef.h"
#include "section_fwd.hpp"
#include "nrniv_mf.h"
#include "md2redef.h"
#include "nrnconf.h"
// clang-format on
#include "neuron/cache/mechanism_range.hpp"
static constexpr auto number_of_datum_variables = 3;
static constexpr auto number_of_floating_point_variables = 39;
namespace {
template <typename T>
using _nrn_mechanism_std_vector = std::vector<T>;
using _nrn_model_sorted_token = neuron::model_sorted_token;
using _nrn_mechanism_cache_range = neuron::cache::MechanismRange<number_of_floating_point_variables, number_of_datum_variables>;
using _nrn_mechanism_cache_instance = neuron::cache::MechanismInstance<number_of_floating_point_variables, number_of_datum_variables>;
using _nrn_non_owning_id_without_container = neuron::container::non_owning_identifier_without_container;
template <typename T>
using _nrn_mechanism_field = neuron::mechanism::field<T>;
template <typename... Args>
void _nrn_mechanism_register_data_fields(Args&&... args) {
  neuron::mechanism::register_data_fields(std::forward<Args>(args)...);
}
}
 
#if !NRNGPU
#undef exp
#define exp hoc_Exp
#if NRN_ENABLE_ARCH_INDEP_EXP_POW
#undef pow
#define pow hoc_pow
#endif
#endif
 
#define nrn_init _nrn_init__GRANULE_KA
#define _nrn_initial _nrn_initial__GRANULE_KA
#define nrn_cur _nrn_cur__GRANULE_KA
#define _nrn_current _nrn_current__GRANULE_KA
#define nrn_jacob _nrn_jacob__GRANULE_KA
#define nrn_state _nrn_state__GRANULE_KA
#define _net_receive _net_receive__GRANULE_KA 
#define _f_rate _f_rate__GRANULE_KA 
#define rate rate__GRANULE_KA 
#define states states__GRANULE_KA 
 
#define _threadargscomma_ _ml, _iml, _ppvar, _thread, _globals, _nt,
#define _threadargsprotocomma_ Memb_list* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt,
#define _internalthreadargsprotocomma_ _nrn_mechanism_cache_range* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt,
#define _threadargs_ _ml, _iml, _ppvar, _thread, _globals, _nt
#define _threadargsproto_ Memb_list* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt
#define _internalthreadargsproto_ _nrn_mechanism_cache_range* _ml, size_t _iml, Datum* _ppvar, Datum* _thread, double* _globals, NrnThread* _nt
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *hoc_getarg(int);
 
#define t _nt->_t
#define dt _nt->_dt
#define Aalpha_a _ml->template fpfield<0>(_iml)
#define Aalpha_a_columnindex 0
#define Kalpha_a _ml->template fpfield<1>(_iml)
#define Kalpha_a_columnindex 1
#define V0alpha_a _ml->template fpfield<2>(_iml)
#define V0alpha_a_columnindex 2
#define Abeta_a _ml->template fpfield<3>(_iml)
#define Abeta_a_columnindex 3
#define Kbeta_a _ml->template fpfield<4>(_iml)
#define Kbeta_a_columnindex 4
#define V0beta_a _ml->template fpfield<5>(_iml)
#define V0beta_a_columnindex 5
#define Aalpha_b _ml->template fpfield<6>(_iml)
#define Aalpha_b_columnindex 6
#define Kalpha_b _ml->template fpfield<7>(_iml)
#define Kalpha_b_columnindex 7
#define V0alpha_b _ml->template fpfield<8>(_iml)
#define V0alpha_b_columnindex 8
#define Abeta_b _ml->template fpfield<9>(_iml)
#define Abeta_b_columnindex 9
#define Kbeta_b _ml->template fpfield<10>(_iml)
#define Kbeta_b_columnindex 10
#define V0beta_b _ml->template fpfield<11>(_iml)
#define V0beta_b_columnindex 11
#define V0_ainf _ml->template fpfield<12>(_iml)
#define V0_ainf_columnindex 12
#define K_ainf _ml->template fpfield<13>(_iml)
#define K_ainf_columnindex 13
#define V0_binf _ml->template fpfield<14>(_iml)
#define V0_binf_columnindex 14
#define K_binf _ml->template fpfield<15>(_iml)
#define K_binf_columnindex 15
#define Q10_diff _ml->template fpfield<16>(_iml)
#define Q10_diff_columnindex 16
#define Q10_channel _ml->template fpfield<17>(_iml)
#define Q10_channel_columnindex 17
#define gbar _ml->template fpfield<18>(_iml)
#define gbar_columnindex 18
#define fix_celsius _ml->template fpfield<19>(_iml)
#define fix_celsius_columnindex 19
#define ic _ml->template fpfield<20>(_iml)
#define ic_columnindex 20
#define a_inf _ml->template fpfield<21>(_iml)
#define a_inf_columnindex 21
#define b_inf _ml->template fpfield<22>(_iml)
#define b_inf_columnindex 22
#define tau_a _ml->template fpfield<23>(_iml)
#define tau_a_columnindex 23
#define tau_b _ml->template fpfield<24>(_iml)
#define tau_b_columnindex 24
#define g _ml->template fpfield<25>(_iml)
#define g_columnindex 25
#define alpha_a _ml->template fpfield<26>(_iml)
#define alpha_a_columnindex 26
#define beta_a _ml->template fpfield<27>(_iml)
#define beta_a_columnindex 27
#define alpha_b _ml->template fpfield<28>(_iml)
#define alpha_b_columnindex 28
#define beta_b _ml->template fpfield<29>(_iml)
#define beta_b_columnindex 29
#define gbar_Q10 _ml->template fpfield<30>(_iml)
#define gbar_Q10_columnindex 30
#define a _ml->template fpfield<31>(_iml)
#define a_columnindex 31
#define b _ml->template fpfield<32>(_iml)
#define b_columnindex 32
#define ek _ml->template fpfield<33>(_iml)
#define ek_columnindex 33
#define Da _ml->template fpfield<34>(_iml)
#define Da_columnindex 34
#define Db _ml->template fpfield<35>(_iml)
#define Db_columnindex 35
#define ik _ml->template fpfield<36>(_iml)
#define ik_columnindex 36
#define v _ml->template fpfield<37>(_iml)
#define v_columnindex 37
#define _g _ml->template fpfield<38>(_iml)
#define _g_columnindex 38
#define _ion_ek *(_ml->dptr_field<0>(_iml))
#define _p_ion_ek static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ik *(_ml->dptr_field<1>(_iml))
#define _p_ion_ik static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dikdv *(_ml->dptr_field<2>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_alp_b(void);
 static void _hoc_alp_a(void);
 static void _hoc_bet_b(void);
 static void _hoc_bet_a(void);
 static void _hoc_linoid(void);
 static void _hoc_rate(void);
 static void _hoc_sigm(void);
 static int _mechtype;
extern void _nrn_cacheloop_reg(int, int);
extern void hoc_register_limits(int, HocParmLimits*);
extern void hoc_register_units(int, HocParmUnits*);
extern void nrn_promote(Prop*, int, int);
 
#define NMODL_TEXT 1
#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mechtype);
#endif
 static void _hoc_setdata();
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 {"setdata_GRANULE_KA", _hoc_setdata},
 {"alp_b_GRANULE_KA", _hoc_alp_b},
 {"alp_a_GRANULE_KA", _hoc_alp_a},
 {"bet_b_GRANULE_KA", _hoc_bet_b},
 {"bet_a_GRANULE_KA", _hoc_bet_a},
 {"linoid_GRANULE_KA", _hoc_linoid},
 {"rate_GRANULE_KA", _hoc_rate},
 {"sigm_GRANULE_KA", _hoc_sigm},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alp_b(Prop*);
 static double _npy_alp_a(Prop*);
 static double _npy_bet_b(Prop*);
 static double _npy_bet_a(Prop*);
 static double _npy_linoid(Prop*);
 static double _npy_rate(Prop*);
 static double _npy_sigm(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alp_b", _npy_alp_b},
 {"alp_a", _npy_alp_a},
 {"bet_b", _npy_bet_b},
 {"bet_a", _npy_bet_a},
 {"linoid", _npy_linoid},
 {"rate", _npy_rate},
 {"sigm", _npy_sigm},
 {0, 0}
};
#define alp_b alp_b_GRANULE_KA
#define alp_a alp_a_GRANULE_KA
#define bet_b bet_b_GRANULE_KA
#define bet_a bet_a_GRANULE_KA
#define linoid linoid_GRANULE_KA
#define sigm sigm_GRANULE_KA
 extern double alp_b( _internalthreadargsprotocomma_ double );
 extern double alp_a( _internalthreadargsprotocomma_ double );
 extern double bet_b( _internalthreadargsprotocomma_ double );
 extern double bet_a( _internalthreadargsprotocomma_ double );
 extern double linoid( _internalthreadargsprotocomma_ double , double );
 extern double sigm( _internalthreadargsprotocomma_ double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define usetable usetable_GRANULE_KA
 double usetable = 1;
 
static void _check_rate(_internalthreadargsproto_); 
static void _check_table_thread(_threadargsprotocomma_ int _type, _nrn_model_sorted_token const& _sorted_token) {
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); } 
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml, _type};
  {
    auto* const _ml = &_lmr;
   _check_rate(_threadargs_);
   }
}
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {"usetable_GRANULE_KA", 0, 1},
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"Aalpha_a_GRANULE_KA", "/ms"},
 {"Kalpha_a_GRANULE_KA", "mV"},
 {"V0alpha_a_GRANULE_KA", "mV"},
 {"Abeta_a_GRANULE_KA", "/ms"},
 {"Kbeta_a_GRANULE_KA", "mV"},
 {"V0beta_a_GRANULE_KA", "mV"},
 {"Aalpha_b_GRANULE_KA", "/ms"},
 {"Kalpha_b_GRANULE_KA", "mV"},
 {"V0alpha_b_GRANULE_KA", "mV"},
 {"Abeta_b_GRANULE_KA", "/ms"},
 {"Kbeta_b_GRANULE_KA", "mV"},
 {"V0beta_b_GRANULE_KA", "mV"},
 {"V0_ainf_GRANULE_KA", "mV"},
 {"K_ainf_GRANULE_KA", "mV"},
 {"V0_binf_GRANULE_KA", "mV"},
 {"K_binf_GRANULE_KA", "mV"},
 {"gbar_GRANULE_KA", "mho/cm2"},
 {"fix_celsius_GRANULE_KA", "degC"},
 {"ic_GRANULE_KA", "mA/cm2"},
 {"tau_a_GRANULE_KA", "ms"},
 {"tau_b_GRANULE_KA", "ms"},
 {"g_GRANULE_KA", "mho/cm2"},
 {"alpha_a_GRANULE_KA", "/ms"},
 {"beta_a_GRANULE_KA", "/ms"},
 {"alpha_b_GRANULE_KA", "/ms"},
 {"beta_b_GRANULE_KA", "/ms"},
 {"gbar_Q10_GRANULE_KA", "mho/cm2"},
 {0, 0}
};
 static double a0 = 0;
 static double b0 = 0;
 static double delta_t = 0.01;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"usetable_GRANULE_KA", &usetable_GRANULE_KA},
 {0, 0}
};
 static DoubVec hoc_vdoub[] = {
 {0, 0, 0}
};
 static double _sav_indep;
 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _extcall_prop = _prop;
 _prop_id = _nrn_get_prop_id(_prop);
 }
 static void _hoc_setdata() {
 Prop *_prop, *hoc_getdata_range(int);
 _prop = hoc_getdata_range(_mechtype);
   _setdata(_prop);
 hoc_retpushx(1.);
}
 static void nrn_alloc(Prop*);
static void nrn_init(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void nrn_state(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 static void nrn_cur(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void nrn_jacob(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 
static int _ode_count(int);
static void _ode_map(Prop*, int, neuron::container::data_handle<double>*, neuron::container::data_handle<double>*, double*, int);
static void _ode_spec(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
static void _ode_matsol(_nrn_model_sorted_token const&, NrnThread*, Memb_list*, int);
 
#define _cvode_ieq _ppvar[3].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"GRANULE_KA",
 "Aalpha_a_GRANULE_KA",
 "Kalpha_a_GRANULE_KA",
 "V0alpha_a_GRANULE_KA",
 "Abeta_a_GRANULE_KA",
 "Kbeta_a_GRANULE_KA",
 "V0beta_a_GRANULE_KA",
 "Aalpha_b_GRANULE_KA",
 "Kalpha_b_GRANULE_KA",
 "V0alpha_b_GRANULE_KA",
 "Abeta_b_GRANULE_KA",
 "Kbeta_b_GRANULE_KA",
 "V0beta_b_GRANULE_KA",
 "V0_ainf_GRANULE_KA",
 "K_ainf_GRANULE_KA",
 "V0_binf_GRANULE_KA",
 "K_binf_GRANULE_KA",
 "Q10_diff_GRANULE_KA",
 "Q10_channel_GRANULE_KA",
 "gbar_GRANULE_KA",
 "fix_celsius_GRANULE_KA",
 0,
 "ic_GRANULE_KA",
 "a_inf_GRANULE_KA",
 "b_inf_GRANULE_KA",
 "tau_a_GRANULE_KA",
 "tau_b_GRANULE_KA",
 "g_GRANULE_KA",
 "alpha_a_GRANULE_KA",
 "beta_a_GRANULE_KA",
 "alpha_b_GRANULE_KA",
 "beta_b_GRANULE_KA",
 "gbar_Q10_GRANULE_KA",
 0,
 "a_GRANULE_KA",
 "b_GRANULE_KA",
 0,
 0};
 static Symbol* _k_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.81, /* Aalpha_a */
     -23.3271, /* Kalpha_a */
     -9.17203, /* V0alpha_a */
     0.17, /* Abeta_a */
     19.4718, /* Kbeta_a */
     -18.2791, /* V0beta_a */
     0.037, /* Aalpha_b */
     12.8433, /* Kalpha_b */
     -111.332, /* V0alpha_b */
     0.034, /* Abeta_b */
     -8.90123, /* Kbeta_b */
     -49.9537, /* V0beta_b */
     -38, /* V0_ainf */
     -16.9492, /* K_ainf */
     -78.8, /* V0_binf */
     8.4, /* K_binf */
     1.5, /* Q10_diff */
     3, /* Q10_channel */
     0.0032, /* gbar */
     37, /* fix_celsius */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 4, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 39);
 	/*initialize range parameters*/
 	Aalpha_a = _parm_default[0]; /* 0.81 */
 	Kalpha_a = _parm_default[1]; /* -23.3271 */
 	V0alpha_a = _parm_default[2]; /* -9.17203 */
 	Abeta_a = _parm_default[3]; /* 0.17 */
 	Kbeta_a = _parm_default[4]; /* 19.4718 */
 	V0beta_a = _parm_default[5]; /* -18.2791 */
 	Aalpha_b = _parm_default[6]; /* 0.037 */
 	Kalpha_b = _parm_default[7]; /* 12.8433 */
 	V0alpha_b = _parm_default[8]; /* -111.332 */
 	Abeta_b = _parm_default[9]; /* 0.034 */
 	Kbeta_b = _parm_default[10]; /* -8.90123 */
 	V0beta_b = _parm_default[11]; /* -49.9537 */
 	V0_ainf = _parm_default[12]; /* -38 */
 	K_ainf = _parm_default[13]; /* -16.9492 */
 	V0_binf = _parm_default[14]; /* -78.8 */
 	K_binf = _parm_default[15]; /* 8.4 */
 	Q10_diff = _parm_default[16]; /* 1.5 */
 	Q10_channel = _parm_default[17]; /* 3 */
 	gbar = _parm_default[18]; /* 0.0032 */
 	fix_celsius = _parm_default[19]; /* 37 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 39);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ek */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ik */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dikdv */
 
}
 static void _initlists();
  /* some states have an absolute tolerance */
 static Symbol** _atollist;
 static HocStateTolerance _hoc_state_tol[] = {
 {0, 0}
};
 static void _thread_mem_init(Datum*);
 static void _thread_cleanup(Datum*);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _GRANULE_KA_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("k", -10000.);
 	_k_sym = hoc_lookup("k_ion");
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 5);
  _extcall_thread.resize(4);
  _thread_mem_init(_extcall_thread.data());
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 1, _thread_mem_init);
     _nrn_thread_reg(_mechtype, 0, _thread_cleanup);
     _nrn_thread_table_reg(_mechtype, _check_table_thread);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"Aalpha_a"} /* 0 */,
                                       _nrn_mechanism_field<double>{"Kalpha_a"} /* 1 */,
                                       _nrn_mechanism_field<double>{"V0alpha_a"} /* 2 */,
                                       _nrn_mechanism_field<double>{"Abeta_a"} /* 3 */,
                                       _nrn_mechanism_field<double>{"Kbeta_a"} /* 4 */,
                                       _nrn_mechanism_field<double>{"V0beta_a"} /* 5 */,
                                       _nrn_mechanism_field<double>{"Aalpha_b"} /* 6 */,
                                       _nrn_mechanism_field<double>{"Kalpha_b"} /* 7 */,
                                       _nrn_mechanism_field<double>{"V0alpha_b"} /* 8 */,
                                       _nrn_mechanism_field<double>{"Abeta_b"} /* 9 */,
                                       _nrn_mechanism_field<double>{"Kbeta_b"} /* 10 */,
                                       _nrn_mechanism_field<double>{"V0beta_b"} /* 11 */,
                                       _nrn_mechanism_field<double>{"V0_ainf"} /* 12 */,
                                       _nrn_mechanism_field<double>{"K_ainf"} /* 13 */,
                                       _nrn_mechanism_field<double>{"V0_binf"} /* 14 */,
                                       _nrn_mechanism_field<double>{"K_binf"} /* 15 */,
                                       _nrn_mechanism_field<double>{"Q10_diff"} /* 16 */,
                                       _nrn_mechanism_field<double>{"Q10_channel"} /* 17 */,
                                       _nrn_mechanism_field<double>{"gbar"} /* 18 */,
                                       _nrn_mechanism_field<double>{"fix_celsius"} /* 19 */,
                                       _nrn_mechanism_field<double>{"ic"} /* 20 */,
                                       _nrn_mechanism_field<double>{"a_inf"} /* 21 */,
                                       _nrn_mechanism_field<double>{"b_inf"} /* 22 */,
                                       _nrn_mechanism_field<double>{"tau_a"} /* 23 */,
                                       _nrn_mechanism_field<double>{"tau_b"} /* 24 */,
                                       _nrn_mechanism_field<double>{"g"} /* 25 */,
                                       _nrn_mechanism_field<double>{"alpha_a"} /* 26 */,
                                       _nrn_mechanism_field<double>{"beta_a"} /* 27 */,
                                       _nrn_mechanism_field<double>{"alpha_b"} /* 28 */,
                                       _nrn_mechanism_field<double>{"beta_b"} /* 29 */,
                                       _nrn_mechanism_field<double>{"gbar_Q10"} /* 30 */,
                                       _nrn_mechanism_field<double>{"a"} /* 31 */,
                                       _nrn_mechanism_field<double>{"b"} /* 32 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 33 */,
                                       _nrn_mechanism_field<double>{"Da"} /* 34 */,
                                       _nrn_mechanism_field<double>{"Db"} /* 35 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 36 */,
                                       _nrn_mechanism_field<double>{"v"} /* 37 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 38 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 39, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 GRANULE_KA /home/oliver/GranularLayerModel/src/mod/GRANULE_KA.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_a_inf;
 static double *_t_tau_a;
 static double *_t_b_inf;
 static double *_t_tau_b;
static int _reset;
static const char *modelname = "Cerebellum Granule Cell Model";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int _f_rate(_internalthreadargsprotocomma_ double);
static int rate(_internalthreadargsprotocomma_ double);
 
#define _deriv1_advance _thread[0].literal_value<int>()
#define _dith1 1
#define _recurse _thread[2].literal_value<int>()
#define _newtonspace1 _thread[3].literal_value<NewtonSpace*>()
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static void _n_rate(_internalthreadargsprotocomma_ double _lv);
 static neuron::container::field_index _slist2[2];
  static neuron::container::field_index _slist1[2], _dlist1[2];
 static int states(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   rate ( _threadargscomma_ v ) ;
   Da = ( a_inf - a ) / tau_a ;
   Db = ( b_inf - b ) / tau_b ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 rate ( _threadargscomma_ v ) ;
 Da = Da  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_a )) ;
 Db = Db  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_b )) ;
  return 0;
}
 /*END CVODE*/
 
static int states (_internalthreadargsproto_) {
  int _reset=0;
  int error = 0;
 {
  auto* _savstate1 =_thread[_dith1].get<double*>();
  auto* _dlist2 = _thread[_dith1].get<double*>() + 2;
  int _counte = -1;
 if (!_recurse) {
 _recurse = 1;
 for(int _id=0; _id < 2; _id++) {
  _savstate1[_id] = _ml->data(_iml, _slist1[_id]);
}
 error = nrn_newton_thread(_newtonspace1, 2, _slist2, neuron::scopmath::row_view{_ml, _iml}, states, _dlist2, _ml, _iml, _ppvar, _thread, _globals, _nt);
 _recurse = 0; if(error) {abort_run(error);}}
 {
   rate ( _threadargscomma_ v ) ;
   Da = ( a_inf - a ) / tau_a ;
   Db = ( b_inf - b ) / tau_b ;
   {int _id; for(_id=0; _id < 2; _id++) {
if (_deriv1_advance) {
 _dlist2[++_counte] = _ml->data(_iml, _dlist1[_id]) - (_ml->data(_iml, _slist1[_id]) - _savstate1[_id])/dt;
 }else{
_dlist2[++_counte] = _ml->data(_iml, _slist1[_id]) - _savstate1[_id];}}}
 } }
 return _reset;}
 
double alp_a ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalp_a;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lalp_a = _lQ10 * Aalpha_a * sigm ( _threadargscomma_ _lv - V0alpha_a , Kalpha_a ) ;
   
return _lalp_a;
 }
 
static void _hoc_alp_a(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_a_GRANULE_KA. Requires prior call to setdata_GRANULE_KA and that the specified mechanism instance still be in existence.", NULL);
  }
  Prop* _local_prop = _extcall_prop;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  alp_a ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alp_a(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  alp_a ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bet_a ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbet_a;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lbet_a = _lQ10 * Abeta_a / ( exp ( ( _lv - V0beta_a ) / Kbeta_a ) ) ;
   
return _lbet_a;
 }
 
static void _hoc_bet_a(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for bet_a_GRANULE_KA. Requires prior call to setdata_GRANULE_KA and that the specified mechanism instance still be in existence.", NULL);
  }
  Prop* _local_prop = _extcall_prop;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  bet_a ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bet_a(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  bet_a ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double alp_b ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalp_b;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lalp_b = _lQ10 * Aalpha_b * sigm ( _threadargscomma_ _lv - V0alpha_b , Kalpha_b ) ;
   
return _lalp_b;
 }
 
static void _hoc_alp_b(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_b_GRANULE_KA. Requires prior call to setdata_GRANULE_KA and that the specified mechanism instance still be in existence.", NULL);
  }
  Prop* _local_prop = _extcall_prop;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  alp_b ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alp_b(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  alp_b ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bet_b ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbet_b;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lbet_b = _lQ10 * Abeta_b * sigm ( _threadargscomma_ _lv - V0beta_b , Kbeta_b ) ;
   
return _lbet_b;
 }
 
static void _hoc_bet_b(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for bet_b_GRANULE_KA. Requires prior call to setdata_GRANULE_KA and that the specified mechanism instance still be in existence.", NULL);
  }
  Prop* _local_prop = _extcall_prop;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  bet_b ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bet_b(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  bet_b ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 static double _mfac_rate, _tmin_rate;
  static void _check_rate(_internalthreadargsproto_) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_Aalpha_a;
  static double _sav_Kalpha_a;
  static double _sav_V0alpha_a;
  static double _sav_Abeta_a;
  static double _sav_Kbeta_a;
  static double _sav_V0beta_a;
  static double _sav_Aalpha_b;
  static double _sav_Kalpha_b;
  static double _sav_V0alpha_b;
  static double _sav_Abeta_b;
  static double _sav_Kbeta_b;
  static double _sav_V0beta_b;
  static double _sav_fix_celsius;
  if (!usetable) {return;}
  if (_sav_Aalpha_a != Aalpha_a) { _maktable = 1;}
  if (_sav_Kalpha_a != Kalpha_a) { _maktable = 1;}
  if (_sav_V0alpha_a != V0alpha_a) { _maktable = 1;}
  if (_sav_Abeta_a != Abeta_a) { _maktable = 1;}
  if (_sav_Kbeta_a != Kbeta_a) { _maktable = 1;}
  if (_sav_V0beta_a != V0beta_a) { _maktable = 1;}
  if (_sav_Aalpha_b != Aalpha_b) { _maktable = 1;}
  if (_sav_Kalpha_b != Kalpha_b) { _maktable = 1;}
  if (_sav_V0alpha_b != V0alpha_b) { _maktable = 1;}
  if (_sav_Abeta_b != Abeta_b) { _maktable = 1;}
  if (_sav_Kbeta_b != Kbeta_b) { _maktable = 1;}
  if (_sav_V0beta_b != V0beta_b) { _maktable = 1;}
  if (_sav_fix_celsius != fix_celsius) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_rate =  - 100.0 ;
   _tmax =  30.0 ;
   _dx = (_tmax - _tmin_rate)/13000.; _mfac_rate = 1./_dx;
   for (_i=0, _x=_tmin_rate; _i < 13001; _x += _dx, _i++) {
    _f_rate(_threadargscomma_ _x);
    _t_a_inf[_i] = a_inf;
    _t_tau_a[_i] = tau_a;
    _t_b_inf[_i] = b_inf;
    _t_tau_b[_i] = tau_b;
   }
   _sav_Aalpha_a = Aalpha_a;
   _sav_Kalpha_a = Kalpha_a;
   _sav_V0alpha_a = V0alpha_a;
   _sav_Abeta_a = Abeta_a;
   _sav_Kbeta_a = Kbeta_a;
   _sav_V0beta_a = V0beta_a;
   _sav_Aalpha_b = Aalpha_b;
   _sav_Kalpha_b = Kalpha_b;
   _sav_V0alpha_b = V0alpha_b;
   _sav_Abeta_b = Abeta_b;
   _sav_Kbeta_b = Kbeta_b;
   _sav_V0beta_b = V0beta_b;
   _sav_fix_celsius = fix_celsius;
  }
 }

 static int rate(_internalthreadargsprotocomma_ double _lv) { 
#if 0
_check_rate(_threadargs_);
#endif
 _n_rate(_threadargscomma_ _lv);
 return 0;
 }

 static void _n_rate(_internalthreadargsprotocomma_ double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 _f_rate(_threadargscomma_ _lv); return; 
}
 _xi = _mfac_rate * (_lv - _tmin_rate);
 if (std::isnan(_xi)) {
  a_inf = _xi;
  tau_a = _xi;
  b_inf = _xi;
  tau_b = _xi;
  return;
 }
 if (_xi <= 0.) {
 a_inf = _t_a_inf[0];
 tau_a = _t_tau_a[0];
 b_inf = _t_b_inf[0];
 tau_b = _t_tau_b[0];
 return; }
 if (_xi >= 13000.) {
 a_inf = _t_a_inf[13000];
 tau_a = _t_tau_a[13000];
 b_inf = _t_b_inf[13000];
 tau_b = _t_tau_b[13000];
 return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 a_inf = _t_a_inf[_i] + _theta*(_t_a_inf[_i+1] - _t_a_inf[_i]);
 tau_a = _t_tau_a[_i] + _theta*(_t_tau_a[_i+1] - _t_tau_a[_i]);
 b_inf = _t_b_inf[_i] + _theta*(_t_b_inf[_i+1] - _t_b_inf[_i]);
 tau_b = _t_tau_b[_i] + _theta*(_t_tau_b[_i+1] - _t_tau_b[_i]);
 }

 
static int  _f_rate ( _internalthreadargsprotocomma_ double _lv ) {
   double _la_a , _lb_a , _la_b , _lb_b ;
 _la_a = alp_a ( _threadargscomma_ _lv ) ;
   _lb_a = bet_a ( _threadargscomma_ _lv ) ;
   _la_b = alp_b ( _threadargscomma_ _lv ) ;
   _lb_b = bet_b ( _threadargscomma_ _lv ) ;
   a_inf = 1.0 / ( 1.0 + exp ( ( _lv - V0_ainf ) / K_ainf ) ) ;
   tau_a = 1.0 / ( _la_a + _lb_a ) ;
   b_inf = 1.0 / ( 1.0 + exp ( ( _lv - V0_binf ) / K_binf ) ) ;
   tau_b = 1.0 / ( _la_b + _lb_b ) ;
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rate_GRANULE_KA. Requires prior call to setdata_GRANULE_KA and that the specified mechanism instance still be in existence.", NULL);
  }
  Prop* _local_prop = _extcall_prop;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 
#if 1
 _check_rate(_threadargs_);
#endif
 _r = 1.;
 rate ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_rate(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 
#if 1
 _check_rate(_threadargs_);
#endif
 _r = 1.;
 rate ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double linoid ( _internalthreadargsprotocomma_ double _lx , double _ly ) {
   double _llinoid;
 if ( fabs ( _lx / _ly ) < 1e-6 ) {
     _llinoid = _ly * ( 1.0 - _lx / _ly / 2.0 ) ;
     }
   else {
     _llinoid = _lx / ( exp ( _lx / _ly ) - 1.0 ) ;
     }
   
return _llinoid;
 }
 
static void _hoc_linoid(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  Prop* _local_prop = _prop_id ? _extcall_prop : nullptr;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  linoid ( _threadargscomma_ *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_linoid(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  linoid ( _threadargscomma_ *getarg(1) , *getarg(2) );
 return(_r);
}
 
double sigm ( _internalthreadargsprotocomma_ double _lx , double _ly ) {
   double _lsigm;
 _lsigm = 1.0 / ( exp ( _lx / _ly ) + 1.0 ) ;
   
return _lsigm;
 }
 
static void _hoc_sigm(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  Prop* _local_prop = _prop_id ? _extcall_prop : nullptr;
  _nrn_mechanism_cache_instance _ml_real{_local_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _local_prop ? _nrn_mechanism_access_dparam(_local_prop) : nullptr;
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  sigm ( _threadargscomma_ *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_sigm(Prop* _prop) {
    double _r{0.0};
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 _nrn_mechanism_cache_instance _ml_real{_prop};
auto* const _ml = &_ml_real;
size_t const _iml{};
_ppvar = _nrn_mechanism_access_dparam(_prop);
_thread = _extcall_thread.data();
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
_nt = nrn_threads;
 _r =  sigm ( _threadargscomma_ *getarg(1) , *getarg(2) );
 return(_r);
}
 
static int _ode_count(int _type){ return 2;}
 
static void _ode_spec(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
   Datum* _ppvar;
   size_t _iml;   _nrn_mechanism_cache_range* _ml;   Node* _nd{};
  double _v{};
  int _cntml;
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
  _ml = &_lmr;
  _cntml = _ml_arg->_nodecount;
  Datum *_thread{_ml_arg->_thread};
  double* _globals = nullptr;
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _ppvar = _ml_arg->_pdata[_iml];
    _nd = _ml_arg->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
     _ode_spec1 (_threadargs_);
  }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 2; ++_i) {
    _pv[_i] = _nrn_mechanism_get_param_handle(_prop, _slist1[_i]);
    _pvdot[_i] = _nrn_mechanism_get_param_handle(_prop, _dlist1[_i]);
    _cvode_abstol(_atollist, _atol, _i);
  }
 }
 
static void _ode_matsol_instance1(_internalthreadargsproto_) {
 _ode_matsol1 (_threadargs_);
 }
 
static void _ode_matsol(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
   Datum* _ppvar;
   size_t _iml;   _nrn_mechanism_cache_range* _ml;   Node* _nd{};
  double _v{};
  int _cntml;
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
  _ml = &_lmr;
  _cntml = _ml_arg->_nodecount;
  Datum *_thread{_ml_arg->_thread};
  double* _globals = nullptr;
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
  for (_iml = 0; _iml < _cntml; ++_iml) {
    _ppvar = _ml_arg->_pdata[_iml];
    _nd = _ml_arg->_nodelist[_iml];
    v = NODEV(_nd);
  ek = _ion_ek;
 _ode_matsol_instance1(_threadargs_);
 }}
 
static void _thread_mem_init(Datum* _thread) {
   _thread[_dith1] = new double[4]{};
   _newtonspace1 = nrn_cons_newtonspace(2);
 }
 
static void _thread_cleanup(Datum* _thread) {
   delete[] _thread[_dith1].get<double*>();
   nrn_destroy_newtonspace(_newtonspace1);
 }

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  a = a0;
  b = b0;
 {
   gbar_Q10 = gbar * ( pow( Q10_diff , ( ( fix_celsius - 30.0 ) / 10.0 ) ) ) ;
   rate ( _threadargscomma_ v ) ;
   a = a_inf ;
   b = b_inf ;
   }
 
}
}

static void nrn_init(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type){
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; double _v; int* _ni; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];

#if 0
 _check_rate(_threadargs_);
#endif
   _v = _vec_v[_ni[_iml]];
 v = _v;
  ek = _ion_ek;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   g = gbar_Q10 * a * a * a * b ;
   ik = g * ( v - ek ) ;
   ic = ik ;
   }
 _current += ik;

} return _current;
}

static void nrn_cur(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_rhs = _nt->node_rhs_storage();
auto const _vec_sav_rhs = _nt->node_sav_rhs_storage();
auto const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
   _v = _vec_v[_ni[_iml]];
  ek = _ion_ek;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dik;
  _dik = ik;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dikdv += (_dik - ik)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ik += ik ;
	 _vec_rhs[_ni[_iml]] -= _rhs;
 
}
 
}

static void nrn_jacob(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto const _vec_d = _nt->node_d_storage();
auto const _vec_sav_d = _nt->node_sav_d_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; int _iml, _cntml;
_ni = _ml_arg->_nodeindices;
_cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (_iml = 0; _iml < _cntml; ++_iml) {
  _vec_d[_ni[_iml]] += _g;
 
}
 
}

static void nrn_state(_nrn_model_sorted_token const& _sorted_token, NrnThread* _nt, Memb_list* _ml_arg, int _type) {
_nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml_arg, _type};
auto* const _vec_v = _nt->node_voltage_storage();
auto* const _ml = &_lmr;
Datum* _ppvar; Datum* _thread;
Node *_nd; double _v = 0.0; int* _ni;
double _dtsav = dt;
if (secondorder) { dt *= 0.5; }
_ni = _ml_arg->_nodeindices;
size_t _cntml = _ml_arg->_nodecount;
_thread = _ml_arg->_thread;
double* _globals = nullptr;
if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); }
for (size_t _iml = 0; _iml < _cntml; ++_iml) {
 _ppvar = _ml_arg->_pdata[_iml];
 _nd = _ml_arg->_nodelist[_iml];
   _v = _vec_v[_ni[_iml]];
 v=_v;
{
  ek = _ion_ek;
 {  _deriv1_advance = 1;
 derivimplicit_thread(2, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, states, _ml, _iml, _ppvar, _thread, _globals, _nt);
_deriv1_advance = 0;
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 2; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 } }}
 dt = _dtsav;
}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {a_columnindex, 0};  _dlist1[0] = {Da_columnindex, 0};
 _slist1[1] = {b_columnindex, 0};  _dlist1[1] = {Db_columnindex, 0};
 _slist2[0] = {a_columnindex, 0};
 _slist2[1] = {b_columnindex, 0};
   _t_a_inf = makevector(13001*sizeof(double));
   _t_tau_a = makevector(13001*sizeof(double));
   _t_b_inf = makevector(13001*sizeof(double));
   _t_tau_b = makevector(13001*sizeof(double));
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/oliver/GranularLayerModel/src/mod/GRANULE_KA.mod";
    const char* nmodl_file_text = 
  "TITLE Cerebellum Granule Cell Model\n"
  "\n"
  "COMMENT\n"
  "        KA channel\n"
  "\n"
  "	Author: E.D'Angelo, T.Nieus, A. Fontana\n"
  "	Last revised: 8.5.2000\n"
  "\n"
  "---\n"
  "Adapted by Sungho Hong and Claus Lang\n"
  "Computational Neuroscience Unit, Okinawa Institute of Science and Technology, Japan\n"
  "Supervisor: Erik De Schutter\n"
  "\n"
  "Correspondence: Sungho Hong (shhong@oist.jp)\n"
  "\n"
  "September 16, 2017\n"
  "ENDCOMMENT\n"
  "\n"
  "NEURON {\n"
  "	SUFFIX GRANULE_KA\n"
  "	USEION k READ ek WRITE ik\n"
  "	RANGE Q10_diff,Q10_channel,gbar_Q10, fix_celsius\n"
  "	RANGE gbar, ic, g, alpha_a, beta_a, alpha_b, beta_b\n"
  "	RANGE Aalpha_a, Kalpha_a, V0alpha_a\n"
  "	RANGE Abeta_a, Kbeta_a, V0beta_a\n"
  "	RANGE Aalpha_b, Kalpha_b, V0alpha_b\n"
  "	RANGE Abeta_b, Kbeta_b, V0beta_b\n"
  "	RANGE V0_ainf, K_ainf, V0_binf, K_binf\n"
  "	RANGE a_inf, tau_a, b_inf, tau_b\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	Aalpha_a = 0.81 (/ms)\n"
  "	Kalpha_a = -23.32708 (mV)\n"
  "	V0alpha_a = -9.17203 (mV)\n"
  "	Abeta_a = 0.17 (/ms)\n"
  "	Kbeta_a = 19.47175 (mV)\n"
  "	V0beta_a = -18.27914 (mV)\n"
  "\n"
  "	Aalpha_b = 0.037 (/ms)\n"
  "	Kalpha_b = 12.8433 (mV)\n"
  "	V0alpha_b = -111.33209 (mV)\n"
  "	Abeta_b = 0.034 (/ms)\n"
  "	Kbeta_b = -8.90123 (mV)\n"
  "	V0beta_b = -49.9537 (mV)\n"
  "\n"
  "	V0_ainf = -38 (mV)\n"
  "	K_ainf = -16.9492 (mV)\n"
  "\n"
  "	V0_binf = -78.8 (mV)\n"
  "	K_binf = 8.4 (mV)\n"
  "	v (mV)\n"
  "	Q10_diff	= 1.5\n"
  "	Q10_channel	= 3\n"
  "	gbar= 0.0032 (mho/cm2)\n"
  "	ek = -84.69 (mV)\n"
  "    fix_celsius = 37 (degC)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	a\n"
  "	b\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ik (mA/cm2)\n"
  "	ic (mA/cm2)\n"
  "	a_inf\n"
  "	b_inf\n"
  "	tau_a (ms)\n"
  "	tau_b (ms)\n"
  "	g (mho/cm2)\n"
  "	alpha_a (/ms)\n"
  "	beta_a (/ms)\n"
  "	alpha_b (/ms)\n"
  "	beta_b (/ms)\n"
  "	gbar_Q10 (mho/cm2)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	gbar_Q10 = gbar*(Q10_diff^((fix_celsius-30)/10))\n"
  "	rate(v)\n"
  "	a = a_inf\n"
  "	b = b_inf\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD derivimplicit\n"
  "	g = gbar_Q10*a*a*a*b\n"
  "	ik = g*(v - ek)\n"
  "	ic = ik\n"
  ":	alpha_a = alp_a(v)\n"
  ":	beta_a = bet_a(v)\n"
  ":	alpha_b = alp_b(v)\n"
  ":	beta_b = bet_b(v)\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	rate(v)\n"
  "	a' =(a_inf - a)/tau_a\n"
  "	b' =(b_inf - b)/tau_b\n"
  "}\n"
  "\n"
  "FUNCTION alp_a(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  ":	alp_a = Q10*Aalpha_a*exp(Kalpha_a*(v-V0alpha_a))\n"
  ":	alp_a = -0.04148(/mV-ms)*linoid(v+67.697(mV),-3.857(mV))\n"
  "	alp_a = Q10*Aalpha_a*sigm(v-V0alpha_a,Kalpha_a)\n"
  "}\n"
  "\n"
  "FUNCTION bet_a(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  ":	bet_a = Q10*Abeta_a*exp(Kbeta_a*(v-V0beta_a))\n"
  ":	bet_a = 0.0359(/mV-ms)*linoid(v+45.878(mV),23.654(mV))\n"
  "	bet_a = Q10*Abeta_a/(exp((v-V0beta_a)/Kbeta_a))\n"
  "}\n"
  "\n"
  "FUNCTION alp_b(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  ":	alp_b = Q10*Aalpha_b*exp(Kalpha_b*(v-V0alpha_b))\n"
  ":	alp_b = 0.356(/mV-ms)*linoid(v+231.03(mV),17.8(mV))\n"
  "	alp_b = Q10*Aalpha_b*sigm(v-V0alpha_b,Kalpha_b)\n"
  "}\n"
  "\n"
  "FUNCTION bet_b(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  ":	bet_b = Q10*Abeta_b*exp(Kbeta_b*(v-V0beta_b))\n"
  ":	bet_b = -0.00825(/mV-ms)*linoid(v+43.284(mV),-8.927(mV))\n"
  "	bet_b = Q10*Abeta_b*sigm(v-V0beta_b,Kbeta_b)\n"
  "}\n"
  "\n"
  "PROCEDURE rate(v (mV)) {LOCAL a_a, b_a, a_b, b_b\n"
  "	TABLE a_inf, tau_a, b_inf, tau_b\n"
  "	DEPEND Aalpha_a, Kalpha_a, V0alpha_a,\n"
  "	       Abeta_a, Kbeta_a, V0beta_a,\n"
  "               Aalpha_b, Kalpha_b, V0alpha_b,\n"
  "               Abeta_b, Kbeta_b, V0beta_b, fix_celsius FROM -100 TO 30 WITH 13000\n"
  "	a_a = alp_a(v)\n"
  "	b_a = bet_a(v)\n"
  "	a_b = alp_b(v)\n"
  "	b_b = bet_b(v)\n"
  "	a_inf = 1/(1+exp((v-V0_ainf)/K_ainf))\n"
  "	tau_a = 1/(a_a + b_a)\n"
  "	b_inf = 1/(1+exp((v-V0_binf)/K_binf))\n"
  "	tau_b = 1/(a_b + b_b)\n"
  ": Bardoni Belluzzi data\n"
  ":	a_inf = 1/(1+exp(-(v+46.7)/19.8))\n"
  ":	tau_a = 0.41*exp(-(v+43.5)/42.8)+0.167\n"
  ":	b_inf = 1/(1+exp((v+78.8)/8.4))\n"
  ":	tau_b = 10.8 + 0.03*v + 1/(57.9*exp(0.127*v)+0.000134*exp(-0.059*v))\n"
  "}\n"
  "\n"
  "FUNCTION linoid(x (mV),y (mV)) (mV) {\n"
  "        if (fabs(x/y) < 1e-6) {\n"
  "                linoid = y*(1 - x/y/2)\n"
  "        }else{\n"
  "                linoid = x/(exp(x/y) - 1)\n"
  "        }\n"
  "}\n"
  "\n"
  "FUNCTION sigm(x (mV),y (mV)) {\n"
  "                sigm = 1/(exp(x/y) + 1)\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
