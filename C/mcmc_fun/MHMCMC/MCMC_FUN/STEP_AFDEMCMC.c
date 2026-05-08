#pragma once
#include <math.h>
#include <stdlib.h>
#include "../../../math_fun/randn.c"

double STEP_AFDEMCMC(double *PARS, double *pars_new, PARAMETER_INFO PI, int C, int NC, double *gratio){
    /* AFDEMCMC takes an Affine Invariant ensemble step (Goodman & Weare) */

    int n, C1=-1;

    /* Step 1. Find random sample (reference chain) other than current chain */
    C1=C;
    while (C1==C){
        C1 = ceil((double)random() * NC / ((double)RAND_MAX)) - 1;
    }

    /* Step 2. Allocate memory for normalized parameter vectors */
    /* (Notice we deleted the heavy step and gamma arrays here!) */
    double *cpar = calloc(PI.npars, sizeof(double));
    double *rpar = calloc(PI.npars, sizeof(double));
    double *npar = calloc(PI.npars, sizeof(double));

    /* Normalize parameters into [0,1] space */
    for (n=0; n<PI.npars; n++){
        cpar[n] = par2nor(PARS[C*PI.npars+n], PI.parmin[n], PI.parmax[n]);
        rpar[n] = par2nor(PARS[C1*PI.npars+n], PI.parmin[n], PI.parmax[n]);
    }

    /* Step 3. The Affine Invariant Step */
   // double a_gw = 2.0; // The Goodman-Weare tuning parameter
   // double u = (double)random() / (double)RAND_MAX; // Uniform distribution between 0 and 1
 
// 1. Draw a_gw using a shifted Log-Normal distribution: 1 + exp(N(0,1))
    // (This heavily favors values between 1.1 and 2.5, but occasionally spikes much higher)
    double a_gw = 1.0 + exp(randn()); 

    // 2. Draw 'u' for the z transformation
    double u_z = (double)random() / (double)RAND_MAX; 
    
    // 3. Transform into the 1/sqrt(z) distribution using our random a_gw
    double z = pow((u_z * (sqrt(a_gw) - 1.0/sqrt(a_gw)) + 1.0/sqrt(a_gw)), 2.0);

    // Transform 'u' into the 1/sqrt(z) distribution
   // double z = pow((u * (sqrt(a_gw) - 1.0/sqrt(a_gw)) + 1.0/sqrt(a_gw)), 2.0);

    int withinlim = 1;

    // Apply the scalar jump 'z' across all dimensions
    for (n=0; n<PI.npars; n++){
        npar[n] = rpar[n] + z * (cpar[n] - rpar[n]);
        
        // Strict boundary check
        if (npar[n] < 0 || npar[n] > 1){ 
            withinlim = 0;
        }
    }

    gratio[0] = 0;

    /* Step 4. Transform back to parameter space & apply penalty */
    if (withinlim == 1){
        for (n=0; n<PI.npars; n++){
            pars_new[n] = nor2par(npar[n], PI.parmin[n], PI.parmax[n]);
        }

        /* Step 5. Geometric Penalty (Replaces gc2n/gn2c logic) */
        /* This log(z) penalty maintains detailed balance in the log-likelihood ratio */
        gratio[0] = ((double)PI.npars - 1.0) * log(z);
    }

    /* Step 6. Free memory to prevent leaks */
    free(rpar);
    free(cpar);
    free(npar);

    return withinlim;
}
