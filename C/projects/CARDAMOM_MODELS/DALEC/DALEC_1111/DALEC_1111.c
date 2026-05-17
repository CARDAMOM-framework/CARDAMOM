#pragma once
//Note: DALEC_OBSERVATION_OPERATORS.c included in DALEC_MODULE.
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/DRAINAGE.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_EWT2MOI.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2EWT.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2CON.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2PSI.c"
#include "../../../DALEC_CODE/DALEC_ALL/LIU_An_et.c"
#include "../../../DALEC_CODE/DALEC_ALL/CH4_MODULES/JCR.c"

/*
 * DALEC_1111: DALEC-PyC
 * Extends DALEC_1110 with pyrogenic carbon (PyC) dynamics.
 *
 * New features:
 *   1. Climate-sensitive combustion factors: CF_lig(t) = CF_max * f(psi_PAW) * g(VPD)
 *   2. VIIRS FRP-driven combustion efficiency (CE) → PyC yield
 *   3. Two PyC pools: C_PyC_L (labile, O-horizon) and C_PyC_R (refractory, mineral)
 *   4. NISAR L-band ΔAGB observation operator via SUPPORT_NISAR_WOOD_OBS
 *
 * Key references:
 *   - Jones et al. (2019) NatGeo: alpha_char by fire type
 *   - Wooster et al. (2005) RSE: FRE-combustion linearity
 *   - Abney et al. (2019): dual-pool PyC architecture
 *   - Akagi et al. (2011): CE sensitivity to atmospheric drying
 *
 * Branch: renato/CARDAMOM-PyC (NISAR DART ROSES-2025)
 */


/* ======================================================================
 * PARAMETER STRUCT
 * Inherits all 63 DALEC_1110 parameters (indices 0-125).
 * Adds 10 new PyC parameters (indices 126-145).
 * Convention: each named param has a "2" pair as reserved slot.
 * nopars = 73*2 = 146
 * ====================================================================== */

struct DALEC_1111_PARAMETERS{
/* Inherited from DALEC_1110 */
int tr_lit2soil;   int tr_lit2soil2;
int tr_cwd2som;    int tr_cwd2som2;
int f_auto;        int f_auto2;
int f_foliar;      int f_foliar2;
int f_root;        int f_root2;
int t_foliar;      int t_foliar2;
int t_wood;        int t_wood2;
int t_root;        int t_root2;
int t_lit;         int t_lit2;
int t_cwd;         int t_cwd2;
int t_soil;        int t_soil2;
int Q10rhco2;      int Q10rhco22;
int Bday;          int Bday2;
int f_lab;         int f_lab2;
int labile_rel;    int labile_rel2;
int Fday;          int Fday2;
int leaf_fall;     int leaf_fall2;
int LCMA;          int LCMA2;
int i_labile;      int i_labile2;
int i_foliar;      int i_foliar2;
int i_root;        int i_root2;
int i_wood;        int i_wood2;
int i_cwd;         int i_cwd2;
int i_lit;         int i_lit2;
int i_soil;        int i_soil2;
int retention;     int retention2;
int wilting;       int wilting2;
int i_PAW;         int i_PAW2;
int cf_foliar;     int cf_foliar2;
int cf_ligneous;   int cf_ligneous2;
int cf_DOM;        int cf_DOM2;
int resilience;    int resilience2;
int t_labile;      int t_labile2;
int moisture;      int moisture2;
int hydr_cond;     int hydr_cond2;
int max_infil;     int max_infil2;
int i_PUW;         int i_PUW2;
int PAW_por;       int PAW_por2;
int PUW_por;       int PUW_por2;
int field_cap;     int field_cap2;
int PAW_z;         int PAW_z2;
int PUW_z;         int PUW_z2;
int Q_excess;      int Q_excess2;
int Med_g1;        int Med_g12;
int Vcmax25;       int Vcmax252;
int Tminmin;       int Tminmin2;
int Tminmax;       int Tminmax2;
int ga;            int ga2;
int Tupp;          int Tupp2;
int Tdown;         int Tdown2;
int C3_frac;       int C3_frac2;
int clumping;      int clumping2;
int leaf_refl;     int leaf_refl2;
int i_SWE;         int i_SWE2;
int min_melt;      int min_melt2;
int melt_slope;    int melt_slope2;
int scf_scalar;    int scf_scalar2;
int S_fv;          int S_fv2;
int thetas_opt;    int thetas_opt2;
int fwc;           int fwc2;
int r_ch4;         int r_ch42;
int Q10ch4;        int Q10ch42;
int maxPevap;      int maxPevap2;
/* New DALEC-PyC parameters */
int CE_factor;     int CE_factor2;      /* FRP→CE scaling (MW-1 km2)             */
int alpha_char;    int alpha_char2;     /* max charring efficiency (gC PyC/gC burned) */
int beta_stab;     int beta_stab2;      /* fraction of new PyC → refractory pool  */
int k_PyC_L;       int k_PyC_L2;        /* labile PyC turnover rate (day-1)       */
int k_PyC_R;       int k_PyC_R2;        /* refractory PyC turnover rate (day-1)   */
int i_PyC_L;       int i_PyC_L2;        /* initial labile PyC pool (gC m-2)       */
int i_PyC_R;       int i_PyC_R2;        /* initial refractory PyC pool (gC m-2)   */
int psi50_fire;    int psi50_fire2;     /* psi (MPa) at 50% CF reduction          */
int fire_lgr;      int fire_lgr2;       /* logistic rate of moisture-CF response  */
int vpd_fire_factor; int vpd_fire_factor2; /* VPD sensitivity of CF (kPa-1)      */
} DALEC_1111_PARAMETERS={
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
   100,101,102,103,104,105,106,107,108,109,
   110,111,112,113,114,115,116,117,118,119,
   120,121,122,123,124,125,
   /* New PyC pars (indices 126-145) */
   126,127,128,129,130,131,132,133,134,135,
   136,137,138,139,140,141,142,143,144,145
};


