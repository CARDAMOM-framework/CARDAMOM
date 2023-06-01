
struct DALEC_1046_PARAMETERS{
/*DALEC PARAMETERS*/
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
int i_som;
int uWUE;
int PAW_Qmax;
int wilting;
int i_PAW;
int cf_foliar;
int cf_ligneous;
int cf_DOM;
int resilience;
int t_labile;
int moisture;
int h2o_xfer;
int PUW_Qmax;
int i_PUW;
int boese_r;
} DALEC_1046_PARAMETERS={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36
};

struct DALEC_1046_FLUXES{
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
int fol2lit;   /*Foliar decomposition*/
int woo2som;   /*Wood description*/
int roo2lit;   /*Root decomposition*/
int resp_het_lit;   /*Litter heterotrophic respiration*/
int resp_het_som;   /*Soil heterotrophic respiration*/
int lit2som;   /*Litter decomposition*/
int lab_release_fact;   /*Labile release factor*/
int f_total;   /*Flux description*/
int f_lab;   /*Labile fire loss*/
int f_fol;   /*Foliar fire loss*/
int f_roo;   /*Wood fire loss*/
int f_woo;   /*Root fire loss*/
int f_lit;   /*Litter fire loss*/
int f_som;   /*Soil fire loss*/

int d_total;   /*Disturbance flux total*/
int d_lab;   /*Labile disturbance loss*/
int d_fol;   /*Foliar disturbance loss*/
int d_roo;   /*Wood disturbance loss*/
int d_woo;   /*Root disturbance loss*/
int d_lit;   /*Litter disturbance loss*/
int d_som;   /*Soil disturbance loss*/

int fx_lab2lit;   /*Fire transfer labile to litter*/
int fx_fol2lit;   /*Fire transfer foliar to litter*/
int fx_roo2lit;   /*Fire transfer root to litter*/
int fx_woo2som;   /*Fire transfer wood to soil*/
int fx_lit2som;   /*Fire transfer litter to soil*/
int a_lab;   /*Labile fire loss*/
int a_fol;   /*Foliar fire loss*/
int a_roo;   /*Wood fire loss*/
int a_woo;   /*Root fire loss*/
int ax_lab2lit;   /*Fire transfer labile to litter*/
int ax_fol2lit;   /*Fire transfer foliar to litter*/
int ax_roo2lit;   /*Fire transfer root to litter*/
int ax_woo2som;   /*Fire transfer wood to soil*/
int et;   /*Evapotranspiration*/
int q_paw;   /*Plant-available water drainage*/
int paw2puw;   /*Plant-available water to plant-unavailable water transfer*/
int q_puw;   /*Plant-unavailable water runoff*/
} DALEC_1046_FLUXES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39
};

struct DALEC_1046_POOLS{
/*DALEC POOLS*/
int C_lab; /*Labile C*/
int C_fol; /*Foliar C*/
int C_roo; /*Root C*/
int C_woo; /*Wood C*/
int C_lit; /*Litter C*/
int C_som; /*Soil C*/
int H2O_PAW; /*Plant available H2O*/
int H2O_PUW; /*Plant unavailable H2O*/
int D_LAI;//leaf area index
} DALEC_1046_POOLS={0,1,2,3,4,5,6,7,8};
