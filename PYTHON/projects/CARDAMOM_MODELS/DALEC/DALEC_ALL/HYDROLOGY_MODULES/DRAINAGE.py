import jax.numpy as jnp
from .CONVERTERS.HYDROFUN_PSI2MOI import HYDROFUN_PSI2MOI
from .CONVERTERS.HYDROFUN_MOI2PSI import HYDROFUN_MOI2PSI

def DRAINAGE(sm, Qexcess, psi_field, psi_porosity, b):
    """
    Calculate drainage for soil layer
    """
    # Soil moisture at field capacity
    sm_field = HYDROFUN_PSI2MOI(psi_field, psi_porosity, b)

    # Check for excessive soil moisture
    excess_drainage = jnp.maximum(sm - 1.0, 0.0)
    sm_adj = jnp.minimum(sm, 1.0)

    # Change in soil moisture
    delta_sm = jnp.maximum(sm_adj - sm_field, 0.0)

    # Potential of layer
    psi = HYDROFUN_MOI2PSI(sm_adj, psi_porosity, b)
    
    # Calculate drainage
    clipped_psi = jnp.clip(psi, psi_field, psi_porosity)
    drainage = excess_drainage + delta_sm * Qexcess * (
        1.0 - (psi_porosity - clipped_psi) / (psi_porosity - psi_field + 1e-9)
    )

    return drainage
