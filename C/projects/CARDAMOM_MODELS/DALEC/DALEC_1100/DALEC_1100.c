#pragma once
//Note: DALEC_OBSERVATION_OPERATORS.c included in DALEC_MODULE.
#include "../DALEC_ALL/DALEC_MODULE.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/DRAINAGE.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_EWT2MOI.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2EWT.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2CON.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2PSI.c"
#include "../DALEC_ALL/LIU_AN_ET_REFACTOR.c"
#include "../DALEC_ALL/CH4_MODULES/HET_RESP_RATES_JCR.c"
#include "../DALEC_ALL/KNORR_ALLOCATION.c"
#include "../DALEC_ALL/SOIL_TEMP_AND_LIQUID_FRAC.c"
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/ALLOC_AND_AUTO_RESP_FLUXES.c"


/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/

struct DALEC_1100_PARAMETERS{
/*DALEC PARAMETERS*/
int tr_lit2som;
int tr_cwd2som;
int rauto_mr;
int rauto_mr_q10;
int rauto_gr;
int t_wood;
int t_root;
int t_lit;
int t_cwd;
int t_som;
int Q10rhco2;
int LCMA;
int i_labile;
int i_foliar;
int i_root;
int i_wood;
int i_cwd;
int i_lit;
int i_som;
int retention;
int i_PAW;
int cf_foliar;
int cf_ligneous;
int cf_DOM;
int resilience;
int hydr_cond;
int max_infil;
int i_PUW;
int PAW_por;
int PUW_por;
int field_cap;
int PAW_z;//PAW depth
int PUW_z;//PUW depth
int PAW_vhc;//PAW volumetric heat capacity
int PUW_vhc;//PUW volumetric heat capacity
int Q_excess;
int Med_g1;
int Vcmax25;
int Tminmin;
int Tminmax;
int ga;
int Tupp;
int Tdown;
int clumping;
int leaf_refl;
int i_SWE;
int min_melt;
int melt_slope;
int scf_scalar;
int S_fv;
int thetas_opt;
int fwc;
int r_ch4;
int Q10ch4;
int maxPevap;
int T_phi;
int T_range;
int plgr;
int k_leaf;
int lambda_max;
int tau_W;
int time_c;
int time_r;
int init_T_mem;
int init_LAIW_mem;
int t_foliar;
int i_PAW_E;
int i_PUW_E;
int psi_50;
int beta_lgr;
int phi_RL;
int phi_WL;
} DALEC_1100_PARAMETERS={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,48,49,
    50,51,52,53,54,55,56,57,58,59,
    60,61,62,63,64,65,66,67,68,69,
    70,71
};

struct DALEC_1100_FLUXES{
/*DALEC FLUXES*/
int gpp;   /*GPP*/
int resp_auto;   /*Autotrophic respiration*/
int lab_prod;   /*Labile production*/
int foliar_prod;   /*Labile release*/
int root_prod;   /*Root production*/
int wood_prod;   /*Wood production*/
int fol2lit;   /*Foliar decomposition*/
int wood2cwd;   /*Wood decomposition*/
int root2lit;   /*Root decomposition*/
int cwd2som;   /*CWD decomposition*/
int lit2som;   /*Litter decomposition*/
int f_total;   /*Flux description*/
int f_lab;   /*Labile fire loss*/
int f_fol;   /*Foliar fire loss*/
int f_roo;   /*Wood fire loss*/
int f_woo;   /*Root fire loss*/
int f_cwd;   /*CWD fire loss*/
int f_lit;   /*Litter fire loss*/
int f_som;   /*Soil fire loss*/
int fx_lab2lit;   /*Fire transfer labile to litter*/
int fx_fol2lit;   /*Fire transfer foliar to litter*/
int fx_roo2lit;   /*Fire transfer root to litter*/
int fx_woo2cwd;   /*Fire transfer wood to CWD*/
int fx_cwd2som;   /*Fire transfer CWD to soil*/
int fx_lit2som;   /*Fire transfer litter to soil*/
int et;   /*Evapotranspiration*/
int q_paw;   /*PAW runoff*/
int paw2puw;   /*PAW->PUW transfer*/
int q_puw;   /*PUW runoff*/
int q_surf;   /*Surface runoff*/
int infil;   /*INFILTRATION*/
int infil_e;   /*INFILTRATION IE: temp = weight average of snow melt (0C) + liquid precip (@air temp)*/
int q_paw_e;   /*Q PAW IE: temp = PAW temp*/
int q_puw_e;   /*Q PUW IE: temp = PUW temp*/
int paw2puw_e;   /*PAW->PUW transfer IE: temp of donor*/
int et_e; /* See Retano's calculation*/
int transp;   /*Transpiration*/
int evap;   /*Evaporation*/
int snow_in;   /*Snowfall to SWE*/
int melt;   /*Snow melt*/
int ae_rh_cwd; /*Aerobic Rh from coarse woody debris*/
int ae_rh_lit; /*Aerobic Rh from litter*/
int ae_rh_som; /*aerobic Rh from SOM*/
int an_rh_cwd; /*anaerobic Rh from coarse woody debris*/
int an_rh_lit; /*anaerobic Rh from litter*/
int an_rh_som; /*anaerobic Rh from SOM*/
int rh_co2; /* */
int rh_ch4; /* */
int aetr;  /*aerobic turnover scalar*/
int antr; /*anaerobic turnover scalar*/
int an_co2_c_ratio; /*CO2 fraction in anaerobic C decomposition*/
int an_ch4_c_ratio; /*CH4 fraction in anaerobic C decomposition*/
int target_LAI;   /*LAI environmental target*/
int dlambda_dt;   /*dLAI/dt*/
int f_temp_thresh;   /*f_temp_thres*/
int f_dayl_thresh;   /*f_dayl_thres*/
int lai_fire;   /*LAI fire loss*/
int foliar_fire_frac;   /*C_fol fire loss frac*/
int net_radiation; /*Net radiation flux*/
int latent_heat; /*latent heat flux*/
int sensible_heat; /*sensible heat flux*/
int ground_heat; /*ground heat flux*/
int gh_in; /*ground heat flux in converted units*/
int resp_auto_growth; /*autotrophic growth respiration*/
int resp_auto_maint; /*autotrophic maintenance respiration*/
} DALEC_1100_FLUXES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,48,49,
    50,51,52,53,54,55,56,57,58,59,
    60,61,62,63,64
};



