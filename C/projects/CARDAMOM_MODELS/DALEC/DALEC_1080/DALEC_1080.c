#pragma once
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"
#include "DALEC_1080_INDICES.c"

int DALEC_1080_MODCONFIG(DALEC * DALECmodel){

struct DALEC_1080_PARAMETERS P=DALEC_1080_PARAMETERS;
struct DALEC_1080_FLUXES F=DALEC_1080_FLUXES;
struct DALEC_1080_POOLS S=DALEC_1080_POOLS;

DALECmodel->nopools=8*3+1;
DALECmodel->nomet=9;/*This should be compatible with CBF file, if not then disp error*/
DALECmodel->nopars=37*2;
DALECmodel->nofluxes=32*2+4;

//declaring observation operator structure, and filling with DALEC configurations
static OBSOPE OBSOPE;
//Initialize all SUPPORT OBS values (default value = false).
INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);

//Set SUPPORT_OBS values to true if model supports observation operation.
OBSOPE.SUPPORT_ABGB_OBS=true;
OBSOPE.SUPPORT_CWOO_OBS=true;
OBSOPE.SUPPORT_DOM_OBS=true;
OBSOPE.SUPPORT_ET_OBS=true;
OBSOPE.SUPPORT_EWT_OBS=true;
OBSOPE.SUPPORT_FIR_OBS=true;
OBSOPE.SUPPORT_GPP_OBS=true;
//OBSOPE.SUPPORT_LAI_OBS=false;
OBSOPE.SUPPORT_iLAI_OBS=true;
OBSOPE.SUPPORT_NBE_OBS=true;

// OBSOPE.SUPPORT_CUE_OBS=true;
// OBSOPE.SUPPORT_Cefficiency_OBS=true;
// OBSOPE.SUPPORT_iniSOM_OBS=true;



//Provide values required by each OBS operator
//Note: each OBS operator requirements are unique, see individual observation operator functions to see what's required 
//Note: no values required for any SUPPORT_*_OBS quantity set to false.

//GPP-specific variables
OBSOPE.GPP_flux=F.gpp;
//LAI-specific variables
//OBSOPE.LAI_foliar_pool=S.C_fol;
//OBSOPE.LAI_LCMA=P.LCMA;
//ET variabiles
OBSOPE.ET_flux=F.et;
//NBE-specific variables
static int NBE_fluxes[1];
NBE_fluxes[0]=F.nbe;

OBSOPE.NBE_fluxes=NBE_fluxes;
static double NBE_flux_signs[]={1};
OBSOPE.NBE_flux_signs=NBE_flux_signs;
OBSOPE.NBE_n_fluxes=1;

//ABGB-specific variables
static int ABGB_pools[4];
ABGB_pools[0]=S.C_lab;
ABGB_pools[1]=S.C_fol;
ABGB_pools[2]=S.C_roo;
ABGB_pools[3]=S.C_woo;
OBSOPE.ABGB_pools=ABGB_pools;
OBSOPE.ABGB_n_pools=4;

//ABGB-specific variables
static int CWOO_pools[1];
CWOO_pools[0]=S.C_woo;
OBSOPE.CWOO_pools=CWOO_pools;
OBSOPE.CWOO_n_pools=1;

//DOM-specific variables
static int DOM_pools[2]; 
DOM_pools[0]=S.C_lit;
DOM_pools[1]=S.C_som;
OBSOPE.DOM_pools=DOM_pools;
OBSOPE.DOM_n_pools=2;
//H2O-specific variables
static int EWT_h2o_pools[2];
EWT_h2o_pools[0]=S.H2O_PAW;
EWT_h2o_pools[1]=S.H2O_PUW;
OBSOPE.EWT_h2o_pools=EWT_h2o_pools;
OBSOPE.EWT_n_h2o_pools=2;
//Fire-specific variables
OBSOPE.FIR_flux=F.f_total;

//CUE parameters
// OBSOPE.CUE_PARAM=P.f_auto;
// OBSOPE.Cefficiency_PARAM=P.canopy_eff;
// OBSOPE.iniSOM_PARAM=P.i_soil;





DALECmodel->OBSOPE=OBSOPE;
return 0;}



