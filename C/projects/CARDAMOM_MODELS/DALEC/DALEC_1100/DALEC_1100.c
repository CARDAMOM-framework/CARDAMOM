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
    //POOLS[S.D_SCF]=POOLS[S.H2O_SWE]/(POOLS[S.H2O_SWE]+pars[P.scf_scalar]);} //snow cover fraction gen1
  //  double smr1=fmin(1.0,POOLS[S.H2O_SWE]/pars[P.SWEmax]); //___scf_gen3
  //  POOLS[S.D_SCF]=1-(1/pi*acos(2*smr1-1))**pars[P.Nmelt]; //___scf_gen3
      POOLS[S.D_SCF]=1-(1/pi*acos(2*fmin(1.0,POOLS[S.H2O_SWE]/pars[P.SWEmax])-1));} //___scf_gen3
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
//     //INITIALIZING PAW and PUW soil moisture
//         POOLS[S.D_SM_PAW]=HYDROFUN_EWT2MOI(POOLS[S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]); //soil moisture PAW
//         POOLS[S.D_SM_PUW]=HYDROFUN_EWT2MOI(POOLS[S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);//soil moisture PUW
// // Convert to potential
// 
//         POOLS[S.D_PSI_PAW]=HYDROFUN_MOI2PSI(  POOLS[S.D_SM_PAW],psi_porosity,pars[P.retention]);
//         POOLS[S.D_PSI_PUW]=HYDROFUN_MOI2PSI(  POOLS[S.D_SM_PUW],psi_porosity,pars[P.retention]);
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
//     
//     //******************Delcare KNORR STRUCT*********************
//     KNORR_ALLOCATION_STRUCT KNORR;
// //define time-invariant parameters
//          KNORR.IN.deltat=deltat;
//          KNORR.IN.n=0;
//          KNORR.IN.latitude=DATA.ncdf_data.LAT;
//          KNORR.IN.T_phi=pars[P.T_phi];
//          KNORR.IN.T_r=pars[P.T_range];
//          KNORR.IN.plgr=pars[P.plgr];
//          KNORR.IN.k_L=pars[P.k_leaf];
//          KNORR.IN.tau_W=pars[P.tau_W];//0.00000001;//
//          KNORR.IN.t_c=pars[P.time_c];
//          KNORR.IN.t_r=pars[P.time_r];;
//          KNORR.IN.lambda_max=pars[P.lambda_max];
//     //Initialize memory states
//     
//     POOLS[S.M_LAI_TEMP]=pars[P.init_T_mem];
//     POOLS[S.M_LAI_MAX]=pars[P.init_LAIW_mem]*pars[P.lambda_max];
//     
// 
//     //******************Allocation fluxes struct**********************
// //    
// //     typedef struct {    
// //     struct {
// //     double   TEMP;//deg C
// //     double   SRAD;//MJ m2 d
// //     double   NSC;//Clab
// //     double   PAW_SM;//m3/m3
// //     double   parameter1;//replace with any name, no constraints on naming convention
// //     double   parameter2;//replace with any name, no constraints on naming convention
// //     } IN;
// //     struct {
// //     double *    AUTO_RESP_MAINTENANCE;
// //       double *       AUTO_RESP_GROWTH;
// //       double *       ALLOC_FOL;
// //       double *       ALLOC_WOO;
// //      double *        ALLOC_ROO;}OUT;
// //   }ALLOC_AND_AUTO_RESP_FLUXES_STRUCT;
//     
//     
//     
//    //Declare
//     //Plant carbon allocation.
//      ALLOC_AND_AUTO_RESP_FLUXES_STRUCT ARFLUXES;
//      //define time-invariant parameters here
//         ARFLUXES.IN.mr_r=pars[P.rauto_mr_r];//
//         ARFLUXES.IN.mr_w=pars[P.rauto_mr_w];//
//         ARFLUXES.IN.gr=pars[P.rauto_gr];//
//         ARFLUXES.IN.Q10mr=pars[P.rauto_mr_q10];//
// 
// 
//         //Heterotrophic respiration module
//     HET_RESP_RATES_JCR_STRUCT HRJCR;
//     //define time invariant parameters here
//     
// /* jc prep input for methane module*/
//         HRJCR.IN.S_FV=pars[P.S_fv];
//         HRJCR.IN.SM_OPT=pars[P.thetas_opt];
//         HRJCR.IN.FWC=pars[P.fwc];
//         HRJCR.IN.R_CH4=pars[P.r_ch4];
//         HRJCR.IN.Q10CH4=pars[P.Q10ch4];
//         HRJCR.IN.Q10CO2=pars[P.Q10rhco2];
// 
//    
// 
// 
// 
// 
// /*Combustion factors*/
// double CF[7];//AAB changed this
// CF[S.C_lab]=pars[P.cf_ligneous];
// CF[S.C_fol]=pars[P.cf_foliar];
// CF[S.C_roo]=pars[P.cf_ligneous];
// CF[S.C_woo]=pars[P.cf_ligneous];
// CF[S.C_cwd]=pars[P.cf_ligneous];
// CF[S.C_lit]=pars[P.cf_foliar]/2+pars[P.cf_ligneous]/2;
// CF[S.C_som]=pars[P.cf_DOM];
// 
// 
// /*resilience factor*/
// 
// /*foliar carbon transfer intermediate variables*/
// double Fcfolavailable;
// 
// /*number of MET drivers*/
// // int nomet=((DALEC *)DATA.MODEL)->nomet;
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
// 
// double LAI=POOLS[p+S.D_LAI];
//      
//         
// /*Calculate light extinction coefficient*/
// double B = (DOY[n]-81)*2*pi/365.;
// double ET1 = 9.87*sin(2*B)-7.53*cos(B)-1.5*sin(B);
// double DA = 23.45*sin((284+DOY[n])*2*pi/365); //Deviation angle
// double LST = (int) (DOY[n]*24*60) % (24*60);
// LST=0.5*24*60;
// double AST = LST+ET1;
// double h = (AST-12*60)/4; //hour angle
// double alpha = asin((sin(pi/180*DATA.ncdf_data.LAT)*sin(pi/180*DA)+cos(pi/180*DATA.ncdf_data.LAT)*cos(pi/180.*DA)*cos(pi/180*h)))*180/pi; //solar altitude
// double zenith_angle = 90-alpha;
// 
// //printf("SZA local = %2.2f, SZA global = %2.2f, SZA diff = %2.2f\n", zenith_angle,DATA.ncdf_data.SZA.values,DATA.ncdf_data.SZA.values - zenith_angle);
// //double LAD = 1.0; //leaf angle distribution
// //double VegK = sqrt(pow(LAD,2)+ pow(tan(zenith_angle/180*pi),2))/(LAD+1.774*pow((1+1.182),-0.733)); //Campbell and Norman 1998
// 
// double LAD = 0.5; //leaf angle distribution// optimize leaf angle distribution. 
// double VegK = LAD/cos(zenith_angle/180*pi);
// 
// /*Temp scaling factor*/
// double g;
// double Tminmin = pars[P.Tminmin] - DGCM_TK0C; 
// double Tminmax = pars[P.Tminmax] - DGCM_TK0C;
// if( T2M_MIN[n] < Tminmin ) {
//     g=0;
// }
// else if (T2M_MIN[n] > Tminmax) {
//     g=1;
// }
// else {
//     g=(T2M_MIN[n] - Tminmin)/(Tminmax - Tminmin);
// }
// 
// // H2O stress scaling factor
// 	//We're also multiplying beta by cold-weather stress 
// //double psi_PAW0 = HYDROFUN_MOI2PSI(max(POOLS[p+S.D_SM_PAW],0),psi_porosity,pars[P.retention]);
// //double psi_PAW0 = HYDROFUN_MOI2PSI(POOLS[p+S.D_SM_PAW],psi_porosity,pars[P.retention_paw]);
// double beta = 1/(1 + exp(pars[P.beta_lgr]*(-1*POOLS[p+S.D_PSI_PAW]/pars[P.psi_50] - 1)));
// 
// // mean air temperature (K)
double air_temp_k = DGCM_TK0C+0.5*(T2M_MIN[n]+T2M_MAX[n]);
// 
// //******************Declare LIU STRUCT*********************
// LIU_AN_ET_STRUCT LIU;
// 
// //define time-invariant parameters
// LIU.IN.SRAD=SSRD[n]*1e6/DGCM_SEC_DAY;
// LIU.IN.VPD=VPD[n]/10;
// LIU.IN.TEMP=air_temp_k;  
// LIU.IN.vcmax25=pars[P.Vcmax25];
// LIU.IN.co2=CO2[n];
// LIU.IN.beta_factor=fmin(beta,g)*POOLS[p+S.D_LF_PAW];
// LIU.IN.g1=pars[P.Med_g1];
// LIU.IN.LAI=LAI;
// LIU.IN.ga=pars[P.ga];
// LIU.IN.VegK=VegK;
// LIU.IN.Tupp=pars[P.Tupp];
// LIU.IN.Tdown=pars[P.Tdown];
// LIU.IN.C3_frac=1., // pars[P.C3_frac]
// LIU.IN.clumping=pars[P.clumping];
// LIU.IN.leaf_refl_par=pars[P.leaf_refl_par];
// LIU.IN.leaf_refl_nir=pars[P.leaf_refl_nir];
// LIU.IN.maxPevap=pars[P.maxPevap];
// LIU.IN.precip=PREC[n];
// LIU.IN.q10canopy=pars[P.q10canopy];
// LIU.IN.q10canopyRd=pars[P.rauto_mrd_q10];
// LIU.IN.canopyRdsf=pars[P.canopyRdsf];
// LIU.IN.NSC=POOLS[p+S.C_lab];
// LIU.IN.deltat=deltat;
// 
// 
// //Call function: uses LIU->IN to update LIU->OUT
// LIU_AN_ET(&LIU);
// 
// double LEAF_MORTALITY_FACTOR=LIU.OUT.LEAF_MORTALITY_FACTOR;
// 
// // GPP--- gross
// FLUXES[f+F.gpp] = LIU.OUT.Ag;
// // GPP net, i.e. GPP- Rd
// FLUXES[f+F.gppnet] = LIU.OUT.An;
// //transpiration//
// FLUXES[f+F.transp] = LIU.OUT.transp;
// //evaporation//
// FLUXES[f+F.evap] = LIU.OUT.evap;


