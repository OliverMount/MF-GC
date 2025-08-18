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
static constexpr auto number_of_datum_variables = 0;
static constexpr auto number_of_floating_point_variables = 14;
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
 
#define nrn_init _nrn_init__GRANULE_Nmda_leak
#define _nrn_initial _nrn_initial__GRANULE_Nmda_leak
#define nrn_cur _nrn_cur__GRANULE_Nmda_leak
#define _nrn_current _nrn_current__GRANULE_Nmda_leak
#define nrn_jacob _nrn_jacob__GRANULE_Nmda_leak
#define nrn_state _nrn_state__GRANULE_Nmda_leak
#define _net_receive _net_receive__GRANULE_Nmda_leak 
#define _f_rates _f_rates__GRANULE_Nmda_leak 
#define rates rates__GRANULE_Nmda_leak 
 
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
#define gmax_factor _ml->template fpfield<0>(_iml)
#define gmax_factor_columnindex 0
#define gmax _ml->template fpfield<1>(_iml)
#define gmax_columnindex 1
#define Erev _ml->template fpfield<2>(_iml)
#define Erev_columnindex 2
#define Q10_diff _ml->template fpfield<3>(_iml)
#define Q10_diff_columnindex 3
#define v0_block _ml->template fpfield<4>(_iml)
#define v0_block_columnindex 4
#define k_block _ml->template fpfield<5>(_iml)
#define k_block_columnindex 5
#define fix_celsius _ml->template fpfield<6>(_iml)
#define fix_celsius_columnindex 6
#define i _ml->template fpfield<7>(_iml)
#define i_columnindex 7
#define ic _ml->template fpfield<8>(_iml)
#define ic_columnindex 8
#define g _ml->template fpfield<9>(_iml)
#define g_columnindex 9
#define MgBlock _ml->template fpfield<10>(_iml)
#define MgBlock_columnindex 10
#define gbar_Q10 _ml->template fpfield<11>(_iml)
#define gbar_Q10_columnindex 11
#define v _ml->template fpfield<12>(_iml)
#define v_columnindex 12
#define _g _ml->template fpfield<13>(_iml)
#define _g_columnindex 13
 /* Thread safe. No static _ml, _iml or _ppvar. */
 static int hoc_nrnpointerindex =  -1;
 static _nrn_mechanism_std_vector<Datum> _extcall_thread;
 static Prop* _extcall_prop;
 /* _prop_id kind of shadows _extcall_prop to allow validity checking. */
 static _nrn_non_owning_id_without_container _prop_id{};
 /* external NEURON variables */
 /* declaration of user functions */
 static void _hoc_rates(void);
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
 {"setdata_GRANULE_Nmda_leak", _hoc_setdata},
 {"rates_GRANULE_Nmda_leak", _hoc_rates},
 {0, 0}
};
 
/* Direct Python call wrappers to density mechanism functions.*/
 static double _npy_rates(Prop*);
 
static NPyDirectMechFunc npy_direct_func_proc[] = {
 {"rates", _npy_rates},
 {0, 0}
};
 /* declare global and static user variables */
 #define gind 0
 #define _gth 0
#define Or Or_GRANULE_Nmda_leak
 double Or = 52;
#define O O_GRANULE_Nmda_leak
 double O = 0.01;
#define surf surf_GRANULE_Nmda_leak
 double surf = 2.9926e-06;
#define usetable usetable_GRANULE_Nmda_leak
 double usetable = 1;
 
