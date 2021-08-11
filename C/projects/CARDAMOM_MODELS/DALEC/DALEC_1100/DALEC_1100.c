#pragma once
//Note: DALEC_OBSERVATION_OPERATORS.c included in DALEC_MODULE.
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/DRAINAGE.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_EWT2MOI.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2EWT.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2CON.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2PSI.c"
#include "../../../DALEC_CODE/DALEC_ALL/LIU_An_et.c"
#include "../../../DALEC_CODE/DALEC_ALL/LAI_KNORR.c"

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/



//***DALEC PARAMETERS***
struct DALEC_1100_PARAMETERS{
  int tr_lit2soil;
  int f_auto;
  int f_foliar;
  int f_root;
  int t_foliar;
  int t_wood;
  int t_root;
  int t_lit;
  int t_soil;
  int temp_factor;
  int canopy_eff;
  int Bday;
  int f_lab;
  int labile_rel;
  int Fday;
  int leaf_fall;
  int LCMA;
  int i_labile;
  int i_foliar;
  int i_root;
  int i_wood;
  int i_lit;
  int i_soil;
  int retention;
  int wilting;
  int i_PAW;
  int cf_foliar;
  int cf_ligneous;
  int cf_DOM;
  int resilience;
  int t_labile;
  int moisture;
  int hydr_cond;
  int max_infil;
  int i_PUW;
  int PAW_por;
  int PUW_por;
  int field_cap;
  int PAW_z;
  int PUW_z;
  int Q_excess;
  int Med_g1;
  int Vcmax25;
  int Tminmin;
  int Tminmax;
  int ga;
  int Tupp;
  int Tdown;
  int C3_frac;
  int i_LAI;
  int T_phi;
  int T_range;
  int tau_m;
  int plgr;
  int k_leaf;
  int tau_W;
  int time_c;
  int time_r;
} DALEC_1100_PARAMETERS={
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
  16,17,18,19,20,21,22,23,24,25,26,27,28,
  29,30,31,32,33,34,35,36,37,38,39,40,41,
  42,43,44,45,46,47,48,49,50,51,52,53,54,
  55,56,57
};

//***DALEC FLUXES***
struct DALEC_1100_FLUXES{
  int gpp;
  int temprate;
  int resp_auto;
  int leaf_prod;
  int lab_prod;
  int root_prod;
  int wood_prod;
  int lab_release;
  int leaffall_fact;
  int leaf2lit;
  int wood2lit;
  int root2lit;
  int resp_het_lit;
  int resp_het_som;
  int lit2som;
  int lab_release_fact;
  int f_total;
  int f_lab;
  int f_fol;
  int f_roo;
  int f_woo;
  int f_lit;
  int f_som;
  int fx_lab2lit;
  int fx_fol2lit;
  int fx_roo2lit;
  int fx_woo2som;
  int fx_lit2som;
  int et;
  int q_paw;
  int paw2puw;
  int q_puw;
  int q_surf;
  int transp;
  int evap;
  int target_LAI;
  int t_memory;
} DALEC_1100_FLUXES={
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
  16,17,18,19,20,21,22,23,24,25,26,27,28,
  29,30,31,32,33,34,35,36
};

//***DALEC POOLS***
struct DALEC_1100_POOLS{
  int C_lab;
  int C_fol;
  int C_roo;
  int C_woo;
  int C_lit;
  int C_som;
  int H2O_PAW;
  int H2O_PUW;
  int H2O_SWE;
} DALEC_1100_POOLS={
  0,1,2,3,4,5,6,7,8
};



int DALEC_1100_MODCONFIG(DALEC * DALECmodel){




DALECmodel->nopools=8;
DALECmodel->nomet=9;/*This should be compatible with CBF file, if not then disp error*/

DALECmodel->nopars=59;
DALECmodel->nofluxes=46;

//declaring observation operator structure, and filling with DALEC configurations
static OBSOPE OBSOPE;
//Initialize all SUPPORT OBS values (default value = false).
INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);

