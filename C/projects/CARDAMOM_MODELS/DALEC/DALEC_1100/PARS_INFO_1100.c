#pragma once
#include "DALEC_1100_INDICES.c"

/*PARAMETER_INFO (typedef struct) must have at least 3 fields
 *  * npars,
 *   * parmax
 *    * parmin*/
/*where is it defined?*/
/*For DALEC_FIRES: as all GPP allocation fractions are inter-dependent (sum = 1)*/
/*MCMC sampling of GPP allocation priors approximated as 0.01-0.5 NPP for*/
/*photosynthetic pools and 0.01-1 of remaining NPP for root and wood pool*/


int PARS_INFO_1100(DALEC * DALECmodel){

struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;


DALECmodel->PARS_INFO.parmin= calloc(DALECmodel->nopars, sizeof(double));
DALECmodel->PARS_INFO.parmax= calloc(DALECmodel->nopars, sizeof(double));
//Abbreviate

double * parmin = DALECmodel->PARS_INFO.parmin;
double * parmax =DALECmodel->PARS_INFO.parmax;


/*iSWE: initial for state variable SWE snow water equivalent*/
parmin[P.i_SWE_H2O]=0.000001;
parmax[P.i_SWE_H2O]=10000;

/*iSWE: initial for state variable SWE snow water equivalent*/
        /*LY1 energy per mm H2O (~ Energy in 1mm H2O @ -50C to 1mm H2O@ 0C with LF=0)*/
parmin[P.i_SWE_E]=4.67e+05;
parmax[P.i_SWE_E]=5.72e+05;

/*sn1: min threshold for melt*/
parmin[P.min_melt]=263.15;
parmax[P.min_melt]=283.15;

/*sn2: slope*/
parmin[P.melt_slope]=0.00001;
parmax[P.melt_slope]=100;

/*sn3: snow cover fraction scalar; SCF = SWE/(SWE +SWEcritical_par) */
parmin[P.scf_scalar]=10;
parmax[P.scf_scalar]=1000.0;

/*Thermal conductivity of SWE (correcting for snow density) - W/m/K*/
    parmin[P.thermal_cond_swe]=0.001;
     parmax[P.thermal_cond_swe]=1;
// 

// 
// 
// /*Surface soil thermal conductivity in W/m/K */
// parmin[P.q10canopy]=1;
// parmax[P.q10canopy]=5;
// 
//     /*Surface soil thermal conductivity in W/m/K */
// parmin[P.canopyRdsf]=0.005;
// parmax[P.canopyRdsf]=0.025;

        /*Sublimation rate: mm/day/SCF/kPha/(MJ/m2/d) Biggest = 1/0.5/2/2; Smallest = 1/1/10/10/*/
parmin[P.sublimation_rate]=0.001;
parmax[P.sublimation_rate]=100;

// /*Fraction of LY2 to LY1 root biomass*/
// parmin[P.root_frac]=0.001;
// parmax[P.root_frac]=1;
  
return 0;

}


