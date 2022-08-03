#pragma once
//Note: DALEC_OBSERVATION_OPERATORS.c included in DALEC_MODULE.
#include "DALEC_1100_INDICES.c"
#include "PARS_INFO_1100.c"
#include "../DALEC_ALL/DALEC_STRUCTS.c"//Includes all calls to DALEC-compatible EDC functions
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
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/ALLOC_AND_AUTO_RESP_FLUXES.c"





//Returns structure with sources and sinks, matches number of fluxes
DALEC_FLUX_SOURCES_SINKS_STRUCT DALEC_1100_FLUX_SOURCES_SINKS(DALEC * DALECmodel){
    
//DALEC flux indices    
    struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
    struct DALEC_1100_POOLS S=DALEC_1100_POOLS;

    //Step 1. Declare & initialize
    DALEC_FLUX_SOURCES_SINKS_STRUCT FLUXFLOWS;
    // external source or pool sink, or not conserved quantity
    //Default = -1 
   
    FLUXFLOWS.SOURCE=calloc(DALECmodel->nofluxes, sizeof(int));
    FLUXFLOWS.SINK=calloc(DALECmodel->nofluxes, sizeof(int));
     
    
    int n;
    for (n=0;n<DALECmodel->nofluxes; n++){FLUXFLOWS.SOURCE[n]=-1;FLUXFLOWS.SINK[n]=-1;}
    
    
    //Step 2. Define
    
    
        //source = GPP
        FLUXFLOWS.SINK[F.lab_prod]=S.C_lab;
        
        FLUXFLOWS.SOURCE[F.foliar_prod]=S.C_lab;
        FLUXFLOWS.SINK[F.foliar_prod]=S.C_fol;
        
        FLUXFLOWS.SOURCE[F.root_prod]=S.C_lab;
        FLUXFLOWS.SINK[F.root_prod]=S.C_roo;
        
        FLUXFLOWS.SOURCE[F.wood_prod]=S.C_lab;
        FLUXFLOWS.SINK[F.wood_prod]=S.C_woo;

        FLUXFLOWS.SOURCE[F.f_lab]=S.C_lab;  
        //Sink = atmosphere; 
         
        FLUXFLOWS.SOURCE[F.fx_lab2lit]=S.C_lab;
        FLUXFLOWS.SINK[F.fx_lab2lit]=S.C_lit;
    
        FLUXFLOWS.SOURCE[F.fol2lit]=S.C_fol;
        FLUXFLOWS.SINK[F.fol2lit]=S.C_lit;
          
        FLUXFLOWS.SOURCE[F.f_fol]=S.C_fol;
        //Sink = atmosphere; 

        FLUXFLOWS.SOURCE[F.fx_fol2lit]=S.C_fol;
        FLUXFLOWS.SINK[F.fx_fol2lit]=S.C_lit;
                       
        FLUXFLOWS.SOURCE[F.roo2lit]=S.C_roo;
        FLUXFLOWS.SINK[F.roo2lit]=S.C_lit;
        
        FLUXFLOWS.SOURCE[F.f_roo]=S.C_roo;
        //Sink = atmosphere; 

        FLUXFLOWS.SOURCE[F.fx_roo2lit]=S.C_roo;
        FLUXFLOWS.SINK[F.fx_roo2lit]=S.C_lit;

        FLUXFLOWS.SOURCE[F.woo2cwd]=S.C_woo;
        FLUXFLOWS.SINK[F.woo2cwd]=S.C_cwd;
        
        FLUXFLOWS.SOURCE[F.f_woo]=S.C_woo;
        //Sink = atmosphere; 

        FLUXFLOWS.SOURCE[F.fx_woo2cwd]=S.C_woo;
        FLUXFLOWS.SINK[F.fx_woo2cwd]=S.C_cwd;
        
        FLUXFLOWS.SOURCE[F.ae_rh_cwd]=S.C_cwd;
        //Sink = atmosphere; 
        
        FLUXFLOWS.SOURCE[F.an_rh_cwd]=S.C_cwd;
        //Sink = atmosphere; 
            
        FLUXFLOWS.SOURCE[F.f_cwd]=S.C_cwd;
        //Sink = atmosphere; 

        FLUXFLOWS.SOURCE[F.cwd2som]=S.C_cwd;
        FLUXFLOWS.SINK[F.cwd2som]=S.C_som;

        FLUXFLOWS.SOURCE[F.fx_cwd2som]=S.C_cwd;
        FLUXFLOWS.SINK[F.fx_cwd2som]=S.C_som;
                        
        FLUXFLOWS.SOURCE[F.ae_rh_lit]=S.C_lit;
        //Sink = atmosphere; 
        
        FLUXFLOWS.SOURCE[F.an_rh_lit]=S.C_lit;
        //Sink = atmosphere; 
        
        FLUXFLOWS.SOURCE[F.f_lit]=S.C_lit;
        //Sink = atmosphere; 

        FLUXFLOWS.SOURCE[F.lit2som]=S.C_lit;
        FLUXFLOWS.SINK[F.lit2som]=S.C_som;

        FLUXFLOWS.SOURCE[F.fx_lit2som]=S.C_lit;
        FLUXFLOWS.SINK[F.fx_lit2som]=S.C_som;

        FLUXFLOWS.SOURCE[F.ae_rh_som]=S.C_som;
        //Sink = atmosphere; 
        
        FLUXFLOWS.SOURCE[F.an_rh_som]=S.C_som;
        //Sink = atmosphere; 
        
        FLUXFLOWS.SOURCE[F.f_som]=S.C_som;
        //Sink = atmosphere; 
 
        // Source = atmosphere and snowmelt
        FLUXFLOWS.SINK[F.infil]=S.H2O_PAW;

        FLUXFLOWS.SOURCE[F.melt]=S.H2O_SWE;
        //Sink = infiltration (F.infil); 

        // Source = atmosphere
        FLUXFLOWS.SINK[F.snowfall]=S.H2O_SWE;

        FLUXFLOWS.SOURCE[F.et]=S.H2O_PAW;
        //Sink = atmosphere; 

        FLUXFLOWS.SOURCE[F.paw2puw]=S.H2O_PAW;
        FLUXFLOWS.SINK[F.paw2puw]=S.H2O_PUW;

        FLUXFLOWS.SOURCE[F.q_paw]=S.H2O_PAW;
        // Sink = runoff

        FLUXFLOWS.SOURCE[F.q_puw]=S.H2O_PUW;
        // Sink = runoff

        // Source = atmosphere
        FLUXFLOWS.SINK[F.ground_heat]=S.E_PAW;

        // Source = atmosphere
        FLUXFLOWS.SINK[F.infil_e]=S.E_PAW;

        FLUXFLOWS.SOURCE[F.et_e]=S.E_PAW;
        // Sink = atmosphere

        FLUXFLOWS.SOURCE[F.q_paw_e]=S.E_PAW;
        // Sink = runoff

        FLUXFLOWS.SOURCE[F.q_puw_e]=S.E_PUW;
        // Sink = runoff

        FLUXFLOWS.SOURCE[F.paw2puw_e]=S.E_PAW;
        FLUXFLOWS.SINK[F.paw2puw_e]=S.E_PUW;


return  FLUXFLOWS;
}



    
    
    
    
    
    

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
int stateranges;
} DALEC_1100_EDCs={
    0,1,2
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
POOLS[nxp+S.H2O_SWE]=POOLS[p+S.H2O_SWE]+SNOWFALL[n]*deltat; /*first step snowfall to SWE*/
FLUXES[f+F.melt]=fmin(fmax(((T2M_MIN[n]+T2M_MAX[n])/2-(pars[P.min_melt]-273.15))*pars[P.melt_slope],0),1)*POOLS[nxp+S.H2O_SWE]/deltat; /*melted snow per day*/  
POOLS[nxp+S.H2O_SWE]=POOLS[nxp+S.H2O_SWE]-FLUXES[f+F.melt]*deltat; /*second step remove snowmelt from SWE*/
FLUXES[f+F.snowfall] = SNOWFALL[n]; /*necessary for EDC IO on SWE*/

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
double H = cp*(tskin_k - ref_temp)*pars[P.ga]; // ga in mol m-2 s-1
FLUXES[f+F.sensible_heat] = H; // W m-2
//soil heat flux 
double G = Rn - H - LE; // W m-2
FLUXES[f+F.ground_heat] = G; // W m-2

// Infiltration (mm/day)
FLUXES[f+F.infil] = pars[P.max_infil]*(1 - exp(-(PREC[n] - SNOWFALL[n] + FLUXES[f+F.melt])/pars[P.max_infil]));

// Surface runoff (mm/day)
FLUXES[f+F.q_surf] = (PREC[n] - SNOWFALL[n] + FLUXES[f+F.melt]) - FLUXES[f+F.infil];

// Volumetric soil moisture from water pools
double sm_PAW = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_PAW] + deltat*FLUXES[f+F.infil],pars[P.PAW_por],pars[P.PAW_z]);
double sm_PUW = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);

