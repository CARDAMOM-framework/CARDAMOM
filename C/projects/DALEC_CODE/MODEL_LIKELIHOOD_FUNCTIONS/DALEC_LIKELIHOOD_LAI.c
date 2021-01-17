double DALEC_LIKELIHOOD_LAI(DATA D){


double tot_exp=0, P=0, mlai=0;
int n,dn;
/*LAI likelihood*/
/*For future reference: average of model LAI at t and t+1 likely more appropriate*/
/*
double *LAIA=calloc(D.nodays, sizeof(double));
*/
/*Step 2. calculate t<->t+1 LAI*/
/*for (n=0;n<D.nodays;n++){LAIA[n]=}
*/


/*D.LAI[dn] = observed LAI at index dn*/
/*D.M_LAI[dn] = modelled LAI at index dn*/
double MLAI;
/*Standard timestep model-data comparison*/
if (D.otherpriors[4]<0 & D.nlai>0){
for (n=0;n<D.nlai;n++){dn=D.laipts[n];tot_exp+=pow(log(D.M_LAI[dn]/D.LAI[dn])/log(2),2);}
//for (n=0;n<D.nlai;n++){dn=D.laipts[n];MLAI = (D.M_LAI[dn]+D.M_LAI[dn+1])/2;tot_exp+=pow(log(MLAI/D.LAI[dn])/log(2),2);}

P=P-0.5*tot_exp;}

/*use timesteps for mean LAI calculation*/
/*Here only using mean LAI*/
else if (D.otherpriors[4]>0){


if (D.nlai>0){
for (n=0;n<D.nlai;n++){dn=D.laipts[n];mlai+=D.M_LAI[dn];}
mlai=mlai/(double)D.nlai;}
else {for (n=0;n<D.nodays;n++){mlai+=D.M_LAI[n];};mlai=mlai/(double)D.nodays;}




if (D.otherpriorunc[4]<0){
P=P-0.5*pow((mlai-D.otherpriors[4])/D.otherpriorunc[4],2);}
else {
P=P-0.5*pow(log(mlai/D.otherpriors[4])/log(D.otherpriorunc[4]),2);}

}


return P;
}
