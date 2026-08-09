import jax.numpy as jnp
import matplotlib.pyplot as plt
from DALEC_1100_JAX_MLF import DALEC_1100_JAX_MLF, S_D_SM_LY1, F_gpp
from CARDAMOM_READ_NETCDF_DATA import CARDAMOM_READ_NETCDF_DATA

# 1. Load real driver file
nc_file = "../../../DATA/CARDAMOM_TEST_DRIVERS_APR26.cbf.nc" # Adjust relative path as needed
forcings, lat, obs_dict, obs_unc = CARDAMOM_READ_NETCDF_DATA(nc_file)

n_steps = forcings.shape[0]
n_states = 30
n_pars = 100

# 2. Setup initial parameters & states
initial_state = jnp.ones(n_states) * 10.0
initial_state = initial_state.at[S_D_SM_LY1].set(0.2)

params = jnp.ones(n_pars) * 0.5
prior_mean = jnp.ones(n_pars) * 0.5
prior_std = jnp.ones(n_pars) * 10.0

# 3. Execute model
print(f"Loaded {n_steps} timesteps of forcing data from NetCDF.")
print("Running DALEC 1100 JAX MLF...")

mlf_value, (states, fluxes) = DALEC_1100_JAX_MLF(
    params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
)

print(f"Simulation complete. MLF Value: {mlf_value:.4f}")
