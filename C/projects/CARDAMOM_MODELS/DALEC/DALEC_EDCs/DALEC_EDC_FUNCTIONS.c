//List functions here
#pragma once


typedef struct {
double (*function) (DATA * , void *);
     void *    data;
     bool prerun;
}EDCs;





//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********
typedef struct {
    int big_par_index;
    int small_par_index;}DALEC_EDC_PARS_INEQUALITY_STRUCT;


//General inequality function
double DALEC_EDC_PARS_INEQUALITY(DATA * DATA, void * EDCstruct){
    
    double PEDC = 0;
    //Reading by casting void pointer to "DALEC_EDC_PARS_INEQUALITY_STRUCT" format
   DALEC_EDC_PARS_INEQUALITY_STRUCT  E = *(DALEC_EDC_PARS_INEQUALITY_STRUCT * ) EDCstruct;
    //Checking inequality
    if (DATA->M_PARS[E.big_par_index]<DATA->M_PARS[E.small_par_index]){PEDC = -INFINITY;} ;
    
    return PEDC;
}







//
double RUN_DALEC_EDCs(DATA * DATA, EDCs * EDCs, bool prerun){
    
    //Step 0; back out EDC structure.
    //Continue from here: need to probably allow for EDCs to be standalone DATA field to allow for more versatile code(maybe) later on


    
    
    int n=0;
    double P=0;
    for (n=0;n<DATA->noedcs;n++){
        //Run function
        if (EDCs[n].prerun==prerun){
            DATA->M_EDCs[n]=EDCs[n].function(DATA, EDCs[n].data);}}    

    
    //Add all probs up
    for (n=0;n<DATA->noedcs;n++){P+=DATA->M_EDCs[n];}
    
    
    return P;

        
        
    }




    
    
    
    



