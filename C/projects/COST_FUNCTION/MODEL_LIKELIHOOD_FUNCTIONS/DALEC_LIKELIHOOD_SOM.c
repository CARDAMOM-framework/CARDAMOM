double DALEC_LIKELIHOOD_SOM(DATA D){



double som, P=0;
int n,dn;
double tot_exp=0;

tot_exp=0;
if (D.nsom>0){
/*Looping through all available constraints*/
for (n=0;n<D.nsom;n++){dn=D.sompts[n];
/*biomass = totals of litter and som*/
//som=D.M_POOLS[D.nopools*(dn+1)+4]+D.M_POOLS[D.nopools*(dn+1)+5];
som=D.M_SOM[dn];
/*Model-data mismatch*/
tot_exp+=pow(log(som/D.SOM[dn])/log(D.otherpriorunc[7]),2);}
//tot_exp+=pow(log(max(som,0.1)/max(D.SOM[dn],0.1))/log(max(D.otherpriorunc[7],100)),2);} /*shuang for correcting -inf*/
/*adding cost to overall probability estimate*/
P=P-0.5*tot_exp;
}




return P;

}
