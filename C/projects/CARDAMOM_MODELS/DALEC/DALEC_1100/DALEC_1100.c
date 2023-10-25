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
#include "../DALEC_ALL/INITIALIZE_INTERNAL_SOIL_ENERGY.c"
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/ALLOC_AND_AUTO_RESP_FLUXES.c"



typedef struct DALEC_1100_DATA_STRUCT{
double * VegK;
double example_const;
}DALEC_1100_DATA_STRUCT;


int PREDERIVE_DALEC_1100_DATA(DALEC * DALECmodel, DATA * DATA){

    //Step 1. Define function here
static DALEC_1100_DATA_STRUCT DALEC_1100_DATA;
//Step 2. Populate with any datasets that will be used repeatedly.


    //******VegK calculcation********





    //

    double * DOY=DATA->ncdf_data.DOY.values;
    double LAT = DATA->ncdf_data.LAT;
    int N_timesteps=DATA->ncdf_data.TIME_INDEX.length;
    double pi=DGCM_PI;

//Declare VegK
    double * VegK = calloc(N_timesteps, sizeof(double));
    

int n;

for (n=0; n < N_timesteps; n++){
 
/*Calculate light extinction coefficient for each timestep*/
double B = (DOY[n]-81)*2*pi/365.;
double ET1 = 9.87*sin(2*B)-7.53*cos(B)-1.5*sin(B);
double DA = 23.45*sin((284+DOY[n])*2*pi/365); //Deviation angle
double LST = (int) (DOY[n]*24*60) % (24*60);
LST=0.5*24*60;
double AST = LST+ET1;
double h = (AST-12*60)/4; //hour angle
double alpha = asin((sin(pi/180*LAT)*sin(pi/180*DA)+cos(pi/180*LAT)*cos(pi/180.*DA)*cos(pi/180*h)))*180/pi; //solar altitude
double zenith_angle = 90-alpha;

//printf("SZA local = %2.2f, SZA global = %2.2f, SZA diff = %2.2f\n", zenith_angle,DATA.ncdf_data.SZA.values,DATA.ncdf_data.SZA.values - zenith_angle);
//double LAD = 1.0; //leaf angle distribution
//double VegK = sqrt(pow(LAD,2)+ pow(tan(zenith_angle/180*pi),2))/(LAD+1.774*pow((1+1.182),-0.733)); //Campbell and Norman 1998

double LAD = 0.5; //leaf angle distribution// optimize leaf angle distribution. 

    
    VegK[n] = LAD/cos(zenith_angle/180*pi);
    
    


}

  
    
//Store data in  DALEC_1100_DATA
    DALEC_1100_DATA.VegK=VegK;


    //Store for later use
    DALECmodel->MODEL_DATA=&DALEC_1100_DATA;

return 0;
}


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
    

        // C_lab
        FIOMATRIX.SINK[F.gpp]=S.C_lab;
        FIOMATRIX.SOURCE[F.resp_auto_maint]=S.C_lab;
        FIOMATRIX.SOURCE[F.foliar_prod]=S.C_lab;
        FIOMATRIX.SOURCE[F.root_prod]=S.C_lab;
        FIOMATRIX.SOURCE[F.wood_prod]=S.C_lab;
        FIOMATRIX.SOURCE[F.resp_auto_growth]=S.C_lab;
        FIOMATRIX.SOURCE[F.f_lab]=S.C_lab;  
        FIOMATRIX.SOURCE[F.lab2lit]=S.C_lab;
        FIOMATRIX.SOURCE[F.fx_lab2lit]=S.C_lab;
        FIOMATRIX.SOURCE[F.dist_lab]=S.C_lab;


        // C_fol
        FIOMATRIX.SINK[F.foliar_prod]=S.C_fol;
        FIOMATRIX.SOURCE[F.fol2lit]=S.C_fol;
        FIOMATRIX.SOURCE[F.ph_fol2lit]=S.C_fol;
        FIOMATRIX.SOURCE[F.f_fol]=S.C_fol;
        FIOMATRIX.SOURCE[F.fx_fol2lit]=S.C_fol;
        FIOMATRIX.SOURCE[F.dist_fol]=S.C_fol;
        
        // C_roo
        FIOMATRIX.SINK[F.root_prod]=S.C_roo;
        FIOMATRIX.SOURCE[F.roo2lit]=S.C_roo;
        FIOMATRIX.SOURCE[F.f_roo]=S.C_roo;
        FIOMATRIX.SOURCE[F.fx_roo2lit]=S.C_roo;
        FIOMATRIX.SOURCE[F.dist_roo]=S.C_roo;
        
        // C_woo
        FIOMATRIX.SINK[F.wood_prod]=S.C_woo;
        FIOMATRIX.SOURCE[F.woo2cwd]=S.C_woo;
        FIOMATRIX.SOURCE[F.f_woo]=S.C_woo;
        FIOMATRIX.SOURCE[F.fx_woo2cwd]=S.C_woo;
        FIOMATRIX.SOURCE[F.dist_woo]=S.C_woo;

        
        // C_lit
        FIOMATRIX.SINK[F.lab2lit]=S.C_lit;
        FIOMATRIX.SINK[F.fx_lab2lit]=S.C_lit;
        FIOMATRIX.SINK[F.ph_fol2lit]=S.C_lit;
        FIOMATRIX.SINK[F.fol2lit]=S.C_lit;
        FIOMATRIX.SINK[F.fx_fol2lit]=S.C_lit;
        FIOMATRIX.SINK[F.roo2lit]=S.C_lit;
        FIOMATRIX.SINK[F.fx_roo2lit]=S.C_lit;
        FIOMATRIX.SOURCE[F.ae_rh_lit]=S.C_lit;
        FIOMATRIX.SOURCE[F.an_rh_lit]=S.C_lit;
        FIOMATRIX.SOURCE[F.f_lit]=S.C_lit;
        FIOMATRIX.SOURCE[F.lit2som]=S.C_lit;
        FIOMATRIX.SOURCE[F.fx_lit2som]=S.C_lit;

        // C_cwd
        FIOMATRIX.SINK[F.woo2cwd]=S.C_cwd;
        FIOMATRIX.SINK[F.fx_woo2cwd]=S.C_cwd;
        FIOMATRIX.SOURCE[F.ae_rh_cwd]=S.C_cwd;
        FIOMATRIX.SOURCE[F.an_rh_cwd]=S.C_cwd;
        FIOMATRIX.SOURCE[F.f_cwd]=S.C_cwd;
        FIOMATRIX.SOURCE[F.cwd2som]=S.C_cwd;
        FIOMATRIX.SOURCE[F.fx_cwd2som]=S.C_cwd;

        // C_som
        FIOMATRIX.SINK[F.cwd2som]=S.C_som;
        FIOMATRIX.SINK[F.fx_cwd2som]=S.C_som;        
        FIOMATRIX.SINK[F.lit2som]=S.C_som;
        FIOMATRIX.SINK[F.fx_lit2som]=S.C_som;
        FIOMATRIX.SOURCE[F.ae_rh_som]=S.C_som;
        FIOMATRIX.SOURCE[F.an_rh_som]=S.C_som;
        FIOMATRIX.SOURCE[F.f_som]=S.C_som;

        // H2O_SWE
        FIOMATRIX.SOURCE[F.melt]=S.H2O_SWE;
        FIOMATRIX.SOURCE[F.sublimation]=S.H2O_SWE;
        FIOMATRIX.SINK[F.snowfall]=S.H2O_SWE;

        // H2O_LY1
        FIOMATRIX.SINK[F.infil]=S.H2O_LY1;
        FIOMATRIX.SOURCE[F.evap]=S.H2O_LY1;
        FIOMATRIX.SOURCE[F.transp1]=S.H2O_LY1;
        FIOMATRIX.SOURCE[F.ly1xly2]=S.H2O_LY1;
        FIOMATRIX.SOURCE[F.q_ly1]=S.H2O_LY1;

        // H2O_LY2
        FIOMATRIX.SINK[F.ly1xly2]=S.H2O_LY2;
        FIOMATRIX.SOURCE[F.transp2]=S.H2O_LY2;
        FIOMATRIX.SOURCE[F.ly2xly3]=S.H2O_LY2;
        FIOMATRIX.SOURCE[F.q_ly2]=S.H2O_LY2;

        // H2O_LY3        
        FIOMATRIX.SOURCE[F.q_ly3]=S.H2O_LY3;
        FIOMATRIX.SINK[F.ly2xly3]=S.H2O_LY3;

        // E_LY1
        FIOMATRIX.SINK[F.gh_in]=S.E_LY1;
        FIOMATRIX.SINK[F.infil_e]=S.E_LY1;
        FIOMATRIX.SOURCE[F.evap_e]=S.E_LY1;
        FIOMATRIX.SOURCE[F.transp1_e]=S.E_LY1;
        FIOMATRIX.SOURCE[F.q_ly1_e]=S.E_LY1;
        FIOMATRIX.SOURCE[F.ly1xly2_e]=S.E_LY1;
        FIOMATRIX.SOURCE[F.ly1xly2_th_e]=S.E_LY1;

        // E_LY2
        FIOMATRIX.SINK[F.ly1xly2_e]=S.E_LY2;
        FIOMATRIX.SINK[F.ly1xly2_th_e]=S.E_LY2;
        FIOMATRIX.SOURCE[F.transp2_e]=S.E_LY2;
        FIOMATRIX.SOURCE[F.q_ly2_e]=S.E_LY2;
        FIOMATRIX.SOURCE[F.ly2xly3_e]=S.E_LY2;
        FIOMATRIX.SOURCE[F.ly2xly3_th_e]=S.E_LY2;

        // E_LY3
        FIOMATRIX.SINK[F.ly2xly3_e]=S.E_LY3;
        FIOMATRIX.SINK[F.geological]=S.E_LY3;
        FIOMATRIX.SINK[F.ly2xly3_th_e]=S.E_LY3;
        FIOMATRIX.SOURCE[F.q_ly3_e]=S.E_LY3;


        
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
double *DIST=DATA.ncdf_data.DISTURBANCE_FLUX.values;

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

  /*L,F,R,W,Lit,SOM*/
  POOLS[S.C_lab]=pars[P.i_labile];
  POOLS[S.C_fol]=pars[P.i_foliar];
  POOLS[S.C_roo]=pars[P.i_root];
  POOLS[S.C_woo]=pars[P.i_wood];
  POOLS[S.C_cwd]=pars[P.i_cwd];
  POOLS[S.C_lit]=pars[P.i_lit];
  POOLS[S.C_som]=pars[P.i_som];
  /*water pools*/
  POOLS[S.H2O_LY1]=HYDROFUN_MOI2EWT(pars[P.i_LY1_SM],pars[P.LY1_por],pars[P.LY1_z]);
  POOLS[S.H2O_LY2]=HYDROFUN_MOI2EWT(pars[P.i_LY2_SM],pars[P.LY2_por],pars[P.LY2_z]);
  POOLS[S.H2O_LY3]=HYDROFUN_MOI2EWT(pars[P.i_LY3_SM],pars[P.LY3_por],pars[P.LY3_z]);
  POOLS[S.H2O_SWE]=pars[P.i_SWE];
  /*Energy pools*/
  //Step 1. derive temperature based on i_LY1_E (which will represent energy per mm H2O).
    //INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(infiltemp);
    //Declare stryct
