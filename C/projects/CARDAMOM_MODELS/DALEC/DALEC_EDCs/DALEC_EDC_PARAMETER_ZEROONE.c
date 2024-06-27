

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_ZEROONE*********
typedef struct {
    int parone;
    int partwo;
    int parthree;}DALEC_EDC_PARAMETER_ZEROONE_STRUCT;


//General inequality function
double DALEC_EDC_PARAMETER_ZEROONE(DATA * DATA, void * EDCstruct){
   
    double PEDC = 0;

    //Reading by casting void pointer to "DALEC_EDC_PARS_INEQUALITY_STRUCT" format
   DALEC_EDC_PARAMETER_ZEROONE_STRUCT  E = *(DALEC_EDC_PARAMETER_ZEROONE_STRUCT * ) EDCstruct;
    //Checking inequality

   
    if (DATA->M_PARS[E.parone]+DATA->M_PARS[E.partwo]+DATA->M_PARS[E.parthree] >= 1 ){
        PEDC = -INFINITY;
    } ;
    

    
    return PEDC;
    

}

