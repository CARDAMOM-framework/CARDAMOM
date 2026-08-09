import sys
from pathlib import Path
from netCDF4 import Dataset
import jax.numpy as jnp
import numpy as np
import matplotlib.pyplot as plt

# =====================================================================
# 1. PATH RESOLUTION (Relative only)
# =====================================================================
SCRIPT_DIR = Path(__file__).resolve().parent
PROJECTS_DIR = SCRIPT_DIR.parents[1]
if str(PROJECTS_DIR) not in sys.path:
    sys.path.insert(0, str(PROJECTS_DIR))

from CARDAMOM_GENERAL.CARDAMOM_READ_NETCDF_DATA import CARDAMOM_READ_NETCDF_DATA
from DALEC_1100_JAX_MLF import DALEC_1100_JAX_MLF

ROOT_DIR = SCRIPT_DIR.parents[4]

input_file = ROOT_DIR / "CARDAMOM-DEV" / "DATA" / "CARDAMOM_TEST_DRIVERS_APR26.cbf.nc"
parameter_file = ROOT_DIR / "DUMPFILES" / "CARDAMOM_TEST_PARAMETERS_APR26.cbr.nc"
output_file = ROOT_DIR / "DUMPFILES" / "CARDAMOM_TEST_OUTPUT_APR26.output.nc"

# Pool and Flux Name Mapping
POOL_NAMES = [
    "C_lab", "C_fol", "C_roo", "C_woo", "C_cwd", "C_lit", "C_som",
    "H2O_LY1", "H2O_LY2", "H2O_LY3", "H2O_SWE",
    "E_LY1", "E_LY2", "E_LY3",
    "D_LAI", "D_SCF", "D_TEMP_LY1", "D_TEMP_LY2", "D_TEMP_LY3",
    "D_LF_LY1", "D_LF_LY2", "D_LF_LY3", "D_SM_LY1", "D_SM_LY2", "D_SM_LY3",
    "D_PSI_LY1", "D_PSI_LY2", "D_PSI_LY3", "M_LAI_MAX", "M_LAI_TEMP"
]

FLUX_NAMES = [
    "GPP", "gppnet", "resp_auto", "lab_prod", "foliar_prod", "root_prod", "wood_prod", "unused7",
    "fol2lit", "woo2cwd", "roo2lit", "unused11", "cwd2som", "lit2som", "f_total", "f_lab",
    "f_fol", "f_roo", "f_woo", "f_cwd", "f_lit", "f_som", "fx_lab2lit", "fx_fol2lit",
    "fx_roo2lit", "fx_woo2cwd", "fx_cwd2som", "fx_lit2som", "q_ly1", "q_ly2", "ly1xly2",
    "ly2xly3", "q_ly3", "q_surf", "infil", "infil_e", "q_ly1_e", "q_ly2_e", "q_ly3_e",
    "ly1xly2_e", "ly2xly3_e", "evap_e", "transp1_e", "transp2_e", "transp1", "transp2",
    "evap", "snowfall", "melt", "ae_rh_cwd", "ae_rh_lit", "ae_rh_som", "an_rh_cwd",
    "an_rh_lit", "an_rh_som", "rh_co2", "rh_ch4", "aetr", "antr", "an_co2_c_ratio",
    "an_ch4_c_ratio", "target_LAI", "dlambda_dt", "f_temp_thresh", "f_dayl_thresh",
    "lai_fire", "foliar_fire_frac", "net_radiation", "latent_heat", "sensible_heat",
    "ground_heat", "gh_in", "resp_auto_growth", "resp_auto_maint", "SWin", "SWout",
    "LWin", "LWout", "ly1xly2_th_e", "ly2xly3_th_e", "resp_auto_maint_dark", "sublimation",
    "geological", "ets", "beta_factor", "soil_beta_factor"
]

# =====================================================================
# 2. READ SPECIFIC SAMPLE FROM C BENCHMARK OUTPUTS (.output.nc)
# =====================================================================
sample_idx = -1

