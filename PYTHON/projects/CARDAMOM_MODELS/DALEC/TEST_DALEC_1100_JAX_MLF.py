import jax.numpy as jnp
import matplotlib.pyplot as plt
from netCDF4 import Dataset

from DALEC_1100_JAX_MLF import DALEC_1100_JAX_MLF, S_D_SM_LY1, F_gpp
from CARDAMOM_READ_NETCDF_DATA import CARDAMOM_READ_NETCDF_DATA

# =====================================================================
# 1. FILE PATHS (Matching MATLAB Step 3 & 3b)
# =====================================================================
input_file = "../CARDAMOM-DEV/DATA/CARDAMOM_TEST_DRIVERS_APR26.cbf.nc"
parameter_file = "../DUMPFILES/CARDAMOM_TEST_PARAMETERS_APR26.cbr.nc"
output_file = "../DUMPFILES/CARDAMOM_TEST_OUTPUT_APR26.output.nc"

# =====================================================================
# 2. READ PARAMETER FILE (.cbr.nc) - Equiv to Step 6
# =====================================================================
print(f"Reading parameters from {parameter_file}...")
with Dataset(parameter_file, 'r') as nc_par:
    # If parameters matrix is 2D (samples x parameters), pick the last/best sample
    raw_pars = nc_par.variables['Parameters'][:]
    if raw_pars.ndim == 2:
        pars_vec = raw_pars[-1, :]  # Extract single parameter vector
    else:
        pars_vec = raw_pars[:]

params = jnp.array(pars_vec)
n_pars = len(params)
print(f"Successfully extracted {n_pars} parameters.")

# =====================================================================
# 3. READ FORCINGS & OBS (.cbf.nc) - Equiv to Step 3/4b
# =====================================================================
print(f"Reading forcings and drivers from {input_file}...")
forcings, lat, obs_dict, obs_unc = CARDAMOM_READ_NETCDF_DATA(input_file)
n_steps = forcings.shape[0]

# Initial states vector (30 state pools)
# Extracted from initial pool parameters embedded within the parameter vector
n_states = 30
initial_state = jnp.zeros(n_states)
initial_state = initial_state.at[0:7].set(params[14:21])  # Initial carbon pools
initial_state = initial_state.at[S_D_SM_LY1].set(0.2)     # Initial soil moisture

# Dummy priors for MLF computation
prior_mean = jnp.zeros(n_pars)
prior_std = jnp.ones(n_pars) * 1000.0

# =====================================================================
# 4. RUN DALEC 1100 JAX FORWARD MODEL - Equiv to Step 4b
# =====================================================================
print("Executing DALEC_1100_JAX_MLF forward model...")
mlf_value, (states, fluxes) = DALEC_1100_JAX_MLF(
    params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
)

print(f"\nExecution Complete!")
print(f"Negative Log Likelihood / Posterior: {mlf_value:.4f}")

# =====================================================================
# 5. PLOT PREDICTIONS
# =====================================================================
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6))

# Plot JAX Predicted GPP
ax1.plot(fluxes[:, F_gpp], label="JAX DALEC_1100 GPP", color="green", linewidth=1.5)
if 'GPP' in obs_dict:
    ax1.plot(obs_dict['GPP'], 'k.', label="Observed GPP", alpha=0.5)
ax1.set_ylabel("GPP (gC/m2/d)")
ax1.set_title("DALEC 1100 JAX Simulation Output")
ax1.legend()
ax1.grid(True)

# Plot JAX Soil Moisture
ax2.plot(states[:, S_D_SM_LY1], label="JAX Volumetric Soil Moisture (LY1)", color="blue")
ax2.set_ylabel("Soil Moisture (m3/m3)")
ax2.set_xlabel("Timestep (Days)")
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.savefig("JAX_DALEC_1100_Benchmark_Run.png")
print("Saved run diagnostic plot to 'JAX_DALEC_1100_Benchmark_Run.png'.")
