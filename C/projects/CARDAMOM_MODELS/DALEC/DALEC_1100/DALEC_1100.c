#pragma once
//Note: DALEC_OBSERVATION_OPERATORS.c included in DALEC_MODULE.
#include "DALEC_1100_INDICES.c"
#include "PARS_INFO_1100.c"
#include "../DALEC_ALL/DALEC_MODULE.c"//Includes all calls to DALEC-compatible EDC functions
#include "../DALEC_ALL/HYDROLOGY_MODULES/DRAINAGE.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_EWT2MOI.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2EWT.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2CON.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2PSI.c"
#include "../DALEC_ALL/LIU_AN_ET_REFACTOR.c"
#include "../DALEC_ALL/CH4_MODULES/HET_RESP_RATES_JCR.c"
#include "../DALEC_ALL/KNORR_ALLOCATION.c"
#include "../DALEC_ALL/SOIL_TEMP_AND_LIQUID_FRAC.c"
#include "../DALEC_ALL/H2O_TEMP_AND_LIQUID_FRAC.c"
#include "../DALEC_ALL/INITIALIZE_INTERNAL_SOIL_ENERGY.c"
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/ALLOC_AND_AUTO_RESP_FLUXES.c"


//Returns structure with sources and sinks, matches number of fluxes
int DALEC_1100_FLUX_SOURCES_SINKS(DALEC * DALECmodel){
    

    struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
    struct DALEC_1100_POOLS S=DALEC_1100_POOLS;

    // /Step 1. Declare & initialize 
 DALEC_FLUX_SOURCE_SINK_MATRIX   FIOMATRIX;
    // external source or pool sink, or not conserved quantity
    //Default = -1 
   
    FIOMATRIX.SOURCE=calloc(DALECmodel->nofluxes, sizeof(int));
    FIOMATRIX.SINK=calloc(DALECmodel->nofluxes, sizeof(int));
     
    
    int n;
    for (n=0;n<DALECmodel->nofluxes; n++){FIOMATRIX.SOURCE[n]=-1;FIOMATRIX.SINK[n]=-1;}
    
    
    //Step 2. Define
//     
// 
//         // C_lab
//         FIOMATRIX.SINK[F.gpp]=S.C_lab;
//         FIOMATRIX.SOURCE[F.resp_auto_maint]=S.C_lab;
//         FIOMATRIX.SOURCE[F.foliar_prod]=S.C_lab;
//         FIOMATRIX.SOURCE[F.root_prod]=S.C_lab;
//         FIOMATRIX.SOURCE[F.wood_prod]=S.C_lab;
//         FIOMATRIX.SOURCE[F.resp_auto_growth]=S.C_lab;
//         FIOMATRIX.SOURCE[F.f_lab]=S.C_lab;  
//         FIOMATRIX.SOURCE[F.fx_lab2lit]=S.C_lab;
// 
// 
//         // C_fol
//         FIOMATRIX.SINK[F.foliar_prod]=S.C_fol;
//         FIOMATRIX.SOURCE[F.fol2lit]=S.C_fol;
//         FIOMATRIX.SOURCE[F.f_fol]=S.C_fol;
//         FIOMATRIX.SOURCE[F.fx_fol2lit]=S.C_fol;
//         
//         // C_roo
//         FIOMATRIX.SINK[F.root_prod]=S.C_roo;
//         FIOMATRIX.SOURCE[F.roo2lit]=S.C_roo;
//         FIOMATRIX.SOURCE[F.f_roo]=S.C_roo;
//         FIOMATRIX.SOURCE[F.fx_roo2lit]=S.C_roo;
//         
//         // C_woo
//         FIOMATRIX.SINK[F.wood_prod]=S.C_woo;
//         FIOMATRIX.SOURCE[F.woo2cwd]=S.C_woo;
//         FIOMATRIX.SOURCE[F.f_woo]=S.C_woo;
//         FIOMATRIX.SOURCE[F.fx_woo2cwd]=S.C_woo;
// 
//         
//         // C_lit
//         FIOMATRIX.SINK[F.fx_lab2lit]=S.C_lit;
//         FIOMATRIX.SINK[F.fol2lit]=S.C_lit;
//         FIOMATRIX.SINK[F.fx_fol2lit]=S.C_lit;
//         FIOMATRIX.SINK[F.roo2lit]=S.C_lit;
//         FIOMATRIX.SINK[F.fx_roo2lit]=S.C_lit;
//         FIOMATRIX.SOURCE[F.ae_rh_lit]=S.C_lit;
//         FIOMATRIX.SOURCE[F.an_rh_lit]=S.C_lit;
//         FIOMATRIX.SOURCE[F.f_lit]=S.C_lit;
//         FIOMATRIX.SOURCE[F.lit2som]=S.C_lit;
//         FIOMATRIX.SOURCE[F.fx_lit2som]=S.C_lit;
// 
//         // C_cwd
//         FIOMATRIX.SINK[F.woo2cwd]=S.C_cwd;
//         FIOMATRIX.SINK[F.fx_woo2cwd]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.ae_rh_cwd]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.an_rh_cwd]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.f_cwd]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.cwd2som]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.fx_cwd2som]=S.C_cwd;
// 
//         // C_som
//         FIOMATRIX.SINK[F.cwd2som]=S.C_som;
//         FIOMATRIX.SINK[F.fx_cwd2som]=S.C_som;        
//         FIOMATRIX.SINK[F.lit2som]=S.C_som;
//         FIOMATRIX.SINK[F.fx_lit2som]=S.C_som;
//         FIOMATRIX.SOURCE[F.ae_rh_som]=S.C_som;
//         FIOMATRIX.SOURCE[F.an_rh_som]=S.C_som;
//         FIOMATRIX.SOURCE[F.f_som]=S.C_som;

        // H2O_SWE
        FIOMATRIX.SOURCE[F.melt]=S.H2O_SWE;
        FIOMATRIX.SOURCE[F.sublimation]=S.H2O_SWE;
        FIOMATRIX.SINK[F.snowfall]=S.H2O_SWE;
// 
//         // H2O_PAW
//         FIOMATRIX.SINK[F.infil]=S.H2O_PAW;
//         FIOMATRIX.SOURCE[F.et]=S.H2O_PAW;
//         FIOMATRIX.SOURCE[F.paw2puw]=S.H2O_PAW;
//         FIOMATRIX.SOURCE[F.q_paw]=S.H2O_PAW;
// 
//         // H2O_PUW        
//         FIOMATRIX.SOURCE[F.q_puw]=S.H2O_PUW;
//         FIOMATRIX.SINK[F.paw2puw]=S.H2O_PUW;
// 
//         // E_PAW
//         FIOMATRIX.SINK[F.gh_in]=S.E_PAW;
//         FIOMATRIX.SINK[F.infil_e]=S.E_PAW;
//         FIOMATRIX.SOURCE[F.et_e]=S.E_PAW;
//         FIOMATRIX.SOURCE[F.q_paw_e]=S.E_PAW;
//         FIOMATRIX.SOURCE[F.paw2puw_e]=S.E_PAW;
//         FIOMATRIX.SOURCE[F.paw2puw_th_e]=S.E_PAW;
// 
//         // E_PUW
//         FIOMATRIX.SINK[F.paw2puw_e]=S.E_PUW;
//         FIOMATRIX.SINK[F.geological]=S.E_PUW;
//         FIOMATRIX.SINK[F.paw2puw_th_e]=S.E_PUW;
//         FIOMATRIX.SOURCE[F.q_puw_e]=S.E_PUW;


        
DALECmodel->FIOMATRIX = FIOMATRIX;
            

//DALEC flux indices    

//Calculate State source sink matrix
DALEC_STATE_SOURCE_SINK_MATRIX_CONFIG(DALECmodel);
return 0;

}



    
    
    



