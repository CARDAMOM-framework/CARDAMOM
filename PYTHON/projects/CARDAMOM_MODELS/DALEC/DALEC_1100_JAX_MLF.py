import jax
import jax.numpy as jnp

# =====================================================================
# IMPORTS FROM SUBMODULES
# =====================================================================
# Core DALEC_ALL Physics Modules
from DALEC_ALL.LIU_AN_ET_REFACTOR import LIU_AN_ET
from DALEC_ALL.HET_RESP_RATES_JCR import HET_RESP_RATES_JCR
from DALEC_ALL.ALLOC_AND_AUTO_RESP_FLUXES import ALLOC_AND_AUTO_RESP_FLUXES
from DALEC_ALL.KNORR_ALLOCATION import KNORR_ALLOCATION
from DALEC_ALL.SOIL_TEMP_AND_LIQUID_FRAC import SOIL_TEMP_AND_LIQUID_FRAC
from DALEC_ALL.INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS import INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS

# Observation Operators
from DALEC_OBSERVATION_OPERATORS.DALEC_OBSERVATION_OPERATORS import DALEC_OBSERVATION_OPERATORS

# Hydrology Modules
from DALEC_ALL.HYDROLOGY_MODULES.DRAINAGE import DRAINAGE
from DALEC_ALL.HYDROLOGY_MODULES.CONVERTERS.HYDROFUN_MOI2EWT import HYDROFUN_MOI2EWT
from DALEC_ALL.HYDROLOGY_MODULES.CONVERTERS.HYDROFUN_MOI2CON import HYDROFUN_MOI2CON
from DALEC_ALL.HYDROLOGY_MODULES.CONVERTERS.HYDROFUN_MOI2PSI import HYDROFUN_MOI2PSI
from DALEC_ALL.HYDROLOGY_MODULES.CONVERTERS.HYDROFUN_EWT2MOI import HYDROFUN_EWT2MOI


# =====================================================================
# 1. INDICES (Map exactly to C Structs)
# =====================================================================
# States (30 Pools)
S_C_lab, S_C_fol, S_C_roo, S_C_woo, S_C_cwd, S_C_lit, S_C_som = 0, 1, 2, 3, 4, 5, 6
S_H2O_LY1, S_H2O_LY2, S_H2O_LY3, S_H2O_SWE = 7, 8, 9, 10
S_E_LY1, S_E_LY2, S_E_LY3 = 11, 12, 13
S_D_LAI, S_D_SCF = 14, 15
S_D_TEMP_LY1, S_D_TEMP_LY2, S_D_TEMP_LY3 = 16, 17, 18
S_D_LF_LY1, S_D_LF_LY2, S_D_LF_LY3 = 19, 20, 21
S_D_SM_LY1, S_D_SM_LY2, S_D_SM_LY3 = 22, 23, 24
S_D_PSI_LY1, S_D_PSI_LY2, S_D_PSI_LY3 = 25, 26, 27
S_M_LAI_MAX, S_M_LAI_TEMP = 28, 29

# Fluxes (100 Fluxes)
F_gpp, F_gppnet, F_resp_auto = 0, 1, 2
F_ets, F_latent_heat, F_sensible_heat, F_ground_heat, F_gh_in = 73, 62, 63, 64, 65
F_melt, F_sublimation, F_snowfall = 45, 71, 44
F_q_surf, F_infil, F_q_ly1, F_q_ly2, F_q_ly3 = 33, 34, 28, 29, 32
F_ly1xly2, F_ly2xly3 = 30, 31
F_infil_e, F_evap_e, F_transp1_e, F_transp2_e = 35, 41, 42, 43
F_q_ly1_e, F_q_ly2_e, F_q_ly3_e = 36, 37, 38
F_ly1xly2_e, F_ly2xly3_e, F_ly1xly2_th_e, F_ly2xly3_th_e = 39, 40, 69, 70
F_geological = 72

