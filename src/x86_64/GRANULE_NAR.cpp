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
static constexpr auto number_of_floating_point_variables = 33;
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
 
#define nrn_init _nrn_init__GRANULE_NAR
#define _nrn_initial _nrn_initial__GRANULE_NAR
#define nrn_cur _nrn_cur__GRANULE_NAR
#define _nrn_current _nrn_current__GRANULE_NAR
#define nrn_jacob _nrn_jacob__GRANULE_NAR
#define nrn_state _nrn_state__GRANULE_NAR
#define _net_receive _net_receive__GRANULE_NAR 
#define _f_rate _f_rate__GRANULE_NAR 
#define rate rate__GRANULE_NAR 
#define states states__GRANULE_NAR 
 
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
#define Aalpha_s _ml->template fpfield<0>(_iml)
#define Aalpha_s_columnindex 0
#define V0alpha_s _ml->template fpfield<1>(_iml)
#define V0alpha_s_columnindex 1
#define Kalpha_s _ml->template fpfield<2>(_iml)
#define Kalpha_s_columnindex 2
#define Shiftalpha_s _ml->template fpfield<3>(_iml)
#define Shiftalpha_s_columnindex 3
#define Abeta_s _ml->template fpfield<4>(_iml)
#define Abeta_s_columnindex 4
#define V0beta_s _ml->template fpfield<5>(_iml)
#define V0beta_s_columnindex 5
#define Kbeta_s _ml->template fpfield<6>(_iml)
#define Kbeta_s_columnindex 6
#define Shiftbeta_s _ml->template fpfield<7>(_iml)
#define Shiftbeta_s_columnindex 7
#define Aalpha_f _ml->template fpfield<8>(_iml)
#define Aalpha_f_columnindex 8
#define V0alpha_f _ml->template fpfield<9>(_iml)
#define V0alpha_f_columnindex 9
#define Kalpha_f _ml->template fpfield<10>(_iml)
#define Kalpha_f_columnindex 10
#define Abeta_f _ml->template fpfield<11>(_iml)
#define Abeta_f_columnindex 11
#define V0beta_f _ml->template fpfield<12>(_iml)
#define V0beta_f_columnindex 12
#define Kbeta_f _ml->template fpfield<13>(_iml)
#define Kbeta_f_columnindex 13
#define gbar _ml->template fpfield<14>(_iml)
#define gbar_columnindex 14
#define Q10_diff _ml->template fpfield<15>(_iml)
#define Q10_diff_columnindex 15
#define Q10_channel _ml->template fpfield<16>(_iml)
#define Q10_channel_columnindex 16
#define fix_celsius _ml->template fpfield<17>(_iml)
#define fix_celsius_columnindex 17
#define ic _ml->template fpfield<18>(_iml)
#define ic_columnindex 18
#define g _ml->template fpfield<19>(_iml)
#define g_columnindex 19
#define s_inf _ml->template fpfield<20>(_iml)
#define s_inf_columnindex 20
#define tau_s _ml->template fpfield<21>(_iml)
#define tau_s_columnindex 21
#define f_inf _ml->template fpfield<22>(_iml)
#define f_inf_columnindex 22
#define tau_f _ml->template fpfield<23>(_iml)
#define tau_f_columnindex 23
#define gbar_Q10 _ml->template fpfield<24>(_iml)
#define gbar_Q10_columnindex 24
#define s _ml->template fpfield<25>(_iml)
#define s_columnindex 25
#define f _ml->template fpfield<26>(_iml)
#define f_columnindex 26
#define ena _ml->template fpfield<27>(_iml)
#define ena_columnindex 27
#define Ds _ml->template fpfield<28>(_iml)
#define Ds_columnindex 28
#define Df _ml->template fpfield<29>(_iml)
#define Df_columnindex 29
#define ina _ml->template fpfield<30>(_iml)
#define ina_columnindex 30
#define v _ml->template fpfield<31>(_iml)
#define v_columnindex 31
#define _g _ml->template fpfield<32>(_iml)
#define _g_columnindex 32
#define _ion_ena *(_ml->dptr_field<0>(_iml))
#define _p_ion_ena static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ina *(_ml->dptr_field<1>(_iml))
#define _p_ion_ina static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dinadv *(_ml->dptr_field<2>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_alp_f(void);
 static void _hoc_alp_s(void);
 static void _hoc_bet_f(void);
 static void _hoc_bet_s(void);
 static void _hoc_rate(void);
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
 {"setdata_GRANULE_NAR", _hoc_setdata},
 {"alp_f_GRANULE_NAR", _hoc_alp_f},
 {"alp_s_GRANULE_NAR", _hoc_alp_s},
 {"bet_f_GRANULE_NAR", _hoc_bet_f},
 {"bet_s_GRANULE_NAR", _hoc_bet_s},
 {"rate_GRANULE_NAR", _hoc_rate},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alp_f(Prop*);
 static double _npy_alp_s(Prop*);
 static double _npy_bet_f(Prop*);
 static double _npy_bet_s(Prop*);
 static double _npy_rate(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alp_f", _npy_alp_f},
 {"alp_s", _npy_alp_s},
 {"bet_f", _npy_bet_f},
 {"bet_s", _npy_bet_s},
 {"rate", _npy_rate},
 {0, 0}
};
#define alp_f alp_f_GRANULE_NAR
#define alp_s alp_s_GRANULE_NAR
#define bet_f bet_f_GRANULE_NAR
#define bet_s bet_s_GRANULE_NAR
 extern double alp_f( _internalthreadargsprotocomma_ double );
 extern double alp_s( _internalthreadargsprotocomma_ double );
 extern double bet_f( _internalthreadargsprotocomma_ double );
 extern double bet_s( _internalthreadargsprotocomma_ double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define usetable usetable_GRANULE_NAR
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
 {"usetable_GRANULE_NAR", 0, 1},
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"Aalpha_s_GRANULE_NAR", "/ms"},
 {"V0alpha_s_GRANULE_NAR", "mV"},
 {"Kalpha_s_GRANULE_NAR", "mV"},
 {"Shiftalpha_s_GRANULE_NAR", "/ms"},
 {"Abeta_s_GRANULE_NAR", "/ms"},
 {"V0beta_s_GRANULE_NAR", "mV"},
 {"Kbeta_s_GRANULE_NAR", "mV"},
 {"Shiftbeta_s_GRANULE_NAR", "/ms"},
 {"Aalpha_f_GRANULE_NAR", "/ms"},
 {"V0alpha_f_GRANULE_NAR", "mV"},
 {"Kalpha_f_GRANULE_NAR", "mV"},
 {"Abeta_f_GRANULE_NAR", "/ms"},
 {"V0beta_f_GRANULE_NAR", "mV"},
 {"Kbeta_f_GRANULE_NAR", "mV"},
 {"gbar_GRANULE_NAR", "mho/cm2"},
 {"fix_celsius_GRANULE_NAR", "degC"},
 {"ic_GRANULE_NAR", "mA/cm2"},
 {"g_GRANULE_NAR", "mho/cm2"},
 {"tau_s_GRANULE_NAR", "ms"},
 {"tau_f_GRANULE_NAR", "ms"},
 {"gbar_Q10_GRANULE_NAR", "mho/cm2"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double f0 = 0;
 static double s0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"usetable_GRANULE_NAR", &usetable_GRANULE_NAR},
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
 0};
 static Symbol* _na_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     -0.00493, /* Aalpha_s */
     -4.48754, /* V0alpha_s */
     -6.81881, /* Kalpha_s */
     8e-05, /* Shiftalpha_s */
     0.01558, /* Abeta_s */
     43.9749, /* V0beta_s */
     0.10818, /* Kbeta_s */
     0.04752, /* Shiftbeta_s */
     0.31836, /* Aalpha_f */
     -80, /* V0alpha_f */
     -62.5262, /* Kalpha_f */
     0.01014, /* Abeta_f */
     -83.3332, /* V0beta_f */
     16.0538, /* Kbeta_f */
     0.0002, /* gbar */
     1.5, /* Q10_diff */
     3, /* Q10_channel */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 33);
 	/*initialize range parameters*/
 	Aalpha_s = _parm_default[0]; /* -0.00493 */
 	V0alpha_s = _parm_default[1]; /* -4.48754 */
 	Kalpha_s = _parm_default[2]; /* -6.81881 */
 	Shiftalpha_s = _parm_default[3]; /* 8e-05 */
 	Abeta_s = _parm_default[4]; /* 0.01558 */
 	V0beta_s = _parm_default[5]; /* 43.9749 */
 	Kbeta_s = _parm_default[6]; /* 0.10818 */
 	Shiftbeta_s = _parm_default[7]; /* 0.04752 */
 	Aalpha_f = _parm_default[8]; /* 0.31836 */
 	V0alpha_f = _parm_default[9]; /* -80 */
 	Kalpha_f = _parm_default[10]; /* -62.5262 */
 	Abeta_f = _parm_default[11]; /* 0.01014 */
 	V0beta_f = _parm_default[12]; /* -83.3332 */
 	Kbeta_f = _parm_default[13]; /* 16.0538 */
 	gbar = _parm_default[14]; /* 0.0002 */
 	Q10_diff = _parm_default[15]; /* 1.5 */
 	Q10_channel = _parm_default[16]; /* 3 */
 	fix_celsius = _parm_default[17]; /* 37 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 33);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_na_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ena */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ina */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dinadv */
 
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

 extern "C" void _GRANULE_NAR_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("na", -10000.);
 	_na_sym = hoc_lookup("na_ion");
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
                                       _nrn_mechanism_field<double>{"Aalpha_s"} /* 0 */,
                                       _nrn_mechanism_field<double>{"V0alpha_s"} /* 1 */,
                                       _nrn_mechanism_field<double>{"Kalpha_s"} /* 2 */,
                                       _nrn_mechanism_field<double>{"Shiftalpha_s"} /* 3 */,
                                       _nrn_mechanism_field<double>{"Abeta_s"} /* 4 */,
                                       _nrn_mechanism_field<double>{"V0beta_s"} /* 5 */,
                                       _nrn_mechanism_field<double>{"Kbeta_s"} /* 6 */,
                                       _nrn_mechanism_field<double>{"Shiftbeta_s"} /* 7 */,
                                       _nrn_mechanism_field<double>{"Aalpha_f"} /* 8 */,
                                       _nrn_mechanism_field<double>{"V0alpha_f"} /* 9 */,
                                       _nrn_mechanism_field<double>{"Kalpha_f"} /* 10 */,
                                       _nrn_mechanism_field<double>{"Abeta_f"} /* 11 */,
                                       _nrn_mechanism_field<double>{"V0beta_f"} /* 12 */,
                                       _nrn_mechanism_field<double>{"Kbeta_f"} /* 13 */,
                                       _nrn_mechanism_field<double>{"gbar"} /* 14 */,
                                       _nrn_mechanism_field<double>{"Q10_diff"} /* 15 */,
                                       _nrn_mechanism_field<double>{"Q10_channel"} /* 16 */,
                                       _nrn_mechanism_field<double>{"fix_celsius"} /* 17 */,
                                       _nrn_mechanism_field<double>{"ic"} /* 18 */,
                                       _nrn_mechanism_field<double>{"g"} /* 19 */,
                                       _nrn_mechanism_field<double>{"s_inf"} /* 20 */,
                                       _nrn_mechanism_field<double>{"tau_s"} /* 21 */,
                                       _nrn_mechanism_field<double>{"f_inf"} /* 22 */,
                                       _nrn_mechanism_field<double>{"tau_f"} /* 23 */,
                                       _nrn_mechanism_field<double>{"gbar_Q10"} /* 24 */,
                                       _nrn_mechanism_field<double>{"s"} /* 25 */,
                                       _nrn_mechanism_field<double>{"f"} /* 26 */,
                                       _nrn_mechanism_field<double>{"ena"} /* 27 */,
                                       _nrn_mechanism_field<double>{"Ds"} /* 28 */,
                                       _nrn_mechanism_field<double>{"Df"} /* 29 */,
                                       _nrn_mechanism_field<double>{"ina"} /* 30 */,
                                       _nrn_mechanism_field<double>{"v"} /* 31 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 32 */,
                                       _nrn_mechanism_field<double*>{"_ion_ena", "na_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ina", "na_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dinadv", "na_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 33, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 GRANULE_NAR /home/oliver/GranularLayerModel/src/mod/GRANULE_NAR.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_s_inf;
 static double *_t_tau_s;
 static double *_t_f_inf;
 static double *_t_tau_f;
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
   Ds = ( s_inf - s ) / tau_s ;
   Df = ( f_inf - f ) / tau_f ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 rate ( _threadargscomma_ v ) ;
 Ds = Ds  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_s )) ;
 Df = Df  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_f )) ;
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
   Ds = ( s_inf - s ) / tau_s ;
   Df = ( f_inf - f ) / tau_f ;
   {int _id; for(_id=0; _id < 2; _id++) {
if (_deriv1_advance) {
 _dlist2[++_counte] = _ml->data(_iml, _dlist1[_id]) - (_ml->data(_iml, _slist1[_id]) - _savstate1[_id])/dt;
 }else{
_dlist2[++_counte] = _ml->data(_iml, _slist1[_id]) - _savstate1[_id];}}}
 } }
 return _reset;}
 static double _mfac_rate, _tmin_rate;
  static void _check_rate(_internalthreadargsproto_) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_fix_celsius;
  if (!usetable) {return;}
  if (_sav_fix_celsius != fix_celsius) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_rate =  - 100.0 ;
   _tmax =  30.0 ;
   _dx = (_tmax - _tmin_rate)/13000.; _mfac_rate = 1./_dx;
   for (_i=0, _x=_tmin_rate; _i < 13001; _x += _dx, _i++) {
    _f_rate(_threadargscomma_ _x);
    _t_s_inf[_i] = s_inf;
    _t_tau_s[_i] = tau_s;
    _t_f_inf[_i] = f_inf;
    _t_tau_f[_i] = tau_f;
   }
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
  s_inf = _xi;
  tau_s = _xi;
  f_inf = _xi;
  tau_f = _xi;
  return;
 }
 if (_xi <= 0.) {
 s_inf = _t_s_inf[0];
 tau_s = _t_tau_s[0];
 f_inf = _t_f_inf[0];
 tau_f = _t_tau_f[0];
 return; }
 if (_xi >= 13000.) {
 s_inf = _t_s_inf[13000];
 tau_s = _t_tau_s[13000];
 f_inf = _t_f_inf[13000];
 tau_f = _t_tau_f[13000];
 return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 s_inf = _t_s_inf[_i] + _theta*(_t_s_inf[_i+1] - _t_s_inf[_i]);
 tau_s = _t_tau_s[_i] + _theta*(_t_tau_s[_i+1] - _t_tau_s[_i]);
 f_inf = _t_f_inf[_i] + _theta*(_t_f_inf[_i+1] - _t_f_inf[_i]);
 tau_f = _t_tau_f[_i] + _theta*(_t_tau_f[_i+1] - _t_tau_f[_i]);
 }

 
