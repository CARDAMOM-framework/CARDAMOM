double DALEC_LIKELIHOOD_NBE(DATA D){



int dn, n,m;
double am,mam,P=0;
/*NBE likelyhood*/
if (D.nnbe>0){
double tot_exp=0;
if (D.nbe_annual_unc<0){

if (D.nnbeunc==0){
/*Standard NBE likelihood approach*/
for (n=0;n<D.nnbe;n++){dn=D.nbepts[n];tot_exp+=pow((D.M_NBE[dn]-D.NBE[dn])/D.nbe_obs_unc,2);}}


if (D.nnbeunc>0){
/*Standard NBE likelihood approach*/
for (n=0;n<D.nnbe;n++){dn=D.nbepts[n];tot_exp+=pow((D.M_NBE[dn]-D.NBE[dn])/D.NBEunc[dn],2);}}

}
else{
/*Decoupling seasonal from interannual variations*/
/*Only use with monthly resolution fluxes, complete years & no missing data*/
/*Step 1. Mean model & data annual NBE*/
/*Step 2. Compare means*/
/*Step 3. Remove means from months for cost function*/
for (m=0;m<D.nnbe/12;m++){
/*Calculate annual mean*/
mam=0;am=0;
for (n=0;n<12;n++){dn=D.nbepts[n+m*12];mam=mam+D.M_NBE[dn];am=am+D.NBE[dn];}
/*normalize means*/
mam=mam/12;am=am/12;
/*Calculate seasonal cost function*/
for (n=0;n<12;n++){dn=D.nbepts[n+m*12];tot_exp+=pow((D.M_NBE[dn]-D.NBE[dn]-mam+am)/D.nbe_obs_unc,2);}
/*Calculate annual cost function*/
/*TEST: normalize model likelihood by normal distribution with mean zero and unc = x2 annual unc.*/
tot_exp+=pow((am-mam)/D.nbe_annual_unc,2);

}}
P=P-0.5*tot_exp;}



return P;
}
