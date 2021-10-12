#pragma once
#include "DALEC_1012.c"

/*PARAMETER_INFO (typedef struct) must have at least 3 fields
 *  * npars,
 *   * parmax
 *    * parmin*/
/*where is it defined?*/
/*For DALEC_FIRES: as all GPP allocation fractions are inter-dependent (sum = 1)*/
/*MCMC sampling of GPP allocation priors approximated as 0.01-0.5 NPP for*/
/*photosynthetic pools and 0.01-1 of remaining NPP for root and wood pool*/

int PARS_INFO_1012(DATA *CARDADATA)
{


struct DALEC_1012_PARAMETERS P=DALEC_1012_PARAMETERS;

/* 0 Decomposition rate*/
CARDADATA->parmin[P.tr_lit2som]=0.00001;
CARDADATA->parmax[P.tr_lit2som]=0.01;

/* 1 Fraction of GPP respired*/
CARDADATA->parmin[P.f_auto]=0.2;
CARDADATA->parmax[P.f_auto]=0.8;

/* 2 Fraction of (1-fgpp) to foliage*/
CARDADATA->parmin[P.f_foliar]=0.01;
CARDADATA->parmax[P.f_foliar]=0.5;

/* 3 Fraction of (1-fgpp) to roots*/
CARDADATA->parmin[P.f_root]=0.01;
CARDADATA->parmax[P.f_root]=1;

/* 4 Leaf Lifespan*/
CARDADATA->parmin[P.t_foliar]=1.001;
CARDADATA->parmax[P.t_foliar]=8;

/* 5 TOR wood* - 1% loss per year value*/
CARDADATA->parmin[P.t_wood]=0.000025;
CARDADATA->parmax[P.t_wood]=0.001;

/* 6 TOR roots*/
CARDADATA->parmin[P.t_root]=0.0001;
CARDADATA->parmax[P.t_root]=0.01;

/* 7 TOR litter*/
CARDADATA->parmin[P.t_lit]=0.0001;
CARDADATA->parmax[P.t_lit]=0.01;

/* 8 TOR SOM*/
CARDADATA->parmin[P.t_som]=0.0000001;
CARDADATA->parmax[P.t_som]=0.001;

/* 9 Temp factor* convert to Q10_rhco2 = 1.2-1.6*/
//CARDADATA->parmin[P.temp_factor]=0.018;
//CARDADATA->parmax[P.temp_factor]=0.08;
CARDADATA->parmin[P.Q10rhco2]=1.2;
CARDADATA->parmax[P.Q10rhco2]=2;

/* 10 Canopy Efficiency*/
CARDADATA->parmin[P.canopy_eff]=5;
CARDADATA->parmax[P.canopy_eff]=50;

/* 11 Bday*/
CARDADATA->parmin[P.Bday]=365.25;
CARDADATA->parmax[P.Bday]=365.25*4;

/* 12 Fraction to Clab*/
CARDADATA->parmin[P.f_lab]=0.01;
CARDADATA->parmax[P.f_lab]=0.5;

/* 13 Clab Release period*/
CARDADATA->parmin[P.labile_rel]=365.25/12;
CARDADATA->parmax[P.labile_rel]=100;

/* 14 Fday*/
CARDADATA->parmin[P.Fday]=365.25;
CARDADATA->parmax[P.Fday]=365.25*4;

/* 15 Leaf fall period*/
CARDADATA->parmin[P.leaf_fall]=365.25/12;
CARDADATA->parmax[P.leaf_fall]=150;

/* 16 LMCA*/
/*Kattge et al. 2011*/
/*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*/
CARDADATA->parmin[P.LCMA]=5;
CARDADATA->parmax[P.LCMA]=200;

/*INITIAL VALUES DECLARED HERE*/

/* 17 C labile*/
CARDADATA->parmin[P.i_labile]=1.0;
CARDADATA->parmax[P.i_labile]=2000.0;

/* 18 C foliar*/
CARDADATA->parmin[P.i_foliar]=1.0;
CARDADATA->parmax[P.i_foliar]=2000.0;

/* 19 C roots*/
CARDADATA->parmin[P.i_root]=1.0;
CARDADATA->parmax[P.i_root]=2000.0;

/* 20 C_wood*/
CARDADATA->parmin[P.i_wood]=1.0;
CARDADATA->parmax[P.i_wood]=100000.0;

/* 21 C litter*/
CARDADATA->parmin[P.i_lit]=1.0;
CARDADATA->parmax[P.i_lit]=2000.0;

/* 22 C_som*/
CARDADATA->parmin[P.i_som]=1.0;
CARDADATA->parmax[P.i_som]=200000.0;

/* 23 uWUE: GPP*sqrt(VPD)/ET: gC/kgH2o *hPa*/
/*The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/
CARDADATA->parmin[P.uWUE]=0.5;
CARDADATA->parmax[P.uWUE]=30;

/* 24 Runoff focal point (~maximum soil storage capacity x 4)*/
CARDADATA->parmin[P.PAW_Qmax]=1;
CARDADATA->parmax[P.PAW_Qmax]=100000;

/* 25 "Wilting point"*/
CARDADATA->parmin[P.wilting]=1;
CARDADATA->parmax[P.wilting]=10000;

/* 26 "Bucket at t0"*/
CARDADATA->parmin[P.i_PAW]=1;
CARDADATA->parmax[P.i_PAW]=10000;

/* 27 Foliar biomass CF*/
CARDADATA->parmin[P.cf_foliar]=0.01;
CARDADATA->parmax[P.cf_foliar]=1;

/* 28 "Ligneous" biomass CF".*/
CARDADATA->parmin[P.cf_ligneous]=0.01;
CARDADATA->parmax[P.cf_ligneous]=1;

/* 29 DOM CF".*/
CARDADATA->parmin[P.cf_DOM]=0.01;
CARDADATA->parmax[P.cf_DOM]=1;

/* 30 Resilience factor (since transfer to litter is represented as (1-pars[30])) ".*/
CARDADATA->parmin[P.resilience]=0.01;
CARDADATA->parmax[P.resilience]=1;

/* 31 Lab pool lifespan*/
CARDADATA->parmin[P.t_labile]=1.001;
CARDADATA->parmax[P.t_labile]=8;

/* 32 Moisture factor*/
CARDADATA->parmin[P.moisture]=0.01;
CARDADATA->parmax[P.moisture]=1;

/* 33 PAW->PUW runoff fraction*/
CARDADATA->parmin[P.h2o_xfer]=0.01;
CARDADATA->parmax[P.h2o_xfer]=1;

/* 34 PUW Runoff focal point (~maximum soil storage capacity x 4)*/
CARDADATA->parmin[P.PUW_Qmax]=1;
CARDADATA->parmax[P.PUW_Qmax]=100000;

/* 35 PUW pool*/
CARDADATA->parmin[P.i_PUW]=1;
CARDADATA->parmax[P.i_PUW]=10000;

/* 36 jc PAW_fs   fully saturated PAW*/
CARDADATA->parmin[P.PAW_fs]=10.0;
CARDADATA->parmax[P.PAW_fs]=100000.0;

/* 37 jc S_fv statistically fitting the fV curves (S1,S2,S3 schemes) with total soil moisture (PAW/PAW_fs)*/
/*jc new name for this par is S_fv, scalar for aerobic volumetric fraction */
CARDADATA->parmin[P.S_fv]=1;
CARDADATA->parmax[P.S_fv]=100.0;

/* 38 jc thetas_opt   optimum thetas for water scaler fW*/
CARDADATA->parmin[P.thetas_opt]=0.2;
CARDADATA->parmax[P.thetas_opt]=1.0;

/* 39 jc fwc the water scaler fW value at the end point C  */
CARDADATA->parmin[P.fwc]=0.01;
CARDADATA->parmax[P.fwc]=1.0;

/* 40 jc r_ch4   CH4:CO2 conversion ratio*/
CARDADATA->parmin[P.r_ch4]=0.001;
CARDADATA->parmax[P.r_ch4]=0.9;

/* 41 jc Q10ch4 Q10 for CH4 production  */
CARDADATA->parmin[P.Q10ch4]=1.0;
CARDADATA->parmax[P.Q10ch4]=3.0;

/* 42 jc v2 rET radiation coefficient in ET, unitless, follow Yan's ET scheme */
CARDADATA->parmin[P.boese_r]=0.01;
CARDADATA->parmax[P.boese_r]=0.3;

/* 43 jc v2 Imax maximum infiltration rate, mm/day same with prec, partially follow Elias's hydrology */
CARDADATA->parmin[P.Imax]=100/(365.25/12);
CARDADATA->parmax[P.Imax]=2000/(365.25/12);

return 0;

}