/*Prognostic states and Diagnostic states (dependent on other states)*/


struct DALEC_1100_POOLS{
/*DALEC POOLS*/
int C_lab; /*Labile C*/
int C_fol; /*Foliar C*/
int C_roo; /*Root C*/
int C_woo; /*Wood C*/
int C_cwd; /*Coarse woody debris C*/
int C_lit; /*Litter C*/
int C_som; /*Soil C*/
int H2O_PAW; /*Plant available H2O*/
int H2O_PUW; /*Plant unavailable H2O*/
int H2O_SWE; /*Snow water equivalent*/
int E_PAW; /*PAW thermal energy state*/
int E_PUW; /*PUW thermal energy state*/
int D_LAI;//leaf area index
int D_SCF;//snow-covered fraction
int D_TEMP_PAW;//PAW temp
int D_TEMP_PUW;//PUW temp
int D_LF_PAW;//PAW liquid h2o frac
int D_LF_PUW;//PUW liquid h2o frac
int D_SM_PAW;//PAW soil moisture
int D_SM_PUW;//PUW soil moisture
int M_LAI_MAX;//KNORR LAI module max LAI memory
int M_LAI_TEMP;//KNORR LAI module temp memory
} DALEC_1100_POOLS={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21
};

/*
//Returns structure with sources and sinks, matches number of fluxes
DALEC_FLUX_SOURCES_SINKS_STRUCT DALEC_1100_FLUX_SOURCES_SINKS(DALEC * DALECmodel){
    //Step 1. Declare & initialize
    DALEC_FLUX_SOURCES_SINKS_STRUCT FLUXFLOWS;
    // external source or pool sink, or not conserved quantity
    //Default = -1 
   
    FLUXFLOWS.SOURCE=calloc(DALECmodel->nofluxes, sizeof(int));
    FLUXFLOWS.SINK=calloc(DALECmodel->nofluxes, sizeof(int));
    
    for (n=0;n<DALECmodel->nofluxes; n++){FLUXFLOWS.SOURCE[n]=-1;FLUXFLOWS.SINK[n]=-1;}
    
    
    //Step 2. Define
    
        // C_lab
        FLUXFLOWS.SINK[F.lab_prod]=P.C_lab;
        FLUXFLOWS.SOURCE[F.foliar_prod]=P.C_lab;
        FLUXFLOWS.SOURCE[F.root_prod]=P.C_lab;
        FLUXFLOWS.SOURCE[F.wood_prod]=P.C_lab;
        FLUXFLOWS.SOURCE[F.f_lab]=P.C_lab;  
        FLUXFLOWS.SOURCE[F.fx_lab2lit]=P.C_lab;


        // C_fol
        FLUXFLOWS.SINK[F.foliar_prod]=P.C_fol;
        FLUXFLOWS.SOURCE[F.fol2lit]=P.C_fol;
        FLUXFLOWS.SOURCE[F.f_fol]=P.C_fol;
        FLUXFLOWS.SOURCE[F.fx_fol2lit]=P.C_fol;
        
        // C_roo
        FLUXFLOWS.SINK[F.root_prod]=P.C_roo;
        FLUXFLOWS.SOURCE[F.roo2lit]=P.C_roo;
        FLUXFLOWS.SOURCE[F.f_roo]=P.C_roo;
        FLUXFLOWS.SOURCE[F.fx_roo2lit]=P.C_roo;
        
        // C_woo
        FLUXFLOWS.SINK[F.wood_prod]=P.C_woo;
        FLUXFLOWS.SOURCE[F.woo2cwd]=P.C_woo;
        FLUXFLOWS.SOURCE[F.f_woo]=P.C_woo;
        FLUXFLOWS.SOURCE[F.fx_woo2cwd]=P.C_woo;

        
        // C_lit
        FLUXFLOWS.SINK[F.fx_lab2lit]=P.C_lit;
        FLUXFLOWS.SINK[F.fol2lit]=P.C_lit;
        FLUXFLOWS.SINK[F.fx_fol2lit]=P.C_lit;
        FLUXFLOWS.SINK[F.roo2lit]=P.C_lit;
        FLUXFLOWS.SINK[F.fx_roo2lit]=P.C_lit;
        FLUXFLOWS.SOURCE[F.ae_rh_lit]=P.C_lit;
        FLUXFLOWS.SOURCE[F.an_rh_lit]=P.C_lit;
        FLUXFLOWS.SOURCE[F.f_lit]=P.C_lit;
        FLUXFLOWS.SOURCE[F.lit2som]=P.C_lit;
        FLUXFLOWS.SOURCE[F.fx_lit2som]=P.C_lit;
    
        // C_cwd
        FLUXFLOWS.SINK[F.woo2cwd]=P.C_cwd;
        FLUXFLOWS.SINK[F.fx_woo2cwd]=P.C_cwd;
        FLUXFLOWS.SOURCE[F.ae_rh_cwd]=P.C_cwd;
        FLUXFLOWS.SOURCE[F.an_rh_cwd]=P.C_cwd;
        FLUXFLOWS.SOURCE[F.f_cwd]=P.C_cwd;
        FLUXFLOWS.SOURCE[F.cwd2som]=P.C_cwd;
        FLUXFLOWS.SOURCE[F.fx_cwd2som]=P.C_cwd;
        
        
        // C_som
        FLUXFLOWS.SINK[F.cwd2som]=P.C_som;
        FLUXFLOWS.SINK[F.fx_cwd2som]=P.C_som;
        FLUXFLOWS.SINK[F.lit2som]=P.C_som;
        FLUXFLOWS.SINK[F.fx_lit2som]=P.C_som;
        FLUXFLOWS.SOURCE[F.ae_rh_som]=P.C_som;
        FLUXFLOWS.SOURCE[F.an_rh_som]=P.C_som;
        FLUXFLOWS.SOURCE[F.f_som]=P.C_som;

        // H2O_SWE
        FLUXFLOWS.SOURCE[F.melt]=P.H2O_SWE;
        FLUXFLOWS.SINK[F.snowfall]=P.H2O_SWE;

        // H2O_PAW
        FLUXFLOWS.SINK[F.infil]=P.H2O_PAW;
        FLUXFLOWS.SOURCE[F.et]=P.H2O_PAW;
        FLUXFLOWS.SOURCE[F.paw2puw]=P.H2O_PAW;
        FLUXFLOWS.SOURCE[F.q_paw]=P.H2O_PAW;

        // H2O_PUW        
        FLUXFLOWS.SOURCE[F.q_puw]=P.H2O_PUW;
        FLUXFLOWS.SINK[F.paw2puw]=P.H2O_PUW;

        // E_PAW
        FLUXFLOWS.SINK[F.ground_heat]=P.E_PAW;
        FLUXFLOWS.SINK[F.infil_e]=P.E_PAW;
        FLUXFLOWS.SOURCE[F.et_e]=P.E_PAW;
        FLUXFLOWS.SOURCE[F.q_paw_e]=P.E_PAW;
        FLUXFLOWS.SOURCE[F.paw2puw_e]=P.E_PAW;

        // E_PUW
        FLUXFLOWS.SINK[F.paw2puw_e]=P.E_PUW;
        FLUXFLOWS.SOURCE[F.q_puw_e]=P.E_PUW;
        }

*/

