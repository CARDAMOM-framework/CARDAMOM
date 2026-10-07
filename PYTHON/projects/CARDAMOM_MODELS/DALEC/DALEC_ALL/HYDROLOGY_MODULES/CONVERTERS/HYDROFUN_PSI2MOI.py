import jax.numpy as jnp

def HYDROFUN_PSI2MOI(psi, psi_porosity, b):
    """Convert potential (MPa) to volumetric soil moisture (m3/m3)"""
    ratio = psi_porosity / jnp.minimum(psi, -1e-9)
    return jnp.power(ratio, 1.0 / b)
