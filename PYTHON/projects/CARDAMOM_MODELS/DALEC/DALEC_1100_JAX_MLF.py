import jax
import jax.numpy as jnp

# ==============================================================================
# PHYSICAL AND CONVERSION CONSTANTS
# ==============================================================================
DGCM_TK0C = 273.15
DGCM_SEC_DAY = 86400.0
DGCM_LATENT_HEAT_VAPORIZATION = 2.501e6
DGCM_LATENT_HEAT_FUSION_3 = 0.334e6
DGCM_PI = 3.141592653589793

# ==============================================================================
# EXACT C-ALIGNED STATE POOL INDICES (0-29)
# ==============================================================================
S_C_lab = 0; S_C_fol = 1; S_C_roo = 2; S_C_woo = 3; S_C_cwd = 4; S_C_lit = 5; S_C_som = 6
S_H2O_LY1 = 7; S_H2O_LY2 = 8; S_H2O_LY3 = 9; S_H2O_SWE = 10
S_E_LY1 = 11; S_E_LY2 = 12; S_E_LY3 = 13
S_D_LAI = 14; S_D_SCF = 15; S_D_TEMP_LY1 = 16; S_D_TEMP_LY2 = 17; S_D_TEMP_LY3 = 18
S_D_LF_LY1 = 19; S_D_LF_LY2 = 20; S_D_LF_LY3 = 21
S_D_SM_LY1 = 22; S_D_SM_LY2 = 23; S_D_SM_LY3 = 24
S_D_PSI_LY1 = 25; S_D_PSI_LY2 = 26; S_D_PSI_LY3 = 27
S_M_LAI_MAX = 28; S_M_LAI_TEMP = 29

# ==============================================================================
# EXACT C-ALIGNED FLUX INDICES (0-99)
# ==============================================================================
F_gpp = 0; F_gppnet = 1; F_resp_auto = 2; F_lab_prod = 3; F_foliar_prod = 4; F_root_prod = 5; F_wood_prod = 6
F_dist_lab = 7; F_fol2lit = 8; F_woo2cwd = 9; F_roo2lit = 10; F_dist_roo = 11; F_cwd2som = 12; F_lit2som = 13
F_f_total = 14; F_f_lab = 15; F_f_fol = 16; F_f_roo = 17; F_f_woo = 18; F_f_cwd = 19; F_f_lit = 20; F_f_som = 21
F_fx_lab2lit = 22; F_fx_fol2lit = 23; F_fx_roo2lit = 24; F_fx_woo2cwd = 25; F_fx_cwd2som = 26; F_fx_lit2som = 27
F_q_ly1 = 28; F_q_ly2 = 29; F_ly1xly2 = 30; F_ly2xly3 = 31; F_q_ly3 = 32; F_q_surf = 33; F_infil = 34; F_infil_e = 35
F_q_ly1_e = 36; F_q_ly2_e = 37; F_q_ly3_e = 38; F_ly1xly2_e = 39; F_ly2xly3_e = 40; F_evap_e = 41; F_transp1_e = 42
F_transp2_e = 43; F_transp1 = 44; F_transp2 = 45; F_evap = 46; F_snowfall = 47; F_melt = 48
F_ae_rh_cwd = 49; F_ae_rh_lit = 50; F_ae_rh_som = 51; F_an_rh_cwd = 52; F_an_rh_lit = 53; F_an_rh_som = 54
F_rh_co2 = 55; F_rh_ch4 = 56; F_aetr = 57; F_antr = 58; F_an_co2_c_ratio = 59; F_an_ch4_c_ratio = 60
F_target_LAI = 61; F_dlambda_dt = 62; F_f_temp_thresh = 63; F_f_dayl_thresh = 64; F_lai_fire = 65; F_foliar_fire_frac = 66
F_net_radiation = 67; F_latent_heat = 68; F_sensible_heat = 69; F_ground_heat = 70; F_gh_in = 71
F_resp_auto_growth = 72; F_resp_auto_maint = 73; F_SWin = 74; F_SWout = 75; F_LWin = 76; F_LWout = 77
F_ly1xly2_th_e = 78; F_ly2xly3_th_e = 79; F_resp_auto_maint_dark = 80; F_sublimation = 81; F_geological = 82
F_ets = 83; F_beta_factor = 84; F_soil_beta_factor = 85
F_hydraulic_mortality_factor = 86; F_leaf_mortality_factor = 87; F_nonleaf_mortality_factor = 88
F_dist_fol = 89; F_dist_woo = 90; F_labyield2lit = 91; F_folyield2lit = 92; F_rooyield2lit = 93; F_wooyield2cwd = 94
F_lab2lit = 95; F_ph_fol2lit = 96; F_lambda_tilde_max = 97; F_lambda_W = 98; F_Rd = 99

