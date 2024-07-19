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
/*Litter decomposition rate*/
parmin[P.tr_lit2som]=0.01;
parmax[P.tr_lit2som]=0.99;

/*CWD decomposition rate*/
parmin[P.tr_cwd2som]=0.01;
parmax[P.tr_cwd2som]=0.99;

/*Autotrophic maintenance respiration coefficient for roots - turnover rate at 25oC (d-1)*/
parmin[P.rauto_mr_r]=0.0001;
parmax[P.rauto_mr_r]=0.01;

/*Autotrophic maintenance respiration coefficient for wood - turnover rate at 25oC (d-1)*/
parmin[P.rauto_mr_w]=0.000001;
parmax[P.rauto_mr_w]=0.005;

/*Autotrophic maintenance respiration Q10 parameter*/
parmin[P.rauto_mr_q10]=1.0;
parmax[P.rauto_mr_q10]=5.0;

/*Dark respiration Q10 parameter*/
parmin[P.rauto_mrd_q10]=1.0;
parmax[P.rauto_mrd_q10]=5.0;

/*Growth yield (g C appearing in new biomass per g C used for growth; "growth yield" in Cannell and Thornley 2000)*/
parmin[P.rauto_gr]=0.60;
parmax[P.rauto_gr]=0.95;

/*TOR wood* - 1% loss per year value*/
parmin[P.t_wood]=0.000025;
parmax[P.t_wood]=0.01;

/*TOR roots*/
parmin[P.t_root]=0.0001;
parmax[P.t_root]=0.01;

/*Inverse of leaf longevity at any period i.e. background turnover (days-1)*/
parmin[P.t_foliar]=0.00068; /*around 4 yrs*/
parmax[P.t_foliar]=0.0334;

/*TOR litter*/
parmin[P.t_lit]=0.0001;
parmax[P.t_lit]=0.1;

/*TOR CWD*/
parmin[P.t_cwd]=0.00005;
parmax[P.t_cwd]=0.05;

/*TOR SOM*/
parmin[P.t_som]=0.0000001;
parmax[P.t_som]=0.01;

/*\Q10 = 1.2-2.0*/
parmin[P.Q10rhco2]=1;
parmax[P.Q10rhco2]=5.0;

/*LMCA*; Kattge et al. 2011*;*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*/
parmin[P.LCMA]=5;
parmax[P.LCMA]=200;

/*C labile*/
parmin[P.i_labile]=1.0;
parmax[P.i_labile]=100000.0;

/*C foliar*/
parmin[P.i_foliar]=1.0;
parmax[P.i_foliar]=2000.0;

/*C roots*/
parmin[P.i_root]=1.0;
parmax[P.i_root]=2000.0;

/*C_wood*/
parmin[P.i_wood]=1.0;
parmax[P.i_wood]=100000.0;

/*C CWD*/
parmin[P.i_cwd]=1.0;
parmax[P.i_cwd]=100000.0;

/*C litter*/
parmin[P.i_lit]=1.0;
parmax[P.i_lit]=2000.0;

/*C_som*/
parmin[P.i_som]=1.0;
parmax[P.i_som]=200000.0;

/*Retention parameter (b)*/
parmin[P.retention]=1.5;
parmax[P.retention]=10;

/*"LY1 SM at t0"*/
parmin[P.i_LY1_SM]=0.01;
parmax[P.i_LY1_SM]=1;

/*"LY2 SM at t0"*/
parmin[P.i_LY2_SM]=0.01;
parmax[P.i_LY2_SM]=1;

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

/*Saturated hydraulic conductivity (m/s)*/
parmin[P.hydr_cond]=1e-9;
parmax[P.hydr_cond]=1e-4;

/*Maximum infiltration (mm/day)*/
parmin[P.max_infil]=1;
parmax[P.max_infil]=100;

/*LY3 SM at t=0*/
parmin[P.i_LY3_SM]=0.01;
parmax[P.i_LY3_SM]=1;

/*LY1 porosity*/
parmin[P.LY1_por]=0.2;
parmax[P.LY1_por]=0.8;

/*LY2 porosity*/
parmin[P.LY2_por]=0.2;
parmax[P.LY2_por]=0.8;

/*LY3 porosity*/
parmin[P.LY3_por]=0.2;
parmax[P.LY3_por]=0.8;

