

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********
typedef struct {
    int big_par_index;
    int small_par_index;
    }DALEC_EDC_PARAMETER_INEQUALITY_STRUCT;


//General inequality function
double DALEC_EDC_PARAMETER_INEQUALITY(DATA * DATA, void * EDCstruct){
   
    double PEDC = 0;

    //Reading by casting void pointer to "DALEC_EDC_PARS_INEQUALITY_STRUCT" format
   DALEC_EDC_PARAMETER_INEQUALITY_STRUCT  E = *(DALEC_EDC_PARAMETER_INEQUALITY_STRUCT * ) EDCstruct;
    //Checking inequality

    double bigp=DATA->M_PARS[E.big_par_index];
    double smallp=DATA->M_PARS[E.small_par_index];
    //ratio should be <1
    double rr = smallp/bigp;
    //assumes parameters are the same sign
    if (rr>1){
        //%Steep dropoff mitigates return
        PEDC = -1e6 - 1e6*rr;
    } ;
    

    return PEDC;
    

}
