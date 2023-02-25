
/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/

struct DALEC_1080_PARAMETERS{
/*DALEC PARAMETERS*/
int tr_lit2soil_1;
int tr_lit2soil_2;
int f_auto_1;
int f_auto_2;
int f_foliar_1;
int f_foliar_2;
int f_root_1;
int f_root_2;
int t_foliar_1;
int t_foliar_2;
int t_wood_1;
int t_wood_2;
int t_root_1;
int t_root_2;
int t_lit_1;
int t_lit_2;
int t_soil_1;
int t_soil_2;
int temp_factor_1;
int temp_factor_2;
int canopy_eff_1;
int canopy_eff_2;
int Bday_1;
int Bday_2;
int f_lab_1;
int f_lab_2;
int labile_rel_1;
int labile_rel_2;
int Fday_1;
int Fday_2;
int leaf_fall_1;
int leaf_fall_2;
int LCMA_1;
int LCMA_2;
int i_labile_1;
int i_labile_2;
int i_foliar_1;
int i_foliar_2;
int i_root_1;
int i_root_2;
int i_wood_1;
int i_wood_2;
int i_lit_1;
int i_lit_2;
int i_soil_1;
int i_soil_2;
int uWUE_1;
int uWUE_2;
int PAW_Qmax_1;
int PAW_Qmax_2;
int wilting_1;
int wilting_2;
int i_PAW_1;
int i_PAW_2;
int cf_foliar_1;
int cf_foliar_2;
int cf_ligneous_1;
int cf_ligneous_2;
int cf_DOM_1;
int cf_DOM_2;
int resilience_1;
int resilience_2;
int t_labile_1;
int t_labile_2;
int moisture_1;
int moisture_2;
int h2o_xfer_1;
int h2o_xfer_2;
int PUW_Qmax_1;
int PUW_Qmax_2;
int i_PUW_1;
int i_PUW_2;
int boese_r_1;
int boese_r_2;
} DALEC_1080_PARAMETERS={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,48,49,
    50,51,52,53,54,55,56,57,58,59,
    60,61,62,63,64,65,66,67,68,69,
    70,71,72,73
};//37*2 = 74

struct DALEC_1080_FLUXES{
/*DALEC FLUXES*/
int gpp_1;   /*GPP*/
int gpp_2;   /*GPP*/
int temprate_1;   /*Temprate*/
int temprate_2;   /*Temprate*/
int resp_auto_1;   /*Autotrophic respiration*/
int resp_auto_2;   /*Autotrophic respiration*/
int fol_prod_1;   /*Foliar production*/
int fol_prod_2;   /*Foliar production*/
int lab_prod_1;   /*Labile production*/
int lab_prod_2;   /*Labile production*/
int root_prod_1;   /*Root production*/
int root_prod_2;   /*Root production*/
int wood_prod_1;   /*Wood production*/
int wood_prod_2;   /*Wood production*/
int lab_release_1;   /*Labile release*/
int lab_release_2;   /*Labile release*/
int leaffall_fact_1;   /*Leaffall factor*/
int leaffall_fact_2;   /*Leaffall factor*/
int fol2lit_1;   /*Foliar decomposition*/
int fol2lit_2;   /*Foliar decomposition*/
int wood2lit_1;   /*Wood description*/
int wood2lit_2;   /*Wood description*/
int root2lit_1;   /*Root decomposition*/
int root2lit_2;   /*Root decomposition*/
int resp_het_lit_1;   /*Litter heterotrophic respiration*/
int resp_het_lit_2;   /*Litter heterotrophic respiration*/
int resp_het_som_1;   /*Soil heterotrophic respiration*/
int resp_het_som_2;   /*Soil heterotrophic respiration*/
int lit2som_1;   /*Litter decomposition*/
int lit2som_2;   /*Litter decomposition*/
int lab_release_fact_1;   /*Labile release factor*/
int lab_release_fact_2;   /*Labile release factor*/
int f_total_1;   /*Flux description*/
int f_total_2;   /*Flux description*/
int f_lab_1;   /*Labile fire loss*/
int f_lab_2;   /*Labile fire loss*/
int f_fol_1;   /*Foliar fire loss*/
int f_fol_2;   /*Foliar fire loss*/
int f_roo_1;   /*Wood fire loss*/
int f_roo_2;   /*Wood fire loss*/
int f_woo_1;   /*Root fire loss*/
int f_woo_2;   /*Root fire loss*/
int f_lit_1;   /*Litter fire loss*/
int f_lit_2;   /*Litter fire loss*/
int f_som_1;   /*Soil fire loss*/
int f_som_2;   /*Soil fire loss*/
int fx_lab2lit_1;   /*Fire transfer labile to litter*/
int fx_lab2lit_2;   /*Fire transfer labile to litter*/
int fx_fol2lit_1;   /*Fire transfer foliar to litter*/
int fx_fol2lit_2;   /*Fire transfer foliar to litter*/
int fx_roo2lit_1;   /*Fire transfer root to litter*/
int fx_roo2lit_2;   /*Fire transfer root to litter*/
int fx_woo2som_1;   /*Fire transfer wood to soil*/
int fx_woo2som_2;   /*Fire transfer wood to soil*/
int fx_lit2som_1;   /*Fire transfer litter to soil*/
int fx_lit2som_2;   /*Fire transfer litter to soil*/
int et_1;   /*Evapotranspiration*/
int et_2;   /*Evapotranspiration*/
int q_paw_1;   /*Plant-available water drainage*/
int q_paw_2;   /*Plant-available water drainage*/
int paw2puw_1;   /*Plant-available water to plant-unavailable water transfer*/
int paw2puw_2;   /*Plant-available water to plant-unavailable water transfer*/
int q_puw_1;   /*Plant-unavailable water runoff*/
int q_puw_2;   /*Plant-unavailable water runoff*/
int gpp;   /*GPP*/
int et;   /*ET*/
int f_total;//FTotal
int nbe;
} DALEC_1080_FLUXES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,48,49,
    50,51,52,53,54,55,56,57,58,59,
    60,61,62,63,
     //aggregates
     64,65,66,67};

struct DALEC_1080_POOLS{
/*DALEC POOLS*/
int C_lab_1; /*Labile C*/
int C_lab_2; /*Labile C*/
int C_fol_1; /*Foliar C*/
int C_fol_2; /*Foliar C*/
int C_roo_1; /*Root C*/
int C_roo_2; /*Root C*/
int C_woo_1; /*Wood C*/
int C_woo_2; /*Wood C*/
int C_lit_1; /*Litter C*/
int C_lit_2; /*Litter C*/
int C_som_1; /*Soil C*/
int C_som_2; /*Soil C*/
int H2O_PAW_1; /*Plant available H2O*/
int H2O_PAW_2; /*Plant available H2O*/
int H2O_PUW_1; /*Plant unavailable H2O*/
int H2O_PUW_2; /*Plant unavailable H2O*/
int C_lab; /*Labile C*/
int C_fol; /*Foliar C*/
int C_roo; /*Root C*/
int C_woo; /*Wood C*/
int C_lit; /*Litter C*/
int C_som; /*Soil C*/
int H2O_PAW; /*Plant available H2O*/
int H2O_PUW; /*Plant unavailable H2O*/
int LAI; /*LAI*/
} DALEC_1080_POOLS={0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,
//aggregates
16,17,18,19,20,21,22,23,
        //ancillary
        24};
