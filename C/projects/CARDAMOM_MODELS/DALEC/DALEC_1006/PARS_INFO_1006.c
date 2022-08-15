#pragma once
#include "DALEC_1006.c"

/*PARAMETER_INFO (typedef struct) must have at least 3 fields
 *  * npars,
 *   * parmax
 *    * parmin*/
/*where is it defined?*/
/*For DALEC_FIRES: as all GPP allocation fractions are inter-dependent (sum = 1)*/
/*MCMC sampling of GPP allocation priors approximated as 0.01-0.5 NPP for*/
/*photosynthetic pools and 0.01-1 of remaining NPP for root and wood pool*/

int PARS_INFO_1006(double *parmin, double *parmax)
{

struct DALEC_1006_PARAMETERS P=DALEC_1006_PARAMETERS;

/*Decomposition rate*/
parmin[P.tr_lit2soil]=0.00001;
parmax[P.tr_lit2soil]=0.01;

/*Fraction of GPP respired*/
parmin[P.f_auto]=0.2;
parmax[P.f_auto]=0.8;

/*Fraction of (1-fgpp) to foliage*/
parmin[P.f_foliar]=0.01;
parmax[P.f_foliar]=0.5;

/*Fraction of (1-fgpp) to roots*/
parmin[P.f_root]=0.01;
parmax[P.f_root]=1;

/*Leaf Lifespan*/
parmin[P.t_foliar]=1.001;
parmax[P.t_foliar]=8;

/*TOR wood* - 1% loss per year value*/
parmin[P.t_wood]=0.000025;
parmax[P.t_wood]=0.001;

/*TOR roots*/
parmin[P.t_root]=0.0001;
parmax[P.t_root]=0.01;

/*TOR litter*/
parmin[P.t_lit]=0.0001;
parmax[P.t_lit]=0.01;

/*TOR SOM*/
parmin[P.t_soil]=0.0000001;
parmax[P.t_soil]=0.001;

/*Temp factor* = Q10 = 1.2-1.6*/
parmin[P.temp_factor]=0.018;
parmax[P.temp_factor]=0.08;

/*Canopy Efficiency*/
parmin[P.canopy_eff]=5;
parmax[P.canopy_eff]=50;

/*Bday*/
parmin[P.Bday]=365.25;
parmax[P.Bday]=365.25*4;

/*Fraction to Clab*/
parmin[P.f_lab]=0.01;
parmax[P.f_lab]=0.5;

/*Clab Release period*/
parmin[P.labile_rel]=365.25/12;
parmax[P.labile_rel]=100;

/*Fday*/
parmin[P.Fday]=365.25;
parmax[P.Fday]=365.25*4;

/*Leaf fall period*/
parmin[P.leaf_fall]=365.25/12;
parmax[P.leaf_fall]=150;

/*LMCA*/
/*Kattge et al. 2011*/
/*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*/
parmin[P.LCMA]=5;
parmax[P.LCMA]=200;

/*INITIAL VALUES DECLARED HERE*/

/*C labile*/
parmin[P.i_labile]=1.0;
parmax[P.i_labile]=2000.0;

/*C foliar*/
parmin[P.i_foliar]=1.0;
parmax[P.i_foliar]=2000.0;

/*C roots*/
parmin[P.i_root]=1.0;
parmax[P.i_root]=2000.0;

/*C_wood*/
parmin[P.i_wood]=1.0;
parmax[P.i_wood]=100000.0;

/*C litter*/
parmin[P.i_lit]=1.0;
parmax[P.i_lit]=2000.0;

/*C_som*/
parmin[P.i_som]=1.0;
parmax[P.i_som]=200000.0;

/*uWUE: GPP*sqrt(VPD)/ET: gC/kgH2o *hPa*/
/*The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/
parmin[P.uWUE]=0.5;
parmax[P.uWUE]=30;

/*Runoff focal point (~maximum soil storage capacity x 4)*/
parmin[P.PAW_Qmax]=1;
parmax[P.PAW_Qmax]=100000;

/*"Wilting point"*/
parmin[P.wilting]=1;
parmax[P.wilting]=10000;

/*"Bucket at t0"*/
parmin[P.i_PAW]=1;
parmax[P.i_PAW]=10000;

/*Foliar biomass CF*/
parmin[P.cf_foliar]=0.01;
parmax[P.cf_foliar]=1;

/*"Ligneous" biomass CF".*/
parmin[P.cf_ligneous]=0.01;
parmax[P.cf_ligneous]=1;

/*DOM CF".*/
parmin[P.cf_DOM]=0.01;
parmax[P.cf_DOM]=1;

/*Resilience factor (since transfer to litter is represented as (1-pars[30])) ".*/
parmin[P.resilience]=0.01;
parmax[P.resilience]=1;

/*Lab pool lifespan*/
parmin[P.t_labile]=1.001;
parmax[P.t_labile]=8;

/*Moisture factor*/
parmin[P.moisture]=0.01;
parmax[P.moisture]=1;

/*PAW->PUW runoff fraction*/
parmin[P.h2o_xfer]=0.01;
parmax[P.h2o_xfer]=1;

/*PUW Runoff focal point (~maximum soil storage capacity x 4)*/
parmin[P.PUW_Qmax]=1;
parmax[P.PUW_Qmax]=100000;

/*PUW pool*/
parmin[P.i_PUW]=1;
parmax[P.i_PUW]=10000;

/*r: The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/
parmin[P.boese_r]=0.01;
parmax[P.boese_r]=0.3;

return 0;

}


