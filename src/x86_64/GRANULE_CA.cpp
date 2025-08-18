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
static constexpr auto number_of_floating_point_variables = 31;
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
 
#define nrn_init _nrn_init__GRANULE_CA
#define _nrn_initial _nrn_initial__GRANULE_CA
#define nrn_cur _nrn_cur__GRANULE_CA
#define _nrn_current _nrn_current__GRANULE_CA
#define nrn_jacob _nrn_jacob__GRANULE_CA
#define nrn_state _nrn_state__GRANULE_CA
#define _net_receive _net_receive__GRANULE_CA 
#define _f_rate _f_rate__GRANULE_CA 
#define rate rate__GRANULE_CA 
#define states states__GRANULE_CA 
 
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
#define Kalpha_s _ml->template fpfield<1>(_iml)
#define Kalpha_s_columnindex 1
#define V0alpha_s _ml->template fpfield<2>(_iml)
#define V0alpha_s_columnindex 2
#define Abeta_s _ml->template fpfield<3>(_iml)
#define Abeta_s_columnindex 3
#define Kbeta_s _ml->template fpfield<4>(_iml)
#define Kbeta_s_columnindex 4
#define V0beta_s _ml->template fpfield<5>(_iml)
#define V0beta_s_columnindex 5
#define Aalpha_u _ml->template fpfield<6>(_iml)
#define Aalpha_u_columnindex 6
#define Kalpha_u _ml->template fpfield<7>(_iml)
#define Kalpha_u_columnindex 7
#define V0alpha_u _ml->template fpfield<8>(_iml)
#define V0alpha_u_columnindex 8
#define Abeta_u _ml->template fpfield<9>(_iml)
#define Abeta_u_columnindex 9
#define Kbeta_u _ml->template fpfield<10>(_iml)
#define Kbeta_u_columnindex 10
#define V0beta_u _ml->template fpfield<11>(_iml)
#define V0beta_u_columnindex 11
#define Q10_diff _ml->template fpfield<12>(_iml)
#define Q10_diff_columnindex 12
#define Q10_channel _ml->template fpfield<13>(_iml)
#define Q10_channel_columnindex 13
#define gbar _ml->template fpfield<14>(_iml)
#define gbar_columnindex 14
#define fix_celsius _ml->template fpfield<15>(_iml)
#define fix_celsius_columnindex 15
#define ica _ml->template fpfield<16>(_iml)
#define ica_columnindex 16
#define ic _ml->template fpfield<17>(_iml)
#define ic_columnindex 17
#define s_inf _ml->template fpfield<18>(_iml)
#define s_inf_columnindex 18
#define u_inf _ml->template fpfield<19>(_iml)
#define u_inf_columnindex 19
#define tau_s _ml->template fpfield<20>(_iml)
#define tau_s_columnindex 20
#define tau_u _ml->template fpfield<21>(_iml)
#define tau_u_columnindex 21
#define g _ml->template fpfield<22>(_iml)
#define g_columnindex 22
#define gbar_Q10 _ml->template fpfield<23>(_iml)
#define gbar_Q10_columnindex 23
#define s _ml->template fpfield<24>(_iml)
#define s_columnindex 24
#define u _ml->template fpfield<25>(_iml)
#define u_columnindex 25
#define eca _ml->template fpfield<26>(_iml)
#define eca_columnindex 26
#define Ds _ml->template fpfield<27>(_iml)
#define Ds_columnindex 27
#define Du _ml->template fpfield<28>(_iml)
#define Du_columnindex 28
#define v _ml->template fpfield<29>(_iml)
#define v_columnindex 29
#define _g _ml->template fpfield<30>(_iml)
#define _g_columnindex 30
#define _ion_eca *(_ml->dptr_field<0>(_iml))
#define _p_ion_eca static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ica *(_ml->dptr_field<1>(_iml))
#define _p_ion_ica static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dicadv *(_ml->dptr_field<2>(_iml))
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_alp_u(void);
 static void _hoc_alp_s(void);
 static void _hoc_bet_u(void);
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
 {"setdata_GRANULE_CA", _hoc_setdata},
 {"alp_u_GRANULE_CA", _hoc_alp_u},
 {"alp_s_GRANULE_CA", _hoc_alp_s},
 {"bet_u_GRANULE_CA", _hoc_bet_u},
 {"bet_s_GRANULE_CA", _hoc_bet_s},
 {"rate_GRANULE_CA", _hoc_rate},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alp_u(Prop*);
 static double _npy_alp_s(Prop*);
 static double _npy_bet_u(Prop*);
 static double _npy_bet_s(Prop*);
 static double _npy_rate(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alp_u", _npy_alp_u},
 {"alp_s", _npy_alp_s},
 {"bet_u", _npy_bet_u},
 {"bet_s", _npy_bet_s},
 {"rate", _npy_rate},
 {0, 0}
};
#define alp_u alp_u_GRANULE_CA
#define alp_s alp_s_GRANULE_CA
#define bet_u bet_u_GRANULE_CA
#define bet_s bet_s_GRANULE_CA
 extern double alp_u( _internalthreadargsprotocomma_ double );
 extern double alp_s( _internalthreadargsprotocomma_ double );
 extern double bet_u( _internalthreadargsprotocomma_ double );
 extern double bet_s( _internalthreadargsprotocomma_ double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define usetable usetable_GRANULE_CA
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
 {"usetable_GRANULE_CA", 0, 1},
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"Aalpha_s_GRANULE_CA", "/ms"},
 {"Kalpha_s_GRANULE_CA", "mV"},
 {"V0alpha_s_GRANULE_CA", "mV"},
 {"Abeta_s_GRANULE_CA", "/ms"},
 {"Kbeta_s_GRANULE_CA", "mV"},
 {"V0beta_s_GRANULE_CA", "mV"},
 {"Aalpha_u_GRANULE_CA", "/ms"},
 {"Kalpha_u_GRANULE_CA", "mV"},
 {"V0alpha_u_GRANULE_CA", "mV"},
 {"Abeta_u_GRANULE_CA", "/ms"},
 {"Kbeta_u_GRANULE_CA", "mV"},
 {"V0beta_u_GRANULE_CA", "mV"},
 {"gbar_GRANULE_CA", "mho/cm2"},
 {"fix_celsius_GRANULE_CA", "degC"},
 {"ica_GRANULE_CA", "mA/cm2"},
 {"ic_GRANULE_CA", "mA/cm2"},
 {"tau_s_GRANULE_CA", "ms"},
 {"tau_u_GRANULE_CA", "ms"},
 {"g_GRANULE_CA", "mho/cm2"},
 {"gbar_Q10_GRANULE_CA", "mho/cm2"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double s0 = 0;
 static double u0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"usetable_GRANULE_CA", &usetable_GRANULE_CA},
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
"GRANULE_CA",
 "Aalpha_s_GRANULE_CA",
 "Kalpha_s_GRANULE_CA",
 "V0alpha_s_GRANULE_CA",
 "Abeta_s_GRANULE_CA",
 "Kbeta_s_GRANULE_CA",
 "V0beta_s_GRANULE_CA",
 "Aalpha_u_GRANULE_CA",
 "Kalpha_u_GRANULE_CA",
 "V0alpha_u_GRANULE_CA",
 "Abeta_u_GRANULE_CA",
 "Kbeta_u_GRANULE_CA",
 "V0beta_u_GRANULE_CA",
 "Q10_diff_GRANULE_CA",
 "Q10_channel_GRANULE_CA",
 "gbar_GRANULE_CA",
 "fix_celsius_GRANULE_CA",
 0,
 "ica_GRANULE_CA",
 "ic_GRANULE_CA",
 "s_inf_GRANULE_CA",
 "u_inf_GRANULE_CA",
 "tau_s_GRANULE_CA",
 "tau_u_GRANULE_CA",
 "g_GRANULE_CA",
 "gbar_Q10_GRANULE_CA",
 0,
 "s_GRANULE_CA",
 "u_GRANULE_CA",
 0,
 0};
 static Symbol* _ca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     0.04944, /* Aalpha_s */
     15.873, /* Kalpha_s */
     -29.06, /* V0alpha_s */
     0.08298, /* Abeta_s */
     -25.641, /* Kbeta_s */
     -18.66, /* V0beta_s */
     0.0013, /* Aalpha_u */
     -18.183, /* Kalpha_u */
     -48, /* V0alpha_u */
     0.0013, /* Abeta_u */
     83.33, /* Kbeta_u */
     -48, /* V0beta_u */
     1.5, /* Q10_diff */
     3, /* Q10_channel */
     0.00046, /* gbar */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 31);
 	/*initialize range parameters*/
 	Aalpha_s = _parm_default[0]; /* 0.04944 */
 	Kalpha_s = _parm_default[1]; /* 15.873 */
 	V0alpha_s = _parm_default[2]; /* -29.06 */
 	Abeta_s = _parm_default[3]; /* 0.08298 */
 	Kbeta_s = _parm_default[4]; /* -25.641 */
 	V0beta_s = _parm_default[5]; /* -18.66 */
 	Aalpha_u = _parm_default[6]; /* 0.0013 */
 	Kalpha_u = _parm_default[7]; /* -18.183 */
 	V0alpha_u = _parm_default[8]; /* -48 */
 	Abeta_u = _parm_default[9]; /* 0.0013 */
 	Kbeta_u = _parm_default[10]; /* 83.33 */
 	V0beta_u = _parm_default[11]; /* -48 */
 	Q10_diff = _parm_default[12]; /* 1.5 */
 	Q10_channel = _parm_default[13]; /* 3 */
 	gbar = _parm_default[14]; /* 0.00046 */
 	fix_celsius = _parm_default[15]; /* 37 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 31);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* eca */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ica */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dicadv */
 
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

 extern "C" void _GRANULE_CA_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("ca", -10000.);
 	_ca_sym = hoc_lookup("ca_ion");
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
                                       _nrn_mechanism_field<double>{"Kalpha_s"} /* 1 */,
                                       _nrn_mechanism_field<double>{"V0alpha_s"} /* 2 */,
                                       _nrn_mechanism_field<double>{"Abeta_s"} /* 3 */,
                                       _nrn_mechanism_field<double>{"Kbeta_s"} /* 4 */,
                                       _nrn_mechanism_field<double>{"V0beta_s"} /* 5 */,
                                       _nrn_mechanism_field<double>{"Aalpha_u"} /* 6 */,
                                       _nrn_mechanism_field<double>{"Kalpha_u"} /* 7 */,
                                       _nrn_mechanism_field<double>{"V0alpha_u"} /* 8 */,
                                       _nrn_mechanism_field<double>{"Abeta_u"} /* 9 */,
                                       _nrn_mechanism_field<double>{"Kbeta_u"} /* 10 */,
                                       _nrn_mechanism_field<double>{"V0beta_u"} /* 11 */,
                                       _nrn_mechanism_field<double>{"Q10_diff"} /* 12 */,
                                       _nrn_mechanism_field<double>{"Q10_channel"} /* 13 */,
                                       _nrn_mechanism_field<double>{"gbar"} /* 14 */,
                                       _nrn_mechanism_field<double>{"fix_celsius"} /* 15 */,
                                       _nrn_mechanism_field<double>{"ica"} /* 16 */,
                                       _nrn_mechanism_field<double>{"ic"} /* 17 */,
                                       _nrn_mechanism_field<double>{"s_inf"} /* 18 */,
                                       _nrn_mechanism_field<double>{"u_inf"} /* 19 */,
                                       _nrn_mechanism_field<double>{"tau_s"} /* 20 */,
                                       _nrn_mechanism_field<double>{"tau_u"} /* 21 */,
                                       _nrn_mechanism_field<double>{"g"} /* 22 */,
                                       _nrn_mechanism_field<double>{"gbar_Q10"} /* 23 */,
                                       _nrn_mechanism_field<double>{"s"} /* 24 */,
                                       _nrn_mechanism_field<double>{"u"} /* 25 */,
                                       _nrn_mechanism_field<double>{"eca"} /* 26 */,
                                       _nrn_mechanism_field<double>{"Ds"} /* 27 */,
                                       _nrn_mechanism_field<double>{"Du"} /* 28 */,
                                       _nrn_mechanism_field<double>{"v"} /* 29 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 30 */,
                                       _nrn_mechanism_field<double*>{"_ion_eca", "ca_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ica", "ca_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dicadv", "ca_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 31, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 GRANULE_CA /home/oliver/GranularLayerModel/src/mod/GRANULE_CA.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_s_inf;
 static double *_t_tau_s;
 static double *_t_u_inf;
 static double *_t_tau_u;
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
   Du = ( u_inf - u ) / tau_u ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 rate ( _threadargscomma_ v ) ;
 Ds = Ds  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_s )) ;
 Du = Du  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_u )) ;
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
   Du = ( u_inf - u ) / tau_u ;
   {int _id; for(_id=0; _id < 2; _id++) {
if (_deriv1_advance) {
 _dlist2[++_counte] = _ml->data(_iml, _dlist1[_id]) - (_ml->data(_iml, _slist1[_id]) - _savstate1[_id])/dt;
 }else{
_dlist2[++_counte] = _ml->data(_iml, _slist1[_id]) - _savstate1[_id];}}}
 } }
 return _reset;}
 
