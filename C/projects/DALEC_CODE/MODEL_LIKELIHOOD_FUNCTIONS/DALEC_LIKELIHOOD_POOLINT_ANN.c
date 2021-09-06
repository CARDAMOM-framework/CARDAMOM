double DALEC_LIKELIHOOD_POOLINT_ANN(DATA D){

/* initial variables */
int dn,n,mstart,mend,pn;
double tot_exp=0, P=0;
double pool_annualmean, Pstart, Pend;


/*Extract DALEC model here*/
DALEC *MODEL=(DALEC *)D.MODEL; /*Copy model pointer for brevity*/
double *POOLS=D.M_POOLS;
int nodays=D.nodays;

/*FIREBUCKET*/
int nopools=MODEL->nopools;


/*Continuous function to relate initial year pool conditions to end year pool conditions state*/
/*This constraint serves as a loose bound towards no long term source*/

/*Tolerance of beginning and end tolerance*/
double pooltol=1.1; /*tried 2, 5*/

/*END STATE OF POOLS - ALL INITIAL POOLS MUST CONFORM*/
/*Using EDC spots 50 - 56, G. Quetin addition 09.30.2019*/
/*Update to do annual means for beginning and end on 08/20/2021 G. Quetin*/
/* m = year index */
/* n = day index */
/* pn = pool index */

/* loop through pools */
for (pn=0;pn<nopools;pn++){
    
	/*Calculate annual mean of start */
	mstart=0; /* the first year */
	Pstart=0; /* initialize annual sum of first year */

	/* Loop through months of the first year, summing pool */
	for (n=0;n<12;n++){Pstart=Pstart+POOLS[nopools*(n+mstart*12)+pn];}
	/*normalize by month for means*/
	Pstart=Pstart/12;
	
	/*Calculate annual mean of the end */
	mend=(nodays/12)-1; /* the last year */
	Pend=0; /* initialize annual sum of last year */

	/* Loop through months of the last year, summing pool */
	for (n=0;n<12;n++){Pend=Pend+POOLS[nopools*(n+mend*12)+pn];}
	/*normalize by month for means*/
	Pend=Pend/12;
	
        /* calculate the difference between the start and end */
    	tot_exp+=pow((log(Pstart/Pend)/log(pooltol)),2);
	
	/*
 	printf("Pool Number = %i\n",pn);
	printf("end year = %i\n", mend);
	printf("Pstart = %2.2f\n",Pstart);
	printf("Pend = %2.2f\n",Pend);
	printf("Total P for pn+  = %2.2f\n",tot_exp);
	*/

	}

P = P - 0.5*tot_exp;

return P;
}
