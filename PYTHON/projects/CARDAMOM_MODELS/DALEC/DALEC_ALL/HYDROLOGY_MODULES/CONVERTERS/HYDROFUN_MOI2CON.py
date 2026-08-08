import jax.numpy as jnp

def HYDROFUN_MOI2CON(moi, k0, b):
    """Convert volumetric soil moisture (m3/m3) to conductivity (m/s)"""
    return k0 * jnp.power(jnp.maximum(moi, 1e-9), 2.0 * b + 3.0)
