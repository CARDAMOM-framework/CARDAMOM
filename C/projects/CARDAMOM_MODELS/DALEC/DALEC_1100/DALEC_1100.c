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
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/ALLOC_AND_AUTO_RESP_FLUXES.c"


//Returns structure with sources and sinks, matches number of fluxes
int DALEC_1100_FLUX_SOURCES_SINKS(DALEC * DALECmodel){
    

    struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
    struct DALEC_1100_POOLS S=DALEC_1100_POOLS;

    //Step 1. Declare & initialize
    DALEC_FLUX_SOURCE_SINK_MATRIX FIOMATRIX;
    // external source or pool sink, or not conserved quantity
    //Default = -1 
   
    FIOMATRIX.SOURCE=calloc(DALECmodel->nofluxes, sizeof(int));
    FIOMATRIX.SINK=calloc(DALECmodel->nofluxes, sizeof(int));
     
    
    int n;
    for (n=0;n<DALECmodel->nofluxes; n++){FIOMATRIX.SOURCE[n]=-1;FIOMATRIX.SINK[n]=-1;}
    
    
    //Step 2. Define
    

        // C_lab
        FIOMATRIX.SINK[F.lab_prod]=S.C_lab;
        FIOMATRIX.SOURCE[F.foliar_prod]=S.C_lab;
        FIOMATRIX.SOURCE[F.root_prod]=S.C_lab;
        FIOMATRIX.SOURCE[F.wood_prod]=S.C_lab;
        FIOMATRIX.SOURCE[F.resp_auto_growth]=S.C_lab;
        FIOMATRIX.SOURCE[F.f_lab]=S.C_lab;  
        FIOMATRIX.SOURCE[F.fx_lab2lit]=S.C_lab;


        // C_fol
        FIOMATRIX.SINK[F.foliar_prod]=S.C_fol;
        FIOMATRIX.SOURCE[F.fol2lit]=S.C_fol;
        FIOMATRIX.SOURCE[F.f_fol]=S.C_fol;
        FIOMATRIX.SOURCE[F.fx_fol2lit]=S.C_fol;
        
        // C_roo
        FIOMATRIX.SINK[F.root_prod]=S.C_roo;
        FIOMATRIX.SOURCE[F.roo2lit]=S.C_roo;
        FIOMATRIX.SOURCE[F.f_roo]=S.C_roo;
        FIOMATRIX.SOURCE[F.fx_roo2lit]=S.C_roo;
        
        // C_woo
        FIOMATRIX.SINK[F.wood_prod]=S.C_woo;
        FIOMATRIX.SOURCE[F.woo2cwd]=S.C_woo;
        FIOMATRIX.SOURCE[F.f_woo]=S.C_woo;
        FIOMATRIX.SOURCE[F.fx_woo2cwd]=S.C_woo;

        
        // C_lit
        FIOMATRIX.SINK[F.fx_lab2lit]=S.C_lit;
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
        FIOMATRIX.SINK[F.snowfall]=S.H2O_SWE;

        // H2O_PAW
        FIOMATRIX.SINK[F.infil]=S.H2O_PAW;
        FIOMATRIX.SOURCE[F.et]=S.H2O_PAW;
        FIOMATRIX.SOURCE[F.paw2puw]=S.H2O_PAW;
        FIOMATRIX.SOURCE[F.q_paw]=S.H2O_PAW;

        // H2O_PUW        
        FIOMATRIX.SOURCE[F.q_puw]=S.H2O_PUW;
        FIOMATRIX.SINK[F.paw2puw]=S.H2O_PUW;

        // E_PAW
        FIOMATRIX.SINK[F.ground_heat]=S.E_PAW;
        FIOMATRIX.SINK[F.infil_e]=S.E_PAW;
        FIOMATRIX.SOURCE[F.et_e]=S.E_PAW;
        FIOMATRIX.SOURCE[F.q_paw_e]=S.E_PAW;
        FIOMATRIX.SOURCE[F.paw2puw_e]=S.E_PAW;
        FIOMATRIX.SOURCE[F.paw2puw_th_e]=S.E_PAW;

        // E_PUW
        FIOMATRIX.SINK[F.paw2puw_e]=S.E_PUW;
        FIOMATRIX.SINK[F.paw2puw_th_e]=S.E_PUW;
        FIOMATRIX.SOURCE[F.q_puw_e]=S.E_PUW;


        
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
  POOLS[S.H2O_PAW]=HYDROFUN_MOI2EWT(pars[P.i_PAW_SM],pars[P.PAW_por],pars[P.PAW_z]);
  POOLS[S.H2O_PUW]=HYDROFUN_MOI2EWT(pars[P.i_PUW_SM],pars[P.PUW_por],pars[P.PUW_z]);
  POOLS[S.H2O_SWE]=pars[P.i_SWE];
  /*Energy pools*/
  POOLS[S.E_PAW]=pars[P.i_PAW_E]*pars[P.PAW_z];
  POOLS[S.E_PUW]=pars[P.i_PUW_E]*pars[P.PUW_z];
  
  
   //---INITIALIZING DIAGNOSTIC STATES---
    POOLS[S.D_LAI]=POOLS[S.C_fol]/pars[P.LCMA]; //LAI
    
    if (POOLS[S.H2O_SWE]>0){
    POOLS[S.D_SCF]=POOLS[S.H2O_SWE]/(POOLS[S.H2O_SWE]+pars[P.scf_scalar]);} //snow cover fraction}
    else
    {POOLS[S.D_SCF]=0;};
    
    
    //INITIALIZING PAW and PUW soil moisture
        POOLS[S.D_SM_PAW]=HYDROFUN_EWT2MOI(POOLS[S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]); //soil moisture PAW
        POOLS[S.D_SM_PUW]=HYDROFUN_EWT2MOI(POOLS[S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);//soil moisture PUW

        //Diagnostic time-invariant quantities
        double PAWmax=pars[P.PAW_por]*pars[P.PAW_z]*1000; //PAW capacity in mm
        double PUWmax=pars[P.PUW_por]*pars[P.PUW_z]*1000; //PUW capacity in mm
        
        
        
    //Declare stryct
    SOIL_TEMP_AND_LIQUID_FRAC_STRUCT PAWSOILTEMP, PUWSOILTEMP;
  //Populate with run-specific constrants
    //PAW
    PAWSOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.PAW_vhc]; ;//J/m3/K
    PAWSOILTEMP.IN.depth = pars[P.PAW_z];//m 
    PAWSOILTEMP.IN.soil_water = POOLS[S.H2O_PAW];//mm (or kg/m2)
    PAWSOILTEMP.IN.internal_energy = POOLS[S.E_PAW];//Joules
    //Pass pointer to function 
    SOIL_TEMP_AND_LIQUID_FRAC(&PAWSOILTEMP);  //Outputs are in K
        //Store outputs 
    POOLS[S.D_TEMP_PAW]=PAWSOILTEMP.OUT.TEMP;  //In K  
    POOLS[S.D_LF_PAW]=PAWSOILTEMP.OUT.LF;

    //PUW
    PUWSOILTEMP.IN.dry_soil_vol_heat_capacity =pars[P.PUW_vhc]; ;//J/m3/K
    PUWSOILTEMP.IN.depth = pars[P.PUW_z];//m 
    PUWSOILTEMP.IN.soil_water = POOLS[S.H2O_PUW];//mm (or kg/m2)
    PUWSOILTEMP.IN.internal_energy = POOLS[S.E_PUW];//Joules
    //Pass pointer to function 
    SOIL_TEMP_AND_LIQUID_FRAC(&PUWSOILTEMP);//Outputs are in K
    //Store outputs 
    POOLS[S.D_TEMP_PUW]=PUWSOILTEMP.OUT.TEMP;    //In K
    POOLS[S.D_LF_PUW]=PUWSOILTEMP.OUT.LF;

    
    
    
    
    //******************Delcare KNORR STRUCT*********************
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
        ARFLUXES.IN.mr_fr=pars[P.rauto_mr_fr];//
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

double LAD = 0.5; //leaf angle distribution// optimize leaf angle distribution. 
double VegK = LAD/cos(zenith_angle/180*pi);

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
//double psi_PAW0 = HYDROFUN_MOI2PSI(max(POOLS[p+S.D_SM_PAW],0),psi_porosity,pars[P.retention]);
double psi_PAW0 = HYDROFUN_MOI2PSI(POOLS[p+S.D_SM_PAW],psi_porosity,pars[P.retention]);
double beta = 1/(1 + exp(pars[P.beta_lgr]*(-1*psi_PAW0/pars[P.psi_50] - 1)));

// mean air temperature (K)
double air_temp_k = DGCM_TK0C+0.5*(T2M_MIN[n]+T2M_MAX[n]);

//******************Declare LIU STRUCT*********************
LIU_AN_ET_STRUCT LIU;

//define time-invariant parameters
LIU.IN.SRAD=SSRD[n]*1e6/DGCM_SEC_DAY;
LIU.IN.VPD=VPD[n]/10;
LIU.IN.TEMP=air_temp_k;  
LIU.IN.vcmax25=pars[P.Vcmax25];
LIU.IN.co2=CO2[n];
LIU.IN.beta_factor=fmin(beta,g)*POOLS[p+S.D_LF_PAW];
LIU.IN.g1=pars[P.Med_g1];
LIU.IN.LAI=LAI;
LIU.IN.ga=pars[P.ga];
LIU.IN.VegK=VegK;
LIU.IN.Tupp=pars[P.Tupp];
LIU.IN.Tdown=pars[P.Tdown];
LIU.IN.C3_frac=1., // pars[P.C3_frac]
LIU.IN.clumping=pars[P.clumping];
LIU.IN.leaf_refl_par=pars[P.leaf_refl_par];
LIU.IN.leaf_refl_nir=pars[P.leaf_refl_nir];
LIU.IN.maxPevap=pars[P.maxPevap];
LIU.IN.precip=PREC[n];


//Call function: uses LIU->IN to update LIU->OUT
LIU_AN_ET(&LIU);

// GPP
FLUXES[f+F.gpp] = LIU.OUT.Ag;
//transpiration//
FLUXES[f+F.transp] = LIU.OUT.transp;
//evaporation//
FLUXES[f+F.evap] = LIU.OUT.evap;

// Evapotranspiration
FLUXES[f+F.et]=FLUXES[f+F.evap]+FLUXES[f+F.transp];

/*Snow water equivalent*/
FLUXES[f+F.snowfall] = SNOWFALL[n];
POOLS[nxp+S.H2O_SWE]=POOLS[p+S.H2O_SWE]+FLUXES[f+F.snowfall]*deltat; /*first step snowfall to SWE*/
FLUXES[f+F.melt]=fmin(fmax((air_temp_k-pars[P.min_melt])*pars[P.melt_slope],0),1)*POOLS[nxp+S.H2O_SWE]/deltat; /*melted snow per day*/  
POOLS[nxp+S.H2O_SWE]=POOLS[nxp+S.H2O_SWE]-FLUXES[f+F.melt]*deltat; /*second step remove snowmelt from SWE*/

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
double LWout = sigma*pow(tskin_k,4.); // W m-2
//Net radiation at the top of the canopy-soil continuum
//
//
double Rn = SWin - SWout + LWin - LWout; // W m-2
//Rnet only into soil
FLUXES[f+F.net_radiation] = Rn; // W m-2

//Rnet snow free
double Rn_snowfree = SWin - SWout_snowfree + LWin - LWout; // W m-2


//These are only fluxes into PAW and out of PAW
FLUXES[f+F.SWin]=SWin;//flag for redundancy and deletion
FLUXES[f+F.LWin]=LWin;//flag for redundancy and deletion
FLUXES[f+F.SWout]=SWout;
FLUXES[f+F.LWout]=LWout;
        
//Latent heat of Vaporization J kg-1 
double lambda = DGCM_LATENT_HEAT_VAPORIZATION; //2.501*1e6 J kg-1 
//Latente heat (W.m-2)
double LE = lambda*FLUXES[f+F.et]/DGCM_SEC_DAY; // W m-2
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
FLUXES[f+F.ground_heat] =(pars[P.thermal_cond_surf]* (tskin_k - POOLS[p+S.D_TEMP_PAW])/(pars[P.PAW_z]*0.5))*(1. - POOLS[p+S.D_SCF]);
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
// Include infiltration into PAW ()
double sm_PAW = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]);
double sm_PUW = HYDROFUN_EWT2MOI(POOLS[p+S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);

// Update PAW SM with infiltration
//sm_PAW += HYDROFUN_EWT2MOI(infil*deltat,pars[P.PAW_por],pars[P.PAW_z]);

// Calculate drainage

//printf("POOLS[p+S.D_LF_PAW] = %2.2f\n",POOLS[p+S.D_LF_PAW]);
double drain_PAW = POOLS[p+S.D_LF_PAW]*DRAINAGE(sm_PAW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);
double drain_PUW = POOLS[p+S.D_LF_PUW]*DRAINAGE(sm_PUW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);

// Drainage becomes runoff from pools
FLUXES[f+F.q_paw] = HYDROFUN_MOI2EWT(drain_PAW,pars[P.PAW_por],pars[P.PAW_z])/deltat;
FLUXES[f+F.q_puw] = HYDROFUN_MOI2EWT(drain_PUW,pars[P.PUW_por],pars[P.PUW_z])/deltat;


// // Remove drainage from layers
// sm_PAW -= drain_PAW;
// sm_PUW -= drain_PUW;

// Convert to conductivity
double k_PAW = HYDROFUN_MOI2CON(sm_PAW,pars[P.hydr_cond],pars[P.retention]);
double k_PUW = HYDROFUN_MOI2CON(sm_PUW,pars[P.hydr_cond],pars[P.retention]);

// Convert to potential
double psi_PAW = HYDROFUN_MOI2PSI(sm_PAW,psi_porosity,pars[P.retention]);
double psi_PUW = HYDROFUN_MOI2PSI(sm_PUW,psi_porosity,pars[P.retention]);



// Calculate inter-pool transfer in m/s (positive is PAW to PUW)
double pot_xfer = 1000 * sqrt(k_PAW*k_PUW) * (1000*(psi_PAW-psi_PUW)/(9.8*0.5*(pars[P.PAW_z]+pars[P.PUW_z])) + 1);
double SPACEavail, H2Oavail, PAW2PUWmax, TEMPxfer;
if (pot_xfer>0) {//Water is going PAW->PUW (down)
// Available space in PUW (after runoff)
SPACEavail=fmax(pars[P.PUW_z]*pars[P.PUW_por]*1e3 - POOLS[p+S.H2O_PUW] + FLUXES[f+F.q_puw]*deltat,0);
// Available water in PAW (after runoff, et, and infiltration)
H2Oavail=fmax(POOLS[p+S.D_LF_PAW]*POOLS[p+S.H2O_PAW] + (FLUXES[f+F.infil] - FLUXES[f+F.q_paw] - FLUXES[f+F.et])*deltat,0);
// Maximum transfer flux in mm (actual transfer may be less due to water or space availability)
PAW2PUWmax= POOLS[p+S.D_LF_PAW]*pot_xfer*1000*DGCM_SEC_DAY*deltat;
//Minimum of three terms for PAW->PUW
//1. PAW2PUWmax
//2. Available space in PUW (after runoff)
//3. PAW*LF (after runoff, et, and infiltration)
FLUXES[f+F.paw2puw] =fmin(PAW2PUWmax , fmin(SPACEavail, H2Oavail))/deltat;
TEMPxfer= POOLS[p+S.D_TEMP_PAW];//In K
}
else { //Water is going PUW->PAW (up)
// Available space in PAW after runoff, et, and infiltration
SPACEavail=fmax(pars[P.PAW_z]*pars[P.PAW_por]*1e3 - POOLS[p+S.H2O_PAW] - (FLUXES[f+F.infil] - FLUXES[f+F.q_paw] - FLUXES[f+F.et])*deltat,0);
// Available water in PUW after runoff
H2Oavail= fmax(POOLS[p+S.D_LF_PUW]*POOLS[p+S.H2O_PUW] - FLUXES[f+F.q_puw]*deltat,0);
// Maximum transfer flux in mm (actual transfer may be less due to water or space availability)
PAW2PUWmax= POOLS[p+S.D_LF_PUW]*pot_xfer*1000*DGCM_SEC_DAY*deltat;
// Reverse sign of previous case
FLUXES[f+F.paw2puw] = -fmin(-PAW2PUWmax , fmin(SPACEavail, H2Oavail))/deltat;
TEMPxfer= POOLS[p+S.D_TEMP_PUW];//In K
}


// Update pools, including ET from PAW
POOLS[nxp+S.H2O_PAW] = POOLS[p+S.H2O_PAW] + (FLUXES[f+F.infil] - FLUXES[f+F.paw2puw] - FLUXES[f+F.q_paw] - FLUXES[f+F.et])*deltat;
POOLS[nxp+S.H2O_PUW] = POOLS[p+S.H2O_PUW] + (FLUXES[f+F.paw2puw] - FLUXES[f+F.q_puw])*deltat;



if (POOLS[nxp+S.H2O_PAW]>PAWmax){
//Dump excess into PAW Q
FLUXES[f+F.q_paw] +=(POOLS[nxp+S.H2O_PAW]-PAWmax)/deltat;
POOLS[nxp+S.H2O_PAW]=PAWmax;}

if (POOLS[nxp+S.H2O_PUW]>PUWmax){
//Dump excess into PAW Q
FLUXES[f+F.q_puw] +=(POOLS[nxp+S.H2O_PUW]-PUWmax)/deltat;
POOLS[nxp+S.H2O_PUW]=PUWmax;}


//**********INTERNAL ENERGT FLUXES FOR ALL H2O FLUXES***************
//Add INFILTRATION, PAW, PUW, PAW2PUW, ET
double infiltemp = air_temp_k ;//Infiltemp needs to be in degrees celcius for IF statement to work
if (FLUXES[f+F.melt]>0){infiltemp = (infiltemp-DGCM_TK0C)*(PREC[n] - SNOWFALL[n])/(PREC[n] - SNOWFALL[n] + FLUXES[f+F.melt])+DGCM_TK0C;}//snowmelt temp = 0, so term multiplied by zero in weighted average 


//All energy fluxes

FLUXES[f+F.infil_e] = FLUXES[f+F.infil]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(infiltemp);
FLUXES[f+F.et_e] = FLUXES[f+F.et]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_PAW]);
FLUXES[f+F.paw2puw_e] = FLUXES[f+F.paw2puw]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(TEMPxfer);
FLUXES[f+F.q_paw_e] = FLUXES[f+F.q_paw]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_PAW]);
FLUXES[f+F.q_puw_e] =  FLUXES[f+F.q_puw]*INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(POOLS[p+S.D_TEMP_PUW]);
//Thermal conductivity = k*dT/dz, units are W/m2, converting to J/m2/d
FLUXES[f+F.paw2puw_th_e] = 2*pars[P.thermal_cond]* (POOLS[p+S.D_TEMP_PAW] - POOLS[p+S.D_TEMP_PUW])/(pars[P.PAW_z] + pars[P.PUW_z])*DGCM_SEC_DAY;

        
//Thermal energy flux only
//FLUXES[f+F.paw2puw_e_thermal] = 

