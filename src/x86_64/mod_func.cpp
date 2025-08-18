#include <stdio.h>
#include "hocdec.h"
extern int nrnmpi_myid;
extern int nrn_nobanner_;

extern "C" void _GRANULE_CA_reg(void);
extern "C" void _GRANULE_CALC_reg(void);
extern "C" void _GRANULE_KA_reg(void);
extern "C" void _GRANULE_KCA_reg(void);
extern "C" void _GRANULE_KIR_reg(void);
extern "C" void _GRANULE_KM_reg(void);
extern "C" void _GRANULE_KV_reg(void);
extern "C" void _GRANULE_LKG1_reg(void);
extern "C" void _GRANULE_LKG2_reg(void);
extern "C" void _GRANULE_NA_reg(void);
extern "C" void _GRANULE_NAR_reg(void);
extern "C" void _GRANULE_Nmda_leak_reg(void);
extern "C" void _GRANULE_Nmda_leak_linear_reg(void);
extern "C" void _GRANULE_PNA_reg(void);
extern "C" void _GRANULE_tonicGABA_reg(void);
extern "C" void _vecstim_reg(void);

extern "C" void modl_reg() {
  if (!nrn_nobanner_) if (nrnmpi_myid < 1) {
    fprintf(stderr, "Additional mechanisms from files\n");
    fprintf(stderr, " \"mod/GRANULE_CA.mod\"");
    fprintf(stderr, " \"mod/GRANULE_CALC.mod\"");
    fprintf(stderr, " \"mod/GRANULE_KA.mod\"");
    fprintf(stderr, " \"mod/GRANULE_KCA.mod\"");
    fprintf(stderr, " \"mod/GRANULE_KIR.mod\"");
    fprintf(stderr, " \"mod/GRANULE_KM.mod\"");
    fprintf(stderr, " \"mod/GRANULE_KV.mod\"");
    fprintf(stderr, " \"mod/GRANULE_LKG1.mod\"");
    fprintf(stderr, " \"mod/GRANULE_LKG2.mod\"");
    fprintf(stderr, " \"mod/GRANULE_NA.mod\"");
    fprintf(stderr, " \"mod/GRANULE_NAR.mod\"");
    fprintf(stderr, " \"mod/GRANULE_Nmda_leak.mod\"");
    fprintf(stderr, " \"mod/GRANULE_Nmda_leak_linear.mod\"");
    fprintf(stderr, " \"mod/GRANULE_PNA.mod\"");
    fprintf(stderr, " \"mod/GRANULE_tonicGABA.mod\"");
    fprintf(stderr, " \"mod/vecstim.mod\"");
    fprintf(stderr, "\n");
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
  _vecstim_reg();
}