//Set SUPPORT_OBS values to true if model supports observation operation.
printf("DALEC_1100_MODCONFIG, Line 22...\n");
OBSOPE.SUPPORT_GPP_OBS=true;
OBSOPE.SUPPORT_LAI_OBS=true;
OBSOPE.SUPPORT_ET_OBS=true;
OBSOPE.SUPPORT_NBE_OBS=true;
OBSOPE.SUPPORT_ABGB_OBS=true;
OBSOPE.SUPPORT_SOM_OBS=true;
OBSOPE.SUPPORT_GRACE_EWT_OBS=true;
OBSOPE.SUPPORT_FIR_OBS=true;

//Provide values required by each OBS operator
//Note: each OBS operator requirements are unique, see individual observation operator functions to see what's required 
//Note: no values required for any SUPPORT_*_OBS quantity set to false.

//GPP-specific variables
OBSOPE.GPP_flux=0;
//LAI-specific variables
OBSOPE.LAI_foliar_pool=1;
OBSOPE.LAI_LCMA=16;
//ET variabiles
OBSOPE.ET_flux=28;
//NBE-specific variables
static int NBE_fluxes[]={0,2,12,13,16};
OBSOPE.NBE_fluxes=NBE_fluxes;
static double NBE_flux_signs[]={-1.,1.,1.,1.,1.};
OBSOPE.NBE_flux_signs=NBE_flux_signs;
OBSOPE.NBE_n_fluxes=5;

//ABGB-specific variables
static int ABGB_pools[]={0,1,2,3}; 
OBSOPE.ABGB_pools=ABGB_pools;
OBSOPE.ABGB_n_pools=4;

//SOM-specific variables
static int SOM_pools[]={4,5}; 
OBSOPE.SOM_pools=SOM_pools;
OBSOPE.SOM_n_pools=2;
//H2O-specific variables
static int GRACE_EWT_h2o_pools[]={6,7};
OBSOPE.GRACE_EWT_h2o_pools=GRACE_EWT_h2o_pools;
OBSOPE.GRACE_EWT_n_h2o_pools=2;
//Fire-specific variables
OBSOPE.FIR_flux=16;

DALECmodel->OBSOPE=OBSOPE;

return 0;}