//     SOIL_TEMP_AND_LIQUID_FRAC_STRUCT LY1SOILTEMP, LY2SOILTEMP, LY3SOILTEMP;
//   //Populate with run-specific constrants
//     //LY1
//     LY1SOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.LY1_vhc]; ;//J/m3/K
//     LY1SOILTEMP.IN.depth = pars[P.LY1_z];//m 
//     LY1SOILTEMP.IN.soil_water = POOLS[S.H2O_LY1];//mm (or kg/m2)
//     LY1SOILTEMP.IN.internal_energy = POOLS[S.E_LY1];//Joules
//     //Pass pointer to function 
//     SOIL_TEMP_AND_LIQUID_FRAC(&LY1SOILTEMP);  //Outputs are in K
    //


    
    //double INITIALIZE_INTERNAL_SOIL_ENERGY(double internal_energy_per_mm_H2O, double H2O_mm, double dry_soil_vol_heat_capacity, double depth )
  POOLS[S.E_LY1]=INITIALIZE_INTERNAL_SOIL_ENERGY(pars[P.i_LY1_E],   POOLS[S.H2O_LY1], pars[P.LY1_vhc], pars[P.LY1_z] );
  POOLS[S.E_LY2]=INITIALIZE_INTERNAL_SOIL_ENERGY(pars[P.i_LY2_E],   POOLS[S.H2O_LY2], pars[P.LY2_vhc], pars[P.LY2_z] );
  POOLS[S.E_LY3]=INITIALIZE_INTERNAL_SOIL_ENERGY(pars[P.i_LY3_E],   POOLS[S.H2O_LY3], pars[P.LY3_vhc], pars[P.LY3_z] );
  
  
   //---INITIALIZING DIAGNOSTIC STATES---
    POOLS[S.D_LAI]=POOLS[S.C_fol]/pars[P.LCMA]; //LAI
    
    if (POOLS[S.H2O_SWE]>0){
    POOLS[S.D_SCF]=POOLS[S.H2O_SWE]/(POOLS[S.H2O_SWE]+pars[P.scf_scalar]);} //snow cover fraction}
    else
    {POOLS[S.D_SCF]=0;};


        //Diagnostic time-invariant quantities
        // Porosity scaling factor (see line 124 of HESS paper)
        double psi_porosity = -0.117/100;
        double LY1max=pars[P.LY1_por]*pars[P.LY1_z]*1000; //LY1 capacity in mm
        double LY2max=pars[P.LY2_por]*pars[P.LY2_z]*1000; //LY3 capacity in mm
        double LY3max=pars[P.LY3_por]*pars[P.LY3_z]*1000; //LY3 capacity in mm
        
        
    
    //INITIALIZING soil moisture
        POOLS[S.D_SM_LY1]=HYDROFUN_EWT2MOI(POOLS[S.H2O_LY1],pars[P.LY1_por],pars[P.LY1_z]); //soil moisture LY1
        POOLS[S.D_SM_LY2]=HYDROFUN_EWT2MOI(POOLS[S.H2O_LY2],pars[P.LY2_por],pars[P.LY2_z]);//soil moisture LY3
        POOLS[S.D_SM_LY3]=HYDROFUN_EWT2MOI(POOLS[S.H2O_LY3],pars[P.LY3_por],pars[P.LY3_z]);//soil moisture LY3
// Convert to potential
        double min_psi=-30; // User set limit of physical soil dryness 
        POOLS[S.D_PSI_LY1]=fmax(min_psi,HYDROFUN_MOI2PSI(  POOLS[S.D_SM_LY1],psi_porosity,pars[P.retention]));
        POOLS[S.D_PSI_LY2]=fmax(min_psi,HYDROFUN_MOI2PSI(  POOLS[S.D_SM_LY2],psi_porosity,pars[P.retention]));
        POOLS[S.D_PSI_LY3]=fmax(min_psi,HYDROFUN_MOI2PSI(  POOLS[S.D_SM_LY3],psi_porosity,pars[P.retention]));



        
    //Declare struct
    SOIL_TEMP_AND_LIQUID_FRAC_STRUCT LY1SOILTEMP, LY2SOILTEMP, LY3SOILTEMP;
  //Populate with run-specific constrants
    //LY1
    LY1SOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.LY1_vhc]; ;//J/m3/K
    LY1SOILTEMP.IN.depth = pars[P.LY1_z];//m 
    LY1SOILTEMP.IN.soil_water = POOLS[S.H2O_LY1];//mm (or kg/m2)
    LY1SOILTEMP.IN.internal_energy = POOLS[S.E_LY1];//Joules
    //Pass pointer to function 
    SOIL_TEMP_AND_LIQUID_FRAC(&LY1SOILTEMP);  //Outputs are in K
    //Store outputs 
    POOLS[S.D_TEMP_LY1]=LY1SOILTEMP.OUT.TEMP;  //In K  
    POOLS[S.D_LF_LY1]=LY1SOILTEMP.OUT.LF;

    //LY2
    LY2SOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.LY2_vhc]; ;//J/m3/K
    LY2SOILTEMP.IN.depth = pars[P.LY2_z];//m 
    LY2SOILTEMP.IN.soil_water = POOLS[S.H2O_LY2];//mm (or kg/m2)
    LY2SOILTEMP.IN.internal_energy = POOLS[S.E_LY2];//Joules
    //Pass pointer to function 
    SOIL_TEMP_AND_LIQUID_FRAC(&LY2SOILTEMP);  //Outputs are in K
    //Store outputs 
    POOLS[S.D_TEMP_LY2]=LY2SOILTEMP.OUT.TEMP;  //In K  
    POOLS[S.D_LF_LY2]=LY2SOILTEMP.OUT.LF;

    //LY3
    LY3SOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.LY3_vhc]; ;//J/m3/K
    LY3SOILTEMP.IN.depth = pars[P.LY3_z];//m 
    LY3SOILTEMP.IN.soil_water = POOLS[S.H2O_LY3];//mm (or kg/m2)
    LY3SOILTEMP.IN.internal_energy = POOLS[S.E_LY3];//Joules
    //Pass pointer to function 
    SOIL_TEMP_AND_LIQUID_FRAC(&LY3SOILTEMP);//Outputs are in K
    //Store outputs 
    POOLS[S.D_TEMP_LY3]=LY3SOILTEMP.OUT.TEMP;    //In K
    POOLS[S.D_LF_LY3]=LY3SOILTEMP.OUT.LF;

    
    
    
    
    //******************Declare KNORR STRUCT*********************
    KNORR_ALLOCATION_STRUCT KNORR;
//define time-invariant parameters
         KNORR.IN.deltat=deltat;
         KNORR.IN.n=0;
         KNORR.IN.latitude=DATA.ncdf_data.LAT;
         KNORR.IN.T_phi=pars[P.T_phi];
         KNORR.IN.T_r=pars[P.T_range];
         KNORR.IN.plgr=pars[P.plgr];
         KNORR.IN.k_L=pars[P.k_leaf];
         KNORR.IN.tau_W=pars[P.tau_W];//0.00000001;//
         KNORR.IN.t_c=pars[P.time_c];
         KNORR.IN.t_r=pars[P.time_r];;
         KNORR.IN.lambda_max=pars[P.lambda_max];
    //Initialize memory states
    
    POOLS[S.M_LAI_TEMP]=pars[P.init_T_mem];
    POOLS[S.M_LAI_MAX]=pars[P.init_LAIW_mem]*pars[P.lambda_max];
    

    //******************Allocation fluxes struct**********************
//    
//     typedef struct {    
//     struct {
//     double   TEMP;//deg C
//     double   SRAD;//MJ m2 d
//     double   NSC;//Clab
//     double   LY1_SM;//m3/m3
//     double   parameter1;//replace with any name, no constraints on naming convention
//     double   parameter2;//replace with any name, no constraints on naming convention
//     } IN;
//     struct {
//     double *    AUTO_RESP_MAINTENANCE;
//       double *       AUTO_RESP_GROWTH;
//       double *       ALLOC_FOL;
//       double *       ALLOC_WOO;
//      double *        ALLOC_ROO;}OUT;
//   }ALLOC_AND_AUTO_RESP_FLUXES_STRUCT;
    
    
    
   //Declare
    //Plant carbon allocation.
     ALLOC_AND_AUTO_RESP_FLUXES_STRUCT ARFLUXES;
     //define time-invariant parameters here
    ARFLUXES.IN.mr_r=pars[P.rauto_mr_r];//
    ARFLUXES.IN.mr_w=pars[P.rauto_mr_w];//
    ARFLUXES.IN.gr=pars[P.rauto_gr];//
    ARFLUXES.IN.Q10mr=pars[P.rauto_mr_q10];//

    //Heterotrophic respiration module
    HET_RESP_RATES_JCR_STRUCT HRJCR;
    //define time invariant parameters here
    
    /* jc prep input for methane module*/
    HRJCR.IN.S_FV=pars[P.S_fv];
    HRJCR.IN.SM_OPT=pars[P.thetas_opt];
    HRJCR.IN.FWC=pars[P.fwc];
    HRJCR.IN.R_CH4=pars[P.r_ch4];
    HRJCR.IN.Q10CH4=pars[P.Q10ch4];
    HRJCR.IN.Q10CO2=pars[P.Q10rhco2];

   




/*Combustion factors*/
double CF[7];//AAB changed this
CF[S.C_lab]=pars[P.cf_ligneous];
CF[S.C_fol]=pars[P.cf_foliar];
CF[S.C_roo]=pars[P.cf_ligneous];
CF[S.C_woo]=pars[P.cf_ligneous];
CF[S.C_cwd]=pars[P.cf_ligneous];
CF[S.C_lit]=(pars[P.cf_foliar]+pars[P.cf_ligneous])*0.5;
CF[S.C_som]=pars[P.cf_DOM];


/*resilience factor*/

/*foliar carbon transfer intermediate variables*/
double Fcfolavailable;

/*number of MET drivers*/
// int nomet=((DALEC *)DATA.MODEL)->nomet;

/*number of DALEC pools*/
int nopools=((DALEC *)DATA.MODEL)->nopools;

/*number of DALEC fluxes to store*/
int nofluxes=((DALEC *)DATA.MODEL)->nofluxes;



//********MAIN LOOP***********

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


double LAI=POOLS[p+S.D_LAI];
     
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
// double VegK0 = LAD/cos(zenith_angle/180*pi);
//     

  DALEC * DALECmodel=(DALEC *)DATA.MODEL;

DALEC_1100_DATA_STRUCT * DALEC_1100_DATA=(DALEC_1100_DATA_STRUCT *)DALECmodel->MODEL_DATA;
    
    double VegK=DALEC_1100_DATA->VegK[n];
//CONTINUE FROM HERE(
    //printf("VegK internal, external = %2.2f,  %2.2f\n",VegK, DALEC_1100_DATA->VegK[n]);







/*Temp scaling factor*/
double g;
double Tminmin = pars[P.Tminmin] - DGCM_TK0C; 
double Tminmax = pars[P.Tminmax] - DGCM_TK0C;
if( T2M_MIN[n] < Tminmin ) {
    g=0;
}
else if (T2M_MIN[n] > Tminmax) {
    g=1;
}
else {
    g=(T2M_MIN[n] - Tminmin)/(Tminmax - Tminmin);
}

// H2O stress scaling factor
	//We're also multiplying beta by cold-weather stress 
//double psi_LY10 = HYDROFUN_MOI2PSI(max(POOLS[p+S.D_SM_LY1],0),psi_porosity,pars[P.retention]);
//double psi_LY10 = HYDROFUN_MOI2PSI(POOLS[p+S.D_SM_LY1],psi_porosity,pars[P.retention_ly1]);

