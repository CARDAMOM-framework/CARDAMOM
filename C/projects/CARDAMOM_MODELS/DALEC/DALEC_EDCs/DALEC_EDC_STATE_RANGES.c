typedef struct {
       double  * min_val;
    double * max_val; }DALEC_EDC_STATE_RANGES_STRUCT;

  
    
//General inequality function
double DALEC_EDC_STATE_RANGES(DATA * DATA, void * EDCstruct){
    
    
    double PEDC = 0;
    //Reading by casting void pointer to "DALEC_EDC_PARS_INEQUALITY_STRUCT" format
   DALEC_EDC_STATE_RANGES_STRUCT  E = *(DALEC_EDC_STATE_RANGES_STRUCT * ) EDCstruct;
    //Checking range
   
   //loop through all timesteps
  int p=0,k=0;
  
while (p<DATA->nopools & k==0){
            //looping through timesteps
    int n=0;
while (n<DATA->ncdf_data.TIME_INDEX.length  & k==0){
    if (p==18){
        
    printf("E.min_val[%i] = %2.2f\n",p,E.min_val[p]);
    printf("E.max_val[%i] = %2.2f\n",p,E.max_val[p]);
    printf("DATA->M_POOLS[p+n*DATA->nopools] = %2.2f\n",DATA->M_POOLS[p+n*DATA->nopools]);}
    
    
         if (DATA->M_POOLS[p+n*DATA->nopools]<E.min_val[p] || DATA->M_POOLS[p+n*DATA->nopools]>E.max_val[p]){PEDC = -INFINITY;k=-1;}
          

  n+=1;}

  p+=1;}
  

  
    return PEDC;
}

    
    
    