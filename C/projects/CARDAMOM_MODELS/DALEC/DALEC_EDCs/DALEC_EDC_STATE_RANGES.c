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
  
while (PEDC==0 & p<DATA->nopools & k==0 ){
            //looping through timesteps
    int n=0;
while (PEDC==0 & n<DATA->ncdf_data.TIME_INDEX.length  & k==0){

    
    
         if (DATA->M_POOLS[p+n*DATA->nopools]<E.min_val[p] || DATA->M_POOLS[p+n*DATA->nopools]>E.max_val[p]){PEDC = -INFINITY;k=-1;}
       //if (p == 18){printf("E.max_val[p] = %2.2f, E.min_val[p] = %2.2f, SM = %2.2f\n",E.max_val[p], E.min_val[p],DATA->M_POOLS[p+n*DATA->nopools]);}

// if (k==-1){
//             printf("Pool Fail check = %i\n",p);
//             printf("Pool fail timestep = %i\n",n);
// 
//           printf("DATA->M_POOLS[p+n*DATA->nopools] = %2.2f\n",DATA->M_POOLS[p+n*DATA->nopools]);
//           printf("E.min_val[p] = %2.2f\n",E.min_val[p]);
//           printf("E.max_val[p] = %2.2f\n",E.max_val[p]);}

  n+=1;}



 
  p+=1;

}


  
    return PEDC;
}

    
    
    