# Constants
DGCM_TK0C = 273.15
DGCM_SEC_DAY = 86400.0


# =====================================================================
# 2. THE MAIN STEP FUNCTION (Replaces C 'for' loop)
# =====================================================================
def dalec_1100_step(state, step_inputs, pars):
    # Unpack Forcings
    # Expected order: [SSRD, T2M_MIN, T2M_MAX, CO2, PREC, VPD, BURNED_AREA, SNOWFALL, SKT, STRD, DIST, YIELD, DOY, LAT]
    SSRD, T2M_MIN, T2M_MAX, CO2, PREC, VPD, BURNED_AREA, SNOWFALL, SKT, STRD, DIST, YIELD, DOY, LAT = step_inputs
    deltat = 1.0 
    air_temp_k = DGCM_TK0C + 0.5 * (T2M_MIN + T2M_MAX)

    # Initialize state update and flux vectors
    state_next = state
    fluxes = jnp.zeros(100)

    # --- 1. Water & Temp Stress (Beta Factors) ---
    beta1 = 1.0 / (1.0 + jnp.exp(pars[84] * (-1.0 * state[S_D_PSI_LY1] / pars[83] - 1.0))) * state[S_D_LF_LY1]
    beta2 = 1.0 / (1.0 + jnp.exp(pars[84] * (-1.0 * state[S_D_PSI_LY2] / pars[83] - 1.0))) * state[S_D_LF_LY2]
    beta = (beta1 * pars[35] + beta2 * pars[36] * pars[96]) / (pars[35] + pars[36] * pars[96] + 1e-9)
    
    Tminmin_k = pars[43] - DGCM_TK0C
    Tminmax_k = pars[44] - DGCM_TK0C
    g = jnp.clip((T2M_MIN - Tminmin_k) / (Tminmax_k - Tminmin_k + 1e-9), 0.0, 1.0)
    beta_factor = jnp.minimum(beta, g)

    # --- 2. Photosynthesis (LIU_AN_ET) ---
    An, Ag, Rd, transp, evap, leaf_mort = LIU_AN_ET(
        SSRD, VPD, air_temp_k, pars[42], CO2, beta_factor, pars[41], state[S_D_LAI], 
        pars[45], 0.5, pars[46], pars[47], pars[48], pars[49], pars[50], 
        pars[99], PREC, pars[93], pars[94], pars[95], state[S_C_lab], deltat
    )
    
    # Split transpiration across layers
    transp_denom = beta1 * pars[35] + beta2 * pars[36] * pars[96] + 1e-9
    transp1 = jnp.where((beta1 > 0) | (beta2 > 0), transp * beta1 * pars[35] / transp_denom, 0.0)
    transp2 = transp - transp1

    # --- 3. Snow Dynamics ---
    fluxes = fluxes.at[F_snowfall].set(SNOWFALL)
    H2O_SWE_int = state[S_H2O_SWE] + SNOWFALL * deltat
    SCFtemp = H2O_SWE_int / (H2O_SWE_int + pars[52] + 1e-9)
    
    snowmelt_pot = jnp.clip((DGCM_TK0C + SKT - pars[50]) * pars[51], 0.0, 1.0) * H2O_SWE_int / deltat
    sublimation_pot = pars[95] * SSRD * SCFtemp
    
    slf = (snowmelt_pot + sublimation_pot) * deltat / (H2O_SWE_int + 1e-9)
    snowmelt = jnp.where(slf > 1.0, snowmelt_pot / slf, snowmelt_pot)
    sublimation = jnp.where(slf > 1.0, sublimation_pot / slf, sublimation_pot)
    
    fluxes = fluxes.at[F_melt].set(snowmelt)
    fluxes = fluxes.at[F_sublimation].set(sublimation)
    state_next = state_next.at[S_H2O_SWE].set(jnp.maximum(H2O_SWE_int - (snowmelt + sublimation) * deltat, 0.0))

    # --- 4. Hydrology Routing ---
    liquid_in = PREC - SNOWFALL + snowmelt
    infil = pars[29] * (1.0 - jnp.exp(-liquid_in / (pars[29] + 1e-9)))
    q_surf = liquid_in - infil
    
    # Drainage
    psi_porosity = -0.117 / 100.0
    drain_LY1 = state[S_D_LF_LY1] * DRAINAGE(state[S_D_SM_LY1], pars[40], -pars[34], psi_porosity, pars[21])
    drain_LY2 = state[S_D_LF_LY2] * DRAINAGE(state[S_D_SM_LY2], pars[40], -pars[34], psi_porosity, pars[21])
    drain_LY3 = state[S_D_LF_LY3] * DRAINAGE(state[S_D_SM_LY3], pars[40], -pars[34], psi_porosity, pars[21])
    
    q_ly1 = HYDROFUN_MOI2EWT(drain_LY1, pars[31], pars[35]) / deltat
    q_ly2 = HYDROFUN_MOI2EWT(drain_LY2, pars[32], pars[36]) / deltat
    q_ly3 = HYDROFUN_MOI2EWT(drain_LY3, pars[33], pars[37]) / deltat

    # Update H2O pools
    state_next = state_next.at[S_H2O_LY1].set(state[S_H2O_LY1] + (infil - q_ly1 - evap - transp1) * deltat)
    state_next = state_next.at[S_H2O_LY2].set(state[S_H2O_LY2] + (- q_ly2 - transp2) * deltat)
    state_next = state_next.at[S_H2O_LY3].set(state[S_H2O_LY3] + (- q_ly3) * deltat)

    # --- 5. Thermal & Energy Updates ---
    infiltemp = jnp.where(snowmelt > 0, ((air_temp_k - DGCM_TK0C) * (PREC - SNOWFALL) / (PREC - SNOWFALL + snowmelt + 1e-9)) + DGCM_TK0C, air_temp_k)
    
    infil_e = infil * INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(infiltemp)
    evap_e = evap * INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(state[S_D_TEMP_LY1])
    q_ly1_e = q_ly1 * INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(state[S_D_TEMP_LY1])
    
    state_next = state_next.at[S_E_LY1].set(state[S_E_LY1] + (infil_e - evap_e - q_ly1_e) * deltat)
    
    # Soil Temp and Liquid Fraction Diagnostics
    temp1, lf1 = SOIL_TEMP_AND_LIQUID_FRAC(pars[38], pars[35], state_next[S_H2O_LY1], state_next[S_E_LY1])
    state_next = state_next.at[S_D_TEMP_LY1].set(temp1)
    state_next = state_next.at[S_D_LF_LY1].set(lf1)

    # --- 6. Carbon Submodules ---
    # Knorr Phenology Allocation
    lambda_next, T_mem, laim, dlambdadt, f_T, f_d, l_tilde, l_W = KNORR_ALLOCATION(
        air_temp_k, deltat, LAT, DOY, state[S_D_LAI], pars[63], pars[59], pars[60], 
        pars[61], pars[62], state[S_H2O_LY1] + state_next[S_H2O_LY1], transp1 + transp2, 
        pars[64], pars[65], pars[66], state[S_M_LAI_TEMP], state[S_M_LAI_MAX]
    )
    state_next = state_next.at[S_M_LAI_MAX].set(laim)
    state_next = state_next.at[S_M_LAI_TEMP].set(T_mem)

    # Allocation and Autotrophic Respiration
    alloc_fol_pot = jnp.maximum(0.0, (lambda_next * pars[13] - state[S_C_fol]) / deltat)
    alloc_roo_pot = jnp.maximum(0.0, (pars[85] * lambda_next * pars[13]) / deltat)
    alloc_woo_pot = jnp.maximum(0.0, (pars[86] * lambda_next * pars[13]) / deltat)
    
    _, _, _, _, alloc_fol, alloc_woo, alloc_roo, auto_resp, _, _, nonleaf_mort = ALLOC_AND_AUTO_RESP_FLUXES(
        deltat, air_temp_k, state[S_C_woo], state[S_C_roo], state[S_C_lab], Ag, Rd, 
        pars[2], pars[3], pars[6], pars[4], alloc_fol_pot, alloc_woo_pot, alloc_roo_pot
    )

    # Heterotrophic Respiration
    ae_tr, an_tr, ch4_ratio, co2_ratio, _, _, _ = HET_RESP_RATES_JCR(
        state[S_D_TEMP_LY1], state[S_D_SM_LY1], state[S_D_LF_LY1], 
        pars[53], pars[54], pars[55], pars[56], pars[57], pars[12]
    )

    # --- 7. Carbon Pool Euler Updates ---
    ph_fol2lit = jnp.where(dlambdadt > 0, 0.0, -dlambdadt * pars[13])
    
    C_lab_next = state[S_C_lab] + (Ag - Rd - alloc_fol - alloc_roo - alloc_woo) * deltat
    C_fol_next = state[S_C_fol] + (alloc_fol - ph_fol2lit) * deltat
    C_roo_next = state[S_C_roo] + alloc_roo * deltat
    C_woo_next = state[S_C_woo] + alloc_woo * deltat
    
    state_next = state_next.at[S_C_lab].set(C_lab_next)
    state_next = state_next.at[S_C_fol].set(C_fol_next)
    state_next = state_next.at[S_C_roo].set(C_roo_next)
    state_next = state_next.at[S_C_woo].set(C_woo_next)
    state_next = state_next.at[S_D_LAI].set(C_fol_next / (pars[13] + 1e-9))

    # Record Target Fluxes
    fluxes = fluxes.at[F_gpp].set(Ag)
    fluxes = fluxes.at[F_resp_auto].set(auto_resp + Rd)
    
    return state_next, (state_next, fluxes)


