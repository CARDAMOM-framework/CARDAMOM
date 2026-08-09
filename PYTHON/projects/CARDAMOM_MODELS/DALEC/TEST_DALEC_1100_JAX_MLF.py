import sys
from pathlib import Path

# Add 'PYTHON/projects' to Python path dynamically
projects_dir = Path(__file__).resolve().parents[2]
if str(projects_dir) not in sys.path:
    sys.path.insert(0, str(projects_dir))

# Now Python can find CARDAMOM_GENERAL
from CARDAMOM_GENERAL.CARDAMOM_READ_NETCDF_DATA import CARDAMOM_READ_NETCDF_DATA

# =====================================================================
# 1. FILE PATHS
# =====================================================================
input_file = "CARDAMOM-DEV/DATA/CARDAMOM_TEST_DRIVERS_APR26.cbf.nc"
parameter_file = "DUMPFILES/CARDAMOM_TEST_PARAMETERS_APR26.cbr.nc"
output_file = "DUMPFILES/CARDAMOM_TEST_OUTPUT_APR26.output.nc"

# =====================================================================
# 2. READ C MODEL OUTPUTS & PARAMETERS (.output.nc)
# =====================================================================
print(f"Reading C Benchmark Outputs from {output_file}...")
with Dataset(output_file, 'r') as nc_out:
    # Dimensions: (Sample, Time, Variable)
    # Pick the last sample index (-1) to match the parameter vector
    c_fluxes = nc_out.variables['FLUXES'][-1, :, :]  # Shape: (216, 100)
    c_pools = nc_out.variables['POOLS'][-1, :, :]    # Shape: (217, 30)
    c_pars = nc_out.variables['PARS'][-1, :]         # Shape: (89,) or (100,)

# Map C Parameter vector to JAX expectations (pad with zeros if 89 length)
params_np = np.zeros(100)
params_np[:len(c_pars)] = c_pars
params = jnp.array(params_np)

print(f"Extracted parameter vector with {len(c_pars)} parameters.")

# =====================================================================
# 3. READ FORCINGS (.cbf.nc)
# =====================================================================
print(f"Reading forcing drivers from {input_file}...")
forcings, lat, obs_dict, obs_unc = CARDAMOM_READ_NETCDF_DATA(input_file)
n_steps = forcings.shape[0]

# Extract initial states directly from C pools at time t=0
initial_state = jnp.array(c_pools[0, :])

# Dummy priors for MLF signature
prior_mean = jnp.zeros(100)
prior_std = jnp.ones(100) * 1000.0

# =====================================================================
# 4. RUN JAX FORWARD MODEL
# =====================================================================
print("Executing JAX DALEC_1100 Forward Model...")
mlf_value, (jax_states, jax_fluxes) = DALEC_1100_JAX_MLF(
    params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
)

print(f"JAX Run Complete! Negative Log Posterior: {mlf_value:.4f}")

# =====================================================================
# 5. OVERLAY PLOTS: C BENCHMARK VS. JAX
# =====================================================================
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(11, 8), sharex=True)

# --- Plot 1: Gross Primary Productivity (GPP) ---
ax1.plot(c_fluxes[:, 0], 'k-', label="C Model GPP", linewidth=2.0)
ax1.plot(jax_fluxes[:, F_gpp], 'r--', label="JAX GPP", linewidth=1.5)
if 'GPP' in obs_dict:
    valid_obs = np.where(obs_dict['GPP'] != -9999.0, obs_dict['GPP'], np.nan)
    ax1.plot(valid_obs, 'g.', label="Observed GPP", alpha=0.6)
ax1.set_ylabel("GPP (gC/m2/d)")
ax1.set_title("CARDAMOM Benchmark: C Implementation vs. JAX Translation")
ax1.legend(loc="upper right")
ax1.grid(True)

# --- Plot 2: Volumetric Soil Moisture (LY1) ---
ax2.plot(c_pools[1:, 22], 'k-', label="C Model SM LY1", linewidth=2.0)
ax2.plot(jax_states[:, S_D_SM_LY1], 'b--', label="JAX SM LY1", linewidth=1.5)
ax2.set_ylabel("Soil Moisture (m3/m3)")
ax2.legend(loc="upper right")
ax2.grid(True)

# --- Plot 3: Autotrophic Respiration ---
ax3.plot(c_fluxes[:, 2], 'k-', label="C Model Auto Resp", linewidth=2.0)
ax3.plot(jax_fluxes[:, F_resp_auto], 'm--', label="JAX Auto Resp", linewidth=1.5)
ax3.set_ylabel("Resp Auto (gC/m2/d)")
ax3.set_xlabel("Timestep (Months/Days)")
ax3.legend(loc="upper right")
ax3.grid(True)

plt.tight_layout()
plt.savefig("C_vs_JAX_Benchmark_Comparison.png")
print("Saved comparison plot to 'C_vs_JAX_Benchmark_Comparison.png'.")