// Calculate drainage
double drain_PAW = POOLS[p+S.D_LF_PAW]*DRAINAGE(sm_PAW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);
double drain_PUW = POOLS[p+S.D_LF_PUW]*DRAINAGE(sm_PUW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);

// Drainage becomes runoff from pools
FLUXES[f+F.q_paw] = HYDROFUN_MOI2EWT(drain_PAW,pars[P.PAW_por],pars[P.PAW_z])/deltat;
FLUXES[f+F.q_puw] = HYDROFUN_MOI2EWT(drain_PUW,pars[P.PUW_por],pars[P.PUW_z])/deltat;

// Remove drainage from layers before calculating transfer flux
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

//Energy fluxes
///FLUXES[F.T_energy_flux]=0;
//FLUXES[F.E_energy_flux]=0;

//Energy states
//fraction of water in soil that is available 
//double frac_paw = POOLS[nxp+S.H2O_PAW]/(POOLS[nxp+S.H2O_PAW]+POOLS[nxp+S.H2O_PUW]);
POOLS[nxp+S.E_PAW] = POOLS[p+S.E_PAW] + (FLUXES[f+F.ground_heat] + FLUXES[F.infil_e] - FLUXES[F.et_e] - FLUXES[F.paw2puw_e] - FLUXES[F.q_paw_e] )*deltat;  
POOLS[nxp+S.E_PUW] = POOLS[p+S.E_PUW] + (FLUXES[F.paw2puw_e] - FLUXES[F.q_puw_e] )*deltat; //ADD REST OF ENERGY FLUXES HERE



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
FLUXES[f+F.woo2cwd] = POOLS[p+S.C_woo]*(1-pow(1-pars[P.t_wood],deltat))/deltat;
/*root litter production*/
FLUXES[f+F.roo2lit] = POOLS[p+S.C_roo]*(1-pow(1-pars[P.t_root],deltat))/deltat;

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


        POOLS[nxp+S.C_lab] = POOLS[p+S.C_lab] + (FLUXES[f+F.lab_prod]-FLUXES[f+F.foliar_prod]-FLUXES[f+F.root_prod]-FLUXES[f+F.wood_prod])*deltat;
        POOLS[nxp+S.C_fol] = POOLS[p+S.C_fol] + (FLUXES[f+F.foliar_prod] - FLUXES[f+F.fol2lit])*deltat;
        POOLS[nxp+S.C_roo] = POOLS[p+S.C_roo] + (FLUXES[f+F.root_prod] - FLUXES[f+F.roo2lit])*deltat;
        POOLS[nxp+S.C_woo] = POOLS[p+S.C_woo] +  (FLUXES[f+F.wood_prod] - FLUXES[f+F.woo2cwd])*deltat;
        POOLS[nxp+S.C_cwd] = POOLS[p+S.C_cwd] + (FLUXES[f+F.woo2cwd] - FLUXES[f+F.ae_rh_cwd]-FLUXES[f+F.an_rh_cwd]-FLUXES[f+F.cwd2som])*deltat;
        POOLS[nxp+S.C_lit] = POOLS[p+S.C_lit] + (FLUXES[f+F.fol2lit] + FLUXES[f+F.roo2lit] - FLUXES[f+F.ae_rh_lit] - FLUXES[f+F.an_rh_lit] - FLUXES[f+F.lit2som])*deltat;
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

