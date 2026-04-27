static int attempt_count[100];
static int pass_count[100];
static int total_calls = 0; 

typedef struct {
    double * min_val;
    double * max_val; 
} DALEC_EDC_STATE_RANGES_STRUCT;

double DALEC_EDC_STATE_RANGES(DATA * DATA, void * EDCstruct){

    double PEDC = 0;
    total_calls += 1;
    DALEC_EDC_STATE_RANGES_STRUCT E = *(DALEC_EDC_STATE_RANGES_STRUCT * ) EDCstruct;
    
    int p = 0, k = 0;
  
    while (PEDC == 0 && p < DATA->nopools && k == 0) {
        int n = 0;
        attempt_count[p] += 1;

        while (PEDC == 0 && n < DATA->ncdf_data.TIME_INDEX.length && k == 0) {
            // Check current value
            double current_val = DATA->M_POOLS[p + n * DATA->nopools];

            if (current_val < E.min_val[p] || current_val > E.max_val[p]) {
                PEDC = -INFINITY;
                k = -1;
            }
            n += 1;
        }
        
        if(k == 0){
            pass_count[p] += 1;
        }
        p += 1;
    }

    // Print every 2000th call
    if (total_calls % 2000 == 0) {
        printf("\n--- Diagnostic at iteration %i ---\n", total_calls);
        for (p = 0; p < DATA->nopools; p++){   
            if (attempt_count[p] > 0) {
                printf("pool %i; Attempt = %i, Pass = %i, Percent = %2.2f%%\n",
                       p, attempt_count[p], pass_count[p], 
                       100.0 * (double)pass_count[p] / (double)attempt_count[p]);
            }
        }

        // --- START NEW SCROLLING CSV BLOCK ---
        // We look at Pool 16 (L1 Temp) and Pool 17 (L2 Temp) specifically
        int log_pools[] = {16}; 
        for (int i = 0; i < 2; i++) {
            int target_p = log_pools[i];
            printf("\nPool %i CSV: ", target_p);
            for (int t = 0; t < DATA->ncdf_data.TIME_INDEX.length; t++) {
                double val = DATA->M_POOLS[target_p + t * DATA->nopools];
                
                // Stop printing if we hit a NaN or an obvious explosion to keep screen clean
                // but still print enough to see the failure.
                printf("%2.1f%s", val, (t == DATA->ncdf_data.TIME_INDEX.length - 1) ? "" : ",");
                
                if (!isfinite(val) || val > 1000.0 || val < -200.0) {
                    printf(" [EXPLODED at t=%i]", t);
                    break; 
                }
            }
        }
        printf("\n------------------------------------\n");
        // --- END NEW SCROLLING CSV BLOCK ---
    }
  
    return PEDC;
}