/*
struct POOLS_INFO{
int n_input_fluxes
int n_output_fluxes
int * input_fluxes
int * output_fluxes}
*/


struct DALEC_1100_EDCs{
int litcwdtor;
int cwdsomtor;
} DALEC_1100_EDCs={
    0,1
};



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

double deltat=DATA.ncdf_data.TIME_INDEX.values[1] - DATA.ncdf_data.TIME_INDEX.values[0];
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
  POOLS[S.H2O_PAW]=pars[P.i_PAW];
  POOLS[S.H2O_PUW]=pars[P.i_PUW];
  POOLS[S.H2O_SWE]=pars[P.i_SWE];
  /*Energy pools*/
  POOLS[S.E_PAW]=pars[P.i_PAW_E];
  POOLS[S.E_PUW]=pars[P.i_PUW_E];
  
  
   //---INITIALIZING DIAGNOSTIC STATES---
    POOLS[S.D_LAI]=POOLS[S.C_fol]/pars[P.LCMA]; //LAI
    POOLS[S.D_SCF]=POOLS[S.H2O_SWE]/(POOLS[S.H2O_SWE]+pars[P.scf_scalar]); //snow cover fraction
    
    
    //INITIALIZING PAW and PUW soil moisture
        POOLS[S.D_SM_PAW]=HYDROFUN_EWT2MOI(POOLS[S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]); //soil moisture PAW
        POOLS[S.D_SM_PUW]=HYDROFUN_EWT2MOI(POOLS[S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]);//soil moisture PUW

        
        
    //Declare stryct
    SOIL_TEMP_AND_LIQUID_FRAC_STRUCT PAWSOILTEMP, PUWSOILTEMP;
  //Populate with run-specific constrants
    //PAW
    PAWSOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.PAW_vhc]; ;//J/m3/K
    PAWSOILTEMP.IN.depth = pars[P.PAW_z];//m 
    PAWSOILTEMP.IN.soil_water = POOLS[S.H2O_PAW];//mm (or kg/m2)
    PAWSOILTEMP.IN.internal_energy = POOLS[S.E_PAW];//Joules
    //Pass pointer to function 
    SOIL_TEMP_AND_LIQUID_FRAC(&PAWSOILTEMP);  
        //Store outputs 
    POOLS[S.D_TEMP_PAW]=PAWSOILTEMP.OUT.TEMP;    
    POOLS[S.D_LF_PAW]=PAWSOILTEMP.OUT.LF;

    //PUW
    PUWSOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.PUW_vhc]; ;//J/m3/K
    PUWSOILTEMP.IN.depth = pars[P.PUW_z];//m 
    PUWSOILTEMP.IN.soil_water = POOLS[S.H2O_PUW];//mm (or kg/m2)
    PUWSOILTEMP.IN.internal_energy = POOLS[S.E_PUW];//Joules
    //Pass pointer to function 
    SOIL_TEMP_AND_LIQUID_FRAC(&PUWSOILTEMP);
    //Store outputs 
    POOLS[S.D_TEMP_PUW]=PUWSOILTEMP.OUT.TEMP;    
    POOLS[S.D_LF_PUW]=PUWSOILTEMP.OUT.LF;

    
    
    
    
    //******************Delcare KNORR STRUCT*********************
    KNORR_ALLOCATION_STRUCT KNORR;
//define time-invariant parameters
         KNORR.IN.deltat=deltat;
         KNORR.IN.latitude=DATA.ncdf_data.LAT;
         KNORR.IN.T_phi=pars[P.T_phi];
         KNORR.IN.T_r=pars[P.T_range];
         KNORR.IN.plgr=pars[P.plgr];
         KNORR.IN.k_L=pars[P.k_leaf];
         KNORR.IN.tau_W=pars[P.tau_W];
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
//     double   PAW_SM;//m3/m3
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
        ARFLUXES.IN.mr=pars[P.rauto_mr];//
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





double meantemp = (DATA.ncdf_data.T2M_MAX.reference_mean + DATA.ncdf_data.T2M_MIN.reference_mean)/2;

// Porosity scaling factor (see line 124 of HESS paper)
double psi_porosity = -0.117/100;