int DALEC_1100(DATA DATA, double const *pars){

    



struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;


    
double *SSRD=DATA.ncdf_data.SSRD.values;
double *T2M_MIN=DATA.ncdf_data.T2M_MIN.values;
double *T2M_MAX=DATA.ncdf_data.T2M_MAX.values;
double *CO2=DATA.ncdf_data.CO2.values;
double *DOY=DATA.ncdf_data.DOY.values;
double *PREC=DATA.ncdf_data.TOTAL_PREC.values;
double *VPD=DATA.ncdf_data.VPD.values;
double *BURNED_AREA=DATA.ncdf_data.BURNED_AREA.values;
double *TIME_INDEX=DATA.ncdf_data.TIME_INDEX.values;
double *SNOWFALL=DATA.ncdf_data.SNOWFALL.values;
double *SKT=DATA.ncdf_data.SKT.values;
double *STRD=DATA.ncdf_data.STRD.values;

/*C-pools, fluxes, meteorology indices*/
int p=0,f,m,nxp, i;
int n=0,nn=0;
//double pi=3.1415927;
double pi=DGCM_PI;

//PREDERIVED TERMS 

    double PREDERIVED_GEO_FLUX=0.105*3600*24;


double deltat=DATA.ncdf_data.TIME_INDEX.values[1] - DATA.ncdf_data.TIME_INDEX.values[0];
double one_over_deltat=1/deltat;
 
int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;


/*Pointer transfer - all data stored in fluxes and pools will be passed to DATA*/
double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;

 


  /*assigning values to pools*/

//   /*L,F,R,W,Lit,SOM*/
//   POOLS[S.C_lab]=pars[P.i_labile];
//   POOLS[S.C_fol]=pars[P.i_foliar];
//   POOLS[S.C_roo]=pars[P.i_root];
//   POOLS[S.C_woo]=pars[P.i_wood];
//   POOLS[S.C_cwd]=pars[P.i_cwd];
//   POOLS[S.C_lit]=pars[P.i_lit];
//   POOLS[S.C_som]=pars[P.i_som];
//   /*water pools*/
//   POOLS[S.H2O_PAW]=HYDROFUN_MOI2EWT(pars[P.i_PAW_SM],pars[P.PAW_por],pars[P.PAW_z]);
//   POOLS[S.H2O_PUW]=HYDROFUN_MOI2EWT(pars[P.i_PUW_SM],pars[P.PUW_por],pars[P.PUW_z]);
  POOLS[S.H2O_SWE]=pars[P.i_SWE_H2O];
    //Assume SWE is at LST
  POOLS[S.E_SWE]=pars[P.i_SWE_H2O]*pars[P.i_SWE_E];
    
    //INTERNAL_ENERGY_PER_H2O_UNIT_MASS(SKT[0]+ DGCM_TK0C, 0);
  /*Energy pools*/
//   POOLS[S.E_PAW]=pars[P.i_PAW_E]*pars[P.PAW_z];
//   POOLS[S.E_PUW]=pars[P.i_PUW_E]*pars[P.PUW_z];
  
  
   //---INITIALIZING DIAGNOSTIC STATES---
//     POOLS[S.D_LAI]=POOLS[S.C_fol]/pars[P.LCMA]; //LAI
    
    if (POOLS[S.H2O_SWE]>0){
    POOLS[S.D_SCF]=POOLS[S.H2O_SWE]/(POOLS[S.H2O_SWE]+pars[P.scf_scalar]);} //snow cover fraction}
    else
    {POOLS[S.D_SCF]=0;};

// 
//         //Diagnostic time-invariant quantities
//         // Porosity scaling factor (see line 124 of HESS paper)
//         double psi_porosity = -0.117/100;
//         double PAWmax=pars[P.PAW_por]*pars[P.PAW_z]*1000; //PAW capacity in mm
//         double PUWmax=pars[P.PUW_por]*pars[P.PUW_z]*1000; //PUW capacity in mm
//         
//         

// 
// 
//         
//     //Declare stryct
     H2O_TEMP_AND_LIQUID_FRAC_STRUCT SWETEMP;
//   //Populate with run-specific constrants
//     //PAW
    SWETEMP.IN.h2o=POOLS[S.H2O_SWE]; ;//mm
     SWETEMP.IN.internal_energy = POOLS[S.E_SWE];//m 
//     PAWSOILTEMP.IN.soil_water = POOLS[S.H2O_PAW];//mm (or kg/m2)
//     PAWSOILTEMP.IN.internal_energy = POOLS[S.E_PAW];//Joules
//     //Pass pointer to function 
     H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);  //Outputs are in K
