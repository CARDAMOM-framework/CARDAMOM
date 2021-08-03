#pragma once
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

/*Decomposition rate*/
CARDADATA->parmin[0]=0.00001;
CARDADATA->parmax[0]=0.01;

/*Fraction of GPP respired*/
CARDADATA->parmin[1]=0.2;
CARDADATA->parmax[1]=0.8;

/*Fraction of (1-fgpp) to foliage*/
CARDADATA->parmin[2]=0.01;
CARDADATA->parmax[2]=0.5;

/*Fraction of (1-fgpp) to roots*/
CARDADATA->parmin[3]=0.01;
CARDADATA->parmax[3]=1;

/*Leaf Lifespan*/
CARDADATA->parmin[4]=1.001;
CARDADATA->parmax[4]=8;

/*TOR wood* - 1% loss per year value*/
CARDADATA->parmin[5]=0.000025;
CARDADATA->parmax[5]=0.001;

/*TOR roots*/
CARDADATA->parmin[6]=0.0001;
CARDADATA->parmax[6]=0.01;

/*TOR litter*/
CARDADATA->parmin[7]=0.0001;
CARDADATA->parmax[7]=0.01;

/*TOR SOM*/
CARDADATA->parmin[8]=0.0000001;
CARDADATA->parmax[8]=0.001;

/*Temp factor* = Q10 = 1.2-1.6*/
CARDADATA->parmin[9]=0.018;
CARDADATA->parmax[9]=0.08;

/*Canopy Efficiency*/
CARDADATA->parmin[10]=5;
CARDADATA->parmax[10]=50;

/*Bday*/
CARDADATA->parmin[11]=365.25;
CARDADATA->parmax[11]=365.25*4;

/*Fraction to Clab*/
CARDADATA->parmin[12]=0.01;
CARDADATA->parmax[12]=0.5;

/*Clab Release period*/
CARDADATA->parmin[13]=365.25/12;
CARDADATA->parmax[13]=100;

/*Fday*/
CARDADATA->parmin[14]=365.25;
CARDADATA->parmax[14]=365.25*4;

/*Leaf fall period*/
CARDADATA->parmin[15]=365.25/12;
CARDADATA->parmax[15]=150;

/*LMCA*/
/*Kattge et al. 2011*/
/*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*/
CARDADATA->parmin[16]=5;
CARDADATA->parmax[16]=200;

/*INITIAL VALUES DECLARED HERE*/

/*C labile*/
CARDADATA->parmin[17]=1.0;
CARDADATA->parmax[17]=2000.0;

/*C foliar*/
CARDADATA->parmin[18]=1.0;
CARDADATA->parmax[18]=2000.0;

/*C roots*/
CARDADATA->parmin[19]=1.0;
CARDADATA->parmax[19]=2000.0;

/*C_wood*/
CARDADATA->parmin[20]=1.0;
CARDADATA->parmax[20]=100000.0;

/*C litter*/
CARDADATA->parmin[21]=1.0;
CARDADATA->parmax[21]=2000.0;

/*C_som*/
CARDADATA->parmin[22]=1.0;
CARDADATA->parmax[22]=200000.0;

/*uWUE: GPP*sqrt(VPD)/ET: gC/kgH2o *hPa*/
CARDADATA->parmin[23]=0.5;
CARDADATA->parmax[23]=30;

/*Retention parameter (b)*/
CARDADATA->parmin[24]=1.5;
CARDADATA->parmax[24]=10;

/*"Wilting point"*/
CARDADATA->parmin[25]=1;
CARDADATA->parmax[25]=10000;

/*"Bucket at t0"*/
CARDADATA->parmin[26]=1;
CARDADATA->parmax[26]=10000;

/*Foliar biomass CF*/
CARDADATA->parmin[27]=0.01;
CARDADATA->parmax[27]=1;

/*"Ligneous" biomass CF".*/
CARDADATA->parmin[28]=0.01;
CARDADATA->parmax[28]=1;

/*DOM CF".*/
CARDADATA->parmin[29]=0.01;
CARDADATA->parmax[29]=1;

/*Resilience factor (since transfer to litter is represented as (1-pars[30])) ".*/
CARDADATA->parmin[30]=0.01;
CARDADATA->parmax[30]=1;

/*Lab pool lifespan*/
CARDADATA->parmin[31]=1.001;
CARDADATA->parmax[31]=8;

/*Moisture factor*/
CARDADATA->parmin[32]=0.01;
CARDADATA->parmax[32]=1;