int DALEC_1100(DATA DATA, double const *pars)
{



struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;




/*C-pools, fluxes, meteorology indices*/
int p=0,f,m,nxp, i;
int n=0,nn=0;
double pi=3.1415927;
double lai_met_list[1],lai_var_list[20];

double deltat=DATA.ncdf_data.TIME_INDEX.values[1] - DATA.ncdf_data.TIME_INDEX.values[0];
int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;


 double constants[10]={pars[P.canopy_eff],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};

/*Pointer transfer - all data stored in fluxes and pools will be passed to DATA*/
double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;
double *LAI=DATA.M_LAI;
// double *NEE=DATA.M_NEE;

  /*assigning values to pools*/
  /*L,F,R,W,Lit,SOM*/
  POOLS[S.C_lab]=pars[P.i_labile];
  POOLS[S.C_fol]=pars[i_LAI]*pars[P.LCMA];
  POOLS[S.C_roo]=pars[P.i_root];
  POOLS[S.C_woo]=pars[P.i_wood];
  POOLS[S.C_lit]=pars[P.i_lit];
  POOLS[S.C_som]=pars[P.i_soil];

  /*water pools*/
  POOLS[S.H2O_PAW]=pars[P.i_PAW];
  POOLS[S.H2O_PUW]=pars[P.i_PUW];

double *SSRD=DATA.ncdf_data.SSRD.values;
double *T2M_MIN=DATA.ncdf_data.T2M_MIN.values;
double *T2M_MAX=DATA.ncdf_data.T2M_MAX.values;
double *CO2=DATA.ncdf_data.CO2.values;
double *DOY=DATA.ncdf_data.DOY.values;
double *PREC=DATA.ncdf_data.TOTAL_PREC.values;
double *VPD=DATA.ncdf_data.VPD.values;
double *BURNED_AREA=DATA.ncdf_data.BURNED_AREA.values;
double *TIME_INDEX=DATA.ncdf_data.TIME_INDEX.values;

double meantemp = (DATA.ncdf_data.T2M_MAX.reference_mean + DATA.ncdf_data.T2M_MIN.reference_mean)/2;
double meanrad = DATA.ncdf_data.SSRD.reference_mean;
double meanprec = DATA.ncdf_data.TOTAL_PREC.reference_mean;



/*constants for exponents of leaffall and labrelease factors*/
/*width*/
double wf=pars[P.leaf_fall]*sqrt(2)/2;
double wl=pars[P.labile_rel]*sqrt(2)/2;


/*factor*/
double ff=(log(pars[P.t_foliar])-log(pars[P.t_foliar]-1))/2;
/*double fl=(log(1.001)-log(0.001))/2;*/
double fl=(log(pars[P.t_labile])-log(pars[P.t_labile]-1))/2;


// Porosity scaling factor (see line 124 of HESS paper)
double psi_porosity = -0.117*9.8/1000;

/*additional offset*/
double osf=offset(pars[P.t_foliar],wf);
double osl=offset(pars[P.t_labile],wl);


/*scaling to biyearly sine curve*/
double sf=365.25/pi;

/*Combustion factors*/
double CF[6];
CF[S.C_lab]=pars[P.cf_ligneous];
CF[S.C_fol]=pars[P.cf_foliar];
CF[S.C_roo]=pars[P.cf_ligneous];
CF[S.C_woo]=pars[P.cf_ligneous];
CF[S.C_lit]=pars[P.cf_foliar]/2+pars[P.cf_ligneous]/2;
CF[S.C_som]=pars[P.cf_DOM];

/*foliar carbon transfer intermediate variables*/
double Fcfolavailable;

/*resilience factor*/


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



/*LAI*/
LAI[n]=POOLS[p+S.C_fol]/pars[P.LCMA]; 


/*Calculate light extinction coefficient*/
double B = (DOY[n]-81)*2*pi/365.;
double ET1 = 9.87*sin(2*B)-7.53*cos(B)-1.5*sin(B);
double DA = 23.45*sin((284+DOY[n])*2*pi/365); //Deviation angle
double LST = (int) (DOY[n]*24*60) % (24*60);
double AST = LST+ET1;
double h = (AST-12*60)/4; //hour angle
double alpha = asin((sin(pi/180*DATA.ncdf_data.LAT)*sin(pi/180*DA)+cos(pi/180*DATA.ncdf_data.LAT)*cos(pi/180.*DA)*cos(pi/180*h)))*180/pi; //solar altitude
double zenith_angle = 90-alpha;
double LAD = 1.0; //leaf angle distribution
double VegK = sqrt(pow(LAD,2)+ pow(tan(zenith_angle/180*pi),2))/(LAD+1.774*pow((1+1.182),-0.733)); //Campbell and Norman 1998


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
double beta = fmin(POOLS[p+S.H2O_PAW]/pars[P.wilting],1);

// GPP, T, and E from LIU_An_et
// Annual radiation, VPD in kPa, mean T in K
double *LIU_An_et_out = LIU_An_et(SSRD[n]*1e6/(24*3600), VPD[n]/10, 
    273.15+0.5*(T2M_MIN[n]+T2M_MAX[n]), pars[P.Vcmax25], CO2[n], beta, pars[P.Med_g1], 
    LAI[n], pars[P.ga], VegK, pars[P.Tupp], pars[P.Tdown], pars[P.C3_frac]);
// GPP
FLUXES[f+F.gpp] = LIU_An_et_out[0]*g;
//transpiration//
FLUXES[f+F.transp] = LIU_An_et_out[1];
//evaporation//
FLUXES[f+F.evap] = LIU_An_et_out[2];
// Evapotranspiration
FLUXES[f+F.et]=FLUXES[f+F.evap]+FLUXES[f+F.transp];


//KNORR LAI
if (n==0){
  /*Initialize phenology memory of air-temperature */
  lai_var_list[5]=pars[P.T_phi]+3*pars[T_range];
  /*Initialize phenology memory of water/structural limitation */
  lai_var_list[11]=pars[P.lambda_max];
}
lai_met_list[0]=(T2M_MAX[n] + T2M_MIN[n])/2.0; /* meantemp, deg C*/
lai_var_list[0]=n; /*current timestep index of model run*/
lai_var_list[19]=deltat; /*time increment of model run*/
lai_var_list[1]=LAI[n]; /*initial LAI parameter*/
lai_var_list[2]=LAI[n];
lai_var_list[3]=pars[P.T_phi]; /*T_phi*/
lai_var_list[4]=pars[P.T_range]; /*T_r*/
lai_var_list[6]=pars[P.tau_m]; /*tau_m*/
lai_var_list[7]=pars[P.plgr]; /*plgr*/
lai_var_list[8]=pars[P.k_leaf]; /*k_L*/
lai_var_list[9]=pars[P.lambda_max]; /*lambda_max*/
lai_var_list[10]=pars[P.tau_W]; /*tau_W*/
lai_var_list[12]=DATA.ncdf_data.LAT; /*latitude*/
lai_var_list[13]=DOY[n]; /*day of year*/
lai_var_list[14]=pi; /*pi*/
lai_var_list[15]=pars[P.time_c]; /*t_c*/
lai_var_list[16]=pars[P.time_r]; /*t_r*/
/*Put the plant-available soil water into the LAI_KNORR input list*/
lai_var_list[17]=(POOLS[p+6]+POOLS[nxp+6])/2.0;
/*Put the evapotranspiration flux into the LAI_KNORR input list*/
lai_var_list[18]=FLUXES[f+28];
// Run KNORR LAI module
// - this computes a potential LAI
double *LAI_KNORR_out = LAI_KNORR(lai_met_list, lai_var_list);
FLUXES[f+F.target_LAI] = LAI_KNORR_out[0];  // LAI (environmental target)
FLUXES[f+F.t_memory] = LAI_KNORR_out[1];  // T_memory
FLUXES[f+39] = LAI_KNORR_out[2];  // lambda_max_memory
FLUXES[f+40] = LAI_KNORR_out[3]/deltat;  // dlambda/dt (units: LAI per day)
FLUXES[f+44] = LAI_KNORR_out[4];  // fraction of plants above temperature threshold
FLUXES[f+45] = LAI_KNORR_out[5];  // fraction of plants above day length threshold
/*Update environmental memory variables for next iteration*/
/*temperature memory*/
lai_var_list[5]=FLUXES[f+F.t_memory];//POOLS[nxp+10];
/*water/structural memory*/
lai_var_list[11]=FLUXES[f+39];//POOLS[nxp+11];

/*temprate - now comparable to Q10 - factor at 0C is 1*/
/* x (1 + a* P/P0)/(1+a)*/
FLUXES[f+F.temprate]=exp(pars[P.temp_factor]*0.5*(T2M_MIN[n]+T2M_MAX[n]-meantemp))*((PREC[n]/meanprec-1)*pars[P.moisture]+1);
/*respiration auto*/
FLUXES[f+F.resp_auto]=pars[P.f_auto]*FLUXES[f+F.gpp];
/*leaf production*/
// FLUXES[f+F.leaf_prod]=(FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto])*pars[P.f_foliar];
FLUXES[f+F.leaf_prod] = 0 // Knorr LAI module
/*labile production*/
// FLUXES[f+F.lab_prod] = (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.leaf_prod])*pars[P.f_lab];              
/* - now the combination of two fractional allocation parameters*/
FLUXES[f+F.lab_prod] = (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto])*(pars[P.f_lab]+pars[P.f_foliar]);
Fcfolavailable=FLUXES[f+F.lab_prod] + POOLS[p+S.C_lab]/deltat;
if (FLUXES[f+40]*pars[P.LCMA] > Fcfolavailable){
  /* flag for carbon availability limitation (0=canopy in senescence, 1=labile C does not limit growth, 2=labile C limits LAI growth) */
  FLUXES[f+41]=2.0;
  /* flux from labile pool to foliar pool */
  FLUXES[F.lab_release]=Fcfolavailable;
  /* flux from foliar pool to litter pool */
  FLUXES[f+F.leaf2lit]=0;
}
else if (FLUXES[f+40]*pars[P.LCMA] < Fcfolavailable && FLUXES[f+40]*pars[P.LCMA] > 0){
  FLUXES[f+41]=1.0;
  /* flux from labile pool to foliar pool */
  FLUXES[F.lab_release]=FLUXES[f+40]*pars[P.LCMA];
  /* flux from foliar pool to litter pool */
  FLUXES[f+F.leaf2lit]=0;
}
else {
  FLUXES[f+41]=0.0;
  /* flux from labile pool to foliar pool */
  FLUXES[F.lab_release]=0;
  /* flux from foliar pool to litter pool */
  FLUXES[f+F.leaf2lit]=-FLUXES[f+F.target_LAI]*pars[P.LCMA];
}
/*root production*/        
FLUXES[f+F.root_prod] = (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.leaf_prod]-FLUXES[f+F.lab_prod])*pars[P.f_root];            
/*wood production*/       
FLUXES[f+F.wood_prod] = FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.leaf_prod]-FLUXES[f+F.root_prod]-FLUXES[f+F.lab_prod]; 
//   /*leaf fall factor*/
//   FLUXES[f+F.leaffall_fact] = (2/sqrt(pi))*(ff/wf)*exp(-pow(sin((TIME_INDEX[n]-pars[P.Fday]+osf)/sf)*sf/wf,2));
//   /*Labrelease factor*/
//   FLUXES[f+F.lab_release_fact]=(2/sqrt(pi))*(fl/wl)*exp(-pow(sin((TIME_INDEX[n]-pars[P.Bday]+osl)/sf)*sf/wl,2));
//   /*labile release - re-arrange order in next versions*/
//   FLUXES[f+F.lab_release] = POOLS[p+S.C_lab]*(1-pow(1-FLUXES[f+F.lab_release_fact],deltat))/deltat;
//   /*leaf litter production*/       
//   FLUXES[f+F.leaf2lit] = POOLS[p+S.C_fol]*(1-pow(1-FLUXES[f+F.leaffall_fact],deltat))/deltat;
/*wood litter production*/       
FLUXES[f+F.wood2lit] = POOLS[p+S.C_woo]*(1-pow(1-pars[P.t_wood],deltat))/deltat;
/*root litter production*/
FLUXES[f+F.root2lit] = POOLS[p+S.C_roo]*(1-pow(1-pars[P.t_root],deltat))/deltat;
/*respiration heterotrophic litter*/
FLUXES[f+F.resp_het_lit] = POOLS[p+S.C_lit]*(1-pow(1-FLUXES[f+F.temprate]*pars[P.t_lit],deltat))/deltat;
/*respiration heterotrophic SOM*/
FLUXES[f+F.resp_het_som] = POOLS[p+S.C_som]*(1-pow(1-FLUXES[f+F.temprate]*pars[P.t_soil],deltat))/deltat;
/*litter to SOM*/
FLUXES[f+F.lit2som] = POOLS[p+S.C_lit]*(1-pow(1-pars[P.tr_lit2soil]*FLUXES[f+F.temprate],deltat))/deltat;