// 
//         //Store outputs 
     POOLS[S.D_TEMP_SWE]=SWETEMP.OUT.TEMP;  //In K  
//     POOLS[S.D_LF_PAW]=PAWSOILTEMP.OUT.LF;
// 
//     //PUW
//     PUWSOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.PUW_vhc]; ;//J/m3/K
//     PUWSOILTEMP.IN.depth = pars[P.PUW_z];//m 
//     PUWSOILTEMP.IN.soil_water = POOLS[S.H2O_PUW];//mm (or kg/m2)
//     PUWSOILTEMP.IN.internal_energy = POOLS[S.E_PUW];//Joules
//     //Pass pointer to function 
//     SOIL_TEMP_AND_LIQUID_FRAC(&PUWSOILTEMP);//Outputs are in K
//     //Store outputs 
//     POOLS[S.D_TEMP_PUW]=PUWSOILTEMP.OUT.TEMP;    //In K
//     POOLS[S.D_LF_PUW]=PUWSOILTEMP.OUT.LF;
// 
//     
//     
//     

// /*number of DALEC pools*/
int nopools=((DALEC *)DATA.MODEL)->nopools;
// 
// /*number of DALEC fluxes to store*/
int nofluxes=((DALEC *)DATA.MODEL)->nofluxes;


