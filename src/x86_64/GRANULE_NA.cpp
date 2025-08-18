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
static constexpr auto number_of_floating_point_variables = 34;
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
 
#define nrn_init _nrn_init__GRANULE_NA
#define _nrn_initial _nrn_initial__GRANULE_NA
#define nrn_cur _nrn_cur__GRANULE_NA
#define _nrn_current _nrn_current__GRANULE_NA
#define nrn_jacob _nrn_jacob__GRANULE_NA
#define nrn_state _nrn_state__GRANULE_NA
#define _net_receive _net_receive__GRANULE_NA 
#define _f_rate _f_rate__GRANULE_NA 
#define rate rate__GRANULE_NA 
#define states states__GRANULE_NA 
 
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
#define Aalpha_m _ml->template fpfield<0>(_iml)
#define Aalpha_m_columnindex 0
#define Kalpha_m _ml->template fpfield<1>(_iml)
#define Kalpha_m_columnindex 1
#define V0alpha_m _ml->template fpfield<2>(_iml)
#define V0alpha_m_columnindex 2
#define Abeta_m _ml->template fpfield<3>(_iml)
#define Abeta_m_columnindex 3
#define Kbeta_m _ml->template fpfield<4>(_iml)
#define Kbeta_m_columnindex 4
#define V0beta_m _ml->template fpfield<5>(_iml)
#define V0beta_m_columnindex 5
#define Aalpha_h _ml->template fpfield<6>(_iml)
#define Aalpha_h_columnindex 6
#define Kalpha_h _ml->template fpfield<7>(_iml)
#define Kalpha_h_columnindex 7
#define V0alpha_h _ml->template fpfield<8>(_iml)
#define V0alpha_h_columnindex 8
#define Abeta_h _ml->template fpfield<9>(_iml)
#define Abeta_h_columnindex 9
#define Kbeta_h _ml->template fpfield<10>(_iml)
#define Kbeta_h_columnindex 10
#define V0beta_h _ml->template fpfield<11>(_iml)
#define V0beta_h_columnindex 11
#define Q10_diff _ml->template fpfield<12>(_iml)
#define Q10_diff_columnindex 12
#define Q10_channel_alp_m _ml->template fpfield<13>(_iml)
#define Q10_channel_alp_m_columnindex 13
#define Q10_channel_bet_m _ml->template fpfield<14>(_iml)
#define Q10_channel_bet_m_columnindex 14
#define Q10_channel_alp_h _ml->template fpfield<15>(_iml)
#define Q10_channel_alp_h_columnindex 15
#define Q10_channel_bet_h _ml->template fpfield<16>(_iml)
#define Q10_channel_bet_h_columnindex 16
#define gbar _ml->template fpfield<17>(_iml)
#define gbar_columnindex 17
#define fix_celsius _ml->template fpfield<18>(_iml)
#define fix_celsius_columnindex 18
#define ic _ml->template fpfield<19>(_iml)
#define ic_columnindex 19
#define m_inf _ml->template fpfield<20>(_iml)
#define m_inf_columnindex 20
#define h_inf _ml->template fpfield<21>(_iml)
#define h_inf_columnindex 21
#define tau_m _ml->template fpfield<22>(_iml)
#define tau_m_columnindex 22
#define tau_h _ml->template fpfield<23>(_iml)
#define tau_h_columnindex 23
#define g _ml->template fpfield<24>(_iml)
#define g_columnindex 24
#define gbar_Q10 _ml->template fpfield<25>(_iml)
#define gbar_Q10_columnindex 25
#define m _ml->template fpfield<26>(_iml)
#define m_columnindex 26
#define h _ml->template fpfield<27>(_iml)
#define h_columnindex 27
#define ena _ml->template fpfield<28>(_iml)
#define ena_columnindex 28
#define Dm _ml->template fpfield<29>(_iml)
#define Dm_columnindex 29
#define Dh _ml->template fpfield<30>(_iml)
#define Dh_columnindex 30
#define ina _ml->template fpfield<31>(_iml)
#define ina_columnindex 31
#define v _ml->template fpfield<32>(_iml)
#define v_columnindex 32
#define _g _ml->template fpfield<33>(_iml)
#define _g_columnindex 33
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
 static void _hoc_alp_h(void);
 static void _hoc_alp_m(void);
 static void _hoc_bet_h(void);
 static void _hoc_bet_m(void);
 static void _hoc_linoid(void);
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
 {"setdata_GRANULE_NA", _hoc_setdata},
 {"alp_h_GRANULE_NA", _hoc_alp_h},
 {"alp_m_GRANULE_NA", _hoc_alp_m},
 {"bet_h_GRANULE_NA", _hoc_bet_h},
 {"bet_m_GRANULE_NA", _hoc_bet_m},
 {"linoid_GRANULE_NA", _hoc_linoid},
 {"rate_GRANULE_NA", _hoc_rate},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_alp_h(Prop*);
 static double _npy_alp_m(Prop*);
 static double _npy_bet_h(Prop*);
 static double _npy_bet_m(Prop*);
 static double _npy_linoid(Prop*);
 static double _npy_rate(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"alp_h", _npy_alp_h},
 {"alp_m", _npy_alp_m},
 {"bet_h", _npy_bet_h},
 {"bet_m", _npy_bet_m},
 {"linoid", _npy_linoid},
 {"rate", _npy_rate},
 {0, 0}
};
#define alp_h alp_h_GRANULE_NA
#define alp_m alp_m_GRANULE_NA
#define bet_h bet_h_GRANULE_NA
#define bet_m bet_m_GRANULE_NA
#define linoid linoid_GRANULE_NA
 extern double alp_h( _internalthreadargsprotocomma_ double );
 extern double alp_m( _internalthreadargsprotocomma_ double );
 extern double bet_h( _internalthreadargsprotocomma_ double );
 extern double bet_m( _internalthreadargsprotocomma_ double );
 extern double linoid( _internalthreadargsprotocomma_ double , double );
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define usetable usetable_GRANULE_NA
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
 {"usetable_GRANULE_NA", 0, 1},
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"Aalpha_m_GRANULE_NA", "/ms-mV"},
 {"Kalpha_m_GRANULE_NA", "mV"},
 {"V0alpha_m_GRANULE_NA", "mV"},
 {"Abeta_m_GRANULE_NA", "/ms"},
 {"Kbeta_m_GRANULE_NA", "mV"},
 {"V0beta_m_GRANULE_NA", "mV"},
 {"Aalpha_h_GRANULE_NA", "/ms"},
 {"Kalpha_h_GRANULE_NA", "mV"},
 {"V0alpha_h_GRANULE_NA", "mV"},
 {"Abeta_h_GRANULE_NA", "/ms"},
 {"Kbeta_h_GRANULE_NA", "mV"},
 {"V0beta_h_GRANULE_NA", "mV"},
 {"gbar_GRANULE_NA", "mho/cm2"},
 {"fix_celsius_GRANULE_NA", "degC"},
 {"ic_GRANULE_NA", "mA/cm2"},
 {"tau_m_GRANULE_NA", "ms"},
 {"tau_h_GRANULE_NA", "ms"},
 {"g_GRANULE_NA", "mho/cm2"},
 {"gbar_Q10_GRANULE_NA", "mho/cm2"},
 {0, 0}
};
 static double delta_t = 0.01;
 static double h0 = 0;
 static double m0 = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"usetable_GRANULE_NA", &usetable_GRANULE_NA},
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
"GRANULE_NA",
 "Aalpha_m_GRANULE_NA",
 "Kalpha_m_GRANULE_NA",
 "V0alpha_m_GRANULE_NA",
 "Abeta_m_GRANULE_NA",
 "Kbeta_m_GRANULE_NA",
 "V0beta_m_GRANULE_NA",
 "Aalpha_h_GRANULE_NA",
 "Kalpha_h_GRANULE_NA",
 "V0alpha_h_GRANULE_NA",
 "Abeta_h_GRANULE_NA",
 "Kbeta_h_GRANULE_NA",
 "V0beta_h_GRANULE_NA",
 "Q10_diff_GRANULE_NA",
 "Q10_channel_alp_m_GRANULE_NA",
 "Q10_channel_bet_m_GRANULE_NA",
 "Q10_channel_alp_h_GRANULE_NA",
 "Q10_channel_bet_h_GRANULE_NA",
 "gbar_GRANULE_NA",
 "fix_celsius_GRANULE_NA",
 0,
 "ic_GRANULE_NA",
 "m_inf_GRANULE_NA",
 "h_inf_GRANULE_NA",
 "tau_m_GRANULE_NA",
 "tau_h_GRANULE_NA",
 "g_GRANULE_NA",
 "gbar_Q10_GRANULE_NA",
 0,
 "m_GRANULE_NA",
 "h_GRANULE_NA",
 0,
 0};
 static Symbol* _na_sym;
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     -0.3, /* Aalpha_m */
     -10, /* Kalpha_m */
     -19, /* V0alpha_m */
     12, /* Abeta_m */
     -18.182, /* Kbeta_m */
     -44, /* V0beta_m */
     0.105, /* Aalpha_h */
     -3.333, /* Kalpha_h */
     -44, /* V0alpha_h */
     0.75, /* Abeta_h */
     -5, /* Kbeta_h */
     -11, /* V0beta_h */
     1.5, /* Q10_diff */
     3, /* Q10_channel_alp_m */
     3, /* Q10_channel_bet_m */
     3, /* Q10_channel_alp_h */
     3, /* Q10_channel_bet_h */
     0.013, /* gbar */
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
    assert(_nrn_mechanism_get_num_vars(_prop) == 34);
 	/*initialize range parameters*/
 	Aalpha_m = _parm_default[0]; /* -0.3 */
 	Kalpha_m = _parm_default[1]; /* -10 */
 	V0alpha_m = _parm_default[2]; /* -19 */
 	Abeta_m = _parm_default[3]; /* 12 */
 	Kbeta_m = _parm_default[4]; /* -18.182 */
 	V0beta_m = _parm_default[5]; /* -44 */
 	Aalpha_h = _parm_default[6]; /* 0.105 */
 	Kalpha_h = _parm_default[7]; /* -3.333 */
 	V0alpha_h = _parm_default[8]; /* -44 */
 	Abeta_h = _parm_default[9]; /* 0.75 */
 	Kbeta_h = _parm_default[10]; /* -5 */
 	V0beta_h = _parm_default[11]; /* -11 */
 	Q10_diff = _parm_default[12]; /* 1.5 */
 	Q10_channel_alp_m = _parm_default[13]; /* 3 */
 	Q10_channel_bet_m = _parm_default[14]; /* 3 */
 	Q10_channel_alp_h = _parm_default[15]; /* 3 */
 	Q10_channel_bet_h = _parm_default[16]; /* 3 */
 	gbar = _parm_default[17]; /* 0.013 */
 	fix_celsius = _parm_default[18]; /* 37 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 34);
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

 extern "C" void _GRANULE_NA_reg() {
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
                                       _nrn_mechanism_field<double>{"Aalpha_m"} /* 0 */,
                                       _nrn_mechanism_field<double>{"Kalpha_m"} /* 1 */,
                                       _nrn_mechanism_field<double>{"V0alpha_m"} /* 2 */,
                                       _nrn_mechanism_field<double>{"Abeta_m"} /* 3 */,
                                       _nrn_mechanism_field<double>{"Kbeta_m"} /* 4 */,
                                       _nrn_mechanism_field<double>{"V0beta_m"} /* 5 */,
                                       _nrn_mechanism_field<double>{"Aalpha_h"} /* 6 */,
                                       _nrn_mechanism_field<double>{"Kalpha_h"} /* 7 */,
                                       _nrn_mechanism_field<double>{"V0alpha_h"} /* 8 */,
                                       _nrn_mechanism_field<double>{"Abeta_h"} /* 9 */,
                                       _nrn_mechanism_field<double>{"Kbeta_h"} /* 10 */,
                                       _nrn_mechanism_field<double>{"V0beta_h"} /* 11 */,
                                       _nrn_mechanism_field<double>{"Q10_diff"} /* 12 */,
                                       _nrn_mechanism_field<double>{"Q10_channel_alp_m"} /* 13 */,
                                       _nrn_mechanism_field<double>{"Q10_channel_bet_m"} /* 14 */,
                                       _nrn_mechanism_field<double>{"Q10_channel_alp_h"} /* 15 */,
                                       _nrn_mechanism_field<double>{"Q10_channel_bet_h"} /* 16 */,
                                       _nrn_mechanism_field<double>{"gbar"} /* 17 */,
                                       _nrn_mechanism_field<double>{"fix_celsius"} /* 18 */,
                                       _nrn_mechanism_field<double>{"ic"} /* 19 */,
                                       _nrn_mechanism_field<double>{"m_inf"} /* 20 */,
                                       _nrn_mechanism_field<double>{"h_inf"} /* 21 */,
                                       _nrn_mechanism_field<double>{"tau_m"} /* 22 */,
                                       _nrn_mechanism_field<double>{"tau_h"} /* 23 */,
                                       _nrn_mechanism_field<double>{"g"} /* 24 */,
                                       _nrn_mechanism_field<double>{"gbar_Q10"} /* 25 */,
                                       _nrn_mechanism_field<double>{"m"} /* 26 */,
                                       _nrn_mechanism_field<double>{"h"} /* 27 */,
                                       _nrn_mechanism_field<double>{"ena"} /* 28 */,
                                       _nrn_mechanism_field<double>{"Dm"} /* 29 */,
                                       _nrn_mechanism_field<double>{"Dh"} /* 30 */,
                                       _nrn_mechanism_field<double>{"ina"} /* 31 */,
                                       _nrn_mechanism_field<double>{"v"} /* 32 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 33 */,
                                       _nrn_mechanism_field<double*>{"_ion_ena", "na_ion"} /* 0 */,
                                       _nrn_mechanism_field<double*>{"_ion_ina", "na_ion"} /* 1 */,
                                       _nrn_mechanism_field<double*>{"_ion_dinadv", "na_ion"} /* 2 */,
                                       _nrn_mechanism_field<int>{"_cvode_ieq", "cvodeieq"} /* 3 */);
  hoc_register_prop_size(_mechtype, 34, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 3, "cvodeieq");
 	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);
 	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 GRANULE_NA /home/oliver/GranularLayerModel/src/mod/GRANULE_NA.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double *_t_m_inf;
 static double *_t_tau_m;
 static double *_t_h_inf;
 static double *_t_tau_h;
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
   Dm = ( m_inf - m ) / tau_m ;
   Dh = ( h_inf - h ) / tau_h ;
   }
 return _reset;
}
 static int _ode_matsol1 (_internalthreadargsproto_) {
 rate ( _threadargscomma_ v ) ;
 Dm = Dm  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_m )) ;
 Dh = Dh  / (1. - dt*( ( ( ( - 1.0 ) ) ) / tau_h )) ;
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
   Dm = ( m_inf - m ) / tau_m ;
   Dh = ( h_inf - h ) / tau_h ;
   {int _id; for(_id=0; _id < 2; _id++) {
if (_deriv1_advance) {
 _dlist2[++_counte] = _ml->data(_iml, _dlist1[_id]) - (_ml->data(_iml, _slist1[_id]) - _savstate1[_id])/dt;
 }else{
_dlist2[++_counte] = _ml->data(_iml, _slist1[_id]) - _savstate1[_id];}}}
 } }
 return _reset;}
 