/*Field capacity (negative) potential (-Mpa)*/
parmin[P.field_cap]=0.01;
parmax[P.field_cap]=0.1;
    
/*LY1 depth (m)*/
parmin[P.LY1_z]=0.01;
parmax[P.LY1_z]=1;

/*LY2 depth (m)*/
parmin[P.LY2_z]=0.01;
parmax[P.LY2_z]=20;

/*LY3 depth (m)*/
parmin[P.LY3_z]=0.01;
parmax[P.LY3_z]=100;

/*LY1 volumetric heat capacity (https://www.sciencedirect.com/topics/engineering/volumetric-heat-capacity)*/
parmin[P.LY1_vhc]=1.3e6;
parmax[P.LY1_vhc]=3e6;

/*LY2 volumetric heat capacity (https://www.sciencedirect.com/topics/engineering/volumetric-heat-capacity)*/
parmin[P.LY2_vhc]=1.3e6;
parmax[P.LY2_vhc]=3e6;

/*LY3 volumetric heat capacity (https://www.sciencedirect.com/topics/engineering/volumetric-heat-capacity)*/
parmin[P.LY3_vhc]=1.3e6;
parmax[P.LY3_vhc]=3e6;

/*Runoff excess*/
parmin[P.Q_excess]=0.01;
parmax[P.Q_excess]=1;

/*Medlyn g1*/
parmin[P.Med_g1]=1.79;
parmax[P.Med_g1]=5.79;

/*Vcmax25*/
parmin[P.Vcmax25]=1;
parmax[P.Vcmax25]=150;

/*Tminmin scaling factor*/
parmin[P.Tminmin]=258.15;
parmax[P.Tminmin]=273.15;

/*Tminmax scaling factor*/
parmin[P.Tminmax]=273.15;
parmax[P.Tminmax]=288.15;

/*aerodynamic conductance (m/s)*/
parmin[P.ga]=0.001;
parmax[P.ga]=10.0;

/*Tupp*/
parmin[P.Tupp]=249.15; // 299.15-50
parmax[P.Tupp]=318.15;

/*Tdown*/
parmin[P.Tdown]=213.15; // 263.15-50
parmax[P.Tdown]=286.15;

/*Clumping index*/
parmin[P.clumping]=0.35;
parmax[P.clumping]=1.0;

/*PAR reflectance*/
parmin[P.leaf_refl_par]=0.05;
parmax[P.leaf_refl_par]=0.5;

/*NIR reflectance*/
parmin[P.leaf_refl_nir]=0.3;
parmax[P.leaf_refl_nir]=0.7;

/*iSWE: initial for state variable SWE snow water equivalent*/
parmin[P.i_SWE]=0.000001;
parmax[P.i_SWE]=10000;

/*sn1: min threshold for melt*/
parmin[P.min_melt]=263.15;
parmax[P.min_melt]=283.15;

/*sn2: slope*/
parmin[P.melt_slope]=0.00001;
parmax[P.melt_slope]=1;

/*sn3: snow cover fraction scalar; SCF = SWE/(SWE +SWEcritical_par) */
parmin[P.scf_scalar]=10;
parmax[P.scf_scalar]=1000.0;

/* jc S_fv statistically fitting the fV curves (S1,S2,S3 schemes) with total soil moisture (LY1/LY1_fs)\n jc new name for this par is S_fv, scalar for aerobic volumetric fraction */
parmin[P.S_fv]=1;
parmax[P.S_fv]=100.0;

/* jc thetas_opt   optimum thetas for water scaler fW*/
parmin[P.thetas_opt]=0.2;
parmax[P.thetas_opt]=1.0;

/* jc fwc the water scaler fW value at the end point C  */
parmin[P.fwc]=0.01;
parmax[P.fwc]=1.0;

/* jc r_ch4   CH4:CO2 conversion ratio*/
parmin[P.r_ch4]=0.001;
parmax[P.r_ch4]=0.9;

/* jc Q10ch4 Q10 for CH4 production  */
parmin[P.Q10ch4]=1.0;
parmax[P.Q10ch4]=5.0;

/* maxPevap in mm/day*/
parmin[P.maxPevap]=0.01;
parmax[P.maxPevap]=100;