// typedef struct PAR_INFO{
//     double * min;
//     double * max;} PARINFO
    

int PARAMETER_INFO_1100(DALEC * D){
//Inputs: DALEC structure
struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;


PARS_INFO_1100(D->PARS_INFO.parmin, D->PARS_INFO.parmax);

return 0;}


//


// 
// INPUT_OUTPUT_FLUXES_STRUCT * DALEC_1100_INPUT_OUTPUT_FLUXES(DALEC * DALECmodel){
//     
//     
//              struct DALEC_1100_POOLS P=DALEC_1100_POOLS;
//              struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
// 
//              
//     //DALECmodel->nopools;
//     
// INPUT_OUTPUT_FLUXES_STRUCT * FIO=calloc(  DALECmodel->nopools, sizeof( INPUT_OUTPUT_FLUXES_STRUCT));;
//            //Step 1. Declaring all as "zero" inputs and "zero" ouputs.  
//  
//            
//     //DALECmodel->nopools;
// //Setting all IO to zero (default)
//     int n=0;
//     for (n=0;n<   DALECmodel->nopools;n++){FIO[n].N_INPUT_FLUXES=0;FIO[n].N_OUTPUT_FLUXES=0;}
//     
//     
// 
//                 //C labile
//                FIO[P.C_lab].N_INPUT_FLUXES=1;
//                FIO[P.C_lab].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.C_lab].INPUT_FLUXES[0]=F.lab_prod;
// 
//                FIO[P.C_lab]->N_OUTPUT_FLUXES=3;
//                FIO[P.C_lab]->OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.C_lab]->OUTPUT_FLUXES[0]=F.lab_release;
//                FIO[P.C_lab]->OUTPUT_FLUXES[1]=F.f_lab;
//                FIO[P.C_lab]->OUTPUT_FLUXES[2]= F.fx_lab2lit;
// 
//                 //C foliar
//                FIO[P.C_fol]->N_INPUT_FLUXES=1;
//                FIO[P.C_fol]->INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.C_fol]->INPUT_FLUXES[0]=F.lab_release;
// 
//                FIO[P.C_fol]->N_OUTPUT_FLUXES=3;
//                FIO[P.C_fol]->OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.C_fol]->OUTPUT_FLUXES[0]=F.fol2lit;
//                FIO[P.C_fol]->OUTPUT_FLUXES[1]=F.f_fol;
//                FIO[P.C_fol].OUTPUT_FLUXES[2]= F.fx_fol2lit;
// 
//                 //C root
//                FIO[P.C_roo].N_INPUT_FLUXES=1;
//                FIO[P.C_roo].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.C_roo].INPUT_FLUXES[0]=F.root_prod;
// 
//                FIO[P.C_roo].N_OUTPUT_FLUXES=3;
//                FIO[P.C_roo].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.C_roo].OUTPUT_FLUXES[0]=F.roo2lit;
//                FIO[P.C_roo].OUTPUT_FLUXES[1]=F.f_roo;
//                FIO[P.C_roo].OUTPUT_FLUXES[2]= F.fx_roo2lit;
// 
//                 //C wood
//                FIO[P.C_woo].N_INPUT_FLUXES=1;
//                FIO[P.C_woo].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.C_woo].INPUT_FLUXES[0]=F.wood_prod;
// 
//                FIO[P.C_woo].N_OUTPUT_FLUXES=3;
//                FIO[P.C_woo].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.C_woo].OUTPUT_FLUXES[0]=F.woo2cwd;
//                FIO[P.C_woo].OUTPUT_FLUXES[1]=F.f_woo;
//                FIO[P.C_woo].OUTPUT_FLUXES[2]= F.fx_woo2cwd;
// 
//    
//                 //C CWD
//                FIO[P.C_cwd].N_INPUT_FLUXES=2;
//                FIO[P.C_cwd].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.C_cwd].INPUT_FLUXES[0]=F.woo2cwd;
//                FIO[P.C_cwd].INPUT_FLUXES[1]=F.fx_woo2cwd;
// 
//                FIO[P.C_cwd].N_OUTPUT_FLUXES=5;
//                FIO[P.C_cwd].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.C_cwd].OUTPUT_FLUXES[0]=F.ae_rh_cwd;
//                FIO[P.C_cwd].OUTPUT_FLUXES[1]=F.an_rh_cwd;
//                FIO[P.C_cwd].OUTPUT_FLUXES[2]= F.cwd2som;
//                FIO[P.C_cwd].OUTPUT_FLUXES[3]=F.f_cwd;
//                FIO[P.C_cwd].OUTPUT_FLUXES[4]= F.fx_cwd2som;
// 
// 
//                 //C Litter
//                FIO[P.C_lit].N_INPUT_FLUXES=5;
//                FIO[P.C_lit].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.C_lit].INPUT_FLUXES[0]=F.fol2lit;
//                FIO[P.C_lit].INPUT_FLUXES[1]=F.fx_fol2lit;
//                FIO[P.C_lit].INPUT_FLUXES[2]=F.roo2lit;
//                FIO[P.C_lit].INPUT_FLUXES[3]=F.fx_roo2lit;
//                FIO[P.C_lit].INPUT_FLUXES[4]=F.fx_lab2lit;
// 
//                FIO[P.C_lit].N_OUTPUT_FLUXES=5;
//                FIO[P.C_lit].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.C_lit].OUTPUT_FLUXES[0]=F.ae_rh_lit;
//                FIO[P.C_lit].OUTPUT_FLUXES[1]=F.an_rh_lit;
//                FIO[P.C_lit].OUTPUT_FLUXES[2]=F.lit2som;
//                FIO[P.C_lit].OUTPUT_FLUXES[3]=F.f_lit;
//                FIO[P.C_lit].OUTPUT_FLUXES[4]= F.fx_lit2som;
// 
//                 //C SOM
//                FIO[P.C_som].N_INPUT_FLUXES=4;
//                FIO[P.C_som].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.C_som].INPUT_FLUXES[0]=F.lit2som;
//                FIO[P.C_som].INPUT_FLUXES[1]=F.fx_lit2som;
//                FIO[P.C_som].INPUT_FLUXES[2]=F.cwd2som;
//                FIO[P.C_som].INPUT_FLUXES[3]=F.fx_cwd2som;
// 
//                FIO[P.C_som].N_OUTPUT_FLUXES=3;
//                FIO[P.C_som].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.C_som].OUTPUT_FLUXES[0]=F.ae_rh_som;
//                FIO[P.C_som].OUTPUT_FLUXES[1]=F.an_rh_som;
//                FIO[P.C_som].OUTPUT_FLUXES[2]=F.f_som;
// 
// 
//                 // H2O PAW
//                FIO[P.H2O_PAW].N_INPUT_FLUXES=1;
//                FIO[P.H2O_PAW].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.H2O_PAW].INPUT_FLUXES[0]=F.infil;
// 
//                FIO[P.H2O_PAW].N_OUTPUT_FLUXES=3;
//                FIO[P.H2O_PAW].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.H2O_PAW].OUTPUT_FLUXES[0]=F.paw2puw;
//                FIO[P.H2O_PAW].OUTPUT_FLUXES[1]=F.q_paw;
//                FIO[P.H2O_PAW].OUTPUT_FLUXES[2]=F.et;
// 
// 
//                 // H2O PUW
//                FIO[P.H2O_PUW].N_INPUT_FLUXES=1;
//                FIO[P.H2O_PUW].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.H2O_PUW].INPUT_FLUXES[0]=F.paw2puw;
// 
//                FIO[P.H2O_PUW].N_OUTPUT_FLUXES=1;
//                FIO[P.H2O_PUW].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.H2O_PUW].OUTPUT_FLUXES[0]=F.q_puw;
// 
// 
//                 // H2O SWE
//                FIO[P.H2O_SWE].N_INPUT_FLUXES=1;
//                FIO[P.H2O_SWE].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.H2O_SWE].INPUT_FLUXES[0]=F.snowfall;
// 
//                FIO[P.H2O_SWE].N_OUTPUT_FLUXES=1;
//                FIO[P.H2O_SWE].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.H2O_SWE].OUTPUT_FLUXES[0]=F.melt;
// 
// 
//                // E PAW
//                FIO[P.E_PAW].N_INPUT_FLUXES=2;
//                FIO[P.E_PAW].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.E_PAW].INPUT_FLUXES[0]=F.ground_heat;
//                FIO[P.E_PAW].INPUT_FLUXES[1]=F.infil_e;
// 
//                FIO[P.E_PAW].N_OUTPUT_FLUXES=3;
//                FIO[P.E_PAW].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.E_PAW].OUTPUT_FLUXES[0]=F.paw2puw_e;
//                FIO[P.E_PAW].OUTPUT_FLUXES[1]=F.q_paw_e;
//                FIO[P.E_PAW].OUTPUT_FLUXES[2]=F.et_e;
// 
//                // E PUW
//                FIO[P.E_PUW].N_INPUT_FLUXES=1;
//                FIO[P.E_PUW].INPUT_FLUXES=calloc(FIO.N_INPUT_FLUXES, sizeof(int));
//                FIO[P.E_PUW].INPUT_FLUXES[0]=F.paw2puw_e;
// 
//                FIO[P.E_PUW].N_OUTPUT_FLUXES=1;
//                FIO[P.E_PUW].OUTPUT_FLUXES=calloc(FIO.N_OUTPUT_FLUXES, sizeof(int));
//                FIO[P.E_PUW].OUTPUT_FLUXES[0]=F.q_puw_e;
// 
// 
//           
// 
// 
//                return FIO;
//     
// }
//         
        
        
   
        
        
        
        
        