double alp_m ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalp_m;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel_alp_m , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lalp_m = _lQ10 * Aalpha_m * linoid ( _threadargscomma_ _lv - V0alpha_m , Kalpha_m ) ;
   
return _lalp_m;
 }
 
static void _hoc_alp_m(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_m_GRANULE_NA. Requires prior call to setdata_GRANULE_NA and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  alp_m ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alp_m(Prop* _prop) {
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
 _r =  alp_m ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bet_m ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbet_m;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel_bet_m , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lbet_m = _lQ10 * Abeta_m * exp ( ( _lv - V0beta_m ) / Kbeta_m ) ;
   
return _lbet_m;
 }
 
static void _hoc_bet_m(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for bet_m_GRANULE_NA. Requires prior call to setdata_GRANULE_NA and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  bet_m ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bet_m(Prop* _prop) {
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
 _r =  bet_m ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double alp_h ( _internalthreadargsprotocomma_ double _lv ) {
   double _lalp_h;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel_alp_h , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lalp_h = _lQ10 * Aalpha_h * exp ( ( _lv - V0alpha_h ) / Kalpha_h ) ;
   
return _lalp_h;
 }
 
static void _hoc_alp_h(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for alp_h_GRANULE_NA. Requires prior call to setdata_GRANULE_NA and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  alp_h ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_alp_h(Prop* _prop) {
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
 _r =  alp_h ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 
double bet_h ( _internalthreadargsprotocomma_ double _lv ) {
   double _lbet_h;
 double _lQ10 ;
 _lQ10 = pow( Q10_channel_bet_h , ( ( fix_celsius - 20.0 ) / 10.0 ) ) ;
   _lbet_h = _lQ10 * Abeta_h / ( 1.0 + exp ( ( _lv - V0beta_h ) / Kbeta_h ) ) ;
   
return _lbet_h;
 }
 
static void _hoc_bet_h(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for bet_h_GRANULE_NA. Requires prior call to setdata_GRANULE_NA and that the specified mechanism instance still be in existence.", NULL);
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
 _r =  bet_h ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_bet_h(Prop* _prop) {
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
 _r =  bet_h ( _threadargscomma_ *getarg(1) );
 return(_r);
}
 static double _mfac_rate, _tmin_rate;
  static void _check_rate(_internalthreadargsproto_) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_Aalpha_m;
  static double _sav_Kalpha_m;
  static double _sav_V0alpha_m;
  static double _sav_Abeta_m;
  static double _sav_Kbeta_m;
  static double _sav_V0beta_m;
  static double _sav_Aalpha_h;
  static double _sav_Kalpha_h;
  static double _sav_V0alpha_h;
  static double _sav_Abeta_h;
  static double _sav_Kbeta_h;
  static double _sav_V0beta_h;
  static double _sav_fix_celsius;
  static double _sav_Q10_channel_bet_h;
  static double _sav_Q10_channel_alp_h;
  static double _sav_Q10_channel_bet_m;
  static double _sav_Q10_channel_alp_m;
  if (!usetable) {return;}
  if (_sav_Aalpha_m != Aalpha_m) { _maktable = 1;}
  if (_sav_Kalpha_m != Kalpha_m) { _maktable = 1;}
  if (_sav_V0alpha_m != V0alpha_m) { _maktable = 1;}
  if (_sav_Abeta_m != Abeta_m) { _maktable = 1;}
  if (_sav_Kbeta_m != Kbeta_m) { _maktable = 1;}
  if (_sav_V0beta_m != V0beta_m) { _maktable = 1;}
  if (_sav_Aalpha_h != Aalpha_h) { _maktable = 1;}
  if (_sav_Kalpha_h != Kalpha_h) { _maktable = 1;}
  if (_sav_V0alpha_h != V0alpha_h) { _maktable = 1;}
  if (_sav_Abeta_h != Abeta_h) { _maktable = 1;}
  if (_sav_Kbeta_h != Kbeta_h) { _maktable = 1;}
  if (_sav_V0beta_h != V0beta_h) { _maktable = 1;}
  if (_sav_fix_celsius != fix_celsius) { _maktable = 1;}
  if (_sav_Q10_channel_bet_h != Q10_channel_bet_h) { _maktable = 1;}
  if (_sav_Q10_channel_alp_h != Q10_channel_alp_h) { _maktable = 1;}
  if (_sav_Q10_channel_bet_m != Q10_channel_bet_m) { _maktable = 1;}
  if (_sav_Q10_channel_alp_m != Q10_channel_alp_m) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_rate =  - 100.0 ;
   _tmax =  30.0 ;
   _dx = (_tmax - _tmin_rate)/13000.; _mfac_rate = 1./_dx;
   for (_i=0, _x=_tmin_rate; _i < 13001; _x += _dx, _i++) {
    _f_rate(_threadargscomma_ _x);
    _t_m_inf[_i] = m_inf;
    _t_tau_m[_i] = tau_m;
    _t_h_inf[_i] = h_inf;
    _t_tau_h[_i] = tau_h;
   }
   _sav_Aalpha_m = Aalpha_m;
   _sav_Kalpha_m = Kalpha_m;
   _sav_V0alpha_m = V0alpha_m;
   _sav_Abeta_m = Abeta_m;
   _sav_Kbeta_m = Kbeta_m;
   _sav_V0beta_m = V0beta_m;
   _sav_Aalpha_h = Aalpha_h;
   _sav_Kalpha_h = Kalpha_h;
   _sav_V0alpha_h = V0alpha_h;
   _sav_Abeta_h = Abeta_h;
   _sav_Kbeta_h = Kbeta_h;
   _sav_V0beta_h = V0beta_h;
   _sav_fix_celsius = fix_celsius;
   _sav_Q10_channel_bet_h = Q10_channel_bet_h;
   _sav_Q10_channel_alp_h = Q10_channel_alp_h;
   _sav_Q10_channel_bet_m = Q10_channel_bet_m;
   _sav_Q10_channel_alp_m = Q10_channel_alp_m;
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
  m_inf = _xi;
  tau_m = _xi;
  h_inf = _xi;
  tau_h = _xi;
  return;
 }
 if (_xi <= 0.) {
 m_inf = _t_m_inf[0];
 tau_m = _t_tau_m[0];
 h_inf = _t_h_inf[0];
 tau_h = _t_tau_h[0];
 return; }
 if (_xi >= 13000.) {
 m_inf = _t_m_inf[13000];
 tau_m = _t_tau_m[13000];
 h_inf = _t_h_inf[13000];
 tau_h = _t_tau_h[13000];
 return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 m_inf = _t_m_inf[_i] + _theta*(_t_m_inf[_i+1] - _t_m_inf[_i]);
 tau_m = _t_tau_m[_i] + _theta*(_t_tau_m[_i+1] - _t_tau_m[_i]);
 h_inf = _t_h_inf[_i] + _theta*(_t_h_inf[_i+1] - _t_h_inf[_i]);
 tau_h = _t_tau_h[_i] + _theta*(_t_tau_h[_i+1] - _t_tau_h[_i]);
 }

 
static int  _f_rate ( _internalthreadargsprotocomma_ double _lv ) {
   double _la_m , _lb_m , _la_h , _lb_h ;
 _la_m = alp_m ( _threadargscomma_ _lv ) ;
   _lb_m = bet_m ( _threadargscomma_ _lv ) ;
   _la_h = alp_h ( _threadargscomma_ _lv ) ;
   _lb_h = bet_h ( _threadargscomma_ _lv ) ;
   m_inf = _la_m / ( _la_m + _lb_m ) ;
   tau_m = 1.0 / ( _la_m + _lb_m ) ;
   h_inf = _la_h / ( _la_h + _lb_h ) ;
   tau_h = 1.0 / ( _la_h + _lb_h ) ;
    return 0; }
 
static void _hoc_rate(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rate_GRANULE_NA. Requires prior call to setdata_GRANULE_NA and that the specified mechanism instance still be in existence.", NULL);
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
  h = h0;
  m = m0;
 {
   gbar_Q10 = gbar * ( pow( Q10_diff , ( ( fix_celsius - 30.0 ) / 10.0 ) ) ) ;
   rate ( _threadargscomma_ v ) ;
   m = m_inf ;
   h = h_inf ;
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
   g = gbar_Q10 * m * m * m * h ;
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
 _slist1[0] = {m_columnindex, 0};  _dlist1[0] = {Dm_columnindex, 0};
 _slist1[1] = {h_columnindex, 0};  _dlist1[1] = {Dh_columnindex, 0};
 _slist2[0] = {h_columnindex, 0};
 _slist2[1] = {m_columnindex, 0};
   _t_m_inf = makevector(13001*sizeof(double));
   _t_tau_m = makevector(13001*sizeof(double));
   _t_h_inf = makevector(13001*sizeof(double));
   _t_tau_h = makevector(13001*sizeof(double));
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/oliver/GranularLayerModel/src/mod/GRANULE_NA.mod";
    const char* nmodl_file_text = 
  "TITLE Cerebellum Granule Cell Model\n"
  "\n"
  "COMMENT\n"
  "        Na channel\n"
  "	Gutfreund parametrization\n"
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
  "	SUFFIX GRANULE_NA\n"
  "	USEION na READ ena WRITE ina\n"
  "	RANGE Q10_diff,Q10_channel,gbar_Q10, gbar, fix_celsius\n"
  "	RANGE gnabar, ic, g :, alpha_m, beta_m, alpha_h, beta_h\n"
  "	RANGE Aalpha_m, Kalpha_m, V0alpha_m\n"
  "	RANGE Abeta_m, Kbeta_m, V0beta_m\n"
  "\n"
  "	RANGE Aalpha_h, Kalpha_h, V0alpha_h\n"
  "	RANGE Abeta_h, Kbeta_h, V0beta_h\n"
  "\n"
  "	RANGE m_inf, tau_m, h_inf, tau_h\n"
  "	RANGE Q10_channel_alp_m, Q10_channel_bet_m,Q10_channel_alp_h,Q10_channel_bet_h\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(mA) = (milliamp)\n"
  "	(mV) = (millivolt)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "\n"
  "	Aalpha_m = -0.3 (/ms-mV)\n"
  "	Kalpha_m = -10 (mV)\n"
  "	V0alpha_m = -19 (mV)\n"
  "\n"
  "	Abeta_m = 12 (/ms)\n"
  "	Kbeta_m = -18.182 (mV)\n"
  "	V0beta_m = -44 (mV)\n"
  "\n"
  "	Aalpha_h  = 0.105 (/ms)\n"
  "	Kalpha_h  = -3.333 (mV)\n"
  "	V0alpha_h = -44 (mV)\n"
  "\n"
  "	Abeta_h  = 0.75 (/ms)\n"
  "	Kbeta_h  = -5 (mV)\n"
  "	V0beta_h = -11 (mV)\n"
  "\n"
  "	v (mV)\n"
  "	Q10_diff	= 1.5\n"
  "	Q10_channel_alp_m	= 3\n"
  "	Q10_channel_bet_m	= 3\n"
  "	Q10_channel_alp_h	= 3\n"
  "	Q10_channel_bet_h	= 3\n"
  "	gbar	=  0.013 (mho/cm2)\n"
  "	ena 	= 87.39 (mV)\n"
  "    fix_celsius = 37 (degC)\n"
  "}\n"
  "\n"
  "STATE {\n"
  "	m\n"
  "	h\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ina (mA/cm2)\n"
  "	ic (mA/cm2)\n"
  "	m_inf\n"
  "	h_inf\n"
  "	tau_m (ms)\n"
  "	tau_h (ms)\n"
  "	g (mho/cm2)\n"
  ":	alpha_m (/ms)\n"
  ":	beta_m (/ms)\n"
  ":	alpha_h (/ms)\n"
  ":	beta_h (/ms)\n"
  "	gbar_Q10 (mho/cm2)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	gbar_Q10 = gbar*(Q10_diff^((fix_celsius-30)/10))\n"
  "	rate(v)\n"
  "	m = m_inf\n"
  "	h = h_inf\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE states METHOD derivimplicit\n"
  "	g = gbar_Q10*m*m*m*h\n"
  "	ina = g*(v - ena)\n"
  "	ic = ina\n"
  ":	alpha_m = alp_m(v)\n"
  ":	beta_m = bet_m(v)\n"
  ":	alpha_h = alp_h(v)\n"
  ":	beta_h = bet_h(v)\n"
  "}\n"
  "\n"
  "DERIVATIVE states {\n"
  "	rate(v)\n"
  "	m' =(m_inf - m)/tau_m\n"
  "	h' =(h_inf - h)/tau_h\n"
  "}\n"
  "\n"
  "FUNCTION alp_m(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel_alp_m^((fix_celsius-20(degC))/10(degC))\n"
  "	alp_m = Q10*Aalpha_m*linoid(v-V0alpha_m,Kalpha_m)\n"
  "}\n"
  "\n"
  "FUNCTION bet_m(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel_bet_m^((fix_celsius-20(degC))/10(degC))\n"
  "	bet_m = Q10*Abeta_m*exp((v-V0beta_m)/Kbeta_m)\n"
  "}\n"
  "\n"
  "FUNCTION alp_h(v(mV))(/ms) { LOCAL Q10\n"
  "	Q10 = Q10_channel_alp_h^((fix_celsius-20(degC))/10(degC))\n"
  "	alp_h = Q10*Aalpha_h*exp((v-V0alpha_h)/Kalpha_h)\n"
  "}\n"
  "\n"
  "FUNCTION bet_h(v(mV))(/ms) { LOCAL Q10\n"
  "    Q10 = Q10_channel_bet_h^((fix_celsius-20(degC))/10(degC))\n"
  "    bet_h = Q10*Abeta_h/(1+exp((v-V0beta_h)/Kbeta_h))\n"
  "}\n"
  "\n"
  "PROCEDURE rate(v (mV)) {LOCAL a_m, b_m, a_h, b_h\n"
  "	TABLE m_inf, tau_m, h_inf, tau_h\n"
  "	DEPEND Aalpha_m, Kalpha_m, V0alpha_m,\n"
  "	       Abeta_m, Kbeta_m, V0beta_m,\n"
  "               Aalpha_h, Kalpha_h, V0alpha_h,\n"
  "               Abeta_h, Kbeta_h, V0beta_h, fix_celsius, Q10_channel_bet_h, Q10_channel_alp_h, Q10_channel_bet_m, Q10_channel_alp_m FROM -100 TO 30 WITH 13000\n"
  "	a_m = alp_m(v)\n"
  "	b_m = bet_m(v)\n"
  "	a_h = alp_h(v)\n"
  "	b_h = bet_h(v)\n"
  "	m_inf = a_m/(a_m + b_m)\n"
  "	tau_m = 1/(a_m + b_m)\n"
  "	h_inf = a_h/(a_h + b_h)\n"
  "	tau_h = 1/(a_h + b_h)\n"
  "	:if (tau_h<0.1 (ms)) {tau_h=0.1 (ms)} : riga aggiunta il 10 giugno 2003\n"
  "}\n"
  "\n"
  "FUNCTION linoid(x (mV),y (mV)) (mV) {\n"
  "	if (fabs(x/y) < 1e-6) {\n"
  "			linoid = y*(1 - x/y/2)\n"
  "	}else{\n"
  "			linoid = x/(exp(x/y) - 1)\n"
  "	}\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
