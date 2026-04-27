#pragma once
#include "PARS_INFO_1111.c"
#include "DALEC_1111.c"
#include "EDC1_1111.c"
#include "EDC2_1111.c"
#include "../../../COST_FUNCTION/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF_DEDC.c"
#include "../../../CARDAMOM_GENERAL/CARDAMOM_MODEL_LIBRARY.c"

/*
 * DALEC_1111: DALEC-PyC
 * Extends DALEC_1110 with:
 *   - Climate-sensitive combustion factors (soil psi + VPD)
 *   - Pyrogenic carbon (PyC) generation from woody fire loss
 *   - Two PyC pools: labile (C_PyC_L) and refractory (C_PyC_R)
 *   - NISAR L-band ΔAGB observation operator (SUPPORT_NISAR_WOOD_OBS)
 *   - VIIRS FRP driver for combustion efficiency (CE)
 *
 * Branch: renato/CARDAMOM-PyC
 * Based on NISAR DART proposal Objectives 2 & 3
 */

int MODEL_INFO_1111(DATA * DATA){

static DALEC DALECmodel;

DALEC_1111_MODCONFIG(&DALECmodel);

DATA->nopools=DALECmodel.nopools;
DATA->nopars=DALECmodel.nopars;
DATA->nofluxes=DALECmodel.nofluxes;

DALECmodel.dalec=DALEC_1111;
DALECmodel.edc1=EDC1_1111;
DALECmodel.edc2=EDC2_1111;

INITIALIZE_PARAMETER_FIELDS(DATA);
PARS_INFO_1111(DATA);

oksofar("about to declare EDCD");
printf("DALECmodel.EDCD = %p\n",DALECmodel.EDCD);
EDCSETUP(*DATA,&DALECmodel.EDCD);
oksofar("done with declaration");
printf("DALECmodel.EDCD->EQF = %2.2f\n",DALECmodel.EDCD->EQF);

DATA->MODEL=&DALECmodel;
DATA->MLF=DALEC_MLF_DEDC;

return 0;}
