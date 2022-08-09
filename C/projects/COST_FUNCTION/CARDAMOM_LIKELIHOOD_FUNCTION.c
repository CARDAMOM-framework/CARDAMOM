#pragma once
#include "../CARDAMOM_GENERAL/NETCDF_AUXILLIARY_FUNCTIONS.c"
#include "../../math_fun/max.c"
#include "../CARDAMOM_GENERAL/CARDAMOM_NETCDF_DATA_STRUCTURE.c"
#include <stdbool.h>

//#include "../CARDAMOM_GENERAL/CARDAMOM_READ_NETCDF_DATA.c"


typedef struct TIMESERIES_OBS_STRUCT{
double * values;//Timeseries of observation values
//**********Variable attributes, as provided in netcf input file*************
int opt_unc_type;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
int opt_normalization;//(0 = none, 1 = remove mean, 2 = divide by mean)
int opt_filter;//(0 = no filter, 1 = mean only, 2==annual mean & monthly anomaly, 3 = annual only). 
double min_threshold;//Minimum value threshold: model and/or data will be rounded up to this value (default = -inf)
double single_monthly_unc;//Fields to be used only with Filter=2 AND opt_unc_type=0;
double single_annual_unc;//Fields to be used only with Filters=2 & 3 (AND opt_unc_type=0 or opt_unc_type=2 for filter = 2);
double single_mean_unc;//Fields to be used only with Filter = 1;`
double single_unc;//Fields to be used only with Filter = 0 ;
double structural_unc;//this gets added to uncertainty in quadrature.
//****Auxiliary variables, separate from timeseries variable****
double * unc;//Timeseries of uncertainty values
//expand as needed
size_t length;//
size_t unc_length;//
int valid_obs_length;//number of non-empty obs
int * valid_obs_indices;//indices of non-empty obs
bool validobs;//True/false  whether no usable data provided
}TIMESERIES_OBS_STRUCT;



typedef struct SINGLE_OBS_STRUCT{
double value;
double unc;
int opt_unc_type;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
double min_threshold;
double min_value;
double max_value;
bool validobs;//True/false  whether no usable data provided
}SINGLE_OBS_STRUCT;








double TIMESERIES_OBS_STRUCT_PREPROCESS(TIMESERIES_OBS_STRUCT * OBS){
//OBS_STRUCT_DEFAULT_VALUES(OBS);
//Loop through obs and find valid obs
    
    //Global defaults for timeseries obs... these are only defined if they haven't been defined earlier
default_int_value(&OBS->opt_unc_type,0);
default_int_value(&OBS->opt_normalization,0);
default_int_value(&OBS->opt_filter,0);
default_double_value(&OBS->min_threshold,log(0));//minus infinity
default_double_value(&OBS->structural_unc,0);
    
    
int n,N=(int)OBS->length;
OBS->valid_obs_length=0;
for (n=0;n<N;n++){if (OBS->values[n]!=DEFAULT_DOUBLE_VAL){OBS->valid_obs_length=OBS->valid_obs_length+1;}}
OBS->valid_obs_indices=calloc(OBS->valid_obs_length,sizeof(int));
int k=0;
for (n=0;n<N;n++){if (OBS->values[n]!=DEFAULT_DOUBLE_VAL){OBS->valid_obs_indices[k]=n;k=k+1;}}

//Populate uncertainty if no values are provided


for (k=0;k<OBS->valid_obs_length;k++){

    if (OBS->unc[OBS->valid_obs_indices[k]]==DEFAULT_DOUBLE_VAL){
if (OBS->opt_unc_type<2){
OBS->unc[OBS->valid_obs_indices[k]]=OBS->single_unc;}
else if (OBS->opt_unc_type==2){
OBS->unc[OBS->valid_obs_indices[k]]=max(OBS->single_unc*OBS->values[k],OBS->single_unc*OBS->min_threshold);}}}
//Scale uncertainty with structural error
if (OBS->structural_unc==DEFAULT_DOUBLE_VAL){OBS->structural_unc=0;}
//Adding structural error
for (k=0;k<OBS->valid_obs_length;k++){
    OBS->unc[OBS->valid_obs_indices[k]]=sqrt(pow(OBS->unc[OBS->valid_obs_indices[k]],2) + pow(OBS->structural_unc,2));}



//isempty flag
OBS->validobs=false;
if (OBS->valid_obs_length>0){OBS->validobs=true;}


return 0;}