int DALEC_1080(DATA DATA, double const *pars)
{

struct DALEC_1080_PARAMETERS P=DALEC_1080_PARAMETERS;
struct DALEC_1080_FLUXES F=DALEC_1080_FLUXES;
struct DALEC_1080_POOLS S=DALEC_1080_POOLS;

double gpppars[11];
/*C-pools, fluxes, meteorology indices*/
int p=0,f,m,nxp, i;
int n=0,nn=0;
double pi=3.1415927;


/*constant gpppars terms*/
gpppars[3]=1;
gpppars[6]=DATA.ncdf_data.LAT;
gpppars[8]=-2.0;
gpppars[9]=1.0;
gpppars[10]=pi;

double deltat=DATA.ncdf_data.TIME_INDEX.values[1] - DATA.ncdf_data.TIME_INDEX.values[0];
int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;


 double constants_1[10]={pars[P.canopy_eff_1],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};
 double constants_2[10]={pars[P.canopy_eff_2],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};

/*Pointer transfer - all data stored in fluxes and pools will be passed to DATA*/
double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;
double *LAI=DATA.M_LAI;

  /*assigning values to pools*/
  /*L,F,R,W,Lit,SOM*/
  POOLS[S.C_lab_1]=pars[P.i_labile_1];
  POOLS[S.C_lab_2]=pars[P.i_labile_2];
  POOLS[S.C_fol_1]=pars[P.i_foliar_1];
  POOLS[S.C_fol_2]=pars[P.i_foliar_2];
  POOLS[S.C_roo_1]=pars[P.i_root_1];
  POOLS[S.C_roo_2]=pars[P.i_root_2];
  POOLS[S.C_woo_1]=pars[P.i_wood_1];
  POOLS[S.C_woo_2]=pars[P.i_wood_2];
  POOLS[S.C_lit_1]=pars[P.i_lit_1];
  POOLS[S.C_lit_2]=pars[P.i_lit_2];
  POOLS[S.C_som_1]=pars[P.i_soil_1];
  POOLS[S.C_som_2]=pars[P.i_soil_2];
  /*water pools*/
  POOLS[S.H2O_PAW_1]=pars[P.i_PAW_1];
  POOLS[S.H2O_PAW_2]=pars[P.i_PAW_2];
  POOLS[S.H2O_PUW_1]=pars[P.i_PUW_1];
  POOLS[S.H2O_PUW_2]=pars[P.i_PUW_2];
  
  
POOLS[S.C_lab]=( POOLS[S.C_lab_1]+  POOLS[S.C_lab_2])/2;
POOLS[S.C_fol]=(POOLS[S.C_fol_1]+POOLS[S.C_fol_2])/2;
POOLS[S.C_roo]=(  POOLS[S.C_roo_1]+  POOLS[S.C_roo_2])/2;
POOLS[S.C_woo]=(  POOLS[S.C_woo_1]+  POOLS[S.C_woo_2])/2;
POOLS[S.C_lit]=(  POOLS[S.C_lit_1]+  POOLS[S.C_lit_2])/2;
POOLS[S.C_som]=(  POOLS[S.C_som_1]+  POOLS[S.C_som_2])/2;
POOLS[S.H2O_PAW]=(  POOLS[S.H2O_PAW_1]+  POOLS[S.H2O_PAW_2])/2;
POOLS[S.H2O_PUW]=(  POOLS[S.H2O_PUW_1]+  POOLS[S.H2O_PUW_2])/2;
  

    POOLS[S.LAI]=(POOLS[S.C_fol_1]/pars[P.LCMA_1] + POOLS[S.C_fol_2]/pars[P.LCMA_2])/2;

  
  
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

double sf=365.25/pi;


double wf[2], wl[2],ff[2],fl[2],osf[2],osl[2];
double CF[16];

for (m=0;m<2;m++){
/*constants for exponents of leaffall and labrelease factors*/
/*width*/

wf[m]=pars[P.leaf_fall_1+m]*sqrt(2)/2;
wl[m]=pars[P.labile_rel_1+m]*sqrt(2)/2;
 ff[m]=(log(pars[P.t_foliar_1+m])-log(pars[P.t_foliar_1+m]-1))/2;
fl[m]=(log(pars[P.t_labile_1+m])-log(pars[P.t_labile_1+m]-1))/2;
osf[m]=offset(pars[P.t_foliar_1+m],wf[m]);
osl[m]=offset(pars[P.t_labile_1+m],wl[m]);


/*scaling to biyearly sine curve*/

/*Combustion factors*/
CF[S.C_lab_1+m]=pars[P.cf_ligneous_1+m];

CF[S.C_fol_1+m]=pars[P.cf_foliar_1+m];

CF[S.C_roo_1+m]=pars[P.cf_ligneous_1+m];


CF[S.C_woo_1+m]=pars[P.cf_ligneous_1+m];


CF[S.C_lit_1+m]=pars[P.cf_foliar_1+m]/2+pars[P.cf_ligneous_1+m]/2;

CF[S.C_som_1+m]=pars[P.cf_DOM_1+m];



}



/*resilience factor*/


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
/*flux array index*/
f=nofluxes*n;




gpppars[1]=T2M_MAX[n];
gpppars[2]=T2M_MIN[n];
gpppars[4]=CO2[n];
gpppars[5]=DOY[n];
gpppars[7]=SSRD[n];

/*LAI*/


/*GPP*/


/*GPP*/
gpppars[0]=POOLS[p+S.C_fol_1]/pars[P.LCMA_2]; 
FLUXES[f+F.gpp_1]=ACM(gpppars,constants_1)*fmin(POOLS[p+S.H2O_PAW_1]/pars[P.wilting_1],1);
gpppars[0]=POOLS[p+S.C_fol_2]/pars[P.LCMA_2]; 
FLUXES[f+F.gpp_2]=ACM(gpppars,constants_2)*fmin(POOLS[p+S.H2O_PAW_2]/pars[P.wilting_2],1);

FLUXES[f+F.gpp]=FLUXES[f+F.gpp_1]/2+FLUXES[f+F.gpp_2]/2;

for (m=0;m<2;m++){

/*Evapotranspiration*/
FLUXES[f+F.et_1+m]=FLUXES[f+F.gpp_1+m]*sqrt(VPD[n])/pars[P.uWUE_1+m]+SSRD[n]*pars[P.boese_r_1+m];


/*temprate - now comparable to Q10 - factor at 0C is 1*/
/* x (1 + a* P/P0)/(1+a)*/
FLUXES[f+F.temprate_1+m]=exp(pars[P.temp_factor_1+m]*0.5*(T2M_MIN[n]+T2M_MAX[n]-meantemp))*((PREC[n]/meanprec-1)*pars[P.moisture_1+m]+1);
/*respiration auto*/
FLUXES[f+F.resp_auto_1+m]=pars[P.f_auto_1+m]*FLUXES[f+F.gpp_1+m];
/*leaf production*/
FLUXES[f+F.fol_prod_1+m]=(FLUXES[f+F.gpp_1+m]-FLUXES[f+F.resp_auto_1+m])*pars[P.f_foliar_1+m];
/*labile production*/
FLUXES[f+F.lab_prod_1+m] = (FLUXES[f+F.gpp_1+m]-FLUXES[f+F.resp_auto_1+m]-FLUXES[f+F.fol_prod_1+m])*pars[P.f_lab_1+m];              
/*root production*/        
FLUXES[f+F.root_prod_1+m] = (FLUXES[f+F.gpp_1+m]-FLUXES[f+F.resp_auto_1+m]-FLUXES[f+F.fol_prod_1+m]-FLUXES[f+F.lab_prod_1+m])*pars[P.f_root_1+m];            
/*wood production*/       
FLUXES[f+F.wood_prod_1+m] = FLUXES[f+F.gpp_1+m]-FLUXES[f+F.resp_auto_1+m]-FLUXES[f+F.fol_prod_1+m]-FLUXES[f+F.root_prod_1+m]-FLUXES[f+F.lab_prod_1+m]; 
/*leaf fall factor*/
FLUXES[f+F.leaffall_fact_1+m] = (2/sqrt(pi))*(ff[m]/wf[m])*exp(-pow(sin((TIME_INDEX[n]-pars[P.Fday_1+m]+osf[m])/sf)*sf/wf[m],2));
/*Labrelease factor*/
FLUXES[f+F.lab_release_fact_1+m]=(2/sqrt(pi))*(fl[m]/wl[m])*exp(-pow(sin((TIME_INDEX[n]-pars[P.Bday_1+m]+osl[m])/sf)*sf/wl[m],2));

/*labile release - re-arrange order in next versions*/
FLUXES[f+F.lab_release_1+m] = POOLS[p+S.C_lab_1+m]*(1-pow(1-FLUXES[f+F.lab_release_fact_1+m],deltat))/deltat;
/*leaf litter production*/       
FLUXES[f+F.fol2lit_1+m] = POOLS[p+S.C_fol_1+m]*(1-pow(1-FLUXES[f+F.leaffall_fact_1+m],deltat))/deltat;
/*wood litter production*/       
FLUXES[f+F.wood2lit_1+m] = POOLS[p+S.C_woo_1+m]*(1-pow(1-pars[P.t_wood_1+m],deltat))/deltat;
/*root litter production*/
FLUXES[f+F.root2lit_1+m] = POOLS[p+S.C_roo_1+m]*(1-pow(1-pars[P.t_root_1+m],deltat))/deltat;
/*respiration heterotrophic litter*/
FLUXES[f+F.resp_het_lit_1+m] = POOLS[p+S.C_lit_1+m]*(1-pow(1-FLUXES[f+F.temprate_1+m]*pars[P.t_lit_1+m],deltat))/deltat;
/*respiration heterotrophic SOM*/
FLUXES[f+F.resp_het_som_1+m] = POOLS[p+S.C_som_1+m]*(1-pow(1-FLUXES[f+F.temprate_1+m]*pars[P.t_soil_1+m],deltat))/deltat;
/*litter to SOM*/
FLUXES[f+F.lit2som_1+m] = POOLS[p+S.C_lit_1+m]*(1-pow(1-pars[P.tr_lit2soil_1+m]*FLUXES[f+F.temprate_1+m],deltat))/deltat;

/*total pool transfers (no fires yet)*/

        POOLS[nxp+S.C_lab_1+m] = POOLS[p+S.C_lab_1+m] + (FLUXES[f+F.lab_prod_1+m]-FLUXES[f+F.lab_release_1+m])*deltat;
        POOLS[nxp+S.C_fol_1+m] = POOLS[p+S.C_fol_1+m] + (FLUXES[f+F.fol_prod_1+m] - FLUXES[f+F.fol2lit_1+m] + FLUXES[f+F.lab_release_1+m])*deltat;
        POOLS[nxp+S.C_roo_1+m] = POOLS[p+S.C_roo_1+m] + (FLUXES[f+F.root_prod_1+m] - FLUXES[f+F.root2lit_1+m])*deltat;
        POOLS[nxp+S.C_woo_1+m] = POOLS[p+S.C_woo_1+m] +  (FLUXES[f+F.wood_prod_1+m] - FLUXES[f+F.wood2lit_1+m])*deltat;
        POOLS[nxp+S.C_lit_1+m] = POOLS[p+S.C_lit_1+m] + (FLUXES[f+F.fol2lit_1+m] + FLUXES[f+F.root2lit_1+m] - FLUXES[f+F.resp_het_lit_1+m] - FLUXES[f+F.lit2som_1+m])*deltat; 
        POOLS[nxp+S.C_som_1+m]= POOLS[p+S.C_som_1+m]+ (FLUXES[f+F.lit2som_1+m] - FLUXES[f+F.resp_het_som_1+m]+FLUXES[f+F.wood2lit_1+m])*deltat;                    
/*Water pool = Water pool - runoff + prec (mm/day) - ET*/
	/*printf("%2.1f\n",POOLS[p+S.H2O_PAW]);*/
	/*PAW total runoff*/

	FLUXES[f+F.q_paw_1+m]=pow(POOLS[p+S.H2O_PAW_1+m],2)/pars[P.PAW_Qmax_1+m]/deltat*(1-pars[P.h2o_xfer_1+m]);	
        /*PAW -> PUW transfer*/
	FLUXES[f+F.paw2puw_1+m]=FLUXES[f+F.q_paw_1+m]*pars[P.h2o_xfer_1+m]/(1-pars[P.h2o_xfer_1+m]);
	/*PUW runoff*/
	FLUXES[f+F.q_puw_1+m]=pow(POOLS[p+S.H2O_PUW_1+m],2)/pars[P.PUW_Qmax_1+m]/deltat;
	/*Maximum water loss at W = pars[P.PAW_Qmax]/2;*/
	if (POOLS[p+S.H2O_PAW_1+m]>pars[P.PAW_Qmax_1+m]/2){FLUXES[f+F.q_paw_1+m]=(POOLS[p+S.H2O_PAW_1+m]-pars[P.PAW_Qmax_1+m]/4)/deltat*(1-pars[P.h2o_xfer_1+m]);
        FLUXES[f+F.paw2puw_1+m]=(POOLS[p+S.H2O_PAW_1+m]-pars[P.PAW_Qmax_1+m]/4)/deltat*pars[P.h2o_xfer_1+m]/(1-pars[P.h2o_xfer_1+m]);}
	if (POOLS[p+S.H2O_PUW_1+m]>pars[P.PUW_Qmax_1+m]/2){FLUXES[f+F.q_puw_1+m]=(POOLS[p+S.H2O_PUW_1+m]-pars[P.PUW_Qmax_1+m]/4)/deltat;}

	POOLS[nxp+S.H2O_PAW_1+m]=POOLS[p+S.H2O_PAW_1+m] + (-FLUXES[f+F.q_paw_1+m] - FLUXES[f+F.paw2puw_1+m] + PREC[n] - FLUXES[f+F.et_1+m])*deltat;

		
	/*Plant-unavailable water budget*/

        POOLS[nxp+S.H2O_PUW_1+m]=POOLS[p+S.H2O_PUW_1+m] + (FLUXES[f+F.paw2puw_1+m] - FLUXES[f+F.q_puw_1+m])*deltat;



	/*total pool transfers - WITH FIRES*/
	/*first fluxes*/

	/*CFF = Combusted C fire flux
	NCFF = Non-combusted C fire flux*/

	/*Calculating all fire transfers (1. combustion, and 2. litter transfer)*/
	/*note: all fluxes are in gC m-2 day-1*/
        double BA=BURNED_AREA[n];
    FLUXES[f+F.f_lab_1+m] = POOLS[nxp+S.C_lab_1+m]*BA*CF[S.C_lab_1+m]/deltat;
    FLUXES[f+F.f_fol_1+m] = POOLS[nxp+S.C_fol_1+m]*BA*CF[S.C_fol_1+m]/deltat;
    FLUXES[f+F.f_roo_1+m] = POOLS[nxp+S.C_roo_1+m]*BA*CF[S.C_roo_1+m]/deltat;
    FLUXES[f+F.f_woo_1+m] = POOLS[nxp+S.C_woo_1+m]*BA*CF[S.C_woo_1+m]/deltat;
    FLUXES[f+F.f_lit_1+m] = POOLS[nxp+S.C_lit_1+m]*BA*CF[S.C_lit_1+m]/deltat;
    FLUXES[f+F.f_som_1+m] = POOLS[nxp+S.C_som_1+m]*BA*CF[S.C_som_1+m]/deltat;

    FLUXES[f+F.fx_lab2lit_1+m] = POOLS[nxp+S.C_lab_1+m]*BA*(1-CF[S.C_lab_1+m])*(1-pars[P.resilience_1+m])/deltat;
    FLUXES[f+F.fx_fol2lit_1+m] = POOLS[nxp+S.C_fol_1+m]*BA*(1-CF[S.C_fol_1+m])*(1-pars[P.resilience_1+m])/deltat;
    FLUXES[f+F.fx_roo2lit_1+m] = POOLS[nxp+S.C_roo_1+m]*BA*(1-CF[S.C_roo_1+m])*(1-pars[P.resilience_1+m])/deltat;
    FLUXES[f+F.fx_woo2som_1+m] = POOLS[nxp+S.C_woo_1+m]*BA*(1-CF[S.C_woo_1+m])*(1-pars[P.resilience_1+m])/deltat;
    FLUXES[f+F.fx_lit2som_1+m] = POOLS[nxp+S.C_lit_1+m]*BA*(1-CF[S.C_lit_1+m])*(1-pars[P.resilience_1+m])/deltat;

	/*Adding all fire pool transfers here*/
	/*live C pools*/	
    POOLS[nxp+S.C_lab_1+m] = POOLS[nxp+S.C_lab_1+m]-(FLUXES[f+F.f_lab_1+m]+FLUXES[f+F.fx_lab2lit_1+m])*deltat;
    POOLS[nxp+S.C_fol_1+m] = POOLS[nxp+S.C_fol_1+m]-(FLUXES[f+F.f_fol_1+m]+FLUXES[f+F.fx_fol2lit_1+m])*deltat;
    POOLS[nxp+S.C_roo_1+m] = POOLS[nxp+S.C_roo_1+m]-(FLUXES[f+F.f_roo_1+m]+FLUXES[f+F.fx_roo2lit_1+m])*deltat;
    POOLS[nxp+S.C_woo_1+m] = POOLS[nxp+S.C_woo_1+m]-(FLUXES[f+F.f_woo_1+m]+FLUXES[f+F.fx_woo2som_1+m])*deltat;
	/*dead C pools*/
	/*litter*/
	POOLS[nxp+S.C_lit_1+m] = POOLS[nxp+S.C_lit_1+m]+(FLUXES[f+F.fx_lab2lit_1+m]+FLUXES[f+F.fx_fol2lit_1+m]+FLUXES[f+F.fx_roo2lit_1+m]-FLUXES[f+F.f_lit_1+m]-FLUXES[f+F.fx_lit2som_1+m])*deltat;
	/*som*/
	POOLS[nxp+S.C_som_1+m] = POOLS[nxp+S.C_som_1+m]+(FLUXES[f+F.fx_woo2som_1+m]+FLUXES[f+F.fx_lit2som_1+m]-FLUXES[f+F.f_som_1+m])*deltat;

	/*fires - total flux in gC m-2 day-1*/
	/*this term is now (essentially) obsolete*/
	/*replace in next version of DALEC_FIRES*/
    FLUXES[f+F.f_total_1+m] = FLUXES[f+F.f_lab_1+m] + FLUXES[f+F.f_fol_1+m] + FLUXES[f+F.f_roo_1+m] + FLUXES[f+F.f_woo_1+m] + FLUXES[f+F.f_lit_1+m] + FLUXES[f+F.f_som_1+m];


}


    FLUXES[f+F.f_total] =(FLUXES[f+F.f_total_1] + FLUXES[f+F.f_total_2])/2;
    FLUXES[f+F.nbe]=(FLUXES[f+F.resp_auto_1] + FLUXES[f+F.resp_het_lit_1] + FLUXES[f+F.resp_het_som_1] + FLUXES[f+F.f_total_1] - FLUXES[f+F.gpp_1])/2 +
                             (FLUXES[f+F.resp_auto_2] + FLUXES[f+F.resp_het_lit_2] + FLUXES[f+F.resp_het_som_2] + FLUXES[f+F.f_total_2] - FLUXES[f+F.gpp_2])/2;
    
    FLUXES[f+F.et]=(FLUXES[f+F.et_1] + FLUXES[f+F.et_2])/2;
    POOLS[nxp+S.C_lab]=(POOLS[nxp+S.C_lab_1]+POOLS[nxp+S.C_lab_2])/2;
    POOLS[nxp+S.C_fol]=(POOLS[nxp+S.C_fol_1]+POOLS[nxp+S.C_fol_2])/2;
    POOLS[nxp+S.C_roo]=(POOLS[nxp+S.C_roo_1]+POOLS[nxp+S.C_roo_2])/2;
    POOLS[nxp+S.C_woo]=(POOLS[nxp+S.C_woo_1]+POOLS[nxp+S.C_woo_2])/2;
    POOLS[nxp+S.C_lit]=(POOLS[nxp+S.C_lit_1]+POOLS[nxp+S.C_lit_2])/2;
    POOLS[nxp+S.C_som]=(POOLS[nxp+S.C_som_1]+POOLS[nxp+S.C_som_2])/2;
    POOLS[nxp+S.H2O_PAW]=(POOLS[nxp+S.H2O_PAW_1]+POOLS[nxp+S.H2O_PAW_2])/2;
    POOLS[nxp+S.H2O_PUW]= (POOLS[nxp+S.H2O_PUW_1]+POOLS[nxp+S.H2O_PUW_2])/2;
//Derive LAI
    
    POOLS[nxp+S.LAI]=(POOLS[nxp+S.C_fol_1]/pars[P.LCMA_1] + POOLS[nxp+S.C_fol_2]/pars[P.LCMA_2])/2;
    //inline (inbuilt) LAI observartion operator
    LAI[n]=(POOLS[nxp+S.LAI] + POOLS[p+S.LAI])/2;


}


return 0;
}







