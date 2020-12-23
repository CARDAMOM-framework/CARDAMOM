/*INCLUDING THE PARAMETER_INFO structure*/
/*Note: it remains unclear as to where this structure should be defined*/



#pragma once
#include "../../auxi_fun/filediag.c"
#include "../../auxi_fun/oksofar.c"
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include "CARDAMOM_DATA_STRUCTURE.c"
#include "CARDAMOM_READ_NETCDF_DATA.c"

/*USER DEFINED STRUCTURE*/
/*This is the ONLY structure the user should alter, adapt, etc.,
 *  * in order to optimally stored all required model drivers, and measurements
 *   * observations, etc. etc.*/

#include "CARDAMOM_MODEL_LIBRARY.c"


//Helper method, primarily for use identifying file extensions. Adapted from https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c for ease and edge case management
int StringEndsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int CARDAMOM_DATA_CHECKS(DATA *DATA){
/*General Checks*/
printf("***CBF FILE SUMMARY***\n");
printf("MODEL ID = %d\n",DATA->ID);
printf("No days = %d\n",DATA->nodays);
printf("Mean Rad = %f\n",DATA->meanrad);
printf("Mean Temp = %f\n",DATA->meantemp);
printf("Mean Prec = %f\n",DATA->meanprec);
printf("Latitude = %f\n",DATA->LAT);
printf("Number of MET drivers%d\n",DATA->nomet);
printf("Number of GPP obs. = %d\n",DATA->ngpp);
printf("Number of LAI obs. = %d\n",DATA->nlai);
printf("Number of NEE obs. = %d\n",DATA->nnee);
printf("Number of EWT obs. = %d\n",DATA->newt); /*shuang*/
printf("Number of CH4 obs. = %d\n",DATA->nch4); /*shuang*/
printf("Number of obs. = %d\n",DATA->noobs); /*shuang*/
printf("Ecological & Dynamic Constraints options\n");
printf("EDC likelihood option = %d\n",DATA->EDC);
printf("EDC diagnostics option = %d\n",DATA->EDCDIAG);
printf("EDC random search (soon obsolete)= %i\n", DATA->edc_random_search);


	printf("DATA->nee_annual_unc=%2.2f\n", DATA->nee_annual_unc);
        printf("DATA->et_annual_unc=%2.2f\n",DATA->et_annual_unc);
        printf("DATA->nee_obs_unc=%2.2f\n",DATA->nee_obs_unc);
        printf("DATA->et_obs_unc=%2.2f\n",DATA->et_obs_unc);
        printf("DATA->ewt_annual_unc=%2.2f\n",DATA->ewt_annual_unc);
        printf("DATA->ewt_obs_unc=%2.2f\n",DATA->ewt_obs_unc);
        printf("DATA->gpp_annual_unc=%2.2f\n",DATA->gpp_annual_unc);
        printf("DATA->gpp_obs_unc%2.2f\n",DATA->gpp_obs_unc);
        printf("DATA->ch4_annual_unc=%2.2f\n",DATA->ch4_annual_unc);
        printf("DATA->ch4_obs_unc=%2.2f\n",DATA->ch4_obs_unc);

/*
int n;
printf("PP \t PPU \t OP \t OPU\n");
for (n=0;n<50;n++){printf("%2.2f \t %2.2f \t %2.2f \t %2.2f\n",DATA->parpriors[n],DATA->parpriorunc[n],DATA->otherpriors[n],DATA->otherpriorunc[n]);} 


printf("PCrotate (soon obsolete) = %i\n",DATA->PCrotate);
printf("Deltat = %2.2f\n",DATA->deltat);
*/
printf("*****END OF CBF FILE SUMMARY***");





return 0;}

