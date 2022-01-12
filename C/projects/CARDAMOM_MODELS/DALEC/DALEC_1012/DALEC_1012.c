#pragma once
//Note: DALEC_OBSERVATION_OPERATORS.c included in DALEC_MODULE.
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"
#include "../../../DALEC_CODE/DALEC_ALL/ACM.c"
#include "../../../DALEC_CODE/DALEC_ALL/offset.c"
#include "../../../DALEC_CODE/DALEC_ALL/CH4_MODULES/JCR.c"

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997
DALEC 1010 is DALEC-JCR v1,
DALEC 1011 is DALEC-JCR v2, with updated ET and replaced Prec with Infiltration
for PAW balance equation, two new output fluxes and two new pars
DALEC 1012 is revised v2, with corrected ET (not sure if this is a correction), using uWUE instead of iWUE*/


struct DALEC_1012_PARAMETERS{
/*DALEC PARAMETERS*/
int tr_lit2som;/*Decomposition rate*/
int f_auto;/*Fraction of GPP respired*/
int f_foliar;/*Fraction of (1-fgpp) to foliage*/
int f_root;/*Fraction of (1-fgpp) to roots*/
int t_foliar;/*Leaf Lifespan*/
int t_wood;/*TOR wood* - 1% loss per year value*/
int t_root;/*TOR roots*/
int t_lit;/*TOR litter*/
int t_som;/*TOR SOM*/
int Q10rhco2;/*Q10_rhco2*/
int canopy_eff;/*Canopy Efficiency*/
int Bday;/*Bday*/
int f_lab;/*Fraction to Clab*/
int labile_rel;/*Clab Release period*/
int Fday;/*Fday*/
int leaf_fall;/*Leaf fall period*/
int LCMA;/*LMCA*/
int i_labile;/*C labile*/
int i_foliar;/*C foliar*/
int i_root;/*C roots*/
int i_wood;/*C_wood*/
int i_lit;/*C litter*/
int i_som;/*C_som*/
int uWUE;/*uWUE: GPP*sqrt(VPD)/ET: gC/kgH2o *hPa*/
int PAW_Qmax;/*Runoff focal point (~maximum soil storage capacity x 4)*/
int wilting;/*"Wilting point"*/
int i_PAW;/*"Bucket at t0"*/
int cf_foliar;/*Foliar biomass CF*/
int cf_ligneous;/*"Ligneous" biomass CF".*/
int cf_DOM;/*DOM CF".*/
int resilience;/*Resilience factor (since transfer to litter is represented as (1-pars[30])) ".*/
int t_labile; /*Lab pool lifespan*/
int moisture;/*Moisture factor*/
int h2o_xfer; /*PAW->PUW runoff fraction*/
int PUW_Qmax; /*PUW Runoff focal point (~maximum soil storage capacity x 4)*/
int i_PUW;/*PUW pool*/
int PAW_fs;/*jc PAW_fs   fully saturated PAW*/
int S_fv;/*S_fv, scalar for aerobic volumetric fraction */
int thetas_opt;/*jc thetas_opt   optimum thetas for water scaler fW*/
int fwc;/*jc fwc the water scaler fW value at the end point C  */
int r_ch4;/*jc r_ch4   CH4:CO2 conversion ratio*/
int Q10ch4;/*jc Q10ch4 Q10 for CH4 production  */
int boese_r;/*jc v2 rET radiation coefficient in ET, unitless, follow Yan's ET scheme */
int Imax;/*jc v2 Imax maximum infiltration rate, mm/day same with prec, partially follow Elias's hydrology */
} DALEC_1012_PARAMETERS={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43
};

