import jax.numpy as jnp

# =====================================================================
# INDICES MAP (Matching DALEC_1100_INDICES.c & PARS_INFO_1100.c)
# =====================================================================
# State Pools
S_C_lab, S_C_fol, S_C_roo, S_C_woo, S_C_cwd, S_C_lit, S_C_som = 0, 1, 2, 3, 4, 5, 6
S_H2O_LY1, S_H2O_LY2, S_H2O_LY3, S_H2O_SWE = 7, 8, 9, 10
S_D_LAI = 14
S_D_SCF = 15

# Fluxes
F_gpp = 0
F_resp_auto = 2
F_f_total = 14
F_q_ly1, F_q_ly2, F_q_ly3, F_q_surf = 28, 29, 32, 33
F_rh_co2 = 52
F_rh_ch4 = 53
F_latent_heat = 62
F_sensible_heat = 63
F_ets = 73

# Parameters
P_LCMA = 13
P_iniSOM = 20
P_Vcmax25 = 36
P_clumping = 46
P_iniSnow = 49
P_S_fv = 53
P_r_ch4 = 56

def DALEC_OBSERVATION_OPERATORS(states_trajectory, fluxes_trajectory, params):
    """
    Differentiable JAX translation of DALEC_OBSERVATION_OPERATORS.c
    
    Inputs:
        states_trajectory: jnp.array of shape (Time, N_Pools)
        fluxes_trajectory: jnp.array of shape (Time, N_Fluxes)
        params: jnp.array of shape (N_Params,)
        
    Returns:
        obs_preds: Dictionary of all predicted observable quantities.
    """
    obs_preds = {}
    
    # -------------------------------------------------------------------------
    # 1. POOL-BASED OBSERVABLES (Biomass, DOM, Hydrology)
    # -------------------------------------------------------------------------
    # Above Ground Biomass (ABGB)
    obs_preds['ABGB'] = (
        states_trajectory[:, S_C_lab] + 
        states_trajectory[:, S_C_fol] + 
        states_trajectory[:, S_C_roo] + 
        states_trajectory[:, S_C_woo]
    )
    
    # Woody Biomass
    obs_preds['CWOO'] = states_trajectory[:, S_C_woo]
    
    # Dead Organic Matter (DOM)
    obs_preds['DOM'] = (
        states_trajectory[:, S_C_cwd] + 
        states_trajectory[:, S_C_lit] + 
        states_trajectory[:, S_C_som]
    )
    
    # Diagnostic States
    obs_preds['LAI'] = states_trajectory[:, S_D_LAI]
    obs_preds['SCF'] = states_trajectory[:, S_D_SCF]
    obs_preds['SWE'] = states_trajectory[:, S_H2O_SWE]
    
    # Equivalent Water Thickness (GRACE H2O)
    obs_preds['EWT'] = (
        states_trajectory[:, S_H2O_LY1] + 
        states_trajectory[:, S_H2O_LY2] + 
        states_trajectory[:, S_H2O_LY3] + 
        states_trajectory[:, S_H2O_SWE]
    )

    # -------------------------------------------------------------------------
    # 2. FLUX-BASED OBSERVABLES
    # -------------------------------------------------------------------------
    obs_preds['GPP'] = fluxes_trajectory[:, F_gpp]
    obs_preds['SIF'] = fluxes_trajectory[:, F_gpp]  # SIF mapped directly to GPP per C code
    obs_preds['ET']  = fluxes_trajectory[:, F_ets]
    obs_preds['LE']  = fluxes_trajectory[:, F_latent_heat]
    obs_preds['H']   = fluxes_trajectory[:, F_sensible_heat]
    obs_preds['CH4'] = fluxes_trajectory[:, F_rh_ch4]
    obs_preds['FIR'] = fluxes_trajectory[:, F_f_total]
    
    # Runoff (ROFF) = Subsurface layers + Surface runoff
    obs_preds['ROFF'] = (
        fluxes_trajectory[:, F_q_ly1] + 
        fluxes_trajectory[:, F_q_ly2] + 
        fluxes_trajectory[:, F_q_ly3] + 
        fluxes_trajectory[:, F_q_surf]
    )
    
    # Net Biome Exchange (NBE) = GPP (in) - AutoResp (out) - HetResp (out) - Fire (out)
    # Signs adjusted to match NBE_flux_signs = {-1, 1, 1, 1} from C code
    obs_preds['NBE'] = (
        fluxes_trajectory[:, F_resp_auto] + 
        fluxes_trajectory[:, F_rh_co2] + 
        fluxes_trajectory[:, F_f_total] - 
        fluxes_trajectory[:, F_gpp]
    )

    # -------------------------------------------------------------------------
    # 3. EMERGENT QUANTITIES (Time-Averaged)
    # -------------------------------------------------------------------------
    mean_GPP = jnp.mean(fluxes_trajectory[:, F_gpp])
    mean_Rauto = jnp.mean(fluxes_trajectory[:, F_resp_auto])
    mean_Rhet = jnp.mean(fluxes_trajectory[:, F_rh_co2])
    mean_Fire = jnp.mean(fluxes_trajectory[:, F_f_total])
    
    # CUEmrg = 1 - (Rauto / GPP)
    obs_preds['PEQ_CUEmrg'] = 1.0 - (mean_Rauto / (mean_GPP + 1e-9))
    
    # NBEmrg = GPP / (Reco + Fire)
    obs_preds['PEQ_NBEmrg'] = mean_GPP / (mean_Rhet + mean_Rauto + mean_Fire + 1e-9)

    # -------------------------------------------------------------------------
    # 4. PARAMETER OBSERVABLES
    # -------------------------------------------------------------------------
    obs_preds['PEQ_Vcmax25'] = params[P_Vcmax25]
    obs_preds['PEQ_iniSnow'] = params[P_iniSnow]
    obs_preds['PEQ_iniSOM']  = params[P_iniSOM]
    obs_preds['PEQ_LCMA']    = params[P_LCMA]
    obs_preds['PEQ_clumping'] = params[P_clumping]
    obs_preds['PEQ_r_ch4']   = params[P_r_ch4]
    obs_preds['PEQ_S_fv']    = params[P_S_fv]
    
    # CH4 to CO2 Respiration Ratio
    mean_rh_ch4 = jnp.mean(fluxes_trajectory[:, F_rh_ch4])
    obs_preds['PEQ_rhch4_rhco2'] = mean_rh_ch4 / (mean_Rhet + 1e-9)

    return obs_preds