int DYNAMIC_DATA_MEMORY_ALLOCATION(DATA * DATA){
	
	DATA->M_LAI=calloc(DATA->nodays,sizeof(double));
	DATA->M_GPP=calloc(DATA->nodays,sizeof(double));
	DATA->M_NEE=calloc(DATA->nodays,sizeof(double));

	printf("DATA->nodays = %i\n", DATA->nodays);
	printf("DATA->nofluxes = %i\n", DATA->nofluxes);
	printf("DATA->nopools = %i\n", DATA->nopools);
	printf("DATA->nomet = %i\n", DATA->nomet);


printf("About to declare model memory\n");
	DATA->M_FLUXES=calloc(DATA->nodays*DATA->nofluxes,sizeof(double));
	DATA->M_POOLS=calloc((DATA->nodays+1)*DATA->nopools,sizeof(double));
	int noedc=100, noprob=1;
	DATA->M_EDCD=calloc(noedc,sizeof(int));
	DATA->M_P=calloc(noprob,sizeof(double));
	DATA->M_leo=calloc(1,sizeof(double));
	DATA->M_leo[0]=1.0/0.0;

	DATA->M_PARS=calloc(DATA->nopars,sizeof(double));



printf("About to declare obs memory\n");
	/*READING TEMPORAL DATA*/
	/*For re-use of DATA structure (presumably), the data is only re-read if the fields are set to zero (or initialized)*/
	/*Currently not sure why this is here: however, this is harmless*/
	/*also it is good practice to initialize pointers in C*/
	if (DATA->MET==0){DATA->MET=calloc(DATA->nomet*DATA->nodays,sizeof(double));}
	if (DATA->GPP==0){DATA->GPP=calloc(DATA->nodays,sizeof(double));}
	if (DATA->NEE==0){DATA->NEE=calloc(DATA->nodays,sizeof(double));}
	if (DATA->LAI==0){DATA->LAI=calloc(DATA->nodays,sizeof(double));}
	if (DATA->WOO==0){DATA->WOO=calloc(DATA->nodays,sizeof(double));}
	if (DATA->ET==0){DATA->ET=calloc(DATA->nodays,sizeof(double));}
	if (DATA->EWT==0){DATA->EWT=calloc(DATA->nodays,sizeof(double));}
	if (DATA->BAND1==0){DATA->BAND1=calloc(DATA->nodays,sizeof(double));}
	if (DATA->BAND2==0){DATA->BAND2=calloc(DATA->nodays,sizeof(double));}
	if (DATA->BAND3==0){DATA->BAND3=calloc(DATA->nodays,sizeof(double));}
	if (DATA->BAND4==0){DATA->BAND4=calloc(DATA->nodays,sizeof(double));}
	if (DATA->SOM==0){DATA->SOM=calloc(DATA->nodays,sizeof(double));}
	if (DATA->NEEunc==0){DATA->NEEunc=calloc(DATA->nodays,sizeof(double));}
	if (DATA->CH4==0){DATA->CH4=calloc(DATA->nodays,sizeof(double));}

	/*What happens:
	- DATA->EWT is populated with 1XN values IF values exist in netcf
	- DATA->EWT is populated with 1XN -9999 values IF either (a) that's what's already there OR (b) single (scaler) fill value is in place of 1XN array (i.e. netcdf "empty" field), OR (c) field doesn't exist */
	/*N - number of points*/


printf("About to declare obs indices\n");

	DATA->ngpp=0;
	DATA->nlai=0;
	DATA->nnee=0;
	DATA->nwoo=0;
	DATA->net=0;
	DATA->newt=0;
	DATA->nband1=0;
	DATA->nband2=0;
	DATA->nband3=0;
	DATA->nband4=0;
	DATA->nsom=0;
	DATA->nneeunc=0;
	DATA->nch4=0; /*shuang*/


printf("Done with dynamic memory declaration\n");

return 0;
};