/*Snow water equivalent*/
FLUXES[f+F.snowfall] = SNOWFALL[n];

POOLS[nxp+S.H2O_SWE]=POOLS[p+S.H2O_SWE]+FLUXES[f+F.snowfall]*deltat; /*first step snowfall to SWE*/

POOLS[nxp+S.E_SWE]=POOLS[p+S.E_SWE]+FLUXES[f+F.snowfall]*INTERNAL_ENERGY_PER_H2O_UNIT_MASS(fmin(air_temp_k,0), 0)*deltat;
    
    
    /*first step snowfall to SWE*/
//transient_SCF
//double smr2=fmin(1.0,POOLS[nxp+S.H2O_SWE]/pars[P.SWEmax]); //___scf_gen3
double SCFtemp = 1-(1/pi*acos(2*fmin(1.0,POOLS[nxp+S.H2O_SWE]/pars[P.SWEmax])-1))**pars[P.Nmelt]; //___scf_gen3
//double SCFtemp = POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]); //___scf_gen1
    //Snow melt, based on new SWE
 double SNOWMELT=fmin(fmax((DGCM_TK0C+SKT[n]-pars[P.min_melt])*pars[P.melt_slope],0),1)*POOLS[nxp+S.H2O_SWE]*one_over_deltat; /*melted snow per day*/  
