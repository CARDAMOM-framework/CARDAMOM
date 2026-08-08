import jax.numpy as jnp

def HYDROFUN_EWT2MOI(ewt, p, z):
    """Convert equivalent water thickness (mm or kg/m2) to volumetric soil moisture (m3/m3)"""
    return ewt / (1000.0 * p * z + 1e-9)
