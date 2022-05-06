typedef struct {
    int nopools
       double min_val;
    double max_val;
    int }DALEC_EDC_STATE_RANGES_STRUCT;

    

            
    
//General inequality function
double DALEC_EDC_STATE_RANGES(DATA * DATA, void * EDCstruct){
    
    double PEDC = 0;
    //Reading by casting void pointer to "DALEC_EDC_PARS_INEQUALITY_STRUCT" format
   DALEC_EDC_PARAMETER_INEQUALITY_STRUCT  E = *(DALEC_EDC_PARAMETER_INEQUALITY_STRUCT * ) EDCstruct;
    //Checking inequality
    if (DATA->M_PARS[E.big_par_index]<DATA->M_PARS[E.small_par_index]){PEDC = -INFINITY;} ;
    
    return PEDC;
}

    
    
    