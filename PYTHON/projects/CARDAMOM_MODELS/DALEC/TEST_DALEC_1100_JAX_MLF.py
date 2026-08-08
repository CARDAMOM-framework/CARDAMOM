import jax.numpy as jnp
import matplotlib.pyplot as plt
from DALEC_1100_JAX_MLF import DALEC_1100_JAX_MLF, S_D_SM_LY1, F_gpp

# =====================================================================
# 1. SETUP DUMMY DATA
# =====================================================================
N_days = 365
n_states = 30
n_pars = 100

# Forcings Matrix: [SSRD, T2M_MIN, T2M_MAX, CO2, PREC, VPD, BURNED_AREA, SNOWFALL, SKT, STRD, DIST, YIELD, DOY, LAT]
forcings = jnp.zeros((N_days, 14))
forcings = forcings.at[:, 0].set(15.0)       # SSRD
forcings = forcings.at[:, 1].set(285.15)     # T2M_MIN
forcings = forcings.at[:, 2].set(295.15)     # T2M_MAX
forcings = forcings.at[:, 3].set(400.0)      # CO2
forcings = forcings.at[:, 4].set(jnp.where(jnp.arange(N_days) % 10 == 0, 10.0, 0.0)) # Precip spikes
forcings = forcings.at[:, 5].set(1.5)        # VPD
forcings = forcings.at[:, 12].set(jnp.arange(N_days)) # DOY
forcings = forcings.at[:, 13].set(45.0)      # LAT

# Initial States & Parameters
initial_state = jnp.ones(n_states) * 10.0
initial_state = initial_state.at[S_D_SM_LY1].set(0.5)

params = jnp.ones(n_pars) * 0.5
params = params.at[13].set(50.0)  # LCMA
params = params.at[36].set(50.0)  # Vcmax25
params = params.at[35].set(1.0)   # LY1_z
params = params.at[40].set(0.1)   # psi_field
params = params.at[84].set(4.1)   # beta_lgr
params = params.at[83].set(2.0)   # psi_50

# Dummy Observations and Priors
obs_dict = {'GPP': jnp.ones(N_days) * 5.0, 'ABGB': jnp.ones(N_days) * 40.0, 'PEQ_CUEmrg': 0.5}
obs_unc = {'GPP': 2.0, 'ABGB': 5.0, 'PEQ_CUEmrg': 0.1}
prior_mean = jnp.ones(n_pars) * 0.5
prior_std = jnp.ones(n_pars) * 10.0

# =====================================================================
# 2. RUN FORWARD MODEL
# =====================================================================
print("Compiling and Running DALEC 1100 JAX MLF...")
mlf_value, (states, fluxes) = DALEC_1100_JAX_MLF(
    params, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
)

print(f"\nSUCCESS! MLF Value (Negative Log Posterior): {mlf_value:.4f}")

# =====================================================================
# 3. PLOT OUTPUTS
# =====================================================================
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6))

ax1.plot(states[:, S_D_SM_LY1], label="SM_LY1 (Soil Moisture)", color='blue')
ax1.set_ylabel("Volumetric (m3/m3)")
ax1.legend()
ax1.grid(True)

ax2.plot(fluxes[:, F_gpp], label="GPP Flux", color='green')
ax2.set_ylabel("GPP (gC/m2/d)")
ax2.set_xlabel("Time (Days)")
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.savefig("DALEC_1100_Forward_Test.png")
print("Saved plot to DALEC_1100_Forward_Test.png")
