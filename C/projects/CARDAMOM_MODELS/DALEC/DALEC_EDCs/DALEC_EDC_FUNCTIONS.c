//List functions here
#pragma once
//#include "DALEC_EDC_PARS_INEQUALITY.c"
#include "DALEC_EDC_PARAMETER_INEQUALITY.c"


typedef struct {
double (*function) (DATA * , void *);
     void *    data;
     bool prerun;
}EDCs;













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




    
    
    
    



