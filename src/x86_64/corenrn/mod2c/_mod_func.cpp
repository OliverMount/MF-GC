#include <cstdio>
namespace coreneuron {
extern int nrnmpi_myid;
extern int nrn_nobanner_;
extern int
  _GRANULE_CA_reg(void),
  _GRANULE_CALC_reg(void),
  _GRANULE_KA_reg(void),
  _GRANULE_KCA_reg(void),
  _GRANULE_KIR_reg(void),
  _GRANULE_KM_reg(void),
  _GRANULE_KV_reg(void),
  _GRANULE_LKG1_reg(void),
  _GRANULE_LKG2_reg(void),
  _GRANULE_NA_reg(void),
  _GRANULE_NAR_reg(void),
  _GRANULE_Nmda_leak_reg(void),
  _GRANULE_Nmda_leak_linear_reg(void),
  _GRANULE_PNA_reg(void),
  _GRANULE_tonicGABA_reg(void),
  _exp2syn_reg(void),
  _expsyn_reg(void),
  _hh_reg(void),
  _netstim_reg(void),
  _passive_reg(void),
  _pattern_reg(void),
  _stim_reg(void),
  _svclmp_reg(void),
  _vecstim_reg(void);

void modl_reg() {
    if (!nrn_nobanner_ && nrnmpi_myid < 1) {
        fprintf(stderr, " Additional mechanisms from files\n");
        fprintf(stderr, " GRANULE_CA.mod");
        fprintf(stderr, " GRANULE_CALC.mod");
        fprintf(stderr, " GRANULE_KA.mod");
        fprintf(stderr, " GRANULE_KCA.mod");
        fprintf(stderr, " GRANULE_KIR.mod");
        fprintf(stderr, " GRANULE_KM.mod");
        fprintf(stderr, " GRANULE_KV.mod");
        fprintf(stderr, " GRANULE_LKG1.mod");
        fprintf(stderr, " GRANULE_LKG2.mod");
        fprintf(stderr, " GRANULE_NA.mod");
        fprintf(stderr, " GRANULE_NAR.mod");
        fprintf(stderr, " GRANULE_Nmda_leak.mod");
        fprintf(stderr, " GRANULE_Nmda_leak_linear.mod");
        fprintf(stderr, " GRANULE_PNA.mod");
        fprintf(stderr, " GRANULE_tonicGABA.mod");
        fprintf(stderr, " exp2syn.mod");
        fprintf(stderr, " expsyn.mod");
        fprintf(stderr, " hh.mod");
        fprintf(stderr, " netstim.mod");
        fprintf(stderr, " passive.mod");
        fprintf(stderr, " pattern.mod");
        fprintf(stderr, " stim.mod");
        fprintf(stderr, " svclmp.mod");
        fprintf(stderr, " vecstim.mod");
        fprintf(stderr, "\n\n");
    }
    _GRANULE_CA_reg();
    _GRANULE_CALC_reg();
    _GRANULE_KA_reg();
    _GRANULE_KCA_reg();
    _GRANULE_KIR_reg();
    _GRANULE_KM_reg();
    _GRANULE_KV_reg();
    _GRANULE_LKG1_reg();
    _GRANULE_LKG2_reg();
    _GRANULE_NA_reg();
    _GRANULE_NAR_reg();
    _GRANULE_Nmda_leak_reg();
    _GRANULE_Nmda_leak_linear_reg();
    _GRANULE_PNA_reg();
    _GRANULE_tonicGABA_reg();
    _exp2syn_reg();
    _expsyn_reg();
    _hh_reg();
    _netstim_reg();
    _passive_reg();
    _pattern_reg();
    _stim_reg();
    _svclmp_reg();
    _vecstim_reg();
}
} //namespace coreneuron