int CARDAMOM_READ_BINARY_DATA(char *filename,DATA *DATA)
{
	//This determines if we are dealing with netCDF files, or traditional cardamom files. By default, we assume the older binary format
	int fileIsCDF =0;
	int testnc=0,n,m;
	char *ncfilename="CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE_beta_v2.nc.cbf";
	if (StringEndsWith(filename, ".nc.cbf") | (testnc==1)){
	  fileIsCDF=1;
		if (testnc==0){ncfilename=filename;}
	}

	if (fileIsCDF){
	  //Step 1.Data file is of the newer NetCDF format
	  CARDAMOM_READ_NETCDF_DATA(ncfilename, &(DATA->ncdf_data));

	//Step 3. Cast into existing data structure
	DATA->ID=DATA->ncdf_data.ID;
	/*Read to get info on NOMET*/
        memcpy(DATA->parpriors,DATA->ncdf_data.PARPRIORS.values,50*sizeof(double));
        memcpy(DATA->parpriorunc,DATA->ncdf_data.PARPRIORUNC.values,50*sizeof(double));
        memcpy(DATA->otherpriors,DATA->ncdf_data.OTHERPRIORS.values,50*sizeof(double));
        memcpy(DATA->otherpriorunc,DATA->ncdf_data.OTHERPRIORSUNC.values,50*sizeof(double));

	DATA->EDC=DATA->ncdf_data.EDC;
	DATA->EDCDIAG=DATA->ncdf_data.EDCDIAG;
	/*Note CARDAMOM_MODEL_LIBRARY requires two initiated quantities
	1. DATA->ID	
	2. DATA->otherpriorunc
	3. DATA->EDC
	4. DATA->EDCDIAG
	This will be obsolete when calling things by ncdf_data availabilty
	*/
	CARDAMOM_MODEL_LIBRARY(DATA);
        /*This will eventually be obsolete*/

        DATA->nomet=((DALEC *)DATA->MODEL)->nomet;
	DATA->noobs=-9999;	

	DATA->LAT=DATA->ncdf_data.LAT;
	DATA->nodays=DATA->ncdf_data.TIME_INDEX.length;
	

	DATA->gppabs=DATA->ncdf_data.GPP.gppabs;

	/*soon obsolete, once dependencies removed elsewhere*/
	DATA->edc_random_search=-9999;
        
	/*GPP,LAI iav were never used*/
	/*
        DATA->gppiav=(int)statdat[11];
        DATA->laiiav=(int)statdat[12];
        DATA->ch4iav=(int)statdat[23];  
	*/


/*

        DATA->edc_random_search=(int)statdat[10];
        DATA->gppiav=(int)statdat[11];
        DATA->laiiav=(int)statdat[12];
        DATA->nee_annual_unc=statdat[13];
        DATA->et_annual_unc=statdat[14];
        DATA->nee_obs_unc=statdat[15];if (statdat[15]<0){DATA->nee_obs_unc=0.5;}
        DATA->et_obs_unc=statdat[16];if (statdat[16]<0){DATA->et_obs_unc=2;}
        DATA->ewt_annual_unc=statdat[17];
        DATA->ewt_obs_unc=statdat[18];if (statdat[18]<0){DATA->ewt_obs_unc=50;}
        DATA->gpp_annual_unc=statdat[19];
        DATA->gpp_obs_unc=statdat[20];if (statdat[20]<0){DATA->gpp_obs_unc=2;}
        DATA->et_obs_threshold=statdat[21]; if (statdat[21]<0){DATA->et_obs_threshold=0;}
        DATA->gpp_obs_threshold=statdat[22]; if (statdat[22]<0){DATA->gpp_obs_threshold=0;}
        DATA->ch4iav=(int)statdat[23];  
        DATA->ch4_annual_unc=statdat[24];  
        DATA->ch4_obs_unc=statdat[25];if (statdat[25]<0){DATA->ch4_obs_unc=0.5;}  
        DATA->ch4_obs_threshold=statdat[26]; if (statdat[26]<0){DATA->ch4_obs_threshold=1e-5;}  
*/





	
	/*Default values*/

	if (DATA->ncdf_data.NBE.Uncertainty<0){DATA->ncdf_data.NBE.Uncertainty=0.5;}
	if (DATA->ncdf_data.GPP.Uncertainty<0){DATA->ncdf_data.GPP.Uncertainty=2;}
	if (DATA->ncdf_data.ET.Uncertainty<0){DATA->ncdf_data.ET.Uncertainty=2;}
	if (DATA->ncdf_data.CH4.Uncertainty<0){DATA->ncdf_data.CH4.Uncertainty=0.5;}
	if (DATA->ncdf_data.EWT.Uncertainty<0){DATA->ncdf_data.EWT.Uncertainty=50;}


 if (DATA->ncdf_data.ET.Uncertainty_Threshold<0){DATA->ncdf_data.ET.Uncertainty_Threshold=0;}
 if (DATA->ncdf_data.GPP.Uncertainty_Threshold<0){DATA->ncdf_data.GPP.Uncertainty_Threshold=0;}
if (DATA->ncdf_data.CH4.Uncertainty_Threshold<0){DATA->ncdf_data.CH4.Uncertainty_Threshold=1e-5;}; /*TO ADDRESS: other fields use "0" threshold as default*/

		
	DATA->nee_annual_unc=DATA->ncdf_data.NBE.Annual_Uncertainty;
        DATA->et_annual_unc=DATA->ncdf_data.ET.Annual_Uncertainty;
        DATA->nee_obs_unc=DATA->ncdf_data.NBE.Uncertainty;
        DATA->et_obs_unc=DATA->ncdf_data.ET.Uncertainty;
        DATA->ewt_annual_unc=DATA->ncdf_data.EWT.Annual_Uncertainty;
        DATA->ewt_obs_unc=DATA->ncdf_data.EWT.Uncertainty;
        DATA->gpp_annual_unc=DATA->ncdf_data.GPP.Annual_Uncertainty;
        DATA->gpp_obs_unc=DATA->ncdf_data.GPP.Uncertainty;
        DATA->ch4_annual_unc=DATA->ncdf_data.CH4.Annual_Uncertainty;
        DATA->ch4_obs_unc=DATA->ncdf_data.CH4.Uncertainty;
	



       DATA->et_obs_threshold=DATA->ncdf_data.ET.Uncertainty_Threshold;
       DATA->gpp_obs_threshold=DATA->ncdf_data.GPP.Uncertainty_Threshold;
       DATA->ch4_obs_threshold=DATA->ncdf_data.CH4.Uncertainty_Threshold; 




	/*NEXT STEPS: figure out how to ceclafre MET and other memory*/
	// READ GPP, LAI, etc.:






	



	

	

	printf("DATA->LAT = %2.2f\n",DATA->LAT);
	printf("DATA->ID = %i\n",DATA->ID);
	
	for (n=0;n<5;n++){printf("*******\n");}
	printf("Testing netcdf I/O!!\n");
	for (n=0;n<5;n++){printf("*******\n");}
	printf("read netcdf and made it to here!!\n");
	for (n=0;n<5;n++){printf("*******\n");}






	}

		  //parse the data file as the older binary format

	/*TEMPLATE FOR ALL DALEC MCMC DATA files*/
	/*Static Elements: 1-100 - use as many as needed*/
	/*Parameter Priors: 101-150*/
	/*Parameter prior uncertainty: 151-200*/
	/*Other priors & uncertainties: 201-300*/
	/*TEMPORAL DRIVERS & DATA: 301-end*/

if (fileIsCDF==0){

printf("About to read cbf binary file...\n");
	FILE *fid=fopen(filename,"rb");
	filediag(fid,filename);

	/*READING STATIC DATA*/

	double statdat[100];
	/*reading 1-100*/
	fread(statdat,sizeof(double),100,fid);
	/*DALEC model run info*/
	DATA->ID=(int)statdat[0];
	DATA->LAT=statdat[1];
	DATA->nodays=(int)statdat[2];
	DATA->nomet=(int)statdat[3];
	DATA->noobs=(int)statdat[4];
	DATA->EDC=(int)statdat[5];
	DATA->EDCDIAG=(int)statdat[6];
	DATA->gppabs=(int)statdat[7];
	/*DALEC MCMC run info*/
	/*set to 1 for (a) few and (b) well constrained priors, otherwise 0*/
	/*binary file mcmc options (need to add all options HERE except inout files)*/
	DATA->edc_random_search=(int)statdat[10];
	DATA->gppiav=(int)statdat[11];
	DATA->laiiav=(int)statdat[12];
	DATA->nee_annual_unc=statdat[13];
	DATA->et_annual_unc=statdat[14];
	DATA->nee_obs_unc=statdat[15];if (statdat[15]<0){DATA->nee_obs_unc=0.5;}
	DATA->et_obs_unc=statdat[16];if (statdat[16]<0){DATA->et_obs_unc=2;}
	DATA->ewt_annual_unc=statdat[17];
	DATA->ewt_obs_unc=statdat[18];if (statdat[18]<0){DATA->ewt_obs_unc=50;}
	DATA->gpp_annual_unc=statdat[19];
	DATA->gpp_obs_unc=statdat[20];if (statdat[20]<0){DATA->gpp_obs_unc=2;}
	DATA->et_obs_threshold=statdat[21]; if (statdat[21]<0){DATA->et_obs_threshold=0;}
	DATA->gpp_obs_threshold=statdat[22]; if (statdat[22]<0){DATA->gpp_obs_threshold=0;}
	DATA->ch4iav=(int)statdat[23];  /*shuang*/
	DATA->ch4_annual_unc=statdat[24];  /*shuang*/
	DATA->ch4_obs_unc=statdat[25];if (statdat[25]<0){DATA->ch4_obs_unc=0.5;}  /*shuang*/
	DATA->ch4_obs_threshold=statdat[26]; if (statdat[26]<0){DATA->ch4_obs_threshold=1e-5;}  /*shuang; AAB = 0.1 mgCH4/m2/day*/


	/*UP TO USER to read data and allocate it to DATA structure*/
	double parpriors[50],parpriorunc[50],otherpriors[50],otherpriorunc[50];
	fread(parpriors,sizeof(double),50,fid);
	fread(parpriorunc,sizeof(double),50,fid);
	fread(otherpriors,sizeof(double),50,fid);
	fread(otherpriorunc,sizeof(double),50,fid);

	/*For universal data structure, DATA contains 50 parameter prior spaces
	 * use as many as needed!*/
	memcpy(DATA->parpriors,parpriors,50*sizeof(double));
	memcpy(DATA->parpriorunc,parpriorunc,50*sizeof(double));
	memcpy(DATA->otherpriors,otherpriors,50*sizeof(double));
	memcpy(DATA->otherpriorunc,otherpriorunc,50*sizeof(double));


	/*loading model specific values*/
	/*this includes information on the number of pools, number of parameters, etc.
	?*this allows an initialization of the model output fields (stored in DATA for simplicity)*/

	CARDAMOM_MODEL_LIBRARY(DATA);





/*********Memory allocation for dynamic fields, common to both netcdf and cbf formats*******/



	/*the following fields (begining by M_) are for storage purposes*/
	/*data stored in these fields is over-written at each model run*/
	/*Future versions: these should be declared within each model structure
	or generically declared for model types (e.g. DALEC, etc).*/
	/*Model-specific quantities (nodays, nofluxes, nopools), will be declared in MODEL_INFO for modularity*/
	
	DYNAMIC_DATA_MEMORY_ALLOCATION(DATA);



	/*6 met fields for DALEC CDEA*/

	int n, nn;
	double *metline, *obsline;
	metline=calloc(DATA->nomet,sizeof(double));
	obsline=calloc(DATA->noobs,sizeof(double));
	/*mean met and mean obs line, in case user-defined*/


	int frfm,frfo;


	printf("reading file...\n");
	for (n=0;n<DATA->nodays+1;n++){
		frfm=fread(metline,sizeof(double),DATA->nomet,fid);
		frfo=fread(obsline,sizeof(double),DATA->noobs,fid);
		if (n<DATA->nodays){
		for (nn=0;nn<DATA->nomet;nn++){DATA->MET[n*DATA->nomet+nn]=metline[nn];}
		DATA->GPP[n]=obsline[0];
		DATA->LAI[n]=obsline[1];
		DATA->NEE[n]=obsline[2];
		if (obsline[0]>-9998){DATA->ngpp=DATA->ngpp+1;}
		if (obsline[1]>-9998){DATA->nlai=DATA->nlai+1;}
		if (obsline[2]>-9998){DATA->nnee=DATA->nnee+1;}
		if (DATA->noobs>3){DATA->WOO[n]=obsline[3];
		if (obsline[3]>-9998){DATA->nwoo=DATA->nwoo+1;}}
	        if (DATA->noobs>4){DATA->ET[n]=obsline[4];
	        if (obsline[4]>-9998){DATA->net=DATA->net+1;}}
	        if (DATA->noobs>5){DATA->EWT[n]=obsline[5];
	        if (obsline[5]>-9998){DATA->newt=DATA->newt+1;}}

	        if (DATA->noobs>6){DATA->BAND1[n]=obsline[6];
	        if (obsline[6]>-9998){DATA->nband1=DATA->nband1+1;}}

	        if (DATA->noobs>7){DATA->BAND2[n]=obsline[7];
	        if (obsline[7]>-9998){DATA->nband2=DATA->nband2+1;}}

	        if (DATA->noobs>8){DATA->BAND3[n]=obsline[8];
	        if (obsline[8]>-9998){DATA->nband3=DATA->nband3+1;}}

	        if (DATA->noobs>9){DATA->BAND4[n]=obsline[9];
	        if (obsline[9]>-9998){DATA->nband4=DATA->nband4+1;}}

	        if (DATA->noobs>10){DATA->SOM[n]=obsline[10];
	        if (obsline[10]>-9998){DATA->nsom=DATA->nsom+1;}}

	     /*   if (DATA->noobs>11){DATA->NEEunc[n]=obsline[11];
	        if (obsline[11]>-9998){DATA->nneeunc=DATA->nneeunc+1;}}*/  /*shuang by default there is not a field for NEEunc in CBF.OBS*/

	        if (DATA->noobs>11){DATA->CH4[n]=obsline[11];
	        if (obsline[11]>-9998){DATA->nch4=DATA->nch4+1;}}   /*shuang this correspond to the new field CH4 added after SOM in write binary.m */
	};

	}

	fclose(fid);



	/*deriving mean temp and mean rad*/
	DATA->meantemp=0;
	DATA->meanrad=0;
	DATA->meanprec=0;
	/*2 options:*/
	/*1. derive mean met values based on "MET" (frfm = DATA->nomet)*/
	/*2. prescribe user-provided mean met values (frfm = 0)*/
	if (frfm==0){
	for (n=0;n<DATA->nodays;n++){DATA->meantemp+=0.5*DATA->MET[DATA->nomet*n+1]/(double)DATA->nodays;}
	for (n=0;n<DATA->nodays;n++){DATA->meantemp+=0.5*DATA->MET[DATA->nomet*n+2]/(double)DATA->nodays;}
	for (n=0;n<DATA->nodays;n++){DATA->meanrad+=DATA->MET[DATA->nomet*n+3]/(double)DATA->nodays;}
	/*only if no met > 6*/
	if (DATA->nomet>8){
	for (n=0;n<DATA->nodays;n++){DATA->meanprec+=DATA->MET[DATA->nomet*n+8]/(double)DATA->nodays;}};
	printf("No prescribed met reference means, calculating based on driver data\n");
	}
	else if (frfm==DATA->nomet){
	DATA->meantemp=0.5*metline[1] + 0.5*metline[2];
	DATA->meanrad=metline[3];
	if (DATA->nomet>6){DATA->meanprec=metline[8];}
	printf("Using prescribed met reference means\n");
	}
	printf("frfm = %i\n",frfm);


        free(metline);
        free(obsline);

/*done reading standard cbf file*/}




/*Placeholder: read netcdf fields here*/

if (fileIsCDF){

printf("***About to do dynamic allocation***\n");


	DYNAMIC_DATA_MEMORY_ALLOCATION(DATA);

printf("***Dynamic allocation done!***\n");


//memcpy(DATA->parpriors,DATA->ncdf_data.PARPRIORS.values,50*sizeof(double));
   memcpy(DATA->GPP,DATA->ncdf_data.GPP.values,DATA->nodays*sizeof(double));
   memcpy(DATA->LAI,DATA->ncdf_data.LAI.values,DATA->nodays*sizeof(double));
   memcpy(DATA->NEE,DATA->ncdf_data.NBE.values,DATA->nodays*sizeof(double));
   memcpy(DATA->WOO,DATA->ncdf_data.ABGB.values,DATA->nodays*sizeof(double));
   memcpy(DATA->ET,DATA->ncdf_data.ET.values,DATA->nodays*sizeof(double));
   memcpy(DATA->EWT,DATA->ncdf_data.EWT.values,DATA->nodays*sizeof(double));
   /*memcpy(DATA->BAND1,DATA->ncdf_data.BAND1.values,DATA->nodays*sizeof(double));
   memcpy(DATA->BAND2,DATA->ncdf_data.BAND2.values,DATA->nodays*sizeof(double));
   memcpy(DATA->BAND3,DATA->ncdf_data.BAND3.values,DATA->nodays*sizeof(double));
   memcpy(DATA->BAND4,DATA->ncdf_data.BAND4.values,DATA->nodays*sizeof(double));
   */
   memcpy(DATA->SOM,DATA->ncdf_data.SOM.values,DATA->nodays*sizeof(double));
  // memcpy(DATA->NEEunc,DATA->ncdf_data.NEE.values,DATA.nodays*sizeof(double));
   memcpy(DATA->CH4,DATA->ncdf_data.CH4.values,DATA->nodays*sizeof(double));

DATA->noobs=12;

printf("***Memcopy from netcdf done***\n");

for (n=0;n<DATA->nodays;n++){
		if (DATA->GPP[n]>-9998){DATA->ngpp=DATA->ngpp+1;}
                if (DATA->LAI[n]>-9998){DATA->nlai=DATA->nlai+1;}
                if (DATA->NEE[n]>-9998){DATA->nnee=DATA->nnee+1;}
                if (DATA->WOO[n]>-9998){DATA->nwoo=DATA->nwoo+1;}
                if (DATA->ET[n]>-9998){DATA->net=DATA->net+1;}
                if (DATA->EWT[n]>-9998){DATA->newt=DATA->newt+1;}
/*              if (DATA->BAND1>-9998){DATA->nband1=DATA->nband1+1;}
                if (DATA->BAND2>-9998){DATA->nband2=DATA->nband2+1;}
                if (DATA->BAND3>-9998){DATA->nband3=DATA->nband3+1;}
                if (DATA->BAND4>-9998){DATA->nband4=DATA->nband4+1;}*/
                if (DATA->SOM[n]>-9998){DATA->nsom=DATA->nsom+1;}
                if (DATA->CH4[n]>-9998){DATA->nch4=DATA->nch4+1;}
/*endofloop*/} 


	/*Populate "MET" with 9 fields*/
	for (n=0;n<DATA->nodays;n++){
	m=n*DATA->nomet;
	DATA->MET[m]=DATA->ncdf_data.TIME_INDEX.values[n];
	DATA->MET[m+1]=DATA->ncdf_data.T2M_MIN.values[n];
	DATA->MET[m+2]=DATA->ncdf_data.T2M_MAX.values[n];
	DATA->MET[m+3]=DATA->ncdf_data.SSRD.values[n];
	DATA->MET[m+4]=DATA->ncdf_data.CO2.values[n];
	DATA->MET[m+5]=DATA->ncdf_data.DOY.values[n];
	DATA->MET[m+6]=DATA->ncdf_data.BURNED_AREA.values[n];
	DATA->MET[m+7]=DATA->ncdf_data.VPD.values[n];
	DATA->MET[m+8]=DATA->ncdf_data.TOTAL_PREC.values[n];

	}



/*Populate with mean fields*/
DATA->meantemp = DATA->ncdf_data.T2M_MAX.reference_mean/2 + DATA->ncdf_data.T2M_MIN.reference_mean/2;
DATA->meanrad = DATA->ncdf_data.SSRD.reference_mean;
DATA->meanprec = DATA->ncdf_data.TOTAL_PREC.reference_mean;



/*Done reading netcdf*/}






	DATA->deltat=DATA->MET[DATA->nomet]-DATA->MET[0];



	if (DATA->ngpp>0){DATA->gpppts=calloc(DATA->ngpp,sizeof(int));}
	if (DATA->nlai>0){DATA->laipts=calloc(DATA->nlai,sizeof(int));}
	if (DATA->nnee>0){DATA->neepts=calloc(DATA->nnee,sizeof(int));}
	if (DATA->nwoo>0){DATA->woopts=calloc(DATA->nwoo,sizeof(int));}
	if (DATA->net>0){DATA->etpts=calloc(DATA->net,sizeof(int));}
	if (DATA->newt>0){DATA->ewtpts=calloc(DATA->newt,sizeof(int));}
	/*if (DATA->nband1>0){DATA->band1pts=calloc(DATA->nband1,sizeof(int));}
	if (DATA->nband2>0){DATA->band2pts=calloc(DATA->nband2,sizeof(int));}
	if (DATA->nband3>0){DATA->band3pts=calloc(DATA->nband3,sizeof(int));}
	if (DATA->nband4>0){DATA->band4pts=calloc(DATA->nband4,sizeof(int));}
	*/
	if (DATA->nsom>0){DATA->sompts=calloc(DATA->nsom,sizeof(int));}
	if (DATA->nneeunc>0){DATA->neeuncpts=calloc(DATA->nneeunc,sizeof(int));}
	if (DATA->nch4>0){DATA->ch4pts=calloc(DATA->nch4,sizeof(int));}       /*shuang*/

	/*Deriving laipts, gpppts, neepts*/
	int c;
	c=0;for (n=0;n<DATA->nodays;n++){if (DATA->GPP[n]>-9998){DATA->gpppts[c]=n;c=c+1;}};
	c=0;for (n=0;n<DATA->nodays;n++){if (DATA->LAI[n]>-9998){DATA->laipts[c]=n;c=c+1;}};
	c=0;for (n=0;n<DATA->nodays;n++){if (DATA->NEE[n]>-9998){DATA->neepts[c]=n;c=c+1;}};
	if (DATA->noobs>3){c=0;for (n=0;n<DATA->nodays;n++){printf("%2.2f\n",DATA->WOO[n]);if (DATA->WOO[n]>-9998){DATA->woopts[c]=n;c=c+1;}}}

	if (DATA->noobs>4){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->ET[n]>-9998){DATA->etpts[c]=n;c=c+1;}}}

	if (DATA->noobs>5){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->EWT[n]>-9998){DATA->ewtpts[c]=n;c=c+1;}}}
