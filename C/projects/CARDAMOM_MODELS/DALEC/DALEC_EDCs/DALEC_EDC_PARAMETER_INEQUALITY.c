

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********
typedef struct {
    int big_par_index;
    int small_par_index;}DALEC_EDC_PARAMETER_INEQUALITY_STRUCT;


//General inequality function
double DALEC_EDC_PARAMETER_INEQUALITY(DATA * DATA, void * EDCstruct){
   
    double PEDC = 0;
        printf("******************\n");
    printf("******************\n");
    printf("IN EDC INEQUALITY\n");
    printf("******************\n");
    printf("******************\n");
    //Reading by casting void pointer to "DALEC_EDC_PARS_INEQUALITY_STRUCT" format
   DALEC_EDC_PARAMETER_INEQUALITY_STRUCT  E = *(DALEC_EDC_PARAMETER_INEQUALITY_STRUCT * ) EDCstruct;
    //Checking inequality
        
   
    if (DATA->M_PARS[E.big_par_index]<DATA->M_PARS[E.small_par_index]){PEDC = -INFINITY;} ;
    
        printf("******************\n");
    printf("******************\n");
    printf("Done in EDC inequality\n");
    printf("******************\n");
    printf("******************\n");
    
    return PEDC;
    

}