import jax.numpy as jnp

def LIU_AN_ET(SRAD, VPD, TEMP, vcmax25, co2, beta_factor, g1, LAI, ga, VegK, 
              Tupp, Tdown, clumping, leaf_refl_par, leaf_refl_nir, maxPevap, 
              precip, q10canopy, q10canopyRd, canopyRdsf, NSC, deltat):
    """
    Differentiable JAX translation of LIU_AN_ET_REFACTOR.c
    Calculates Photosynthesis (Ag, An), Dark Respiration (Rd), Transpiration, and Evaporation.
    Assumes 100% C3 vegetation.
    """
    # -------------------------------------------------------------------------
    # Constants
    # -------------------------------------------------------------------------
    DGCM_TK0C = 273.15
    Ephoton = 2.0e-25 / 500.0e-9 # Planck const * c / wavelength
    NA = 6.02e23                 # Avogadro's constant
    lambda0 = 2.26e6             # Latent heat of vaporization
    gammaV = 100.0 * 1005.0 / (lambda0 * 0.622)

    T_C = TEMP - DGCM_TK0C

    # -------------------------------------------------------------------------
    # PAR & Canopy Scaling
    # -------------------------------------------------------------------------
    PAR_inc = SRAD / (2.0 * Ephoton * NA) * 1e6
    
    # 1e-9 added to denominator to prevent NaN gradients if VegK hits exactly 0
    canopy_scale = (1.0 - jnp.exp(-VegK * LAI * clumping)) / (VegK + 1e-9)
    PAR = PAR_inc * (1.0 - leaf_refl_par) * (1.0 - jnp.exp(-VegK * LAI * clumping))

    # -------------------------------------------------------------------------
    # Photosynthesis (Ag) and Respiration (Rd) Rates
    # -------------------------------------------------------------------------
    Kc = 300.0 * jnp.exp(0.074 * (T_C - 25.0))
    Ko = 300.0 * jnp.exp(0.015 * (T_C - 25.0))
    cp = 36.9 + 1.18 * (T_C - 25.0) + 0.036 * ((T_C - 25.0) ** 2.0)

    fT = q10canopyRd ** ((T_C - 25.0) / 10.0)
    
    Vcmax = vcmax25 * (q10canopy ** (0.1 * (T_C - 25.0))) / (
        (1.0 + jnp.exp(0.3 * (T_C - (Tupp - DGCM_TK0C)))) * 
        (1.0 + jnp.exp(0.3 * ((Tdown - DGCM_TK0C) - T_C)))
    )
    Jmax = Vcmax * jnp.exp(1.0)
    J = (0.3 * PAR + Jmax - jnp.sqrt((0.3 * PAR + Jmax)**2.0 - 4.0 * 0.9 * 0.3 * PAR * Jmax)) / (2.0 * 0.9)

    medlyn_term = 1.0 + g1 / jnp.sqrt(VPD + 1e-9)
    ci_raw = co2 * (1.0 - 1.0 / medlyn_term)
    ci = jnp.maximum(ci_raw, cp) # CLM 4.5 limit where ci cannot drop below cp

    # C3 Photosynthesis
    a1 = Vcmax * (ci - cp) / (ci + Kc * (1.0 + 209.0 / Ko))
    a2 = J * (ci - cp) / (4.0 * (ci + 2.0 * cp))
    
    Ag = jnp.minimum(a1 * beta_factor, a2)
    Rd = canopyRdsf * vcmax25 * fT

    # -------------------------------------------------------------------------
    # Mortality and Carbon Starvation
    # -------------------------------------------------------------------------
    Rd_daily_potential = Rd * canopy_scale * (12.0e-6) * (24.0 * 60.0 * 60.0)

    # Exponential starvation formulation replacing C if/else block
    LEAF_MORTALITY_FACTOR = jnp.where(
        Rd_daily_potential == 0.0,
        0.0,
        1.0 / jnp.exp(NSC / (Rd_daily_potential * deltat + 1e-9))
    )

    # Actual rates after accounting for starvation
    Rd_actual_canopy = Rd_daily_potential * (1.0 - LEAF_MORTALITY_FACTOR)
    Rd_leaf = Rd * (1.0 - LEAF_MORTALITY_FACTOR)
    An_leaf = Ag - Rd_leaf

    Ag_out = Ag * canopy_scale * (12.0e-6) * (24.0 * 60.0 * 60.0)
    An_out = An_leaf * canopy_scale * (12.0e-6) * (24.0 * 60.0 * 60.0)

    # -------------------------------------------------------------------------
    # Evapotranspiration
    # -------------------------------------------------------------------------
    sV = 0.04145 * jnp.exp(0.06088 * T_C)
    SRADg = (1.0 - 0.5 * (leaf_refl_par + leaf_refl_nir)) * SRAD * jnp.exp(-VegK * LAI * clumping)
    SRAD_net = (1.0 - 0.5 * (leaf_refl_par + leaf_refl_nir)) * SRAD

    petVnum = (sV * (SRAD_net - SRADg) + 1.225 * 1005.0 * VPD * ga) / lambda0 * 60.0 * 60.0
    petVnumB = 1.26 * (sV * SRADg) / (sV + gammaV) / lambda0 * 60.0 * 60.0

    gs = jnp.maximum(0.0, 1.6 * An_leaf / (co2 - ci + 1e-9) * LAI * 0.02405)

    transp = jnp.where(
        (beta_factor > 0.0) & (SRAD_net > 0.0),
        petVnum / (sV + gammaV * (ga * (1.0 / ga + 1.0 / (gs + 1e-9)))),
        0.0
    )

    evap_scale_factor = jnp.minimum(precip / (maxPevap + 1e-9), 1.0)
    evap = petVnumB * evap_scale_factor

    return An_out, Ag_out, Rd_actual_canopy, transp * 24.0, evap * 24.0, LEAF_MORTALITY_FACTOR
