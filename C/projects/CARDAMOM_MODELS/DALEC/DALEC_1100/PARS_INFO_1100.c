#pragma once
#include "DALEC_1100.c"

/*PARAMETER_INFO (typedef struct) must have at least 3 fields
 *  * npars,
 *   * parmax
 *    * parmin*/
/*where is it defined?*/
/*For DALEC_FIRES: as all GPP allocation fractions are inter-dependent (sum = 1)*/
/*MCMC sampling of GPP allocation priors approximated as 0.01-0.5 NPP for*/
/*photosynthetic pools and 0.01-1 of remaining NPP for root and wood pool*/

int PARS_INFO_1100(DATA *CARDADATA)
{


struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;

/*Litter decomposition rate*/
CARDADATA->parmin[P.tr_lit2som]=0.00001;
CARDADATA->parmax[P.tr_lit2som]=0.01;

/*CWD decomposition rate*/
CARDADATA->parmin[P.tr_cwd2som]=0.00001;
CARDADATA->parmax[P.tr_cwd2som]=0.01;

/*Fraction of GPP respired*/
CARDADATA->parmin[P.f_auto]=0.2;
CARDADATA->parmax[P.f_auto]=0.8;

/*Fraction of (1-fgpp) to roots*/
CARDADATA->parmin[P.f_root]=0.01;
CARDADATA->parmax[P.f_root]=1;

/*TOR wood* - 1% loss per year value*/
CARDADATA->parmin[P.t_wood]=0.000025;
CARDADATA->parmax[P.t_wood]=0.001;

/*TOR roots*/
CARDADATA->parmin[P.t_root]=0.0001;
CARDADATA->parmax[P.t_root]=0.01;

/*TOR litter*/
CARDADATA->parmin[P.t_lit]=0.0001;
CARDADATA->parmax[P.t_lit]=0.01;

/*TOR CWD*/
CARDADATA->parmin[P.t_cwd]=0.00005;
CARDADATA->parmax[P.t_cwd]=0.005;

/*TOR SOM*/
CARDADATA->parmin[P.t_som]=0.0000001;
CARDADATA->parmax[P.t_som]=0.001;

/*\Q10 = 1.2-2.0*/
CARDADATA->parmin[P.Q10rhco2]=1.2;
CARDADATA->parmax[P.Q10rhco2]=2.0;

/*Fraction to Clab*/
CARDADATA->parmin[P.f_lab]=0.01;
CARDADATA->parmax[P.f_lab]=0.5;

/*LMCA*/
/*Kattge et al. 2011*/
/*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*/
CARDADATA->parmin[P.LCMA]=5;
CARDADATA->parmax[P.LCMA]=200;

/*INITIAL VALUES DECLARED HERE*/

/*C labile*/
CARDADATA->parmin[P.i_labile]=1.0;
CARDADATA->parmax[P.i_labile]=2000.0;

/*C foliar*/
CARDADATA->parmin[P.i_foliar]=1.0;
CARDADATA->parmax[P.i_foliar]=2000.0;

/*C roots*/
CARDADATA->parmin[P.i_root]=1.0;
CARDADATA->parmax[P.i_root]=2000.0;

/*C_wood*/
CARDADATA->parmin[P.i_wood]=1.0;
CARDADATA->parmax[P.i_wood]=100000.0;

/*C CWD*/
CARDADATA->parmin[P.i_cwd]=1.0;
CARDADATA->parmax[P.i_cwd]=100000.0;

/*C litter*/
CARDADATA->parmin[P.i_lit]=1.0;
CARDADATA->parmax[P.i_lit]=2000.0;

/*C_som*/
CARDADATA->parmin[P.i_som]=1.0;
CARDADATA->parmax[P.i_som]=200000.0;

/*Retention parameter (b)*/
CARDADATA->parmin[P.retention]=1.5;
CARDADATA->parmax[P.retention]=10;

/*"Wilting point"*/
CARDADATA->parmin[P.wilting]=1;
CARDADATA->parmax[P.wilting]=10000;

/*"Bucket at t0"*/
CARDADATA->parmin[P.i_PAW]=1;
CARDADATA->parmax[P.i_PAW]=10000;

/*Foliar biomass CF*/
CARDADATA->parmin[P.cf_foliar]=0.01;
CARDADATA->parmax[P.cf_foliar]=1;

/*"Ligneous" biomass CF".*/
CARDADATA->parmin[P.cf_ligneous]=0.01;
CARDADATA->parmax[P.cf_ligneous]=1;

/*DOM CF".*/
CARDADATA->parmin[P.cf_DOM]=0.01;
CARDADATA->parmax[P.cf_DOM]=1;

/*Resilience factor (since transfer to litter is represented as (1-pars[30])) ".*/
CARDADATA->parmin[P.resilience]=0.01;
CARDADATA->parmax[P.resilience]=1;

/*Moisture factor*/
CARDADATA->parmin[P.moisture]=0.01;
CARDADATA->parmax[P.moisture]=1;

/*Saturated hydraulic conductivity (m/s)*/
CARDADATA->parmin[P.hydr_cond]=0.0000001;
CARDADATA->parmax[P.hydr_cond]=0.00001;

/*Maximum infiltration (mm/day)*/
CARDADATA->parmin[P.max_infil]=1;
CARDADATA->parmax[P.max_infil]=100;

/*PUW pool*/
CARDADATA->parmin[P.i_PUW]=1;
CARDADATA->parmax[P.i_PUW]=10000;

/*PAW porosity*/
CARDADATA->parmin[P.PAW_por]=0.2;
CARDADATA->parmax[P.PAW_por]=0.8;

/*PUW porosity*/
CARDADATA->parmin[P.PUW_por]=0.2;
CARDADATA->parmax[P.PUW_por]=0.8;

/*Field capacity (negative) potential (-Mpa)*/
CARDADATA->parmin[P.field_cap]=0.01;
CARDADATA->parmax[P.field_cap]=0.1;

/*PAW depth (m)*/
CARDADATA->parmin[P.PAW_z]=0.01;
CARDADATA->parmax[P.PAW_z]=100;

/*PUW depth (m)*/
CARDADATA->parmin[P.PUW_z]=0.01;
CARDADATA->parmax[P.PUW_z]=100;

/*Runoff excess*/
CARDADATA->parmin[P.Q_excess]=0.01;
CARDADATA->parmax[P.Q_excess]=1;

/*Medlyn g1*/
CARDADATA->parmin[P.Med_g1]=1.79;
CARDADATA->parmax[P.Med_g1]=5.79;

/*Vcmax25*/
CARDADATA->parmin[P.Vcmax25]=1e-8;
CARDADATA->parmax[P.Vcmax25]=140;

/*Tminmin scaling factor*/
CARDADATA->parmin[P.Tminmin]=258.15;
CARDADATA->parmax[P.Tminmin]=273.15;

/*Tminmax scaling factor*/
CARDADATA->parmin[P.Tminmax]=273.15;
CARDADATA->parmax[P.Tminmax]=288.15;

/*aerodynamic conductance*/
CARDADATA->parmin[P.ga]=0.01;
CARDADATA->parmax[P.ga]=10.0;

/*Tupp*/
CARDADATA->parmin[P.Tupp]=299.15;
CARDADATA->parmax[P.Tupp]=318.15;

/*Tdown*/
CARDADATA->parmin[P.Tdown]=263.15;
CARDADATA->parmax[P.Tdown]=286.15;

/*C3_frac*/
CARDADATA->parmin[P.C3_frac]=1e-8;
CARDADATA->parmax[P.C3_frac]=1.0;

/*Clumping index*/
CARDADATA->parmin[P.clumping]=0.35;
CARDADATA->parmax[P.clumping]=1.0;

/*Leaf single scattering albedo*/
CARDADATA->parmin[P.leaf_refl]=1e-8;
CARDADATA->parmax[P.leaf_refl]=1.0;

/*iSWE: initial for state variable SWE snow water equivalent*/
CARDADATA->parmin[P.i_SWE]=0.000001;
CARDADATA->parmax[P.i_SWE]=10000;

/*sn1: min threshold for melt*/
CARDADATA->parmin[P.min_melt]=240;
CARDADATA->parmax[P.min_melt]=270;

/*sn2: slope*/
CARDADATA->parmin[P.melt_slope]=0.00001;
CARDADATA->parmax[P.melt_slope]=1;

/*sn3: snow cover fraction scalar*/
CARDADATA->parmin[P.scf_scalar]=0.001;
CARDADATA->parmax[P.scf_scalar]=1000.0;

/* jc S_fv statistically fitting the fV curves (S1,S2,S3 schemes) with total soil moisture (PAW/PAW_fs)*/
/*jc new name for this par is S_fv, scalar for aerobic volumetric fraction */
CARDADATA->parmin[P.S_fv]=1;
CARDADATA->parmax[P.S_fv]=100.0;

/* jc thetas_opt   optimum thetas for water scaler fW*/
CARDADATA->parmin[P.thetas_opt]=0.2;
CARDADATA->parmax[P.thetas_opt]=1.0;

/* jc fwc the water scaler fW value at the end point C  */
CARDADATA->parmin[P.fwc]=0.01;
CARDADATA->parmax[P.fwc]=1.0;

/* jc r_ch4   CH4:CO2 conversion ratio*/
CARDADATA->parmin[P.r_ch4]=0.001;
CARDADATA->parmax[P.r_ch4]=0.9;

/* jc Q10ch4 Q10 for CH4 production  */
CARDADATA->parmin[P.Q10ch4]=1.0;
CARDADATA->parmax[P.Q10ch4]=3.0;

/* maxPevap in mm/day*/
CARDADATA->parmin[P.maxPevap]=0.01;
CARDADATA->parmax[P.maxPevap]=20;

/*Mean temperature at leaf onset (T_phi) (degrees kelvin)*/
CARDADATA->parmin[P.T_phi]=268.15;
CARDADATA->parmax[P.T_phi]=323.15;

/*Spatial range of mean temperature at leaf onset (T_r) (degrees C or degrees kelvin)*/
CARDADATA->parmin[P.T_range]=0.1;
CARDADATA->parmax[P.T_range]=10.0;

/*Averaging period for temperature growth trigger T (time units of model), usually kept constant*/
CARDADATA->parmin[P.tau_m]=1.0;
CARDADATA->parmax[P.tau_m]=1.01;

/*LAI linear growth constant (inverse of model time units; e.g. days-1 or months-1)*/
CARDADATA->parmin[P.plgr]=0.001;
CARDADATA->parmax[P.plgr]=0.5;

/*Inverse of leaf longevity during senescence period (inverse of model time units; e.g. days-1 or months-1)*/
CARDADATA->parmin[P.k_leaf]=0.001;
CARDADATA->parmax[P.k_leaf]=0.5;

/*Intrinsic maximum LAI (m^2 m^-2)*/
CARDADATA->parmin[P.lambda_max]=0.1;
CARDADATA->parmax[P.lambda_max]=10.0;

/*Target survival time for LAI under water-deficit conditions (days; or same unit as ET and PAW)*/
CARDADATA->parmin[P.tau_W]=0.1;
CARDADATA->parmax[P.tau_W]=300;

/*Mean daylength at leaf shedding (t_c; in units of hours sunlight per day)*/
CARDADATA->parmin[P.time_c]=2;
CARDADATA->parmax[P.time_c]=22;

/*Spatial range of mean daylength at leaf shedding (t_r)*/
CARDADATA->parmin[P.time_r]=0.1;
CARDADATA->parmax[P.time_r]=6.0;

/*initialization of temperature memory (fractional value between mintemp and maxtemp at t=0)*/
CARDADATA->parmin[P.init_T_mem]=0.01;
CARDADATA->parmax[P.init_T_mem]=1;

/*initialization of water/structural memory (fractional value of intrinsic maximum LAI)*/
CARDADATA->parmin[P.init_LAIW_mem]=0.01;
CARDADATA->parmax[P.init_LAIW_mem]=1;

/*Inverse of leaf longevity at any period i.e. background turnover (days-1)*/
CARDADATA->parmin[P.t_foliar]=0.001;
CARDADATA->parmax[P.t_foliar]=0.1;

return 0;

}


