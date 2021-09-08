#pragma once
#include "../CARDAMOM_GENERAL/NETCDF_AUXILLIARY_FUNCTIONS.c"
#include "../../math_fun/max.c"

//#include "../CARDAMOM_GENERAL/CARDAMOM_READ_NETCDF_DATA.c"



typedef struct TIMESERIES_OBS_STRUCT{
size_t length;
double * values;
size_t unc_length;
double * unc;
int opt_unc_type;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
int opt_normalization;//(0 = none, 1 = remove mean, 2 = divide by mean)
int opt_filter;//(0 = no filter, 1 = mean only, 2==annual mean & monthly anomaly, 3 = seasonal cycle & inter-annual anomalies). 
int opt_structural_error;
double min_threshold_value;
double single_monthly_unc;
double single_annual_unc;
double single_mean_unc;
double single_unc;
double structural_unc;
//expand as needed
int valid_obs_length;//number of non-empty obs
int * valid_obs_indices;//indices of non-empty obs
}TIMESERIES_OBS_STRUCT;



typedef struct SINGLE_OBS_STRUCT{
double value;
double unc;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
int opt_unc_type;//log-transform data 
double min_threshold_value;
}SINGLE_OBS_STRUCT;








double TIMESERIES_OBS_STRUCT_PREPROCESS(TIMESERIES_OBS_STRUCT * OBS){



//OBS_STRUCT_DEFAULT_VALUES(OBS);

//Loop through obs and find valid obs
int n,N=(int)OBS->length;

OBS->valid_obs_length=0;

for (n=0;n<N;n++){if (OBS->values[n]!=-9999){OBS->valid_obs_length=OBS->valid_obs_length+1;}}

OBS->valid_obs_indices=calloc(OBS->valid_obs_length,sizeof(int));

int k=0;

for (n=0;n<N;n++){if (OBS->values[n]!=-9999){OBS->valid_obs_indices[k]=n;k=k+1;}}



//Populate uncertainty if no values are provided
for (k=0;k<OBS->valid_obs_length;k++){
if (OBS->unc[OBS->valid_obs_indices[k]]==-9999){

if (OBS->opt_unc_type<2){
OBS->unc[OBS->valid_obs_indices[k]]=OBS->single_unc;}

else if (OBS->opt_unc_type==2){
OBS->unc[OBS->valid_obs_indices[k]]=OBS->single_unc*OBS->values[k];}


}}

return 0;


}





//Function for reading these
TIMESERIES_OBS_STRUCT  READ_NETCDF_TIMESERIES_OBS_FIELDS(int ncid, char * OBSNAME){
TIMESERIES_OBS_STRUCT OBS;
printf("Struct declared OK\n");

printf("OBSNAME = %s\n",OBSNAME);


char uncsf[50],OBSunc[50];
strcpy(OBSunc,OBSNAME);
strcpy(uncsf,"unc");
strcat(OBSunc,uncsf);

printf("(before read) OBS.opt_unc_type = %i\n",OBS.opt_unc_type);
printf("(before read) OBS.opt_filter = %i\n",OBS.opt_filter);


OBS.unc = ncdf_read_double_var(ncid, OBSunc , &OBS.unc_length);

OBS.values = ncdf_read_double_var(ncid, OBSNAME , &(OBS.length));
OBS.opt_unc_type=ncdf_read_int_attr(ncid, OBSNAME,"opt_unc_type");//absolute, log, percentage
OBS.opt_normalization=ncdf_read_int_attr(ncid, OBSNAME,"opt_normalization");
OBS.opt_filter=ncdf_read_int_attr(ncid, OBSNAME,"opt_filter");
OBS.opt_structural_error=ncdf_read_int_attr(ncid, OBSNAME,"opt_structural_error");
OBS.min_threshold_value=ncdf_read_double_attr(ncid, OBSNAME,"min_threshold_value");
OBS.single_monthly_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_monthly_unc");
OBS.single_annual_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_annual_unc");
OBS.single_mean_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_mean_unc");
OBS.single_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_unc");
OBS.structural_unc=ncdf_read_double_attr(ncid, OBSNAME,"structural_unc");



printf("(after read) OBS.opt_unc_type = %i\n",OBS.opt_unc_type);
printf("(after read) OBS.opt_filter = %i\n",OBS.opt_filter);



//pre-process obs
OBS_STRUCT_PREPROCESS(&OBS);

return OBS;
};




// typedef struct SINGLE_OBS_STRUCT{
// double value;
// double unc;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
// int opt_unc_type;//log-transform data 
// double min_threshold_value;
// }SINGLE_OBS_STRUCT;
// 




//Function for reading these
SINGLE_OBS_STRUCT OBS_STRUCT READ_NETCDF_SINGLE_OBS_FIELDS(int ncid, char * OBSNAME){
SINGLE_OBS_STRUCT OBS;

printf("OBSNAME = %s\n",OBSNAME);


char uncsf[50],OBSunc[50];
strcpy(OBSunc,OBSNAME);
strcpy(uncsf,"unc");
strcat(OBSunc,uncsf);



OBS.unc = ncdf_read_double_var(ncid, OBSunc , &OBS.unc_length);
OBS.value = ncdf_read_double_var(ncid, OBSNAME , &(OBS.length));
OBS.opt_unc_type=ncdf_read_int_attr(ncid, OBSNAME,"opt_unc_type");//absolute, log, percentage
OBS.min_threshold_value=ncdf_read_double_attr(ncid, OBSNAME,"min_threshold_value");


return OBS;
};






double CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(TIMESERIES_OBS_STRUCT * OBS,double * MOD){

/*Data structure, includes model and data*/
/*EWT constraint*/
double tot_exp=0;
int n,m,dn;
/*General notes*/
/* If D.et_annual_unc<1, then ET constraint is occurring on monthly basis*/
/* For log_et_obs*/

/*
typedef struct OBS_STRUCT{
size_t length;
double * values;
double * unc;
int opt_log_transform;//log-transform data 
int opt_normalization;//(0 = none, 1 = remove mean, 2 = divide by mean)
int opt_structural_error;
double min_threshold_value;
double single_monthly_unc;
double single_annual_unc;
double single_mean_unc;
double structural_unc;
//expand as needed
int valid_obs_length;//number of non-empty obs
int * valid_obs_indices;//indices of non-empty obs
}OBS_STRUCT;
*/




int N=OBS->valid_obs_length;
double * mod = calloc(sizeof(double),N);
double * obs = calloc(sizeof(double),N);
double * unc = calloc(sizeof(double),N);





//Copying memory

for (n=0;n<N;n++){
mod[n] = MOD[OBS->valid_obs_indices[n]];
obs[n] = OBS->values[OBS->valid_obs_indices[n]];
unc[n] = OBS->unc[OBS->valid_obs_indices[n]];}

//Setting threshold

if (isinf(OBS->min_threshold_value)==0){
for (n=0;n<N;n++){
mod[n] = max(mod[n],OBS->min_threshold_value);
obs[n] = max(obs[n],OBS->min_threshold_value);}}


double mean_mod=0, mean_obs=0;


//calculation of mean where obs are available
if (OBS->opt_normalization>0 || OBS->opt_filter==1){
for (n=0;n<N;n++){
mean_mod += mod[n];
mean_obs += obs[n];}
mean_mod=mean_mod/(double)N;
mean_obs=mean_obs/(double)N;}




//Normalization
if (OBS->opt_normalization==1){
for (n=0;n<N;n++){
mod[n]=mod[n]-mean_mod;
obs[n]=obs[n]-mean_obs;}
}
else if (OBS->opt_normalization==2){
for (n=0;n<N;n++){
mod[n]=mod[n]/mean_mod;
obs[n]=obs[n]/mean_obs;}
}

//log transform


if (OBS->opt_unc_type==1){
for (n=0;n<N;n++){
mod[n]=log(mod[n]);
obs[n]=log(obs[n]);
unc[n]=log(unc[n]);}}


//Cost function
//This is the only option available for single value (e.g. time invariant) observations
if (OBS->opt_filter==0){//no filter
for (n=0;n<N;n++){tot_exp += pow((mod[n] - obs[n])/unc[n],2);}}

else if (OBS->opt_filter==1){//mean only
tot_exp  = pow((mean_mod - mean_obs)/OBS->single_mean_unc,2);}

else if (OBS->opt_filter==2){//monthly and annual flux
/*Decoupling seasonal from interannual variations*/
/*Only use with monthly resolution fluxes, complete years & no missing data*/
/*Step 1. Mean model & data annual NBE*/
/*Step 2. Compare means*/
/*Step 3. Remove means from months for cost function*/
int m, dn;
for (m=0;m<N/12;m++){
/*Calculate annual mean*/
double mam=0, oam=0;
for (n=0;n<12;n++){dn=n+m*12;mam=mam+mod[dn];oam=oam+obs[dn];}
/*normalize means*/
mam=mam/12;oam=oam/12;
/*Calculate seasonal cost function*/
for (n=0;n<12;n++){dn=n+m*12;tot_exp+=pow((mod[dn]-obs[dn]-mam+oam)/OBS->single_monthly_unc,2);}
/*Calculate annual cost function*/
/*TEST: normalize model likelihood by normal distribution with mean zero and unc = x2 annual unc.*/
tot_exp+=pow((oam-mam)/OBS->single_annual_unc,2);}}

free(mod);free(obs);free(unc);


double P=-0.5*tot_exp;
// printf("Completed likelihood function...P = %2.2f\n",P);
// printf("OBS->opt_filter = %i\n",OBS->opt_filter);
// printf("OBS->opt_unc_type = %i\n",OBS->opt_unc_type);

return P;

}




// typedef struct SINGLE_OBS_STRUCT{
// double value;
// double unc;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
// int opt_unc_type;//log-transform data 
// double min_threshold_value;
// }SINGLE_OBS_STRUCT;
// 




double CARDAMOM_SINGLE_OBS_LIKELIHOOD(SINGLE_OBS_STRUCT * OBS,double MOD){

/*Data structure, includes model and data*/
/*EWT constraint*/
double tot_exp=0;
int n,m,dn;
/*General notes*/
/* If D.et_annual_unc<1, then ET constraint is occurring on monthly basis*/
/* For log_et_obs*/




//Copying memory

double mod = MOD;
double obs = OBS->value;
double unc= OBS->unc;

//Setting threshold

if (isinf(OBS->min_threshold_value)==0){
mod = max(mod,OBS->min_threshold_value);
obs = max(obs,OBS->min_threshold_value);}


if (OBS->opt_unc_type==1){
for (n=0;n<N;n++){
mod=log(mod);
obs=log(obs);
unc=log(unc);}}


//Cost function
//This is the only option available for single value (e.g. time invariant) observations
tot_exp += pow((mod- obs)/unc,2);



double P=-0.5*tot_exp;
// printf("Completed likelihood function...P = %2.2f\n",P);
// printf("OBS->opt_filter = %i\n",OBS->opt_filter);
// printf("OBS->opt_unc_type = %i\n",OBS->opt_unc_type);

return P;

}
