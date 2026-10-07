import jax.numpy as jnp

def INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(TEMP):
    """
    Internal energy per mm (or kg) H2O
    Temp is in Kelvin. Assumes liquid fraction is = 1.
    """
    DGCM_SPECIFIC_HEAT_WATER = 4184.0 # J/kg/K
    DGCM_T_LIQUID_H2O_ZERO_ENERGY = 273.15 # K
    
    U = DGCM_SPECIFIC_HEAT_WATER * (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY)
    return U
