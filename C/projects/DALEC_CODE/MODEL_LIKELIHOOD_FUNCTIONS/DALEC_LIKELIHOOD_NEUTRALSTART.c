double DALEC_LIKELIHOOD_NEUTRALSTART(DATA D){

/* initial variables */
int n;
double tot_exp=0, P=0;
int nodays=D.nodays;

/* Neutral start for NBE */
double nee_start = 0;
double nee_start_condition = 0.000001;
int years_of_start = 10;
int days_of_start;
days_of_start = years_of_start*12;

/*Tolerance on neutrality*/
double nee_start_tol=1;

/*Continuous function to constrain starting years to zero source*/
/*This constraint serves as a loose bound towards no long term source*/


/* Loop over first years for zero */


/*Calculate mean of start */
        
/* Loop through months to sum */
for (n=0;n<days_of_start;n++){nee_start=nee_start+D.M_NEE[n];}
/*normalize by month for means*/
nee_start=nee_start/days_of_start;

if (nee_start == 0){nee_start=.000001;}
        
/* calculate the difference between the start and end */
tot_exp=pow((nee_start_condition-nee_start)/nee_start_tol,2);
/*tot_exp=pow((log(nee_start_condition/nee_start)/log(nee_start_tol)),2);*/

/*
 * printf("day number = %i\n",n);
 * printf("end year = %i\n", mend);
 * */

        

P = P - 0.5*tot_exp;

return P;
}