print(f"Reading C Benchmark Sample Index {sample_idx} from {output_file}...")
with Dataset(output_file, 'r') as nc_out:
    c_fluxes_t0 = nc_out.variables['FLUXES'][sample_idx, 0, :]    # Fluxes at t=0
    c_pools_t0 = nc_out.variables['POOLS'][sample_idx, 0, :]      # Initial state (t=0)
    c_pools_t1 = nc_out.variables['POOLS'][sample_idx, 1, :]      # Updated state (t=1)
    c_pars = nc_out.variables['PARS'][sample_idx, :]

params_np = np.zeros(100)
params_np[:len(c_pars)] = c_pars
params = jnp.array(params_np)

# =====================================================================
# 3. READ FORCINGS & RUN JAX MODEL
# =====================================================================
forcings, lat, obs_dict, obs_unc = CARDAMOM_READ_NETCDF_DATA(str(input_file))
initial_state = jnp.array(c_pools_t0)

prior_mean = jnp.zeros(100)
prior_std = jnp.ones(100) * 1000.0

mlf_value, (jax_states, jax_fluxes) = DALEC_1100_JAX_MLF(
    params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
)

jax_pools_t1 = jax_states[0, :]
jax_fluxes_t0 = jax_fluxes[0, :]

# =====================================================================
# 4. PRINT TERMINAL COMPARISON TABLES FOR TIMESTEP 0 / 1
# =====================================================================
print("\n" + "="*80)
print(f"{'POOL NAME':<18} | {'INIT (t=0)':<12} | {'C STATE (t=1)':<14} | {'JAX STATE (t=1)':<14} | {'DIFF':<12}")
print("="*80)
for i in range(30):
    p_name = POOL_NAMES[i] if i < len(POOL_NAMES) else f"Pool_{i}"
    init_val = c_pools_t0[i]
    c_val = c_pools_t1[i]
    jax_val = float(jax_pools_t1[i])
    diff = jax_val - c_val
    print(f"{p_name:<18} | {init_val:<12.4f} | {c_val:<14.4f} | {jax_val:<14.4f} | {diff:<12.4f}")

print("\n" + "="*80)
print(f"{'FLUX NAME':<18} | {'C FLUX (t=0)':<16} | {'JAX FLUX (t=0)':<16} | {'DIFF':<12}")
print("="*80)
for i in range(min(86, len(FLUX_NAMES))):
    f_name = FLUX_NAMES[i]
    c_val = c_fluxes_t0[i]
    jax_val = float(jax_fluxes_t0[i])
    diff = jax_val - c_val
    # Print if either C or JAX flux is non-zero
    if abs(c_val) > 1e-6 or abs(jax_val) > 1e-6:
        print(f"{f_name:<18} | {c_val:<16.4f} | {jax_val:<16.4f} | {diff:<12.4f}")
print("="*80 + "\n")

# =====================================================================
# 5. DIAGNOSTIC COMPARISON FIGURE (TIMESTEP 0 / 1)
# =====================================================================
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 8))

x_indices = np.arange(30)
width = 0.35

# Plot 1: States at t=1
ax1.bar(x_indices - width/2, c_pools_t1, width, label='C Model State (t=1)', color='black', alpha=0.8)
ax1.bar(x_indices + width/2, jax_pools_t1, width, label='JAX Model State (t=1)', color='crimson', alpha=0.8)
ax1.set_xticks(x_indices)
ax1.set_xticklabels(POOL_NAMES, rotation=90, fontsize=8)
ax1.set_ylabel("State Pool Value")
ax1.set_title("Timestep 1 State Comparison (C vs JAX)")
ax1.legend()
ax1.grid(True, axis='y', linestyle='--', alpha=0.5)

# Plot 2: First 30 Fluxes at t=0
flux_indices = np.arange(30)
ax2.bar(flux_indices - width/2, c_fluxes_t0[:30], width, label='C Model Flux (t=0)', color='black', alpha=0.8)
ax2.bar(flux_indices + width/2, jax_fluxes_t0[:30], width, label='JAX Model Flux (t=0)', color='crimson', alpha=0.8)
ax2.set_xticks(flux_indices)
ax2.set_xticklabels(FLUX_NAMES[:30], rotation=90, fontsize=8)
ax2.set_ylabel("Flux Value")
ax2.set_title("Timestep 0 Flux Comparison (First 30 Fluxes)")
ax2.legend()
ax2.grid(True, axis='y', linestyle='--', alpha=0.5)

plt.tight_layout()
plt.show()