double beta1 = 1/(1 + exp(pars[P.beta_lgr]*(-1*POOLS[p+S.D_PSI_LY1]/pars[P.psi_50] - 1)))*POOLS[p+S.D_LF_LY1];
double beta2 = 1/(1 + exp(pars[P.beta_lgr]*(-1*POOLS[p+S.D_PSI_LY2]/pars[P.psi_50] - 1)))*POOLS[p+S.D_LF_LY2];
double beta = (beta1*pars[P.LY1_z] + beta2*pars[P.LY2_z]*pars[P.root_frac])/(pars[P.LY1_z]+pars[P.LY2_z]*pars[P.root_frac]);

// H2O stress biomass mortality factor: parallel structure to GPP scaling factor above

double betaHMF_1 = 1/(1 + exp(pars[P.beta_lgrHMF]*(-1*POOLS[p+S.D_PSI_LY1]/pars[P.psi_50HMF] - 1)))*POOLS[p+S.D_LF_LY1]; 
double betaHMF_2 = 1/(1 + exp(pars[P.beta_lgrHMF]*(-1*POOLS[p+S.D_PSI_LY2]/pars[P.psi_50HMF] - 1)))*POOLS[p+S.D_LF_LY2];
double betaHMF = (betaHMF_1*pars[P.LY1_z] + betaHMF_2*pars[P.LY2_z]*pars[P.root_frac])/(pars[P.LY1_z] +pars[P.LY2_z]*pars[P.root_frac]);

//if (betaHMF>1 | betaHMF<0) {printf("error in HMF, =%2.2f \n",betaHMF);} 

double HMF; // Hydraulic mortality factor

if( (POOLS[p+S.D_LF_LY1] + POOLS[p+S.D_LF_LY2]) == 2  ) { //no frozen water
   HMF = (1-betaHMF);
}
else {
    HMF = 0; //no hydraulic mortality when soils are frozen
}

// mean air temperature (K)
double air_temp_k = DGCM_TK0C+0.5*(T2M_MIN[n]+T2M_MAX[n]);

    FLUXES[f+F.beta_factor]=fmin(beta,g);
    FLUXES[f+F.soil_beta_factor]=beta;
    FLUXES[f+F.hydraulic_mortality_factor]=HMF;

//******************Declare LIU STRUCT*********************
LIU_AN_ET_STRUCT LIU;

//define time-invariant parameters
LIU.IN.SRAD=SSRD[n]*1e6/DGCM_SEC_DAY;
LIU.IN.VPD=VPD[n]/10;
LIU.IN.TEMP=air_temp_k;  
LIU.IN.vcmax25=pars[P.Vcmax25];
LIU.IN.co2=CO2[n];
LIU.IN.beta_factor=   FLUXES[f+F.beta_factor];
LIU.IN.g1=pars[P.Med_g1];
LIU.IN.LAI=LAI;
LIU.IN.ga=pars[P.ga];
LIU.IN.VegK=VegK;
LIU.IN.Tupp=pars[P.Tupp];
LIU.IN.Tdown=pars[P.Tdown];
LIU.IN.C3_frac=1.; // pars[P.C3_frac]
LIU.IN.clumping=pars[P.clumping];
LIU.IN.leaf_refl_par=pars[P.leaf_refl_par];
LIU.IN.leaf_refl_nir=pars[P.leaf_refl_nir];
LIU.IN.maxPevap=pars[P.maxPevap];
LIU.IN.precip=PREC[n];
LIU.IN.q10canopy=pars[P.q10canopy];
LIU.IN.q10canopyRd=pars[P.rauto_mrd_q10];
LIU.IN.canopyRdsf=pars[P.canopyRdsf];
LIU.IN.NSC=POOLS[p+S.C_lab];
LIU.IN.deltat=deltat;


//Call function: uses LIU->IN to update LIU->OUT
LIU_AN_ET(&LIU);

double LEAF_MORTALITY_FACTOR=LIU.OUT.LEAF_MORTALITY_FACTOR;

// GPP--- gross
FLUXES[f+F.gpp] = LIU.OUT.Ag;
// GPP net, i.e. GPP- Rd
FLUXES[f+F.gppnet] = LIU.OUT.An;
//transpiration//
double transp = LIU.OUT.transp;

if (beta1>0 && beta2 >0) { 
FLUXES[f+F.transp1] = transp*beta1*pars[P.LY1_z]/(beta1*pars[P.LY1_z]+beta2*pars[P.LY2_z]*pars[P.root_frac]);
FLUXES[f+F.transp2] = transp*beta2*pars[P.LY2_z]*pars[P.root_frac]/(beta1*pars[P.LY1_z]+beta2*pars[P.LY2_z]*pars[P.root_frac]);
//FLUXES[f+F.transp2] = transp - FLUXES[f+F.transp1];
}
else if (beta1>0 && !(beta2 >0)) {
FLUXES[f+F.transp1] = transp*beta1*pars[P.LY1_z]/(beta1*pars[P.LY1_z]+beta2*pars[P.LY2_z]*pars[P.root_frac]);
// FLUXES[f+F.transp2] = transp*beta2*pars[P.LY2_z]*pars[P.root_frac]/(beta1*pars[P.LY1_z]+beta2*pars[P.LY2_z]*pars[P.root_frac]);
FLUXES[f+F.transp2] = transp - FLUXES[f+F.transp1];
}
else if (!(beta1>0) && beta2 >0) {
//FLUXES[f+F.transp1] = transp*beta1*pars[P.LY1_z]/(beta1*pars[P.LY1_z]+beta2*pars[P.LY2_z]*pars[P.root_frac]);
FLUXES[f+F.transp2] = transp*beta2*pars[P.LY2_z]*pars[P.root_frac]/(beta1*pars[P.LY1_z]+beta2*pars[P.LY2_z]*pars[P.root_frac]);
FLUXES[f+F.transp1] = transp - FLUXES[f+F.transp2];
}
else { FLUXES[f+F.transp1] = 0;
       FLUXES[f+F.transp1] = 0;
    }

//evaporation//
FLUXES[f+F.evap] = LIU.OUT.evap;


/*Snow water equivalent*/
FLUXES[f+F.snowfall] = SNOWFALL[n];
POOLS[nxp+S.H2O_SWE]=POOLS[p+S.H2O_SWE]+FLUXES[f+F.snowfall]*deltat; /*first step snowfall to SWE*/
//transient_SCF
double SCFtemp = POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]);
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


/*Insure SWE does not go negative due to machine error*/
POOLS[nxp+S.H2O_SWE]=fmax(POOLS[nxp+S.H2O_SWE]-(FLUXES[f+F.melt] + FLUXES[f+F.sublimation])*deltat,0);
/*second step remove snowmelt from SWE*/
FLUXES[f+F.ets]=FLUXES[f+F.evap] + FLUXES[f+F.transp1] + FLUXES[f+F.transp2] + FLUXES[f+F.sublimation];

//Energy balance: Rn = LE + H - G
// Rn = SWin - SWout + LWin - LWout


double SWin = SSRD[n]*1e6/DGCM_SEC_DAY; // W m-2

//Snow free
double SWout_snowfree =(SWin*0.5*(pars[P.leaf_refl_par]+pars[P.leaf_refl_nir])); // W m-2
//Weighted average of surface albedo considering SW snow albedo as 0.9
double snow_albedo=0.9;//Consider age-dependent albedo.
    
//SW out
double SWout = (1. - POOLS[p+S.D_SCF])*SWout_snowfree + POOLS[p+S.D_SCF]*(SWin*snow_albedo); // W m-2
        



//Stefan-Boltzmann constant W.m-2.K-4
double sigma = 5.67*1e-8;
//Incident LW radiation - calculated
//double LWin = sigma*pow(air_temp_k,4.);
double LWin = STRD[n]*1e6/DGCM_SEC_DAY; // W m-2
//Outgoing LW radiation
double tskin_k = SKT[n]+DGCM_TK0C;
double LWout = sigma*(tskin_k*tskin_k)*(tskin_k*tskin_k); // W m-2
//Net radiation at the top of the canopy-soil continuum
//
//
double Rn = SWin - SWout + LWin - LWout; // W m-2
//Rnet only into soil
FLUXES[f+F.net_radiation] = Rn; // W m-2

//Rnet snow free
double Rn_snowfree = SWin - SWout_snowfree + LWin - LWout; // W m-2


//These are only fluxes into LY1 and out of LY1
FLUXES[f+F.SWin]=SWin;//flag for redundancy and deletion
FLUXES[f+F.LWin]=LWin;//flag for redundancy and deletion
FLUXES[f+F.SWout]=SWout;
FLUXES[f+F.LWout]=LWout;
        
//Latent heat of Vaporization J kg-1 
double lambda = DGCM_LATENT_HEAT_VAPORIZATION; //2.501*1e6 J kg-1 
//Latent heat (W.m-2)
double LE = lambda*(FLUXES[f+F.evap]+FLUXES[f+F.transp1]+FLUXES[f+F.transp2])/DGCM_SEC_DAY; // W m-2
FLUXES[f+F.latent_heat] = LE; // W m-2
//specific heat capacity of dry air is 1.00464 KJ kg -1 K -1
// Consider surface pressure as forcing for more accurate conversion from mol to m3
// Consider explicitly calculating cp based on humidity (derived from VPD and pressure)
double cp = 29.2; // J mol-1 K-1 representative specific heat of moist air at const pressure from Bonan book
double Psurf = 1e5; // Pa (representative surface pressure)
double Rgas = 8.31; // Universal gas constant (J mol-1 K-1)
// Pa / (J mol-1 K-1 * K) = mol m-3
double moles_per_m3 = Psurf/(Rgas*air_temp_k);
//Sensible heat 
// double H = cp*(tskin_k - air_temp_k)*pars[P.ga]*moles_per_m3; // ga in m s-1, 
// FLUXES[f+F.sensible_heat] = H; // W m-2
//Ground heat flux ONLY for energy in&out of vegetation-soil continuum
//Rn is scaled by snow free area, because we exclude snow energy balance from energy ODEs
//H is scaled by snow free area, because we exclude snow energy balance from energy ODEs
//LE is fully included, as evaporation and transpiration are assumed to come fully from snow-free areas (caveat: snow evaporation is a thing, btut we assume it's zero for this model)

//Ideally, Rn should be snow-free vs snow-covered.

// double  G = Rn*(1. - POOLS[p+S.D_SCF]) - H*(1. - POOLS[p+S.D_SCF]) - LE; // W m-2
// FLUXES[f+F.ground_heat] = G; // W m-2
// FLUXES[f+F.gh_in] = G*DGCM_SEC_DAY; // J m-2 d-1

//Gh_in approach 2 based on soil and LST
FLUXES[f+F.ground_heat] =(pars[P.thermal_cond_surf]* (tskin_k - POOLS[p+S.D_TEMP_LY1])/(pars[P.LY1_z]*0.5))*(1. - POOLS[p+S.D_SCF]);
FLUXES[f+F.gh_in] =FLUXES[f+F.ground_heat] *DGCM_SEC_DAY;        
//Using G, Rn and LE to derive H
// H = Rn - G  - LE
FLUXES[f+F.sensible_heat] = Rn - FLUXES[f+F.ground_heat] - FLUXES[f+F.latent_heat];
        

// Infiltration (mm/day)
double liquid_in = (PREC[n] - SNOWFALL[n] + FLUXES[f+F.melt]);
FLUXES[f+F.infil] = pars[P.max_infil]*(1 - exp(-liquid_in/pars[P.max_infil]));



