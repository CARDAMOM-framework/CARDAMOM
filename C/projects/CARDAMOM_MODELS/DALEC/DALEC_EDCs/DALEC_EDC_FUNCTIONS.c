//List functions here


#pragma once
#include "DALEC_EDC_PARAMETER_INEQUALITY.c"
#include "DALEC_EDC_STATE_RANGES.c"
#include "DALEC_EDC_TRAJECTORY.c"
#include "DALEC_EDC_START_TEMP.c"
#include "DALEC_EDC_START_SM.c"
#include "../DALEC_ALL/DALEC_MODULE.c"







//
double RUN_DALEC_EDCs(DATA * DATA, EDCs * EDCs, bool prerun){
    
    //Step 0; back out EDC structure.
    //Continue from here: need to probably allow for EDCs to be standalone DATA field to allow for more versatile code(maybe) later on


    int n=0;



    double P=0;
    for (n=0;n<DATA->noedcs;n++){
              

        //Run function
        if (EDCs[n].prerun==prerun){    

//Add EDC.diagnostic_mode = 1 here
            if (P>-INFINITY){
            DATA->M_EDCs[n]=EDCs[n].function(DATA, EDCs[n].data);
            //Add all probs up
        P+=DATA->M_EDCs[n];
                //printf("Inside run EDCs: DATA->M_EDCs[%i] = %2.2f\n",n,DATA->M_EDCs[n] );
            
            
            }
        
        
        }}



    


    
    return P;

        
        
    }




    
    
    
    



