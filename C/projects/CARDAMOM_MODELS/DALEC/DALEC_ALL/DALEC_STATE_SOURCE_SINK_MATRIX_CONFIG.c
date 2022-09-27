
  
            
int DALEC_STATE_SOURCE_SINK_MATRIX_CONFIG(DALEC * DALEC){
    

printf("********\n");
printf("********\n");
printf("****SO FAR SO GOOD DALEC_STATE_SOURCE_SINK_MATRIX_CONFIG start****\n");
printf("********\n");
printf("********\n");
printf("DALEC->nopools = %2i\n",DALEC->nopools);


     int nopools=DALEC->nopools;
     int nofluxes=DALEC->nofluxes;
     DALEC_FLUX_SOURCE_SINK_MATRIX FIOMATRIX=DALEC->FIOMATRIX;
     int f,s;
     
    
//static EDCs * EDCs;EDCs=calloc(DALECmodel->noedcs,sizeof( * EDCs));

         DALEC_STATE_SOURCE_SINK_MATRIX * SIOMATRIX;



SIOMATRIX=calloc(nopools,sizeof(DALEC_STATE_SOURCE_SINK_MATRIX));
         

     
         
     
         for (s=0;s<nopools;s++){SIOMATRIX[s].N_STATE_INPUT_FLUXES=0;SIOMATRIX[s].N_STATE_OUTPUT_FLUXES=0;}

         //Step 1. Count inputs and outputs
     for (f=0;f<nofluxes;f++){
         if (FIOMATRIX.SOURCE[f]!=-1){SIOMATRIX[FIOMATRIX.SOURCE[f]].N_STATE_OUTPUT_FLUXES+=1;}
         if (FIOMATRIX.SINK[f]!=-1){SIOMATRIX[FIOMATRIX.SINK[f]].N_STATE_INPUT_FLUXES+=1;}}

         //Step 2. Allocate
                     for (s=0;s<nopools;s++){
                    SIOMATRIX[s].STATE_INPUT_FLUXES=calloc(SIOMATRIX[s].N_STATE_INPUT_FLUXES, sizeof(int));
                     SIOMATRIX[s].STATE_OUTPUT_FLUXES=calloc(SIOMATRIX[s].N_STATE_OUTPUT_FLUXES, sizeof(int));}
                     
        //Step 3. define
    for (s=0;s<nopools;s++){
            int ki=0, ko=0;
        for (f=0;f<nofluxes;f++){
            if (FIOMATRIX.SOURCE[f]==s){SIOMATRIX[s].STATE_OUTPUT_FLUXES[ki]=f;ki+=1;}
            if (FIOMATRIX.SINK[f]==s){SIOMATRIX[s].STATE_INPUT_FLUXES[ko]=f;ko+=1;}
        }}
    
    
          DALEC->SIOMATRIX= SIOMATRIX;
                     //Step 3. 
  
          

    return 0;
    
}