/*
	if (DATA->noobs>6){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->BAND1[n]>-9998){DATA->band1pts[c]=n;c=c+1;}}}

	if (DATA->noobs>7){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->BAND2[n]>-9998){DATA->band2pts[c]=n;c=c+1;}}}

	if (DATA->noobs>8){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->BAND3[n]>-9998){DATA->band3pts[c]=n;c=c+1;}}}

	if (DATA->noobs>9){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->BAND4[n]>-9998){DATA->band4pts[c]=n;c=c+1;}}}
*/
	if (DATA->noobs>10){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->SOM[n]>-9998){DATA->sompts[c]=n;c=c+1;}}}

	/*if (DATA->noobs>11){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->NEEunc[n]>-9998){DATA->neeuncpts[c]=n;c=c+1;}}}*/

	if (DATA->noobs>11){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->CH4[n]>-9998){DATA->ch4pts[c]=n;c=c+1;}}} /*shuang*/



	CARDAMOM_DATA_CHECKS(DATA);







return 0;



}




int INITIALIZE_DATA_STRUCT(DATA *CDATA){

/*initialising array pointers as zero*/
/*NOTE: These may need to be set to NULL*/
CDATA->MET=0;
CDATA->GPP=0;
CDATA->LAI=0;
CDATA->NEE=0;
CDATA->WOO=0;
CDATA->ET=0;
CDATA->EWT=0;
CDATA->BAND1=0;
CDATA->BAND2=0;
CDATA->BAND3=0;
CDATA->BAND4=0;
CDATA->SOM=0;
CDATA->NEEunc=0;
CDATA->CH4=0; /*shuang*/


CDATA->assemble_model=0;

return 0;

}


