int DALEC_OBSOPE_GPP(DATA D, int gpp_flux){

//Time varying GPP 
if (D.ngpp>0){int n;for (n=0;n<D.nodays;n++){D.M_GPP[n]=D.M_FLUXES[D.nofluxes*n+gpp_flux];}};

//Mean GPP 
if (D.otherpriors[5]>-9999){int n;D.M_MGPP[0]=0;for (n=0;n<D.nodays;n++){D.M_MGPP[0]+=D.M_FLUXES[n*D.nofluxes+gpp_flux];};D.M_MGPP[0]=D.M_MGPP[0]/(double)D.nodays;}



return 0;}


//Mean fire co2 flux
int DALEC_OBSOPE_FIRE(DATA D, int fire_flux){

if (D.otherpriors[2]>-9999){int n;D.M_MFIRE[0]=0;for (n=0;n<D.nodays;n++){D.M_MFIRE[0]+=D.M_FLUXES[n*D.nofluxes+fire_flux];};D.M_MFIRE[0]=D.M_MFIRE[0]/(double)D.nodays;}


return 0;}





//NBE observation operator
// R
int DALEC_OBSOPE_NBE(DATA D, int * nbe_fluxes, int * nbe_flux_signs, int n_nbe_fluxes){
  
if (D.nnbe>0){
int n,nn;
for (n=0;n<D.nodays;n++){
D.M_NBE[n]=0;
for (nn=0;nn<n_nbe_fluxes;nn++){
D.M_NBE[n]+=D.M_FLUXES[D.nofluxes*n+nbe_fluxes[nn]]*nbe_flux_signs[nn];}}};

return 0;}

//ET observation operator, assuming one flux
int DALEC_OBSOPE_ET(DATA D, int et_flux){
  
if (D.net>0){int n;for (n=0;n<D.nodays;n++){D.M_ET[n]=D.M_FLUXES[D.nofluxes*n+et_flux];}};

return 0;}


//LAI observation operator
int DALEC_OBSOPE_LAI(DATA D, int folc_pool,double lcma){

if (D.otherpriors[4]>0 | D.nlai>0){int n;for (n=0;n<D.nodays;n++){D.M_LAI[n]=(D.M_POOLS[D.nopools*n+folc_pool]+D.M_POOLS[D.nopools*(n+1)+folc_pool])*0.5/lcma;}};


return 0;}


//GRACE H2O timeseries
int DALEC_OBSOPE_GRACE_EWT(DATA D, int * h2o_pools, int n_h2o_pools){

if (D.newt>0){int n,nn;for (n=0;n<D.nodays;n++){D.M_EWT[n]=0;
for (nn=0;nn<n_h2o_pools;nn++){
D.M_EWT[n]+=(D.M_POOLS[D.nopools*n+nn] +D.M_POOLS[D.nopools*(n+1)+nn])*0.5;}}}
return 0;}



int DALEC_OBSOPE_ABGB(DATA D, int * abgb_pools, int n_abgb_pools){



if (D.otherpriors[0]>-9999){

int nn;
D.M_ABGB_t0[0]=0;
for (nn=0;nn<n_abgb_pools;nn++){D.M_ABGB_t0[0]+=D.M_POOLS[abgb_pools[nn]];}
}


if (D.nabgb>0){
//declarations
int p,nxp,n,nn;

for (n=0;n<D.nodays;n++){
//indices
nxp=D.nopools*(n+1);p=D.nopools*n;

//inialize
D.M_ABGB[n]=0;
//loop through pools
for (nn=0;nn<n_abgb_pools;nn++){
D.M_ABGB[n]+=(D.M_POOLS[p+abgb_pools[nn]]+D.M_POOLS[nxp+abgb_pools[nn]])*0.5;}

}}


return 0;
}







int DALEC_OBSOPE_SOM(DATA D, int * som_pools, int n_som_pools){



if (D.nsom>0){for (n=0;n<D.nodays;n++){
//declarations
int p,nxp,n,nn;

//indices
nxp=D.nopools*(n+1);p=D.nopools*n;

//inialize
D.M_SOM[n]=0;
//loop through pools
for (nn=0;nn<n_som_pools;nn++){
D.M_SOM[n]+=(D.M_POOLS[p+som_pools[nn]]+D.M_POOLS[nxp+som_pools[nn]])*0.5;}

}}


return 0;
}








//