//Function for reading these
TIMESERIES_OBS_STRUCT  READ_NETCDF_TIMESERIES_OBS_FIELDS(int ncid, char * OBSNAME){
TIMESERIES_OBS_STRUCT OBS;
printf("Struct declared OK\n");

printf("OBSNAME = %s\n",OBSNAME);


char uncsf[50],OBSunc[50];
strcpy(OBSunc,OBSNAME);
strcpy(uncsf,"unc");
strcat(OBSunc,uncsf);


OBS.values = ncdf_read_double_var(ncid, OBSNAME , &(OBS.length));
OBS.unc = ncdf_read_double_var(ncid, OBSunc , &(OBS.unc_length));

//Empty array with same dims guaranteed
if (OBS.unc_length==0){
        OBS.unc = ncdf_read_double_var(ncid, OBSNAME , &(OBS.unc_length));
        int n;for (n=0;n<OBS.unc_length;n++){OBS.unc[n]=DEFAULT_DOUBLE_VAL;}}
        
        
        

OBS.opt_unc_type=ncdf_read_int_attr(ncid, OBSNAME,"opt_unc_type");//absolute, log, percentage
OBS.opt_normalization=ncdf_read_int_attr(ncid, OBSNAME,"opt_normalization");
OBS.opt_filter=ncdf_read_int_attr(ncid, OBSNAME,"opt_filter");
OBS.min_threshold=ncdf_read_double_attr(ncid, OBSNAME,"min_threshold");
OBS.single_monthly_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_monthly_unc");
OBS.single_annual_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_annual_unc");
OBS.single_mean_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_mean_unc");
OBS.single_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_unc");
OBS.structural_unc=ncdf_read_double_attr(ncid, OBSNAME,"structural_unc");









return OBS;
}




// typedef struct SINGLE_OBS_STRUCT{
// double value;
// double unc;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
// int opt_unc_type;//log-transform data 
// double min_threshold;
// }SINGLE_OBS_STRUCT;
// 




//Function for reading these
SINGLE_OBS_STRUCT READ_NETCDF_SINGLE_OBS_FIELDS(int ncid, char * OBSNAME){
    
SINGLE_OBS_STRUCT OBS;

printf("OBSNAME = %s\n",OBSNAME);


size_t length;

OBS.value = ncdf_read_single_double_var(ncid, OBSNAME);
if (isnan(OBS.value)){OBS.value=DEFAULT_DOUBLE_VAL;}

OBS.unc = ncdf_read_double_attr(ncid, OBSNAME,"unc");

OBS.opt_unc_type=ncdf_read_int_attr(ncid, OBSNAME,"opt_unc_type");//absolute, log, percentage

OBS.min_threshold=ncdf_read_double_attr(ncid, OBSNAME,"min_threshold");

// //MINMAX LIMITS: OBS.min_value=ncdf_read_double_attr(ncid, OBSNAME,"min_value");
////MINMAX LIMITS: OBS.max_value=ncdf_read_double_attr(ncid, OBSNAME,"max_value");

//MINMAX LIMITS: default_double_value(&OBS->min_value,log(0));//-INFINITY
//MINMAX LIMITS: default_double_value(&OBS->max_value,-log(0));//INFINITY

//isempty flag
OBS.validobs=false;
if(OBS.min_value!=DEFAULT_DOUBLE_VAL){OBS.validobs=true;}
if(OBS.max_value!=DEFAULT_DOUBLE_VAL){OBS.validobs=true;}
if(OBS.value!=DEFAULT_DOUBLE_VAL){OBS.validobs=true;}


return OBS;
}






double CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(TIMESERIES_OBS_STRUCT * OBS,double * MOD){

/*Data structure, includes model and data*/
/*EWT constraint*/
    double P=0;
    if (OBS->valid_obs_length>0){
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
double min_threshold;
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
unc[n] = OBS->unc[OBS->valid_obs_indices[n]];
}
//Filter uncertainty copies
double single_mean_unc=OBS->single_mean_unc;
double single_monthly_unc=OBS->single_monthly_unc;
double single_annual_unc=OBS->single_annual_unc;



double mean_mod=0, mean_obs=0;


//calculation of mean where obs are available
//Only calculate means for opt nomormalization
//Recalculate later for opt_filter=1
if (OBS->opt_normalization>0){
for (n=0;n<N;n++){
mean_mod += mod[n];
mean_obs += obs[n];
}
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

//threshold-- applies after normalization, before log-transform (if selected)
if (isinf(OBS->min_threshold)==0){
for (n=0;n<N;n++){
mod[n] = max(mod[n],OBS->min_threshold);
obs[n] = max(obs[n],OBS->min_threshold);}}


//log transform

if (OBS->opt_unc_type==1){
for (n=0;n<N;n++){
mod[n]=log(mod[n]);
obs[n]=log(obs[n]);
unc[n]=log(unc[n]);}

single_mean_unc=log(single_mean_unc);
single_monthly_unc=log(single_monthly_unc);
single_annual_unc=log(single_annual_unc);

}



//Cost function
//This is the only option available for single value (e.g. time invariant) observations
if (OBS->opt_filter==0){//no filter
    for (n=0;n<N;n++){tot_exp += pow((mod[n] - obs[n])/unc[n],2);}}

else if (OBS->opt_filter==1){//mean only

    double mean_mod_of1=0, mean_obs_of1=0;

    for (n=0;n<N;n++){
mean_mod_of1 += mod[n];
mean_obs_of1 += obs[n];

}
mean_mod_of1=mean_mod_of1/(double)N;
mean_obs_of1=mean_obs_of1/(double)N;


    
    
    tot_exp  = pow((mean_mod_of1 - mean_obs_of1)/single_mean_unc,2);}

else if (OBS->opt_filter==2 |  OBS->opt_filter==3){//monthly and annual flux
    /*Decoupling seasonal from interannual variations*/
    /*Only use with monthly resolution fluxes, complete years & no missing data*/
    /*Step 1. Mean model & data annual NBE*/
    /*Step 2. Compare means*/
    /*Step 3. Remove means from months for cost function*/
    int m, dn;
    //Looping through all years
    for (m=0;m<N/12;m++){
    /*Calculate annual mean of monthly values*/
    double mam=0, oam=0;
    //Looping through all months
    for (n=0;n<12;n++){dn=n+m*12;mam=mam+mod[dn];oam=oam+obs[dn];}
    /*normalize means*/
    mam=mam/12;oam=oam/12;
    /*Calculate seasonal cost function*/
    if (OBS->opt_filter==2){for (n=0;n<12;n++){dn=n+m*12;tot_exp+=pow((mod[dn]-obs[dn]-mam+oam)/single_monthly_unc,2);}}
    /*Calculate annual cost function*/
    /*TEST: normalize model likelihood by normal distribution with mean zero and unc = x2 annual unc.*/
    tot_exp+=pow((oam-mam)/single_annual_unc,2);
    }}


else if (OBS->opt_filter==4 | OBS->opt_filter==5){//climatology and inter-annual variability, only to use with monthly data
    //Decoupling climatological seasonal cycle from inter-anual variations*/
    //Only use with monthly resolution fluxes, complete years & no missing data*/
    //Step 1. Mean model & data annual NBE*/
    //Step 2. Compare means*/
    //Step 3. Remove means from months for cost function*/
    int m, dn,i;
    double * modcm = calloc(sizeof(double),12);
    double * obscm = calloc(sizeof(double),12);
    int * countcm = calloc(sizeof(int),12);
    
    int * icm = calloc(sizeof(double),N);

    //Step 1. 
    //Loop through valid_obs_indices, and bin into mod(I,12) 
    
    for (m=0;m<N;m++){
        //Obs index from 1-12
        icm[m]=OBS->valid_obs_indices[m] % 12;i=icm[m];
        //Average model and obs
        modcm[i] +=mod[m];
        obscm[i] +=obs[m];
        countcm[i]+=1;

        }
    
        //Climatological cost function component: Avoid averaging by scaling cost function uncertainties accordingly!
        for (i=0;i<12;i++){
            if (countcm[i]>0){
            obscm[i]/=(double)countcm[i];
            modcm[i]/=(double)countcm[i];
               tot_exp+=pow((modcm[i]-obscm[i])/single_monthly_unc,2);}}
        
         //Inter-annual cost function component: Avoid averaging by scaling cost function uncertainties accordingly!
               if (OBS->opt_filter==4){
                       for (m=0;m<N;m++){
                                       i=icm[m];
            if (countcm[i]>1){
                tot_exp+=pow((mod[m] - modcm[i] - obs[m]  + obscm[i])/single_annual_unc,2);}}}
//Free pointers
        free(modcm);free(obscm);free(countcm);free(icm);

    }
        
        
        
        
        
  

free(mod);free(obs);free(unc);





P=-0.5*tot_exp;}
// printf("Completed likelihood function...P = %2.2f\n",P);
// printf("OBS->opt_filter = %i\n",OBS->opt_filter);
// printf("OBS->opt_unc_type = %i\n",OBS->opt_unc_type);

return P;

}




// typedef struct SINGLE_OBS_STRUCT{
// double value;
// double unc;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
// int opt_unc_type;//log-transform data 
// double min_threshold;
// }SINGLE_OBS_STRUCT;
// 




double CARDAMOM_SINGLE_OBS_LIKELIHOOD(SINGLE_OBS_STRUCT * OBS,double MOD){

    
double P=0;

if (OBS->value!=DEFAULT_DOUBLE_VAL){
            
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

if (isinf(OBS->min_threshold)==0){
mod = max(mod,OBS->min_threshold);
obs = max(obs,OBS->min_threshold);}


if (OBS->opt_unc_type==1){
mod=log(mod);
obs=log(obs);
unc=log(unc);}


//Cost function
//This is the only option available for single value (e.g. time invariant) observations
tot_exp = pow((mod- obs)/unc,2);


P=-0.5*tot_exp;

//MINMAX LIMITS if (mod < OBS->min_value || mod > OBS->max_value ){P=log(0);}

}


// printf("Completed likelihood function...P = %2.2f\n",P);
// printf("OBS->opt_filter = %i\n",OBS->opt_filter);
// printf("OBS->opt_unc_type = %i\n",OBS->opt_unc_type);

return P;

}