static void _check_rates(_internalthreadargsproto_); 
static void _check_table_thread(_threadargsprotocomma_ int _type, _nrn_model_sorted_token const& _sorted_token) {
  if (gind != 0 && _thread != nullptr) { _globals = _thread[_gth].get<double*>(); } 
  _nrn_mechanism_cache_range _lmr{_sorted_token, *_nt, *_ml, _type};
  {
    auto* const _ml = &_lmr;
   _check_rates(_threadargs_);
   }
}
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 {"usetable_GRANULE_Nmda_leak", 0, 1},
 {0, 0, 0}
};
 static HocParmUnits _hoc_parm_units[] = {
 {"surf_GRANULE_Nmda_leak", "cm2"},
 {"O_GRANULE_Nmda_leak", "s"},
 {"gmax_GRANULE_Nmda_leak", "mho"},
 {"Erev_GRANULE_Nmda_leak", "mV"},
 {"v0_block_GRANULE_Nmda_leak", "mV"},
 {"k_block_GRANULE_Nmda_leak", "mV"},
 {"fix_celsius_GRANULE_Nmda_leak", "degC"},
 {"i_GRANULE_Nmda_leak", "mA/cm2"},
 {"ic_GRANULE_Nmda_leak", "mA/cm2"},
 {"g_GRANULE_Nmda_leak", "mho/cm2"},
 {0, 0}
};
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 {"surf_GRANULE_Nmda_leak", &surf_GRANULE_Nmda_leak},
 {"O_GRANULE_Nmda_leak", &O_GRANULE_Nmda_leak},
 {"Or_GRANULE_Nmda_leak", &Or_GRANULE_Nmda_leak},
 {"usetable_GRANULE_Nmda_leak", &usetable_GRANULE_Nmda_leak},
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
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
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
 0};
 
 /* Used by NrnProperty */
 static _nrn_mechanism_std_vector<double> _parm_default{
     1, /* gmax_factor */
     5e-11, /* gmax */
     -3.7, /* Erev */
     1.5, /* Q10_diff */
     -20, /* v0_block */
     13, /* k_block */
     37, /* fix_celsius */
 }; 
 
 
extern Prop* need_memb(Symbol*);
static void nrn_alloc(Prop* _prop) {
  Prop *prop_ion{};
  Datum *_ppvar{};
     _nrn_mechanism_cache_instance _ml_real{_prop};
    auto* const _ml = &_ml_real;
    size_t const _iml{};
    assert(_nrn_mechanism_get_num_vars(_prop) == 14);
 	/*initialize range parameters*/
 	gmax_factor = _parm_default[0]; /* 1 */
 	gmax = _parm_default[1]; /* 5e-11 */
 	Erev = _parm_default[2]; /* -3.7 */
 	Q10_diff = _parm_default[3]; /* 1.5 */
 	v0_block = _parm_default[4]; /* -20 */
 	k_block = _parm_default[5]; /* 13 */
 	fix_celsius = _parm_default[6]; /* 37 */
 	 assert(_nrn_mechanism_get_num_vars(_prop) == 14);
 
}
 static void _initlists();
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
void _nrn_thread_table_reg(int, nrn_thread_table_check_t);
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 extern "C" void _GRANULE_Nmda_leak_reg() {
	int _vectorized = 1;
  _initlists();
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 1);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 hoc_register_parm_default(_mechtype, &_parm_default);
         hoc_register_npy_direct(_mechtype, npy_direct_func_proc);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_table_reg(_mechtype, _check_table_thread);
 #if NMODL_TEXT
  register_nmodl_text_and_filename(_mechtype);
#endif
   _nrn_mechanism_register_data_fields(_mechtype,
                                       _nrn_mechanism_field<double>{"gmax_factor"} /* 0 */,
                                       _nrn_mechanism_field<double>{"gmax"} /* 1 */,
                                       _nrn_mechanism_field<double>{"Erev"} /* 2 */,
                                       _nrn_mechanism_field<double>{"Q10_diff"} /* 3 */,
                                       _nrn_mechanism_field<double>{"v0_block"} /* 4 */,
                                       _nrn_mechanism_field<double>{"k_block"} /* 5 */,
                                       _nrn_mechanism_field<double>{"fix_celsius"} /* 6 */,
                                       _nrn_mechanism_field<double>{"i"} /* 7 */,
                                       _nrn_mechanism_field<double>{"ic"} /* 8 */,
                                       _nrn_mechanism_field<double>{"g"} /* 9 */,
                                       _nrn_mechanism_field<double>{"MgBlock"} /* 10 */,
                                       _nrn_mechanism_field<double>{"gbar_Q10"} /* 11 */,
                                       _nrn_mechanism_field<double>{"v"} /* 12 */,
                                       _nrn_mechanism_field<double>{"_g"} /* 13 */);
  hoc_register_prop_size(_mechtype, 14, 0);
 
    hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 GRANULE_Nmda_leak /home/oliver/GranularLayerModel/src/mod/GRANULE_Nmda_leak.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
 static double PI = 0x1.921fb54442d18p+1;
 static double *_t_MgBlock;