double alp_s ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalp_s;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lalp_s = _lQ10 * Aalpha_s * exp ( ( _lv - V0alpha_s ) / Kalpha_s ) ;
   
return _lalp_s;
 }
 
static void _hoc_alp_s(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_s_GRANULE_CA. Requires prior call to setdata_GRANULE_CA and that the specified mechanism instance still be in existence.", NULL);
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
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lbet_s = _lQ10 * Abeta_s * exp ( ( _lv - V0beta_s ) / Kbeta_s ) ;
   
return _lbet_s;
 }
 
static void _hoc_bet_s(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for bet_s_GRANULE_CA. Requires prior call to setdata_GRANULE_CA and that the specified mechanism instance still be in existence.", NULL);
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
 
double alp_u ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalp_u;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lalp_u = _lQ10 * Aalpha_u * exp ( ( _lv - V0alpha_u ) / Kalpha_u ) ;
   
return _lalp_u;
 }
 
static void _hoc_alp_u(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_u_GRANULE_CA. Requires prior call to setdata_GRANULE_CA and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  alp_u ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alp_u(Prop* _prop) {
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
 _r =  alp_u ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bet_u ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbet_u;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lbet_u = _lQ10 * Abeta_u * exp ( ( _lv - V0beta_u ) / Kbeta_u ) ;
   
return _lbet_u;
 }
 
static void _hoc_bet_u(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for bet_u_GRANULE_CA. Requires prior call to setdata_GRANULE_CA and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  bet_u ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bet_u(Prop* _prop) {
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
 _r =  bet_u ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 static double _mfac_rate, _tmin_rate;
  static void _check_rate(_internalthreadargsproto_) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_Aalpha_s;
  static double _sav_Kalpha_s;
  static double _sav_V0alpha_s;
  static double _sav_Abeta_s;
  static double _sav_Kbeta_s;
  static double _sav_V0beta_s;
  static double _sav_Aalpha_u;
  static double _sav_Kalpha_u;
  static double _sav_V0alpha_u;
  static double _sav_Abeta_u;
  static double _sav_Kbeta_u;
  static double _sav_V0beta_u;
  static double _sav_fix_celsius;
  if (!usetable) {return;}
  if (_sav_Aalpha_s != Aalpha_s) { _maktable = 1;}
  if (_sav_Kalpha_s != Kalpha_s) { _maktable = 1;}
  if (_sav_V0alpha_s != V0alpha_s) { _maktable = 1;}
  if (_sav_Abeta_s != Abeta_s) { _maktable = 1;}
  if (_sav_Kbeta_s != Kbeta_s) { _maktable = 1;}
  if (_sav_V0beta_s != V0beta_s) { _maktable = 1;}
  if (_sav_Aalpha_u != Aalpha_u) { _maktable = 1;}
  if (_sav_Kalpha_u != Kalpha_u) { _maktable = 1;}
  if (_sav_V0alpha_u != V0alpha_u) { _maktable = 1;}
  if (_sav_Abeta_u != Abeta_u) { _maktable = 1;}
  if (_sav_Kbeta_u != Kbeta_u) { _maktable = 1;}
  if (_sav_V0beta_u != V0beta_u) { _maktable = 1;}
  if (_sav_fix_celsius != fix_celsius) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_rate =  - 100.0 ;
   _tmax =  30.0 ;
   _dx = (_tmax - _tmin_rate)/13000.; _mfac_rate = 1./_dx;
   for (_i=0, _x=_tmin_rate; _i < 13001; _x += _dx, _i++) {
    _f_rate(_threadargscomma_ _x);
    _t_s_inf[_i] = s_inf;
    _t_tau_s[_i] = tau_s;
    _t_u_inf[_i] = u_inf;
    _t_tau_u[_i] = tau_u;
   }
   _sav_Aalpha_s = Aalpha_s;
   _sav_Kalpha_s = Kalpha_s;
   _sav_V0alpha_s = V0alpha_s;
   _sav_Abeta_s = Abeta_s;
   _sav_Kbeta_s = Kbeta_s;
   _sav_V0beta_s = V0beta_s;
   _sav_Aalpha_u = Aalpha_u;
   _sav_Kalpha_u = Kalpha_u;
   _sav_V0alpha_u = V0alpha_u;
   _sav_Abeta_u = Abeta_u;
   _sav_Kbeta_u = Kbeta_u;
   _sav_V0beta_u = V0beta_u;
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
  u_inf = _xi;
  tau_u = _xi;
  return;
 }
 if (_xi <= 0.) {
 s_inf = _t_s_inf[0];
 tau_s = _t_tau_s[0];
 u_inf = _t_u_inf[0];
 tau_u = _t_tau_u[0];
 return; }
 if (_xi >= 13000.) {
 s_inf = _t_s_inf[13000];
 tau_s = _t_tau_s[13000];
 u_inf = _t_u_inf[13000];
 tau_u = _t_tau_u[13000];
 return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 s_inf = _t_s_inf[_i] + _theta*(_t_s_inf[_i+1] - _t_s_inf[_i]);
 tau_s = _t_tau_s[_i] + _theta*(_t_tau_s[_i+1] - _t_tau_s[_i]);
 u_inf = _t_u_inf[_i] + _theta*(_t_u_inf[_i+1] - _t_u_inf[_i]);
 tau_u = _t_tau_u[_i] + _theta*(_t_tau_u[_i+1] - _t_tau_u[_i]);
 }

 
static int  _f_rate ( _internalthreadargsprotocomma_ double _lv ) {
   double _la_s , _lb_s , _la_u , _lb_u ;
 _la_s = alp_s ( _threadargscomma_ _lv ) ;
   _lb_s = bet_s ( _threadargscomma_ _lv ) ;
   _la_u = alp_u ( _threadargscomma_ _lv ) ;
   _lb_u = bet_u ( _threadargscomma_ _lv ) ;
   s_inf = _la_s / ( _la_s + _lb_s ) ;
   tau_s = 1.0 / ( _la_s + _lb_s ) ;
   u_inf = _la_u / ( _la_u + _lb_u ) ;
   tau_u = 1.0 / ( _la_u + _lb_u ) ;
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rate_GRANULE_CA. Requires prior call to setdata_GRANULE_CA and that the specified mechanism instance still be in existence.", NULL);
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
  eca = _ion_eca;
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
  eca = _ion_eca;
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
  s = s0;
  u = u0;
 {
   gbar_Q10 = gbar * ( pow( Q10_diff , ( ( fix_celsius - 30.0 ) / 10.0 ) ) ) ;
   rate ( _threadargscomma_ v ) ;
   s = s_inf ;
   u = u_inf ;
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
  eca = _ion_eca;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   g = gbar_Q10 * s * s * u ;
   ica = g * ( v - eca ) ;
   ic = ica ;
   }
 _current += ica;

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
  eca = _ion_eca;
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ double _dica;
  _dica = ica;
 _rhs = _nrn_current(_threadargscomma_ _v);
  _ion_dicadv += (_dica - ica)/.001 ;
 	}
 _g = (_g_local - _rhs)/.001;
  _ion_ica += ica ;
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
  eca = _ion_eca;
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
 _slist1[1] = {u_columnindex, 0};  _dlist1[1] = {Du_columnindex, 0};
 _slist2[0] = {s_columnindex, 0};
 _slist2[1] = {u_columnindex, 0};
   _t_s_inf = makevector(13001*sizeof(double));
   _t_tau_s = makevector(13001*sizeof(double));
   _t_u_inf = makevector(13001*sizeof(double));
   _t_tau_u = makevector(13001*sizeof(double));
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/oliver/GranularLayerModel/src/mod/GRANULE_CA.mod";
    const char* nmodl_file_text = 
  "TITLE Cerebellum Granule Cell Model\n"
  "\n"
  "COMMENT\n"
  "        CaHVA channel\n"
  "\n"
  "	Author: E.D'Angelo, T.Nieus, A. Fontana\n"
  "	Last revised: 8.5.2000\n"
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
  "	SUFFIX GRANULE_CA\n"
  "	USEION ca READ eca WRITE ica\n"
  "	RANGE Q10_diff,Q10_channel,gbar_Q10, fix_celsius\n"
  "	RANGE gbar, ica, ic , g :, alpha_s, beta_s, alpha_u, beta_u\n"
  "	RANGE Aalpha_s, Kalpha_s, V0alpha_s\n"
  "	RANGE Abeta_s, Kbeta_s, V0beta_s\n"
  "	RANGE Aalpha_u, Kalpha_u, V0alpha_u\n"
  "	RANGE Abeta_u, Kbeta_u, V0beta_u\n"
  "	RANGE s_inf, tau_s, u_inf, tau_u\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "\n"
  "	Aalpha_s = 0.04944 (/ms)\n"
  "	Kalpha_s =  15.873 (mV)\n"
  "	V0alpha_s = -29.06 (mV)\n"
  "\n"
  "	Abeta_s = 0.08298 (/ms)\n"
  "	Kbeta_s =  -25.641 (mV)\n"
  "	V0beta_s = -18.66 (mV)\n"
  "\n"
  "\n"
  "\n"
  "	Aalpha_u = 0.0013 (/ms)\n"
  "	Kalpha_u =  -18.183 (mV)\n"
  "	V0alpha_u = -48 (mV)\n"
  "\n"
  "	Abeta_u = 0.0013 (/ms)\n"
  "	Kbeta_u =   83.33 (mV)\n"
  "	V0beta_u = -48 (mV)\n"
  "\n"
  "	v (mV)\n"
  "	Q10_diff	= 1.5\n"
  "	Q10_channel	= 3\n"
  "	gbar= 0.00046 (mho/cm2)\n"
  "	eca (mV)\n"
  "    fix_celsius = 37 (degC)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	s\n"
  "	u\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ica (mA/cm2)\n"
  "	ic (mA/cm2)\n"
  "	s_inf\n"
  "	u_inf\n"
  "	tau_s (ms)\n"
  "	tau_u (ms)\n"
  "	g (mho/cm2)\n"
  "	gbar_Q10 (mho/cm2)\n"
  ":	alpha_s (/ms)\n"
  ":	beta_s (/ms)\n"
  ":	alpha_u (/ms)\n"
  ":	beta_u (/ms)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	gbar_Q10 = gbar*(Q10_diff^((fix_celsius-30)/10))\n"
  "	rate(v)\n"
  "	s = s_inf\n"
  "	u = u_inf\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD derivimplicit\n"
  "	g = gbar_Q10*s*s*u\n"
  "	ica = g*(v - eca)\n"
  "	ic = ica\n"
  ":	alpha_s = alp_s(v)\n"
  ":	beta_s = bet_s(v)\n"
  ":	alpha_u = alp_u(v)\n"
  ":	beta_u = bet_u(v)\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	rate(v)\n"
  "	s' =(s_inf - s)/tau_s\n"
  "	u' =(u_inf - u)/tau_u\n"
  "}\n"
  "\n"
  "FUNCTION alp_s(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  "	alp_s = Q10*Aalpha_s*exp((v-V0alpha_s)/Kalpha_s)\n"
  "}\n"
  "\n"
  "FUNCTION bet_s(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  "	bet_s = Q10*Abeta_s*exp((v-V0beta_s)/Kbeta_s)\n"
  "}\n"
  "\n"
  "FUNCTION alp_u(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  "	alp_u = Q10*Aalpha_u*exp((v-V0alpha_u)/Kalpha_u)\n"
  "}\n"
  "\n"
  "FUNCTION bet_u(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel^((fix_celsius-20(degC))/10(degC))\n"
  "	bet_u = Q10*Abeta_u*exp((v-V0beta_u)/Kbeta_u)\n"
  "}\n"
  "\n"
  "PROCEDURE rate(v (mV)) {LOCAL a_s, b_s, a_u, b_u\n"
  "	TABLE s_inf, tau_s, u_inf, tau_u\n"
  "	DEPEND Aalpha_s, Kalpha_s, V0alpha_s,\n"
  "	       Abeta_s, Kbeta_s, V0beta_s,\n"
  "               Aalpha_u, Kalpha_u, V0alpha_u,\n"
  "               Abeta_u, Kbeta_u, V0beta_u, fix_celsius FROM -100 TO 30 WITH 13000\n"
  "	a_s = alp_s(v)\n"
  "	b_s = bet_s(v)\n"
  "	a_u = alp_u(v)\n"
  "	b_u = bet_u(v)\n"
  "	s_inf = a_s/(a_s + b_s)\n"
  "	tau_s = 1/(a_s + b_s)\n"
  "	u_inf = a_u/(a_u + b_u)\n"
  "	tau_u = 1/(a_u + b_u)\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