# ==============================================================================
# HELPER FUNCTIONS (HYDROLOGY, SOIL ENERGY, AND CONVERTERS)
# ==============================================================================
def hydrofun_ewt2moi(ewt, porosity, depth):
    return ewt / (porosity * depth * 1000.0)

def hydrofun_moi2ewt(moi, porosity, depth):
    return moi * porosity * depth * 1000.0

def hydrofun_moi2psi(sm, psi_porosity, retention):
    return psi_porosity * jnp.power(jnp.maximum(sm, 1e-4), -retention)

def hydrofun_moi2con(sm, hydr_cond, retention):
    return hydr_cond * jnp.power(jnp.maximum(sm, 1e-4), 2.0 * retention + 3.0)

def drainage_fun(sm, q_excess, field_cap, psi_porosity, retention):
    s_fc = jnp.power(jnp.maximum(-field_cap / psi_porosity, 1e-4), -1.0 / retention)
    drain_rate = jnp.where(sm > s_fc, q_excess * (sm - s_fc), 0.0)
    return drain_rate

def soil_temp_and_liquid_frac(internal_energy, soil_water, dry_vhc, depth):
    c_water = 4184.0 # J/kg/K
    c_ice = 2090.0   # J/kg/K
    l_fusion = DGCM_LATENT_HEAT_FUSION_3 # J/kg
    
    mass_water = soil_water # mm = kg/m2
    heat_cap_dry = dry_vhc * depth # J/m2/K
    
    temp_unfrozen = 273.15 + internal_energy / (heat_cap_dry + mass_water * c_water)
    temp_frozen = 273.15 + (internal_energy + mass_water * l_fusion) / (heat_cap_dry + mass_water * c_ice)
    
    is_unfrozen = internal_energy >= 0.0
    is_frozen = internal_energy <= -mass_water * l_fusion
    
    temp = jnp.where(is_unfrozen, temp_unfrozen, jnp.where(is_frozen, temp_frozen, 273.15))
    
    latent_energy = -internal_energy
    fraction_frozen = jnp.clip(latent_energy / jnp.maximum(mass_water * l_fusion, 1e-6), 0.0, 1.0)
    liquid_fraction = jnp.where(is_unfrozen, 1.0, jnp.where(is_frozen, 0.0, 1.0 - fraction_frozen))
    
    return temp, liquid_fraction

def internal_energy_per_unit_mass(temp_k):
    c_water = 4184.0
    c_ice = 2090.0
    l_fusion = DGCM_LATENT_HEAT_FUSION_3
    
    temp_c = temp_k - 273.15
    energy = jnp.where(temp_c >= 0.0, c_water * temp_c, c_ice * temp_c - l_fusion)
    return energy

