double DALEC_LIKELIHOOD_POOLINT(DATA D){


double tot_exp=0, P=0, mlai=0;
int n,dn;
double Pstart=0;
double Pend=0;


/*Extract DALEC model here*/
/*Copy model pointer for brevity*/
DALEC *MODEL=(DALEC *)D.MODEL;

double *POOLS=D.M_POOLS;
int nodays=D.nodays;

/* printf("Number of days = %i\n",nodays);*/

/*FIREBUCKET*/
int nopools=MODEL->nopools;




/*Continuous function to relate initial pool conditions to end pool conditions state*/
/*For future reference: average of model LAI at t and t+1 likely more appropriate*/
/*
double *LAIA=calloc(D.nodays, sizeof(double));
*/
/*Step 2. calculate t<->t+1 LAI*/
/*for (n=0;n<D.nodays;n++){LAIA[n]=}
*/

/*Tolerance of beginning and end tolerance*/
double pooltol=5;

/*END STATE OF POOLS - ALL INITIAL POOLS MUST CONFORM*/
/*Using EDC spots 50 - 56, G. Quetin addition 09.30.2019*/
for (n=0;n<nopools;n++){
    /*start and end pools*/
    Pstart=POOLS[n];
    Pend=POOLS[nopools*(nodays-11)+n];
    
    /*
    printf("Pool Number = %2.2f\n",n);
    printf("Pstart = %2.2f\n",Pstart);
    printf("Pend = %2.2f\n",Pend);
    */

 
    P=P-0.5*pow((log(Pstart/Pend)/log(pooltol)),2);
        
        
        }



return P;
}
