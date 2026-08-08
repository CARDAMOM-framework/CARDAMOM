import jax.numpy as jnp

def SOIL_TEMP_AND_LIQUID_FRAC(dry_soil_vol_heat_capacity, depth, soil_water, internal_energy):
    """
    Calculates soil temperature (K) and liquid fraction (0 to 1).
    Differentiable translation replacing C if/else with jnp.where.
    """
    # Standard thermodynamic constants
    DGCM_SPECIFIC_HEAT_ICE = 2108.0      # J/kg/K
    DGCM_SPECIFIC_HEAT_WATER = 4184.0    # J/kg/K
    DGCM_LATENT_HEAT_FUSION_3 = 3.337e5  # J/kg
    DGCM_T3 = 273.16                     # K (Triple point)
    DGCM_TK0C = 273.15                   # K
    DGCM_T_LIQUID_H2O_ZERO_ENERGY = 273.15 # K

    dry_soil_sh = dry_soil_vol_heat_capacity * depth
    UI3 = (dry_soil_sh + soil_water * DGCM_SPECIFIC_HEAT_ICE) * DGCM_T3
    UL3 = UI3 + soil_water * DGCM_LATENT_HEAT_FUSION_3

    # --- Condition 1: Fully Frozen ---
    T_frozen = (internal_energy / (UI3 + 1e-9)) * DGCM_T3
    LF_frozen = 0.0

    # --- Condition 2: Fully Thawed ---
    T_thawed = (internal_energy + soil_water * DGCM_SPECIFIC_HEAT_WATER * DGCM_T_LIQUID_H2O_ZERO_ENERGY) / \
               (dry_soil_sh + soil_water * DGCM_SPECIFIC_HEAT_WATER + 1e-9)
    LF_thawed = 1.0

    # --- Condition 3: Mixed Phase (Freezing/Melting at 0 C) ---
    T_mixed = DGCM_TK0C
    LF_mixed = (internal_energy - UI3) / (soil_water * DGCM_LATENT_HEAT_FUSION_3 + 1e-9)

    # Resolve using jnp.where for differentiability
    TEMP = jnp.where(
        internal_energy < UI3, T_frozen,
        jnp.where(internal_energy > UL3, T_thawed, T_mixed)
    )
    
    LF = jnp.where(
        internal_energy < UI3, LF_frozen,
        jnp.where(internal_energy > UL3, LF_thawed, LF_mixed)
    )

    return TEMP, LF