/*repeating loop for each timestep*/
for (n=0; n < N_timesteps; n++){
/*ppol index*/
p=nopools*n;
/*next pool index*/
nxp=nopools*(n+1);
/*met index*/
// m=nomet*n;
/*flux array index*/
f=nofluxes*n;
// 


    
    double air_temp_k = DGCM_TK0C+0.5*(T2M_MIN[n]+T2M_MAX[n]);


    double tskin_k = SKT[n]+DGCM_TK0C;


/*Snow water equivalent*/
FLUXES[f+F.snowfall] = SNOWFALL[n];

POOLS[nxp+S.H2O_SWE]=POOLS[p+S.H2O_SWE]+FLUXES[f+F.snowfall]*deltat; /*first step snowfall to SWE*/
    

FLUXES[f+F.e_snowfall]=FLUXES[f+F.snowfall]*INTERNAL_ENERGY_PER_H2O_UNIT_MASS(fmin(air_temp_k,DGCM_TK0C), 0)*deltat;
POOLS[nxp+S.E_SWE]=POOLS[p+S.E_SWE]+FLUXES[f+F.e_snowfall];


    /*first step snowfall to SWE*/
//transient_SCF
    double SCFtemp = POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]);

// //Gh_in approach 2 based on soil and LST
// FLUXES[f+F.ground_heat] =(pars[P.thermal_cond_surf]* (tskin_k - POOLS[p+S.D_TEMP_LY1])/(pars[P.LY1_z]*0.5))*(1. - POOLS[p+S.D_SCF]);
// FLUXES[f+F.gh_in] =FLUXES[f+F.ground_heat] *DGCM_SEC_DAY;    

FLUXES[f+F.snow_heat] = SCFtemp*(pars[P.thermal_cond_swe]* (tskin_k - POOLS[p+S.D_TEMP_SWE])/(POOLS[nxp+S.H2O_SWE]*1e-3*0.5))*DGCM_SEC_DAY;

POOLS[nxp+S.E_SWE]+=FLUXES[f+F.snow_heat]*deltat;
    
    

    //Snow melt, based on new SWE
 //double SNOWMELT=fmin(fmax((DGCM_TK0C+SKT[n]-pars[P.min_melt])*pars[P.melt_slope],0),1)*POOLS[nxp+S.H2O_SWE]*one_over_deltat; /*melted snow per day*/  

//*Remove snow melt first 
    //Calculate liquid fraction
       SWETEMP.IN.h2o=POOLS[nxp+S.H2O_SWE]; ;//mm
     SWETEMP.IN.internal_energy = POOLS[nxp+S.E_SWE];//m 
//     PAWSOILTEMP.IN.soil_water = POOLS[S.H2O_PAW];//mm (or kg/m2)
//     PAWSOILTEMP.IN.internal_energy = POOLS[S.E_PAW];//Joules
//     //Pass pointer to function 
     H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);  //Outputs are in K