static int  _f_rate ( _internalthreadargsprotocomma_ double _lv ) {
   double _la_s , _lb_s , _la_f , _lb_f ;
 _la_s = alp_s ( _threadargscomma_ _lv ) ;
   _lb_s = bet_s ( _threadargscomma_ _lv ) ;
   s_inf = _la_s / ( _la_s + _lb_s ) ;
   tau_s = 1.0 / ( _la_s + _lb_s ) ;
   _la_f = alp_f ( _threadargscomma_ _lv ) ;
   _lb_f = bet_f ( _threadargscomma_ _lv ) ;
   f_inf = _la_f / ( _la_f + _lb_f ) ;
   tau_f = 1.0 / ( _la_f + _lb_f ) ;
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rate_GRANULE_NAR. Requires prior call to setdata_GRANULE_NAR and that the specified mechanism instance still be in existence.", NULL);
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
 
double alp_s ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalp_s;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lalp_s = _lQ10 * ( Shiftalpha_s + Aalpha_s * ( ( _lv + V0alpha_s ) / 1.0 ) / ( exp ( ( _lv + V0alpha_s ) / Kalpha_s ) - 1.0 ) ) ;
   
return _lalp_s;
 }
 
static void _hoc_alp_s(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_s_GRANULE_NAR. Requires prior call to setdata_GRANULE_NAR and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  alp_s ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alp_s(Prop* _prop) {
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
 _r =  alp_s ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bet_s ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbet_s;
 double _lQ10 , _lx1 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lx1 = ( _lv + V0beta_s ) / Kbeta_s ;
   if ( _lx1 > 200.0 ) {
     _lx1 = 200.0 ;
     }
   _lbet_s = _lQ10 * ( Shiftbeta_s + Abeta_s * ( ( _lv + V0beta_s ) / 1.0 ) / ( exp ( _lx1 ) - 1.0 ) ) ;
   
return _lbet_s;
 }
 
static void _hoc_bet_s(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for bet_s_GRANULE_NAR. Requires prior call to setdata_GRANULE_NAR and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  bet_s ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bet_s(Prop* _prop) {
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
 _r =  bet_s ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double alp_f ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalp_f;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lalp_f = _lQ10 * Aalpha_f * exp ( ( _lv - V0alpha_f ) / Kalpha_f ) ;
   
return _lalp_f;
 }
 
static void _hoc_alp_f(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_f_GRANULE_NAR. Requires prior call to setdata_GRANULE_NAR and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  alp_f ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alp_f(Prop* _prop) {
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
 _r =  alp_f ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bet_f ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbet_f;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lbet_f = _lQ10 * Abeta_f * exp ( ( _lv - V0beta_f ) / Kbeta_f ) ;
   
return _lbet_f;
 }
 
static void _hoc_bet_f(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for bet_f_GRANULE_NAR. Requires prior call to setdata_GRANULE_NAR and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  bet_f ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bet_f(Prop* _prop) {
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
 _r =  bet_f ( _threadargscomma_ *getarg(1) );
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
  ena = _ion_ena;
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
  ena = _ion_ena;
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
  f = f0;
  s = s0;
 {
   gbar_Q10 = gbar * ( pow( Q10_diff , ( ( fix_celsius - 30.0 ) / 10.0 ) ) ) ;
   rate ( _threadargscomma_ v ) ;
   s = s_inf ;
   f = f_inf ;
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
  ena = _ion_ena;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   g = gbar_Q10 * s * f ;
   ina = g * ( v - ena ) ;
   ic = ina ;
   }
 _current += ina;

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
  ena = _ion_ena;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dina;
  _dina = ina;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dinadv += (_dina - ina)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ina += ina ;
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
  ena = _ion_ena;
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
 _slist1[0] = {s_columnindex, 0};  _dlist1[0] = {Ds_columnindex, 0};
 _slist1[1] = {f_columnindex, 0};  _dlist1[1] = {Df_columnindex, 0};
 _slist2[0] = {f_columnindex, 0};
 _slist2[1] = {s_columnindex, 0};
   _t_s_inf = makevector(13001*sizeof(double));
   _t_tau_s = makevector(13001*sizeof(double));
   _t_f_inf = makevector(13001*sizeof(double));
   _t_tau_f = makevector(13001*sizeof(double));
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/oliver/GranularLayerModel/src/mod/GRANULE_NAR.mod";
    const char* nmodl_file_text = 
  "TITLE Cerebellum Granule Cell Model\n"
  "\n"
  "COMMENT\n"
  "        Na resurgent channel\n"
  "\n"
  "	Author: T.Nieus\n"
  "	Last revised: 30.6.2003\n"
  "	Critical value gNa\n"
  "	Inserted a control in bet_s to avoid huge values of x1\n"
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
  "	SUFFIX GRANULE_NAR\n"
  "	USEION na READ ena WRITE ina\n"
  "	RANGE Q10_diff,Q10_channel,gbar_Q10, fix_celsius\n"
  "	RANGE gbar, ic, g\n"
  "	RANGE Aalpha_s,Abeta_s,V0alpha_s,V0beta_s,Kalpha_s,Kbeta_s\n"
  "        RANGE Shiftalpha_s,Shiftbeta_s,tau_s,s_inf\n"
  "	RANGE Aalpha_f,Abeta_f,V0alpha_f,V0beta_f,Kalpha_f, Kbeta_f\n"
  "	RANGE tau_f,f_inf\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "\n"
  "	: s-ALFA\n"
  "	Aalpha_s = -0.00493 (/ms)\n"
  "	V0alpha_s = -4.48754 (mV)\n"
  "	Kalpha_s = -6.81881 (mV)\n"
  "	Shiftalpha_s = 0.00008 (/ms)\n"
  "\n"
  "	: s-BETA\n"
  "	Abeta_s = 0.01558 (/ms)\n"
  "	V0beta_s = 43.97494 (mV)\n"
  "	Kbeta_s =  0.10818 (mV)\n"
  "	Shiftbeta_s = 0.04752 (/ms)\n"
  "\n"
  "	: f-ALFA\n"
  "	Aalpha_f = 0.31836 (/ms)\n"
  "	V0alpha_f = -80 (mV)\n"
  "	Kalpha_f = -62.52621 (mV)\n"
  "\n"
  "	: f-BETA\n"
  "	Abeta_f = 0.01014 (/ms)\n"
  "	V0beta_f = -83.3332 (mV)\n"
  "	Kbeta_f = 16.05379 (mV)\n"
  "\n"
  "	v (mV)\n"
  "	gbar= 0.0002 (mho/cm2)\n"
  "	ena = 87.39 (mV)\n"
  "	Q10_diff	= 1.5\n"
  "	Q10_channel	= 3\n"
  "    fix_celsius = 37 (degC)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	s\n"
  "	f\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ina (mA/cm2)\n"
  "	ic (mA/cm2)\n"
  "	g (mho/cm2)\n"
  "\n"
  ":	alpha_s (/ms)\n"
  ":	beta_s (/ms)\n"
  "	s_inf\n"
  "	tau_s (ms)\n"
  "\n"
  ":	alpha_f (/ms)\n"
  ":	beta_f (/ms)\n"
  "	f_inf\n"
  "	tau_f (ms)\n"
  "	gbar_Q10 (mho/cm2)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	gbar_Q10 = gbar*(Q10_diff^((fix_celsius-30)/10))\n"
  "	rate(v)\n"
  "	s = s_inf\n"
  "	f = f_inf\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD derivimplicit\n"
  "	g = gbar_Q10*s*f\n"
  "	ina = g*(v - ena)\n"
  "	ic = ina\n"
  "\n"
  ":	alpha_s = alp_s(v)\n"
  ":	beta_s = bet_s(v)\n"
  ":\n"
  ":	alpha_f = alp_f(v)\n"
  ":	beta_f = bet_f(v)\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	rate(v)\n"
  "	s' = ( s_inf - s ) / tau_s\n"
  "	f' = ( f_inf - f ) / tau_f\n"
  "}\n"
  "\n"
  "PROCEDURE rate(v (mV)) { LOCAL a_s,b_s,a_f,b_f\n"
  "	TABLE s_inf,tau_s,f_inf,tau_f DEPEND fix_celsius FROM -100 TO 30 WITH 13000\n"
  "\n"
  "	a_s = alp_s(v)\n"
  "	b_s = bet_s(v)\n"
  "	s_inf = a_s / ( a_s + b_s )\n"
  "	tau_s = 1 / ( a_s + b_s )\n"
  "\n"
  "	a_f = alp_f(v)\n"
  "	b_f = bet_f(v)\n"
  "	f_inf = a_f / ( a_f + b_f )\n"
  "	tau_f = 1 / ( a_f + b_f )\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "FUNCTION alp_s(v (mV)) (/ms){ LOCAL Q10\n"
  "	Q10 = Q10_channel^( ( fix_celsius - 20 (degC) ) / 10 (degC) )\n"
  "	alp_s = Q10*(Shiftalpha_s+Aalpha_s*((v+V0alpha_s)/ 1 (mV) )/(exp((v+V0alpha_s)/Kalpha_s)-1))\n"
  "}\n"
  "\n"
  "FUNCTION bet_s(v (mV)) (/ms){ LOCAL Q10,x1\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  "\n"
  "	x1=(v+V0beta_s)/Kbeta_s\n"
  "	if (x1>200) {x1=200}\n"
  "	bet_s =Q10*(Shiftbeta_s+Abeta_s*((v+V0beta_s)/1 (mV) )/(exp(x1)-1))\n"
  "\n"
  "	:x1=(v+V0beta_s)/Kbeta_s\n"
  "	:if(x1>200){x1=200}\n"
  "	:(v+V0beta_s)/Kbeta_s)\n"
  "	:bet_s =	Q10*(Shiftbeta_s+Abeta_s*((v+V0beta_s)/1 (mV) )/(exp(x1-1))\n"
  "}\n"
  "\n"
  "FUNCTION alp_f(v (mV)) (/ms){ LOCAL Q10\n"
  "	Q10 = Q10_channel^( ( fix_celsius - 20 (degC) ) / 10 (degC) )\n"
  "	alp_f =	Q10 * Aalpha_f * exp( ( v - V0alpha_f ) / Kalpha_f)\n"
  "}\n"
  "\n"
  "FUNCTION bet_f(v (mV)) (/ms){ LOCAL Q10\n"
  "	Q10 = Q10_channel^( ( fix_celsius - 20 (degC) ) / 10 (degC) )\n"
  "	bet_f =	Q10 * Abeta_f * exp( ( v - V0beta_f ) / Kbeta_f )\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