/*Combustion factors*/
double CF[7];//AAB changed this
CF[S.C_lab]=pars[P.cf_ligneous];
CF[S.C_fol]=pars[P.cf_foliar];
CF[S.C_roo]=pars[P.cf_ligneous];
CF[S.C_woo]=pars[P.cf_ligneous];
CF[S.C_cwd]=pars[P.cf_ligneous];
CF[S.C_lit]=pars[P.cf_foliar]/2+pars[P.cf_ligneous]/2;
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
     
        
/*Calculate light extinction coefficient*/
double B = (DOY[n]-81)*2*pi/365.;
double ET1 = 9.87*sin(2*B)-7.53*cos(B)-1.5*sin(B);
double DA = 23.45*sin((284+DOY[n])*2*pi/365); //Deviation angle
double LST = (int) (DOY[n]*24*60) % (24*60);
LST=0.5*24*60;
double AST = LST+ET1;
double h = (AST-12*60)/4; //hour angle
double alpha = asin((sin(pi/180*DATA.ncdf_data.LAT)*sin(pi/180*DA)+cos(pi/180*DATA.ncdf_data.LAT)*cos(pi/180.*DA)*cos(pi/180*h)))*180/pi; //solar altitude
double zenith_angle = 90-alpha;

//printf("SZA local = %2.2f, SZA global = %2.2f, SZA diff = %2.2f\n", zenith_angle,DATA.ncdf_data.SZA.values,DATA.ncdf_data.SZA.values - zenith_angle);
//double LAD = 1.0; //leaf angle distribution
//double VegK = sqrt(pow(LAD,2)+ pow(tan(zenith_angle/180*pi),2))/(LAD+1.774*pow((1+1.182),-0.733)); //Campbell and Norman 1998

double LAD = 0.5; //leaf angle distribution
double VegK = LAD/cos(zenith_angle/180*pi);

/*Temp scaling factor*/
double g;
int Tminmin = pars[P.Tminmin] - 273.15; 
int Tminmax = pars[P.Tminmax] - 273.15;
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
double sm_PAW0 = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]);
double psi_PAW0 = HYDROFUN_MOI2PSI(sm_PAW0,psi_porosity,pars[P.retention]);
double beta = 1/(1 + exp(pars[P.beta_lgr]*(-1*psi_PAW0/pars[P.psi_50] - 1)));
       beta = fmin(beta,g);


//******************Declare LIU STRUCT*********************
LIU_AN_ET_STRUCT LIU;

//define time-invariant parameters
LIU.IN.SRAD=SSRD[n]*1e6/(24*3600);
LIU.IN.VPD=VPD[n]/10;
LIU.IN.TEMP=273.15+0.5*(T2M_MIN[n]+T2M_MAX[n]);  
LIU.IN.vcmax25=pars[P.Vcmax25];
LIU.IN.co2=CO2[n];
LIU.IN.beta_factor=beta;
LIU.IN.g1=pars[P.Med_g1];
LIU.IN.LAI=LAI;
LIU.IN.ga=pars[P.ga];
LIU.IN.VegK=VegK;
LIU.IN.Tupp=pars[P.Tupp];
LIU.IN.Tdown=pars[P.Tdown];
LIU.IN.C3_frac=1., // pars[P.C3_frac]
LIU.IN.clumping=pars[P.clumping];
LIU.IN.leaf_refl=pars[P.leaf_refl];
LIU.IN.maxPevap=pars[P.maxPevap];
LIU.IN.precip=PREC[n];

//Call function: uses LIU->IN to update LIU->OUT
LIU_AN_ET(&LIU);

// GPP
FLUXES[f+F.gpp] = LIU.OUT.An;
//transpiration//
FLUXES[f+F.transp] = LIU.OUT.transp;
//evaporation//
FLUXES[f+F.evap] = LIU.OUT.evap;
// Evapotranspiration
FLUXES[f+F.et]=FLUXES[f+F.evap]+FLUXES[f+F.transp];


/*Snow water equivalent*/
FLUXES[f+F.snow_in] = SNOWFALL[n];
POOLS[nxp+S.H2O_SWE]=POOLS[p+S.H2O_SWE]+FLUXES[f+F.snow_in]*deltat; /*first step snowfall to SWE*/
FLUXES[f+F.melt]=fmin(fmax(((T2M_MIN[n]+T2M_MAX[n])/2-(pars[P.min_melt]-273.15))*pars[P.melt_slope],0),1)*POOLS[nxp+S.H2O_SWE]/deltat; /*melted snow per day*/  
POOLS[nxp+S.H2O_SWE]=POOLS[nxp+S.H2O_SWE]-FLUXES[f+F.melt]*deltat; /*second step remove snowmelt from SWE*/

//Energy balance: Rn = LE + H - G
// Rn = SWin - SWout + LWin - LWout
double SWin = SSRD[n]*1e6/(24*3600); // W m-2
//Weighted average of surface albedo considering SW snow albedo as 0.9
double snow_albedo=0.9;//Consider age-dependent albedo.
double SWout = (1. - POOLS[p+S.D_SCF])*(SWin*pars[P.leaf_refl]) + POOLS[p+S.D_SCF]*(SWin*snow_albedo); // W m-2
//Stefan-Boltzmann constant W.m-2.K-4
double sigma = 5.67*1e-8;
//reference air temperature
double ref_temp = 273.15+0.5*(T2M_MIN[n]+T2M_MAX[n]);
//Incident LW radiation - calculated
//double LWin = sigma*pow(ref_temp,4.);
double LWin = STRD[n]*1e6/(24*3600); // W m-2
//Outgoing LW radiation
double tskin_k = SKT[n]+273.15;
double LWout = sigma*pow(tskin_k,4.); // W m-2
//Net radiation at the top of the canopy
double Rn = SWin - SWout + LWin - LWout; // W m-2
FLUXES[f+F.net_radiation] = Rn; // W m-2
//Latent heat of Vaporization J kg-1 
double lambda = DGCM_LATENT_HEAT_VAPORIZATION; //2.501*1e6 J kg-1 
//Latente heat (W.m-2)
double LE = lambda*FLUXES[f+F.et]/(24*60*60); // W m-2
FLUXES[f+F.latent_heat] = LE; // W m-2
//specific heat capacity of dry air is 1.00464 KJ kg -1 K -1
double cp = 29.2; // J mol-1 K-1 representative specific heat of moist air at const pressure from Bonan book
//Sensible heat 
double H = cp*(tskin_k - ref_temp)*pars[P.ga]; // ga in mol m-2 s-1 ??????
FLUXES[f+F.sensible_heat] = H; // W m-2
//soil heat flux 
double G = Rn - H - LE; // W m-2
FLUXES[f+F.ground_heat] = G; // W m-2
FLUXES[f+F.gh_in] = G*60*60*24; // J m-2 d-1

