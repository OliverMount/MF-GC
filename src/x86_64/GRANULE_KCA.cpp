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
static constexpr auto number_of_datum_variables = 5;
static constexpr auto number_of_floating_point_variables = 27;
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
 
#define nrn_init _nrn_init__GRANULE_KCA
#define _nrn_initial _nrn_initial__GRANULE_KCA
#define nrn_cur _nrn_cur__GRANULE_KCA
#define _nrn_current _nrn_current__GRANULE_KCA
#define nrn_jacob _nrn_jacob__GRANULE_KCA
#define nrn_state _nrn_state__GRANULE_KCA
#define _net_receive _net_receive__GRANULE_KCA 
#define _f_exprate _f_exprate__GRANULE_KCA 
#define alp_c_bet_c alp_c_bet_c__GRANULE_KCA 
#define exprate exprate__GRANULE_KCA 
#define rate rate__GRANULE_KCA 
#define states states__GRANULE_KCA 
 
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
#define Aalpha_c _ml->template fpfield<0>(_iml)
#define Aalpha_c_columnindex 0
#define Balpha_c _ml->template fpfield<1>(_iml)
#define Balpha_c_columnindex 1
#define Kalpha_c _ml->template fpfield<2>(_iml)
#define Kalpha_c_columnindex 2
#define Abeta_c _ml->template fpfield<3>(_iml)
#define Abeta_c_columnindex 3
#define Bbeta_c _ml->template fpfield<4>(_iml)
#define Bbeta_c_columnindex 4
#define Kbeta_c _ml->template fpfield<5>(_iml)
#define Kbeta_c_columnindex 5
#define Q10_diff _ml->template fpfield<6>(_iml)
#define Q10_diff_columnindex 6
#define Q10_channel _ml->template fpfield<7>(_iml)
#define Q10_channel_columnindex 7
#define gbar _ml->template fpfield<8>(_iml)
#define gbar_columnindex 8
#define fix_celsius _ml->template fpfield<9>(_iml)
#define fix_celsius_columnindex 9
#define ic _ml->template fpfield<10>(_iml)
#define ic_columnindex 10
#define c_inf _ml->template fpfield<11>(_iml)
#define c_inf_columnindex 11
#define tau_c _ml->template fpfield<12>(_iml)
#define tau_c_columnindex 12
#define g _ml->template fpfield<13>(_iml)
#define g_columnindex 13
#define alpha_c _ml->template fpfield<14>(_iml)
#define alpha_c_columnindex 14
#define beta_c _ml->template fpfield<15>(_iml)
#define beta_c_columnindex 15
#define gbar_Q10 _ml->template fpfield<16>(_iml)
#define gbar_Q10_columnindex 16
#define c _ml->template fpfield<17>(_iml)
#define c_columnindex 17
#define cai _ml->template fpfield<18>(_iml)
#define cai_columnindex 18
#define ek _ml->template fpfield<19>(_iml)
#define ek_columnindex 19
#define Dc _ml->template fpfield<20>(_iml)
#define Dc_columnindex 20
#define ik _ml->template fpfield<21>(_iml)
#define ik_columnindex 21
#define tcorr _ml->template fpfield<22>(_iml)
#define tcorr_columnindex 22
#define bavc _ml->template fpfield<23>(_iml)
#define bavc_columnindex 23
#define bbvc _ml->template fpfield<24>(_iml)
#define bbvc_columnindex 24
#define v _ml->template fpfield<25>(_iml)
#define v_columnindex 25
#define _g _ml->template fpfield<26>(_iml)
#define _g_columnindex 26
#define _ion_ek *(_ml->dptr_field<0>(_iml))
#define _p_ion_ek static_cast<neuron::container::data_handle<double>>(_ppvar[0])
#define _ion_ik *(_ml->dptr_field<1>(_iml))
#define _p_ion_ik static_cast<neuron::container::data_handle<double>>(_ppvar[1])
#define _ion_dikdv *(_ml->dptr_field<2>(_iml))
#define _ion_cai *(_ml->dptr_field<3>(_iml))
#define _p_ion_cai static_cast<neuron::container::data_handle<double>>(_ppvar[3])
#define _ion_cao *(_ml->dptr_field<4>(_iml))
#define _p_ion_cao static_cast<neuron::container::data_handle<double>>(_ppvar[4])
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_alp_c_bet_c(void);
 static void _hoc_exprate(void);
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
 {"setdata_GRANULE_KCA", _hoc_setdata},
 {"alp_c_bet_c_GRANULE_KCA", _hoc_alp_c_bet_c},
 {"exprate_GRANULE_KCA", _hoc_exprate},
 {"rate_GRANULE_KCA", _hoc_rate},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alp_c_bet_c(Prop*);
 static double _npy_exprate(Prop*);
 static double _npy_rate(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alp_c_bet_c", _npy_alp_c_bet_c},
 {"exprate", _npy_exprate},
 {"rate", _npy_rate},
 {0, 0}
};
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define usetable usetable_GRANULE_KCA
 double usetable = 1;
 
