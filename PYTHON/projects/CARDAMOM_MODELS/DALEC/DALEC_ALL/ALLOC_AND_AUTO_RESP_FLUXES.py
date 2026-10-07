import jax.numpy as jnp

def ALLOC_AND_AUTO_RESP_FLUXES(deltat, TEMP, C_LIVE_W, C_LIVE_R, NSC, GPP, Rd, 
                               mr_r, mr_w, gr, Q10mr, 
                               ALLOC_FOL_POT, ALLOC_WOO_POT, ALLOC_ROO_POT):
    """
    Differentiable JAX translation of ALLOC_AND_AUTO_RESP_FLUXES.c
    Calculates maintenance respiration, growth respiration, and actual carbon allocation fluxes.
    """
    # -------------------------------------------------------------------------
    # 1. Maintenance Respiration
    # -------------------------------------------------------------------------
    # Reference temperature is 25 C (298.15 K)
    DGCM_TK0C = 273.15
    fT = Q10mr ** ((TEMP - (25.0 + DGCM_TK0C)) / 10.0) 
    
    # Autotrophic maintenance for wood and non-wood
    POTENTIAL_AUTO_RESP_MAINTENANCE = mr_w * fT * C_LIVE_W + mr_r * fT * C_LIVE_R

    # Available resources
    F_LABPROD = GPP - Rd
    NSC_PLUS_GPP_RATE = (NSC / deltat) + F_LABPROD

    # Carbon Starvation Factor (Non-leaf mortality)
    # Using jnp.where to handle the condition where potential maintenance is exactly zero
    NONLEAF_MORTALITY_FACTOR = jnp.where(
        POTENTIAL_AUTO_RESP_MAINTENANCE == 0.0,
        0.0,
        1.0 / jnp.exp(NSC_PLUS_GPP_RATE / (POTENTIAL_AUTO_RESP_MAINTENANCE + 1e-9))
    )

    AUTO_RESP_MAINTENANCE = POTENTIAL_AUTO_RESP_MAINTENANCE * (1.0 - NONLEAF_MORTALITY_FACTOR)

    # -------------------------------------------------------------------------
    # 2. Allocation Supply & Demand
    # -------------------------------------------------------------------------
    # Leftover NSCs after maintenance respiration is accounted for
    LEFTOVER_NSC_RATE = NSC_PLUS_GPP_RATE - AUTO_RESP_MAINTENANCE
    
    # Potential supply of labile carbon for plant growth
    F_LABREL_SUPPLY = jnp.maximum(0.0, gr * LEFTOVER_NSC_RATE)
    
    # Potential demand of labile carbon by plant growth
    TOTAL_GROWTH_POT = ALLOC_FOL_POT + ALLOC_WOO_POT + ALLOC_ROO_POT
    F_LABREL_DEMAND = jnp.maximum(0.0, TOTAL_GROWTH_POT)

    # Compute exponential growth factor (GF) representing amount of resources mobilizable
    GF = jnp.where(
        F_LABREL_DEMAND != 0.0,
        1.0 / jnp.exp(F_LABREL_SUPPLY / (F_LABREL_DEMAND + 1e-9)),
        0.0
    )

    # Actual release of labile carbon (before growth respiration costs subtracted)
    F_LABREL_ACTUAL = F_LABREL_DEMAND * (1.0 - GF)

    # -------------------------------------------------------------------------
    # 3. Final Allocation Fluxes
    # -------------------------------------------------------------------------
    ALLOC_FOL_ACTUAL = ALLOC_FOL_POT * (1.0 - GF)
    ALLOC_WOO_ACTUAL = ALLOC_WOO_POT * (1.0 - GF)
    ALLOC_ROO_ACTUAL = ALLOC_ROO_POT * (1.0 - GF)

    # -------------------------------------------------------------------------
    # 4. Growth Respiration & Diagnostics
    # -------------------------------------------------------------------------
    TOTAL_GROWTH_ACTUAL = ALLOC_FOL_ACTUAL + ALLOC_WOO_ACTUAL + ALLOC_ROO_ACTUAL
    
    # Growth respiration
    AUTO_RESP_GROWTH = ((1.0 - gr) / (gr + 1e-9)) * TOTAL_GROWTH_ACTUAL

    # Diagnostics
    AUTO_RESP_TOTAL = AUTO_RESP_MAINTENANCE + AUTO_RESP_GROWTH
    NPP = GPP - AUTO_RESP_TOTAL
    
    # Protect against divide-by-zero for CUE calculation
    CUE = jnp.where(GPP == 0.0, 0.0, NPP / (GPP + 1e-9))

    return (F_LABPROD, F_LABREL_ACTUAL, AUTO_RESP_MAINTENANCE, AUTO_RESP_GROWTH, 
            ALLOC_FOL_ACTUAL, ALLOC_WOO_ACTUAL, ALLOC_ROO_ACTUAL, 
            AUTO_RESP_TOTAL, NPP, CUE, NONLEAF_MORTALITY_FACTOR)