// Infiltration (mm/day)
double liquid_in = (PREC[n] - SNOWFALL[n] + FLUXES[f+F.melt]);
FLUXES[f+F.infil] = pars[P.max_infil]*(1 - exp(-liquid_in/pars[P.max_infil]));

// Surface runoff (mm/day)
FLUXES[f+F.q_surf] = liquid_in - FLUXES[f+F.infil];

// Volumetric soil moisture from water pools
// Include infiltration into PAW ()
double sm_PAW = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]);
double sm_PUW = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);

// Update PAW SM with infiltration
//sm_PAW += HYDROFUN_EWT2MOI(infil*deltat,pars[P.PAW_por],pars[P.PAW_z]);

// Calculate drainage
double drain_PAW = POOLS[p+S.D_LF_PAW]*DRAINAGE(sm_PAW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);
double drain_PUW = POOLS[p+S.D_LF_PUW]*DRAINAGE(sm_PUW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);

// Drainage becomes runoff from pools
FLUXES[f+F.q_paw] = HYDROFUN_MOI2EWT(drain_PAW,pars[P.PAW_por],pars[P.PAW_z])/deltat;
FLUXES[f+F.q_puw] = HYDROFUN_MOI2EWT(drain_PUW,pars[P.PUW_por],pars[P.PUW_z])/deltat;

// Remove drainage from layers
sm_PAW -= drain_PAW;
sm_PUW -= drain_PUW;

// Convert to conductivity
double k_PAW = HYDROFUN_MOI2CON(sm_PAW,pars[P.hydr_cond],pars[P.retention]);
double k_PUW = HYDROFUN_MOI2CON(sm_PUW,pars[P.hydr_cond],pars[P.retention]);

// Convert to potential
double psi_PAW = HYDROFUN_MOI2PSI(sm_PAW,psi_porosity,pars[P.retention]);
double psi_PUW = HYDROFUN_MOI2PSI(sm_PUW,psi_porosity,pars[P.retention]);

//Assumes PAW->PUW, so PAW LF & TEMP; if reverse flow, then switches to PUW LF & TEMP.
double LFxfer=POOLS[p+S.D_LF_PAW]; 
double TEMPxfer= POOLS[p+S.D_TEMP_PAW];

// Calculate inter-pool transfer in m/s (positive is PAW to PUW)
double pot_xfer = 1000 * sqrt(k_PAW*k_PUW) * (1000*(psi_PAW-psi_PUW)/(9.8*0.5*(pars[P.PAW_z]+pars[P.PUW_z])) + 1);
if (pot_xfer<0) {LFxfer=POOLS[p+S.D_LF_PUW];TEMPxfer= POOLS[p+S.D_TEMP_PUW];}

// Transfer flux in mm/day
//scale with donor pool LF
FLUXES[f+F.paw2puw] = pot_xfer*1000*3600*24*LFxfer;

// Update pools, including ET from PAW
POOLS[nxp+S.H2O_PAW] = POOLS[p+S.H2O_PAW] + (FLUXES[f+F.infil] - FLUXES[f+F.paw2puw] - FLUXES[f+F.q_paw] - FLUXES[f+F.et])*deltat;
POOLS[nxp+S.H2O_PUW] = POOLS[p+S.H2O_PUW] + (FLUXES[f+F.paw2puw] - FLUXES[f+F.q_puw])*deltat;



//**********INTERNAL ENERGT FLUXES FOR ALL H2O FLUXES***************
//Add INFILTRATION, PAW, PUW, PAW2PUW, ET
double infiltemp = (T2M_MAX[n] + T2M_MIN[n])*0.5*(PREC[n] - SNOWFALL[n])/(PREC[n] - SNOWFALL[n] + FLUXES[f+F.melt]) ;//snowmelt temp = 0, so term multiplied by zero in weighted average 


//All energy fluxes
FLUXES[F.infil_e] = FLUXES[F.infil]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(infiltemp + 273.15);
FLUXES[F.et_e] = FLUXES[f+F.et]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(SKT[n] + 273.15);
FLUXES[F.paw2puw_e] = FLUXES[f+F.paw2puw]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(TEMPxfer + 273.15);
FLUXES[F.q_paw_e] = FLUXES[f+F.q_paw]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_PAW] + 273.15);
FLUXES[F.q_puw_e] =  FLUXES[f+F.q_puw]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_PUW] + 273.15);

//Energy states
//fraction of water in soil that is available 
//double frac_paw = POOLS[nxp+S.H2O_PAW]/(POOLS[nxp+S.H2O_PAW]+POOLS[nxp+S.H2O_PUW]);
POOLS[nxp+S.E_PAW] = POOLS[p+S.E_PAW] + (FLUXES[f+F.gh_in] + FLUXES[F.infil_e] - FLUXES[F.et_e] - FLUXES[F.paw2puw_e] - FLUXES[F.q_paw_e])*deltat;  
POOLS[nxp+S.E_PUW] = POOLS[p+S.E_PUW] + (FLUXES[F.paw2puw_e] - FLUXES[F.q_puw_e] )*deltat; 



//*************KNORR LAI**************


//Time varying KNORR function terms

KNORR.IN.lambda_max_memory=  POOLS[p+S.M_LAI_MAX];
KNORR.IN.T_memory=POOLS[p+S.M_LAI_TEMP];
KNORR.IN.temp=(T2M_MAX[n]+T2M_MIN[n])*0.5;
KNORR.IN.DOY=DOY[n];
KNORR.IN.lambda=LAI;
KNORR.IN.pasm=(POOLS[p+S.H2O_PAW]+POOLS[nxp+S.H2O_PAW])/2.0;//Note: soil moisture also available here
KNORR.IN.ET= FLUXES[f+F.et];
//Call function: uses KNORR->IN to update KNORR->OUT
KNORR_ALLOCATION(&KNORR);
 
