#pragma once

#include "DALEC_1100_INDICES.c"
//#include "CARDAMOM_DATA_STRUCTURE.c"
void POPULATE_INFO_STRUCTS(DALEC * DALECmodel){

    struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
    struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
    struct DALEC_1100_POOLS S=DALEC_1100_POOLS;
    struct DALEC_1100_EDCs E=DALEC_1100_EDCs;


    //Populate the arrays with valid memory blocks, so we are writing to something real!
    //Warning: this part of the code is likely not going to need to be modified, skip ahead to "METADATA DEFINITIONS"
    DALECmodel->FLUX_META.NAME= calloc(DALECmodel->nofluxes, sizeof(char *));
    DALECmodel->FLUX_META.ABBREVIATION= calloc(DALECmodel->nofluxes, sizeof(char *));
    DALECmodel->FLUX_META.UNITS= calloc(DALECmodel->nofluxes, sizeof(char *));
    DALECmodel->FLUX_META.DESCRIPTION= calloc(DALECmodel->nofluxes, sizeof(char *));

    DALECmodel->PARS_META.NAME= calloc(DALECmodel->nopars, sizeof(char *));
    DALECmodel->PARS_META.ABBREVIATION= calloc(DALECmodel->nopars, sizeof(char *));
    DALECmodel->PARS_META.UNITS= calloc(DALECmodel->nopars, sizeof(char *));
    DALECmodel->PARS_META.DESCRIPTION= calloc(DALECmodel->nopars, sizeof(char *));

    DALECmodel->POOLS_META.NAME= calloc(DALECmodel->nopools, sizeof(char *));
    DALECmodel->POOLS_META.ABBREVIATION= calloc(DALECmodel->nopools, sizeof(char *));
    DALECmodel->POOLS_META.UNITS= calloc(DALECmodel->nopools, sizeof(char *));
    DALECmodel->POOLS_META.DESCRIPTION= calloc(DALECmodel->nopools, sizeof(char *));

    DALECmodel->EDC_META.NAME= calloc(DALECmodel->noedcs, sizeof(char *));
    DALECmodel->EDC_META.ABBREVIATION= calloc(DALECmodel->noedcs, sizeof(char *));
    DALECmodel->EDC_META.UNITS= calloc(DALECmodel->noedcs, sizeof(char *));
    DALECmodel->EDC_META.DESCRIPTION= calloc(DALECmodel->noedcs, sizeof(char *));


    //----------------METADATA DEFINITIONS----------------
    // This block contains the actual metadata itself
    // NOTE: While ABBREVIATION is required, NAME, UNITS and DESCRIPTION are optional 
    //
    // WARNING: if you are adding a whole new item, you must also add the new item to DALEC_####_INDICES.c 
    //
    // EXAMPLE: the flux "Banana crop yield" with abbreviation "banana" needs to be added to DALEC 1234
    // The following would be added to this current file:
    //
    //++     DALECmodel->FLUX_META.NAME[F.banana]="Banana crop yield";
    //++     DALECmodel->FLUX_META.ABBREVIATION[F.banana]="banana";   --I strongly advise keeping the abbreviation the same as the name in DALEC_####_INDICES.c 
    //++     DALECmodel->FLUX_META.UNITS[F.banana]="KiloBanana";
    //++     DALECmodel->FLUX_META.DESCRIPTION[F.banana]="Number of bananas harvested, in industry standard KiloBananas";
    //
    //
    // In addition, one would need to change the struct DALEC_1234_FLUXES in DALEC_1234_INDICES.c
    //
    //       struct DALEC_1234_FLUXES{
    //       /*DALEC FLUXES*/
    //       int gpp;   /*GPP*/
    //       int gppnet;   /*GPP*/
    //       int resp_auto;   /*Autotrophic respiration*/
    //++     int banana;   /*Bananas harvested*/
    //       } DALEC_1234_FLUXES={
    //++      0, 1, 2, 3, 4
    //       };
    //



    // Carbon, Water, Energy Fluxes
    DALECmodel->FLUX_META.NAME[F.gpp]="Gross Primary productivity";
    DALECmodel->FLUX_META.ABBREVIATION[F.gpp]="GPP";
    DALECmodel->FLUX_META.UNITS[F.gpp]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.gpp]="GPP, doesn\"t include maintenance respiration";

    DALECmodel->FLUX_META.NAME[F.gppnet]="Net Gross Primary Productivity";
    DALECmodel->FLUX_META.ABBREVIATION[F.gppnet]="gppnet";
    DALECmodel->FLUX_META.UNITS[F.gppnet]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.gppnet]="GPP, including maintenance respiration";

    DALECmodel->FLUX_META.NAME[F.resp_auto]="Total autotrophic respiration";
    DALECmodel->FLUX_META.ABBREVIATION[F.resp_auto]="resp_auto";
    DALECmodel->FLUX_META.UNITS[F.resp_auto]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.resp_auto]="Includes maintenance respiration and growth respiration";

    DALECmodel->FLUX_META.NAME[F.lab_prod]="Labile production";
    DALECmodel->FLUX_META.ABBREVIATION[F.lab_prod]="lab_prod";
    DALECmodel->FLUX_META.UNITS[F.lab_prod]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.lab_prod]="TBD";

    DALECmodel->FLUX_META.NAME[F.foliar_prod]="Labile release";
    DALECmodel->FLUX_META.ABBREVIATION[F.foliar_prod]="foliar_prod";
    DALECmodel->FLUX_META.UNITS[F.foliar_prod]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.foliar_prod]="TBD";

    DALECmodel->FLUX_META.NAME[F.root_prod]="Root production";
    DALECmodel->FLUX_META.ABBREVIATION[F.root_prod]="root_prod";
    DALECmodel->FLUX_META.UNITS[F.root_prod]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.root_prod]="TBD";

    DALECmodel->FLUX_META.NAME[F.wood_prod]="Wood production";
    DALECmodel->FLUX_META.ABBREVIATION[F.wood_prod]="wood_prod";
    DALECmodel->FLUX_META.UNITS[F.wood_prod]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.wood_prod]="TBD";

    DALECmodel->FLUX_META.NAME[F.fol2lit]="Foliar decomposition";
    DALECmodel->FLUX_META.ABBREVIATION[F.fol2lit]="fol2lit";
    DALECmodel->FLUX_META.UNITS[F.fol2lit]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.fol2lit]="TBD";

    DALECmodel->FLUX_META.NAME[F.woo2cwd]="Wood decomposition";
    DALECmodel->FLUX_META.ABBREVIATION[F.woo2cwd]="woo2cwd";
    DALECmodel->FLUX_META.UNITS[F.woo2cwd]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.woo2cwd]="TBD";

    DALECmodel->FLUX_META.NAME[F.roo2lit]="Root decomposition";
    DALECmodel->FLUX_META.ABBREVIATION[F.roo2lit]="roo2lit";
    DALECmodel->FLUX_META.UNITS[F.roo2lit]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.roo2lit]="TBD";

    DALECmodel->FLUX_META.NAME[F.cwd2som]="CWD decomposition";
    DALECmodel->FLUX_META.ABBREVIATION[F.cwd2som]="cwd2som";
    DALECmodel->FLUX_META.UNITS[F.cwd2som]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.cwd2som]="TBD";

    DALECmodel->FLUX_META.NAME[F.lit2som]="Litter decomposition";
    DALECmodel->FLUX_META.ABBREVIATION[F.lit2som]="lit2som";
    DALECmodel->FLUX_META.UNITS[F.lit2som]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.lit2som]="TBD";

    DALECmodel->FLUX_META.NAME[F.f_total]="Total Fire Flux";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_total]="f_total";
    DALECmodel->FLUX_META.UNITS[F.f_total]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_total]="TBD";

    DALECmodel->FLUX_META.NAME[F.f_lab]="Labile fire loss";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_lab]="f_lab";
    DALECmodel->FLUX_META.UNITS[F.f_lab]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_lab]="TBD";

    DALECmodel->FLUX_META.NAME[F.f_fol]="Foliar fire loss";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_fol]="f_fol";
    DALECmodel->FLUX_META.UNITS[F.f_fol]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_fol]="TBD";

    DALECmodel->FLUX_META.NAME[F.f_roo]="Root fire loss";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_roo]="f_roo";
    DALECmodel->FLUX_META.UNITS[F.f_roo]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_roo]="TBD";

    DALECmodel->FLUX_META.NAME[F.f_woo]="Wood fire loss";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_woo]="f_woo";
    DALECmodel->FLUX_META.UNITS[F.f_woo]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_woo]="TBD";

    DALECmodel->FLUX_META.NAME[F.f_cwd]="CWD fire loss";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_cwd]="f_cwd";
    DALECmodel->FLUX_META.UNITS[F.f_cwd]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_cwd]="TBD";

    DALECmodel->FLUX_META.NAME[F.f_lit]="Litter fire loss";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_lit]="f_lit";
    DALECmodel->FLUX_META.UNITS[F.f_lit]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_lit]="TBD";

    DALECmodel->FLUX_META.NAME[F.f_som]="Soil fire loss";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_som]="f_som";
    DALECmodel->FLUX_META.UNITS[F.f_som]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_som]="TBD";

    DALECmodel->FLUX_META.NAME[F.fx_lab2lit]="Fire labile to litter";
    DALECmodel->FLUX_META.ABBREVIATION[F.fx_lab2lit]="fx_lab2lit";
    DALECmodel->FLUX_META.UNITS[F.fx_lab2lit]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.fx_lab2lit]="Fire transfer labile to litter";

    DALECmodel->FLUX_META.NAME[F.fx_fol2lit]="Fire foliar to litter";
    DALECmodel->FLUX_META.ABBREVIATION[F.fx_fol2lit]="fx_fol2lit";
    DALECmodel->FLUX_META.UNITS[F.fx_fol2lit]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.fx_fol2lit]="Fire transfer foliar to litter";

    DALECmodel->FLUX_META.NAME[F.fx_roo2lit]="Fire root to litter";
    DALECmodel->FLUX_META.ABBREVIATION[F.fx_roo2lit]="fx_roo2lit";
    DALECmodel->FLUX_META.UNITS[F.fx_roo2lit]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.fx_roo2lit]="Fire transfer root to litter";

    DALECmodel->FLUX_META.NAME[F.fx_woo2cwd]="Fire wood to CWD";
    DALECmodel->FLUX_META.ABBREVIATION[F.fx_woo2cwd]="fx_woo2cwd";
    DALECmodel->FLUX_META.UNITS[F.fx_woo2cwd]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.fx_woo2cwd]="Fire transfer wood to CWD";

    DALECmodel->FLUX_META.NAME[F.fx_cwd2som]="Fire CWD to soil";
    DALECmodel->FLUX_META.ABBREVIATION[F.fx_cwd2som]="fx_cwd2som";
    DALECmodel->FLUX_META.UNITS[F.fx_cwd2som]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.fx_cwd2som]="Fire transfer CWD to soil";

    DALECmodel->FLUX_META.NAME[F.fx_lit2som]="Fire litter to soil";
    DALECmodel->FLUX_META.ABBREVIATION[F.fx_lit2som]="fx_lit2som";
    DALECmodel->FLUX_META.UNITS[F.fx_lit2som]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.fx_lit2som]="Fire transfer litter to soil";

    DALECmodel->FLUX_META.NAME[F.q_ly1]="LY1 runoff";
    DALECmodel->FLUX_META.ABBREVIATION[F.q_ly1]="q_ly1";
    DALECmodel->FLUX_META.UNITS[F.q_ly1]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.q_ly1]="LY1 runoff";

    DALECmodel->FLUX_META.NAME[F.q_ly2]="LY2 runoff";
    DALECmodel->FLUX_META.ABBREVIATION[F.q_ly2]="q_ly2";
    DALECmodel->FLUX_META.UNITS[F.q_ly2]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.q_ly2]="LY2 runoff";

    DALECmodel->FLUX_META.NAME[F.ly1xly2]="LY1 to LY2 transfer";
    DALECmodel->FLUX_META.ABBREVIATION[F.ly1xly2]="ly1xly2";
    DALECmodel->FLUX_META.UNITS[F.ly1xly2]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.ly1xly2]="LY1->LY2 transfer";

    DALECmodel->FLUX_META.NAME[F.ly2xly3]="LY2 to LY3 transfer";
    DALECmodel->FLUX_META.ABBREVIATION[F.ly2xly3]="ly2xly3";
    DALECmodel->FLUX_META.UNITS[F.ly2xly3]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.ly2xly3]="LY2->LY3 transfer";

    DALECmodel->FLUX_META.NAME[F.q_ly3]="LY3 runoff";
    DALECmodel->FLUX_META.ABBREVIATION[F.q_ly3]="q_ly3";
    DALECmodel->FLUX_META.UNITS[F.q_ly3]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.q_ly3]="LY3 runoff";

    DALECmodel->FLUX_META.NAME[F.q_surf]="Surface runoff";
    DALECmodel->FLUX_META.ABBREVIATION[F.q_surf]="q_surf";
    DALECmodel->FLUX_META.UNITS[F.q_surf]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.q_surf]="Surface runoff";

    DALECmodel->FLUX_META.NAME[F.infil]="Infiltration";
    DALECmodel->FLUX_META.ABBREVIATION[F.infil]="infil";
    DALECmodel->FLUX_META.UNITS[F.infil]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.infil]="INFILTRATION";

    DALECmodel->FLUX_META.NAME[F.infil_e]="Infiltration IE";
    DALECmodel->FLUX_META.ABBREVIATION[F.infil_e]="infil_e";
    DALECmodel->FLUX_META.UNITS[F.infil_e]="TBD";
    DALECmodel->FLUX_META.DESCRIPTION[F.infil_e]="INFILTRATION IE: temp = weight average of snow melt (0C) + liquid precip (@air temp)";

    DALECmodel->FLUX_META.NAME[F.q_ly1_e]="Q LY1 IE";
    DALECmodel->FLUX_META.ABBREVIATION[F.q_ly1_e]="q_ly1_e";
    DALECmodel->FLUX_META.UNITS[F.q_ly1_e]="TBD";
    DALECmodel->FLUX_META.DESCRIPTION[F.q_ly1_e]="Q LY1 IE: temp = LY1 temp";

    DALECmodel->FLUX_META.NAME[F.q_ly2_e]="Q LY2 IE";
    DALECmodel->FLUX_META.ABBREVIATION[F.q_ly2_e]="q_ly2_e";
    DALECmodel->FLUX_META.UNITS[F.q_ly2_e]="TBD";
    DALECmodel->FLUX_META.DESCRIPTION[F.q_ly2_e]="Q LY2 IE: temp = LY2 temp";

    DALECmodel->FLUX_META.NAME[F.q_ly3_e]="Q LY3 IE";
    DALECmodel->FLUX_META.ABBREVIATION[F.q_ly3_e]="q_ly3_e";
    DALECmodel->FLUX_META.UNITS[F.q_ly3_e]="TBDy";
    DALECmodel->FLUX_META.DESCRIPTION[F.q_ly3_e]="Q LY3 IE: temp = LY3 temp";

    DALECmodel->FLUX_META.NAME[F.ly1xly2_e]="LY1 to NDZ IE";
    DALECmodel->FLUX_META.ABBREVIATION[F.ly1xly2_e]="ly1xly2_e";
    DALECmodel->FLUX_META.UNITS[F.ly1xly2_e]="TBD";
    DALECmodel->FLUX_META.DESCRIPTION[F.ly1xly2_e]="LY1 to NDZ transfer IE: temp of donor";

    DALECmodel->FLUX_META.NAME[F.ly2xly3_e]="LY2 to LY3 IE";
    DALECmodel->FLUX_META.ABBREVIATION[F.ly2xly3_e]="ly2xly3_e";
    DALECmodel->FLUX_META.UNITS[F.ly2xly3_e]="TBD";
    DALECmodel->FLUX_META.DESCRIPTION[F.ly2xly3_e]="LY2 to LY3 transfer IE: temp of donor";

    DALECmodel->FLUX_META.NAME[F.evap_e]="Evaporation E";
    DALECmodel->FLUX_META.ABBREVIATION[F.evap_e]="evap_e";
    DALECmodel->FLUX_META.UNITS[F.evap_e]="TBD";
    DALECmodel->FLUX_META.DESCRIPTION[F.evap_e]="See Retano calculation";

    DALECmodel->FLUX_META.NAME[F.transp1_e]="Transpiration 1 E";
    DALECmodel->FLUX_META.ABBREVIATION[F.transp1_e]="transp1_e";
    DALECmodel->FLUX_META.UNITS[F.transp1_e]="TBD";
    DALECmodel->FLUX_META.DESCRIPTION[F.transp1_e]="See Retano calculation";

    DALECmodel->FLUX_META.NAME[F.transp2_e]="Transpiration 2 E";
    DALECmodel->FLUX_META.ABBREVIATION[F.transp2_e]="transp2_e";
    DALECmodel->FLUX_META.UNITS[F.transp2_e]="TBD";
    DALECmodel->FLUX_META.DESCRIPTION[F.transp2_e]="See Retano calculation";

    DALECmodel->FLUX_META.NAME[F.transp1]="Transpiration 1";
    DALECmodel->FLUX_META.ABBREVIATION[F.transp1]="transp1";
    DALECmodel->FLUX_META.UNITS[F.transp1]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.transp1]="Transpiration";

    DALECmodel->FLUX_META.NAME[F.transp2]="Transpiration 2";
    DALECmodel->FLUX_META.ABBREVIATION[F.transp2]="transp2";
    DALECmodel->FLUX_META.UNITS[F.transp2]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.transp2]="Transpiration";

    DALECmodel->FLUX_META.NAME[F.evap]="Evaporation";
    DALECmodel->FLUX_META.ABBREVIATION[F.evap]="evap";
    DALECmodel->FLUX_META.UNITS[F.evap]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.evap]="Evaporation";

    DALECmodel->FLUX_META.NAME[F.snowfall]="Snowfall";
    DALECmodel->FLUX_META.ABBREVIATION[F.snowfall]="snowfall";
    DALECmodel->FLUX_META.UNITS[F.snowfall]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.snowfall]="Snowfall";

    DALECmodel->FLUX_META.NAME[F.melt]="Melt";
    DALECmodel->FLUX_META.ABBREVIATION[F.melt]="melt";
    DALECmodel->FLUX_META.UNITS[F.melt]="kgH20/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.melt]="melt";

    DALECmodel->FLUX_META.NAME[F.ae_rh_cwd]="Aerobic heterotrophic respiration coarse woody debris";
    DALECmodel->FLUX_META.ABBREVIATION[F.ae_rh_cwd]="ae_rh_cwd";
    DALECmodel->FLUX_META.UNITS[F.ae_rh_cwd]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.ae_rh_cwd]="Aerobic heterotrophic respiration from coarse woody debris pool";

    DALECmodel->FLUX_META.NAME[F.ae_rh_lit]="Aerobic heterotrophic respiration coarse woody litter";
    DALECmodel->FLUX_META.ABBREVIATION[F.ae_rh_lit]="ae_rh_lit";
    DALECmodel->FLUX_META.UNITS[F.ae_rh_lit]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.ae_rh_lit]="Aerobic heterotrophic respiration from litter pool";

    DALECmodel->FLUX_META.NAME[F.ae_rh_som]="Aerobic heterotrophic respiration soil organic matter";
    DALECmodel->FLUX_META.ABBREVIATION[F.ae_rh_som]="ae_rh_som";
    DALECmodel->FLUX_META.UNITS[F.ae_rh_som]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.ae_rh_som]="Aerobic heterotrophic respiration from soil organic matter pool";

    DALECmodel->FLUX_META.NAME[F.an_rh_cwd]="Anaerobic heterotrophic respiration coarse woody debris";
    DALECmodel->FLUX_META.ABBREVIATION[F.an_rh_cwd]="an_rh_cwd";
    DALECmodel->FLUX_META.UNITS[F.an_rh_cwd]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.an_rh_cwd]="Anaerobic heterotrophic respiration from coarse woody debris pool";

    DALECmodel->FLUX_META.NAME[F.an_rh_lit]="Anaerobic heterotrophic respiration litter";
    DALECmodel->FLUX_META.ABBREVIATION[F.an_rh_lit]="an_rh_lit";
    DALECmodel->FLUX_META.UNITS[F.an_rh_lit]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.an_rh_lit]="Anaerobic heterotrophic respiration from litter pool";

    DALECmodel->FLUX_META.NAME[F.an_rh_som]="Anaerobic heterotrophic respiration soil organic matter";
    DALECmodel->FLUX_META.ABBREVIATION[F.an_rh_som]="an_rh_som";
    DALECmodel->FLUX_META.UNITS[F.an_rh_som]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.an_rh_som]="Anaerobic heterotrophic respiration from soil organic matter pool";
    
    DALECmodel->FLUX_META.NAME[F.rh_co2]="Heterotrophic respiration CO2 flux";
    DALECmodel->FLUX_META.ABBREVIATION[F.rh_co2]="rh_co2";
    DALECmodel->FLUX_META.UNITS[F.rh_co2]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.rh_co2]="Total CO2 flux from heterotrophic respiration";
    
    DALECmodel->FLUX_META.NAME[F.rh_ch4]="Heterotrophic respiration CH4 flux";
    DALECmodel->FLUX_META.ABBREVIATION[F.rh_ch4]="rh_ch4";
    DALECmodel->FLUX_META.UNITS[F.rh_ch4]="gCH4/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.rh_ch4]="Total CH4 flux from heterotrophic respiration";
    
    DALECmodel->FLUX_META.NAME[F.aetr]="Aerobic turnover scalar";
    DALECmodel->FLUX_META.ABBREVIATION[F.aetr]="aetr";
    DALECmodel->FLUX_META.UNITS[F.aetr]="scalar*";
    DALECmodel->FLUX_META.DESCRIPTION[F.aetr]="Aerobic turnover scalar*";

    DALECmodel->FLUX_META.NAME[F.antr]="Anaerobic turnover scalar";
    DALECmodel->FLUX_META.ABBREVIATION[F.antr]="antr";
    DALECmodel->FLUX_META.UNITS[F.antr]="scalar*";
    DALECmodel->FLUX_META.DESCRIPTION[F.antr]="Anaerobic turnover scalar*";

    DALECmodel->FLUX_META.NAME[F.an_co2_c_ratio]="Anaerobic CO2 C ratio";
    DALECmodel->FLUX_META.ABBREVIATION[F.an_co2_c_ratio]="an_co2_c_ratio";
    DALECmodel->FLUX_META.UNITS[F.an_co2_c_ratio]="fraction*";
    DALECmodel->FLUX_META.DESCRIPTION[F.an_co2_c_ratio]="CO2 fraction in anaerobic C decomposition*";

    DALECmodel->FLUX_META.NAME[F.an_ch4_c_ratio]="Anaerobic CH4 C ratio";
    DALECmodel->FLUX_META.ABBREVIATION[F.an_ch4_c_ratio]="an_ch4_c_ratio";
    DALECmodel->FLUX_META.UNITS[F.an_ch4_c_ratio]="fraction*";
    DALECmodel->FLUX_META.DESCRIPTION[F.an_ch4_c_ratio]="CH4 fraction in anaerobic C decomposition*";

    DALECmodel->FLUX_META.NAME[F.target_LAI]="Target leaf area index";
    DALECmodel->FLUX_META.ABBREVIATION[F.target_LAI]="target_LAI";
    DALECmodel->FLUX_META.UNITS[F.target_LAI]="m2/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.target_LAI]="Target LAI calculated based on KNORR phenology scheme influenced by water, light, and temperature. Coded in KNORR_ALLOCATION.c";

    DALECmodel->FLUX_META.NAME[F.dlambda_dt]="Delta leaf area index";
    DALECmodel->FLUX_META.ABBREVIATION[F.dlambda_dt]="dlambda_dt";
    DALECmodel->FLUX_META.UNITS[F.dlambda_dt]="m2/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.dlambda_dt]="Difference between Target leaf area index (target_LAI) and current leaf area index. Coded in KNORR_ALLOCATION.c ";

    DALECmodel->FLUX_META.NAME[F.f_temp_thresh]="Temperature threshold";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_temp_thresh]="f_temp_thresh";
    DALECmodel->FLUX_META.UNITS[F.f_temp_thresh]="fraction";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_temp_thresh]="Fraction of plants within pixel able to grow foliage on temperature threshold. Coded in KNORR_ALLOCATION.c";

    DALECmodel->FLUX_META.NAME[F.f_dayl_thresh]="Daylight threshold";
    DALECmodel->FLUX_META.ABBREVIATION[F.f_dayl_thresh]="f_dayl_thresh";
    DALECmodel->FLUX_META.UNITS[F.f_dayl_thresh]="fraction";
    DALECmodel->FLUX_META.DESCRIPTION[F.f_dayl_thresh]="Fraction of plants within pixel able to grow foliage based on daylight threshold. Coded in KNORR_ALLOCATION.c";

    DALECmodel->FLUX_META.NAME[F.lai_fire]="Leaf area index fire loss";
    DALECmodel->FLUX_META.ABBREVIATION[F.lai_fire]="lai_fire";
    DALECmodel->FLUX_META.UNITS[F.lai_fire]="m2/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.lai_fire]="Leaf area index fire lost to fire*";

    DALECmodel->FLUX_META.NAME[F.foliar_fire_frac]="C_fol fire loss fraction";
    DALECmodel->FLUX_META.ABBREVIATION[F.foliar_fire_frac]="foliar_fire_frac";
    DALECmodel->FLUX_META.UNITS[F.foliar_fire_frac]="fraction";
    DALECmodel->FLUX_META.DESCRIPTION[F.foliar_fire_frac]="Foliage pool lost due to fire*";

    DALECmodel->FLUX_META.NAME[F.net_radiation]="Net radiation flux";
    DALECmodel->FLUX_META.ABBREVIATION[F.net_radiation]="net_radiation";
    DALECmodel->FLUX_META.UNITS[F.net_radiation]="W/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.net_radiation]="Net radiation flux defined as incoming shortwave radiation (SWin) - outgoing shortwave radiation (SWout) + incoming longwave radiation (LWin) - outgoing longwave radiation (LWout)";

    DALECmodel->FLUX_META.NAME[F.latent_heat]="Latent heat flux";
    DALECmodel->FLUX_META.ABBREVIATION[F.latent_heat]="latent_heat";
    DALECmodel->FLUX_META.UNITS[F.latent_heat]="W/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.latent_heat]="Latent heat flux";

    DALECmodel->FLUX_META.NAME[F.sensible_heat]="Sensible heat flux";
    DALECmodel->FLUX_META.ABBREVIATION[F.sensible_heat]="sensible_heat";
    DALECmodel->FLUX_META.UNITS[F.sensible_heat]="W/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.sensible_heat]="Sensible heat flux";

    DALECmodel->FLUX_META.NAME[F.ground_heat]="Ground heat flux";
    DALECmodel->FLUX_META.ABBREVIATION[F.ground_heat]="ground_heat";
    DALECmodel->FLUX_META.UNITS[F.ground_heat]="W/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.ground_heat]="Ground heat flux";

    DALECmodel->FLUX_META.NAME[F.gh_in]="Ground heat flux in converted units";
    DALECmodel->FLUX_META.ABBREVIATION[F.gh_in]="gh_in";
    DALECmodel->FLUX_META.UNITS[F.gh_in]="J/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.gh_in]="Ground heat flux in converted units";

    DALECmodel->FLUX_META.NAME[F.resp_auto_growth]="Autotrophic growth respiration";
    DALECmodel->FLUX_META.ABBREVIATION[F.resp_auto_growth]="resp_auto_growth";
    DALECmodel->FLUX_META.UNITS[F.resp_auto_growth]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.resp_auto_growth]="Autotrophic growth respiration";

    DALECmodel->FLUX_META.NAME[F.resp_auto_maint]="Autotrophic maintenance respiration";
    DALECmodel->FLUX_META.ABBREVIATION[F.resp_auto_maint]="resp_auto_maint";
    DALECmodel->FLUX_META.UNITS[F.resp_auto_maint]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.resp_auto_maint]="Autotrophic maintenance respiration";

    DALECmodel->FLUX_META.NAME[F.SWin]="Incoming shortwave radiation";
    DALECmodel->FLUX_META.ABBREVIATION[F.SWin]="SWin";
    DALECmodel->FLUX_META.UNITS[F.SWin]="W/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.SWin]="Incoming shortwave radiation";

    DALECmodel->FLUX_META.NAME[F.SWout]="Outgoing shortwave radiation";
    DALECmodel->FLUX_META.ABBREVIATION[F.SWout]="SWout";
    DALECmodel->FLUX_META.UNITS[F.SWout]="W/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.SWout]="Outgoing shortwave radiation";

    DALECmodel->FLUX_META.NAME[F.LWin]="Incoming longwave radiation";
    DALECmodel->FLUX_META.ABBREVIATION[F.LWin]="LWin";
    DALECmodel->FLUX_META.UNITS[F.LWin]="W/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.LWin]="Incoming longwave radiation";

    DALECmodel->FLUX_META.NAME[F.LWout]="Outgoing longwave radiation";
    DALECmodel->FLUX_META.ABBREVIATION[F.LWout]="LWout";
    DALECmodel->FLUX_META.UNITS[F.LWout]="W/m2";
    DALECmodel->FLUX_META.DESCRIPTION[F.LWout]="Outgoing longwave radiation";

    DALECmodel->FLUX_META.NAME[F.ly1xly2_th_e]="Thermal conductivity layer one to layer two*";
    DALECmodel->FLUX_META.ABBREVIATION[F.ly1xly2_th_e]="ly1xly2_th_e";
    DALECmodel->FLUX_META.UNITS[F.ly1xly2_th_e]="J/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.ly1xly2_th_e]="Thermal conductivity between layer one and layer two*";

    DALECmodel->FLUX_META.NAME[F.ly2xly3_th_e]="Thermal conductivity layer two to layer three*";
    DALECmodel->FLUX_META.ABBREVIATION[F.ly2xly3_th_e]="ly2xly3_th_e";
    DALECmodel->FLUX_META.UNITS[F.ly2xly3_th_e]="J/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.ly2xly3_th_e]="Thermal conductivity between layer two and layer three*";

    DALECmodel->FLUX_META.NAME[F.resp_auto_maint_dark]="Autotrophic maintenance dark respiration";
    DALECmodel->FLUX_META.ABBREVIATION[F.resp_auto_maint_dark]="resp_auto_maint_dark";
    DALECmodel->FLUX_META.UNITS[F.resp_auto_maint_dark]="gC/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.resp_auto_maint_dark]="Autotrophic maintenance dark respiration from foliage*";

    DALECmodel->FLUX_META.NAME[F.sublimation]="Sublimation";
    DALECmodel->FLUX_META.ABBREVIATION[F.sublimation]="sublimation";
    DALECmodel->FLUX_META.UNITS[F.sublimation]="kgH2O/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.sublimation]="Sublimation*";

    DALECmodel->FLUX_META.NAME[F.geological]="Geological";
    DALECmodel->FLUX_META.ABBREVIATION[F.geological]="geological";
    DALECmodel->FLUX_META.UNITS[F.geological]="J/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.geological]="Prederived geological flux in J/m2/day(105mW/m2)";

    DALECmodel->FLUX_META.NAME[F.ets]="Total land-atmosphere water";
    DALECmodel->FLUX_META.ABBREVIATION[F.ets]="ets";
    DALECmodel->FLUX_META.UNITS[F.ets]="kgH2O/m2/day";
    DALECmodel->FLUX_META.DESCRIPTION[F.ets]="Total land-atmosphere water flux (evaporation + transpiration + sublimation)";

    DALECmodel->FLUX_META.NAME[F.beta_factor]="Stress scaling factor";
    DALECmodel->FLUX_META.ABBREVIATION[F.beta_factor]="beta_factor";
    DALECmodel->FLUX_META.UNITS[F.beta_factor]="fraction";
    DALECmodel->FLUX_META.DESCRIPTION[F.beta_factor]="Stress due to cold weather and water availability";

    DALECmodel->FLUX_META.NAME[F.soil_beta_factor]="H2O stress scaling factor";
    DALECmodel->FLUX_META.ABBREVIATION[F.soil_beta_factor]="soil_beta_factor";
    DALECmodel->FLUX_META.UNITS[F.soil_beta_factor]="fraction";
    DALECmodel->FLUX_META.DESCRIPTION[F.soil_beta_factor]="Stress due to soil water availability";

    //Parameter info 

    //TODO: Anthony, this entry seems inconsistent with the rest of the code, was it in error?
    //DALECmodel->PARS_META.NAME[P.twood]="Turnover rate of wood";
    //DALECmodel->PARS_META.ABBREVIATION[P.twood]="twood";
    //DALECmodel->PARS_META.UNITS[P.twood]="gC/m2/day";
    //DALECmodel->PARS_META.DESCRIPTION[P.twood]="Includes maintenance respiration and growth respiration";

    DALECmodel->PARS_META.NAME[P.tr_lit2som]="Litter decomposition rate";
    DALECmodel->PARS_META.ABBREVIATION[P.tr_lit2som]="lit2som";
    DALECmodel->PARS_META.UNITS[P.tr_lit2som]="fraction";
    DALECmodel->PARS_META.DESCRIPTION[P.tr_lit2som]="Fraction of litter lost to soil organic matter per day";

    DALECmodel->PARS_META.NAME[P.tr_cwd2som]="CWD decomposition rate";
    DALECmodel->PARS_META.ABBREVIATION[P.tr_cwd2som]="cwd2som";
    DALECmodel->PARS_META.UNITS[P.tr_cwd2som]="fraction";
    DALECmodel->PARS_META.DESCRIPTION[P.tr_cwd2som]="Fraction of course woody debris lost to soil organic matter per day";

    DALECmodel->PARS_META.NAME[P.rauto_mr_r]="Autotrophic maintenance respiration coefficient for roots";
    DALECmodel->PARS_META.ABBREVIATION[P.rauto_mr_r]="rauto_mr_r]";
    //DALECmodel->PARS_META.UNITS[P.rauto_mr_r]="";
    DALECmodel->PARS_META.DESCRIPTION[P.rauto_mr_r]="Autotrophic maintenance respiration coefficient for roots - turnover rate at 25oC (d-1)";

    DALECmodel->PARS_META.NAME[P.rauto_mr_w]="Autotrophic maintenance respiration coefficient for wood";
    DALECmodel->PARS_META.ABBREVIATION[P.rauto_mr_w]="rauto_mr_w";
    //DALECmodel->PARS_META.UNITS[P.rauto_mr_w]="";
    DALECmodel->PARS_META.DESCRIPTION[P.rauto_mr_w]="Autotrophic maintenance respiration coefficient for wood - turnover rate at 25oC (d-1)";

    DALECmodel->PARS_META.NAME[P.rauto_mr_q10]="Autotrophic maintenance respiration Q10 parameter";
    DALECmodel->PARS_META.ABBREVIATION[P.rauto_mr_q10]="rauto_mr_q10";
    //DALECmodel->PARS_META.UNITS[P.rauto_mr_q10]="";
    DALECmodel->PARS_META.DESCRIPTION[P.rauto_mr_q10]="Autotrophic maintenance respiration Q10 parameter";

    DALECmodel->PARS_META.NAME[P.rauto_mrd_q10]="Dark respiration Q10 parameter";
    DALECmodel->PARS_META.ABBREVIATION[P.rauto_mrd_q10]="rauto_mrd_q10";
    //DALECmodel->PARS_META.UNITS[P.rauto_mrd_q10]="";
    DALECmodel->PARS_META.DESCRIPTION[P.rauto_mrd_q10]="Dark respiration Q10 parameter";

    DALECmodel->PARS_META.NAME[P.rauto_gr]="Growth yield";
    DALECmodel->PARS_META.ABBREVIATION[P.rauto_gr]="rauto_gr";
    DALECmodel->PARS_META.UNITS[P.rauto_gr]="g C";
    DALECmodel->PARS_META.DESCRIPTION[P.rauto_gr]="Growth yield (g C appearing in new biomass per g C used for growth; \"growth yield\" in Cannell and Thornley 2000)";

    DALECmodel->PARS_META.NAME[P.t_wood]="Turnover rate of wood";
    DALECmodel->PARS_META.ABBREVIATION[P.t_wood]="t_wood";
    DALECmodel->PARS_META.UNITS[P.t_wood]="gC/m2/day";
    DALECmodel->PARS_META.DESCRIPTION[P.t_wood]="Rate of wood loss per day* - 1% loss per year value";
    
    DALECmodel->PARS_META.NAME[P.t_root]="Turnover rate of roots";
    DALECmodel->PARS_META.ABBREVIATION[P.t_root]="t_root";
    DALECmodel->PARS_META.UNITS[P.t_root]="gC/m2/day";
    DALECmodel->PARS_META.DESCRIPTION[P.t_root]="Rate of root loss per day";

    DALECmodel->PARS_META.NAME[P.t_lit]="Turnover rate of litter";
    DALECmodel->PARS_META.ABBREVIATION[P.t_lit]="t_lit";
    DALECmodel->PARS_META.UNITS[P.t_lit]="gC/m2/day";
    DALECmodel->PARS_META.DESCRIPTION[P.t_lit]="Rate of litter turnover per day";
    
    DALECmodel->PARS_META.NAME[P.t_cwd]="Turnover rate of coarse woody debris";
    DALECmodel->PARS_META.ABBREVIATION[P.t_cwd]="t_cwd";
    DALECmodel->PARS_META.UNITS[P.t_cwd]="gC/m2/day";
    DALECmodel->PARS_META.DESCRIPTION[P.t_cwd]="Rate of coarse woody debris turnover per day";

    DALECmodel->PARS_META.NAME[P.t_som]="Turnover rate of soil organic matter";
    DALECmodel->PARS_META.ABBREVIATION[P.t_som]="t_som";
    DALECmodel->PARS_META.UNITS[P.t_som]="gC/m2/day";
    DALECmodel->PARS_META.DESCRIPTION[P.t_som]="Rate of soil turnover to heterotrophic respiration";
    
    DALECmodel->PARS_META.NAME[P.Q10rhco2]="Q10";
    DALECmodel->PARS_META.ABBREVIATION[P.Q10rhco2]="Q10";
    //DALECmodel->PARS_META.UNITS[P.Q10rhco2]="";
    //DALECmodel->PARS_META.DESCRIPTION[P.Q10rhco2]="";

    DALECmodel->PARS_META.NAME[P.LCMA]="Leaf mass carbon per area";
    DALECmodel->PARS_META.ABBREVIATION[P.LCMA]="LCMA";
    DALECmodel->PARS_META.UNITS[P.LCMA]="gC/m2";
    DALECmodel->PARS_META.DESCRIPTION[P.LCMA]=" Leaf mass carbon per unit leaf area; Kattge et al. 2011*;*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*";

    DALECmodel->PARS_META.NAME[P.i_labile]="Initial labile";
    DALECmodel->PARS_META.ABBREVIATION[P.i_labile]="i_labile";
    DALECmodel->PARS_META.UNITS[P.i_labile]="gC/m2";
    DALECmodel->PARS_META.DESCRIPTION[P.i_labile]="Size of labile C pool at time step 1";

    DALECmodel->PARS_META.NAME[P.i_foliar]="Initial foliar";
    DALECmodel->PARS_META.ABBREVIATION[P.i_foliar]="i_foliar";
    DALECmodel->PARS_META.UNITS[P.i_foliar]="gC/m2";
    DALECmodel->PARS_META.DESCRIPTION[P.i_labile]="Size of foliar C pool at time step 1";

    DALECmodel->PARS_META.NAME[P.i_root]="Initial roots";
    DALECmodel->PARS_META.ABBREVIATION[P.i_root]="i_root";
    DALECmodel->PARS_META.UNITS[P.i_root]="gC/m2";
    DALECmodel->PARS_META.DESCRIPTION[P.i_root]="Size of roots C pool at time step 1";

    DALECmodel->PARS_META.NAME[P.i_wood]="Initial wood";
    DALECmodel->PARS_META.ABBREVIATION[P.i_wood]="i_wood";
    DALECmodel->PARS_META.UNITS[P.i_wood]="gC/m2";
    DALECmodel->PARS_META.DESCRIPTION[P.i_wood]="Size of wood C pool at time step 1";

    DALECmodel->PARS_META.NAME[P.i_cwd]="Initial CWD";
    DALECmodel->PARS_META.ABBREVIATION[P.i_cwd]="i_cwd";
    DALECmodel->PARS_META.UNITS[P.i_cwd]="gC/m2";
    DALECmodel->PARS_META.DESCRIPTION[P.i_cwd]="Size of coarse woody debris C pool at time step 1";

    DALECmodel->PARS_META.NAME[P.i_lit]="Initial litter";
    DALECmodel->PARS_META.ABBREVIATION[P.i_lit]="i_lit";
    DALECmodel->PARS_META.UNITS[P.i_lit]="gC/m2";
    DALECmodel->PARS_META.DESCRIPTION[P.i_lit]="Size of litter C pool at time step 1";

    DALECmodel->PARS_META.NAME[P.i_som]="Initial soil organic matter";
    DALECmodel->PARS_META.ABBREVIATION[P.i_som]="i_som";
    DALECmodel->PARS_META.UNITS[P.i_som]="gC/m2";
    DALECmodel->PARS_META.DESCRIPTION[P.i_som]="Size of soil organic matter C pool at time step 1";

    DALECmodel->PARS_META.NAME[P.retention]="Retention parameter";
    DALECmodel->PARS_META.ABBREVIATION[P.retention]="retention";
    //DALECmodel->PARS_META.UNITS[P.retention]="";
    DALECmodel->PARS_META.DESCRIPTION[P.retention]="Retention parameter (b)";

    DALECmodel->PARS_META.NAME[P.i_LY1_SM]="LY1 SM at t0";
    DALECmodel->PARS_META.ABBREVIATION[P.i_LY1_SM]="i_LY1_SM";
    //DALECmodel->PARS_META.UNITS[P.i_LY1_SM]="";
    DALECmodel->PARS_META.DESCRIPTION[P.i_LY1_SM]="LY1 SM at t0";

    DALECmodel->PARS_META.NAME[P.cf_foliar]="Foliar biomass CF";
    DALECmodel->PARS_META.ABBREVIATION[P.cf_foliar]="cf_foliar";
    DALECmodel->PARS_META.UNITS[P.cf_foliar]="%";
    DALECmodel->PARS_META.DESCRIPTION[P.cf_foliar]="Foliar biomass combustion factor";
    
    DALECmodel->PARS_META.NAME[P.cf_ligneous]="Ligneous biomass CF";
    DALECmodel->PARS_META.ABBREVIATION[P.cf_ligneous]="cf_ligneous";
    DALECmodel->PARS_META.UNITS[P.cf_ligneous]="%";
    DALECmodel->PARS_META.DESCRIPTION[P.cf_ligneous]="Ligneous biomass combustion factor";
    
    DALECmodel->PARS_META.NAME[P.cf_DOM]="DOM CF";
    DALECmodel->PARS_META.ABBREVIATION[P.cf_DOM]="cf_DOM";
    DALECmodel->PARS_META.UNITS[P.cf_DOM]="%";
    DALECmodel->PARS_META.DESCRIPTION[P.cf_DOM]="Dead organic matter combustion factor";

    DALECmodel->PARS_META.NAME[P.resilience]="Resilience factor";
    DALECmodel->PARS_META.ABBREVIATION[P.resilience]="resilience";
    DALECmodel->PARS_META.UNITS[P.resilience]="%";
    DALECmodel->PARS_META.DESCRIPTION[P.resilience]="Resilience factor (since transfer to litter is represented as (1-pars[30]))";

    DALECmodel->PARS_META.NAME[P.hydr_cond]="Saturated hydraulic conductivity";
    DALECmodel->PARS_META.ABBREVIATION[P.hydr_cond]="hydr_cond";
    DALECmodel->PARS_META.UNITS[P.hydr_cond]="m/s";
    DALECmodel->PARS_META.DESCRIPTION[P.hydr_cond]="Saturated hydraulic conductivity";

    DALECmodel->PARS_META.NAME[P.max_infil]="Maximum infiltration";
    DALECmodel->PARS_META.ABBREVIATION[P.max_infil]="max_infil";
    DALECmodel->PARS_META.UNITS[P.max_infil]="mm/day";
    DALECmodel->PARS_META.DESCRIPTION[P.max_infil]="Maximum infiltration";

    DALECmodel->PARS_META.NAME[P.i_LY3_SM]="LY3 SM at t=0";
    DALECmodel->PARS_META.ABBREVIATION[P.i_LY3_SM]="i_LY3_SM";
    //DALECmodel->PARS_META.UNITS[P.i_LY3_SM]="";
    DALECmodel->PARS_META.DESCRIPTION[P.i_LY3_SM]="LY3 SM at t=0";

    DALECmodel->PARS_META.NAME[P.LY1_por]="LY1 porosity";
    DALECmodel->PARS_META.ABBREVIATION[P.LY1_por]="LY1_por";
    //DALECmodel->PARS_META.UNITS[P.LY1_por]="";
    DALECmodel->PARS_META.DESCRIPTION[P.LY1_por]="LY1 porosity";

    DALECmodel->PARS_META.NAME[P.LY2_por]="LY2 porosity";
    DALECmodel->PARS_META.ABBREVIATION[P.LY2_por]="LY2_por";
    //DALECmodel->PARS_META.UNITS[P.LY2_por]="";
    DALECmodel->PARS_META.DESCRIPTION[P.LY2_por]="LY2 porosity";

    DALECmodel->PARS_META.NAME[P.LY3_por]="LY3 porosity";
    DALECmodel->PARS_META.ABBREVIATION[P.LY3_por]="LY3_por";
    //DALECmodel->PARS_META.UNITS[P.LY3_por]="";
    DALECmodel->PARS_META.DESCRIPTION[P.LY3_por]="LY3 porosity";

    DALECmodel->PARS_META.NAME[P.field_cap]="Field capacity (negative) potential";
    DALECmodel->PARS_META.ABBREVIATION[P.field_cap]="field_cap";
    DALECmodel->PARS_META.UNITS[P.field_cap]="-Mpa";
    DALECmodel->PARS_META.DESCRIPTION[P.field_cap]="Field capacity (negative) potential";

    DALECmodel->PARS_META.NAME[P.LY1_z]="LY1 depth";
    DALECmodel->PARS_META.ABBREVIATION[P.LY1_z]="LY1_z";
    DALECmodel->PARS_META.UNITS[P.LY1_z]="m";
    DALECmodel->PARS_META.DESCRIPTION[P.LY1_z]="LY1 depth";

    DALECmodel->PARS_META.NAME[P.LY2_z]="LY2 depth";
    DALECmodel->PARS_META.ABBREVIATION[P.LY2_z]="LY2_z";
    DALECmodel->PARS_META.UNITS[P.LY2_z]="m";
    DALECmodel->PARS_META.DESCRIPTION[P.LY2_z]="LY2 depth";

    DALECmodel->PARS_META.NAME[P.LY3_z]="LY3 depth";
    DALECmodel->PARS_META.ABBREVIATION[P.LY3_z]="LY3_z";
    DALECmodel->PARS_META.UNITS[P.LY3_z]="m";
    DALECmodel->PARS_META.DESCRIPTION[P.LY3_z]="LY3 depth";

    DALECmodel->PARS_META.NAME[P.LY1_vhc]="LY1 volumetric heat capacity";
    DALECmodel->PARS_META.ABBREVIATION[P.LY1_vhc]="LY1_vhc";
    //DALECmodel->PARS_META.UNITS[P.LY1_vhc]="";
    DALECmodel->PARS_META.DESCRIPTION[P.LY1_vhc]="LY1 the change in heat content of a unit bulk volume of soil per unit change in temperature (https://www.sciencedirect.com/topics/engineering/volumetric-heat-capacity)";

    DALECmodel->PARS_META.NAME[P.LY2_vhc]="LY2 volumetric heat capacity";
    DALECmodel->PARS_META.ABBREVIATION[P.LY2_vhc]="LY2_vhc";
    //DALECmodel->PARS_META.UNITS[P.LY2_vhc]="";
    DALECmodel->PARS_META.DESCRIPTION[P.LY2_vhc]="LY2 the change in heat content of a unit bulk volume of soil per unit change in temperature (https://www.sciencedirect.com/topics/engineering/volumetric-heat-capacity)";

    DALECmodel->PARS_META.NAME[P.LY3_vhc]="LY3 volumetric heat capacity";
    DALECmodel->PARS_META.ABBREVIATION[P.LY3_vhc]="LY3_vhc";
    //DALECmodel->PARS_META.UNITS[P.LY3_vhc]="";
    DALECmodel->PARS_META.DESCRIPTION[P.LY3_vhc]="LY3 the change in heat content of a unit bulk volume of soil per unit change in temperature (https://www.sciencedirect.com/topics/engineering/volumetric-heat-capacity)";

    DALECmodel->PARS_META.NAME[P.Q_excess]="Runoff excess";
    DALECmodel->PARS_META.ABBREVIATION[P.Q_excess]="Q_excess";
    //DALECmodel->PARS_META.UNITS[P.Q_excess]="";
    DALECmodel->PARS_META.DESCRIPTION[P.Q_excess]="Runoff excess";

    //Pool info {Eren Bilir added this section 9/15/2023}

    DALECmodel->POOLS_META.NAME[S.C_lab]="Labile carbon pool";
    DALECmodel->POOLS_META.ABBREVIATION[S.C_lab]="C_lab";
    DALECmodel->POOLS_META.UNITS[S.C_lab]="gC/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.C_lab]="Stored non-structural carbohydrate (NSC) pool; also includes any structural C that is not wood, root, or leaf, e.g. flowers/fruit/seeds";

    DALECmodel->POOLS_META.NAME[S.C_fol]="Foliar carbon pool";
    DALECmodel->POOLS_META.ABBREVIATION[S.C_fol]="C_fol";
    DALECmodel->POOLS_META.UNITS[S.C_fol]="gC/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.C_fol]="Leaf carbon pool";

    DALECmodel->POOLS_META.NAME[S.C_roo]="Fine root carbon pool";
    DALECmodel->POOLS_META.ABBREVIATION[S.C_roo]="C_roo";
    DALECmodel->POOLS_META.UNITS[S.C_roo]="gC/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.C_roo]="Fine root carbon pool; does not include woody root biomass";

    DALECmodel->POOLS_META.NAME[S.C_woo]="Wood/ligneous carbon pool";
    DALECmodel->POOLS_META.ABBREVIATION[S.C_woo]="C_woo";
    DALECmodel->POOLS_META.UNITS[S.C_woo]="gC/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.C_woo]="Wood/ligneous carbon pool; includes woody root biomass";

    DALECmodel->POOLS_META.NAME[S.C_cwd]="Coarse woody debris carbon pool";
    DALECmodel->POOLS_META.ABBREVIATION[S.C_cwd]="C_cwd";
    DALECmodel->POOLS_META.UNITS[S.C_cwd]="gC/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.C_cwd]="Coarse woody debris carbon pool";

    DALECmodel->POOLS_META.NAME[S.C_lit]="Litter carbon pool";
    DALECmodel->POOLS_META.ABBREVIATION[S.C_lit]="C_lit";
    DALECmodel->POOLS_META.UNITS[S.C_lit]="gC/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.C_lit]="Litter carbon pool";

    DALECmodel->POOLS_META.NAME[S.C_som]="soil organic matter carbon pool";
    DALECmodel->POOLS_META.ABBREVIATION[S.C_som]="C_som";
    DALECmodel->POOLS_META.UNITS[S.C_som]="gC/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.C_som]="soil organic matter carbon pool";

    DALECmodel->POOLS_META.NAME[S.H2O_LY1]="Layer 1 water content";
    DALECmodel->POOLS_META.ABBREVIATION[S.H2O_LY1]="H2O_LY1";
    DALECmodel->POOLS_META.UNITS[S.H2O_LY1]="(kg H2O)/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.H2O_LY1]="Bulk water content of the first belowground layer. This layer hosts shallow vegetation roots and supplies water for transpiration, evaporation, runoff, and infiltration to LY2.";

    DALECmodel->POOLS_META.NAME[S.H2O_LY2]="Layer 2 water content";
    DALECmodel->POOLS_META.ABBREVIATION[S.H2O_LY2]="H2O_LY2";
    DALECmodel->POOLS_META.UNITS[S.H2O_LY2]="(kg H2O)/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.H2O_LY2]="Bulk water content of the second belowground layer. This layer hosts deep vegetation roots and supplies water for transpiration), runoff, and infiltration to LY3.";

    DALECmodel->POOLS_META.NAME[S.H2O_LY3]="Layer 3 water content";
    DALECmodel->POOLS_META.ABBREVIATION[S.H2O_LY3]="H2O_LY3";
    DALECmodel->POOLS_META.UNITS[S.H2O_LY3]="(kg H2O)/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.H2O_LY3]="Bulk water content of the third belowground layer. This layer is inaccessible to vegetation roots, and supplies water for runoff only.";

    DALECmodel->POOLS_META.NAME[S.H2O_SWE]="Snow Water Equivalent";
    DALECmodel->POOLS_META.ABBREVIATION[S.H2O_SWE]="H2O_SWE";
    DALECmodel->POOLS_META.UNITS[S.H2O_SWE]="(kg H2O)/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.H2O_SWE]="Bulk water content of the snow pack layer. ";

    DALECmodel->POOLS_META.NAME[S.E_LY1]="Layer 1 energy content";
    DALECmodel->POOLS_META.ABBREVIATION[S.E_LY1]="E_LY1";
    DALECmodel->POOLS_META.UNITS[S.E_LY1]="J/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.E_LY1]="Energy content of the first belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.E_LY2]="Layer 2 energy content";
    DALECmodel->POOLS_META.ABBREVIATION[S.E_LY2]="E_LY2";
    DALECmodel->POOLS_META.UNITS[S.E_LY2]="J/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.E_LY2]="Energy content of the second belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.E_LY3]="Layer 3 energy content";
    DALECmodel->POOLS_META.ABBREVIATION[S.E_LY3]="E_LY3";
    DALECmodel->POOLS_META.UNITS[S.E_LY3]="J/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.E_LY3]="Energy content of the third belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.D_LAI]="Leaf Area Index";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_LAI]="D_LAI";
    DALECmodel->POOLS_META.UNITS[S.D_LAI]="m2/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_LAI]="Leaf Area Index. \"D_\" flag denotes a diagnostic variable, which is stored for the user but is not passed to downstream model calculations.";

    DALECmodel->POOLS_META.NAME[S.D_SCF]="Snow Covered Fraction";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_SCF]="D_SCF";
    DALECmodel->POOLS_META.UNITS[S.D_SCF]="m2/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_SCF]="Fraction of non-vegetation-covered land surface (gap fraction) that is covered by a layer of snow. ";

    DALECmodel->POOLS_META.NAME[S.D_TEMP_LY1]="Layer 1 temperature";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_TEMP_LY1]="D_TEMP_LY1";
    DALECmodel->POOLS_META.UNITS[S.D_TEMP_LY1]="K";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_TEMP_LY1]="Temperature (K) of the first belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.D_TEMP_LY2]="Layer 2 temperature";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_TEMP_LY2]="D_TEMP_LY2";
    DALECmodel->POOLS_META.UNITS[S.D_TEMP_LY2]="K";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_TEMP_LY2]="Temperature (K) of the second belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.D_TEMP_LY3]="Layer 3 temperature";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_TEMP_LY3]="D_TEMP_LY3";
    DALECmodel->POOLS_META.UNITS[S.D_TEMP_LY3]="K";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_TEMP_LY3]="Temperature (K) of the third belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.D_LF_LY1]="Layer 1 Liquid Fraction";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_LF_LY1]="D_LF_LY1";
    DALECmodel->POOLS_META.UNITS[S.D_LF_LY1]="(kg H20 liquid water)/(kg H20 total water)";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_LF_LY1]="Fraction of the bulk water content existing in liquid state in the first belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.D_LF_LY2]="Layer 2 Liquid Fraction";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_LF_LY2]="D_LF_LY2";
    DALECmodel->POOLS_META.UNITS[S.D_LF_LY2]="(kg H20 liquid water)/(kg H20 total water)";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_LF_LY2]="Fraction of the bulk water content existing in liquid state in the second belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.D_LF_LY3]="Layer 3 Liquid Fraction";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_LF_LY3]="D_LF_LY3";
    DALECmodel->POOLS_META.UNITS[S.D_LF_LY3]="(kg H20 liquid water)/(kg H20 total water)";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_LF_LY3]="Fraction of the bulk water content existing in liquid state in the third belowground layer. ";

    DALECmodel->POOLS_META.NAME[S.D_SM_LY1]="Layer 1 soil moisture";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_SM_LY1]="D_SM_LY1";
    DALECmodel->POOLS_META.UNITS[S.D_SM_LY1]="((m3 H20)/(m3 total volume of pore space in soil volume)";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_SM_LY1]="Volumetric water content relative to available pore space in the first belowground layer.";

    DALECmodel->POOLS_META.NAME[S.D_SM_LY2]="Layer 2 soil moisture";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_SM_LY2]="D_SM_LY2";
    DALECmodel->POOLS_META.UNITS[S.D_SM_LY2]="((m3 H20)/(m3 total volume of pore space in soil volume)";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_SM_LY2]="Volumetric water content relative to available pore space in the second belowground layer.";

    DALECmodel->POOLS_META.NAME[S.D_SM_LY3]="Layer 3 soil moisture";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_SM_LY3]="D_SM_LY3";
    DALECmodel->POOLS_META.UNITS[S.D_SM_LY3]="((m3 H20)/(m3 total volume of pore space in soil volume)";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_SM_LY3]="Volumetric water content relative to available pore space in the third belowground layer.";

    DALECmodel->POOLS_META.NAME[S.D_PSI_LY1]="Layer 1 water potential";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_PSI_LY1]="D_PSI_LY1";
    DALECmodel->POOLS_META.UNITS[S.D_PSI_LY1]="MPa";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_PSI_LY1]="Water potential of the first belowground layer";

    DALECmodel->POOLS_META.NAME[S.D_PSI_LY2]="Layer 2 water potential";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_PSI_LY2]="D_PSI_LY2";
    DALECmodel->POOLS_META.UNITS[S.D_PSI_LY2]="MPa";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_PSI_LY2]="Water potential of the second belowground layer";

    DALECmodel->POOLS_META.NAME[S.D_PSI_LY3]="Layer 3 water potential";
    DALECmodel->POOLS_META.ABBREVIATION[S.D_PSI_LY3]="D_PSI_LY3";
    DALECmodel->POOLS_META.UNITS[S.D_PSI_LY3]="MPa";
    DALECmodel->POOLS_META.DESCRIPTION[S.D_PSI_LY3]="Water potential of the third belowground layer";

    DALECmodel->POOLS_META.NAME[S.M_LAI_MAX]="Past Maximum Leaf Area Index in vegetation memory";
    DALECmodel->POOLS_META.ABBREVIATION[S.M_LAI_MAX]="M_LAI_MAX";
    DALECmodel->POOLS_META.UNITS[S.M_LAI_MAX]="m2/m2";
    DALECmodel->POOLS_META.DESCRIPTION[S.M_LAI_MAX]="Exponentially declining term encoding vegetation \"memory\" of past water/structural limitations for LAI, to capture lagged effects of past stress.";

    DALECmodel->POOLS_META.NAME[S.M_LAI_TEMP]="Past Leaf temperature in vegetation memory";
    DALECmodel->POOLS_META.ABBREVIATION[S.M_LAI_TEMP]="M_LAI_TEMP";
    DALECmodel->POOLS_META.UNITS[S.M_LAI_TEMP]="K";
    DALECmodel->POOLS_META.DESCRIPTION[S.M_LAI_TEMP]="Exponentially declining term encoding vegetation \"memory\" of past temperature limitations for LAI, to capture lagged effects of past stress.";

//EDC info
    /*
    DALECmodel->EDC_META.NAME[E.twood]="Turnover rate of wood";
    DALECmodel->EDC_META.ABBREVIATION[E.twood]="twood";
    DALECmodel->EDC_META.UNITS[E.twood]="gC/m2/day";
    DALECmodel->EDC_META.DESCRIPTION[E.twood]="Includes maintenance respiration and growth respiration";

    /*/


    
}