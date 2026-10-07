import jax
import jax.numpy as jnp
from DALEC_1100_JAX_MLF import DALEC_1100_JAX_MLF

# =====================================================================
# 1. SETUP HMC SCENARIO
# =====================================================================
N_days = 365
n_states = 30
n_pars = 100

forcings = jnp.ones((N_days, 14)) * 10.0
initial_state = jnp.ones(n_states) * 10.0
params = jnp.ones(n_pars) * 0.5
obs_dict = {'GPP': jnp.ones(N_days) * 5.0}
obs_unc = {'GPP': 2.0}
prior_mean = jnp.ones(n_pars) * 0.5
prior_std = jnp.ones(n_pars) * 1.0

# Mass Matrix (Diagonal Fisher Approximation for HMC)
M = 1.0 / (prior_std ** 2)

# =====================================================================
# 2. HAMILTONIAN MONTE CARLO STEP
# =====================================================================
@jax.jit
def hmc_step(key, params_current, step_size, L):
    # 1. Sample random momentum
    key, subkey = jax.random.split(key)
    v_current = jax.random.normal(subkey, shape=params_current.shape) * jnp.sqrt(M)
    
    # 2. Extract Posterior AND Gradients instantly
    (U_current, _), grad_U = jax.value_and_grad(DALEC_1100_JAX_MLF, has_aux=True)(
        params_current, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
    )
    
    params_new = params_current
    v_new = v_current
    
    # 3. Leapfrog Integrator
    v_new = v_new - 0.5 * step_size * grad_U
    
    # Use jax.lax.fori_loop to keep the leapfrog compiled on the GPU
    def leapfrog_body(i, val):
        p, v = val
        p = p + step_size * (v / M)
        (_, _), g = jax.value_and_grad(DALEC_1100_JAX_MLF, has_aux=True)(
            p, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
        )
        v = v - step_size * g
        return p, v
        
    params_new, v_new = jax.lax.fori_loop(0, L - 1, leapfrog_body, (params_new, v_new))
    
    params_new = params_new + step_size * (v_new / M)
    (U_new, _), grad_U = jax.value_and_grad(DALEC_1100_JAX_MLF, has_aux=True)(
        params_new, initial_state, forcings, obs_dict, obs_unc, prior_mean, prior_std
    )
    v_new = v_new - 0.5 * step_size * grad_U
    
    # 4. Accept / Reject
    K_current = 0.5 * jnp.sum((v_current ** 2) / M)
    K_new = 0.5 * jnp.sum((v_new ** 2) / M)
    
    accept_prob = jnp.exp((U_current + K_current) - (U_new + K_new))
    key, subkey = jax.random.split(key)
    accept = jax.random.uniform(subkey) < accept_prob
    
    params_next = jnp.where(accept, params_new, params_current)
    U_next = jnp.where(accept, U_new, U_current)
    
    return params_next, U_next, accept, key

# =====================================================================
# 3. RUN CHAIN
# =====================================================================
print("Starting DALEC 1100 JAX HMC Chain...")
key = jax.random.PRNGKey(42)
current_params = prior_mean.copy()

num_samples = 50
for i in range(num_samples):
    current_params, current_U, acc, key = hmc_step(key, current_params, step_size=0.005, L=10)
    print(f"Step {i+1}/{num_samples} | NegLogPost: {current_U:.2f} | Accept: {acc}")

print("HMC Routine Complete!")