FLUXES[f+F.target_LAI]=KNORR.OUT.lambda_next ;
FLUXES[f+F.dlambda_dt]=KNORR.OUT.dlambdadt;
FLUXES[f+F.f_temp_thresh]= KNORR.OUT.f_T;
FLUXES[f+F.f_dayl_thresh]= KNORR.OUT.f_d;

//Update KNORR memory variables for next iteration
POOLS[nxp+S.M_LAI_MAX]=KNORR.OUT.laim;
POOLS[nxp+S.M_LAI_TEMP]=KNORR.OUT.T;

    
//************Allocation*******************

ARFLUXES.IN.deltat=deltat;
ARFLUXES.IN.GPP=FLUXES[f+F.gpp];
ARFLUXES.IN.TEMP=(T2M_MIN[n]+T2M_MAX[n])*0.5;
ARFLUXES.IN.C_LIVE=POOLS[p+S.C_fol]+POOLS[p+S.C_woo]+POOLS[p+S.C_roo];
// Potential plant allocation (growth) fluxes
ARFLUXES.IN.ALLOC_FOL_POT=fmax(0, (FLUXES[f+F.target_LAI] * pars[P.LCMA]) - POOLS[p+S.C_fol]);
ARFLUXES.IN.ALLOC_ROO_POT=fmax(0, (pars[P.phi_RL] * FLUXES[f+F.target_LAI] * pars[P.LCMA]) - POOLS[p+S.C_roo]);
ARFLUXES.IN.ALLOC_WOO_POT=fmax(0, (pars[P.phi_WL] * FLUXES[f+F.target_LAI] * pars[P.LCMA]) - POOLS[p+S.C_woo]);

ALLOC_AND_AUTO_RESP_FLUXES(&ARFLUXES);

/*respiration auto*/
FLUXES[f+F.resp_auto]=ARFLUXES.OUT.AUTO_RESP_TOTAL;
/*growth respiration*/
FLUXES[f+F.resp_auto_growth]=ARFLUXES.OUT.AUTO_RESP_GROWTH;
/*maintenance respiration*/
FLUXES[f+F.resp_auto_maint]=ARFLUXES.OUT.AUTO_RESP_MAINTENANCE;

