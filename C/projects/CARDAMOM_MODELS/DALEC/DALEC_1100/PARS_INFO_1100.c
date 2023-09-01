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
parmin[P.i_SWE_H2O]=0.00001;
parmax[P.i_SWE_H2O]=10000;

/*iSWE: initial for state variable SWE snow water equivalent*/
        /*LY1 energy per mm H2O (~ Energy in 1mm H2O @ -50C to 1mm H2O@ 0C with LF=0)*/
parmin[P.i_SWE_E]=4.67e+05;
parmax[P.i_SWE_E]=5.72e+05;

// /*sn1: min threshold for melt*/
// parmin[P.min_melt]=263.15;
// parmax[P.min_melt]=283.15;

// /*sn2: slope*/
// parmin[P.melt_slope]=0.00001;
// parmax[P.melt_slope]=100;

/*Thermal conductivity of SWE (correcting for snow density) - W/m/K*/
    //3 provides an upper limit of ~ 0.5m/m snow density, with 1 W/m/K observed conductivity at 0.5m
    //Chapter 2 - Physical, Thermal, and Mechanical Properties of Snow, Ice, and Permafrost (ref)
    parmin[P.thermal_cond_swe]=0.001;
     parmax[P.thermal_cond_swe]=3;


// ___scf_gen3 start
///*sn3: snow cover fraction scalar; SCF = SWE/(SWE +SWEcritical_par) */
//parmin[P.scf_scalar]=10;
//parmax[P.scf_scalar]=1000.0;

/*sn3: snow cover fraction parameter, an index of topographic variability defined in relation to the standard deviation of elevation within a grid cellcontrols the shape of the SCF */
parmin[P.SWEmax]=10.0;
parmax[P.SWEmax]=10000.0;

/*sn4: snow cover fraction parameter, maximum accumulated SWE; SCF = 1-(1/pi * acos(2*SWE/SWEmax-1))**Nmelt */
parmin[P.Nmelt]=0.25;
parmax[P.Nmelt]=10.0;
// ___scf_gen3 end 
/*Sublimation rate: mm/day/SCF/kPha/(MJ/m2/d) Biggest = 1/0.5/2/2; Smallest = 1/1/10/10/*/
parmin[P.sublimation_rate]=0.00001;
parmax[P.sublimation_rate]=1;

  
parmin[P.snow_sub_temp_exp]=0.00001;
parmax[P.snow_sub_temp_exp]=1;

    return 0;

}