struct DALEC_1012_FLUXES{
/*DALEC FLUXES*/
int gpp;   /*GPP*/
int temprate;   /*Temprate*/
int resp_auto;   /*Autotrophic respiration*/
int fol_prod;   /*Foliar production*/
int lab_prod;   /*Labile production*/
int root_prod;   /*Root production*/
int wood_prod;   /*Wood production*/
int lab_release;   /*Labile release*/
int leaffall_fact;   /*Leaffall factor*/
int fol2lit; /*leaflitter_production 10 */
int wood2lit; /*woodlitter_production*/
int root2lit;   /*rootlitter_production*/
int resp_het_lit;   /*nominaly there, not actual fluxes Litter heterotrophic respiration*/
int resp_het_som;   /*nominaly there, not actual fluxes Soil heterotrophic respiration*/
int lit2som;   /*lit2som*/
int lab_release_fact;   /*Labile release factor*/
int f_total;   /*Flux total*/
int f_lab;   /*Labile fire loss*/
int f_fol;   /*Foliar fire loss*/
int f_roo;   /*Wood fire loss 20 */
int f_woo;   /*Root fire loss*/
int f_lit;   /*Litter fire loss*/
int f_som;   /*Soil fire loss*/
int fx_lab2lit;   /*Fire transfer labile to litter*/
int fx_fol2lit;   /*Fire transfer foliar to litter*/
int fx_roo2lit;   /*Fire transfer root to litter*/
int fx_woo2som;   /*Fire transfer wood to soil*/
int fx_lit2som;   /*Fire transfer litter to soil*/
int et;   /*Evapotranspiration  Boese et al 2017 and Yan et al 2021*/
int q_paw;   /*PAW runoff 30 */
int paw2puw;   /*PAW->PUW transfer*/
int q_puw;   /*PUW runoff*/
int ae_rh_lit; /*Aerobic Rh from litter*/
int ae_rh_som; /*aerobic Rh from SOM*/
int an_rh_lit; /*anaerobic Rh from litter*/
int an_rh_som; /*anaerobic Rh from SOM*/
int rh_co2; /* */
int rh_ch4; /* */
int fV; /* Volumetric fraction of aerobic Rh */
int fT; /*Temperature scaler 40 */
int fW; /*Water scaler*/
int fCH4; /*CH4 fraction in anaerobic C decomposition*/
int soil_moist; /*thetas = PAW/PAW_fs fraction*/
int infilt_rate; /*Infiltration rate; mm/day*/
int runoff_rate; /*Runoff rate, mm/day*/
} DALEC_1012_FLUXES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44
};



struct DALEC_1012_POOLS{
/*DALEC POOLS*/
int C_lab; /*Labile C*/
int C_fol; /*Foliar C*/
int C_roo; /*Root C*/
int C_woo; /*Wood C*/
int C_lit; /*Litter C*/
int C_som; /*Soil C*/
int H2O_PAW; /*Plant available H2O*/
int H2O_PUW; /*Plant unavailable H2O*/
} DALEC_1012_POOLS={
    0,1,2,3,4,5,6,7
};



int DALEC_1012_MODCONFIG(DALEC * DALECmodel){


struct DALEC_1012_PARAMETERS P=DALEC_1012_PARAMETERS;
struct DALEC_1012_FLUXES F=DALEC_1012_FLUXES;
struct DALEC_1012_POOLS S=DALEC_1012_POOLS;

DALECmodel->nopools=8;
DALECmodel->nomet=9;/*This should be compatible with CBF file, if not then disp error*/
DALECmodel->nopars=44;
DALECmodel->nofluxes=45;

//declaring observation operator structure, and filling with DALEC configurations
static OBSOPE OBSOPE;
//Initialize all SUPPORT OBS values (default value = false).
INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);

//Set SUPPORT_OBS values to true if model supports observation operation.
OBSOPE.SUPPORT_GPP_OBS=true;
OBSOPE.SUPPORT_LAI_OBS=true;
OBSOPE.SUPPORT_ET_OBS=true;
OBSOPE.SUPPORT_NBE_OBS=true;
OBSOPE.SUPPORT_ABGB_OBS=true;
OBSOPE.SUPPORT_DOM_OBS=true;
OBSOPE.SUPPORT_EWT_OBS=true;
OBSOPE.SUPPORT_FIR_OBS=true;
OBSOPE.SUPPORT_CH4_OBS=true;

OBSOPE.SUPPORT_CUE_OBS=true;
//Provide values required by each OBS operator
//Note: each OBS operator requirements are unique, see individual observation operator functions to see what's required 
//Note: no values required for any SUPPORT_*_OBS quantity set to false.