// Fcfolavailable=FLUXES[f+F.lab_prod] + POOLS[p+S.C_lab]/deltat;
if (FLUXES[f+F.dlambda_dt] > 0){
  FLUXES[f+F.fol2lit]=POOLS[p+S.C_fol]*(1-pow(1-pars[P.t_foliar],deltat))/deltat;
}
else {
  FLUXES[f+F.fol2lit]=-FLUXES[f+F.dlambda_dt]*pars[P.LCMA]+POOLS[p+S.C_fol]*(1-pow(1-pars[P.t_foliar],deltat))/deltat;
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
/*wood CWD production*/       
FLUXES[f+F.wood2cwd] = POOLS[p+S.C_woo]*(1-pow(1-pars[P.t_wood],deltat))/deltat;
/*root litter production*/
FLUXES[f+F.root2lit] = POOLS[p+S.C_roo]*(1-pow(1-pars[P.t_root],deltat))/deltat;

/*-----------------------------------------------------------------------*/

  
    //TIME-VARYING INPUTS
     HRJCR.IN.SM=POOLS[p+S.D_SM_PAW];
     HRJCR.IN.TEMP=SKT[n];
        //JCR
        HET_RESP_RATES_JCR(&HRJCR);
             //OUtputs --- store anything we want here---
            FLUXES[f+F.antr]=HRJCR.OUT.aerobic_tr;//Aerobic turnover rate scalar
       FLUXES[f+F.aetr]=HRJCR.OUT.anaerobic_tr;//Anaerobic turnover rate scalar
       FLUXES[f+F.an_co2_c_ratio]=HRJCR.OUT.anaerobic_co2_c_ratio;//CO2_C_ratio
        FLUXES[f+F.an_ch4_c_ratio]=HRJCR.OUT.anaerobic_ch4_c_ratio;//CH4_C_ratio



//outputformat
//jcr_o 0-3 fT,fV,fW,fCH4; /*jc*/ /* output from JCR module */
double ae_loss_cwd = POOLS[p+S.C_cwd]*(1-pow(1-HRJCR.OUT.aerobic_tr*pars[P.t_cwd],deltat))/deltat;
/* aerobic Rh from coarse woody debris*/
FLUXES[f+F.ae_rh_cwd] = ae_loss_cwd*(1-pars[P.tr_cwd2som]);
double ae_loss_lit = POOLS[p+S.C_lit]*(1-pow(1-HRJCR.OUT.aerobic_tr*pars[P.t_lit],deltat))/deltat;
/* aerobic Rh from litter*/
FLUXES[f+F.ae_rh_lit] = ae_loss_lit*(1-pars[P.tr_lit2som]);
/* aerobic Rh from SOM*/
FLUXES[f+F.ae_rh_som] = POOLS[p+S.C_som]*(1-pow(1-HRJCR.OUT.aerobic_tr*pars[P.t_som],deltat))/deltat;

//******Anaerobic fluxes
double an_loss_cwd = POOLS[p+S.C_cwd]*(1-pow(1-HRJCR.OUT.anaerobic_tr*pars[P.t_cwd],deltat))/deltat;
/* anaerobic Rh from coarse woody debris*/
FLUXES[f+F.an_rh_cwd] = an_loss_cwd*(1-pars[P.tr_cwd2som]);
/* anaerobic Rh from litter*/
double an_loss_lit = POOLS[p+S.C_lit]*(1-pow(1-HRJCR.OUT.anaerobic_tr*pars[P.t_lit],deltat))/deltat;
FLUXES[f+F.an_rh_lit] = an_loss_lit*(1-pars[P.tr_lit2som]);
/* anaerobic Rh from SOM*/
FLUXES[f+F.an_rh_som] = POOLS[p+S.C_som]*(1-pow(1-HRJCR.OUT.anaerobic_tr*pars[P.t_som],deltat))/deltat;
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

        POOLS[nxp+S.C_lab] = POOLS[p+S.C_lab] + (FLUXES[f+F.lab_prod]-FLUXES[f+F.foliar_prod]-FLUXES[f+F.root_prod]-FLUXES[f+F.wood_prod]-FLUXES[f+F.resp_auto_growth])*deltat;
        POOLS[nxp+S.C_fol] = POOLS[p+S.C_fol] + (FLUXES[f+F.foliar_prod] - FLUXES[f+F.fol2lit])*deltat;
        POOLS[nxp+S.C_roo] = POOLS[p+S.C_roo] + (FLUXES[f+F.root_prod] - FLUXES[f+F.root2lit])*deltat;
        POOLS[nxp+S.C_woo] = POOLS[p+S.C_woo] + (FLUXES[f+F.wood_prod] - FLUXES[f+F.wood2cwd])*deltat;
        POOLS[nxp+S.C_cwd] = POOLS[p+S.C_cwd] + (FLUXES[f+F.wood2cwd] - FLUXES[f+F.ae_rh_cwd]-FLUXES[f+F.an_rh_cwd]-FLUXES[f+F.cwd2som])*deltat;
        POOLS[nxp+S.C_lit] = POOLS[p+S.C_lit] + (FLUXES[f+F.fol2lit] + FLUXES[f+F.root2lit] - FLUXES[f+F.ae_rh_lit] - FLUXES[f+F.an_rh_lit] - FLUXES[f+F.lit2som])*deltat;
        POOLS[nxp+S.C_som] = POOLS[p+S.C_som] + (FLUXES[f+F.lit2som] - FLUXES[f+F.ae_rh_som] - FLUXES[f+F.an_rh_som] + FLUXES[f+F.cwd2som])*deltat;

        
        //Energy ppols
        //Root zone
          //POOLS[nxp+S.E_PAW] = POOLS[p+S.E_PAW] + (Renato's ground heat flux, inc LWup, etc.)*deltaT  - Runoff terms - Marcos' evaporation terms + Precip energ
        
        //POOLS[nxp+S.E_PUW] = POOLS[p+S.E_PAW] + (Renato's ground heat flux)*deltaT 



	/*total pool transfers - WITH FIRES*/
	/*first fluxes*/

	/*CFF = Combusted C fire flux
	NCFF = Non-combusted C fire flux*/

	/*Calculating all fire transfers (1. combustion, and 2. litter transfer)*/
	/*note: all fluxes are in gC m-2 day-1*/

    FLUXES[f+F.f_lab] = POOLS[nxp+S.C_lab]*BURNED_AREA[n]*CF[S.C_lab]/deltat;
    FLUXES[f+F.f_fol] = POOLS[nxp+S.C_fol]*BURNED_AREA[n]*CF[S.C_fol]/deltat;
    FLUXES[f+F.f_roo] = POOLS[nxp+S.C_roo]*BURNED_AREA[n]*CF[S.C_roo]/deltat;
    FLUXES[f+F.f_woo] = POOLS[nxp+S.C_woo]*BURNED_AREA[n]*CF[S.C_woo]/deltat;
    FLUXES[f+F.f_cwd] = POOLS[nxp+S.C_cwd]*BURNED_AREA[n]*CF[S.C_cwd]/deltat;
    FLUXES[f+F.f_lit] = POOLS[nxp+S.C_lit]*BURNED_AREA[n]*CF[S.C_lit]/deltat;
    FLUXES[f+F.f_som] = POOLS[nxp+S.C_som]*BURNED_AREA[n]*CF[S.C_som]/deltat;

    FLUXES[f+F.fx_lab2lit] = POOLS[nxp+S.C_lab]*BURNED_AREA[n]*(1-CF[S.C_lab])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_fol2lit] = POOLS[nxp+S.C_fol]*BURNED_AREA[n]*(1-CF[S.C_fol])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_roo2lit] = POOLS[nxp+S.C_roo]*BURNED_AREA[n]*(1-CF[S.C_roo])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_woo2cwd] = POOLS[nxp+S.C_woo]*BURNED_AREA[n]*(1-CF[S.C_woo])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_cwd2som] = POOLS[nxp+S.C_cwd]*BURNED_AREA[n]*(1-CF[S.C_cwd])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_lit2som] = POOLS[nxp+S.C_lit]*BURNED_AREA[n]*(1-CF[S.C_lit])*(1-pars[P.resilience])/deltat;


	/*Adding all fire pool transfers here*/
	/*live C pools*/	
    POOLS[nxp+S.C_lab] = POOLS[nxp+S.C_lab]-(FLUXES[f+F.f_lab]+FLUXES[f+F.fx_lab2lit])*deltat;
    POOLS[nxp+S.C_fol] = POOLS[nxp+S.C_fol]-(FLUXES[f+F.f_fol]+FLUXES[f+F.fx_fol2lit])*deltat;
    POOLS[nxp+S.C_roo] = POOLS[nxp+S.C_roo]-(FLUXES[f+F.f_roo]+FLUXES[f+F.fx_roo2lit])*deltat;
    POOLS[nxp+S.C_woo] = POOLS[nxp+S.C_woo]-(FLUXES[f+F.f_woo]+FLUXES[f+F.fx_woo2cwd])*deltat;
	/*dead C pools*/
    /*CWD*/
    POOLS[nxp+S.C_cwd] = POOLS[nxp+S.C_cwd]+(FLUXES[f+F.fx_woo2cwd]-FLUXES[f+F.f_cwd]-FLUXES[f+F.fx_cwd2som])*deltat;
    /*litter*/
    POOLS[nxp+S.C_lit] = POOLS[nxp+S.C_lit]+(FLUXES[f+F.fx_lab2lit]+FLUXES[f+F.fx_fol2lit]+FLUXES[f+F.fx_roo2lit]-FLUXES[f+F.f_lit]-FLUXES[f+F.fx_lit2som])*deltat;
	/*som*/
	POOLS[nxp+S.C_som] = POOLS[nxp+S.C_som]+(FLUXES[f+F.fx_cwd2som]+FLUXES[f+F.fx_lit2som]-FLUXES[f+F.f_som])*deltat;

	/*fires - total flux in gC m-2 day-1*/
	/*this term is now (essentially) obsolete*/
	/*replace in next version of DALEC_FIRES*/
    FLUXES[f+F.f_total] = FLUXES[f+F.f_lab] + FLUXES[f+F.f_fol] + FLUXES[f+F.f_roo] + FLUXES[f+F.f_woo] + FLUXES[f+F.f_cwd] + FLUXES[f+F.f_lit] + FLUXES[f+F.f_som];

    /*Fraction of C-foliar lost due to fires*/
    FLUXES[f+F.foliar_fire_frac] = BURNED_AREA[n]*(CF[S.C_lab] + (1-CF[S.C_lab])*(1-pars[P.resilience]));
    /*Calculate LAI (lambda) lost due to fire
      - we lose the same fraction of LAI as we do C-foliar 
      - FE_\Lambda^{(t+1)} = \Lambda^{(t+1)'} * BA ( k_{factor(i)} + (1 - k_{factor(i)}) r )*/
    FLUXES[f+F.lai_fire] = (POOLS[p+S.C_fol]/pars[P.LCMA])*BURNED_AREA[n]*(CF[S.C_lab] + (1-CF[S.C_lab])*(1-pars[P.resilience]));

    /****************************RECORD t+1 DIAGNOSTIC STATES*************************/
    POOLS[nxp+S.D_LAI]=POOLS[nxp+S.C_fol]/pars[P.LCMA]; //LAI
    POOLS[nxp+S.D_SCF]=POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]); //snow cover fraction
    
    
    
    //Update time-varying inputs
    PAWSOILTEMP.IN.soil_water = POOLS[nxp+S.H2O_PAW];//mm (or kg/m2)
    PUWSOILTEMP.IN.soil_water = POOLS[nxp+S.H2O_PUW];//mm (or kg/m2)
    PAWSOILTEMP.IN.internal_energy = POOLS[nxp+S.E_PAW];//Joules
    PUWSOILTEMP.IN.internal_energy = POOLS[nxp+S.E_PUW];//Joules
    
    SOIL_TEMP_AND_LIQUID_FRAC(&PAWSOILTEMP);
    SOIL_TEMP_AND_LIQUID_FRAC(&PUWSOILTEMP);

    POOLS[nxp+S.D_TEMP_PAW]=PAWSOILTEMP.OUT.TEMP;
    POOLS[nxp+S.D_TEMP_PUW]=PUWSOILTEMP.OUT.TEMP;
    POOLS[nxp+S.D_LF_PAW]=PAWSOILTEMP.OUT.LF;
    POOLS[nxp+S.D_LF_PUW]=PUWSOILTEMP.OUT.LF;
    

    //Pass pointers to function 
    
    
    //Soil moisture
        POOLS[nxp+S.D_SM_PAW]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]); //soil moisture PAW
        POOLS[nxp+S.D_SM_PUW]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]);//soil moisture PUW


}



