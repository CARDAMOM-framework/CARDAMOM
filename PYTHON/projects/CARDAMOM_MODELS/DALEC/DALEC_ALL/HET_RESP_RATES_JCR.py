import jax.numpy as jnp

def HET_RESP_RATES_JCR(TEMP, SM, LF, S_FV, SM_OPT, FWC, R_CH4, Q10CH4, Q10CO2):
    """
    Differentiable JAX translation of HET_RESP_RATES_JCR.c
    Calculates environmental scalars for aerobic and anaerobic heterotrophic respiration.
    """
    # Reference temperature (25 C in Kelvin)
    reftemp = 298.15

    # -------------------------------------------------------------------------
    # 1. Temperature Scalar (fT) & Volumetric Scalar (fV)
    # -------------------------------------------------------------------------
    # Q10 method for CO2
    fT = (Q10CO2 ** ((TEMP - reftemp) / 10.0)) * LF
    
    # fV separates aerobic and anaerobic respiration based on soil moisture (SM)
    fV = jnp.maximum(0.0, 1.0 - (SM ** S_FV))

    # -------------------------------------------------------------------------
    # 2. Water Scalar (fW)
    # -------------------------------------------------------------------------
    # Soil moisture scaler is a function of theta_ae
    # Added 1e-9 to fV to prevent division by zero when soil is perfectly saturated
    theta_ae = ((SM - 1.0) / (fV + 1e-9) + 1.0)
    
    fW1 = (1.0 / SM_OPT) * theta_ae
    fW2 = ((1.0 - FWC) / (SM_OPT - 1.0) * theta_ae + (FWC - (1.0 - FWC) / (SM_OPT - 1.0)))

    # Piecewise logic using jnp.where for differentiability
    fW = jnp.where(
        (theta_ae >= 0.0) & (theta_ae < SM_OPT), 
        fW1, 
        jnp.where(
            (theta_ae >= SM_OPT) & (theta_ae <= 1.0), 
            fW2, 
            0.0
        )
    )

    # -------------------------------------------------------------------------
    # 3. CH4 Production Scalars
    # -------------------------------------------------------------------------
    fT_ch4 = (Q10CH4 ** ((TEMP - reftemp) / 10.0)) * LF
    
    # fCH4 is the fraction of anaerobic carbon loss that becomes methane
    fCH4 = jnp.minimum(R_CH4 * fT_ch4, 1.0)

    # -------------------------------------------------------------------------
    # 4. Final Turnover Rates
    # -------------------------------------------------------------------------
    aerobic_tr = fW * fT * fV
    anaerobic_tr = FWC * fT * (1.0 - fV)
    
    anaerobic_ch4_c_ratio = fCH4
    anaerobic_co2_c_ratio = 1.0 - fCH4

    return aerobic_tr, anaerobic_tr, anaerobic_ch4_c_ratio, anaerobic_co2_c_ratio, fT, fV, fW
