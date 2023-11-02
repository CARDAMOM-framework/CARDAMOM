#pragma once
/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/


/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/

struct DALEC_1130_PARAMETERS{
/*DALEC PARAMETERS*/
int tr_lit2som;
int tr_cwd2som;
int rauto_mr_r;
int rauto_mr_w;
int rauto_mr_q10;
int rauto_mrd_q10;
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
int i_LY1_SM;
int i_LY2_SM;
int cf_foliar;
int cf_ligneous;
int cf_DOM;
int resilience;
int hydr_cond;
int max_infil;
int i_LY3_SM;
int LY1_por;
int LY2_por;
int LY3_por;
int field_cap;
int LY1_z;//LY1 depth
int LY2_z;//LY2 depth
int LY3_z;//LY3 depth
int LY1_vhc;//LY1 volumetric heat capacity
int LY2_vhc;//LY2 volumetric heat capacity
int LY3_vhc;//LY3 volumetric heat capacity
int Q_excess;
int Med_g1;
int Vcmax25;
int Tminmin;
int Tminmax;
int ga;
int Tupp;
int Tdown;
int clumping;
int leaf_refl_par;
int leaf_refl_nir;
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
int i_LY1_E;
int i_LY2_E;
int i_LY3_E;
int psi_50;
int beta_lgr;
int thermal_cond;
int thermal_cond_surf;
int q10canopy;
int canopyRdsf;
int sublimation_rate;
int root_frac;
int beta_lgrHMF;
int psi_50HMF; 
int t_lab;


int s_ar;
int s_wood;
int s_root;
int e_ar;
int e_wood;
int e_root;
int W_o;
int k;
} DALEC_1130_PARAMETERS={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,48,49,
    50,51,52,53,54,55,56,57,58,59,
    60,61,62,63,64,65,66,67,68,69,
    70,71,72,73,74,75,76,77,78,79,
    80,81,82,83,84,85,

    86,87,88,89,90,91,92,93
};

struct DALEC_1130_FLUXES{
/*DALEC FLUXES*/
int gpp;   /*GPP*/
int gppnet;   /*GPP*/
int resp_auto;   /*Autotrophic respiration*/
int lab_prod;   /*Labile production*/
int foliar_prod;   /*Labile release*/
int root_prod;   /*Root production*/
int wood_prod;   /*Wood production*/
int ph_fol2lit;  /*Foliar phenological senescence*/
int fol2lit;   /*Foliar decomposition*/
int woo2cwd;   /*Wood decomposition*/
int roo2lit;   /*Root decomposition*/
int lab2lit;   /*Labile C decomposition*/
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
int q_ly1;   /*LY1 runoff*/
int q_ly2;   /*LY2 runoff*/
int ly1xly2;   /*LY1->LY2 transfer*/
int ly2xly3;   /*LY2->LY3 transfer*/
int q_ly3;   /*LY3 runoff*/
int q_surf;   /*Surface runoff*/
int infil;   /*INFILTRATION*/
int infil_e;   /*INFILTRATION IE: temp = weight average of snow melt (0C) + liquid precip (@air temp)*/
int q_ly1_e;   /*Q LY1 IE: temp = LY1 temp*/
int q_ly2_e;   /*Q LY2 IE: temp = LY2 temp*/
int q_ly3_e;   /*Q LY3 IE: temp = LY3 temp*/
int ly1xly2_e;   /*LY1->NDZ transfer IE: temp of donor*/
int ly2xly3_e;   /*LY2->LY3 transfer IE: temp of donor*/
int evap_e; /* See Retano's calculation*/
int transp1_e; /* See Retano's calculation*/
int transp2_e; /* See Retano's calculation*/
int transp1;   /*Transpiration*/
int transp2;   /*Transpiration*/
int evap;   /*Evaporation*/
int snowfall;   /*Snowfall to SWE*/
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
int SWin;
int SWout;
int LWin;
int LWout;
int ly1xly2_th_e;
int ly2xly3_th_e;
int resp_auto_maint_dark;
int sublimation;
int geological;
int ets;  /* Total land-atmosphere water flux (evaporation + transpiration + sublimation) */
int beta_factor; 
int soil_beta_factor;
int hydraulic_mortality_factor; 
int nonleaf_mortality_factor;
int leaf_mortality_factor;
int dist_lab;
int dist_fol;
int dist_roo;
int dist_woo;
int Rd;
} DALEC_1130_FLUXES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,48,49,
    50,51,52,53,54,55,56,57,58,59,
    60,61,62,63,64,65,66,67,68,69,
    70,71,72,73,74,75,76,77,78,79,
    80,81,82,83,84,85,86,87,88,89,
    90,91,92, 93
};



/*Prognostic states and Diagnostic states (dependent on other states)*/


struct DALEC_1130_POOLS{
/*DALEC POOLS*/
int C_lab; /*Labile C*/
int C_fol; /*Foliar C*/
int C_roo; /*Root C*/
int C_woo; /*Wood C*/
int C_cwd; /*Coarse woody debris C*/
int C_lit; /*Litter C*/
int C_som; /*Soil C*/
int H2O_LY1; /*Layer 1 H2O*/
int H2O_LY2; /*Layer 2 H2O*/
int H2O_LY3; /*Layer 3 H2O*/
int H2O_SWE; /*Snow water equivalent*/
int E_LY1; /*LY1 thermal energy state*/
int E_LY2; /*LY2 thermal energy state*/
int E_LY3; /*LY3 thermal energy state*/
int D_LAI;//leaf area index
int D_SCF;//snow-covered fraction
int D_TEMP_LY1;//LY1 temp in K
int D_TEMP_LY2;//LY2 temp in K
int D_TEMP_LY3;//LY3 temp in K
int D_LF_LY1;//LY1 liquid h2o frac
int D_LF_LY2;//LY2 liquid h2o frac
int D_LF_LY3;//LY3 liquid h2o frac
int D_SM_LY1;//LY1 soil moisture
int D_SM_LY2;//LY2 soil moisture
int D_SM_LY3;//LY3 soil moisture
int D_PSI_LY1;//LY1 soil moisture
int D_PSI_LY2;//LY2 soil moisture
int D_PSI_LY3;//LY3 soil moisture
int M_LAI_MAX;//KNORR LAI module max LAI memory
int M_LAI_TEMP;//KNORR LAI module temp memory
} DALEC_1130_POOLS={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29
};

struct DALEC_1130_EDCs{
int vcmax_lcma;
int litcwdtor;
int cwdsomtor;
int mr_rates;
int rootwoodtor;
int fol2lig_cf;
int relativepsi50;
int state_ranges;
int state_trajectories;
int nsc_ratio;
int cfcr_ratio;
int fffr_ratio;
int mean_ly1_temp;
int mean_ly2_temp;
int mean_ly3_temp;

int EDC_wetalloc;
int EDC_dryalloc;
} DALEC_1130_EDCs={
    0, 1, 2, 3, 4, 5, 6, 
    7, 8, 9,10,11,12,13,
    14,

    15,16
};



