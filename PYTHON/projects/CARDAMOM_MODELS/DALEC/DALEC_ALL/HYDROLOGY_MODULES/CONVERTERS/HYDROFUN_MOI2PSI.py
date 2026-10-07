import jax.numpy as jnp

def HYDROFUN_MOI2PSI(moi, psi_porosity, b):
    """Convert volumetric soil moisture (m3/m3) to potential (MPa)"""
    return psi_porosity * jnp.power(1.0 / jnp.maximum(moi, 1e-9), b)