// Surface runoff (mm/day)

FLUXES[f+F.q_surf] = liquid_in - FLUXES[f+F.infil];

// Volumetric soil moisture from water pools
// Include infiltration into LY1 ()
// double sm_LY1 = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_LY1],pars[P.LY1_por],pars[P.LY1_z]);
// double sm_LY3 = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_LY3],pars[P.LY3_por],pars[P.LY3_z]);

// Update LY1 SM with infiltration
//sm_LY1 += HYDROFUN_EWT2MOI(infil*deltat,pars[P.LY1_por],pars[P.LY1_z]);

// Calculate drainage

//printf("POOLS[p+S.D_LF_LY1] = %2.2f\n",POOLS[p+S.D_LF_LY1]);
double drain_LY1 = POOLS[p+S.D_LF_LY1]*DRAINAGE(POOLS[p+S.D_SM_LY1],pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);
double drain_LY2 = POOLS[p+S.D_LF_LY2]*DRAINAGE(POOLS[p+S.D_SM_LY2],pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);
double drain_LY3 = POOLS[p+S.D_LF_LY3]*DRAINAGE(POOLS[p+S.D_SM_LY3],pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);

// Drainage becomes runoff from pools
FLUXES[f+F.q_ly1] = HYDROFUN_MOI2EWT(drain_LY1,pars[P.LY1_por],pars[P.LY1_z])*one_over_deltat;
FLUXES[f+F.q_ly2] = HYDROFUN_MOI2EWT(drain_LY2,pars[P.LY2_por],pars[P.LY2_z])*one_over_deltat;
FLUXES[f+F.q_ly3] = HYDROFUN_MOI2EWT(drain_LY3,pars[P.LY3_por],pars[P.LY3_z])*one_over_deltat;


// Convert to conductivity
double k_LY1 = HYDROFUN_MOI2CON(POOLS[p+S.D_SM_LY1],pars[P.hydr_cond],pars[P.retention]);
double k_LY2 = HYDROFUN_MOI2CON(POOLS[p+S.D_SM_LY2],pars[P.hydr_cond],pars[P.retention]);
double k_LY3 = HYDROFUN_MOI2CON(POOLS[p+S.D_SM_LY3],pars[P.hydr_cond],pars[P.retention]);



// Calculate inter-pool transfer in m/s (positive is LY1 to LY2)
double pot_xfer = 1000 * sqrt(k_LY1*k_LY2) * (1000*(POOLS[p+S.D_PSI_LY1]-POOLS[p+S.D_PSI_LY2])/(9.8*0.5*(pars[P.LY1_z]+pars[P.LY2_z])) + 1);
double SPACEavail, H2Oavail, Max_H2O_xfer, TEMPxfer_1to2;
if (pot_xfer>0) {//Water is going LY1->LY2 (down)
// Available space in LY2 (after runoff)
SPACEavail=fmax(pars[P.LY2_z]*pars[P.LY2_por]*1e3 - POOLS[p+S.H2O_LY2] + (FLUXES[f+F.q_ly2] + FLUXES[f+F.transp2])*deltat,0);
// Available water in LY1 (after runoff, et, and infiltration)
H2Oavail=fmax(POOLS[p+S.D_LF_LY1]*POOLS[p+S.H2O_LY1] + (FLUXES[f+F.infil] - FLUXES[f+F.q_ly1] - FLUXES[f+F.evap] - FLUXES[f+F.transp1])*deltat,0);
// Maximum transfer flux in mm (actual transfer may be less due to water or space availability)
Max_H2O_xfer= POOLS[p+S.D_LF_LY1]*pot_xfer*1000*DGCM_SEC_DAY*deltat;
//Minimum of three terms for LY1->LY2
//1. Max_H2O_xfer
//2. Available space in LY2 (after runoff)
//3. LY1*LF (after runoff, et, and infiltration)
FLUXES[f+F.ly1xly2] =fmin(Max_H2O_xfer , fmin(SPACEavail, H2Oavail))*one_over_deltat;
TEMPxfer_1to2= POOLS[p+S.D_TEMP_LY1];//In K
}
else { //Water is going LY2->LY1 (up)
// Available space in LY1 after runoff, et, and infiltration
SPACEavail=fmax(pars[P.LY1_z]*pars[P.LY1_por]*1e3 - POOLS[p+S.H2O_LY1] - (FLUXES[f+F.infil] - FLUXES[f+F.q_ly1] - FLUXES[f+F.evap] - FLUXES[f+F.transp1])*deltat,0);
// Available water in LY2 after runoff
H2Oavail= fmax(POOLS[p+S.D_LF_LY2]*POOLS[p+S.H2O_LY2] - (FLUXES[f+F.q_ly2] + FLUXES[f+F.transp2])*deltat,0);
// Maximum transfer flux in mm (actual transfer may be less due to water or space availability)
Max_H2O_xfer= POOLS[p+S.D_LF_LY2]*pot_xfer*1000*DGCM_SEC_DAY*deltat;
// Reverse sign of previous case
FLUXES[f+F.ly1xly2] = -fmin(-Max_H2O_xfer , fmin(SPACEavail, H2Oavail))*one_over_deltat;
TEMPxfer_1to2= POOLS[p+S.D_TEMP_LY2];//In K
}

  


// Calculate inter-pool transfer in m/s (positive is LY1 to LY3)
pot_xfer = 1000 * sqrt(k_LY2*k_LY3) * (1000*(POOLS[p+S.D_PSI_LY2]-POOLS[p+S.D_PSI_LY3])/(9.8*0.5*(pars[P.LY2_z]+pars[P.LY3_z])) + 1);
double TEMPxfer_2to3;
if (pot_xfer>0) {//Water is going LY2->LY3 (down)
// Available space in LY3 (after runoff)
SPACEavail=fmax(pars[P.LY3_z]*pars[P.LY3_por]*1e3 - POOLS[p+S.H2O_LY3] + FLUXES[f+F.q_ly3]*deltat,0);
// Available water in LY2 (after runoff, et, and infiltration)
H2Oavail=fmax(POOLS[p+S.D_LF_LY2]*POOLS[p+S.H2O_LY2] - (FLUXES[f+F.q_ly2] + FLUXES[f+F.transp2])*deltat,0);
// Maximum transfer flux in mm (actual transfer may be less due to water or space availability)
Max_H2O_xfer= POOLS[p+S.D_LF_LY2]*pot_xfer*1000*DGCM_SEC_DAY*deltat;
//Minimum of three terms for LY2->LY3
//1. Max_H2O_xfer
//2. Available space in LY3 (after runoff)
//3. LY2*LF (after runoff, et, and infiltration)
FLUXES[f+F.ly2xly3] =fmin(Max_H2O_xfer , fmin(SPACEavail, H2Oavail))*one_over_deltat;
TEMPxfer_2to3= POOLS[p+S.D_TEMP_LY2];//In K
}
else { //Water is going LY3->LY2 (up)
// Available space in LY2 after runoff, et, and infiltration
SPACEavail=fmax(pars[P.LY2_z]*pars[P.LY2_por]*1e3 - POOLS[p+S.H2O_LY2] + (FLUXES[f+F.q_ly2] + FLUXES[f+F.transp2])*deltat,0);
// Available water in LY3 after runoff
H2Oavail= fmax(POOLS[p+S.D_LF_LY3]*POOLS[p+S.H2O_LY3] - FLUXES[f+F.q_ly3]*deltat,0);
// Maximum transfer flux in mm (actual transfer may be less due to water or space availability)
Max_H2O_xfer= POOLS[p+S.D_LF_LY3]*pot_xfer*1000*DGCM_SEC_DAY*deltat;
// Reverse sign of previous case
FLUXES[f+F.ly2xly3] = -fmin(-Max_H2O_xfer , fmin(SPACEavail, H2Oavail))*one_over_deltat;
TEMPxfer_2to3= POOLS[p+S.D_TEMP_LY3];//In K
}


// Update pools, including ET from LY1
    //Ensure all pools greater or equal to zero

POOLS[nxp+S.H2O_LY1] = POOLS[p+S.H2O_LY1] + (FLUXES[f+F.infil] - FLUXES[f+F.ly1xly2] - FLUXES[f+F.q_ly1] - FLUXES[f+F.evap] - FLUXES[f+F.transp1])*deltat;
POOLS[nxp+S.H2O_LY2] = POOLS[p+S.H2O_LY2] + (FLUXES[f+F.ly1xly2] - FLUXES[f+F.ly2xly3] - FLUXES[f+F.q_ly2] - FLUXES[f+F.transp2])*deltat;
POOLS[nxp+S.H2O_LY3] = POOLS[p+S.H2O_LY3] + (FLUXES[f+F.ly2xly3] - FLUXES[f+F.q_ly3])*deltat;




if (POOLS[nxp+S.H2O_LY1]>LY1max){
//Dump excess into LY1 Q
FLUXES[f+F.q_ly1] +=(POOLS[nxp+S.H2O_LY1]-LY1max)*one_over_deltat;
POOLS[nxp+S.H2O_LY1]=LY1max;}

if (POOLS[nxp+S.H2O_LY2]>LY2max){
//Dump excess into LY2 Q
FLUXES[f+F.q_ly1] +=(POOLS[nxp+S.H2O_LY2]-LY2max)*one_over_deltat;
POOLS[nxp+S.H2O_LY2]=LY2max;}

if (POOLS[nxp+S.H2O_LY3]>LY3max){
//Dump excess into LY3 Q
FLUXES[f+F.q_ly3] +=(POOLS[nxp+S.H2O_LY3]-LY3max)*one_over_deltat;
POOLS[nxp+S.H2O_LY3]=LY3max;}


//**********INTERNAL ENERGT FLUXES FOR ALL H2O FLUXES***************
//Add INFILTRATION, LY1, LY3, LY1xLY3, ET
double infiltemp = air_temp_k ;//Infiltemp needs to be in degrees celcius for IF statement to work
if (FLUXES[f+F.melt]>0){infiltemp = (infiltemp-DGCM_TK0C)*(PREC[n] - SNOWFALL[n])/(PREC[n] - SNOWFALL[n] + FLUXES[f+F.melt])+DGCM_TK0C;}//snowmelt temp = 0, so term multiplied by zero in weighted average 


//All energy fluxes

FLUXES[f+F.infil_e] = FLUXES[f+F.infil]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(infiltemp);
FLUXES[f+F.evap_e] = FLUXES[f+F.evap]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_LY1]);
FLUXES[f+F.transp1_e] = FLUXES[f+F.transp1]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_LY1]);
FLUXES[f+F.transp2_e] = FLUXES[f+F.transp2]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_LY2]);
FLUXES[f+F.ly1xly2_e] = FLUXES[f+F.ly1xly2]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(TEMPxfer_1to2);
FLUXES[f+F.ly2xly3_e] = FLUXES[f+F.ly2xly3]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(TEMPxfer_2to3);
FLUXES[f+F.q_ly1_e] = FLUXES[f+F.q_ly1]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_LY1]);
FLUXES[f+F.q_ly2_e] = FLUXES[f+F.q_ly2]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_LY2]);
FLUXES[f+F.q_ly3_e] =  FLUXES[f+F.q_ly3]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_LY3]);
//Thermal conductivity = k*dT/dz, units are W/m2, converting to J/m2/d
FLUXES[f+F.ly1xly2_th_e] = 2*pars[P.thermal_cond]* (POOLS[p+S.D_TEMP_LY1] - POOLS[p+S.D_TEMP_LY2])/(pars[P.LY1_z] + pars[P.LY2_z])*DGCM_SEC_DAY;
FLUXES[f+F.ly2xly3_th_e] = 2*pars[P.thermal_cond]* (POOLS[p+S.D_TEMP_LY2] - POOLS[p+S.D_TEMP_LY3])/(pars[P.LY2_z] + pars[P.LY3_z])*DGCM_SEC_DAY;

        
//Thermal energy flux only
//FLUXES[f+F.ly1xly3_e_thermal] = 

