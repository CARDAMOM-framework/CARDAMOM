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

/*Lab pool lifespan*/
CARDADATA->parmin[P.t_labile]=1.001;
CARDADATA->parmax[P.t_labile]=8;

/*Moisture factor*/
CARDADATA->parmin[P.moisture]=0.01;
CARDADATA->parmax[P.moisture]=1;

/*Saturated hydraulic conductivity (m/s)*/
CARDADATA->parmin[P.hydr_cond]=0.0000001;
CARDADATA->parmax[P.hydr_cond]=0.00001;

/*Maximum infiltration (mm/day)*/
CARDADATA->parmin[P.max_infil]=1;
CARDADATA->parmax[P.max_infil]=1e4;

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
CARDADATA->parmin[P.ga]=1.e-6;
CARDADATA->parmax[P.ga]=2.0;

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
CARDADATA->parmin[P.clumping]=1e-8;
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


return 0;

}