//Energy states
//fraction of water in soil that is available 
//double frac_paw = POOLS[nxp+S.H2O_PAW]/(POOLS[nxp+S.H2O_PAW]+POOLS[nxp+S.H2O_PUW]);

POOLS[nxp+S.E_PAW] = POOLS[p+S.E_PAW] + (FLUXES[f+F.gh_in] + FLUXES[f+F.infil_e] - FLUXES[f+F.et_e] - FLUXES[f+F.paw2puw_e] - FLUXES[f+F.q_paw_e]-FLUXES[f+F.paw2puw_th_e])*deltat;  
POOLS[nxp+S.E_PUW] = POOLS[p+S.E_PUW] + (FLUXES[f+F.paw2puw_e] - FLUXES[f+F.q_puw_e] +FLUXES[f+F.paw2puw_th_e])*deltat; 



//*************KNORR LAI**************


//Time varying KNORR function terms

KNORR.IN.lambda_max_memory=  POOLS[p+S.M_LAI_MAX];
KNORR.IN.T_memory=POOLS[p+S.M_LAI_TEMP];
KNORR.IN.temp=air_temp_k;
KNORR.IN.n=n;
KNORR.IN.DOY=DOY[n];
KNORR.IN.lambda=LAI;
KNORR.IN.pasm=(POOLS[p+S.H2O_PAW]+POOLS[nxp+S.H2O_PAW])/2.0;//Note: soil moisture also available here
KNORR.IN.ET= FLUXES[f+F.et];
//Call function: uses KNORR->IN to update KNORR->OUT
KNORR_ALLOCATION(&KNORR);
 