//double SNOWMELT 
     FLUXES[f+F.melt] = POOLS[nxp+S.H2O_SWE]*SWETEMP.OUT.LF/deltat;

//Remove sublimation next
    double SUBLIMATIONtemp =  pars[P.sublimation_rate]*SSRD[n]*SCFtemp;

    if (POOLS[nxp+S.H2O_SWE] - deltat*(   FLUXES[f+F.melt] + SUBLIMATIONtemp)<0){
        FLUXES[f+F.sublimation]=POOLS[nxp+S.H2O_SWE]/deltat -    FLUXES[f+F.melt];
        POOLS[nxp+S.H2O_SWE]=0;}
        else{
        FLUXES[f+F.sublimation]=SUBLIMATIONtemp;
        POOLS[nxp+S.H2O_SWE]=POOLS[nxp+S.H2O_SWE]-(FLUXES[f+F.melt] + FLUXES[f+F.sublimation])*deltat; }



// 
//  //Splitting snow loss into sublimation and snow melt with constant frac
// FLUXES[f+F.melt]=SNOWMELT;
//     //    POOLS[nxp+S.D_SCF]=POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]); //snow cover fraction
// 
// FLUXES[f+F.sublimation]=fmax( pars[P.sublimation_rate]*VPD[n]*SRAD[n]*POOLS[nxp+S.D_SCF]);
// 

// 
// 
// // Evapotranspiration
// FLUXES[f+F.et]=FLUXES[f+F.evap]+FLUXES[f+F.transp];
// FLUXES[f+F.ets]=FLUXES[f+F.et] + FLUXES[f+F.sublimation];

/*second step remove snowmelt from SWE*/


//Sublimation first 
    if (      POOLS[nxp+S.H2O_SWE]>0){
FLUXES[f+F.e_sublimation] = FLUXES[f+F.sublimation] * INTERNAL_ENERGY_PER_H2O_UNIT_MASS(DGCM_TK0C, 1);
/*Ensure SWE does not go negative due to machine error*/
//POOLS[nxp+S.H2O_SWE]=fmax(POOLS[nxp+S.H2O_SWE]-(FLUXES[f+F.melt] + FLUXES[f+F.sublimation])*deltat,0); /*second step remove snowmelt from SWE*/
FLUXES[f+F.e_melt] = FLUXES[f+F.melt] * INTERNAL_ENERGY_PER_H2O_UNIT_MASS(DGCM_TK0C, 1);


    POOLS[nxp+S.E_SWE]=POOLS[nxp+S.E_SWE]-(FLUXES[f+F.e_melt]+ FLUXES[f+F.e_sublimation]  )*deltat; }/*second step remove snowmelt from SWE*/

    else
    { POOLS[nxp+S.E_SWE]=0;
    FLUXES[f+F.e_melt] =0;
    FLUXES[f+F.e_sublimation] =0;}

//     /****************************RECORD t+1 DIAGNOSTIC STATES*************************/
//     POOLS[nxp+S.D_LAI]=POOLS[nxp+S.C_fol]/pars[P.LCMA]; //LAI
    POOLS[nxp+S.D_SCF]=POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]); //snow cover fraction
    
   
    SWETEMP.IN.h2o = POOLS[nxp+S.H2O_SWE];//mm (or kg/m2)
    SWETEMP.IN.internal_energy = POOLS[nxp+S.E_SWE];//Joules
   H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);

   POOLS[nxp+S.D_TEMP_SWE]=SWETEMP.OUT.TEMP;//In K



    
}


return 0;
}


