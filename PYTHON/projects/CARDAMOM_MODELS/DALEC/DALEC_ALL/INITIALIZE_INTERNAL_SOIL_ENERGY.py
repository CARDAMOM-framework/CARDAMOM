import jax.numpy as jnp
from .SOIL_TEMP_AND_LIQUID_FRAC import SOIL_TEMP_AND_LIQUID_FRAC

def INITIALIZE_INTERNAL_SOIL_ENERGY(internal_energy_per_mm_H2O, H2O_mm, dry_soil_vol_heat_capacity, depth):
    """
    Initializes the thermal energy pool of a soil layer.
    """
    # Calculate H2O temp (1mm, no soil)
    TEMP, _ = SOIL_TEMP_AND_LIQUID_FRAC(0.0, 0.0, 1.0, internal_energy_per_mm_H2O)
    
    # Energy in dry soil
    SOIL_E = dry_soil_vol_heat_capacity * depth * TEMP
    
    # Energy in H2O
    H2O_E = internal_energy_per_mm_H2O * H2O_mm
    
    # Total energy
    TOTAL_E = SOIL_E + H2O_E
    
    return TOTAL_E
