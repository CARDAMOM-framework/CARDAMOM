#pragma once
#include "DALEC_1025.c"

/*PARAMETER_INFO (typedef struct) must have at least 3 fields
 *  * npars,
 *   * parmax
 *    * parmin*/
/*where is it defined?*/
/*For DALEC_FIRES: as all GPP allocation fractions are inter-dependent (sum = 1)*/
/*MCMC sampling of GPP allocation priors approximated as 0.01-0.5 NPP for*/
/*photosynthetic pools and 0.01-1 of remaining NPP for root and wood pool*/

int PARS_INFO_1025(DATA *CARDADATA)
{

struct DALEC_1025_PARAMETERS P=DALEC_1025_PARAMETERS;

/*Decomposition rate*/
CARDADATA->parmin[P.tr_lit2soil]=0.00001;
CARDADATA->parmax[P.tr_lit2soil]=0.01;

/*Fraction of GPP respired*/
CARDADATA->parmin[P.f_auto]=0.2;
CARDADATA->parmax[P.f_auto]=0.8;

/*Fraction of (1-fgpp) to foliage*/
CARDADATA->parmin[P.f_foliar]=0.01;
CARDADATA->parmax[P.f_foliar]=0.5;

/*Fraction of (1-fgpp) to roots*/
CARDADATA->parmin[P.f_root]=0.01;
CARDADATA->parmax[P.f_root]=1;

/*Leaf Lifespan*/
CARDADATA->parmin[P.t_foliar]=1.001;
CARDADATA->parmax[P.t_foliar]=8;

/*TOR wood* - 1% loss per year value*/
CARDADATA->parmin[P.t_wood]=0.000025;
CARDADATA->parmax[P.t_wood]=0.001;

/*TOR roots*/
CARDADATA->parmin[P.t_root]=0.0001;
CARDADATA->parmax[P.t_root]=0.01;

/*TOR litter*/
CARDADATA->parmin[P.t_lit]=0.0001;
CARDADATA->parmax[P.t_lit]=0.01;

/*TOR SOM*/
CARDADATA->parmin[P.t_soil]=0.0000001;
CARDADATA->parmax[P.t_soil]=0.001;

/*Temp factor* = Q10 = 1.2-1.6*/
CARDADATA->parmin[P.temp_factor]=0.018;
CARDADATA->parmax[P.temp_factor]=0.08;

/*Canopy Efficiency*/
CARDADATA->parmin[P.canopy_eff]=5;
CARDADATA->parmax[P.canopy_eff]=50;

/*Bday*/
CARDADATA->parmin[P.Bday]=365.25;
CARDADATA->parmax[P.Bday]=365.25*4;

/*Fraction to Clab*/
CARDADATA->parmin[P.f_lab]=0.01;
CARDADATA->parmax[P.f_lab]=0.5;

/*Clab Release period*/
CARDADATA->parmin[P.labile_rel]=365.25/12;
CARDADATA->parmax[P.labile_rel]=100;

/*Fday*/
CARDADATA->parmin[P.Fday]=365.25;
CARDADATA->parmax[P.Fday]=365.25*4;

/*Leaf fall period*/
CARDADATA->parmin[P.leaf_fall]=365.25/12;
CARDADATA->parmax[P.leaf_fall]=150;

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

/*C litter*/
CARDADATA->parmin[P.i_lit]=1.0;
CARDADATA->parmax[P.i_lit]=2000.0;

/*C_som*/
CARDADATA->parmin[P.i_soil]=1.0;
CARDADATA->parmax[P.i_soil]=200000.0;

/*uWUE: GPP*sqrt(VPD)/ET: gC/kgH2o *hPa*/
/*The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/

CARDADATA->parmin[P.uWUE]=0.5;
CARDADATA->parmax[P.uWUE]=30;

/*Runoff focal point (~maximum soil storage capacity x 4)*/
CARDADATA->parmin[P.PAW_Qmax]=1;
CARDADATA->parmax[P.PAW_Qmax]=100000;

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

/*Lab pool lifespan*/
CARDADATA->parmin[P.t_labile]=1.001;
CARDADATA->parmax[P.t_labile]=8;

/*Moisture factor*/
CARDADATA->parmin[P.moisture]=0.01;
CARDADATA->parmax[P.moisture]=1;

/*PAW->PUW runoff fraction*/
CARDADATA->parmin[P.h2o_xfer]=0.01;
CARDADATA->parmax[P.h2o_xfer]=1;

/*PUW Runoff focal point (~maximum soil storage capacity x 4)*/
CARDADATA->parmin[P.PUW_Qmax]=1;
CARDADATA->parmax[P.PUW_Qmax]=100000;

/*PUW pool*/
CARDADATA->parmin[P.i_PUW]=1;
CARDADATA->parmax[P.i_PUW]=10000;

/*r: The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/
CARDADATA->parmin[P.boese_r]=0.01;
CARDADATA->parmax[P.boese_r]=0.3;

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

return 0;

}


