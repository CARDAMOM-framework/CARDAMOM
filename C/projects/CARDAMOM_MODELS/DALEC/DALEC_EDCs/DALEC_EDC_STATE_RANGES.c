// static int attempt_count[100];
// static int pass_count[100];


typedef struct {
       double  * min_val;
    double * max_val; 
}DALEC_EDC_STATE_RANGES_STRUCT;

  
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
        //attempt_count[p]+=1;

while (PEDC==0 & n<DATA->ncdf_data.TIME_INDEX.length  & k==0){

    

         if (DATA->M_POOLS[p+n*DATA->nopools]<E.min_val[p] || DATA->M_POOLS[p+n*DATA->nopools]>E.max_val[p]){PEDC = -INFINITY;k=-1;}


       //if (p == 18){printf("E.max_val[p] = %2.2f, E.min_val[p] = %2.2f, SM = %2.2f\n",E.max_val[p], E.min_val[p],DATA->M_POOLS[p+n*DATA->nopools]);}

  n+=1;}
            //if(k==0){pass_count[p]+=1;}

 
  p+=1;

}
//     if (k==0){
//         for (p=0;p<DATA->nopools;p++){   
//         printf("pool %i; Attempt = %i, Pass = %i, Percent = %2.2f\%\n",p,attempt_count[p], pass_count[p], 100*(double)pass_count[p]/(double)attempt_count[p]);
//         }}
       //printf("Fail check = %2.2i\n",p);


  
    return PEDC;
}

    
    
    