/* ======================================================================
 * FLUX STRUCT
 * Inherits all 54 DALEC_1110 fluxes (indices 0-53).
 * Adds 3 new PyC fluxes (indices 54-56).
 * nofluxes = 57*2 = 114
 * ====================================================================== */

struct DALEC_1111_FLUXES{
/* Inherited from DALEC_1110 */
int gpp;           int gpp2;
int temprate;      int temprate2;
int resp_auto;     int resp_auto2;
int fol_prod;      int fol_prod2;
int lab_prod;      int lab_prod2;
int root_prod;     int root_prod2;
int wood_prod;     int wood_prod2;
int lab_release;   int lab_release2;
int leaffall_fact; int leaffall_fact2;
int fol2lit;       int fol2lit2;
int wood2cwd;      int wood2cwd2;
int root2lit;      int root2lit2;
int resp_het_cwd;
int resp_het_lit;
int resp_het_som;
int cwd2som;
int lit2som;
int lab_release_fact;
int f_total;
int f_lab;
int f_fol;
int f_roo;
int f_woo;
int f_cwd;
int f_lit;
int f_som;
int fx_lab2lit;
int fx_fol2lit;
int fx_roo2lit;
int fx_woo2cwd;    /* fire transfer wood → CWD (non-PyC fraction)              */
int fx_cwd2som;
int fx_lit2som;
int et;
int q_paw;
int paw2puw;
int q_puw;
int q_surf;
int transp;
int evap;
int melt;
int scf;
int ae_rh_cwd;
int ae_rh_lit;
int ae_rh_som;
int an_rh_cwd;
int an_rh_lit;
int an_rh_som;
int rh_co2;
int rh_ch4;
int fV;
int fT;
int fW;
int fCH4;
int soil_moist;
/* New DALEC-PyC fluxes */
int fx_woo2PyC;    /* fire transfer wood → PyC (total PyC formation flux)      */
int f_PyC_L_dec;   /* labile PyC decomposition → CO2                           */
int f_PyC_R_dec;   /* refractory PyC decomposition → CO2                       */
} DALEC_1111_FLUXES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,48,49,
    50,51,52,53,
    54,55,56
};


/* ======================================================================
 * POOL STRUCT
 * Inherits all 10 DALEC_1110 pools (indices 0-9).
 * Adds C_PyC_L (10) and C_PyC_R (11).
 * nopools = 12*2 = 24
 * ====================================================================== */

struct DALEC_1111_POOLS{
/* Inherited from DALEC_1110 */
int C_lab;     /* Labile C                   */
int C_fol;     /* Foliar C                   */
int C_roo;     /* Root C                     */
int C_woo;     /* Wood C                     */
int C_cwd;     /* Coarse woody debris C      */
int C_lit;     /* Litter C                   */
int C_som;     /* Soil organic matter C      */
int H2O_PAW;   /* Plant available H2O        */
int H2O_PUW;   /* Plant unavailable H2O      */
int H2O_SWE;   /* Snow water equivalent      */
/* New PyC pools */
int C_PyC_L;   /* Labile pyrogenic C (O-horizon, decadal turnover)       */
int C_PyC_R;   /* Refractory pyrogenic C (mineral soil, centennial+)     */
} DALEC_1111_POOLS={
    0,1,2,3,4,5,6,7,8,9,
    10,11
};


/* ======================================================================
 * MODCONFIG: register model dimensions and observation operators
 * ====================================================================== */

int DALEC_1111_MODCONFIG(DALEC * DALECmodel){

struct DALEC_1111_PARAMETERS P=DALEC_1111_PARAMETERS;
struct DALEC_1111_FLUXES F=DALEC_1111_FLUXES;
struct DALEC_1111_POOLS S=DALEC_1111_POOLS;

DALECmodel->nopools=12*2;    /* 10 inherited + 2 PyC */
DALECmodel->nopars=73*2;     /* 63 inherited + 10 PyC */
DALECmodel->nofluxes=57*2;   /* 54 inherited + 3 PyC  */

/* ---- Observation operators ---- */
static OBSOPE OBSOPE;
INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);