static void _check_exprate(_internalthreadargsproto_); 
static void _check_table_thread(_threadargsprotocomma_ int _type, _nrn_model_sorted_token const& _sorted_token) {
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); } 
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml, _type};
  {
    auto* const _ml = &_lmr;
   _check_exprate(_threadargs_);
   }
}
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {"usetable_GRANULE_KCA", 0, 1},
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"Aalpha_c_GRANULE_KCA", "/ms"},
 {"Balpha_c_GRANULE_KCA", "mM"},
 {"Kalpha_c_GRANULE_KCA", "mV"},
 {"Abeta_c_GRANULE_KCA", "/ms"},
 {"Bbeta_c_GRANULE_KCA", "mM"},
 {"Kbeta_c_GRANULE_KCA", "mV"},
 {"gbar_GRANULE_KCA", "mho/cm2"},
 {"fix_celsius_GRANULE_KCA", "degC"},
 {"ic_GRANULE_KCA", "mA/cm2"},
 {"tau_c_GRANULE_KCA", "ms"},
 {"g_GRANULE_KCA", "mho/cm2"},
 {"alpha_c_GRANULE_KCA", "/ms"},
 {"beta_c_GRANULE_KCA", "/ms"},
 {"gbar_Q10_GRANULE_KCA", "mho/cm2"},
 {0, 0}
};
 static double c0 = 0;
 static double delta_t = 0.01;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"usetable_GRANULE_KCA", &usetable_GRANULE_KCA},
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
 