/*Saturated hydraulic conductivity (m/s)*/
CARDADATA->parmin[33]=0.0000001;
CARDADATA->parmax[33]=0.00001;

/*Maximum infiltration (mm/day)*/
CARDADATA->parmin[34]=1;
CARDADATA->parmax[34]=1e4;

/*PUW pool*/
CARDADATA->parmin[35]=1;
CARDADATA->parmax[35]=10000;

/*Reference VPD */
CARDADATA->parmin[36]=10;
CARDADATA->parmax[36]=10000;

/*VPD curvature exponent */
CARDADATA->parmin[37]=0.001;
CARDADATA->parmax[37]=1000;

/*r: The chosen prior range in r conservatively captures the range of values by Boese et al.(2017)*/
CARDADATA->parmin[38]=0.01;
CARDADATA->parmax[38]=0.3;

/*PAW porosity*/
CARDADATA->parmin[39]=0.2;
CARDADATA->parmax[39]=0.8;

/*PUW porosity*/
CARDADATA->parmin[40]=0.2;
CARDADATA->parmax[40]=0.8;

/*Field capacity (negative) potential (-Mpa)*/
CARDADATA->parmin[41]=0.01;
CARDADATA->parmax[41]=0.1;

/*PAW depth (m)*/
CARDADATA->parmin[42]=0.01;
CARDADATA->parmax[42]=100;

/*PUW depth (m)*/
CARDADATA->parmin[43]=0.01;
CARDADATA->parmax[43]=100;

/*Runoff excess*/
CARDADATA->parmin[44]=0.01;
CARDADATA->parmax[44]=1;

/*Medlyn g1*/
CARDADATA->parmin[45]=1.79;
CARDADATA->parmax[45]=5.79;

/*Vcmax25*/
CARDADATA->parmin[46]=1e-8;
CARDADATA->parmax[46]=140;

/*Tminmin scaling factor*/
CARDADATA->parmin[47]=258.15;
CARDADATA->parmax[47]=273.15;

/*Tminmax scaling factor*/
CARDADATA->parmin[48]=273.15;
CARDADATA->parmax[48]=288.15;

/*aerodynamic conductance*/
CARDADATA->parmin[49]=1.e-6;
CARDADATA->parmax[49]=2.0;

/*Tupp*/
CARDADATA->parmin[50]=299.15;
CARDADATA->parmax[50]=318.15;

/*Tdown*/
CARDADATA->parmin[51]=263.15;
CARDADATA->parmax[51]=286.15;

/*C3_frac*/
CARDADATA->parmin[52]=1e-8;
CARDADATA->parmax[52]=1.0;

/*Initial LAI*/
CARDADATA->parmin[53]=0.1;
CARDADATA->parmax[53]=10.0;

/*Mean temperature at leaf onset (T_phi) (degrees kelvin)*/
CARDADATA->parmin[54]=268.15;
CARDADATA->parmax[54]=323.15;

/*Spatial range of mean temperature at leaf onset (T_r) (degrees C or degrees kelvin)*/
CARDADATA->parmin[55]=0.1;
CARDADATA->parmax[55]=10.0;

/*Averaging period for temperature growth trigger T (time units of model), usually kept constant*/
CARDADATA->parmin[56]=1.0;
CARDADATA->parmax[56]=1.01;

/*LAI linear growth constant (inverse of model time units; e.g. days-1 or months-1)*/
CARDADATA->parmin[57]=0.001;
CARDADATA->parmax[57]=0.5;

/*Inverse of leaf longevity during senescence period (inverse of model time units; e.g. days-1 or months-1)*/
CARDADATA->parmin[58]=0.001;
CARDADATA->parmax[58]=0.5;

/*Intrinsic maximum LAI (m^2 m^-2)*/
CARDADATA->parmin[59]=0.1;
CARDADATA->parmax[59]=10.0;

/*Target survival time for LAI under water-deficit conditions (days; or same unit as ET and PAW)*/
CARDADATA->parmin[60]=0.1;
CARDADATA->parmax[60]=300;

/*Mean daylength at leaf shedding (t_c; in units of hours sunlight per day)*/
CARDADATA->parmin[61]=2;
CARDADATA->parmax[61]=22;

/*Spatial range of mean daylength at leaf shedding (t_r)*/
CARDADATA->parmin[62]=0.1;
CARDADATA->parmax[62]=6.0;


return 0;

}