double SUBLIMATION =  pars[P.sublimation_rate]*SSRD[n]*SCFtemp;

double slf=(SNOWMELT + SUBLIMATION)*deltat/POOLS[nxp+S.H2O_SWE];
    if (slf>1){
        FLUXES[f+F.melt]=SNOWMELT/slf;
        FLUXES[f+F.sublimation]=SUBLIMATION/slf;}
        else{
                  FLUXES[f+F.melt]=SNOWMELT;
        FLUXES[f+F.sublimation]=SUBLIMATION;}



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

/*Insure SWE does not go negative due to machine error*/
POOLS[nxp+S.H2O_SWE]=fmax(POOLS[nxp+S.H2O_SWE]-(FLUXES[f+F.melt] + FLUXES[f+F.sublimation])*deltat,0); /*second step remove snowmelt from SWE*/

    double E_MELT = FLUXES[f+F.melt] * INTERNAL_ENERGY_PER_H2O_UNIT_MASS(DGCM_TK0C, 1);
    double E_SUBLIMATION = FLUXES[f+F.sublimation] * INTERNAL_ENERGY_PER_H2O_UNIT_MASS(DGCM_TK0C, 1);

    POOLS[nxp+S.E_SWE]=POOLS[nxp+S.E_SWE]-(E_MELT + E_SUBLIMATION )*deltat; /*second step remove snowmelt from SWE*/