OBSOPE.SUPPORT_GPP_OBS=true;
OBSOPE.SUPPORT_LAI_OBS=true;
OBSOPE.SUPPORT_ET_OBS=true;
OBSOPE.SUPPORT_NBE_OBS=true;
OBSOPE.SUPPORT_ABGB_OBS=true;
OBSOPE.SUPPORT_DOM_OBS=true;
OBSOPE.SUPPORT_EWT_OBS=true;
OBSOPE.SUPPORT_FIR_OBS=true;
OBSOPE.SUPPORT_CH4_OBS=true;
OBSOPE.SUPPORT_ROFF_OBS=true;
OBSOPE.SUPPORT_CUE_OBS=true;
OBSOPE.SUPPORT_C3frac_OBS=true;
OBSOPE.SUPPORT_iniSnow_OBS=true;
OBSOPE.SUPPORT_iniSOM_OBS=true;

/* NISAR L-band woody fire structural loss */
OBSOPE.SUPPORT_NISAR_WOOD_OBS=true;

/* GPP */
OBSOPE.GPP_flux=F.gpp;
/* LAI */
OBSOPE.LAI_foliar_pool=S.C_fol;
OBSOPE.LAI_LCMA=P.LCMA;
/* ET */
OBSOPE.ET_flux=F.et;
/* Runoff */
static int ROFF_fluxes[3];
ROFF_fluxes[0]=F.q_paw;
ROFF_fluxes[1]=F.q_puw;
ROFF_fluxes[2]=F.q_surf;
OBSOPE.ROFF_fluxes=ROFF_fluxes;
static double ROFF_flux_signs[]={1.,1.,1.};
OBSOPE.ROFF_flux_signs=ROFF_flux_signs;
OBSOPE.ROFF_n_fluxes=3;
/* NBE */
static int NBE_fluxes[4];
NBE_fluxes[0]=F.gpp;
NBE_fluxes[1]=F.resp_auto;
NBE_fluxes[2]=F.rh_co2;
NBE_fluxes[3]=F.f_total;
OBSOPE.NBE_fluxes=NBE_fluxes;
static double NBE_flux_signs[]={-1.,1.,1.,1.};
OBSOPE.NBE_flux_signs=NBE_flux_signs;
OBSOPE.NBE_n_fluxes=4;
/* ABGB */
static int ABGB_pools[4];
ABGB_pools[0]=S.C_lab;
ABGB_pools[1]=S.C_fol;
ABGB_pools[2]=S.C_roo;
ABGB_pools[3]=S.C_woo;
OBSOPE.ABGB_pools=ABGB_pools;
OBSOPE.ABGB_n_pools=4;
/* DOM */
static int DOM_pools[3];
DOM_pools[0]=S.C_cwd;
DOM_pools[1]=S.C_lit;
DOM_pools[2]=S.C_som;
OBSOPE.DOM_pools=DOM_pools;
OBSOPE.DOM_n_pools=3;
/* EWT */
static int EWT_h2o_pools[3];
EWT_h2o_pools[0]=S.H2O_PAW;
EWT_h2o_pools[1]=S.H2O_PUW;
EWT_h2o_pools[2]=S.H2O_SWE;
OBSOPE.EWT_h2o_pools=EWT_h2o_pools;
OBSOPE.EWT_n_h2o_pools=3;
/* Fire total */
OBSOPE.FIR_flux=F.f_total;
/* CUE / C3 frac / Snow / SOM */
OBSOPE.CUE_PARAM=P.f_auto;
OBSOPE.C3frac_PARAM=P.C3_frac;
OBSOPE.iniSnow_PARAM=P.i_SWE;
OBSOPE.iniSOM_PARAM=P.i_soil;

/* NISAR WOOD: sum of all woody fire structural loss fluxes
 * = direct woody combustion (f_woo)
 *   + fire transfer wood→CWD (fx_woo2cwd, non-PyC)
 *   + fire transfer wood→PyC (fx_woo2PyC)
 * Together these equal total woody pool fire loss (NISAR L-band ΔAGB).
 */
static int NISAR_WOOD_fluxes[3];
NISAR_WOOD_fluxes[0]=F.f_woo;
NISAR_WOOD_fluxes[1]=F.fx_woo2cwd;
NISAR_WOOD_fluxes[2]=F.fx_woo2PyC;
OBSOPE.NISAR_WOOD_fluxes=NISAR_WOOD_fluxes;
OBSOPE.NISAR_WOOD_n_fluxes=3;

DALECmodel->OBSOPE=OBSOPE;

return 0;}


/* ======================================================================
 * DALEC_1111: main model timestep loop
 * ====================================================================== */