//Energy states
//fraction of water in soil that is available 
//double frac_ly1 = POOLS[nxp+S.H2O_LY1]/(POOLS[nxp+S.H2O_LY1]+POOLS[nxp+S.H2O_LY3]);
        // E_LY1


    FLUXES[f+F.geological]=PREDERIVED_GEO_FLUX;//In J/m2/d //105mW/m2
POOLS[nxp+S.E_LY1] = POOLS[p+S.E_LY1] + (FLUXES[f+F.gh_in] + FLUXES[f+F.infil_e] - FLUXES[f+F.evap_e] - FLUXES[f+F.transp1_e]  - FLUXES[f+F.q_ly1_e] - FLUXES[f+F.ly1xly2_e] - FLUXES[f+F.ly1xly2_th_e])*deltat;  
POOLS[nxp+S.E_LY2] = POOLS[p+S.E_LY2] + (FLUXES[f+F.ly1xly2_e] + FLUXES[f+F.ly1xly2_th_e] - FLUXES[f+F.transp2_e]  - FLUXES[f+F.q_ly2_e] - FLUXES[f+F.ly2xly3_e] - FLUXES[f+F.ly2xly3_th_e])*deltat;  
POOLS[nxp+S.E_LY3] = POOLS[p+S.E_LY3] + (FLUXES[f+F.ly2xly3_e] - FLUXES[f+F.q_ly3_e] + FLUXES[f+F.ly2xly3_th_e] + FLUXES[f + F.geological])*deltat; 

// 
// 


//*************KNORR LAI**************


//Time varying KNORR function terms

KNORR.IN.lambda_max_memory=  POOLS[p+S.M_LAI_MAX];
KNORR.IN.T_memory=POOLS[p+S.M_LAI_TEMP];
KNORR.IN.temp=air_temp_k;
KNORR.IN.n=n;
KNORR.IN.DOY=DOY[n];
KNORR.IN.lambda=LAI;
KNORR.IN.pasm=(POOLS[p+S.H2O_LY1]+POOLS[nxp+S.H2O_LY1]+POOLS[p+S.H2O_LY2]+POOLS[nxp+S.H2O_LY2])*0.5;//Note: soil moisture also available here
//KNORR.IN.transp= FLUXES[f+F.transp1]+FLUXES[f+F.transp2];
KNORR.IN.transp= transp;
//Call function: uses KNORR->IN to update KNORR->OUT
KNORR_ALLOCATION(&KNORR);
 
FLUXES[f+F.target_LAI]=KNORR.OUT.lambda_next;
//KNORR.OUT.dlambdadt is in units per timestep; converting thest to units per day (as required for CARDAMOM)
//"FLUXES" have to be in "per day" units
FLUXES[f+F.dlambda_dt]=KNORR.OUT.dlambdadt*one_over_deltat;
FLUXES[f+F.f_temp_thresh]= KNORR.OUT.f_T;
FLUXES[f+F.f_dayl_thresh]= KNORR.OUT.f_d;

//Update KNORR memory variables for next iteration
POOLS[nxp+S.M_LAI_MAX]=KNORR.OUT.laim;
POOLS[nxp+S.M_LAI_TEMP]=KNORR.OUT.T;
    

    
//************Allocation*******************

ARFLUXES.IN.deltat=deltat;
ARFLUXES.IN.GPP=FLUXES[f+F.gpp];
ARFLUXES.IN.Rd=LIU.OUT.Rd;
ARFLUXES.IN.TEMP=air_temp_k;
ARFLUXES.IN.NSC=POOLS[p+S.C_lab];
ARFLUXES.IN.C_LIVE_W=POOLS[p+S.C_woo];
ARFLUXES.IN.C_LIVE_R= POOLS[p+S.C_roo];
// Potential plant allocation (growth) fluxes
ARFLUXES.IN.ALLOC_FOL_POT=fmax(0, ((FLUXES[f+F.target_LAI] * pars[P.LCMA]) - POOLS[p+S.C_fol])*one_over_deltat);
ARFLUXES.IN.ALLOC_ROO_POT=fmax(0, (pars[P.phi_RL] * (FLUXES[f+F.target_LAI] * pars[P.LCMA]))*one_over_deltat);
ARFLUXES.IN.ALLOC_WOO_POT=fmax(0, (pars[P.phi_WL] * (FLUXES[f+F.target_LAI] * pars[P.LCMA]))*one_over_deltat);

ALLOC_AND_AUTO_RESP_FLUXES(&ARFLUXES);

    double NONLEAF_MORTALITY_FACTOR=ARFLUXES.OUT.NONLEAF_MORTALITY_FACTOR;
    




/*respiration auto*/
FLUXES[f+F.resp_auto]=ARFLUXES.OUT.AUTO_RESP_TOTAL;
/*growth respiration*/
FLUXES[f+F.resp_auto_growth]=ARFLUXES.OUT.AUTO_RESP_GROWTH;
/*maintenance respiration*/
FLUXES[f+F.resp_auto_maint]=ARFLUXES.OUT.AUTO_RESP_MAINTENANCE;
FLUXES[f+F.resp_auto_maint_dark]=LIU.OUT.Rd;

/*Compute leaf senescence: 
this is a C_fol removal based on Knorr output dlambda_dt,
which itself is computed based on the LAI passed to Knorr module, 
which is based on start-of-month C_fol; hence subsequent removals 
based on this flux should also go first into C_fol, to maintain 
compatible pool size */
if (FLUXES[f+F.dlambda_dt] > 0){ // i.e. when leaf growth is occuring 
    FLUXES[f+F.ph_fol2lit]=0;
}
else { // i.e. when leaf fall is occuring 
    //FLUXES[f+F.dlambda_dt] is in m2/m2/day
    //LCMA = gC/m2/m2
    FLUXES[f+F.ph_fol2lit]=-FLUXES[f+F.dlambda_dt]*pars[P.LCMA];
}




/*labile production*/
FLUXES[f+F.lab_prod] = ARFLUXES.OUT.F_LABPROD;
/*labile production*/
//FLUXES[f+F.lab_release] = ARFLUXES.OUT.F_LABREL_ACTUAL;
/*foliar production*/
FLUXES[f+F.foliar_prod] = ARFLUXES.OUT.ALLOC_FOL_ACTUAL;
/*root production*/        
FLUXES[f+F.root_prod] = ARFLUXES.OUT.ALLOC_ROO_ACTUAL;
/*wood production*/       
FLUXES[f+F.wood_prod] = ARFLUXES.OUT.ALLOC_WOO_ACTUAL;

/*-----------------------------------------------------------------------*/

  
    //TIME-VARYING INPUTS
     HRJCR.IN.SM=POOLS[p+S.D_SM_LY1];
     HRJCR.IN.TEMP=POOLS[p+S.D_TEMP_LY1];; // Input in degrees K
     HRJCR.IN.LF=POOLS[p+S.D_LF_LY1];; // Input in degrees 

       //JCR
       HET_RESP_RATES_JCR(&HRJCR);
       //OUtputs --- store anything we want here---
       FLUXES[f+F.aetr]=HRJCR.OUT.aerobic_tr;//Aerobic turnover rate scalar
       FLUXES[f+F.antr]=HRJCR.OUT.anaerobic_tr;//Anaerobic turnover rate scalar
       FLUXES[f+F.an_co2_c_ratio]=HRJCR.OUT.anaerobic_co2_c_ratio;//CO2_C_ratio
        FLUXES[f+F.an_ch4_c_ratio]=HRJCR.OUT.anaerobic_ch4_c_ratio;//CH4_C_ratio



//outputformat
//jcr_o 0-3 fT,fV,fW,fCH4; /*jc*/ /* output from JCR module */
double ae_loss_cwd = POOLS[p+S.C_cwd]*HRJCR.OUT.aerobic_tr*pars[P.t_cwd];
/* aerobic Rh from coarse woody debris*/
FLUXES[f+F.ae_rh_cwd] = ae_loss_cwd*(1-pars[P.tr_cwd2som]);
double ae_loss_lit = POOLS[p+S.C_lit]*HRJCR.OUT.aerobic_tr*pars[P.t_lit];
/* aerobic Rh from litter*/
FLUXES[f+F.ae_rh_lit] = ae_loss_lit*(1-pars[P.tr_lit2som]);
/* aerobic Rh from SOM*/
FLUXES[f+F.ae_rh_som] = POOLS[p+S.C_som]*HRJCR.OUT.aerobic_tr*pars[P.t_som];

//******Anaerobic fluxes
double an_loss_cwd = POOLS[p+S.C_cwd]*HRJCR.OUT.anaerobic_tr*pars[P.t_cwd];
/* anaerobic Rh from coarse woody debris*/
FLUXES[f+F.an_rh_cwd] = an_loss_cwd*(1-pars[P.tr_cwd2som]);
/* anaerobic Rh from litter*/
double an_loss_lit = POOLS[p+S.C_lit]*HRJCR.OUT.anaerobic_tr*pars[P.t_lit];
FLUXES[f+F.an_rh_lit] = an_loss_lit*(1-pars[P.tr_lit2som]);
/* anaerobic Rh from SOM*/
FLUXES[f+F.an_rh_som] = POOLS[p+S.C_som]*HRJCR.OUT.anaerobic_tr*pars[P.t_som];
/*CWD to SOM*/
FLUXES[f+F.cwd2som] = (an_loss_cwd + ae_loss_cwd)*pars[P.tr_cwd2som];
/*litter to SOM*/
FLUXES[f+F.lit2som] = (an_loss_lit + ae_loss_lit)*pars[P.tr_lit2som];
/* Rh_CO2*/
FLUXES[f+F.rh_co2] = (FLUXES[f+F.an_rh_lit]+FLUXES[f+F.an_rh_cwd]+FLUXES[f+F.an_rh_som])*HRJCR.OUT.anaerobic_co2_c_ratio + (FLUXES[f+F.ae_rh_lit]+FLUXES[f+F.ae_rh_cwd]+FLUXES[f+F.ae_rh_som]);
/* Rh_CH4*/
FLUXES[f+F.rh_ch4] = (FLUXES[f+F.an_rh_lit]+FLUXES[f+F.an_rh_cwd]+FLUXES[f+F.an_rh_som])*HRJCR.OUT.anaerobic_ch4_c_ratio;

/*----------------------  end of JCR  --------------------------------------------*/


/*total pool transfers (no fires yet)*/