/*total pool transfers (no fires yet)*/

        POOLS[nxp+S.C_lab] = POOLS[p+S.C_lab] + (FLUXES[f+F.lab_prod]-FLUXES[f+F.lab_release])*deltat;
        POOLS[nxp+S.C_fol] = POOLS[p+S.C_fol] + (FLUXES[f+F.lab_release] - FLUXES[f+F.leaf2lit])*deltat;
        POOLS[nxp+S.C_roo] = POOLS[p+S.C_roo] + (FLUXES[f+F.root_prod] - FLUXES[f+F.root2lit])*deltat;
        POOLS[nxp+S.C_woo] = POOLS[p+S.C_woo] +  (FLUXES[f+F.wood_prod] - FLUXES[f+F.wood2lit])*deltat;
        POOLS[nxp+S.C_lit] = POOLS[p+S.C_lit] + (FLUXES[f+F.leaf2lit] + FLUXES[f+F.root2lit] - FLUXES[f+F.resp_het_lit] - FLUXES[f+F.lit2som])*deltat; 
        POOLS[nxp+S.C_som]= POOLS[p+S.C_som]+ (FLUXES[f+F.lit2som] - FLUXES[f+F.resp_het_som] + FLUXES[f+F.wood2lit])*deltat;                    


// Infiltration (mm/day)
double infil = pars[P.max_infil]*(1 - exp(-PREC[n]/pars[P.max_infil]));

