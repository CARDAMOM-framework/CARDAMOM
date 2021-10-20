double DALEC_LIKELIHOOD_ABGB(DATA D){



double biomass, P=0;
int n,dn;
double tot_exp=0;


/*Mean biomass at time = 0 */
if (D.otherpriors[0]>-9999){
//p=p-0.5*pow(log((PARS[17]+PARS[18]+PARS[19]+PARS[20])/D.otherpriors[0])/log(D.otherpriorunc[0]),2);}
P=P-0.5*pow(log((D.M_ABGB_t0[0])/D.otherpriors[0])/log(D.otherpriorunc[0]),2);}




/**Time resolved biomass**/
if (D.nabgb>0){
/*Looping through all available constraints*/
for (n=0;n<D.nabgb;n++){dn=D.abgbpts[n];
/*biomass = totals of labile, foliar, root, wood*/
//biomass=D.M_POOLS[D.nopools*(dn+1)+0]+D.M_POOLS[D.nopools*(dn+1)+1]+D.M_POOLS[D.nopools*(dn+1)+2]+D.M_POOLS[D.nopools*(dn+1)+3];
biomass=D.M_ABGB[dn];

/*Model-data mismatch*/
tot_exp+=pow(log(biomass/D.ABGB[dn])/log(D.otherpriorunc[1]),2);}


//tot_exp+=pow(log(max(biomass,0.01)/max(D.WOO[dn],0.01))/log(max(D.otherpriorunc[1],0.1)),2);} /*shuang for correcting -inf*/
/*adding cost to overall probability estimate*/
P=P-0.5*tot_exp;
}


return P;

}