int DALEC_1111(DATA DATA, double const *pars){

struct DALEC_1111_PARAMETERS P=DALEC_1111_PARAMETERS;
struct DALEC_1111_FLUXES F=DALEC_1111_FLUXES;
struct DALEC_1111_POOLS S=DALEC_1111_POOLS;

int p=0,f,m,nxp,i;
int n=0,nn=0;
double pi=3.1415927;

double deltat=DATA.ncdf_data.TIME_INDEX.values[1] - DATA.ncdf_data.TIME_INDEX.values[0];
int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;

double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;
double *LAI=DATA.M_LAI;

/* ---- Initial pool values ---- */
POOLS[S.C_lab]=pars[P.i_labile];
POOLS[S.C_fol]=pars[P.i_foliar];
POOLS[S.C_roo]=pars[P.i_root];
POOLS[S.C_woo]=pars[P.i_wood];
POOLS[S.C_cwd]=pars[P.i_cwd];
POOLS[S.C_lit]=pars[P.i_lit];
POOLS[S.C_som]=pars[P.i_soil];
POOLS[S.H2O_PAW]=pars[P.i_PAW];
POOLS[S.H2O_PUW]=pars[P.i_PUW];
POOLS[S.H2O_SWE]=pars[P.i_SWE];
POOLS[S.C_PyC_L]=pars[P.i_PyC_L];
POOLS[S.C_PyC_R]=pars[P.i_PyC_R];

/* ---- Met drivers ---- */
double *SSRD=DATA.ncdf_data.SSRD.values;
double *T2M_MIN=DATA.ncdf_data.T2M_MIN.values;
double *T2M_MAX=DATA.ncdf_data.T2M_MAX.values;
double *CO2=DATA.ncdf_data.CO2.values;
double *DOY=DATA.ncdf_data.DOY.values;
double *PREC=DATA.ncdf_data.TOTAL_PREC.values;
double *VPD=DATA.ncdf_data.VPD.values;          /* hPa = 10*kPa */
double *BURNED_AREA=DATA.ncdf_data.BURNED_AREA.values;
double *TIME_INDEX=DATA.ncdf_data.TIME_INDEX.values;
double *SNOWFALL=DATA.ncdf_data.SNOWFALL.values;

/* FRP driver (MW km-2) — may be NULL if not provided */
double *FRP_values=DATA.ncdf_data.FRP.values;

double meantemp = (DATA.ncdf_data.T2M_MAX.reference_mean + DATA.ncdf_data.T2M_MIN.reference_mean)/2;
double meanrad  = DATA.ncdf_data.SSRD.reference_mean;
double meanprec = DATA.ncdf_data.TOTAL_PREC.reference_mean;

/* ---- Methane module inputs ---- */
double PAW_fs = HYDROFUN_MOI2EWT(1,pars[P.PAW_por],pars[P.PAW_z]);
double ch4pars[8]={PAW_fs,pars[P.S_fv],pars[P.thetas_opt],pars[P.fwc],pars[P.r_ch4],pars[P.Q10ch4],pars[P.Q10rhco2],meantemp};

/* ---- Leaf/labile phenology constants ---- */
double wf=pars[P.leaf_fall]*sqrt(2)/2;
double wl=pars[P.labile_rel]*sqrt(2)/2;
double ff=(log(pars[P.t_foliar])-log(pars[P.t_foliar]-1))/2;
double fl=(log(pars[P.t_labile])-log(pars[P.t_labile]-1))/2;
double psi_porosity = -0.117/100;
double osf=offset(pars[P.t_foliar],wf);
double osl=offset(pars[P.t_labile],wl);
double sf=365.25/pi;

/* ---- Static combustion factors (fallback if FRP=0/missing) ---- */
/* cf_ligneous and cf_foliar are MCMC-learnable maximum values;
 * climate-sensitive scaling is applied inside the timestep loop.
 * cf_DOM remains static (dead pool combustion less sensitive to moisture).
 */

int nopools=((DALEC *)DATA.MODEL)->nopools;
int nofluxes=((DALEC *)DATA.MODEL)->nofluxes;

/* ======================================================================
 * TIMESTEP LOOP
 * ====================================================================== */
for (n=0; n < N_timesteps; n++){

p=nopools*n;
nxp=nopools*(n+1);
f=nofluxes*n;

/* LAI */
LAI[n]=POOLS[p+S.C_fol]/pars[P.LCMA];

double zenith_angle=DATA.ncdf_data.SZA[n];
double LAD = 0.5;
double VegK = LAD/cos(zenith_angle/180*pi);

/* Temperature scaling factor */
double g;
int Tminmin = pars[P.Tminmin] - 273.15;
int Tminmax = pars[P.Tminmax] - 273.15;
if( T2M_MIN[n] < Tminmin ){
    g=0;
} else if (T2M_MIN[n] > Tminmax){
    g=1;
} else {
    g=(T2M_MIN[n] - Tminmin)/(Tminmax - Tminmin);
}

/* H2O stress scaling */
double beta = fmin(POOLS[p+S.H2O_PAW]/pars[P.wilting],1.);
       beta = fmin(beta,g);

/* GPP, transpiration, evaporation */
double *LIU_An_et_out = LIU_An_et(SSRD[n]*1e6/(24*3600), VPD[n]/10,
    273.15+0.5*(T2M_MIN[n]+T2M_MAX[n]), pars[P.Vcmax25], CO2[n], beta, pars[P.Med_g1],
    LAI[n], pars[P.ga], VegK, pars[P.Tupp], pars[P.Tdown], 1.,
    pars[P.clumping], pars[P.leaf_refl], pars[P.maxPevap], PREC[n]);

FLUXES[f+F.gpp]   = LIU_An_et_out[0];
FLUXES[f+F.transp]= LIU_An_et_out[1];
FLUXES[f+F.evap]  = LIU_An_et_out[2];
FLUXES[f+F.et]    = FLUXES[f+F.evap]+FLUXES[f+F.transp];

/* Temprate */
FLUXES[f+F.temprate]=pow(pars[P.Q10rhco2],(0.5*(T2M_MIN[n]+T2M_MAX[n])-meantemp)/10)*((PREC[n]/meanprec-1)*pars[P.moisture]+1);

/* Carbon fluxes */
FLUXES[f+F.resp_auto]    = pars[P.f_auto]*FLUXES[f+F.gpp];
FLUXES[f+F.fol_prod]     = (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto])*pars[P.f_foliar];
FLUXES[f+F.lab_prod]     = (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.fol_prod])*pars[P.f_lab];
FLUXES[f+F.root_prod]    = (FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.fol_prod]-FLUXES[f+F.lab_prod])*pars[P.f_root];
FLUXES[f+F.wood_prod]    = FLUXES[f+F.gpp]-FLUXES[f+F.resp_auto]-FLUXES[f+F.fol_prod]-FLUXES[f+F.root_prod]-FLUXES[f+F.lab_prod];