// Surface runoff (mm/day)
FLUXES[f+F.q_surf] = PREC[n] - infil;

// Update pools, including infiltration
POOLS[nxp+S.H2O_PAW] = POOLS[p+S.H2O_PAW] + deltat*infil;
POOLS[nxp+S.H2O_PUW] = POOLS[p+S.H2O_PUW];

// Volumetric soil moisture from water pools
double sm_PAW = HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]);
double sm_PUW = HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);

// Update PAW SM with infiltration
//sm_PAW += HYDROFUN_EWT2MOI(infil*deltat,pars[P.PAW_por],pars[P.PAW_z]);

// Calculate drainage
double drain_PAW = DRAINAGE(sm_PAW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);
double drain_PUW = DRAINAGE(sm_PUW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);

// Drainage becomes runoff from pools
FLUXES[f+F.q_paw] = HYDROFUN_MOI2EWT(drain_PAW,pars[P.PAW_por],pars[P.PAW_z])/deltat;
FLUXES[f+F.q_puw] = HYDROFUN_MOI2EWT(drain_PUW,pars[P.PUW_por],pars[P.PUW_z])/deltat;

// Remove drainage from layers
sm_PAW -= drain_PAW;
sm_PUW -= drain_PUW;

// Convert to conductivity
double k_PAW = HYDROFUN_MOI2CON(sm_PAW,pars[P.hydr_cond],pars[P.hydr_cond]);
double k_PUW = HYDROFUN_MOI2CON(sm_PUW,pars[P.hydr_cond],pars[P.hydr_cond]);

