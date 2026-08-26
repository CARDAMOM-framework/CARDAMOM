#pragma once
#include "DALEC_1111.c"

/*
 * PARS_INFO_1111: Parameter priors for DALEC-PyC
 * Inherits all 63 parameters from DALEC_1110 unchanged.
 * Adds 11 new parameters for climate-sensitive combustion and PyC dynamics.
 */

int PARS_INFO_1111(DATA *CARDADATA)
{

struct DALEC_1111_PARAMETERS P=DALEC_1111_PARAMETERS;

/* ---- All parameters inherited from DALEC_1110 (identical priors) ---- */

CARDADATA->parmin[P.tr_lit2soil]=0.00001;
CARDADATA->parmax[P.tr_lit2soil]=0.01;

CARDADATA->parmin[P.tr_cwd2som]=0.00001;
CARDADATA->parmax[P.tr_cwd2som]=0.01;

CARDADATA->parmin[P.f_auto]=0.2;
CARDADATA->parmax[P.f_auto]=0.8;

CARDADATA->parmin[P.f_foliar]=0.01;
CARDADATA->parmax[P.f_foliar]=0.5;

CARDADATA->parmin[P.f_root]=0.01;
CARDADATA->parmax[P.f_root]=1;

CARDADATA->parmin[P.t_foliar]=1.001;
CARDADATA->parmax[P.t_foliar]=8;

CARDADATA->parmin[P.t_wood]=0.000025;
CARDADATA->parmax[P.t_wood]=0.001;

CARDADATA->parmin[P.t_root]=0.0001;
CARDADATA->parmax[P.t_root]=0.01;

CARDADATA->parmin[P.t_lit]=0.0001;
CARDADATA->parmax[P.t_lit]=0.01;

CARDADATA->parmin[P.t_cwd]=0.00005;
CARDADATA->parmax[P.t_cwd]=0.005;

CARDADATA->parmin[P.t_soil]=0.0000001;
CARDADATA->parmax[P.t_soil]=0.001;

CARDADATA->parmin[P.Q10rhco2]=1.2;
CARDADATA->parmax[P.Q10rhco2]=2.0;

CARDADATA->parmin[P.Bday]=365.25;
CARDADATA->parmax[P.Bday]=365.25*4;

CARDADATA->parmin[P.f_lab]=0.01;
CARDADATA->parmax[P.f_lab]=0.5;

CARDADATA->parmin[P.labile_rel]=365.25/12;
CARDADATA->parmax[P.labile_rel]=100;

CARDADATA->parmin[P.Fday]=365.25;
CARDADATA->parmax[P.Fday]=365.25*4;

CARDADATA->parmin[P.leaf_fall]=365.25/12;
CARDADATA->parmax[P.leaf_fall]=150;

CARDADATA->parmin[P.LCMA]=5;
CARDADATA->parmax[P.LCMA]=200;

CARDADATA->parmin[P.i_labile]=1.0;
CARDADATA->parmax[P.i_labile]=2000.0;

CARDADATA->parmin[P.i_foliar]=1.0;
CARDADATA->parmax[P.i_foliar]=2000.0;

CARDADATA->parmin[P.i_root]=1.0;
CARDADATA->parmax[P.i_root]=2000.0;

CARDADATA->parmin[P.i_wood]=1.0;
CARDADATA->parmax[P.i_wood]=100000.0;

CARDADATA->parmin[P.i_cwd]=1.0;
CARDADATA->parmax[P.i_cwd]=100000.0;

CARDADATA->parmin[P.i_lit]=1.0;
CARDADATA->parmax[P.i_lit]=2000.0;

CARDADATA->parmin[P.i_soil]=1.0;
CARDADATA->parmax[P.i_soil]=200000.0;

CARDADATA->parmin[P.retention]=1.5;
CARDADATA->parmax[P.retention]=10;

CARDADATA->parmin[P.wilting]=1;
CARDADATA->parmax[P.wilting]=10000;

CARDADATA->parmin[P.i_PAW]=1;
CARDADATA->parmax[P.i_PAW]=10000;

CARDADATA->parmin[P.cf_foliar]=0.01;
CARDADATA->parmax[P.cf_foliar]=1;

CARDADATA->parmin[P.cf_ligneous]=0.01;
CARDADATA->parmax[P.cf_ligneous]=1;

CARDADATA->parmin[P.cf_DOM]=0.01;
CARDADATA->parmax[P.cf_DOM]=1;

CARDADATA->parmin[P.resilience]=0.01;
CARDADATA->parmax[P.resilience]=1;

CARDADATA->parmin[P.t_labile]=1.001;
CARDADATA->parmax[P.t_labile]=8;

CARDADATA->parmin[P.moisture]=0.01;
CARDADATA->parmax[P.moisture]=1;

CARDADATA->parmin[P.hydr_cond]=0.0000001;
CARDADATA->parmax[P.hydr_cond]=0.00001;

CARDADATA->parmin[P.max_infil]=1;
CARDADATA->parmax[P.max_infil]=100;

CARDADATA->parmin[P.i_PUW]=1;
CARDADATA->parmax[P.i_PUW]=10000;

CARDADATA->parmin[P.PAW_por]=0.2;
CARDADATA->parmax[P.PAW_por]=0.8;

CARDADATA->parmin[P.PUW_por]=0.2;
CARDADATA->parmax[P.PUW_por]=0.8;

CARDADATA->parmin[P.field_cap]=0.01;
CARDADATA->parmax[P.field_cap]=0.1;

CARDADATA->parmin[P.PAW_z]=0.01;
CARDADATA->parmax[P.PAW_z]=100;

CARDADATA->parmin[P.PUW_z]=0.01;
CARDADATA->parmax[P.PUW_z]=100;

CARDADATA->parmin[P.Q_excess]=0.01;
CARDADATA->parmax[P.Q_excess]=1;

CARDADATA->parmin[P.Med_g1]=1.79;
CARDADATA->parmax[P.Med_g1]=5.79;

CARDADATA->parmin[P.Vcmax25]=1e-8;
CARDADATA->parmax[P.Vcmax25]=140;

CARDADATA->parmin[P.Tminmin]=258.15;
CARDADATA->parmax[P.Tminmin]=273.15;

CARDADATA->parmin[P.Tminmax]=273.15;
CARDADATA->parmax[P.Tminmax]=288.15;

CARDADATA->parmin[P.ga]=0.01;
CARDADATA->parmax[P.ga]=10.0;

CARDADATA->parmin[P.Tupp]=299.15;
CARDADATA->parmax[P.Tupp]=318.15;

CARDADATA->parmin[P.Tdown]=263.15;
CARDADATA->parmax[P.Tdown]=286.15;

CARDADATA->parmin[P.C3_frac]=1e-8;
CARDADATA->parmax[P.C3_frac]=1.0;

CARDADATA->parmin[P.clumping]=0.35;
CARDADATA->parmax[P.clumping]=1.0;

CARDADATA->parmin[P.leaf_refl]=1e-8;
CARDADATA->parmax[P.leaf_refl]=1.0;

CARDADATA->parmin[P.i_SWE]=0.000001;
CARDADATA->parmax[P.i_SWE]=10000;

CARDADATA->parmin[P.min_melt]=240;
CARDADATA->parmax[P.min_melt]=270;

CARDADATA->parmin[P.melt_slope]=0.00001;
CARDADATA->parmax[P.melt_slope]=1;

CARDADATA->parmin[P.scf_scalar]=0.001;
CARDADATA->parmax[P.scf_scalar]=1000.0;

CARDADATA->parmin[P.S_fv]=1;
CARDADATA->parmax[P.S_fv]=100.0;

CARDADATA->parmin[P.thetas_opt]=0.2;
CARDADATA->parmax[P.thetas_opt]=1.0;

CARDADATA->parmin[P.fwc]=0.01;
CARDADATA->parmax[P.fwc]=1.0;

CARDADATA->parmin[P.r_ch4]=0.001;
CARDADATA->parmax[P.r_ch4]=0.9;

CARDADATA->parmin[P.Q10ch4]=1.0;
CARDADATA->parmax[P.Q10ch4]=3.0;

CARDADATA->parmin[P.maxPevap]=0.01;
CARDADATA->parmax[P.maxPevap]=20;

/* ---- New DALEC-PyC parameters ---- */

/*
 * CE_factor: controls combustion efficiency response to FRP (MW km-2).
 * CE = CE_min + (CE_max-CE_min) * (1 - exp(-CE_factor * FRP))
 * Calibrated against Wooster et al. (2005): FRE-combustion linear relationship.
 * Units depend on FRP units; 0.001-0.1 spans typical tropical fire FRP range.
 */
CARDADATA->parmin[P.CE_factor]=0.001;
CARDADATA->parmax[P.CE_factor]=0.1;

/*
 * alpha_char: charring efficiency — fraction of combusted woody C partitioned
 * to PyC rather than emitted as CO2/CO.
 * Prior from Jones et al. (2019) NatGeo: CWF ~17%, FWF ~8%, NWF ~5%.
 * Using upper end of CWF range as max; lower end of NWF as min.
 * Effective value modulated by f(CE) in the model.
 */
CARDADATA->parmin[P.alpha_char]=0.03;
CARDADATA->parmax[P.alpha_char]=0.20;

/*
 * beta_stab: stabilization fraction — proportion of new PyC entering C_PyC_R
 * (refractory, mineral-soil-incorporated) vs C_PyC_L (labile, O-horizon).
 * Prior: Beta(2.5, 3.5) → mean ~0.42 (Miesel et al. 2018; DeLuca et al. 2020).
 */
CARDADATA->parmin[P.beta_stab]=0.1;
CARDADATA->parmax[P.beta_stab]=0.9;

/*
 * k_PyC_L: labile PyC turnover rate (day-1).
 * Mean residence time 10-50 yr → 0.005-0.03 yr-1 → daily 1.37e-5 to 8.22e-5.
 * Wider prior to capture months-to-decades range (Abney et al. 2019).
 */
CARDADATA->parmin[P.k_PyC_L]=1e-6;
CARDADATA->parmax[P.k_PyC_L]=3e-4;

/*
 * k_PyC_R: refractory PyC turnover rate (day-1).
 * MRT 250-1000 yr → daily 2.74e-6 to 1.10e-5.
 * Extended lower bound for millennial-scale persistence (Schiedung et al. 2024).
 */
CARDADATA->parmin[P.k_PyC_R]=1e-8;
CARDADATA->parmax[P.k_PyC_R]=1e-5;

/*
 * i_PyC_L: initial labile PyC pool (gC m-2).
 * Small prior; labile pool is rapidly consumed post-fire.
 */
CARDADATA->parmin[P.i_PyC_L]=0.001;
CARDADATA->parmax[P.i_PyC_L]=500.0;

/*
 * i_PyC_R: initial refractory PyC pool (gC m-2).
 * Informed by SoilGrids SOC * regional PyC/SOC fractions (Reisser et al. 2016).
 * PyC can be 2-35% of SOC; SOC 0-30cm typically 1000-15000 gC m-2 globally.
 * Prior spans 0-5000 to accommodate fire-frequented ecosystems.
 */
CARDADATA->parmin[P.i_PyC_R]=0.001;
CARDADATA->parmax[P.i_PyC_R]=5000.0;

/*
 * psi50_fire: soil water potential threshold (MPa, entered as positive value)
 * at which fire combustion factors are half their maximum.
 * Mirrors hydraulic failure sigmoidal function; drier soil → higher CF.
 * Range: 0.5-5.0 MPa (fire occurs under pronounced drought stress).
 */
CARDADATA->parmin[P.psi50_fire]=0.5;
CARDADATA->parmax[P.psi50_fire]=5.0;

/*
 * fire_lgr: logistic growth rate of fire combustion response to soil drying.
 * Steeper = more threshold-like; shallower = more gradual.
 */
CARDADATA->parmin[P.fire_lgr]=0.5;
CARDADATA->parmax[P.fire_lgr]=10.0;

/*
 * vpd_fire_factor: VPD sensitivity of combustion factors (kPa-1).
 * CF_t = CF_max * f(psi) * (1 - exp(-vpd_fire_factor * VPD))
 * VPD in hPa in CARDAMOM drivers; converted to kPa in model (*0.1).
 * Range from Akagi et al. (2011) CE sensitivity to atmospheric drying.
 */
CARDADATA->parmin[P.vpd_fire_factor]=0.05;
CARDADATA->parmax[P.vpd_fire_factor]=5.0;

return 0;
}
