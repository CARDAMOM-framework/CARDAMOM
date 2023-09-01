#pragma once
/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/


/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/

struct DALEC_1100_PARAMETERS{
/*DALEC PARAMETERS*/
int i_SWE_H2O;
int i_SWE_E;
int SWEmax; //___scf_gen3
int Nmelt;  //___scf_gen3
int sublimation_rate;
int thermal_cond_swe;
int snow_sub_temp_exp;
} DALEC_1100_PARAMETERS={
     0, 1, 2, 3, 4, 5,6}; //___scf_gen3

struct DALEC_1100_FLUXES{
/*DALEC FLUXES*/
int snowfall;   /*Snowfall to SWE*/
int e_snowfall;   /*Snowfall to SWE*/
int sublimation;
int e_sublimation;
int melt;   /*Snow melt*/
int e_melt;   
int snow_heat;
} DALEC_1100_FLUXES={
     0, 1, 2,3,4,5,6};



/*Prognostic states and Diagnostic states (dependent on other states)*/


struct DALEC_1100_POOLS{
/*DALEC POOLS*/
int H2O_SWE; /*Snow water equivalent*/
int E_SWE; /*Snow water equivalent energy*/
int D_SCF;//snow-covered fraction
int D_TEMP_SWE; /*Snow water equivalent energy*/
} DALEC_1100_POOLS={
      0,1,2,3};

struct DALEC_1100_EDCs{
// int paw_start_temp;
// int puw_start_temp;
// int litcwdtor;
// int cwdsomtor;
// int mr_rates;
// int mean_paw_temp;
// int mean_puw_temp;
// int nsc_ratio;
//int state_trajectories;
int state_ranges;
int state_trajectories;
//int vcmax_lcma;
} DALEC_1100_EDCs={
   0,1};