FLUXES[f+F.leaffall_fact]   = (2/sqrt(pi))*(ff/wf)*exp(-pow(sin((TIME_INDEX[n]-pars[P.Fday]+osf)/sf)*sf/wf,2));
FLUXES[f+F.lab_release_fact]= (2/sqrt(pi))*(fl/wl)*exp(-pow(sin((TIME_INDEX[n]-pars[P.Bday]+osl)/sf)*sf/wl,2));
FLUXES[f+F.lab_release]     = POOLS[p+S.C_lab]*(1-pow(1-FLUXES[f+F.lab_release_fact],deltat))/deltat;
FLUXES[f+F.fol2lit]         = POOLS[p+S.C_fol]*(1-pow(1-FLUXES[f+F.leaffall_fact],deltat))/deltat;
FLUXES[f+F.wood2cwd]        = POOLS[p+S.C_woo]*(1-pow(1-pars[P.t_wood],deltat))/deltat;
FLUXES[f+F.root2lit]        = POOLS[p+S.C_roo]*(1-pow(1-pars[P.t_root],deltat))/deltat;
FLUXES[f+F.resp_het_cwd]    = POOLS[p+S.C_cwd]*(1-pow(1-FLUXES[f+F.temprate]*pars[P.t_cwd],deltat))/deltat;
FLUXES[f+F.resp_het_lit]    = POOLS[p+S.C_lit]*(1-pow(1-FLUXES[f+F.temprate]*pars[P.t_lit],deltat))/deltat;
FLUXES[f+F.resp_het_som]    = POOLS[p+S.C_som]*(1-pow(1-FLUXES[f+F.temprate]*pars[P.t_soil],deltat))/deltat;

/* JCR methane module */
double *jcr_o = JCR(ch4pars,T2M_MIN[n],T2M_MAX[n],POOLS[S.H2O_PAW]);
FLUXES[f+F.ae_rh_cwd]= POOLS[p+S.C_cwd]*(1-pow(1-jcr_o[3]*jcr_o[1]*jcr_o[2]*pars[P.t_cwd],deltat))/deltat;
FLUXES[f+F.an_rh_lit] = POOLS[p+S.C_lit]*(1-pow(1-pars[P.fwc]*jcr_o[1]*(1-jcr_o[2])*pars[P.t_lit],deltat))/deltat;
FLUXES[f+F.ae_rh_som] = POOLS[p+S.C_som]*(1-pow(1-jcr_o[3]*jcr_o[1]*jcr_o[2]*pars[P.t_soil],deltat))/deltat;
FLUXES[f+F.ae_rh_cwd] = POOLS[p+S.C_cwd]*(1-pow(1-jcr_o[3]*jcr_o[1]*jcr_o[2]*pars[P.t_cwd],deltat))/deltat;
FLUXES[f+F.an_rh_lit] = POOLS[p+S.C_lit]*(1-pow(1-pars[P.fwc]*jcr_o[1]*(1-jcr_o[2])*pars[P.t_lit],deltat))/deltat;
FLUXES[f+F.an_rh_som] = POOLS[p+S.C_som]*(1-pow(1-pars[P.fwc]*jcr_o[1]*(1-jcr_o[2])*pars[P.t_soil],deltat))/deltat;
FLUXES[f+F.rh_co2]    = (FLUXES[f+F.ae_rh_lit]+FLUXES[f+F.ae_rh_cwd]+FLUXES[f+F.ae_rh_som])+(FLUXES[f+F.an_rh_lit]+FLUXES[f+F.an_rh_cwd]+FLUXES[f+F.an_rh_som])*(1-jcr_o[4]);
FLUXES[f+F.rh_ch4]    = (FLUXES[f+F.an_rh_lit]+FLUXES[f+F.an_rh_cwd]+FLUXES[f+F.an_rh_som])*jcr_o[4];
FLUXES[f+F.fV]        = jcr_o[2];
FLUXES[f+F.fT]        = jcr_o[1];
FLUXES[f+F.fW]        = jcr_o[3];
FLUXES[f+F.fCH4]      = jcr_o[4];
FLUXES[f+F.soil_moist]= jcr_o[0];