//GPP-specific variables
OBSOPE.GPP_flux=F.gpp;
//LAI-specific variables
OBSOPE.LAI_foliar_pool=S.C_fol;
OBSOPE.LAI_LCMA=P.LCMA;
//ET variabiles
OBSOPE.ET_flux=F.et;
//NBE-specific variables
static int NBE_fluxes[4]; /* jc special changes */
NBE_fluxes[0]=F.gpp;
NBE_fluxes[1]=F.resp_auto;
NBE_fluxes[2]=F.rh_co2;
NBE_fluxes[3]=F.f_total;

OBSOPE.NBE_fluxes=NBE_fluxes;
static double NBE_flux_signs[]={-1.,1.,1.,1.};/* jc special changes */
OBSOPE.NBE_flux_signs=NBE_flux_signs;
OBSOPE.NBE_n_fluxes=4;/* jc special changes */

//ABGB-specific variables
static int ABGB_pools[4];
ABGB_pools[0]=S.C_lab;
ABGB_pools[1]=S.C_fol;
ABGB_pools[2]=S.C_roo;
ABGB_pools[3]=S.C_woo;
OBSOPE.ABGB_pools=ABGB_pools;
OBSOPE.ABGB_n_pools=4;


//SOM-specific variables
static int DOM_pools[2]; 
DOM_pools[0]=S.C_lit;
DOM_pools[1]=S.C_som;
OBSOPE.DOM_pools=DOM_pools;
OBSOPE.DOM_n_pools=2;
//H2O-specific variables
static int EWT_h2o_pools[2];/* jc special changes */
EWT_h2o_pools[0]=S.H2O_PAW;
EWT_h2o_pools[1]=S.H2O_PUW;

OBSOPE.EWT_h2o_pools=EWT_h2o_pools;
OBSOPE.EWT_n_h2o_pools=2;
//Fire-specific variables
OBSOPE.FIR_flux=F.f_total;
//CUE parameters
OBSOPE.CUE_PARAM=P.f_auto;



DALECmodel->OBSOPE=OBSOPE;



return 0;}