int FREE_DATA_STRUCT(DATA DATA){


if (DATA.ngpp>0){free(DATA.gpppts);}
if (DATA.nlai>0){free(DATA.laipts);}
if (DATA.nnee>0){free(DATA.neepts);}
if (DATA.nwoo>0){free(DATA.woopts);}
if (DATA.net>0){free(DATA.etpts);}
if (DATA.newt>0){free(DATA.ewtpts);}
if (DATA.nband1>0){free(DATA.band1pts);}
if (DATA.nband2>0){free(DATA.band2pts);}
if (DATA.nband3>0){free(DATA.band3pts);}
if (DATA.nband4>0){free(DATA.band4pts);}
if (DATA.nsom>0){free(DATA.sompts);}
if (DATA.nneeunc>0){free(DATA.neeuncpts);}
if (DATA.nch4>0){free(DATA.ch4pts);} /*shuang*/

free(DATA.MET);
free(DATA.LAI);
free(DATA.NEE);
free(DATA.WOO);
free(DATA.ET);
free(DATA.GPP);
free(DATA.EWT);
free(DATA.BAND4);
free(DATA.BAND3);
free(DATA.BAND2);
free(DATA.BAND1);
free(DATA.SOM);
free(DATA.NEEunc);
free(DATA.CH4);/*shuang*/

free(DATA.M_PARS);
free(DATA.M_LAI);
free(DATA.M_GPP);
free(DATA.M_FLUXES);
free(DATA.M_NEE);
free(DATA.M_POOLS);
free(DATA.M_P);
free(DATA.M_EDCD);

free(DATA.parmin);
free(DATA.parmax);
free(DATA.parname);
free(DATA.M_leo);

return 0;

}
