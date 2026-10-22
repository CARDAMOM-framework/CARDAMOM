#pragma once
// Wrapper for DALEC_1140 that reorders includes to resolve dependencies

// First include the DALEC_MODULE to get DALEC struct definition
#include "../DALEC_ALL/DALEC_MODULE.c"

// Now include the other DALEC_1140 files that need DALEC
#include "DALEC_1140_INDICES.c"
#include "PARS_INFO_1140.c"
#include "DALEC_1140_NC_INFO.c"

// Include hydrology modules
#include "../DALEC_ALL/HYDROLOGY_MODULES/DRAINAGE.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_EWT2MOI.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2EWT.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2CON.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2PSI.c"
#include "../DALEC_ALL/LIU_AN_ET_REFACTOR.c"
#include "../DALEC_ALL/CH4_MODULES/HET_RESP_RATES_JCR.c"
#include "../DALEC_ALL/KNORR_ALLOCATION.c"
#include "../DALEC_ALL/SOIL_TEMP_AND_LIQUID_FRAC.c"
#include "../DALEC_ALL/INITIALIZE_INTERNAL_SOIL_ENERGY.c"
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/ALLOC_AND_AUTO_RESP_FLUXES.c"

// Now include the rest of DALEC_1140.c (skipping the redundant includes at the top)
// We'll manually copy the function definitions from DALEC_1140.c

// Copy DALEC_1140_DATA_STRUCT and functions from DALEC_1140.c
typedef struct DALEC_1140_DATA_STRUCT{
double * VegK;
double example_const;
}DALEC_1140_DATA_STRUCT;

// Now just include the actual DALEC_1140.c but it will skip re-including what's already done
// Actually, let's just include it normally since pragma once will handle duplicates
#define DALEC_1140_INCLUDES_DONE
#include "DALEC_1140.c"