FLUXES[f+F.cwd2som] = POOLS[p+S.C_cwd]*(1-pow(1-pars[P.tr_cwd2som]*FLUXES[f+F.temprate],deltat))/deltat;
FLUXES[f+F.lit2som] = POOLS[p+S.C_lit]*(1-pow(1-pars[P.tr_lit2soil]*FLUXES[f+F.temprate],deltat))/deltat;

/* ---- Pool updates (no fire yet) ---- */
POOLS[nxp+S.C_lab] = POOLS[p+S.C_lab] + (FLUXES[f+F.lab_prod]-FLUXES[f+F.lab_release])*deltat;
POOLS[nxp+S.C_fol] = POOLS[p+S.C_fol] + (FLUXES[f+F.fol_prod]-FLUXES[f+F.fol2lit]+FLUXES[f+F.lab_release])*deltat;
POOLS[nxp+S.C_roo] = POOLS[p+S.C_roo] + (FLUXES[f+F.root_prod]-FLUXES[f+F.root2lit])*deltat;
POOLS[nxp+S.C_woo] = POOLS[p+S.C_woo] + (FLUXES[f+F.wood_prod]-FLUXES[f+F.wood2cwd])*deltat;
POOLS[nxp+S.C_cwd] = POOLS[p+S.C_cwd] + (FLUXES[f+F.wood2cwd]-FLUXES[f+F.ae_rh_cwd]-FLUXES[f+F.an_rh_cwd]-FLUXES[f+F.cwd2som])*deltat;
POOLS[nxp+S.C_lit] = POOLS[p+S.C_lit] + (FLUXES[f+F.fol2lit]+FLUXES[f+F.root2lit]-FLUXES[f+F.ae_rh_lit]-FLUXES[f+F.an_rh_lit]-FLUXES[f+F.lit2som])*deltat;
POOLS[nxp+S.C_som] = POOLS[p+S.C_som] + (FLUXES[f+F.lit2som]-FLUXES[f+F.ae_rh_som]-FLUXES[f+F.an_rh_som]+FLUXES[f+F.cwd2som])*deltat;

/* ---- Hydrology ---- */
POOLS[nxp+S.H2O_SWE]=POOLS[p+S.H2O_SWE]+SNOWFALL[n]*deltat;
FLUXES[f+F.melt]=fmin(fmax(((T2M_MIN[n]+T2M_MAX[n])/2-(pars[P.min_melt]-273.15))*pars[P.melt_slope],0),1)*POOLS[nxp+S.H2O_SWE]/deltat;
POOLS[nxp+S.H2O_SWE]=POOLS[nxp+S.H2O_SWE]-FLUXES[f+F.melt]*deltat;
FLUXES[f+F.scf]=POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]);

double infil = pars[P.max_infil]*(1-exp(-(PREC[n]-SNOWFALL[n]+FLUXES[f+F.melt])/pars[P.max_infil]));
FLUXES[f+F.q_surf]=(PREC[n]-SNOWFALL[n]+FLUXES[f+F.melt])-infil;

POOLS[nxp+S.H2O_PAW] = POOLS[p+S.H2O_PAW] + deltat*infil;
POOLS[nxp+S.H2O_PUW] = POOLS[p+S.H2O_PUW];

double sm_PAW = HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PAW],pars[P.PAW_por],pars[P.PAW_z]);
double sm_PUW = HYDROFUN_EWT2MOI(POOLS[nxp+S.H2O_PUW],pars[P.PUW_por],pars[P.PUW_z]);

double drain_PAW = DRAINAGE(sm_PAW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);
double drain_PUW = DRAINAGE(sm_PUW,pars[P.Q_excess],-pars[P.field_cap],psi_porosity,pars[P.retention]);

FLUXES[f+F.q_paw] = HYDROFUN_MOI2EWT(drain_PAW,pars[P.PAW_por],pars[P.PAW_z])/deltat;
FLUXES[f+F.q_puw] = HYDROFUN_MOI2EWT(drain_PUW,pars[P.PUW_por],pars[P.PUW_z])/deltat;

sm_PAW -= drain_PAW;
sm_PUW -= drain_PUW;

double k_PAW = HYDROFUN_MOI2CON(sm_PAW,pars[P.hydr_cond],pars[P.retention]);
double k_PUW = HYDROFUN_MOI2CON(sm_PUW,pars[P.hydr_cond],pars[P.retention]);
double psi_PAW = HYDROFUN_MOI2PSI(sm_PAW,psi_porosity,pars[P.retention]);
double psi_PUW = HYDROFUN_MOI2PSI(sm_PUW,psi_porosity,pars[P.retention]);

double xfer = 1000*sqrt(k_PAW*k_PUW)*(1000*(psi_PAW-psi_PUW)/(9.8*0.5*(pars[P.PAW_z]+pars[P.PUW_z]))+1);
FLUXES[f+F.paw2puw] = xfer*1000*3600*24;

POOLS[nxp+S.H2O_PAW] += (-FLUXES[f+F.paw2puw]-FLUXES[f+F.q_paw]-FLUXES[f+F.et])*deltat;
POOLS[nxp+S.H2O_PUW] += (FLUXES[f+F.paw2puw]-FLUXES[f+F.q_puw])*deltat;


