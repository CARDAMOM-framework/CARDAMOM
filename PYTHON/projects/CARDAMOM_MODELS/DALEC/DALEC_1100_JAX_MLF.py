import jax
import jax.numpy as jnp

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

# ==============================================================================
# JAX STEP FUNCTION (Forward time-stepping)
# ==============================================================================
def dalec_1100_step(carry, step_inputs):
    """
    Executes a single daily timestep of the DALEC 1100 model.
    """
    states, params = carry
    forcings = step_inputs
    
    # 1. Unpack Forcings (Order matches CARDAMOM_READ_NETCDF_DATA)
    # [SSRD, T2M_MIN, T2M_MAX, CO2, PREC, VPD, BURNED_AREA, SNOWFALL, SKT, STRD, DIST, YIELD, DOY, LAT]
    ssrd, t2m_min, t2m_max, co2, prec, vpd, burned_area, snowfall, skt, strd, dist, yield_val, doy, lat = forcings
    
    # -------------------------------------------------------------------------
    # INSERT JAX PHYSICS EQUATIONS HERE
    # (LIU_AN_ET, KNORR, Hydrology, Respiration, Allocation, etc.)
    # -------------------------------------------------------------------------
    # Example placeholder calculations (replace with your full JAX physics block):
    GPP = 0.0  # Replace with LIU.OUT.Ag
    GPPnet = 0.0 # Replace with LIU.OUT.An
    resp_auto = 0.0 
    lab_prod = 0.0; foliar_prod = 0.0; root_prod = 0.0; wood_prod = 0.0
    transp1 = 0.0; transp2 = 0.0; evap = 0.0; melt = 0.0; sublimation = 0.0
    ets = 0.0; infil = 0.0; q_surf = 0.0; q_ly1 = 0.0; q_ly2 = 0.0; q_ly3 = 0.0
    ly1xly2 = 0.0; ly2xly3 = 0.0
    
    # Placeholder state updates
    C_lab_next = states[S_C_lab]
    C_fol_next = states[S_C_fol]
    C_roo_next = states[S_C_roo]
    C_woo_next = states[S_C_woo]
    C_cwd_next = states[S_C_cwd]
    C_lit_next = states[S_C_lit]
    C_som_next = states[S_C_som]
    H2O_LY1_next = states[S_H2O_LY1]
    H2O_LY2_next = states[S_H2O_LY2]
    H2O_LY3_next = states[S_H2O_LY3]
    H2O_SWE_next = states[S_H2O_SWE]
    
    # -------------------------------------------------------------------------
    # FLUX VECTOR PACKING (Strict absolute indexing)
    # -------------------------------------------------------------------------
    fluxes = jnp.zeros(100)
    
    # Primary Carbon & Respiration
    fluxes = fluxes.at[F_gpp].set(GPP)
    fluxes = fluxes.at[F_gppnet].set(GPPnet)
    fluxes = fluxes.at[F_resp_auto].set(resp_auto)
    fluxes = fluxes.at[F_lab_prod].set(lab_prod)
    fluxes = fluxes.at[F_foliar_prod].set(foliar_prod)
    fluxes = fluxes.at[F_root_prod].set(root_prod)
    fluxes = fluxes.at[F_wood_prod].set(wood_prod)
    
    # Water & Snow
    fluxes = fluxes.at[F_transp1].set(transp1)
    fluxes = fluxes.at[F_transp2].set(transp2)
    fluxes = fluxes.at[F_evap].set(evap)
    fluxes = fluxes.at[F_snowfall].set(snowfall)
    fluxes = fluxes.at[F_melt].set(melt)
    fluxes = fluxes.at[F_sublimation].set(sublimation)
    fluxes = fluxes.at[F_ets].set(ets)
    
    # Hydrology Routing
    fluxes = fluxes.at[F_infil].set(infil)
    fluxes = fluxes.at[F_q_surf].set(q_surf)
    fluxes = fluxes.at[F_q_ly1].set(q_ly1)
    fluxes = fluxes.at[F_q_ly2].set(q_ly2)
    fluxes = fluxes.at[F_q_ly3].set(q_ly3)
    fluxes = fluxes.at[F_ly1xly2].set(ly1xly2)
    fluxes = fluxes.at[F_ly2xly3].set(ly2xly3)
    
    # Note: Add remaining calculated fluxes mapped to their specific F_ indices
    
    # -------------------------------------------------------------------------
    # STATE VECTOR UPDATES (Strict absolute indexing)
    # -------------------------------------------------------------------------
    states_next = jnp.zeros(30)
    
    # Biological Pools
    states_next = states_next.at[S_C_lab].set(C_lab_next)
    states_next = states_next.at[S_C_fol].set(C_fol_next)
    states_next = states_next.at[S_C_roo].set(C_roo_next)
    states_next = states_next.at[S_C_woo].set(C_woo_next)
    states_next = states_next.at[S_C_cwd].set(C_cwd_next)
    states_next = states_next.at[S_C_lit].set(C_lit_next)
    states_next = states_next.at[S_C_som].set(C_som_next)
    
    # Hydrology Pools
    states_next = states_next.at[S_H2O_LY1].set(H2O_LY1_next)
    states_next = states_next.at[S_H2O_LY2].set(H2O_LY2_next)
    states_next = states_next.at[S_H2O_LY3].set(H2O_LY3_next)
    states_next = states_next.at[S_H2O_SWE].set(H2O_SWE_next)
    
    # Update Energy, LAI, SCF and Diagnostic states (Add these below)
    # states_next = states_next.at[S_D_SM_LY1].set(...)
    
    return (states_next, params), (states_next, fluxes)

# ==============================================================================
# MLF (MODEL LIKELIHOOD FUNCTION) WRAPPER
# ==============================================================================
@jax.jit
def DALEC_1100_JAX_MLF(params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std):
    """
    Runs the DALEC 1100 model over the full forcing timeseries and computes 
    the Negative Log-Likelihood of the observations + prior.
    """
    # 1. Forward run via lax.scan
    initial_carry = (initial_state, params)
    _, (states_out, fluxes_out) = jax.lax.scan(dalec_1100_step, initial_carry, forcings)
    
    # 2. Prior Cost Calculation
    prior_cost = jnp.sum(((params - prior_mean) / prior_std) ** 2) * 0.5
    
    # 3. Observation Cost Calculation
    obs_cost = 0.0
    
    if 'GPP' in obs_dict:
        # Extract predicted GPP from the exact index (F_gpp = 0)
        pred_gpp = fluxes_out[:, F_gpp]
        obs_gpp = obs_dict['GPP']
        unc_gpp = obs_unc.get('GPP', 1.0)
        
        # Mask out missing obs (-9999.0)
        valid_mask = (obs_gpp != -9999.0)
        cost = jnp.where(valid_mask, ((pred_gpp - obs_gpp) / unc_gpp) ** 2, 0.0)
        obs_cost += jnp.sum(cost) * 0.5
        
    # (Add additional obs operators for LAI, ABGB, etc. mapping to appropriate indices)
        
    total_negative_log_likelihood = prior_cost + obs_cost
    
    return total_negative_log_likelihood, (states_out, fluxes_out)