/*Mean temperature at leaf onset (T_phi) (degrees kelvin)*/
parmin[P.T_phi]=268.15;
parmax[P.T_phi]=323.15;

/*Spatial range of mean temperature at leaf onset (T_r) (degrees C or degrees kelvin)*/
parmin[P.T_range]=0.1;
parmax[P.T_range]=10.0;

/*LAI linear growth constant (inverse of model time units; e.g. days-1 or months-1)*/
parmin[P.plgr]=0.001;
parmax[P.plgr]=0.5;

/*Inverse of leaf longevity during senescence period (inverse of model time units; e.g. days-1 or months-1)*/
parmin[P.k_leaf]=0.001;
parmax[P.k_leaf]=0.5;

/*Intrinsic maximum LAI (m^2 m^-2)*/
parmin[P.lambda_max]=0.1;
parmax[P.lambda_max]=10.0;

/*Target survival time for LAI under water-deficit conditions (days; or same unit as ET and LY1)*/
parmin[P.tau_W]=0.1;
parmax[P.tau_W]=300;

/*Mean daylength at leaf shedding (t_c; in units of hours sunlight per day)*/
parmin[P.time_c]=2;
parmax[P.time_c]=22;

/*Spatial range of mean daylength at leaf shedding (t_r)*/
parmin[P.time_r]=0.1;
parmax[P.time_r]=6.0;

/*initialization of temperature memory at t=0 (in absolute temperature units)*/
parmin[P.init_T_mem]=268.14;
parmax[P.init_T_mem]=323.15;

/*initialization of water/structural memory (fractional value of intrinsic maximum LAI)*/
parmin[P.init_LAIW_mem]=0.01;
parmax[P.init_LAIW_mem]=1;

/*Inverse of leaf longevity at any period i.e. background turnover (days-1)*/
parmin[P.t_foliar]=0.001;
parmax[P.t_foliar]=0.1;

/*LY1 energy per mm H2O (~ Energy in 1mm H2O @ -50C to 1mm H2O@ +50C)*/
parmin[P.i_LY1_E]=4.67e+05;
parmax[P.i_LY1_E]=1.11e+06;

/*LY2 energy per mm H2O(~ Energy in 1mm H2O @ -50C to 1mm H2O@ +50C)*/
parmin[P.i_LY2_E]=4.67e+05;
parmax[P.i_LY2_E]=1.11e+06;

/*LY3 energy per mm H2O (~ Energy in 1mm H2O @ -50C to 1mm H2O@ +50C)*/
parmin[P.i_LY3_E]=4.67e+05;
parmax[P.i_LY3_E]=1.11e+06;

/*PSI 50: water potential when soil-plant continum is at 50% hydraulic conductivity (-MPa)*/
parmin[P.psi_50]=0.1;
parmax[P.psi_50]=30;

/*Beta function normalized logistic growth rate */
parmin[P.beta_lgr]=4.1;
parmax[P.beta_lgr]=50;
 
/*Ratio of carbon allocation to root per target foliar pool size (gC/gC)*/
parmin[P.phi_RL]=0.0001;
parmax[P.phi_RL]=5.;


/*Ratio of carbon allocation to wood per target foliar pool size (gC/gC)*/
parmin[P.phi_WL]=0.0001;
parmax[P.phi_WL]=50.;
  

/*Deep thermal conductivity in W/m/K */
parmin[P.thermal_cond]=0.3;
parmax[P.thermal_cond]=2;


/*Surface soil thermal conductivity in W/m/K */
parmin[P.thermal_cond_surf]=0.03;
parmax[P.thermal_cond_surf]=2;


/*Surface soil thermal conductivity in W/m/K */
parmin[P.q10canopy]=1;
parmax[P.q10canopy]=5;

    /*Surface soil thermal conductivity in W/m/K */
parmin[P.canopyRdsf]=0.005;
parmax[P.canopyRdsf]=0.025;

        /*Sublimation rate: mm/day/SCF/kPha/(MJ/m2/d) Biggest = 1/0.5/2/2; Smallest = 1/1/10/10/*/
parmin[P.sublimation_rate]=0.001;
parmax[P.sublimation_rate]=10;

/*Fraction of LY2 to LY1 root biomass*/
parmin[P.root_frac]=0.001;
parmax[P.root_frac]=1;
  
return 0;

}


