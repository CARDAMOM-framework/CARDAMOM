//*************** CFL Stability EDC ******************
/*Pre-run EDC: Evaluates the Courant–Friedrichs–Lewy condition of heat 
propagation to the unit soil volume under maximally conductive conditions 
to ensure numerical stability of joint soil thermal parameters*/

typedef struct {
    int depth_index;
    int vhc_index;
    int cond_index;
    int porosity_index; // needed to calculate max frozen heat capacity
    double timestep_days;  // usually 1.0
} DALEC_EDC_CFL_STABILITY_STRUCT;

double DALEC_EDC_CFL_STABILITY(DATA * DATA, void * EDCstruct){
    double PEDC = 0;
    DALEC_EDC_CFL_STABILITY_STRUCT E = *(DALEC_EDC_CFL_STABILITY_STRUCT *) EDCstruct;
    
    double depth = DATA->M_PARS[E.depth_index];
    double dry_vhc = DATA->M_PARS[E.vhc_index];
    double cond = DATA->M_PARS[E.cond_index];
    double por = DATA->M_PARS[E.porosity_index];
    
    // Calculate the physically realistic worst-case heat capacity (100% frozen saturation)
    // 1.9e6 is volumetric heat capacity of ice
    double worst_case_cv = dry_vhc + (por * 1900000.0);
    
    // Calculate critical timestep in days
    // dt = (z^2 * Cv) / (2 * k * 86400)
    double dt_crit = (depth * depth * worst_case_cv) / (2.0 * cond * 86400.0);
    
    // If the critical timestep is smaller than the model timestep, math will explode
    if (dt_crit < E.timestep_days) {
        PEDC = -INFINITY;
    }
    
    return PEDC;
}