# ==============================================================================
# JAX STEP FUNCTION (Forward time-stepping)
# ==============================================================================
def dalec_1100_step(carry, step_inputs):
    states, params = carry
    forcings = step_inputs
    
    # Unpack Forcings
    ssrd, t2m_min, t2m_max, co2, prec, vpd, burned_area, snowfall, skt, strd, dist, yield_val, doy, lat = forcings
    
    deltat = 1.0
    one_over_deltat = 1.0 / deltat
    air_temp_k = DGCM_TK0C + 0.5 * (t2m_min + t2m_max)
    
    # Extract Parameter Vector (indices aligned with CARDAMOM standard)
    LCMA = params[13]
    t_wood = params[7]; t_root = params[8]; t_lit = params[9]; t_cwd = params[10]; t_som = params[11]; t_lab = params[23]
    cf_foliar = params[24]; cf_ligneous = params[25]; cf_DOM = params[26]; resilience = params[27]
    hydr_cond = params[28]; max_infil = params[29]
    LY1_por = params[31]; LY2_por = params[32]; LY3_por = params[33]; field_cap = params[34]
    LY1_z = params[35]; LY2_z = params[36]; LY3_z = params[37]
    LY1_vhc = params[38]; LY2_vhc = params[39]; LY3_vhc = params[40]
    Q_excess = params[41]; retention = params[21]
    
    psi_50 = params[42]; psi_50HMF = params[43]; root_frac = params[44]
    beta_lgr = params[45]; beta_lgrHMF = params[46]
    Tminmin = params[47]; Tminmax = params[48]
    Vcmax25 = params[49]; Med_g1 = params[50]; ga = params[51]
    Tupp = params[52]; Tdown = params[53]; clumping = params[54]
    leaf_refl_par = params[55]; leaf_refl_nir = params[56]
    q10canopy = params[57]; rauto_mrd_q10 = params[58]; canopyRdsf = params[59]; maxPevap = params[60]
    scf_scalar = params[61]; min_melt = params[62]; melt_slope = params[63]; sublimation_rate = params[64]
    thermal_cond_surf = params[65]; thermal_cond = params[66]
    T_phi = params[67]; T_range = params[68]; plgr = params[69]; k_leaf = params[70]; tau_W = params[71]; time_c = params[72]; time_r = params[73]; lambda_max = params[74]
    rauto_mr_r = params[2]; rauto_mr_w = params[3]; rauto_gr = params[6]; rauto_mr_q10 = params[4]
    phi_RL = params[75]; phi_WL = params[76]
    S_fv = params[77]; thetas_opt = params[78]; fwc = params[79]; r_ch4 = params[80]; Q10ch4 = params[81]; Q10rhco2 = params[12]
    tr_cwd2som = params[82]; tr_lit2som = params[83]; t_foliar = params[84]

    # Current States
    LAI = states[S_D_LAI]
    C_lab = states[S_C_lab]; C_fol = states[S_C_fol]; C_roo = states[S_C_roo]; C_woo = states[S_C_woo]
    C_cwd = states[S_C_cwd]; C_lit = states[S_C_lit]; C_som = states[S_C_som]
    H2O_LY1 = states[S_H2O_LY1]; H2O_LY2 = states[S_H2O_LY2]; H2O_LY3 = states[S_H2O_LY3]; H2O_SWE = states[S_H2O_SWE]
    E_LY1 = states[S_E_LY1]; E_LY2 = states[S_E_LY2]; E_LY3 = states[S_E_LY3]

    # Cold Stress Factor (g)
    g_stress = jnp.clip((t2m_min - (Tminmin - DGCM_TK0C)) / jnp.maximum(Tminmax - Tminmin, 1e-4), 0.0, 1.0)

    # Water Stress (Beta and HMF)
    psi_porosity = -0.117 / 100.0
    beta1 = 1.0 / (1.0 + jnp.exp(beta_lgr * (-1.0 * states[S_D_PSI_LY1] / jnp.maximum(psi_50, 1e-4) - 1.0))) * states[S_D_LF_LY1]
    beta2 = 1.0 / (1.0 + jnp.exp(beta_lgr * (-1.0 * states[S_D_PSI_LY2] / jnp.maximum(psi_50, 1e-4) - 1.0))) * states[S_D_LF_LY2]
    beta = (beta1 * LY1_z + beta2 * LY2_z * root_frac) / jnp.maximum(LY1_z + LY2_z * root_frac, 1e-4)

    betaHMF_1 = 1.0 / (1.0 + jnp.exp(beta_lgrHMF * (-1.0 * states[S_D_PSI_LY1] / jnp.maximum(psi_50HMF, 1e-4) - 1.0))) * states[S_D_LF_LY1]
    betaHMF_2 = 1.0 / (1.0 + jnp.exp(beta_lgrHMF * (-1.0 * states[S_D_PSI_LY2] / jnp.maximum(psi_50HMF, 1e-4) - 1.0))) * states[S_D_LF_LY2]
    betaHMF = (betaHMF_1 * LY1_z + betaHMF_2 * LY2_z * root_frac) / jnp.maximum(LY1_z + LY2_z * root_frac, 1e-4)

    HMF = jnp.where((states[S_D_LF_LY1] + states[S_D_LF_LY2]) == 2.0, 1.0 - betaHMF, 0.0)
    beta_factor = jnp.minimum(beta, g_stress)

    # Simplified Canopy Photosynthesis (LIU_AN_ET Proxy for JAX)
    pfd = ssrd * 1e6 / DGCM_SEC_DAY * 4.6 * 0.5
    vcmax_t = Vcmax25 * jnp.power(2.0, (air_temp_k - 298.15) / 10.0)
    Ag = jnp.maximum(0.0, vcmax_t * (1.0 - jnp.exp(-0.5 * LAI)) * beta_factor)
    Rd = Ag * canopyRdsf
    An = jnp.maximum(0.0, Ag - Rd)
    
    transp = An * 0.05 * (vpd / 10.0)
    evap = jnp.maximum(0.0, prec * 0.1 * (1.0 - states[S_D_SCF]))

    transp1 = jnp.where((beta1 > 0) | (beta2 > 0), transp * beta1 * LY1_z / jnp.maximum(beta1 * LY1_z + beta2 * LY2_z * root_frac, 1e-6), 0.0)
    transp2 = jnp.where((beta1 > 0) | (beta2 > 0), transp - transp1, 0.0)

    # Snow Accumulation & Melt
    H2O_SWE_temp = H2O_SWE + snowfall * deltat
    scf_temp = H2O_SWE_temp / (H2O_SWE_temp + scf_scalar)
    snowmelt = jnp.clip((skt + DGCM_TK0C - min_melt) * melt_slope, 0.0, 1.0) * H2O_SWE_temp * one_over_deltat
    sublimation = sublimation_rate * ssrd * scf_temp

    slf = (snowmelt + sublimation) * deltat / jnp.maximum(H2O_SWE_temp, 1e-6)
    melt_act = jnp.where(slf > 1.0, snowmelt / slf, snowmelt)
    sublimation_act = jnp.where(slf > 1.0, sublimation / slf, sublimation)
    H2O_SWE_next = jnp.maximum(0.0, H2O_SWE_temp - (melt_act + sublimation_act) * deltat)

    # Energy Balance
    SWin = ssrd * 1e6 / DGCM_SEC_DAY
    SWout_snowfree = SWin * 0.5 * (leaf_refl_par + leaf_refl_nir)
    SWout = (1.0 - states[S_D_SCF]) * SWout_snowfree + states[S_D_SCF] * (SWin * 0.9)
    LWin = strd * 1e6 / DGCM_SEC_DAY
    tskin_k = skt + DGCM_TK0C
    LWout = 5.67e-8 * jnp.power(tskin_k, 4.0)
    Rn = SWin - SWout + LWin - LWout

    LE = (DGCM_LATENT_HEAT_VAPORIZATION * (evap + transp1 + transp2) + (DGCM_LATENT_HEAT_FUSION_3 + DGCM_LATENT_HEAT_VAPORIZATION) * sublimation_act) / DGCM_SEC_DAY
    ground_heat = thermal_cond_surf * (tskin_k - states[S_D_TEMP_LY1]) / jnp.maximum(LY1_z * 0.5, 1e-4) * (1.0 - states[S_D_SCF])
    gh_in = ground_heat * DGCM_SEC_DAY
    sensible_heat = Rn - ground_heat - LE

    # Hydrology Routing
    liquid_in = prec - snowfall + melt_act
    infil = max_infil * (1.0 - jnp.exp(-liquid_in / jnp.maximum(max_infil, 1e-4)))
    q_surf = liquid_in - infil

    drain_LY1 = states[S_D_LF_LY1] * drainage_fun(states[S_D_SM_LY1], Q_excess, -field_cap, psi_porosity, retention)
    drain_LY2 = states[S_D_LF_LY2] * drainage_fun(states[S_D_SM_LY2], Q_excess, -field_cap, psi_porosity, retention)
    drain_LY3 = states[S_D_LF_LY3] * drainage_fun(states[S_D_SM_LY3], Q_excess, -field_cap, psi_porosity, retention)

    q_ly1 = hydrofun_moi2ewt(drain_LY1, LY1_por, LY1_z) * one_over_deltat
    q_ly2 = hydrofun_moi2ewt(drain_LY2, LY2_por, LY2_z) * one_over_deltat
    q_ly3 = hydrofun_moi2ewt(drain_LY3, LY3_por, LY3_z) * one_over_deltat

    # Inter-layer Percolation
    k_LY1 = hydrofun_moi2con(states[S_D_SM_LY1], hydr_cond, retention)
    k_LY2 = hydrofun_moi2con(states[S_D_SM_LY2], hydr_cond, retention)
    k_LY3 = hydrofun_moi2con(states[S_D_SM_LY3], hydr_cond, retention)

    pot_xfer_12 = 1000.0 * jnp.sqrt(jnp.maximum(k_LY1 * k_LY2, 1e-12)) * (1e-9 * (states[S_D_PSI_LY1] - states[S_D_PSI_LY2]) / (9.8 * 0.5 * (LY1_z + LY2_z)) + 1.0)
    
    space_avail_2 = jnp.maximum(LY2_z * LY2_por * 1e3 - H2O_LY2 + (q_ly2 + transp2) * deltat, 0.0)
    h2o_avail_1 = jnp.maximum(states[S_D_LF_LY1] * H2O_LY1 + (infil - q_ly1 - evap - transp1) * deltat, 0.0)
    max_xfer_12 = states[S_D_LF_LY1] * pot_xfer_12 * DGCM_SEC_DAY * deltat

    ly1xly2 = jnp.where(pot_xfer_12 > 0, jnp.minimum(max_xfer_12, jnp.minimum(space_avail_2, h2o_avail_1)) * one_over_deltat,
                        -jnp.minimum(-max_xfer_12, jnp.minimum(space_avail_2, h2o_avail_1)) * one_over_deltat)
    
    ly2xly3 = 0.0 # Standard simplified flux

    # Pool Water Updates
    H2O_LY1_next = jnp.maximum(0.0, H2O_LY1 + (infil - ly1xly2 - q_ly1 - evap - transp1) * deltat)
    H2O_LY2_next = jnp.maximum(0.0, H2O_LY2 + (ly1xly2 - ly2xly3 - q_ly2 - transp2) * deltat)
    H2O_LY3_next = jnp.maximum(0.0, H2O_LY3 + (ly2xly3 - q_ly3) * deltat)

    # Phenology and Allocation
    target_LAI = jnp.clip(LAI + 0.01 * (air_temp_k - 273.15) / 10.0, 0.1, lambda_max)
    dlambda_dt = (target_LAI - LAI) * one_over_deltat
    ph_fol2lit = jnp.where(dlambda_dt < 0.0, -dlambda_dt * LCMA, 0.0)

    alloc_fol = jnp.maximum(0.0, (target_LAI * LCMA - C_fol) * one_over_deltat)
    alloc_roo = alloc_fol * phi_RL
    alloc_woo = alloc_fol * phi_WL
    lab_prod = An

    resp_auto_growth = (alloc_fol + alloc_roo + alloc_woo) * rauto_gr
    resp_auto_maint = (C_roo * rauto_mr_r + C_woo * rauto_mr_w) * jnp.power(rauto_mr_q10, (air_temp_k - 298.15) / 10.0)
    resp_auto = resp_auto_growth + resp_auto_maint + Rd

    # Heterotrophic Respiration (JCR)
    ae_tr = jnp.power(Q10rhco2, (states[S_D_TEMP_LY1] - 298.15) / 10.0) * states[S_D_LF_LY1]
    ae_rh_cwd = C_cwd * ae_tr * t_cwd * (1.0 - tr_cwd2som)
    ae_rh_lit = C_lit * ae_tr * t_lit * (1.0 - tr_lit2som)
    ae_rh_som = C_som * ae_tr * t_som
    rh_co2 = ae_rh_cwd + ae_rh_lit + ae_rh_som
    rh_ch4 = 0.0

    cwd2som = C_cwd * ae_tr * t_cwd * tr_cwd2som
    lit2som = C_lit * ae_tr * t_lit * tr_lit2som

    # Carbon Pool Updates
    C_lab_next = jnp.maximum(0.0, C_lab + (Ag - Rd - resp_auto_maint - alloc_fol - alloc_roo - alloc_woo - resp_auto_growth) * deltat)
    C_fol_next = jnp.maximum(0.0, C_fol + (alloc_fol - ph_fol2lit) * deltat)
    C_roo_next = jnp.maximum(0.0, C_roo + alloc_roo * deltat)
    C_woo_next = jnp.maximum(0.0, C_woo + alloc_woo * deltat)

    woo2cwd = C_woo_next * t_wood
    roo2lit = C_roo_next * t_root
    fol2lit = C_fol_next * t_foliar
    lab2lit = C_lab_next * t_lab

    C_lab_next -= lab2lit * deltat
    C_fol_next -= fol2lit * deltat
    C_roo_next -= roo2lit * deltat
    C_woo_next -= woo2cwd * deltat

    C_cwd_next = jnp.maximum(0.0, C_cwd + (woo2cwd - ae_rh_cwd - cwd2som) * deltat)
    C_lit_next = jnp.maximum(0.0, C_lit + (ph_fol2lit + fol2lit + roo2lit + lab2lit - ae_rh_lit - lit2som) * deltat)
    C_som_next = jnp.maximum(0.0, C_som + (lit2som + cwd2som - ae_rh_som) * deltat)

    # Energy Pool Updates
    infil_e = infil * internal_energy_per_unit_mass(air_temp_k)
    evap_e = evap * internal_energy_per_unit_mass(states[S_D_TEMP_LY1])
    transp1_e = transp1 * internal_energy_per_unit_mass(states[S_D_TEMP_LY1])
    transp2_e = transp2 * internal_energy_per_unit_mass(states[S_D_TEMP_LY2])
    q_ly1_e = q_ly1 * internal_energy_per_unit_mass(states[S_D_TEMP_LY1])

    E_LY1_next = E_LY1 + (gh_in + infil_e - evap_e - transp1_e - q_ly1_e) * deltat
    E_LY2_next = E_LY2
    E_LY3_next = E_LY3

    # Diagnostic States for t+1
    temp_ly1_next, lf_ly1_next = soil_temp_and_liquid_frac(E_LY1_next, H2O_LY1_next, LY1_vhc, LY1_z)
    temp_ly2_next, lf_ly2_next = soil_temp_and_liquid_frac(E_LY2_next, H2O_LY2_next, LY2_vhc, LY2_z)
    temp_ly3_next, lf_ly3_next = soil_temp_and_liquid_frac(E_LY3_next, H2O_LY3_next, LY3_vhc, LY3_z)

    sm_ly1_next = hydrofun_ewt2moi(H2O_LY1_next, LY1_por, LY1_z)
    sm_ly2_next = hydrofun_ewt2moi(H2O_LY2_next, LY2_por, LY2_z)
    sm_ly3_next = hydrofun_ewt2moi(H2O_LY3_next, LY3_por, LY3_z)

    psi_ly1_next = jnp.maximum(-30.0, hydrofun_moi2psi(sm_ly1_next, psi_porosity, retention))
    psi_ly2_next = jnp.maximum(-30.0, hydrofun_moi2psi(sm_ly2_next, psi_porosity, retention))
    psi_ly3_next = jnp.maximum(-30.0, hydrofun_moi2psi(sm_ly3_next, psi_porosity, retention))

    lai_next = C_fol_next / LCMA
    scf_next = jnp.where(H2O_SWE_next > 0, H2O_SWE_next / (H2O_SWE_next + scf_scalar), 0.0)

    # -------------------------------------------------------------------------
    # FLUX VECTOR PACKING (Strict Absolute Indexing)
    # -------------------------------------------------------------------------
    fluxes = jnp.zeros(100)
    fluxes = fluxes.at[F_gpp].set(Ag)
    fluxes = fluxes.at[F_gppnet].set(An)
    fluxes = fluxes.at[F_resp_auto].set(resp_auto)
    fluxes = fluxes.at[F_lab_prod].set(lab_prod)
    fluxes = fluxes.at[F_foliar_prod].set(alloc_fol)
    fluxes = fluxes.at[F_root_prod].set(alloc_roo)
    fluxes = fluxes.at[F_wood_prod].set(alloc_woo)

    fluxes = fluxes.at[F_transp1].set(transp1)
    fluxes = fluxes.at[F_transp2].set(transp2)
    fluxes = fluxes.at[F_evap].set(evap)
    fluxes = fluxes.at[F_snowfall].set(snowfall)
    fluxes = fluxes.at[F_melt].set(melt_act)
    fluxes = fluxes.at[F_sublimation].set(sublimation_act)
    fluxes = fluxes.at[F_ets].set(evap + transp1 + transp2 + sublimation_act)

    fluxes = fluxes.at[F_infil].set(infil)
    fluxes = fluxes.at[F_q_surf].set(q_surf)
    fluxes = fluxes.at[F_q_ly1].set(q_ly1)
    fluxes = fluxes.at[F_q_ly2].set(q_ly2)
    fluxes = fluxes.at[F_q_ly3].set(q_ly3)
    fluxes = fluxes.at[F_ly1xly2].set(ly1xly2)

    fluxes = fluxes.at[F_rh_co2].set(rh_co2)
    fluxes = fluxes.at[F_ae_rh_cwd].set(ae_rh_cwd)
    fluxes = fluxes.at[F_ae_rh_lit].set(ae_rh_lit)
    fluxes = fluxes.at[F_ae_rh_som].set(ae_rh_som)

    fluxes = fluxes.at[F_net_radiation].set(Rn)
    fluxes = fluxes.at[F_latent_heat].set(LE)
    fluxes = fluxes.at[F_sensible_heat].set(sensible_heat)
    fluxes = fluxes.at[F_ground_heat].set(ground_heat)
    fluxes = fluxes.at[F_gh_in].set(gh_in)

    # -------------------------------------------------------------------------
    # STATE VECTOR UPDATES (Strict Absolute Indexing)
    # -------------------------------------------------------------------------
    states_next = jnp.zeros(30)
    states_next = states_next.at[S_C_lab].set(C_lab_next)
    states_next = states_next.at[S_C_fol].set(C_fol_next)
    states_next = states_next.at[S_C_roo].set(C_roo_next)
    states_next = states_next.at[S_C_woo].set(C_woo_next)
    states_next = states_next.at[S_C_cwd].set(C_cwd_next)
    states_next = states_next.at[S_C_lit].set(C_lit_next)
    states_next = states_next.at[S_C_som].set(C_som_next)

    states_next = states_next.at[S_H2O_LY1].set(H2O_LY1_next)
    states_next = states_next.at[S_H2O_LY2].set(H2O_LY2_next)
    states_next = states_next.at[S_H2O_LY3].set(H2O_LY3_next)
    states_next = states_next.at[S_H2O_SWE].set(H2O_SWE_next)

    states_next = states_next.at[S_E_LY1].set(E_LY1_next)
    states_next = states_next.at[S_E_LY2].set(E_LY2_next)
    states_next = states_next.at[S_E_LY3].set(E_LY3_next)

    states_next = states_next.at[S_D_LAI].set(lai_next)
    states_next = states_next.at[S_D_SCF].set(scf_next)
    states_next = states_next.at[S_D_TEMP_LY1].set(temp_ly1_next)
    states_next = states_next.at[S_D_TEMP_LY2].set(temp_ly2_next)
    states_next = states_next.at[S_D_TEMP_LY3].set(temp_ly3_next)

    states_next = states_next.at[S_D_LF_LY1].set(lf_ly1_next)
    states_next = states_next.at[S_D_LF_LY2].set(lf_ly2_next)
    states_next = states_next.at[S_D_LF_LY3].set(lf_ly3_next)

    states_next = states_next.at[S_D_SM_LY1].set(sm_ly1_next)
    states_next = states_next.at[S_D_SM_LY2].set(sm_ly2_next)
    states_next = states_next.at[S_D_SM_LY3].set(sm_ly3_next)

    states_next = states_next.at[S_D_PSI_LY1].set(psi_ly1_next)
    states_next = states_next.at[S_D_PSI_LY2].set(psi_ly2_next)
    states_next = states_next.at[S_D_PSI_LY3].set(psi_ly3_next)

    states_next = states_next.at[S_M_LAI_MAX].set(states[S_M_LAI_MAX])
    states_next = states_next.at[S_M_LAI_TEMP].set(states[S_M_LAI_TEMP])

    return (states_next, params), (states_next, fluxes)

# ==============================================================================
# MODEL LIKELIHOOD FUNCTION (MLF)
# ==============================================================================
@jax.jit
def DALEC_1100_JAX_MLF(params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std):
    initial_carry = (initial_state, params)
    _, (states_out, fluxes_out) = jax.lax.scan(dalec_1100_step, initial_carry, forcings)

    prior_cost = jnp.sum(((params - prior_mean) / prior_std) ** 2) * 0.5

    obs_cost = 0.0
    if 'GPP' in obs_dict:
        pred_gpp = fluxes_out[:, F_gpp]
        obs_gpp = obs_dict['GPP']
        unc_gpp = obs_unc.get('GPP', 1.0)
        valid_mask = (obs_gpp != -9999.0)
        cost = jnp.where(valid_mask, ((pred_gpp - obs_gpp) / unc_gpp) ** 2, 0.0)
        obs_cost += jnp.sum(cost) * 0.5

    total_negative_log_likelihood = prior_cost + obs_cost
    return total_negative_log_likelihood, (states_out, fluxes_out)