static int _reset;
static const char *modelname = "NMDA leakage";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int _f_rates(_internalthreadargsprotocomma_ double);
static int rates(_internalthreadargsprotocomma_ double);
 static void _n_rates(_internalthreadargsprotocomma_ double _lv);
 static double _mfac_rates, _tmin_rates;
  static void _check_rates(_internalthreadargsproto_) {
  static int _maktable=1; int _i, _j, _ix = 0;
  double _xi, _tmax;
  static double _sav_v0_block;
  static double _sav_k_block;
  if (!usetable) {return;}
  if (_sav_v0_block != v0_block) { _maktable = 1;}
  if (_sav_k_block != k_block) { _maktable = 1;}
  if (_maktable) { double _x, _dx; _maktable=0;
   _tmin_rates =  - 120.0 ;
   _tmax =  30.0 ;
   _dx = (_tmax - _tmin_rates)/150.; _mfac_rates = 1./_dx;
   for (_i=0, _x=_tmin_rates; _i < 151; _x += _dx, _i++) {
    _f_rates(_threadargscomma_ _x);
    _t_MgBlock[_i] = MgBlock;
   }
   _sav_v0_block = v0_block;
   _sav_k_block = k_block;
  }
 }

 static int rates(_internalthreadargsprotocomma_ double _lv) { 
#if 0
_check_rates(_threadargs_);
#endif
 _n_rates(_threadargscomma_ _lv);
 return 0;
 }

 static void _n_rates(_internalthreadargsprotocomma_ double _lv){ int _i, _j;
 double _xi, _theta;
 if (!usetable) {
 _f_rates(_threadargscomma_ _lv); return; 
}
 _xi = _mfac_rates * (_lv - _tmin_rates);
 if (std::isnan(_xi)) {
  MgBlock = _xi;
  return;
 }
 if (_xi <= 0.) {
 MgBlock = _t_MgBlock[0];
 return; }
 if (_xi >= 150.) {
 MgBlock = _t_MgBlock[150];
 return; }
 _i = (int) _xi;
 _theta = _xi - (double)_i;
 MgBlock = _t_MgBlock[_i] + _theta*(_t_MgBlock[_i+1] - _t_MgBlock[_i]);
 }

 
static int  _f_rates ( _internalthreadargsprotocomma_ double _lv ) {
   MgBlock = 1.0 / ( 1.0 + exp ( - ( _lv - v0_block ) / k_block ) ) ;
    return 0; }
 