int DALEC_1012(DATA DATA, double const *pars){

struct DALEC_1012_PARAMETERS P=DALEC_1012_PARAMETERS;
struct DALEC_1012_FLUXES F=DALEC_1012_FLUXES;
struct DALEC_1012_POOLS S=DALEC_1012_POOLS;

double gpppars[11]; /* jc */

/*C-pools, fluxes, meteorology indices*/
int p=0,f,m,nxp, i; /* added =0 mk */
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


double constants[10]={pars[P.canopy_eff],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};

/*Pointer transfer - all data stored in fluxes and pools will be passed to DATA*/
double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;
double *LAI=DATA.M_LAI;


/*assigning values to pools*/

  /*L,F,R,W,Lit,SOM*/
  POOLS[S.C_lab]=pars[P.i_labile];
  POOLS[S.C_fol]=pars[P.i_foliar];
  POOLS[S.C_roo]=pars[P.i_root];
  POOLS[S.C_woo]=pars[P.i_wood];
  POOLS[S.C_lit]=pars[P.i_lit];
  POOLS[S.C_som]=pars[P.i_som];
  /*water pools*/
  POOLS[S.H2O_PAW]=pars[P.i_PAW];
  POOLS[S.H2O_PUW]=pars[P.i_PUW];

/* bring in drivers mk new in main jc */
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

/* jc prep input for methane module*/
double ch4pars[7]={pars[P.S_fv],pars[P.thetas_opt],pars[P.fwc],pars[P.r_ch4],pars[P.Q10ch4],pars[P.Q10rhco2],meantemp};

/*constants for exponents of leaffall and labrelease factors*/
/*width*/
double wf=pars[P.leaf_fall]*sqrt(2)/2;
double wl=pars[P.labile_rel]*sqrt(2)/2;


/*factor*/
double ff=(log(pars[P.t_foliar])-log(pars[P.t_foliar]-1))/2;
/*double fl=(log(1.001)-log(0.001))/2;*/
double fl=(log(pars[P.t_labile])-log(pars[P.t_labile]-1))/2;

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


/*GPP*/
gpppars[0]=LAI[n];
gpppars[1]=T2M_MAX[n];
gpppars[2]=T2M_MIN[n];
gpppars[4]=CO2[n];
gpppars[5]=DOY[n];
gpppars[7]=SSRD[n];



/*GPP*/
FLUXES[f+F.gpp]=ACM(gpppars,constants)*fmin(POOLS[p+S.H2O_PAW]/pars[P.wilting],1);
/*Evapotranspiration*/
FLUXES[f+F.et]=FLUXES[f+F.gpp]*sqrt(VPD[n])/pars[P.uWUE]+SSRD[n]*pars[P.boese_r];
/*temprate - now comparable to Q10 - factor at 0C is 1*/
/* x (1 + a* P/P0)/(1+a)*/
//FLUXES[f+F.temprate]=exp(pars[P.temp_factor]*0.5*(T2M_MIN[n]+T2M_MAX[n]-meantemp))*((PREC[n]/meanprec-1)*pars[P.moisture]+1);
FLUXES[f+F.temprate]=pow(pars[P.Q10rhco2],(0.5*(T2M_MIN[n]+T2M_MAX[n])-meantemp)/10)*((PREC[n]/meanprec-1)*pars[P.moisture]+1);
/*respiration auto*/
FLUXES[f+F.resp_auto]=pars[P.f_auto]*FLUXES[f+F.gpp];
/*leaf production*/
FLUXES[f+F.fol_prod]=(FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto])*pars[P.f_foliar];
/*labile production*/
FLUXES[f+F.lab_prod] = (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.fol_prod])*pars[P.f_lab];              
/*root production*/        
FLUXES[f+F.root_prod] = (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.fol_prod]-FLUXES[f+F.lab_prod])*pars[P.f_root];            
/*wood production*/       
FLUXES[f+F.wood_prod] = FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.fol_prod]-FLUXES[f+F.root_prod]-FLUXES[f+F.lab_prod]; 
/*leaf fall factor*/
FLUXES[f+F.leaffall_fact] = (2/sqrt(pi))*(ff/wf)*exp(-pow(sin((TIME_INDEX[n]-pars[P.Fday]+osf)/sf)*sf/wf,2));
/*Labrelease factor*/
FLUXES[f+F.lab_release_fact]=(2/sqrt(pi))*(fl/wl)*exp(-pow(sin((TIME_INDEX[n]-pars[P.Bday]+osl)/sf)*sf/wl,2));
/*labile release - re-arrange order in next versions*/
FLUXES[f+F.lab_release] = POOLS[p+S.C_lab]*(1-pow(1-FLUXES[f+F.lab_release_fact],deltat))/deltat;
/*leaf litter production*/       
FLUXES[f+F.fol2lit] = POOLS[p+S.C_fol]*(1-pow(1-FLUXES[f+F.leaffall_fact],deltat))/deltat;
/*wood litter production*/       
FLUXES[f+F.wood2lit] = POOLS[p+S.C_woo]*(1-pow(1-pars[P.t_wood],deltat))/deltat;
/*root litter production*/
FLUXES[f+F.root2lit] = POOLS[p+S.C_roo]*(1-pow(1-pars[P.t_root],deltat))/deltat;
/*nominaly there, not actual fluxes respiration heterotrophic litter*/
FLUXES[f+F.resp_het_lit] = POOLS[p+S.C_lit]*(1-pow(1-FLUXES[f+F.temprate]*pars[P.t_lit],deltat))/deltat;
/*nominaly there, not actual fluxes respiration heterotrophic SOM*/
FLUXES[f+F.resp_het_som] = POOLS[p+S.C_som]*(1-pow(1-FLUXES[f+F.temprate]*pars[P.t_som],deltat))/deltat;
/*-----------------------------------------------------------------------*/
/*jc calculate aerobic and anaerobic respirations*/
double thetas = fmin(POOLS[p+S.H2O_PAW]/pars[P.PAW_fs],1);
double *jcr_o = JCR(ch4pars,T2M_MIN[n],T2M_MAX[n],thetas);
//outputformat
//jcr_o 0-3 fT,fV,fW,fCH4; /*jc*/ /* output from JCR module */
/*32 aerobic Rh from litter*/
FLUXES[f+F.ae_rh_lit] = POOLS[p+S.C_lit]*(1-pow(1-jcr_o[2]*jcr_o[0]*jcr_o[1]*pars[P.t_lit],deltat))/deltat;
/*33 aerobic Rh from SOM*/
FLUXES[f+F.ae_rh_som] = POOLS[p+S.C_som]*(1-pow(1-jcr_o[2]*jcr_o[0]*jcr_o[1]*pars[P.t_som],deltat))/deltat;
/*34 anaerobic Rh from litter*/
//FLUXES[f+34] = POOLS[p+S.C_lit]*(1-pow(1-1*fT*(1-jcr_o[1])*pars[P.t_lit],deltat))/deltat;
FLUXES[f+F.an_rh_lit] = POOLS[p+S.C_lit]*(1-pow(1-pars[P.fwc]*jcr_o[0]*(1-jcr_o[1])*pars[P.t_lit],deltat))/deltat;
/*35 anaerobic Rh from SOM*/
//FLUXES[f+] = POOLS[p+S.C_som]*(1-pow(1-1*fT*(1-jcr_o[1])*pars[P.t_som],deltat))/deltat;
FLUXES[f+F.an_rh_som] = POOLS[p+S.C_som]*(1-pow(1-pars[P.fwc]*jcr_o[0]*(1-jcr_o[1])*pars[P.t_som],deltat))/deltat;
/*36 Rh_CO2*/
FLUXES[f+F.rh_co2] = (FLUXES[f+F.ae_rh_lit]+FLUXES[f+F.ae_rh_som])*1+(FLUXES[f+F.an_rh_lit]+FLUXES[f+F.an_rh_som])*(1-jcr_o[3]);
/*37 Rh_CH4*/
FLUXES[f+F.rh_ch4] = (FLUXES[f+F.ae_rh_lit]+FLUXES[f+F.ae_rh_som])*0+(FLUXES[f+F.an_rh_lit]+FLUXES[f+F.an_rh_som])*jcr_o[3];
/*38 fV Volumetric fraction of aerobic Rh*/
FLUXES[f+F.fV] = jcr_o[1];
/*39 fT Temperature scaler*/
FLUXES[f+F.fT] = jcr_o[0];
/*40 fW Water scaler*/
FLUXES[f+F.fW] = jcr_o[2];
/*41 fCH4 CH4 fraction*/
FLUXES[f+F.fCH4] = jcr_o[3];
/*42 PAW/PAW_fs thetas*/
FLUXES[f+F.soil_moist] = thetas;
/* CH4 production=TEMP*RH*WEXT*Q10 */
/*FLUXES[f+32] = ch4pars[0]*(FLUXES[f+12]+FLUXES[f+13])*pow(ch4pars[1],(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-15)/10)*ch4pars[2];*/
/*----------------------  end of JCR  --------------------------------------------*/
/*litter to SOM*/
FLUXES[f+F.lit2som] = POOLS[p+S.C_lit]*(1-pow(1-pars[P.tr_lit2som]*FLUXES[f+F.temprate],deltat))/deltat;