/* ================================================================
 * FIRE BLOCK — DALEC-PyC extensions
 * ================================================================
 *
 * 1. Climate-sensitive combustion factors
 *    CF_lig_t  = cf_ligneous * f(psi_PAW) * g(VPD)
 *    CF_fol_t  = cf_foliar   * f(psi_PAW) * g(VPD)
 *    where:
 *      f(psi) = logistic: drier soil → higher CF
 *      g(VPD) = 1 - exp(-vpd_fire_factor * VPD_kPa): higher VPD → higher CF
 *
 * 2. FRP-driven combustion efficiency (CE) → PyC yield
 *    fre_index = 1 - exp(-CE_factor * FRP_val)   [0=smoldering, 1=flaming]
 *    alpha_eff = alpha_char * (1 - fre_index)     [flaming → low charring]
 *
 * 3. Wood fire partitioning:
 *    fx_woo2PyC = wood_burned * (1-resilience) * alpha_eff   [to PyC]
 *    fx_woo2cwd = wood_burned * (1-resilience) * (1-alpha_eff) [to CWD]
 *
 * 4. PyC pool dynamics:
 *    C_PyC_L += (fx_woo2PyC*(1-beta_stab) - f_PyC_L_dec)*deltat
 *    C_PyC_R += (fx_woo2PyC*beta_stab     - f_PyC_R_dec)*deltat
 * ================================================================ */

/* VPD in drivers is in hPa; convert to kPa (*0.1) */
double VPD_kPa = VPD[n] * 0.1;

/* psi_PAW is negative (MPa); D_PSI is positive dryness index */
double D_PSI = -psi_PAW;

/* Logistic soil-moisture modulation of combustion factors (0-1) */
double f_psi = 1.0/(1.0+exp(-pars[P.fire_lgr]*(D_PSI-pars[P.psi50_fire])));

/* VPD modulation (0-1, saturating) */
double g_vpd = 1.0-exp(-pars[P.vpd_fire_factor]*VPD_kPa);

/* Climate-sensitive combustion factors */
double CF_lig_t = pars[P.cf_ligneous] * f_psi * g_vpd;
double CF_fol_t = pars[P.cf_foliar]   * f_psi * g_vpd;
/* Clamp to [0, 1] */
CF_lig_t = fmin(fmax(CF_lig_t, 0.0), 1.0);
CF_fol_t = fmin(fmax(CF_fol_t, 0.0), 1.0);

/* Combustion factor array — indexed by pool */
double CF[12];
CF[S.C_lab]   = CF_lig_t;
CF[S.C_fol]   = CF_fol_t;
CF[S.C_roo]   = 0.5*CF_lig_t + 0.5*CF_fol_t;  /* roots intermediate */
CF[S.C_woo]   = CF_lig_t;
CF[S.C_cwd]   = CF_lig_t;
CF[S.C_lit]   = 0.5*CF_fol_t + 0.5*CF_lig_t;
CF[S.C_som]   = pars[P.cf_DOM];                 /* DOM: static prior  */
CF[S.H2O_PAW] = 0.0;
CF[S.H2O_PUW] = 0.0;
CF[S.H2O_SWE] = 0.0;
CF[S.C_PyC_L] = 0.0;   /* PyC pools assumed fire-resistant                  */
CF[S.C_PyC_R] = 0.0;

/* FRP-driven PyC yield
 * fre_index: 0 = purely smoldering (high PyC yield)
 *            1 = purely flaming (low PyC yield, high CE)
 * alpha_eff = alpha_char * (1 - fre_index) per Jones et al. (2019) & Wooster et al. (2005)
 */
double FRP_val = (FRP_values != NULL) ? FRP_values[n] : 0.0;
if (FRP_val < 0.0){ FRP_val = 0.0; }
double fre_index = 1.0 - exp(-pars[P.CE_factor] * FRP_val);
double alpha_eff = pars[P.alpha_char] * (1.0 - fre_index);

/* ---- Fire combustion fluxes ---- */
FLUXES[f+F.f_lab] = POOLS[nxp+S.C_lab]*BURNED_AREA[n]*CF[S.C_lab]/deltat;
FLUXES[f+F.f_fol] = POOLS[nxp+S.C_fol]*BURNED_AREA[n]*CF[S.C_fol]/deltat;
FLUXES[f+F.f_roo] = POOLS[nxp+S.C_roo]*BURNED_AREA[n]*CF[S.C_roo]/deltat;
FLUXES[f+F.f_woo] = POOLS[nxp+S.C_woo]*BURNED_AREA[n]*CF[S.C_woo]/deltat;
FLUXES[f+F.f_cwd] = POOLS[nxp+S.C_cwd]*BURNED_AREA[n]*CF[S.C_cwd]/deltat;
FLUXES[f+F.f_lit] = POOLS[nxp+S.C_lit]*BURNED_AREA[n]*CF[S.C_lit]/deltat;
FLUXES[f+F.f_som] = POOLS[nxp+S.C_som]*BURNED_AREA[n]*CF[S.C_som]/deltat;