static void _hoc_rates(void) {
  double _r;
 Datum* _ppvar; Datum* _thread; NrnThread* _nt;
 
  if(!_prop_id) {
    hoc_execerror("No data for rates_GRANULE_Nmda_leak. Requires prior call to setdata_GRANULE_Nmda_leak and that the specified mechanism instance still be in existence.", NULL);
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
 _check_rates(_threadargs_);
#endif
 _r = 1.;
 rates ( _threadargscomma_ *getarg(1) );
 hoc_retpushx(_r);
}
 
static double _npy_rates(Prop* _prop) {
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
 _check_rates(_threadargs_);
#endif
 _r = 1.;
 rates ( _threadargscomma_ *getarg(1) );
 return(_r);
}

static void initmodel(_internalthreadargsproto_) {
  int _i; double _save;{
 {
   rates ( _threadargscomma_ v ) ;
   gbar_Q10 = pow( Q10_diff , ( ( fix_celsius - 30.0 ) / 10.0 ) ) ;
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
 _check_rates(_threadargs_);
#endif
   _v = _vec_v[_ni[_iml]];
 v = _v;
 initmodel(_threadargs_);
}
}

static double _nrn_current(_internalthreadargsprotocomma_ double _v) {
double _current=0.; v=_v;
{ {
   rates ( _threadargscomma_ v ) ;
   g = gmax / surf * gbar_Q10 * O * Or * gmax_factor * MgBlock ;
   i = g * ( v - Erev ) ;
   ic = i ;
   }
 _current += i;

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
 auto const _g_local = _nrn_current(_threadargscomma_ _v + .001);
 	{ _rhs = _nrn_current(_threadargscomma_ _v);
 	}
 _g = (_g_local - _rhs)/.001;
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

}

static void terminal(){}

static void _initlists(){
 int _i; static int _first = 1;
  if (!_first) return;
   _t_MgBlock = makevector(151*sizeof(double));
_first = 0;
}

#if NMODL_TEXT
static void register_nmodl_text_and_filename(int mech_type) {
    const char* nmodl_filename = "/home/oliver/GranularLayerModel/src/mod/GRANULE_Nmda_leak.mod";
    const char* nmodl_file_text = 
  "TITLE NMDA leakage\n"
  "COMMENT\n"
  "	NMDA: descritto da Westbrook\n"
  "	derived from the kinetic scheme from Nieus2006 and also folder \"DeterministicApproximated\"\n"
  "	rates of the kinetic scheme from Rossi2002\n"
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
  "	SUFFIX GRANULE_Nmda_leak\n"
  "	NONSPECIFIC_CURRENT i\n"
  "	RANGE Q10_diff\n"
  "	RANGE g , ic, Erev, gmax, gmax_factor, fix_celsius\n"
  "	RANGE MgBlock,v0_block,k_block\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "    (nA) = (nanoamp)\n"
  "    (mV) = (millivolt)\n"
  "    (umho) = (micromho)\n"
  "    (mM) = (milli/liter)\n"
  "    (pS) = (picosiemens)\n"
  "    (nS) = (nanosiemens)\n"
  "    (um) = (micrometer)\n"
  "    PI	= (pi)		(1)\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "    gmax_factor = 1\n"
  "    gmax	= 50e-12	(mho)\n"
  "    surf        = 299.26e-8 (cm2)\n"
  "    Erev	= -3.7  (mV)	: 0 (mV)\n"
  "    Q10_diff	= 1.5\n"
  "    v0_block = -20 (mV)\n"
  "    k_block  = 13 (mV)\n"
  "    O = 10e-3 (s) : mean open time\n"
  "    Or = 52 : spontaneous opening rate  (Hz)\n"
  "    v		(mV)		: postsynaptic voltage\n"
  "    fix_celsius = 37 (degC)\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "    i 		(mA/cm2)		: current = g*(v - Erev)\n"
  "    ic 		(mA/cm2)		: current = g*(v - Erev)\n"
  "    g 		(mho/cm2)		: actual conductance\n"
  "    MgBlock\n"
  "    gbar_Q10 (1)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	rates(v)\n"
  "	gbar_Q10 = Q10_diff^((fix_celsius-30)/10)\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	rates(v)\n"
  "	g = gmax / surf * gbar_Q10 * O * Or * gmax_factor  * MgBlock\n"
  "	i = g * (v - Erev)\n"
  "	ic = i\n"
  "}\n"
  "\n"
  "PROCEDURE rates(v(mV)) {\n"
  "	: E' necessario includere DEPEND v0_block,k_block per aggiornare le tabelle!\n"
  "	TABLE MgBlock DEPEND v0_block,k_block FROM -120 TO 30 WITH 150\n"
  "	MgBlock = 1 / ( 1 + exp ( - ( v - v0_block ) / k_block ) )\n"
  "}\n"
  ;
    hoc_reg_nmodl_filename(mech_type, nmodl_filename);
    hoc_reg_nmodl_text(mech_type, nmodl_file_text);
}
#endif
