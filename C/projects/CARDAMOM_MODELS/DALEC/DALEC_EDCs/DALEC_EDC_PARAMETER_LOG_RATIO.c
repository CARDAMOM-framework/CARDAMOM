

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********
typedef struct {
    int numerator_index;
    int denominator_index;
    double mean_ratio;
    double std_ratio;
    }DALEC_EDC_PARAMETER_LOG_RATIO_STRUCT;


//General inequality function
double DALEC_EDC_PARAMETER_LOG_RATIO(DATA * DATA, void * EDCstruct){
   

    //Reading by casting void pointer to "DALEC_EDC_PARS_INEQUALITY_STRUCT" format
    DALEC_EDC_PARAMETER_LOG_RATIO_STRUCT  E = *(DALEC_EDC_PARAMETER_LOG_RATIO_STRUCT * ) EDCstruct;

    double mod_ratio = log(DATA->M_PARS[E.numerator_index])/log(DATA->M_PARS[E.denominator_index]);
    double residual = (mod_ratio - E.mean_ratio)/E.std_ratio;
    double PEDC=-0.5*residual*residual;

    
    return PEDC;
    

}