#define _cvode_ieq _ppvar[5].literal_value<int>()
 static void _ode_matsol_instance1(_internalthreadargsproto_);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"GRANULE_KCA",
 "Aalpha_c_GRANULE_KCA",
 "Balpha_c_GRANULE_KCA",
 "Kalpha_c_GRANULE_KCA",
 "Abeta_c_GRANULE_KCA",
 "Bbeta_c_GRANULE_KCA",
 "Kbeta_c_GRANULE_KCA",
 "Q10_diff_GRANULE_KCA",
 "Q10_channel_GRANULE_KCA",
 "gbar_GRANULE_KCA",
 "fix_celsius_GRANULE_KCA",
 0,
 "ic_GRANULE_KCA",
 "c_inf_GRANULE_KCA",
 "tau_c_GRANULE_KCA",
 "g_GRANULE_KCA",
 "alpha_c_GRANULE_KCA",
 "beta_c_GRANULE_KCA",
 "gbar_Q10_GRANULE_KCA",
 0,
 "c_GRANULE_KCA",
 0,
 0};
 static Symbol* _k_sym;
 static Symbol* _ca_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     2.5, /* Aalpha_c */
     0.0015, /* Balpha_c */
     -11.765, /* Kalpha_c */
     1.5, /* Abeta_c */
     0.00015, /* Bbeta_c */
     -11.765, /* Kbeta_c */
     1.5, /* Q10_diff */
     3, /* Q10_channel */
     0.0045, /* gbar */
     37, /* fix_celsius */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
   _ppvar = nrn_prop_datum_alloc(_mechtype, 6, _prop);
    _nrn_mechanism_access_dparam(_prop) = _ppvar;
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 27);
 	/*initialize range parameters*/
 	Aalpha_c = _parm_default[0]; /* 2.5 */
 	Balpha_c = _parm_default[1]; /* 0.0015 */
 	Kalpha_c = _parm_default[2]; /* -11.765 */
 	Abeta_c = _parm_default[3]; /* 1.5 */
 	Bbeta_c = _parm_default[4]; /* 0.00015 */
 	Kbeta_c = _parm_default[5]; /* -11.765 */
 	Q10_diff = _parm_default[6]; /* 1.5 */
 	Q10_channel = _parm_default[7]; /* 3 */
 	gbar = _parm_default[8]; /* 0.0045 */
 	fix_celsius = _parm_default[9]; /* 37 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 27);
 	_nrn_mechanism_access_dparam(_prop) = _ppvar;
 	/*connect ionic variables to this model*/
 prop_ion = need_memb(_k_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0] = _nrn_mechanism_get_param_handle(prop_ion, 0); /* ek */
 	_ppvar[1] = _nrn_mechanism_get_param_handle(prop_ion, 3); /* ik */
 	_ppvar[2] = _nrn_mechanism_get_param_handle(prop_ion, 4); /* _ion_dikdv */
 prop_ion = need_memb(_ca_sym);
 nrn_promote(prop_ion, 1, 0);
 	_ppvar[3] = _nrn_mechanism_get_param_handle(prop_ion, 1); /* cai */
 	_ppvar[4] = _nrn_mechanism_get_param_handle(prop_ion, 2); /* cao */
 
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

 extern "C" void _GRANULE_KCA_reg() {
	int _vectorized = 1;
  _initlists();
 	ion_reg("k", -10000.);
 	ion_reg("ca", -10000.);
 	_k_sym = hoc_lookup("k_ion");
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
                                       _nrn_mechanism_field<double>{"Aalpha_c"} /* 0 */,
                                       _nrn_mechanism_field<double>{"Balpha_c"} /* 1 */,
                                       _nrn_mechanism_field<double>{"Kalpha_c"} /* 2 */,
                                       _nrn_mechanism_field<double>{"Abeta_c"} /* 3 */,
                                       _nrn_mechanism_field<double>{"Bbeta_c"} /* 4 */,
                                       _nrn_mechanism_field<double>{"Kbeta_c"} /* 5 */,
                                       _nrn_mechanism_field<double>{"Q10_diff"} /* 6 */,
                                       _nrn_mechanism_field<double>{"Q10_channel"} /* 7 */,
                                       _nrn_mechanism_field<double>{"gbar"} /* 8 */,
                                       _nrn_mechanism_field<double>{"fix_celsius"} /* 9 */,
                                       _nrn_mechanism_field<double>{"ic"} /* 10 */,
                                       _nrn_mechanism_field<double>{"c_inf"} /* 11 */,
                                       _nrn_mechanism_field<double>{"tau_c"} /* 12 */,
                                       _nrn_mechanism_field<double>{"g"} /* 13 */,
                                       _nrn_mechanism_field<double>{"alpha_c"} /* 14 */,
                                       _nrn_mechanism_field<double>{"beta_c"} /* 15 */,
                                       _nrn_mechanism_field<double>{"gbar_Q10"} /* 16 */,
                                       _nrn_mechanism_field<double>{"c"} /* 17 */,
                                       _nrn_mechanism_field<double>{"cai"} /* 18 */,
                                       _nrn_mechanism_field<double>{"ek"} /* 19 */,
                                       _nrn_mechanism_field<double>{"Dc"} /* 20 */,
                                       _nrn_mechanism_field<double>{"ik"} /* 21 */,
                                       _nrn_mechanism_field<double>{"tcorr"} /* 22 */,
                                       _nrn_mechanism_field<double>{"bavc"} /* 23 */,
                                       _nrn_mechanism_field<double>{"bbvc"} /* 24 */,
                                       _nrn_mechanism_field<double>{"v"} /* 25 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 26 */,
                                       _nrn_mechanism_field<double*>{"_ion_ek", "k_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ik", "k_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dikdv", "k_ion"} /* 2 */,
                                       _nrn_mechanism_field<double*>{"_ion_cai", "ca_ion"} /* 3 */,
                                       _nrn_mechanism_field<double*>{"_ion_cao", "ca_ion"} /* 4 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 5 */);
  hoc_register_prop_size(_mechtype, 27, 6);
  hoc_register_dparam_semantics(_mechtype, 0, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "k_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 4, "ca_ion");
  hoc_register_dparam_semantics(_mechtype, 5, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 GRANULE_KCA /home/oliver/GranularLayerModel/src/mod/GRANULE_KCA.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_bavc;
 static double *_t_bbvc;
static int _reset;
static const char *modelname = "Cerebellum Granule Cell Model";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int _f_exprate(_internalthreadargsprotocomma_ double);
static int alp_c_bet_c(_internalthreadargsprotocomma_ double, double);
static int exprate(_internalthreadargsprotocomma_ double);
static int rate(_internalthreadargsprotocomma_ double, double);
 
#define _deriv1_advance _thread[0].literal_value<int>()
#define _dith1 1
#define _recurse _thread[2].literal_value<int>()
#define _newtonspace1 _thread[3].literal_value<NewtonSpace*>()
 
static int _ode_spec1(_internalthreadargsproto_);
/*static int _ode_matsol1(_internalthreadargsproto_);*/
 static void _n_exprate(_internalthreadargsprotocomma_ double _lv);
 static neuron::container::field_index _slist2[1];
  static neuron::container::field_index _slist1[1], _dlist1[1];
 static int states(_internalthreadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_internalthreadargsproto_) {int _reset = 0; {
   rate ( _threadargscomma_ v , cai ) ;
   Dc = ( c_inf - c ) / tau_c ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 rate ( _threadargscomma_ v , cai ) ;
 Dc = Dc  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_c )) ;
  return 0;
}
 /*END CVODE*/
 
static int states (_internalthreadargsproto_) {
  int _reset=0;
  int error = 0;
 {
  auto* _savstate1 =_thread[_dith1].get<double*>();
  auto* _dlist2 = _thread[_dith1].get<double*>() + 1;
  int _counte = -1;
 if (!_recurse) {
 _recurse = 1;
 for(int _id=0; _id < 1; _id++) {
  _savstate1[_id] = _ml->data(_iml, _slist1[_id]);
}
 error = nrn_newton_thread(_newtonspace1, 1, _slist2, neuron::scopmath::row_view{_ml, _iml}, states, _dlist2, _ml, _iml, _ppvar, _thread, _globals, _nt);
 _recurse = 0; if(error) {abort_run(error);}}
 {
   rate ( _threadargscomma_ v , cai ) ;
   Dc = ( c_inf - c ) / tau_c ;
   {int _id; for(_id=0; _id < 1; _id++) {
if (_deriv1_advance) {
 _dlist2[++_counte] = _ml->data(_iml, _dlist1[_id]) - (_ml->data(_iml, _slist1[_id]) - _savstate1[_id])/dt;
 }else{
_dlist2[++_counte] = _ml->data(_iml, _slist1[_id]) - _savstate1[_id];}}}
 } }
 return _reset;}
 static double _mfac_exprate, _tmin_exprate;
  static void _check_exprate(_internalthreadargsproto_) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_Balpha_c;
  static double _sav_Kalpha_c;
  static double _sav_Bbeta_c;
  static double _sav_Kbeta_c;
  if (!usetable) {return;}
  if (_sav_Balpha_c != Balpha_c) { _maktable = 1;}
  if (_sav_Kalpha_c != Kalpha_c) { _maktable = 1;}
  if (_sav_Bbeta_c != Bbeta_c) { _maktable = 1;}
  if (_sav_Kbeta_c != Kbeta_c) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_exprate =  - 100.0 ;
   _tmax =  30.0 ;
   _dx = (_tmax - _tmin_exprate)/13000.; _mfac_exprate = 1./_dx;
   for (_i=0, _x=_tmin_exprate; _i < 13001; _x += _dx, _i++) {
    _f_exprate(_threadargscomma_ _x);
    _t_bavc[_i] = bavc;
    _t_bbvc[_i] = bbvc;
   }
   _sav_Balpha_c = Balpha_c;
   _sav_Kalpha_c = Kalpha_c;
   _sav_Bbeta_c = Bbeta_c;
   _sav_Kbeta_c = Kbeta_c;
  }
 }

 static int exprate(_internalthreadargsprotocomma_ double _lv) { 
#if 0
_check_exprate(_threadargs_);
#endif
 _n_exprate(_threadargscomma_ _lv);
 return 0;
 }

 static void _n_exprate(_internalthreadargsprotocomma_ double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 _f_exprate(_threadargscomma_ _lv); return; 
}
 _xi = _mfac_exprate * (_lv - _tmin_exprate);
 if (std::isnan(_xi)) {
  bavc = _xi;
  bbvc = _xi;
  return;
 }
 if (_xi <= 0.) {
 bavc = _t_bavc[0];
 bbvc = _t_bbvc[0];
 return; }
 if (_xi >= 13000.) {
 bavc = _t_bavc[13000];
 bbvc = _t_bbvc[13000];
 return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 bavc = _t_bavc[_i] + _theta*(_t_bavc[_i+1] - _t_bavc[_i]);
 bbvc = _t_bbvc[_i] + _theta*(_t_bbvc[_i+1] - _t_bbvc[_i]);
 }

 