FLUXES[f+F.target_LAI]=KNORR.OUT.lambda_next;
//KNORR.OUT.dlambdadt is in units per timestep; converting thest to units per day (as required for CARDAMOM)
//"FLUXES" have to be in "per day" units
FLUXES[f+F.dlambda_dt]=KNORR.OUT.dlambdadt/deltat;
FLUXES[f+F.f_temp_thresh]= KNORR.OUT.f_T;
FLUXES[f+F.f_dayl_thresh]= KNORR.OUT.f_d;

//Update KNORR memory variables for next iteration
POOLS[nxp+S.M_LAI_MAX]=KNORR.OUT.laim;
POOLS[nxp+S.M_LAI_TEMP]=KNORR.OUT.T;
    

    
//************Allocation*******************

ARFLUXES.IN.deltat=deltat;
ARFLUXES.IN.GPP=FLUXES[f+F.gpp];
ARFLUXES.IN.TEMP=air_temp_k;
ARFLUXES.IN.NSC=POOLS[p+S.C_lab];
ARFLUXES.IN.C_LIVE_W=POOLS[p+S.C_woo];
ARFLUXES.IN.C_LIVE_FR= POOLS[p+S.C_fol]+POOLS[p+S.C_roo];
// Potential plant allocation (growth) fluxes
ARFLUXES.IN.ALLOC_FOL_POT=fmax(0, ((FLUXES[f+F.target_LAI] * pars[P.LCMA]) - POOLS[p+S.C_fol])/deltat);
ARFLUXES.IN.ALLOC_ROO_POT=fmax(0, (pars[P.phi_RL] * (FLUXES[f+F.target_LAI] * pars[P.LCMA]))/deltat);
ARFLUXES.IN.ALLOC_WOO_POT=fmax(0, (pars[P.phi_WL] * (FLUXES[f+F.target_LAI] * pars[P.LCMA]))/deltat);

