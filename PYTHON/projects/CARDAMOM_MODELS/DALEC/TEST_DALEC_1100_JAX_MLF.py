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
from DALEC_1100_JAX_MLF import DALEC_1100_JAX_MLF, F_gpp, F_resp_auto, S_D_SM_LY1

ROOT_DIR = SCRIPT_DIR.parents[4]

input_file = ROOT_DIR / "CARDAMOM-DEV" / "DATA" / "CARDAMOM_TEST_DRIVERS_APR26.cbf.nc"
parameter_file = ROOT_DIR / "DUMPFILES" / "CARDAMOM_TEST_PARAMETERS_APR26.cbr.nc"
output_file = ROOT_DIR / "DUMPFILES" / "CARDAMOM_TEST_OUTPUT_APR26.output.nc"

# =====================================================================
# 2. READ SPECIFIC SAMPLE FROM C BENCHMARK OUTPUTS (.output.nc)
# =====================================================================
sample_idx = -1  # -1 = 800th (last) sample vector

print(f"Reading C Benchmark Sample Index {sample_idx} from {output_file}...")
with Dataset(output_file, 'r') as nc_out:
    c_fluxes = nc_out.variables['FLUXES'][sample_idx, :, :]  # (216, 100)
    c_pools = nc_out.variables['POOLS'][sample_idx, :, :]    # (217, 30)
    c_pars = nc_out.variables['PARS'][sample_idx, :]         # (89,)

params_np = np.zeros(100)
params_np[:len(c_pars)] = c_pars
params = jnp.array(params_np)

# =====================================================================
# 3. READ FORCINGS (.cbf.nc) & RUN JAX MODEL
# =====================================================================
forcings, lat, obs_dict, obs_unc = CARDAMOM_READ_NETCDF_DATA(str(input_file))
initial_state = jnp.array(c_pools[0, :])

prior_mean = jnp.zeros(100)
prior_std = jnp.ones(100) * 1000.0

mlf_value, (jax_states, jax_fluxes) = DALEC_1100_JAX_MLF(
    params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
)

# =====================================================================
# 4. SIDE-BY-SIDE COMPARISON FIGURE (Shared Y-axes across columns)
# =====================================================================
fig, axes = plt.subplots(5, 2, figsize=(14, 12), sharex=True, sharey='row')

# Column Headers
axes[0, 0].set_title("C Model Output", fontsize=12, fontweight='bold')
axes[0, 1].set_title("JAX Model Output", fontsize=12, fontweight='bold')

# Row 1: Gross Primary Productivity (GPP)
axes[0, 0].plot(c_fluxes[:, 0], 'k-', label="C GPP")
axes[0, 0].set_ylabel("GPP (gC/m2/d)")
axes[0, 0].grid(True)
axes[0, 0].legend(loc="upper right")

axes[0, 1].plot(jax_fluxes[:, F_gpp], 'r-', label="JAX GPP")
axes[0, 1].grid(True)
axes[0, 1].legend(loc="upper right")

# Row 2: Autotrophic Respiration
axes[1, 0].plot(c_fluxes[:, 2], 'k-', label="C Auto Resp")
axes[1, 0].set_ylabel("Auto Resp (gC/m2/d)")
axes[1, 0].grid(True)
axes[1, 0].legend(loc="upper right")

axes[1, 1].plot(jax_fluxes[:, F_resp_auto], 'm-', label="JAX Auto Resp")
axes[1, 1].grid(True)
axes[1, 1].legend(loc="upper right")

# Row 3: Volumetric Soil Moisture LY1 Diagnostic
axes[2, 0].plot(c_pools[1:, 22], 'k-', label="C SM LY1")
axes[2, 0].set_ylabel("Soil Moisture (m3/m3)")
axes[2, 0].grid(True)
axes[2, 0].legend(loc="upper right")

axes[2, 1].plot(jax_states[:, S_D_SM_LY1], 'b-', label="JAX SM LY1")
axes[2, 1].grid(True)
axes[2, 1].legend(loc="upper right")

# Row 4: Foliar Carbon Pool
axes[3, 0].plot(c_pools[1:, 1], 'k-', label="C Foliar Carbon")
axes[3, 0].set_ylabel("C Fol (gC/m2)")
axes[3, 0].grid(True)
axes[3, 0].legend(loc="upper right")

axes[3, 1].plot(jax_states[:, 1], 'g-', label="JAX Foliar Carbon")
axes[3, 1].grid(True)
axes[3, 1].legend(loc="upper right")

# Row 5: Water Pool Layer 1 (H2O_LY1)
axes[4, 0].plot(c_pools[1:, 7], 'k-', label="C H2O LY1")
axes[4, 0].set_ylabel("H2O Pool (mm)")
axes[4, 0].set_xlabel("Timestep")
axes[4, 0].grid(True)
axes[4, 0].legend(loc="upper right")

axes[4, 1].plot(jax_states[:, 7], 'c-', label="JAX H2O LY1")
axes[4, 1].set_xlabel("Timestep")
axes[4, 1].grid(True)
axes[4, 1].legend(loc="upper right")

plt.tight_layout()
plt.show()