/*CARBON POOL GROWTH AND PHENOLOGICAL LEAF FLUX*/
            /*LIVE POOLS*/
        POOLS[nxp+S.C_lab] = POOLS[p+S.C_lab] + (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto_maint]-FLUXES[f+F.foliar_prod]-FLUXES[f+F.root_prod]-FLUXES[f+F.wood_prod]-FLUXES[f+F.resp_auto_growth])*deltat;
        POOLS[nxp+S.C_fol] = POOLS[p+S.C_fol] + (FLUXES[f+F.foliar_prod]-FLUXES[f+F.ph_fol2lit])*deltat;
        POOLS[nxp+S.C_roo] = POOLS[p+S.C_roo] + FLUXES[f+F.root_prod]*deltat;
        POOLS[nxp+S.C_woo] = POOLS[p+S.C_woo] + FLUXES[f+F.wood_prod]*deltat;
            /*DEAD POOLS*/
        POOLS[nxp+S.C_cwd] = POOLS[p+S.C_cwd] - (FLUXES[f+F.ae_rh_cwd]+FLUXES[f+F.an_rh_cwd]+FLUXES[f+F.cwd2som])*deltat;
        POOLS[nxp+S.C_lit] = POOLS[p+S.C_lit] + (FLUXES[f+F.ph_fol2lit] - FLUXES[f+F.ae_rh_lit] - FLUXES[f+F.an_rh_lit] - FLUXES[f+F.lit2som])*deltat;
        POOLS[nxp+S.C_som] = POOLS[p+S.C_som] + (FLUXES[f+F.lit2som] - FLUXES[f+F.ae_rh_som] - FLUXES[f+F.an_rh_som] + FLUXES[f+F.cwd2som])*deltat;

	/*Carbon pool transfers - 
    WITH FIRES, 
    HYDRAULIC FAILURE, 
    CARBON STARVATION, 
    HUMAN DISTURBANCE, 
    AND BACKGROUND MORTALITY*/
	/*first fluxes*/

    /*Calculating disturbance flux as percent of live biomass*/
    /*NB: p=index of current pool timestep; nxp=index of next pool timestep; 
    removals are scaled by pool(nxp) which has received additions from growth, above*/
    double TotalABGB=POOLS[nxp+S.C_lab]+POOLS[nxp+S.C_fol]+POOLS[nxp+S.C_roo]+POOLS[nxp+S.C_woo]; 
    double DMF = DIST[n]/TotalABGB; //DIST[n]=disturbance flux at current flux timestep, halfway in between p and nxp 
    
        //Store fluxes for FIOMATRIX balance checks 
    FLUXES[f+F.dist_lab] = POOLS[nxp+S.C_lab]*DMF*one_over_deltat;
    FLUXES[f+F.dist_fol] = POOLS[nxp+S.C_fol]*DMF*one_over_deltat;
    FLUXES[f+F.dist_roo] = POOLS[nxp+S.C_roo]*DMF*one_over_deltat;
    FLUXES[f+F.dist_woo] = POOLS[nxp+S.C_woo]*DMF*one_over_deltat;

/*DIRECT LIVE CARBON POOL REMOVALS PART 1 of 4: Removing ABGB disturbance from live pools here*/
    /*Note: these are lateral fluxes, and are discarded, not transferred!*/
    POOLS[nxp+S.C_lab] = POOLS[nxp+S.C_lab]-FLUXES[f+F.dist_lab]*deltat;
    POOLS[nxp+S.C_fol] = POOLS[nxp+S.C_fol]-FLUXES[f+F.dist_fol]*deltat;
    POOLS[nxp+S.C_roo] = POOLS[nxp+S.C_roo]-FLUXES[f+F.dist_roo]*deltat;
    POOLS[nxp+S.C_woo] = POOLS[nxp+S.C_woo]-FLUXES[f+F.dist_woo]*deltat;

	/*Calculating all fire transfers from C pools to atmosphere via combustion*/
	    /*note: all fluxes are in gC m-2 day-1*/
    FLUXES[f+F.f_lab] = POOLS[nxp+S.C_lab]*BURNED_AREA[n]*CF[S.C_lab]*one_over_deltat;
    FLUXES[f+F.f_fol] = POOLS[nxp+S.C_fol]*BURNED_AREA[n]*CF[S.C_fol]*one_over_deltat;
    FLUXES[f+F.f_roo] = POOLS[nxp+S.C_roo]*BURNED_AREA[n]*CF[S.C_roo]*one_over_deltat;
    FLUXES[f+F.f_woo] = POOLS[nxp+S.C_woo]*BURNED_AREA[n]*CF[S.C_woo]*one_over_deltat;
    FLUXES[f+F.f_cwd] = POOLS[nxp+S.C_cwd]*BURNED_AREA[n]*CF[S.C_cwd]*one_over_deltat;
    FLUXES[f+F.f_lit] = POOLS[nxp+S.C_lit]*BURNED_AREA[n]*CF[S.C_lit]*one_over_deltat;
    FLUXES[f+F.f_som] = POOLS[nxp+S.C_som]*BURNED_AREA[n]*CF[S.C_som]*one_over_deltat;  

    
/*LIVE CARBON POOL REMOVALS PART 2 of 4: Removing fire fluxes from live pools here*/
    POOLS[nxp+S.C_lab] = POOLS[nxp+S.C_lab]-FLUXES[f+F.f_lab]*deltat;
    POOLS[nxp+S.C_fol] = POOLS[nxp+S.C_fol]-FLUXES[f+F.f_fol]*deltat;
    POOLS[nxp+S.C_roo] = POOLS[nxp+S.C_roo]-FLUXES[f+F.f_roo]*deltat;
    POOLS[nxp+S.C_woo] = POOLS[nxp+S.C_woo]-FLUXES[f+F.f_woo]*deltat;
	/*DEAD CARBON POOL removals for fire*/
    POOLS[nxp+S.C_cwd] = POOLS[nxp+S.C_cwd]-FLUXES[f+F.f_cwd]*deltat;
    POOLS[nxp+S.C_lit] = POOLS[nxp+S.C_lit]-FLUXES[f+F.f_lit]*deltat;
    POOLS[nxp+S.C_som] = POOLS[nxp+S.C_som]-FLUXES[f+F.f_som]*deltat; 

  //LIVE BIOMASS MORTALITY FLUXES
    /* Compute aggregate mortality factor by pool from competing environmental stress: 
    -C starvation 
    -Hydraulic Failure 
    -Fire injury mortality*/ 
    double AMF_C_lab = (1 - (1-NONLEAF_MORTALITY_FACTOR) * (1-(BURNED_AREA[n]*(pars[P.resilience]))) * (1-HMF));
    double AMF_C_fol = (1 - (1-LEAF_MORTALITY_FACTOR) * (1-(BURNED_AREA[n]*(pars[P.resilience]))) * (1-HMF));
    double AMF_C_roo = (1 - (1-NONLEAF_MORTALITY_FACTOR) * (1-(BURNED_AREA[n]*(pars[P.resilience]))) * (1-HMF));
    double AMF_C_woo = (1 - (1-NONLEAF_MORTALITY_FACTOR) * (1-(BURNED_AREA[n]*(pars[P.resilience]))) * (1-HMF));
   
    FLUXES[f+F.fx_lab2lit] = POOLS[nxp+S.C_lab]*(AMF_C_lab)*one_over_deltat;
    FLUXES[f+F.fx_fol2lit] = POOLS[nxp+S.C_fol]*(AMF_C_fol)*one_over_deltat;
    FLUXES[f+F.fx_roo2lit] = POOLS[nxp+S.C_roo]*(AMF_C_roo)*one_over_deltat;
    FLUXES[f+F.fx_woo2cwd] = POOLS[nxp+S.C_woo]*(AMF_C_woo)*one_over_deltat;
    //No mortality in these pools
    FLUXES[f+F.fx_cwd2som] = POOLS[nxp+S.C_cwd]*BURNED_AREA[n]*(pars[P.resilience])*one_over_deltat;
    FLUXES[f+F.fx_lit2som] = POOLS[nxp+S.C_lit]*BURNED_AREA[n]*(pars[P.resilience])*one_over_deltat;
	
/*LIVE CARBON POOL TRANSFERS PART 3 of 4: environmental stress mortality fluxes to dead pools*/	
    
    POOLS[nxp+S.C_lab] = POOLS[nxp+S.C_lab]-FLUXES[f+F.fx_lab2lit]*deltat;
    POOLS[nxp+S.C_fol] = POOLS[nxp+S.C_fol]-FLUXES[f+F.fx_fol2lit]*deltat;
    POOLS[nxp+S.C_roo] = POOLS[nxp+S.C_roo]-FLUXES[f+F.fx_roo2lit]*deltat;
    POOLS[nxp+S.C_woo] = POOLS[nxp+S.C_woo]-FLUXES[f+F.fx_woo2cwd]*deltat;

/*Calculating background mortality (age/herbivory/impact) flux as percent of remaining live biomass*/
/*wood CWD production*/       
FLUXES[f+F.woo2cwd] = POOLS[nxp+S.C_woo]*pars[P.t_wood];
/*root litter production*/
FLUXES[f+F.roo2lit] = POOLS[nxp+S.C_roo]*pars[P.t_root];
/*labile litter production*/
FLUXES[f+F.lab2lit] = POOLS[nxp+S.C_lab]*pars[P.t_lab];

// Fcfolavailable=FLUXES[f+F.lab_prod] + POOLS[p+S.C_lab]*one_over_deltat;
if (FLUXES[f+F.dlambda_dt] > 0){
  FLUXES[f+F.fol2lit]=POOLS[p+S.C_fol]*(1-pow(1-pars[P.t_foliar],deltat))*one_over_deltat;
}
else {
    //FLUXES[f+F.dlambda_dt] is in m2/m2/day
    //LCMA = gC/m2/m2
  FLUXES[f+F.fol2lit]=POOLS[p+S.C_fol]*pars[P.t_foliar];
}

/*LIVE CARBON POOL TRANSFERS PART 4 of 4: background mortality fluxes to dead pools*/	

    POOLS[nxp+S.C_lab] = POOLS[nxp+S.C_lab]-FLUXES[f+F.lab2lit]*deltat;
    POOLS[nxp+S.C_fol] = POOLS[nxp+S.C_fol]-FLUXES[f+F.fol2lit]*deltat;
    POOLS[nxp+S.C_roo] = POOLS[nxp+S.C_roo]-FLUXES[f+F.roo2lit]*deltat;
    POOLS[nxp+S.C_woo] = POOLS[nxp+S.C_woo]-FLUXES[f+F.woo2cwd]*deltat;
	