return 0;
}



// 
// int ODE_FLUX_TABLE(){
//     
//     //Clabile
//     P.noinputs=1;
//             P.in_indices[1]{F.lab_prod };
//     P.nooutputs=1;
//             P.out_indices[1]{F.lab_release};
//     
//Copy this convention
// static int ROFF_fluxes[3];
// ROFF_fluxes[0]=F.q_paw;
// ROFF_fluxes[1]=F.q_puw;
// ROFF_fluxes[2]=F.q_surf;
//     
//     
// }



int DALEC_1100_MODCONFIG(DALEC * DALECmodel){


struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;
struct DALEC_1100_EDCs E=DALEC_1100_EDCs;

DALECmodel->nopools=22;
DALECmodel->nomet=10;/*This should be compatible with CBF file, if not then disp error*/
DALECmodel->nopars=72;
DALECmodel->nofluxes=65;
DALECmodel->dalec=DALEC_1100;
DALECmodel->noedcs=2;

//EDC operator
//Make sure has same number as number of EDCs
//This is generic EDCs structure defined in ../DALEC_EDCs/DALEC_EDC_FUNCTIONS.c
//Has three args, data (void), function (in "DATA", and "void *", and "double" out), and "boolean" prerun.

static EDCs * EDCs;EDCs=calloc(DALECmodel->noedcs,sizeof( * EDCs));


//Som lit turnover rate

//EDC: litter tor > cwd tor
//List all inequality calls here
static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_litcwdtor, EDC_cwdsomtor;

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



// Define all pools here
// static DALEC_EDC_TRAJECTORY_STRUCT EDC_SSED_Clab;
// EDC_SSED_Clab.inputs[2]
// EDC_SSED_Clab.outputs[2]=[F.
        
        
//Eventually adopt more succinct notation (to consider)
//e.g. INEQUALITY_EDC(P.t_cwd,P.t_som,EDCs[E.cwdsomtor])

DALECmodel->EDCs=EDCs;

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
//Provide values required by each OBS operator
//Note: each OBS operator requirements are unique, see individual observation operator functions to see what's required 
//Note: no values required for any SUPPORT_*_OBS quantity set to false.

//GPP-specific variables
OBSOPE.GPP_flux=F.gpp;
//LAI-specific variables
OBSOPE.LAI_pool=S.D_LAI;
//ET variabiles
OBSOPE.ET_flux=F.et;
//Runoff variables
static int ROFF_fluxes[3];
ROFF_fluxes[0]=F.q_paw;
ROFF_fluxes[1]=F.q_puw;
ROFF_fluxes[2]=F.q_surf;
OBSOPE.ROFF_fluxes=ROFF_fluxes;
static double ROFF_flux_signs[]={1.,1.,1.};
OBSOPE.ROFF_flux_signs=ROFF_flux_signs;
OBSOPE.ROFF_n_fluxes=3;
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
static int EWT_h2o_pools[3];
EWT_h2o_pools[0]=S.H2O_PAW;
EWT_h2o_pools[1]=S.H2O_PUW;
EWT_h2o_pools[2]=S.H2O_SWE;
OBSOPE.EWT_h2o_pools=EWT_h2o_pools;
OBSOPE.EWT_n_h2o_pools=3;
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

//CH4-specific variables 
OBSOPE.CH4_flux = F.rh_ch4;

DALECmodel->OBSOPE=OBSOPE;



return 0;}