int DALEC_1100_MODCONFIG(DALEC * DALECmodel){


struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;
struct DALEC_1100_EDCs E=DALEC_1100_EDCs;

DALEC_1100_FLUX_SOURCES_SINKS(DALECmodel);

DALECmodel->nopools=22;
DALECmodel->nomet=10;/*This should be compatible with CBF file, if not then disp error*/
DALECmodel->nopars=72;

DALECmodel->nofluxes=68;

DALECmodel->dalec=DALEC_1100;
DALECmodel->noedcs=4;

//Define PARS_INFO here (ranges, and eventually names, etc)
PARAMETER_INFO_1100(DALECmodel);

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

//State ranges
   DALEC_EDC_STATE_RANGES_STRUCT EDC_sr;
   //Create array (1x number of pools) for minimum and max values 
   //Index exactly corresponds to "POOLS" indices
   EDC_sr.nopools=DALECmodel->nopools;
   EDC_sr.min_val=calloc(DALECmodel->nopools,sizeof(double));
   EDC_sr.max_val=calloc(DALECmodel->nopools,sizeof(double));
   
   
   EDC_sr.min_val[S.C_lab]=0;
   EDC_sr.max_val[S.C_lab]=DALECmodel->PARS_INFO.parmax[P.i_labile];
                
   EDC_sr.min_val[S.C_fol]=0;
   EDC_sr.max_val[S.C_fol]=DALECmodel->PARS_INFO.parmax[P.i_foliar];
                
   EDC_sr.min_val[S.C_roo]=0;
   EDC_sr.max_val[S.C_roo]=DALECmodel->PARS_INFO.parmax[P.i_root];
                
   EDC_sr.min_val[S.C_woo]=0;
   EDC_sr.max_val[S.C_woo]=DALECmodel->PARS_INFO.parmax[P.i_wood];
                
   EDC_sr.min_val[S.C_cwd]=0;
   EDC_sr.max_val[S.C_cwd]=DALECmodel->PARS_INFO.parmax[P.i_cwd];
                
   EDC_sr.min_val[S.C_lit]=0;
   EDC_sr.max_val[S.C_lit]=DALECmodel->PARS_INFO.parmax[P.i_lit];
                
   EDC_sr.min_val[S.C_som]=0;
   EDC_sr.max_val[S.C_som]=DALECmodel->PARS_INFO.parmax[P.i_som];
                
   EDC_sr.min_val[S.H2O_PAW]=0;
   EDC_sr.max_val[S.H2O_PAW]=DALECmodel->PARS_INFO.parmax[P.i_PAW];
                
   EDC_sr.min_val[S.H2O_PUW]=0;
   EDC_sr.max_val[S.H2O_PUW]=DALECmodel->PARS_INFO.parmax[P.i_PUW];
                
   EDC_sr.min_val[S.H2O_SWE]=0;
   EDC_sr.max_val[S.H2O_SWE]=DALECmodel->PARS_INFO.parmax[P.i_SWE];
    
   EDC_sr.min_val[S.E_PAW]=0;
   EDC_sr.max_val[S.E_PAW]=DALECmodel->PARS_INFO.parmax[P.i_PAW_E];
    
   EDC_sr.min_val[S.E_PUW]=0;
   EDC_sr.max_val[S.E_PUW]=DALECmodel->PARS_INFO.parmax[P.i_PUW_E];
    
   EDC_sr.min_val[S.D_LAI]=0;
   EDC_sr.max_val[S.D_LAI]=DALECmodel->PARS_INFO.parmax[P.lambda_max];
    
   EDC_sr.min_val[S.D_SCF]=0;
   EDC_sr.max_val[S.D_SCF]=1;
    
   EDC_sr.min_val[S.D_TEMP_PAW]=-100;
   EDC_sr.max_val[S.D_TEMP_PAW]=100;
    
   EDC_sr.min_val[S.D_TEMP_PUW]=-100;
   EDC_sr.max_val[S.D_TEMP_PUW]=100;
    
   EDC_sr.min_val[S.D_LF_PAW]=0;
   EDC_sr.max_val[S.D_LF_PAW]=1;
    
   EDC_sr.min_val[S.D_LF_PUW]=0;
   EDC_sr.max_val[S.D_LF_PUW]=1;
    
   EDC_sr.min_val[S.D_SM_PAW]=0;
   EDC_sr.max_val[S.D_SM_PAW]=1;
    
   EDC_sr.min_val[S.D_SM_PUW]=0;
   EDC_sr.max_val[S.D_SM_PUW]=1;
    
   EDC_sr.min_val[S.M_LAI_MAX]=-INFINITY; // get a better value from Alex N.
   EDC_sr.max_val[S.M_LAI_MAX]=INFINITY; // get a better value from Alex N.
    
   EDC_sr.min_val[S.M_LAI_TEMP]=-INFINITY;  // get a better value from Alex N.
   EDC_sr.max_val[S.M_LAI_TEMP]=INFINITY; // get a better value from Alex N.


//Adding EDC to the EDCs list
    EDCs[E.stateranges].data=&EDC_sr;
    EDCs[E.stateranges].function=&DALEC_EDC_STATE_RANGES;
    EDCs[E.stateranges].prerun=false;
// 
// 
// // Define all pools here
//static DALEC_EDC_TRAJECTORY_STRUCT EDC_t_C_lab;
//  
// EDC_t_Clab.pool_idx = S.C_lab;
// static int C_lab_fin = ;
// EDC_t_Clab.nfin = ;
// EDC_t_Clab.FIN = ;
// EDC_t_Clab.nfout = ;
// EDC_t_Clab.FOUT = ;
// } DALEC_EDC_TRAJECTORY_STRUCT;
    
    
    //Define which pools need to be constrained here
    
    EDC_st.n_pools=...

    EDC_st.pool_indices=...
            
            EDC_st.FLUX_SOURCE_SINK_STRUCT=...
        //Rest can be done by code without additional input
    EDCs[E.statetrajectory].data=&EDC_st;
    EDCs[E.statetrajectory].function=&DALEC_EDC_STATE_TRAJECTORIES;
    EDCs[E.statetrajectory].prerun=false;
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