int DALEC_1100_MODCONFIG(DALEC * DALECmodel, DATA * DATA){


struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;
struct DALEC_1100_EDCs E=DALEC_1100_EDCs;




DALECmodel->dalec=DALEC_1100;
DALECmodel->nopools=4;
DALECmodel->nopars=7;
DALECmodel->nofluxes=7;
DALECmodel->noedcs=2;


DALEC_1100_FLUX_SOURCES_SINKS(DALECmodel);




//Define PARS_INFO here (ranges, and eventually names, etc)
PARS_INFO_1100(DALECmodel);

//EDC operator
//Make sure has same number as number of EDCs
//This is generic EDCs structure defined in ../DALEC_EDCs/DALEC_EDC_FUNCTIONS.c
//Has three args, data (void), function (in "DATA", and "void *", and "double" out), and "boolean" prerun.



// 
// 
 DALECmodel->EDCs=calloc(DALECmodel->noedcs,sizeof(EDCs));
// //Abbrieviate
EDCs * EDCs=DALECmodel->EDCs;
// 
// //Som lit turnover rate
// 
// //EDC: litter tor > cwd tor
// //List all inequality calls here
// static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_litcwdtor; 
// static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_cwdsomtor;
// static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_mr_rates;
// 
// EDC_litcwdtor.big_par_index=P.t_lit;
// EDC_litcwdtor.small_par_index=P.t_cwd;
// EDCs[E.litcwdtor].data=&EDC_litcwdtor;
// EDCs[E.litcwdtor].function=&DALEC_EDC_PARAMETER_INEQUALITY;
// EDCs[E.litcwdtor].prerun=true;
// 
// //EDC: cwd tor > som tor
// EDC_cwdsomtor.big_par_index=P.t_cwd;
// EDC_cwdsomtor.small_par_index=P.t_som;
// EDCs[E.cwdsomtor].data=&EDC_cwdsomtor;
// EDCs[E.cwdsomtor].function=&DALEC_EDC_PARAMETER_INEQUALITY;
// EDCs[E.cwdsomtor].prerun=true;
// 
// //EDC: foliar and root mr > wood mr
// EDC_mr_rates.big_par_index=P.rauto_mr_r;
// EDC_mr_rates.small_par_index=P.rauto_mr_w;
// EDCs[E.mr_rates].data=&EDC_mr_rates;
// EDCs[E.mr_rates].function=&DALEC_EDC_PARAMETER_INEQUALITY;
// EDCs[E.mr_rates].prerun=true;
// 
// 


//State ranges
   static DALEC_EDC_STATE_RANGES_STRUCT EDC_sr;

   //Create array (1x number of pools) for minimum and max values 
   //Index exactly corresponds to "POOLS" indices

   EDC_sr.min_val=calloc(DALECmodel->nopools,sizeof(double));

   EDC_sr.max_val=calloc(DALECmodel->nopools,sizeof(double));
   
   int n;
   for (n=0;n<DALECmodel->nopools;n++){
   EDC_sr.min_val[n]=-INFINITY;
              EDC_sr.max_val[n]=INFINITY;}

//    //  
    EDC_sr.min_val[S.H2O_SWE]=0;
    EDC_sr.max_val[S.H2O_SWE]=DALECmodel->PARS_INFO.parmax[P.i_SWE_H2O];

    //EDC_sr.min_val[S.E_SWE]=0;



//Adding EDC to the EDCs list


    EDCs[E.state_ranges].data=&EDC_sr;
    EDCs[E.state_ranges].function=&DALEC_EDC_STATE_RANGES;
    EDCs[E.state_ranges].prerun=false;







 static DALEC_EDC_TRAJECTORY_STRUCT EDC_st;
 
 static int edc_pool_indices[1];
    static double edc_pool_eqf[1];
EDC_st.pool_indices=edc_pool_indices;
EDC_st.pool_eqf=edc_pool_eqf;
EDC_st.no_pools_to_check=1;

EDC_st.pool_indices[0]=S.H2O_SWE;




for (n=0;n<EDC_st.no_pools_to_check;n++){EDC_st.pool_eqf[n]=DATA->ncdf_data.EDC_EQF;printf("DATA->ncdf_data.EDC_EQF = %2.2f\n",DATA->ncdf_data.EDC_EQF);}

    
EDCs[E.state_trajectories].data=&EDC_st;
EDCs[E.state_trajectories].function=&DALEC_EDC_TRAJECTORY;
EDCs[E.state_trajectories].prerun=false;



//declaring observation operator structure, and filling with DALEC configurations
static OBSOPE OBSOPE;
//Initialize all SUPPORT OBS values (default value = false).
INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);

OBSOPE.SUPPORT_SCF_OBS=true;

OBSOPE.SCF_pool=S.D_SCF;


DALECmodel->OBSOPE=OBSOPE;



return 0;}







