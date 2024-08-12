double DALEC_LIKELIHOOD_FIRE(DATA D){



double P=0;
int n,dn;
double tot_exp=0;



/*Constrain fire emissions here*/
if (D.otherpriors[2]>-9999){
/*Step 1. Sum fire emissions*/
for (n=0;n<D.nodays;n++){//mfire+=D.M_FLUXES[n*D.nofluxes+16];}
/*Normalize fire constraint to daily mean flux*/
//mfire=mfire/((double)D.nodays);

/*Step 2. Constrain against fire emissions*/
if (D.otherpriorunc[2]>0){P=P-0.5*pow(log(D.M_MFIRE[0]/D.otherpriors[2])/log(D.otherpriorunc[2]),2);}
else {P=P-0.5*pow((D.M_MFIRE[0]-D.otherpriors[2])/D.otherpriorunc[2],2);}
}
}














return P;

}