/*total pool transfers (no fires yet)*/

        POOLS[nxp+S.C_lab] = POOLS[p+S.C_lab] + (FLUXES[f+F.lab_prod]-FLUXES[f+F.lab_release])*deltat;
        POOLS[nxp+S.C_fol] = POOLS[p+S.C_fol] + (FLUXES[f+F.fol_prod] - FLUXES[f+F.fol2lit] + FLUXES[f+F.lab_release])*deltat;
        POOLS[nxp+S.C_roo] = POOLS[p+S.C_roo] + (FLUXES[f+F.root_prod] - FLUXES[f+F.root2lit])*deltat;
        POOLS[nxp+S.C_woo] = POOLS[p+S.C_woo] +  (FLUXES[f+F.wood_prod] - FLUXES[f+F.wood2lit])*deltat;
/*-----------------------------------------------------------------------*/
        /*jc*/
        //POOLS[nxp+S.C_lit] = POOLS[p+S.C_lit] + (FLUXES[f+F.fol2lit] + FLUXES[f+F.root2lit] - FLUXES[f+F.resp_het_lit] - FLUXES[f+F.lit2som])*deltat; 
        POOLS[nxp+S.C_lit] = POOLS[p+S.C_lit] + (FLUXES[f+F.fol2lit]+FLUXES[f+F.root2lit]-FLUXES[f+F.ae_rh_lit]-FLUXES[f+F.an_rh_lit]-FLUXES[f+F.lit2som])*deltat;
       // POOLS[nxp+S.C_som] = POOLS[p+S.C_som] + (FLUXES[f+F.lit2som] - FLUXES[f+F.resp_het_som]+FLUXES[f+F.wood2lit])*deltat;                    
        POOLS[nxp+S.C_som] = POOLS[p+S.C_som] + (FLUXES[f+F.lit2som]-FLUXES[f+F.ae_rh_som]-FLUXES[f+F.an_rh_som]+FLUXES[f+F.wood2lit])*deltat;