# =====================================================================
# 3. JAX SCAN WRAPPER
# =====================================================================
@jax.jit
def run_dalec_1100(params, initial_state, forcing_matrix):
    """
    Executes the DALEC 1100 step function iteratively over the forcing matrix.
    """
    _, (states_traj, fluxes_traj) = jax.lax.scan(
        f=lambda state, forcing_t: dalec_1100_step(state, forcing_t, params),
        init=initial_state,
        xs=forcing_matrix
    )
    return states_traj, fluxes_traj


# =====================================================================
# 4. MODEL LIKELIHOOD FUNCTION (MLF)
# =====================================================================
@jax.jit
def DALEC_1100_JAX_MLF(params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std):
    """
    Calculates the negative log posterior.
    Returns: (MLF_value, (states_trajectory, fluxes_trajectory))
    """
    # Forward run
    states_traj, fluxes_traj = run_dalec_1100(params, initial_state, forcings)
    
    # Generate Observation Equivalents
    preds = DALEC_OBSERVATION_OPERATORS(states_traj, fluxes_traj, params)
    
    total_log_lik = 0.0
    
    # Loop over all requested observation targets dynamically
    # Use jnp.where to mask missing observation data (-9999)
    for key in obs_dict.keys():
        valid_mask = (obs_dict[key] != -9999.0)
        sq_error = jnp.where(valid_mask, ((obs_dict[key] - preds[key]) / (obs_unc[key] + 1e-9)) ** 2, 0.0)
        total_log_lik += -0.5 * jnp.sum(sq_error)
        
    # Log Prior
    log_prior = -0.5 * jnp.sum(((params - prior_mean) / (prior_std + 1e-9)) ** 2)
    
    # MLF value is the Negative Log Posterior
    mlf_value = -(total_log_lik + log_prior)
    
    return mlf_value, (states_traj, fluxes_traj)
