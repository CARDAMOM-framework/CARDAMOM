#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../DALEC_ALL/mean_pool.c"
#include "../DALEC_ALL/mean_annual_pool.c"

//*************** State Proximity EDC ******************
// Penalizes the model if the max-min difference among specified pools 
// (e.g., annual mean soil temperatures) exceeds a defined threshold.

typedef struct {
    int no_pools_to_check;
    int *pool_indices;
    double max_allowed_diff; // E.g., 4.0 (degrees C)
    double penalty_scale;    // E.g., 1.0 (controls steepness of the quadratic penalty)
} DALEC_EDC_STATE_PROXIMITY_STRUCT;

// General state proximity function
double DALEC_EDC_STATE_PROXIMITY(void * data, DATA * DATA, int prerun){
    
    // Correctly cast the generic void pointer to your specific struct pointer
    DALEC_EDC_STATE_PROXIMITY_STRUCT * E = (DALEC_EDC_STATE_PROXIMITY_STRUCT *) data;
    double PEDC = 0.0;
    
    double *TIME_INDEX = DATA->ncdf_data.TIME_INDEX.values;
    DALEC *DALECmodel = (DALEC *)DATA->MODEL;
    
    int N_timesteps = DATA->ncdf_data.TIME_INDEX.length;
    int nopools = DALECmodel->nopools;
    double * POOLS = DATA->M_POOLS;
    
    // Derive deltat from TIME_INDEX (assuming continuous daily or monthly steps)
    double deltat_double = TIME_INDEX[1] - TIME_INDEX[0];
    
    // mean_annual_pool.c expects an integer deltat (e.g., 1 for daily data)
    int deltat = (int)round(deltat_double); 
    if (deltat < 1) deltat = 1; // Failsafe 
    
    // Calculate total number of complete years to evaluate
    int num_years = (int)floor((N_timesteps * deltat_double) / 365.25);
    
    int yr, s, p;
    
    // Loop through each complete year
    for (yr = 0; yr < num_years; yr++) {
        
        double min_mean = 1e9;
        double max_mean = -1e9;
        
        // Check each pool for this specific year (using -> operator for pointer access)
        for (s = 0; s < E->no_pools_to_check; s++) {
            p = E->pool_indices[s];
            double mean_pool_yr = mean_annual_pool(POOLS, yr, p, nopools, deltat);
            
            // Track the maximum and minimum annual means across the layers
            if (mean_pool_yr > max_mean) max_mean = mean_pool_yr;
            if (mean_pool_yr < min_mean) min_mean = mean_pool_yr;
        }
        
        double diff = max_mean - min_mean;
        
        // If the gradient exceeds your physical threshold, apply quadratic penalty
        if (diff > E->max_allowed_diff) {
            // Evaluates as: -0.5 * ((diff - threshold) / scale)^2
            PEDC += -0.5 * pow((diff - E->max_allowed_diff) / E->penalty_scale, 2.0);
        }
    }
    
    return PEDC;
}