/*DEAD C POOLS TRANSFERS PART 1 of 1: Adding fire removals here together with additions from live pools*/
    /*CWD*/
    POOLS[nxp+S.C_cwd] = POOLS[nxp+S.C_cwd]+(FLUXES[f+F.woo2cwd]+FLUXES[f+F.fx_woo2cwd]-FLUXES[f+F.fx_cwd2som])*deltat;
    /*litter*/
    POOLS[nxp+S.C_lit] = POOLS[nxp+S.C_lit]+(FLUXES[f+F.lab2lit]+FLUXES[f+F.fx_lab2lit]+FLUXES[f+F.fol2lit]+FLUXES[f+F.fx_fol2lit]+FLUXES[f+F.roo2lit]+FLUXES[f+F.fx_roo2lit]-FLUXES[f+F.fx_lit2som])*deltat;
	/*som*/
	POOLS[nxp+S.C_som] = POOLS[nxp+S.C_som]+(FLUXES[f+F.fx_cwd2som]+FLUXES[f+F.fx_lit2som])*deltat;
        
        
	/*fires - total flux in gC m-2 day-1*/
	/*this term is now (essentially) obsolete*/
	/*replace in next version of DALEC_FIRES*/
    FLUXES[f+F.f_total] = FLUXES[f+F.f_lab] + FLUXES[f+F.f_fol] + FLUXES[f+F.f_roo] + FLUXES[f+F.f_woo] + FLUXES[f+F.f_cwd] + FLUXES[f+F.f_lit] + FLUXES[f+F.f_som];

    /*Fraction of C-foliar lost due to fires*/
    //FLUXES[f+F.foliar_fire_frac] = BURNED_AREA[n]*(CF[S.C_lab] + (1-CF[S.C_lab])*(1-pars[P.resilience]));
    /*Calculate LAI (lambda) lost due to fire
      - we lose the same fraction of LAI as we do C-foliar 
      - FE_\Lambda^{(t+1)} = \Lambda^{(t+1)'} * BA ( k_{factor(i)} + (1 - k_{factor(i)}) r )*/
    //FLUXES[f+F.lai_fire] = (POOLS[p+S.C_fol]/pars[P.LCMA])*BURNED_AREA[n]*(CF[S.C_lab] + (1-CF[S.C_lab])*(1-pars[P.resilience]));

    /****************************RECORD t+1 DIAGNOSTIC STATES*************************/
    POOLS[nxp+S.D_LAI]=POOLS[nxp+S.C_fol]/pars[P.LCMA]; //LAI
    POOLS[nxp+S.D_SCF]=POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]); //snow cover fraction
    
    
    
    //Update time-varying inputs
    LY1SOILTEMP.IN.soil_water = POOLS[nxp+S.H2O_LY1];//mm (or kg/m2)
    LY2SOILTEMP.IN.soil_water = POOLS[nxp+S.H2O_LY2];//mm (or kg/m2)
    LY3SOILTEMP.IN.soil_water = POOLS[nxp+S.H2O_LY3];//mm (or kg/m2)
    LY1SOILTEMP.IN.internal_energy = POOLS[nxp+S.E_LY1];//Joules
    LY2SOILTEMP.IN.internal_energy = POOLS[nxp+S.E_LY2];//Joules
    LY3SOILTEMP.IN.internal_energy = POOLS[nxp+S.E_LY3];//Joules
    
    SOIL_TEMP_AND_LIQUID_FRAC(&LY1SOILTEMP);
    SOIL_TEMP_AND_LIQUID_FRAC(&LY2SOILTEMP);
    SOIL_TEMP_AND_LIQUID_FRAC(&LY3SOILTEMP);

    POOLS[nxp+S.D_TEMP_LY1]=LY1SOILTEMP.OUT.TEMP;//In K
    POOLS[nxp+S.D_TEMP_LY2]=LY2SOILTEMP.OUT.TEMP;//In K
    POOLS[nxp+S.D_TEMP_LY3]=LY3SOILTEMP.OUT.TEMP;//In K
    
    POOLS[nxp+S.D_LF_LY1]=LY1SOILTEMP.OUT.LF;
    POOLS[nxp+S.D_LF_LY2]=LY2SOILTEMP.OUT.LF;
    POOLS[nxp+S.D_LF_LY3]=LY3SOILTEMP.OUT.LF;
    

    //Pass pointers to function 

    
    //Soil moisture

        POOLS[nxp+S.D_SM_LY1]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_LY1],pars[P.LY1_por],pars[P.LY1_z]); //soil moisture LY1
        POOLS[nxp+S.D_SM_LY2]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_LY2],pars[P.LY2_por],pars[P.LY2_z]);//soil moisture LY2
        POOLS[nxp+S.D_SM_LY3]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_LY3],pars[P.LY3_por],pars[P.LY3_z]);//soil moisture LY3


        POOLS[nxp+S.D_PSI_LY1]=fmax(min_psi, HYDROFUN_MOI2PSI(  POOLS[nxp+S.D_SM_LY1],psi_porosity,pars[P.retention]));
        POOLS[nxp+S.D_PSI_LY2]=fmax(min_psi, HYDROFUN_MOI2PSI(  POOLS[nxp+S.D_SM_LY2],psi_porosity,pars[P.retention]));
        POOLS[nxp+S.D_PSI_LY3]=fmax(min_psi, HYDROFUN_MOI2PSI(  POOLS[nxp+S.D_SM_LY3],psi_porosity,pars[P.retention]));


//     //Isfinite check for 14 progronstic pools only
//     int nnn, isfinitecheck=1;
//     for (nnn=0;nnn<14;nnn++){if ( isfinite(POOLS[nxp+nnn])==false){isfinitecheck=0;}};
//     if (isfinitecheck==0){break;};

}



return 0;
}


int DALEC_1100_MODCONFIG(DALEC * DALECmodel, DATA * DATA){


    PREDERIVE_DALEC_1100_DATA(DALECmodel, DATA);

struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;
struct DALEC_1100_EDCs E=DALEC_1100_EDCs;




 //DALECmodel->data=DALEC_1100_DATA;
DALECmodel->dalec=DALEC_1100;
DALECmodel->nopools=30;
DALECmodel->nomet=10;/*This should be compatible with CBF file, if not then disp error*/
DALECmodel->nopars=89;
DALECmodel->nofluxes=89;
DALECmodel->noedcs=15;

DALEC_1100_FLUX_SOURCES_SINKS(DALECmodel);




//Define PARS_INFO here (ranges, and eventually names, etc)
PARS_INFO_1100(DALECmodel);

//EDC operator
//Make sure has same number as number of EDCs
//This is generic EDCs structure defined in ../DALEC_EDCs/DALEC_EDC_FUNCTIONS.c
//Has three args, data (void), function (in "DATA", and "void *", and "double" out), and "boolean" prerun.





DALECmodel->EDCs=calloc(DALECmodel->noedcs,sizeof(EDCs));
//Abbrieviate
EDCs * EDCs=DALECmodel->EDCs;

//Som lit turnover rate

//EDC: litter tor > cwd tor
//List all inequality calls here
static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_litcwdtor; 
static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_cwdsomtor;
static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_rootwoodtor;
static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_mr_rates;
static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_fol2lig_cf;
static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_relativepsi50;

//EDC: lit tor > cwd tor
EDC_litcwdtor.big_par_index=P.t_lit;
EDC_litcwdtor.small_par_index=P.t_cwd;
EDCs[E.litcwdtor].data=&EDC_litcwdtor;
EDCs[E.litcwdtor].function=&DALEC_EDC_PARAMETER_INEQUALITY;
EDCs[E.litcwdtor].prerun=true;

//EDC: cwd tor > som tor
EDC_cwdsomtor.big_par_index=P.t_cwd;
EDC_cwdsomtor.small_par_index=P.t_som;
EDCs[E.cwdsomtor].data=&EDC_cwdsomtor;
EDCs[E.cwdsomtor].function=&DALEC_EDC_PARAMETER_INEQUALITY;
EDCs[E.cwdsomtor].prerun=true;

//EDC: root tor > wood tor
EDC_rootwoodtor.big_par_index=P.t_root;
EDC_rootwoodtor.small_par_index=P.t_wood;
EDCs[E.rootwoodtor].data=&EDC_rootwoodtor;
EDCs[E.rootwoodtor].function=&DALEC_EDC_PARAMETER_INEQUALITY;
EDCs[E.rootwoodtor].prerun=true;

//EDC: foliar and root mr > wood mr
EDC_mr_rates.big_par_index=P.rauto_mr_r;
EDC_mr_rates.small_par_index=P.rauto_mr_w;
EDCs[E.mr_rates].data=&EDC_mr_rates;
EDCs[E.mr_rates].function=&DALEC_EDC_PARAMETER_INEQUALITY;
EDCs[E.mr_rates].prerun=true;

//EDC: cf_foliar > cf_ligneous
EDC_fol2lig_cf.big_par_index=P.cf_foliar;
EDC_fol2lig_cf.small_par_index=P.cf_ligneous;
EDCs[E.fol2lig_cf].data=&EDC_fol2lig_cf;
EDCs[E.fol2lig_cf].function=&DALEC_EDC_PARAMETER_INEQUALITY;
EDCs[E.fol2lig_cf].prerun=true;

//EDC: psi50HMF > psi50 
EDC_relativepsi50.big_par_index=P.psi_50HMF;
EDC_relativepsi50.small_par_index=P.psi_50;
EDCs[E.relativepsi50].data=&EDC_relativepsi50;
EDCs[E.relativepsi50].function=&DALEC_EDC_PARAMETER_INEQUALITY;
EDCs[E.relativepsi50].prerun=true;

static DALEC_EDC_PARAMETER_LOG_RATIO_STRUCT EDC_vcmax_lcma;
//EDC: ratio of Vcmax25 to LCMA
EDC_vcmax_lcma.numerator_index=P.Vcmax25;
EDC_vcmax_lcma.denominator_index=P.LCMA;
EDC_vcmax_lcma.mean_ratio = 1.0399; // From TRY database
EDC_vcmax_lcma.std_ratio = 0.1956; // From TRY database
EDCs[E.vcmax_lcma].data=&EDC_vcmax_lcma;
EDCs[E.vcmax_lcma].function=&DALEC_EDC_PARAMETER_LOG_RATIO;
EDCs[E.vcmax_lcma].prerun=true;





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
  
   EDC_sr.min_val[S.C_lab]=0;
   EDC_sr.max_val[S.C_lab]=3*(DALECmodel->PARS_INFO.parmax[P.i_labile]);
                
   EDC_sr.min_val[S.C_fol]=0;
   EDC_sr.max_val[S.C_fol]=3*(DALECmodel->PARS_INFO.parmax[P.i_foliar]);
                
   EDC_sr.min_val[S.C_roo]=0;
   EDC_sr.max_val[S.C_roo]=3*(DALECmodel->PARS_INFO.parmax[P.i_root]);
                
   EDC_sr.min_val[S.C_woo]=0;
   EDC_sr.max_val[S.C_woo]=3*(DALECmodel->PARS_INFO.parmax[P.i_wood]);
                
   EDC_sr.min_val[S.C_cwd]=0;
   EDC_sr.max_val[S.C_cwd]=3*(DALECmodel->PARS_INFO.parmax[P.i_cwd]);
                
   EDC_sr.min_val[S.C_lit]=0;
   EDC_sr.max_val[S.C_lit]=3*(DALECmodel->PARS_INFO.parmax[P.i_lit]);
                
   EDC_sr.min_val[S.C_som]=0;
   EDC_sr.max_val[S.C_som]=3*(DALECmodel->PARS_INFO.parmax[P.i_som]);
                
    EDC_sr.min_val[S.H2O_LY1]=0;
   //
                
   EDC_sr.min_val[S.H2O_LY2]=0;
   //
                
   EDC_sr.min_val[S.H2O_LY3]=0;
   //
                
    EDC_sr.min_val[S.H2O_SWE]=0;
//    EDC_sr.max_val[S.H2O_SWE]=DALECmodel->PARS_INFO.parmax[P.i_SWE];
//     
    EDC_sr.min_val[S.E_LY1]=0;
//     
    EDC_sr.min_val[S.E_LY2]=0;
//     
    EDC_sr.min_val[S.E_LY3]=0;
//     
    EDC_sr.min_val[S.D_LAI]=0;
    EDC_sr.max_val[S.D_LAI]=DALECmodel->PARS_INFO.parmax[P.lambda_max];
//     
    EDC_sr.min_val[S.D_SCF]=0;
    EDC_sr.max_val[S.D_SCF]=1;
//     
    EDC_sr.min_val[S.D_TEMP_LY1]=173.15;
    EDC_sr.max_val[S.D_TEMP_LY1]=373.15;
//     
    EDC_sr.min_val[S.D_TEMP_LY2]=173.15;
    EDC_sr.max_val[S.D_TEMP_LY2]=373.15;
//     
    EDC_sr.min_val[S.D_TEMP_LY3]=173.15;
    EDC_sr.max_val[S.D_TEMP_LY3]=373.15;
//     
    EDC_sr.min_val[S.D_LF_LY1]=0;
    EDC_sr.max_val[S.D_LF_LY1]=1;
//     
    EDC_sr.min_val[S.D_LF_LY3]=0;
    EDC_sr.max_val[S.D_LF_LY3]=1;
//     
   EDC_sr.min_val[S.D_SM_LY1]=0;
    EDC_sr.max_val[S.D_SM_LY1]=1;
//     
    EDC_sr.min_val[S.D_SM_LY2]=0;
    EDC_sr.max_val[S.D_SM_LY2]=1;
//     
    EDC_sr.min_val[S.D_SM_LY3]=0;
    EDC_sr.max_val[S.D_SM_LY3]=1;
    
//    EDC_sr.min_val[S.M_LAI_MAX]=-INFINITY; // get a better value from Alex N.
//    EDC_sr.max_val[S.M_LAI_MAX]=INFINITY; // get a better value from Alex N.
    
//    EDC_sr.min_val[S.M_LAI_TEMP]=-INFINITY;  // get a better value from Alex N.
//    EDC_sr.max_val[S.M_LAI_TEMP]=INFINITY; // get a better value from Alex N.





//Adding EDC to the EDCs list


    EDCs[E.state_ranges].data=&EDC_sr;
    EDCs[E.state_ranges].function=&DALEC_EDC_STATE_RANGES;
    EDCs[E.state_ranges].prerun=false;







 static DALEC_EDC_TRAJECTORY_STRUCT EDC_st;
 
 static int edc_pool_indices[14];
    static double edc_pool_eqf[14];
EDC_st.pool_indices=edc_pool_indices;
EDC_st.pool_eqf=edc_pool_eqf;
EDC_st.no_pools_to_check=14;

EDC_st.pool_indices[0]=S.C_lab;
EDC_st.pool_indices[1]=S.C_fol;
EDC_st.pool_indices[2]=S.C_roo;
EDC_st.pool_indices[3]=S.C_woo;
EDC_st.pool_indices[4]=S.C_cwd;
EDC_st.pool_indices[5]=S.C_lit;
EDC_st.pool_indices[6]=S.C_som;
EDC_st.pool_indices[7]=S.H2O_LY1;
EDC_st.pool_indices[8]=S.H2O_LY2;
EDC_st.pool_indices[9]=S.H2O_LY3;
EDC_st.pool_indices[10]=S.H2O_SWE;
EDC_st.pool_indices[11]=S.E_LY1;
EDC_st.pool_indices[12]=S.E_LY2;
EDC_st.pool_indices[13]=S.E_LY3;
//EDC_st.pool_indices[12]=S.M_LAI_MAX;```
//EDC_st.pool_indices[13]=S.M_LAI_TEMP;



for (n=0;n<EDC_st.no_pools_to_check;n++){EDC_st.pool_eqf[n]=DATA->ncdf_data.EDC_EQF;printf("DATA->ncdf_data.EDC_EQF = %2.2f\n",DATA->ncdf_data.EDC_EQF);}
//  EDC_st.pool_eqf[9]=10;
//     EDC_st.pool_eqf[10]=10;

// //Rest can be done by code without additional input
    
EDCs[E.state_trajectories].data=&EDC_st;
EDCs[E.state_trajectories].function=&DALEC_EDC_TRAJECTORY;
EDCs[E.state_trajectories].prerun=false;

//Eventually adopt more succinct notation (to consider)
//e.g. INEQUALITY_EDC(P.t_cwd,P.t_som,EDCs[E.cwdsomtor])

 static DALEC_EDC_NSC_ABGB_RATIO_STRUCT EDC_nscr;

    //
    EDC_nscr.no_other_pool_indices=3;
    static int nscratio_other_pool_indices[3];

    EDC_nscr.other_pool_indices=nscratio_other_pool_indices;
    EDC_nscr.other_pool_indices[0]=S.C_fol;
    EDC_nscr.other_pool_indices[1]=S.C_roo;
    EDC_nscr.other_pool_indices[2]=S.C_woo;
    EDC_nscr.nsc_pool_index=S.C_lab;


    EDCs[E.nsc_ratio].data=&EDC_nscr;
    EDCs[E.nsc_ratio].function=&DALEC_EDC_NSC_ABGB_RATIO;
    EDCs[E.nsc_ratio].prerun=false;

static DALEC_EDC_POOL_RATIO_STRUCT EDC_cfcrr;
    //
    EDC_cfcrr.numerator_index=S.C_fol;
    EDC_cfcrr.denominator_index=S.C_roo;
    
    EDCs[E.cfcr_ratio].data=&EDC_cfcrr;
    EDCs[E.cfcr_ratio].function=&DALEC_EDC_POOL_RATIO;
    EDCs[E.cfcr_ratio].prerun=false;    

static DALEC_EDC_FLUX_RATIO_STRUCT EDC_fffrr;
    //
    EDC_fffrr.numerator_flux_index=F.foliar_prod;
    EDC_fffrr.denominator_flux_index=F.root_prod;
    
    EDCs[E.fffr_ratio].data=&EDC_fffrr;
    EDCs[E.fffr_ratio].function=&DALEC_EDC_FLUX_RATIO;
    EDCs[E.fffr_ratio].prerun=false;    

static DALEC_EDC_MEAN_TEMP_STRUCT EDC_mean_ly1_temp, EDC_mean_ly2_temp, EDC_mean_ly3_temp;

    //
    EDC_mean_ly1_temp.temp_index=S.D_TEMP_LY1;
    EDCs[E.mean_ly1_temp].data=&EDC_mean_ly1_temp;
    EDCs[E.mean_ly1_temp].function=&DALEC_EDC_MEAN_TEMP;
    EDCs[E.mean_ly1_temp].prerun=false;


    EDC_mean_ly2_temp.temp_index=S.D_TEMP_LY2;
    EDCs[E.mean_ly2_temp].data=&EDC_mean_ly2_temp;
    EDCs[E.mean_ly2_temp].function=&DALEC_EDC_MEAN_TEMP;
    EDCs[E.mean_ly2_temp].prerun=false;


    EDC_mean_ly3_temp.temp_index=S.D_TEMP_LY3;
    EDCs[E.mean_ly3_temp].data=&EDC_mean_ly3_temp;
    EDCs[E.mean_ly3_temp].function=&DALEC_EDC_MEAN_TEMP;
    EDCs[E.mean_ly3_temp].prerun=false;



//ecological
//EDCOPE.SUPPORT_LITCWDSOM_trpar_EDC=true;
//dynamical
//EDCOPE.SUPPORT_POOL_SSP_EDC=true;
//Numerical checks
//EDCOPE.SUPPORT_FINITE_EDC=true;




//declaring observation operator structure, and filling with DALEC configurations
static OBSOPE OBSOPE;
//Initialize all SUPPORT OBS values (default value = false).
INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);