static int  _f_exprate ( _internalthreadargsprotocomma_ double _lv ) {
   bavc = Balpha_c * exp ( _lv / Kalpha_c ) ;
   bbvc = Bbeta_c * exp ( _lv / Kbeta_c ) ;
    return 0; }
 
static void _hoc_exprate(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for exprate_GRANULE_KCA. Requires prior call to setdata_GRANULE_KCA and that the specified mechanism instance still be in existence.", NULL);
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
 _check_exprate(_threadargs_);
#endif
 _r = 1.;
 exprate ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_exprate(Prop* _prop) {
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
 _check_exprate(_threadargs_);
#endif
 _r = 1.;
 exprate ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
static int  alp_c_bet_c ( _internalthreadargsprotocomma_ double _lv , double _lcai ) {
   exprate ( _threadargscomma_ _lv ) ;
   alpha_c = tcorr * Aalpha_c / ( 1.0 + ( bavc / _lcai ) ) ;
   beta_c = tcorr * Abeta_c / ( 1.0 + _lcai / bbvc ) ;
    return 0; }
 
static void _hoc_alp_c_bet_c(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_c_bet_c_GRANULE_KCA. Requires prior call to setdata_GRANULE_KCA and that the specified mechanism instance still be in existence.", NULL);
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
 _r = 1.;
 alp_c_bet_c ( _threadargscomma_ *getarg(1) , *getarg(2) );
 hoc_retpushx(_r);
}
 
static double _npy_alp_c_bet_c(Prop* _prop) {
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
 _r = 1.;
 alp_c_bet_c ( _threadargscomma_ *getarg(1) , *getarg(2) );
 return(_r);
}
 
static int  rate ( _internalthreadargsprotocomma_ double _lv , double _lcai ) {
   alp_c_bet_c ( _threadargscomma_ _lv , _lcai ) ;
   tau_c = 1.0 / ( alpha_c + beta_c ) ;
   c_inf = alpha_c / ( alpha_c + beta_c ) ;
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rate_GRANULE_KCA. Requires prior call to setdata_GRANULE_KCA and that the specified mechanism instance still be in existence.", NULL);
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
 _r = 1.;
 rate ( _threadargscomma_ *getarg(1) , *getarg(2) );
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
 _r = 1.;
 rate ( _threadargscomma_ *getarg(1) , *getarg(2) );
 return(_r);
}
 
static int _ode_count(int _type){ return 1;}
 
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
  cai = _ion_cai;
     _ode_spec1 (_threadargs_);
  }}
 