ALLOC_AND_AUTO_RESP_FLUXES(&ARFLUXES);

    double MORTALITY_FACTOR=ARFLUXES.OUT.MORTALITY_FACTOR;




/*respiration auto*/
FLUXES[f+F.resp_auto]=ARFLUXES.OUT.AUTO_RESP_TOTAL;
/*growth respiration*/
FLUXES[f+F.resp_auto_growth]=ARFLUXES.OUT.AUTO_RESP_GROWTH;
/*maintenance respiration*/
FLUXES[f+F.resp_auto_maint]=ARFLUXES.OUT.AUTO_RESP_MAINTENANCE + LIU.OUT.Rd;

// Fcfolavailable=FLUXES[f+F.lab_prod] + POOLS[p+S.C_lab]/deltat;
if (FLUXES[f+F.dlambda_dt] > 0){
  FLUXES[f+F.fol2lit]=POOLS[p+S.C_fol]*(1-pow(1-pars[P.t_foliar],deltat))/deltat;
}
else {
    //FLUXES[f+F.dlambda_dt] is in m2/m2/day
    //LCMA = gC/m2/m2
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
     HRJCR.IN.TEMP=POOLS[p+S.D_TEMP_PAW];; // Input in degrees K
     HRJCR.IN.LF=POOLS[p+S.D_LF_PAW];; // Input in degrees 

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

        POOLS[nxp+S.C_lab] = POOLS[p+S.C_lab] + (FLUXES[f+F.lab_prod]-FLUXES[f+F.resp_auto_maint]-FLUXES[f+F.foliar_prod]-FLUXES[f+F.root_prod]-FLUXES[f+F.wood_prod]-FLUXES[f+F.resp_auto_growth])*deltat;
        POOLS[nxp+S.C_fol] = POOLS[p+S.C_fol] + (FLUXES[f+F.foliar_prod] - FLUXES[f+F.fol2lit])*deltat;
        POOLS[nxp+S.C_roo] = POOLS[p+S.C_roo] + (FLUXES[f+F.root_prod] - FLUXES[f+F.roo2lit])*deltat;
        POOLS[nxp+S.C_woo] = POOLS[p+S.C_woo] + (FLUXES[f+F.wood_prod] - FLUXES[f+F.woo2cwd])*deltat;
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

//P*M + P*(1-M)*BAf = P*M + P*BAf - P*M*BAf = P*(M + BAf - M*BAf)  = P*(BAf*(1 - M) + M)

    //LIVE BIOMASS MORTALITY FLUXES
    //if MORTALITY
    FLUXES[f+F.fx_lab2lit] = POOLS[nxp+S.C_lab]*(MORTALITY_FACTOR + (1-MORTALITY_FACTOR)*BURNED_AREA[n]*(1-CF[S.C_lab])*(1-pars[P.resilience]))/deltat;
    FLUXES[f+F.fx_fol2lit] = POOLS[nxp+S.C_fol]*(MORTALITY_FACTOR + (1-MORTALITY_FACTOR)*BURNED_AREA[n]*(1-CF[S.C_fol])*(1-pars[P.resilience]))/deltat;
    FLUXES[f+F.fx_roo2lit] = POOLS[nxp+S.C_roo]*(MORTALITY_FACTOR + (1-MORTALITY_FACTOR)*MORTALITY_FACTOR*BURNED_AREA[n]*(1-CF[S.C_roo])*(1-pars[P.resilience]))/deltat;
    FLUXES[f+F.fx_woo2cwd] = POOLS[nxp+S.C_woo]*(MORTALITY_FACTOR + (1-MORTALITY_FACTOR)*BURNED_AREA[n]*(1-CF[S.C_woo])*(1-pars[P.resilience]))/deltat;
    //No mortality in these pools
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

    POOLS[nxp+S.D_TEMP_PAW]=PAWSOILTEMP.OUT.TEMP;//In K
    POOLS[nxp+S.D_TEMP_PUW]=PUWSOILTEMP.OUT.TEMP;//In K
    
    POOLS[nxp+S.D_LF_PAW]=PAWSOILTEMP.OUT.LF;
    POOLS[nxp+S.D_LF_PUW]=PUWSOILTEMP.OUT.LF;
    

    //Pass pointers to function 

    
    //Soil moisture

        POOLS[nxp+S.D_SM_PAW]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]); //soil moisture PAW
        POOLS[nxp+S.D_SM_PUW]=HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);//soil moisture PUW



}