/*-----------------------------------------------------------------------*/
  /*Water pool = Water pool - runoff + prec (mm/day) - ET*/
  /*printf("%2.1f\n",POOLS[p+S.H2O_PAW]);*/
  /*PAW total runoff*/
  FLUXES[f+F.q_paw]=pow(POOLS[p+S.H2O_PAW],2)/pars[P.PAW_Qmax]/deltat*(1-pars[P.h2o_xfer]); 
  /*PAW -> PUW transfer*/
  FLUXES[f+F.paw2puw]=FLUXES[f+F.q_paw]*pars[P.h2o_xfer]/(1-pars[P.h2o_xfer]);
  /*PUW runoff*/
  FLUXES[f+F.q_puw]=pow(POOLS[p+S.H2O_PUW],2)/pars[P.PUW_Qmax]/deltat;
  /*Maximum water loss at W = pars[P.PAW_Qmax]/2;*/
  if (POOLS[p+S.H2O_PAW]>pars[P.PAW_Qmax]/2){FLUXES[f+F.q_paw]=(POOLS[p+S.H2O_PAW]-pars[P.PAW_Qmax]/4)/deltat*(1-pars[P.h2o_xfer]);
  FLUXES[f+F.paw2puw]=(POOLS[p+S.H2O_PAW]-pars[P.PAW_Qmax]/4)/deltat*pars[P.h2o_xfer]/(1-pars[P.h2o_xfer]);}
  if (POOLS[p+S.H2O_PUW]>pars[P.PUW_Qmax]/2){FLUXES[f+F.q_puw]=(POOLS[p+S.H2O_PUW]-pars[P.PUW_Qmax]/4)/deltat;}
  /*-----------------------------------------------------------------------*/
  /*updates in DALEC-JCR v2*/
  /*I =Imax *(1-exp(-Pt/Imax))
  PAW = PAW + (-PAWrunoff – PAW->PUWtransfer + I- ET)*deltat;*/
  /*Infiltration rate*/
  FLUXES[f+F.infilt_rate]=pars[P.Imax]*(1-exp(-PREC[n]/pars[43]));
  /*runoff rate*/
  FLUXES[f+F.runoff_rate]=PREC[n]-FLUXES[f+F.infilt_rate];
  /*Plant-available water budget*/
  POOLS[nxp+S.H2O_PAW]=POOLS[p+S.H2O_PAW] + (-FLUXES[f+F.q_paw] - FLUXES[f+F.paw2puw] + FLUXES[f+F.infilt_rate] - FLUXES[f+F.et])*deltat;
  /*----  end of updates in JCR v2  ---------------------------------------------------*/
  
  /*Plant-unavailable water budget*/
  POOLS[nxp+S.H2O_PUW]=POOLS[p+S.H2O_PUW] + (FLUXES[f+F.paw2puw] - FLUXES[f+F.q_puw])*deltat;



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


}

return 0;
}