static void _ode_map(Prop* _prop, int _ieq, neuron::container::data_handle<double>* _pv, neuron::container::data_handle<double>* _pvdot, double* _atol, int _type) { 
  Datum* _ppvar;
  _ppvar = _nrn_mechanism_access_dparam(_prop);
  _cvode_ieq = _ieq;
  for (int _i=0; _i < 1; ++_i) {
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
  cai = _ion_cai;
 _ode_matsol_instance1(_threadargs_);
 }}
 
static void _thread_mem_init(Datum* _thread) {
   _thread[_dith1] = new double[2]{};
   _newtonspace1 = nrn_cons_newtonspace(1);
 }
 
static void _thread_cleanup(Datum* _thread) {
   delete[] _thread[_dith1].get<double*>();
   nrn_destroy_newtonspace(_newtonspace1);
 }

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
  c = c0;
 {
   gbar_Q10 = gbar * ( pow( Q10_diff , ( ( fix_celsius - 30.0 ) / 10.0 ) ) ) ;
   tcorr = pow( Q10_channel , ( ( fix_celsius - 30.0 ) / 10.0 ) ) ;
   rate ( _threadargscomma_ v , cai ) ;
   c = c_inf ;
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
 _check_exprate(_threadargs_);
#endif
   _v = _vec_v[_ni[_iml]];
 v = _v;
  ek = _ion_ek;
  cai = _ion_cai;
 initmodel(_threadargs_);
 }
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   g = gbar_Q10 * c ;
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
  cai = _ion_cai;
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
  cai = _ion_cai;
 {  _deriv1_advance = 1;
 derivimplicit_thread(1, _slist1, _dlist1, neuron::scopmath::row_view{_ml, _iml}, states, _ml, _iml, _ppvar, _thread, _globals, _nt);
_deriv1_advance = 0;
     if (secondorder) {
    int _i;
    for (_i = 0; _i < 1; ++_i) {
      _ml->data(_iml, _slist1[_i]) += dt*_ml->data(_iml, _dlist1[_i]);
    }}
 } }}
 dt = _dtsav;
}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = {c_columnindex, 0};  _dlist1[0] = {Dc_columnindex, 0};
 _slist2[0] = {c_columnindex, 0};
   _t_bavc = makevector(13001*sizeof(double));
   _t_bbvc = makevector(13001*sizeof(double));
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/oliver/GranularLayerModel/src/mod/GRANULE_KCA.mod";
    const char* nmodl_file_text = 
  "TITLE Cerebellum Granule Cell Model\n"
  "\n"
  "COMMENT\n"
  "        KCa channel\n"
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
  "	SUFFIX GRANULE_KCA\n"
  "	USEION k READ ek WRITE ik\n"
  "	USEION ca READ cai\n"
  "	RANGE Q10_diff,Q10_channel,gbar_Q10, fix_celsius\n"
  "	RANGE gbar, ic, g, alpha_c, beta_c\n"
  "	RANGE Aalpha_c, Balpha_c, Kalpha_c\n"
  "	RANGE Abeta_c, Bbeta_c, Kbeta_c\n"
  "	RANGE c_inf, tau_c\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "	(molar) = (1/liter)\n"
  "	(mM) = (millimolar)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	Aalpha_c = 2.5 (/ms)\n"
  "	Balpha_c = 1.5e-3 (mM)\n"
  "\n"
  "	Kalpha_c =  -11.765 (mV)\n"
  "\n"
  "	Abeta_c = 1.5 (/ms)\n"
  "	Bbeta_c = 0.15e-3 (mM)\n"
  "\n"
  "	Kbeta_c = -11.765 (mV)\n"
  "\n"
  "	v (mV)\n"
  "	cai (mM)\n"
  "	Q10_diff	= 1.5\n"
  "	Q10_channel	= 3\n"
  "	gbar= 0.0045 (mho/cm2)\n"
  "	ek = -84.69 (mV)\n"
  "    fix_celsius = 37 (degC)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	c\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ic (mA/cm2)\n"
  "	ik (mA/cm2)\n"
  "\n"
  "	c_inf\n"
  "	tau_c (ms)\n"
  "	g (mho/cm2)\n"
  "	alpha_c (/ms)\n"
  "	beta_c (/ms)\n"
  "	gbar_Q10 (mho/cm2)\n"
  "\n"
  "  tcorr (1)\n"
  "\n"
  "  bavc (mM)\n"
  "  bbvc (mM)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	gbar_Q10 = gbar*(Q10_diff^((fix_celsius-30)/10))\n"
  "	tcorr = Q10_channel^((fix_celsius-30(degC))/10(degC))\n"
  "	rate(v, cai)\n"
  "	c = c_inf\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD derivimplicit\n"
  "	g = gbar_Q10*c\n"
  "	ik = g*(v - ek)\n"
  "	ic = ik\n"
  ":  alp_c_bet_c(v, cai)\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	rate(v, cai)\n"
  "	c' = (c_inf - c)/tau_c\n"
  "}\n"
  "\n"
  "PROCEDURE exprate(v(mv))(mM) {\n"
  "  TABLE bavc, bbvc DEPEND Balpha_c, Kalpha_c, Bbeta_c, Kbeta_c FROM -100 TO 30 WITH 13000\n"
  "  bavc = Balpha_c*exp(v/Kalpha_c)\n"
  "  bbvc = Bbeta_c*exp(v/Kbeta_c)\n"
  "}\n"
  "\n"
  "PROCEDURE alp_c_bet_c(v(mV), cai(mM))(/ms) {\n"
  "	exprate(v)\n"
  "	alpha_c = tcorr*Aalpha_c/(1+(bavc/cai))\n"
  "	beta_c = tcorr*Abeta_c/(1+cai/bbvc)\n"
  "}\n"
  "\n"
  "PROCEDURE rate(v (mV), cai(mM)) {\n"
  "	alp_c_bet_c(v, cai)\n"
  "	tau_c = 1/(alpha_c + beta_c)\n"
  "	c_inf = alpha_c/(alpha_c + beta_c)\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