//     /****************************RECORD t+1 DIAGNOSTIC STATES*************************/
//     POOLS[nxp+S.D_LAI]=POOLS[nxp+S.C_fol]/pars[P.LCMA]; //LAI

//___scf_gen3 start
//    POOLS[nxp+S.D_SCF]=POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]); //snow cover fraction gen1
//double smr3=fmin(1.0,POOLS[nxp+S.H2O_SWE]/pars[P.SWEmax]);
    POOLS[nxp+S.D_SCF]=1-(1/pi*acos(2*fmin(1.0,POOLS[nxp+S.H2O_SWE]/pars[P.SWEmax])-1))**pars[P.Nmelt]; //snow cover fraction gen3
//___scf_gen3 end  

    SWETEMP.IN.h2o = POOLS[nxp+S.H2O_SWE];//mm (or kg/m2)
    SWETEMP.IN.internal_energy = POOLS[nxp+S.E_SWE];//Joules
   H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);

   POOLS[nxp+S.D_TEMP_SWE]=SWETEMP.OUT.TEMP;//In K
// 
//     //Pass pointers to function 
// 
//     
//     //Soil moisture
// 
//         POOLS[nxp+S.D_SM_PAW]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]); //soil moisture PAW
//         POOLS[nxp+S.D_SM_PUW]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);//soil moisture PUW
// 
// 
//         POOLS[nxp+S.D_PSI_PAW]=HYDROFUN_MOI2PSI(  POOLS[nxp+S.D_SM_PAW],psi_porosity,pars[P.retention]);
//         POOLS[nxp+S.D_PSI_PUW]=HYDROFUN_MOI2PSI(  POOLS[nxp+S.D_SM_PUW],psi_porosity,pars[P.retention]);
// 

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
DALECmodel->nopars=6;
DALECmodel->nofluxes=3;
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
//   
//    EDC_sr.min_val[S.C_lab]=0;
//    EDC_sr.max_val[S.C_lab]=DALECmodel->PARS_INFO.parmax[P.i_labile];
//                 
//    EDC_sr.min_val[S.C_fol]=0;
//    EDC_sr.max_val[S.C_fol]=DALECmodel->PARS_INFO.parmax[P.i_foliar];
//                 
//    EDC_sr.min_val[S.C_roo]=0;
//    EDC_sr.max_val[S.C_roo]=DALECmodel->PARS_INFO.parmax[P.i_root];
//                 
//    EDC_sr.min_val[S.C_woo]=0;
//    EDC_sr.max_val[S.C_woo]=DALECmodel->PARS_INFO.parmax[P.i_wood];
//                 
//    EDC_sr.min_val[S.C_cwd]=0;
//    EDC_sr.max_val[S.C_cwd]=DALECmodel->PARS_INFO.parmax[P.i_cwd];
//                 
//    EDC_sr.min_val[S.C_lit]=0;
//    EDC_sr.max_val[S.C_lit]=DALECmodel->PARS_INFO.parmax[P.i_lit];
//                 
//    EDC_sr.min_val[S.C_som]=0;
//    EDC_sr.max_val[S.C_som]=DALECmodel->PARS_INFO.parmax[P.i_som];
                
//     EDC_sr.min_val[S.H2O_PAW]=0;
//    //
//                 
//    EDC_sr.min_val[S.H2O_PUW]=0;
//    //
                
    EDC_sr.min_val[S.H2O_SWE]=0;
    EDC_sr.max_val[S.H2O_SWE]=DALECmodel->PARS_INFO.parmax[P.i_SWE_H2O];

    EDC_sr.min_val[S.E_SWE]=0;
    EDC_sr.max_val[S.E_SWE]=DALECmodel->PARS_INFO.parmax[P.i_SWE_E];



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







