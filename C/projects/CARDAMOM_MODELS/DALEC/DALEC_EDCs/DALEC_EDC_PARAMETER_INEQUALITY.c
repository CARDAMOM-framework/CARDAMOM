

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********
typedef struct {
    int big_par_index;      // Set to -1 if comparing a fixed number
    double big_par_value;   // The fixed number (used if big_par_index == -1)
    
    int small_par_index;    // Set to -1 if comparing a fixed number
    double small_par_value; // The fixed number (used if small_par_index == -1)
} DALEC_EDC_PARAMETER_INEQUALITY_STRUCT;


//General inequality function
double DALEC_EDC_PARAMETER_INEQUALITY(DATA * DATA, void * EDCstruct){
   
    double PEDC = 0;

    // Reading by casting void pointer
    DALEC_EDC_PARAMETER_INEQUALITY_STRUCT E = *(DALEC_EDC_PARAMETER_INEQUALITY_STRUCT *) EDCstruct;
    
    double big_val, small_val;

    // Resolve the "big" side of the inequality
    if (E.big_par_index >= 0) {
        big_val = DATA->M_PARS[E.big_par_index];
    } else {
        big_val = E.big_par_value;
    }

    // Resolve the "small" side of the inequality
    if (E.small_par_index >= 0) {
        small_val = DATA->M_PARS[E.small_par_index];
    } else {
        small_val = E.small_par_value;
    }

    // Checking inequality
    if (big_val < small_val) {
        PEDC = -INFINITY;
    }
    
    return PEDC;
}