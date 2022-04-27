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

int PARS_INFO_1006(DATA *CARDADATA)
{

struct DALEC_1006_PARAMETERS P=DALEC_1006_PARAMETERS;

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
CARDADATA->parmin[P.i_som]=1.0;
CARDADATA->parmax[P.i_som]=200000.0;

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

return 0;

}


