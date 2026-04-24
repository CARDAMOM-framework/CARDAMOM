#pragma once
#include <math.h>

/* =================================================================================
 * SWE_SNOW_DENSITY
 * * Calculates the dynamic density of the snowpack based on temperature-driven 
 * destructive metamorphism (compaction) and fresh snowfall.
 * * CITATIONS:
 * 1. Anderson, E. A. (1976). A point energy and mass balance model of a snow cover. 
 * (NOAA Technical Report NWS 19). 
 * -> Establishes the exponential temperature dependence of snow metamorphism (0.08 / C).
 * * 2. Verseghy, D. L. (1991). CLASS—A Canadian land surface scheme for GCMS. I. Soil model. 
 * International Journal of Climatology, 11(2), 111-133. 
 * -> Establishes the exponential relaxation of snow density toward a maximum 
 * of 300 kg/m3 over time.
 * ================================================================================= */

typedef struct {    
    struct {
        double snowfall_rate;
        double deltat;
        double tskin_k;
        double swe_old;
        double rho_old;
    } IN;
    struct {
        double swe_new;
        double rho_new;
    } OUT;
} SWE_SNOW_DENSITY_STRUCT;

int SWE_SNOW_DENSITY(SWE_SNOW_DENSITY_STRUCT * S) {

    // --- Physical Constants (Verseghy 1991 / CLASS Model) ---
    const double rho_fresh = 100.0; // Density of fresh powder (kg m-3)
    const double rho_max   = 300.0; // Maximum density of ripe seasonal snow (kg m-3)
    const double c_T       = 0.08;  // Temperature sensitivity constant (degC-1; Anderson 1976)
    const double k_base    = 0.1;   // Baseline compaction rate at 0 degC (approx 0.1 day-1)
    
    // Initialize working variables from the struct
    double swe_old = S->IN.swe_old;
    double rho_old = S->IN.rho_old;

    // Safety check for uninitialized density on first timestep
    if (rho_old < rho_fresh) {
        rho_old = rho_fresh;
    }

    // --- 1. Age / Compact the Old Snow ---
    // Convert LST to Celsius. Cap at 0.0C because active melting (>0C) 
    // maxes out the compaction rate, and phase change is handled downstream.
    double T_C = S->IN.tskin_k - 273.15;
    if (T_C > 0.0) {
        T_C = 0.0; 
    }

    // Exponential compaction rate based on temperature
    double k_age = k_base * exp(c_T * T_C);
    
    // Relax density toward rho_max
    double rho_compacted = rho_max - (rho_max - rho_old) * exp(-k_age * S->IN.deltat);

    // --- 2. Introduce New Snow ---
    double p_snow = S->IN.snowfall_rate * S->IN.deltat; // Total fresh snow mass added this step
    S->OUT.swe_new = swe_old + p_snow;

    // --- 3. Aggregate Density (Mass-Weighted) ---
    if (S->OUT.swe_new > 1e-6) { // Prevent division by zero if ground is bare
        S->OUT.rho_new = ((swe_old * rho_compacted) + (p_snow * rho_fresh)) / S->OUT.swe_new;
    } else {
        S->OUT.rho_new = rho_fresh; // Fallback for bare ground
    }

    // Enforce physical bounds
    if (S->OUT.rho_new > rho_max) S->OUT.rho_new = rho_max;
    if (S->OUT.rho_new < rho_fresh) S->OUT.rho_new = rho_fresh;

    return 0;
}