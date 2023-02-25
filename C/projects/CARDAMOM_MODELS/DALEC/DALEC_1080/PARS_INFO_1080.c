#pragma once
#include "DALEC_1080.c"

/*PARAMETER_INFO (typedef struct) must have at least 3 fields
 *  * npars,
 *   * parmax
 *    * parmin*/
/*where is it defined?*/
/*For DALEC_FIRES: as all GPP allocation fractions are inter-dependent (sum = 1)*/
/*MCMC sampling of GPP allocation priors approximated as 0.01-0.5 NPP for*/
/*photosynthetic pools and 0.01-1 of remaining NPP for root and wood pool*/

int PARS_INFO_1080(DATA *CARDADATA)
{

struct DALEC_1080_PARAMETERS P=DALEC_1080_PARAMETERS;

double * parmin=CARDADATA->parmin;
double * parmax=CARDADATA->parmax;
/*Decomposition rate*/
parmin[P.tr_lit2soil_1]=0.00001;
parmax[P.tr_lit2soil_1]=0.01;

/*Fraction of GPP respired*/
parmin[P.f_auto_1]=0.2;
parmax[P.f_auto_1]=0.8;

/*Fraction of (1-fgpp) to foliage*/
parmin[P.f_foliar_1]=0.01;
parmax[P.f_foliar_1]=0.5;

/*Fraction of (1-fgpp) to roots*/
parmin[P.f_root_1]=0.01;
parmax[P.f_root_1]=1;

/*Leaf Lifespan*/
parmin[P.t_foliar_1]=1.001;
parmax[P.t_foliar_1]=8;

/*TOR wood* - 1% loss per year value*/
parmin[P.t_wood_1]=0.000025;
parmax[P.t_wood_1]=0.001;

/*TOR roots*/
parmin[P.t_root_1]=0.0001;
parmax[P.t_root_1]=0.01;

/*TOR litter*/
parmin[P.t_lit_1]=0.0001;
parmax[P.t_lit_1]=0.01;

/*TOR SOM*/
parmin[P.t_soil_1]=0.0000001;
parmax[P.t_soil_1]=0.001;

/*Temp factor* = Q10 = 1.2-1.6*/
parmin[P.temp_factor_1]=0.018;
parmax[P.temp_factor_1]=0.08;

/*Canopy Efficiency*/
parmin[P.canopy_eff_1]=5;
parmax[P.canopy_eff_1]=50;

/*Bday*/
parmin[P.Bday_1]=365.25;
parmax[P.Bday_1]=365.25*4;

/*Fraction to Clab*/
parmin[P.f_lab_1]=0.01;
parmax[P.f_lab_1]=0.5;

/*Clab Release period*/
parmin[P.labile_rel_1]=365.25/12;
parmax[P.labile_rel_1]=100;

/*Fday*/
parmin[P.Fday_1]=365.25;
parmax[P.Fday_1]=365.25*4;

/*Leaf fall period*/
parmin[P.leaf_fall_1]=365.25/12;
parmax[P.leaf_fall_1]=150;

/*LMCA*/
/*Kattge et al. 2011*/
/*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*/
parmin[P.LCMA_1]=5;
parmax[P.LCMA_1]=200;

/*INITIAL VALUES DECLARED HERE*/

/*C labile*/
parmin[P.i_labile_1]=1.0;
parmax[P.i_labile_1]=2000.0;

/*C foliar*/
parmin[P.i_foliar_1]=1.0;
parmax[P.i_foliar_1]=2000.0;

/*C roots*/
parmin[P.i_root_1]=1.0;
parmax[P.i_root_1]=2000.0;

/*C_wood*/
parmin[P.i_wood_1]=1.0;
parmax[P.i_wood_1]=100000.0;

/*C litter*/
parmin[P.i_lit_1]=1.0;
parmax[P.i_lit_1]=2000.0;

/*C_som*/
parmin[P.i_soil_1]=1.0;
parmax[P.i_soil_1]=200000.0;

/*uWUE: GPP*sqrt(VPD)/ET: gC/kgH2o *hPa*/
/*The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/

parmin[P.uWUE_1]=0.5;
parmax[P.uWUE_1]=30;

/*Runoff focal point (~maximum soil storage capacity x 4)*/
parmin[P.PAW_Qmax_1]=1;
parmax[P.PAW_Qmax_1]=100000;

/*"Wilting point"*/
parmin[P.wilting_1]=1;
parmax[P.wilting_1]=10000;

/*"Bucket at t0"*/
parmin[P.i_PAW_1]=1;
parmax[P.i_PAW_1]=10000;

/*Foliar biomass CF*/
parmin[P.cf_foliar_1]=0.01;
parmax[P.cf_foliar_1]=1;

/*"Ligneous" biomass CF".*/
parmin[P.cf_ligneous_1]=0.01;
parmax[P.cf_ligneous_1]=1;

/*DOM CF".*/
parmin[P.cf_DOM_1]=0.01;
parmax[P.cf_DOM_1]=1;

/*Resilience factor (since transfer to litter is represented as (1-pars[30_1])) ".*/
parmin[P.resilience_1]=0.01;
parmax[P.resilience_1]=1;

/*Lab pool lifespan*/
parmin[P.t_labile_1]=1.001;
parmax[P.t_labile_1]=8;

/*Moisture factor*/
parmin[P.moisture_1]=0.01;
parmax[P.moisture_1]=1;

/*PAW->PUW runoff fraction*/
parmin[P.h2o_xfer_1]=0.01;
parmax[P.h2o_xfer_1]=1;

/*PUW Runoff focal point (~maximum soil storage capacity x 4)*/
parmin[P.PUW_Qmax_1]=1;
parmax[P.PUW_Qmax_1]=100000;

/*PUW pool*/
parmin[P.i_PUW_1]=1;
parmax[P.i_PUW_1]=10000;

/*r: The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/
parmin[P.boese_r_1]=0.01;
parmax[P.boese_r_1]=0.3;




/*Decomposition rate*/
parmin[P.tr_lit2soil_2]=0.00001;
parmax[P.tr_lit2soil_2]=0.01;

/*Fraction of GPP respired*/
parmin[P.f_auto_2]=0.2;
parmax[P.f_auto_2]=0.8;

/*Fraction of (1-fgpp) to foliage*/
parmin[P.f_foliar_2]=0.01;
parmax[P.f_foliar_2]=0.5;

/*Fraction of (1-fgpp) to roots*/
parmin[P.f_root_2]=0.01;
parmax[P.f_root_2]=1;

/*Leaf Lifespan*/
parmin[P.t_foliar_2]=1.001;
parmax[P.t_foliar_2]=8;

/*TOR wood* - 1% loss per year value*/
parmin[P.t_wood_2]=0.000025;
parmax[P.t_wood_2]=0.001;

/*TOR roots*/
parmin[P.t_root_2]=0.0001;
parmax[P.t_root_2]=0.01;

/*TOR litter*/
parmin[P.t_lit_2]=0.0001;
parmax[P.t_lit_2]=0.01;

/*TOR SOM*/
parmin[P.t_soil_2]=0.0000001;
parmax[P.t_soil_2]=0.001;

/*Temp factor* = Q10 = 1.2-1.6*/
parmin[P.temp_factor_2]=0.018;
parmax[P.temp_factor_2]=0.08;

/*Canopy Efficiency*/
parmin[P.canopy_eff_2]=5;
parmax[P.canopy_eff_2]=50;

/*Bday*/
parmin[P.Bday_2]=365.25;
parmax[P.Bday_2]=365.25*4;

/*Fraction to Clab*/
parmin[P.f_lab_2]=0.01;
parmax[P.f_lab_2]=0.5;

/*Clab Release period*/
parmin[P.labile_rel_2]=365.25/12;
parmax[P.labile_rel_2]=100;

/*Fday*/
parmin[P.Fday_2]=365.25;
parmax[P.Fday_2]=365.25*4;

/*Leaf fall period*/
parmin[P.leaf_fall_2]=365.25/12;
parmax[P.leaf_fall_2]=150;

/*LMCA*/
/*Kattge et al. 2011*/
/*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*/
parmin[P.LCMA_2]=5;
parmax[P.LCMA_2]=200;

/*INITIAL VALUES DECLARED HERE*/

/*C labile*/
parmin[P.i_labile_2]=1.0;
parmax[P.i_labile_2]=2000.0;

/*C foliar*/
parmin[P.i_foliar_2]=1.0;
parmax[P.i_foliar_2]=2000.0;

/*C roots*/
parmin[P.i_root_2]=1.0;
parmax[P.i_root_2]=2000.0;

/*C_wood*/
parmin[P.i_wood_2]=1.0;
parmax[P.i_wood_2]=100000.0;

/*C litter*/
parmin[P.i_lit_2]=1.0;
parmax[P.i_lit_2]=2000.0;

/*C_som*/
parmin[P.i_soil_2]=1.0;
parmax[P.i_soil_2]=200000.0;

/*uWUE: GPP*sqrt(VPD)/ET: gC/kgH2o *hPa*/
/*The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/

parmin[P.uWUE_2]=0.5;
parmax[P.uWUE_2]=30;

/*Runoff focal point (~maximum soil storage capacity x 4)*/
parmin[P.PAW_Qmax_2]=1;
parmax[P.PAW_Qmax_2]=100000;

/*"Wilting point"*/
parmin[P.wilting_2]=1;
parmax[P.wilting_2]=10000;

/*"Bucket at t0"*/
parmin[P.i_PAW_2]=1;
parmax[P.i_PAW_2]=10000;

/*Foliar biomass CF*/
parmin[P.cf_foliar_2]=0.01;
parmax[P.cf_foliar_2]=1;

/*"Ligneous" biomass CF".*/
parmin[P.cf_ligneous_2]=0.01;
parmax[P.cf_ligneous_2]=1;

/*DOM CF".*/
parmin[P.cf_DOM_2]=0.01;
parmax[P.cf_DOM_2]=1;

/*Resilience factor (since transfer to litter is represented as (1-pars[30_2])) ".*/
parmin[P.resilience_2]=0.01;
parmax[P.resilience_2]=1;

/*Lab pool lifespan*/
parmin[P.t_labile_2]=1.001;
parmax[P.t_labile_2]=8;

/*Moisture factor*/
parmin[P.moisture_2]=0.01;
parmax[P.moisture_2]=1;

/*PAW->PUW runoff fraction*/
parmin[P.h2o_xfer_2]=0.01;
parmax[P.h2o_xfer_2]=1;

/*PUW Runoff focal point (~maximum soil storage capacity x 4)*/
parmin[P.PUW_Qmax_2]=1;
parmax[P.PUW_Qmax_2]=100000;

/*PUW pool*/
parmin[P.i_PUW_2]=1;
parmax[P.i_PUW_2]=10000;

/*r: The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/
parmin[P.boese_r_2]=0.01;
parmax[P.boese_r_2]=0.3;

return 0;

}