// Convert to potential
double psi_PAW = HYDROFUN_MOI2PSI(sm_PAW,psi_porosity,pars[P.hydr_cond]);
double psi_PUW = HYDROFUN_MOI2PSI(sm_PUW,psi_porosity,pars[P.hydr_cond]);

// Calculate inter-pool transfer in m/s (positive is PAW to PUW)
double xfer = 1000 * sqrt(k_PAW*k_PUW) * (1000*(psi_PAW-psi_PUW)/(9.8*0.5*(pars[P.PAW_z]+pars[P.PUW_z])) + 1);

// Transfer flux in mm/day
FLUXES[f+F.paw2puw] = xfer*1000*3600*24;

// Update pools, including ET from PAW
POOLS[nxp+S.H2O_PAW] += (-FLUXES[f+F.paw2puw] - FLUXES[f+F.q_paw] - FLUXES[f+F.et])*deltat;
POOLS[nxp+S.H2O_PUW] += (FLUXES[f+F.paw2puw] - FLUXES[f+F.q_puw])*deltat;



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
    FLUXES[f+F.f_lit] = POOLS[nxp+S.C_lit]*BURNED_AREA[n]*CF[S.C_lit]/deltat;
    FLUXES[f+F.f_som] = POOLS[nxp+S.C_som]*BURNED_AREA[n]*CF[S.C_som]/deltat;

    FLUXES[f+F.fx_lab2lit] = POOLS[nxp+S.C_lab]*BURNED_AREA[n]*(1-CF[S.C_lab])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_fol2lit] = POOLS[nxp+S.C_fol]*BURNED_AREA[n]*(1-CF[S.C_fol])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_roo2lit] = POOLS[nxp+S.C_roo]*BURNED_AREA[n]*(1-CF[S.C_roo])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_woo2som] = POOLS[nxp+S.C_woo]*BURNED_AREA[n]*(1-CF[S.C_woo])*(1-pars[P.resilience])/deltat;
    FLUXES[f+F.fx_lit2som] = POOLS[nxp+S.C_lit]*BURNED_AREA[n]*(1-CF[S.C_lit])*(1-pars[P.resilience])/deltat;


	/*Adding all fire pool transfers here*/
	/*live C pools*/	
    POOLS[nxp+S.C_lab] = POOLS[nxp+S.C_lab]-(FLUXES[f+F.f_lab]+FLUXES[f+F.fx_lab2lit])*deltat;
    POOLS[nxp+S.C_fol] = POOLS[nxp+S.C_fol]-(FLUXES[f+F.f_fol]+FLUXES[f+F.fx_fol2lit])*deltat;
    POOLS[nxp+S.C_roo] = POOLS[nxp+S.C_roo]-(FLUXES[f+F.f_roo]+FLUXES[f+F.fx_roo2lit])*deltat;
    POOLS[nxp+S.C_woo] = POOLS[nxp+S.C_woo]-(FLUXES[f+F.f_woo]+FLUXES[f+F.fx_woo2som])*deltat;
	/*dead C pools*/
	/*litter*/
	POOLS[nxp+S.C_lit] = POOLS[nxp+S.C_lit]+(FLUXES[f+F.fx_lab2lit]+FLUXES[f+F.fx_fol2lit]+FLUXES[f+F.fx_roo2lit]-FLUXES[f+F.f_lit]-FLUXES[f+F.fx_lit2som])*deltat;
	/*som*/
	POOLS[nxp+S.C_som] = POOLS[nxp+S.C_som]+(FLUXES[f+F.fx_woo2som]+FLUXES[f+F.fx_lit2som]-FLUXES[f+F.f_som])*deltat;

	/*fires - total flux in gC m-2 day-1*/
	/*this term is now (essentially) obsolete*/
	/*replace in next version of DALEC_FIRES*/
    FLUXES[f+F.f_total] = FLUXES[f+F.f_lab] + FLUXES[f+F.f_fol] + FLUXES[f+F.f_roo] + FLUXES[f+F.f_woo] + FLUXES[f+F.f_lit] + FLUXES[f+F.f_som];

   /*Fraction of C-foliar lost due to fires*/
    FLUXES[f+43] = BURNED_AREA[n]*(CF[S.C_lab] + (1-CF[S.C_lab])*(1-pars[P.resilience]));
   /*Calculate LAI (lambda) lost due to fire
     - we lose the same fraction of LAI as we do C-foliar 
     - FE_\Lambda^{(t+1)} = \Lambda^{(t+1)'} * BA ( k_{factor(i)} + (1 - k_{factor(i)}) r )*/
   FLUXES[f+42] = FLUXES[f+F.target_LAI]*BURNED_AREA[n]*(CF[S.C_lab] + (1-CF[S.C_lab])*(1-pars[P.resilience]));
   /*Subtract fire loss LAI from current LAI*/
   FLUXES[f+F.target_LAI] = FLUXES[f+F.target_LAI] - FLUXES[f+42];
  
}

/*LAI is a CARDAMOM-wide state variable, ensuring available at first/last timestep in general (LAI) form, rather than only as "Cfol/LCMA"*/
LAI[n+1]=POOLS[nxp+1]/pars[P.LCMA];

return 0;
}






