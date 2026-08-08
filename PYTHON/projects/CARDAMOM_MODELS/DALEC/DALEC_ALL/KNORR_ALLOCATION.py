import jax.numpy as jnp
from jax.scipy.special import erfc

# =====================================================================
# HELPER FUNCTIONS (From LAI_KNORR_funcs.c)
# =====================================================================
def MinQuadraticSmooth(x, y, eta):
    """Minimum function with quadratic transition (smoothed) between two values."""
    z = (x + y)**2.0 - 4.0 * eta * x * y
    z = jnp.maximum(z, 1e-18)
    mins = (x + y - jnp.sqrt(z)) / (2.0 * eta)
    return mins

def MaxExponentialSmooth(x, y, x0):
    """Maximum function with exponential transition (smoothed) between two values."""
    return jnp.where(
        x >= (y - x0), 
        x + x0 * jnp.exp(-(x - y) / x0 - 1.0), 
        y
    )

def ComputeDaylightHours(latitude, DOY):
    """Day length function - hours of sunlight per day."""
    pi = jnp.pi
    
    # Declination angle
    dec = -23.4 * jnp.cos((360.0 * (DOY + 10.0) / 365.0) * pi / 180.0) * pi / 180.0
    mult = jnp.tan(latitude * pi / 180.0) * jnp.tan(dec)
    
    # Clip multiplier to [-1, 1] to avoid NaNs in arccos, replacing the C if/else bounds
    mult_clipped = jnp.clip(mult, -1.0, 1.0)
    dayl = 24.0 * jnp.arccos(-mult_clipped) / pi
    
    return dayl

# =====================================================================
# MAIN MODULE (From KNORR_ALLOCATION.c)
# =====================================================================
def KNORR_ALLOCATION(temp, deltat, latitude, DOY, lambda_lai, lambda_max, 
                     T_phi, T_r, plgr, k_L, pasm, transp, tau_W, t_c, t_r, 
                     T_memory, lambda_max_memory):
    """
    Differentiable JAX translation of the Knorr Leaf Phenology Model.
    Calculates the target LAI, growth rate, and updates phenology memories.
    """
    tau_m = 30.0 # Temperature memory averaging period
    tau_s = 30.0 # Water/structural memory averaging period
    
    # -------------------------------------------------------------------------
    # 1. Temperature Threshold (f_T)
    # -------------------------------------------------------------------------
    # Exponentially declining memory of temperature
    T = jnp.exp(-deltat / tau_m) * T_memory + temp * (1.0 - jnp.exp(-deltat / tau_m))
    T_deviation = (T - T_phi) / T_r
    
    # Fraction of plants above temp threshold using cumulative normal distribution
    f_T = 0.5 * erfc(-T_deviation * jnp.sqrt(0.5))
    
    # -------------------------------------------------------------------------
    # 2. Daylength Threshold (f_d)
    # -------------------------------------------------------------------------
    daylength = ComputeDaylightHours(latitude, DOY)
    td_deviation = (daylength - t_c) / t_r
    
    # Fraction of plants above daylength threshold
    f_d = 0.5 * erfc(-td_deviation * jnp.sqrt(0.5))
    
    # -------------------------------------------------------------------------
    # 3. Growth Rates and Smoothing
    # -------------------------------------------------------------------------
    # Fraction of plants above both thresholds
    f = f_T * f_d
    
    # Current growth rate
    r = plgr * f + (1.0 - f) * k_L
    
    # Compute water-limited max LAI (added 1e-9 to prevent divide by zero)
    transp_smooth = MaxExponentialSmooth(transp, 1e-3, 2e-2)
    lambda_W = (pasm * lambda_lai) / (tau_W * transp_smooth + 1e-9)
    
    # Smoothed max LAI
    lambda_tilde_max = MinQuadraticSmooth(lambda_max, lambda_W, 0.99)
    
    # Update LAI structural memory
    laim = jnp.exp(-deltat / tau_s) * lambda_max_memory + lambda_tilde_max * (1.0 - jnp.exp(-deltat / tau_s))
    
    # Limiting target LAI
    lambda_lim = MaxExponentialSmooth(plgr * laim * f / (r + 1e-9), 1e-9, 5e-3)
    
    # -------------------------------------------------------------------------
    # 4. Final Updates
    # -------------------------------------------------------------------------
    # Updated target LAI (units of m2/m2)
    lambda_next = lambda_lim - (lambda_lim - lambda_lai) * jnp.exp(-r * deltat)
    
    # Change in LAI over deltat
    dlambdadt = lambda_next - lambda_lai

    return (lambda_next, T, laim, dlambdadt, f_T, f_d, lambda_tilde_max, lambda_W)