/* ---- Fire transfer fluxes (non-combusted structural loss) ---- */
FLUXES[f+F.fx_lab2lit] = POOLS[nxp+S.C_lab]*BURNED_AREA[n]*(1-CF[S.C_lab])*(1-pars[P.resilience])/deltat;
FLUXES[f+F.fx_fol2lit] = POOLS[nxp+S.C_fol]*BURNED_AREA[n]*(1-CF[S.C_fol])*(1-pars[P.resilience])/deltat;
FLUXES[f+F.fx_roo2lit] = POOLS[nxp+S.C_roo]*BURNED_AREA[n]*(1-CF[S.C_roo])*(1-pars[P.resilience])/deltat;
FLUXES[f+F.fx_cwd2som] = POOLS[nxp+S.C_cwd]*BURNED_AREA[n]*(1-CF[S.C_cwd])*(1-pars[P.resilience])/deltat;
FLUXES[f+F.fx_lit2som] = POOLS[nxp+S.C_lit]*BURNED_AREA[n]*(1-CF[S.C_lit])*(1-pars[P.resilience])/deltat;

/* Wood fire structural loss split into PyC formation vs CWD transfer
 * Total non-combusted woody residual = C_woo * BA * (1-CF_lig) * (1-resilience)
 * alpha_eff fraction → PyC; remainder → CWD (charcoal pieces that decompose)
 */
double woody_nc_flux = POOLS[nxp+S.C_woo]*BURNED_AREA[n]*(1-CF[S.C_woo])*(1-pars[P.resilience])/deltat;
FLUXES[f+F.fx_woo2PyC] = woody_nc_flux * alpha_eff;
FLUXES[f+F.fx_woo2cwd] = woody_nc_flux * (1.0 - alpha_eff);

/* ---- PyC decomposition fluxes ---- */
FLUXES[f+F.f_PyC_L_dec] = POOLS[nxp+S.C_PyC_L]*(1-pow(1-pars[P.k_PyC_L],deltat))/deltat;
FLUXES[f+F.f_PyC_R_dec] = POOLS[nxp+S.C_PyC_R]*(1-pow(1-pars[P.k_PyC_R],deltat))/deltat;

/* ---- Apply fire pool updates ---- */
/* Live C pools */
POOLS[nxp+S.C_lab] = POOLS[nxp+S.C_lab]-(FLUXES[f+F.f_lab]+FLUXES[f+F.fx_lab2lit])*deltat;
POOLS[nxp+S.C_fol] = POOLS[nxp+S.C_fol]-(FLUXES[f+F.f_fol]+FLUXES[f+F.fx_fol2lit])*deltat;
POOLS[nxp+S.C_roo] = POOLS[nxp+S.C_roo]-(FLUXES[f+F.f_roo]+FLUXES[f+F.fx_roo2lit])*deltat;
POOLS[nxp+S.C_woo] = POOLS[nxp+S.C_woo]-(FLUXES[f+F.f_woo]+woody_nc_flux)*deltat;

/* Dead C pools */
POOLS[nxp+S.C_cwd] = POOLS[nxp+S.C_cwd]+(FLUXES[f+F.fx_woo2cwd]-FLUXES[f+F.f_cwd]-FLUXES[f+F.fx_cwd2som])*deltat;
POOLS[nxp+S.C_lit] = POOLS[nxp+S.C_lit]+(FLUXES[f+F.fx_lab2lit]+FLUXES[f+F.fx_fol2lit]+FLUXES[f+F.fx_roo2lit]-FLUXES[f+F.f_lit]-FLUXES[f+F.fx_lit2som])*deltat;
POOLS[nxp+S.C_som] = POOLS[nxp+S.C_som]+(FLUXES[f+F.fx_cwd2som]+FLUXES[f+F.fx_lit2som]-FLUXES[f+F.f_som])*deltat;

/* PyC pools */
double f_woo2PyC_L = FLUXES[f+F.fx_woo2PyC] * (1.0 - pars[P.beta_stab]);  /* labile fraction  */
double f_woo2PyC_R = FLUXES[f+F.fx_woo2PyC] * pars[P.beta_stab];          /* refractory fraction */

POOLS[nxp+S.C_PyC_L] = POOLS[nxp+S.C_PyC_L] + (f_woo2PyC_L - FLUXES[f+F.f_PyC_L_dec])*deltat;
POOLS[nxp+S.C_PyC_R] = POOLS[nxp+S.C_PyC_R] + (f_woo2PyC_R - FLUXES[f+F.f_PyC_R_dec])*deltat;

/* Ensure PyC pools remain non-negative (numerical safeguard) */
if (POOLS[nxp+S.C_PyC_L] < 0.0){ POOLS[nxp+S.C_PyC_L] = 0.0; }
if (POOLS[nxp+S.C_PyC_R] < 0.0){ POOLS[nxp+S.C_PyC_R] = 0.0; }

/* Total fire C flux (for NBE and FIR obs operators) */
FLUXES[f+F.f_total] = FLUXES[f+F.f_lab]+FLUXES[f+F.f_fol]+FLUXES[f+F.f_roo]+
                       FLUXES[f+F.f_woo]+FLUXES[f+F.f_cwd]+FLUXES[f+F.f_lit]+FLUXES[f+F.f_som];

} /* end timestep loop */

return 0;
}