return 0;
}


int DALEC_1100_MODCONFIG(DALEC * DALECmodel){


struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;
struct DALEC_1100_EDCs E=DALEC_1100_EDCs;




DALECmodel->dalec=DALEC_1100;
DALECmodel->nopools=22;
DALECmodel->nomet=10;/*This should be compatible with CBF file, if not then disp error*/
DALECmodel->nopars=76;
DALECmodel->nofluxes=70;
DALECmodel->noedcs=5;

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
static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_mr_rates;

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

//EDC: foliar and root mr > wood mr
EDC_mr_rates.big_par_index=P.rauto_mr_fr;
EDC_mr_rates.small_par_index=P.rauto_mr_w;
EDCs[E.mr_rates].data=&EDC_mr_rates;
EDCs[E.mr_rates].function=&DALEC_EDC_PARAMETER_INEQUALITY;
EDCs[E.mr_rates].prerun=true;



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
   //
                
   EDC_sr.min_val[S.H2O_PUW]=0;
   //
                
    EDC_sr.min_val[S.H2O_SWE]=0;
//    EDC_sr.max_val[S.H2O_SWE]=DALECmodel->PARS_INFO.parmax[P.i_SWE];
//     
    EDC_sr.min_val[S.E_PAW]=0;
//     
    EDC_sr.min_val[S.E_PUW]=0;
//     
    EDC_sr.min_val[S.D_LAI]=0;
    EDC_sr.max_val[S.D_LAI]=DALECmodel->PARS_INFO.parmax[P.lambda_max];
//     
//     EDC_sr.min_val[S.D_SCF]=0;
//     EDC_sr.max_val[S.D_SCF]=1;
//     
    EDC_sr.min_val[S.D_TEMP_PAW]=173.15;
    EDC_sr.max_val[S.D_TEMP_PAW]=373.15;
//     
    EDC_sr.min_val[S.D_TEMP_PUW]=173.15;
    EDC_sr.max_val[S.D_TEMP_PUW]=373.15;
// //     
//     EDC_sr.min_val[S.D_LF_PAW]=0;
//     EDC_sr.max_val[S.D_LF_PAW]=1;
// //     
//     EDC_sr.min_val[S.D_LF_PUW]=0;
//     EDC_sr.max_val[S.D_LF_PUW]=1;
//     
   EDC_sr.min_val[S.D_SM_PAW]=0;
    EDC_sr.max_val[S.D_SM_PAW]=1;
//     
    EDC_sr.min_val[S.D_SM_PUW]=0;
    EDC_sr.max_val[S.D_SM_PUW]=1;
    
   EDC_sr.min_val[S.M_LAI_MAX]=-INFINITY; // get a better value from Alex N.
   EDC_sr.max_val[S.M_LAI_MAX]=INFINITY; // get a better value from Alex N.
    
   EDC_sr.min_val[S.M_LAI_TEMP]=-INFINITY;  // get a better value from Alex N.
   EDC_sr.max_val[S.M_LAI_TEMP]=INFINITY; // get a better value from Alex N.





//Adding EDC to the EDCs list


    EDCs[E.state_ranges].data=&EDC_sr;
    EDCs[E.state_ranges].function=&DALEC_EDC_STATE_RANGES;
    EDCs[E.state_ranges].prerun=false;


//Start soil moistures for PAW and PUW
//         static DALEC_EDC_START_SM_STRUCT EDCpawsm, EDCpuwsm;
// 
//     EDCpawsm.z_idx=P.PAW_z;
// EDCpawsm.por_idx=P.PAW_por;
// EDCpawsm.i_H2O_idx =P.i_PAW;
// 
// EDCpuwsm.z_idx=P.PUW_z;
// EDCpuwsm.por_idx=P.PUW_por;
// EDCpuwsm.i_H2O_idx =P.i_PUW;
// 
// 
// 
// EDCs[E.paw_sm].data=&EDCpawsm;
// EDCs[E.paw_sm].function=&DALEC_EDC_START_SM;
// EDCs[E.paw_sm].prerun=true;
// 
// EDCs[E.puw_sm].data=&EDCpuwsm;
// EDCs[E.puw_sm].function=&DALEC_EDC_START_SM;
// EDCs[E.puw_sm].prerun=true;



//Start temperatures for PAW and PUW




 static DALEC_EDC_TRAJECTORY_STRUCT EDC_st;
 
 static int edc_pool_indices[7];
EDC_st.pool_indices=edc_pool_indices;
EDC_st.no_pools_to_check=7;
            
//EDC_st.pool_indices[0]=S.E_PAW;
//EDC_st.pool_indices[1]=S.E_PUW;
//EDC_st.pool_indices[2]=S.H2O_PAW;
//EDC_st.pool_indices[3]=S.H2O_PUW;
//EDC_st.pool_indices[4]=S.H2O_SWE;


    
EDC_st.pool_indices[0]=S.C_lab;
EDC_st.pool_indices[1]=S.C_fol;
 EDC_st.pool_indices[2]=S.C_roo;
EDC_st.pool_indices[3]=S.C_woo;
EDC_st.pool_indices[4]=S.C_cwd;
EDC_st.pool_indices[5]=S.C_lit;
 EDC_st.pool_indices[6]=S.C_som;
    
// //Rest can be done by code without additional input
    
EDCs[E.state_trajectories].data=&EDC_st;
EDCs[E.state_trajectories].function=&DALEC_EDC_TRAJECTORY;
EDCs[E.state_trajectories].prerun=false;

//Eventually adopt more succinct notation (to consider)
//e.g. INEQUALITY_EDC(P.t_cwd,P.t_som,EDCs[E.cwdsomtor])






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