//Set SUPPORT_OBS values to true if model supports external observation operations.
OBSOPE.SUPPORT_GPP_OBS=true;
OBSOPE.SUPPORT_SIF_OBS=true;
OBSOPE.SUPPORT_LAI_OBS=true;
OBSOPE.SUPPORT_ET_OBS=true;
OBSOPE.SUPPORT_NBE_OBS=true;
OBSOPE.SUPPORT_ABGB_OBS=true;
OBSOPE.SUPPORT_DOM_OBS=true;
OBSOPE.SUPPORT_EWT_OBS=true;
OBSOPE.SUPPORT_FIR_OBS=true;
OBSOPE.SUPPORT_CH4_OBS=true;
OBSOPE.SUPPORT_ROFF_OBS=true;
OBSOPE.SUPPORT_SCF_OBS=true;




// OBSOPE.SUPPORT_CUE_OBS=true;
OBSOPE.SUPPORT_Vcmax25_OBS=true;
OBSOPE.SUPPORT_C3frac_OBS=true;
OBSOPE.SUPPORT_iniSnow_OBS=true;
OBSOPE.SUPPORT_iniSOM_OBS=true;
OBSOPE.SUPPORT_CUEmrg_OBS=true;
OBSOPE.SUPPORT_LCMA_OBS=true;
//Provide values required by each OBS operator
//Note: each OBS operator requirements are unique, see individual observation operator functions to see what's required 
//Note: no values required for any SUPPORT_*_OBS quantity set to false.

//GPP-specific variables//Assuming FLUXNET GPP is assumed to be Ag.
    //Wohlfahrt & Gu, 2015 
OBSOPE.GPP_flux=F.gpp;
// For constraint on emergent autotrophic fraction
OBSOPE.Rauto_flux=F.resp_auto;
//LAI-specific variables
OBSOPE.LAI_pool=S.D_LAI;
//ET variables
OBSOPE.ET_flux=F.ets;
//Runoff variables
static int ROFF_fluxes[4];
ROFF_fluxes[0]=F.q_ly1;
ROFF_fluxes[1]=F.q_ly2;
ROFF_fluxes[2]=F.q_ly3;
ROFF_fluxes[3]=F.q_surf;
OBSOPE.ROFF_fluxes=ROFF_fluxes;
static double ROFF_flux_signs[]={1.,1.,1.,1.};
OBSOPE.ROFF_flux_signs=ROFF_flux_signs;
OBSOPE.ROFF_n_fluxes=4;
//NBE-specific variables
static int NBE_fluxes[4];
NBE_fluxes[0]=F.gpp;
NBE_fluxes[1]=F.resp_auto;
NBE_fluxes[2]=F.rh_co2;
NBE_fluxes[3]=F.f_total;
OBSOPE.NBE_fluxes=NBE_fluxes;
static double NBE_flux_signs[]={-1.,1.,1.,1.};
OBSOPE.NBE_flux_signs=NBE_flux_signs;
OBSOPE.NBE_n_fluxes=4;

//ABGB-specific variables
static int ABGB_pools[4];
ABGB_pools[0]=S.C_lab;
ABGB_pools[1]=S.C_fol;
ABGB_pools[2]=S.C_roo;
ABGB_pools[3]=S.C_woo;
OBSOPE.ABGB_pools=ABGB_pools;
OBSOPE.ABGB_n_pools=4;


//SOM-specific variables
static int DOM_pools[3]; 
DOM_pools[0]=S.C_cwd;
DOM_pools[1]=S.C_lit;
DOM_pools[2]=S.C_som;
OBSOPE.DOM_pools=DOM_pools;
OBSOPE.DOM_n_pools=3;
//H2O-specific variables
static int EWT_h2o_pools[4];
EWT_h2o_pools[0]=S.H2O_LY1;
EWT_h2o_pools[1]=S.H2O_LY2;
EWT_h2o_pools[2]=S.H2O_LY3;
EWT_h2o_pools[3]=S.H2O_SWE;
OBSOPE.EWT_h2o_pools=EWT_h2o_pools;
OBSOPE.EWT_n_h2o_pools=4;
//Fire-specific variables
OBSOPE.FIR_flux=F.f_total;

//SCF-specific variables
OBSOPE.SCF_pool=S.D_SCF;


//Vcmax25 parameters
OBSOPE.Vcmax25_PARAM=P.Vcmax25;
// //C3frac parameters
// OBSOPE.C3frac_PARAM=P.C3_frac;
//Initial Snow parameter
OBSOPE.iniSnow_PARAM=P.i_SWE;
//Initial SOM parameter
OBSOPE.iniSOM_PARAM=P.i_som;
OBSOPE.LCMA_PARAM=P.LCMA;

//CH4-specific variables 
OBSOPE.CH4_flux = F.rh_ch4;

DALECmodel->OBSOPE=OBSOPE;



return 0;}