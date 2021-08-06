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
printf("Number of NBE obs. = %d\n",DATA->nnbe);
printf("Number of EWT obs. = %d\n",DATA->newt); /*shuang*/
printf("Number of CH4 obs. = %d\n",DATA->nch4); /*shuang*/
printf("Number of obs. = %d\n",DATA->noobs); /*shuang*/
printf("Ecological & Dynamic Constraints options\n");
printf("EDC likelihood option = %d\n",DATA->EDC);
printf("EDC diagnostics option = %d\n",DATA->EDCDIAG);
printf("EDC random search (soon obsolete)= %i\n", DATA->edc_random_search);


	printf("DATA->nbe_annual_unc=%2.2f\n", DATA->nbe_annual_unc);
        printf("DATA->et_annual_unc=%2.2f\n",DATA->et_annual_unc);
        printf("DATA->nbe_obs_unc=%2.2f\n",DATA->nbe_obs_unc);
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
DATA->M_NBE=calloc(DATA->nodays,sizeof(double));
DATA->M_ET=calloc(DATA->nodays,sizeof(double));
DATA->M_EWT=calloc(DATA->nodays,sizeof(double));
DATA->M_CH4=calloc(DATA->nodays,sizeof(double));
DATA->M_ABGB_t0=calloc(1,sizeof(double));
DATA->M_ABGB=calloc(DATA->nodays,sizeof(double));
DATA->M_SOM=calloc(DATA->nodays,sizeof(double));
DATA->M_MGPP=calloc(1,sizeof(double));
DATA->M_MFIRE=calloc(1,sizeof(double));
    

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
	if (DATA->NBE==0){DATA->NBE=calloc(DATA->nodays,sizeof(double));}
	if (DATA->LAI==0){DATA->LAI=calloc(DATA->nodays,sizeof(double));}
	if (DATA->ABGB==0){DATA->ABGB=calloc(DATA->nodays,sizeof(double));}
	//if (DATA->ET==0){DATA->ET=calloc(DATA->nodays,sizeof(double));}
	if (DATA->EWT==0){DATA->EWT=calloc(DATA->nodays,sizeof(double));}
	if (DATA->BAND1==0){DATA->BAND1=calloc(DATA->nodays,sizeof(double));}
	if (DATA->BAND2==0){DATA->BAND2=calloc(DATA->nodays,sizeof(double));}
	if (DATA->BAND3==0){DATA->BAND3=calloc(DATA->nodays,sizeof(double));}
	if (DATA->BAND4==0){DATA->BAND4=calloc(DATA->nodays,sizeof(double));}
	if (DATA->SOM==0){DATA->SOM=calloc(DATA->nodays,sizeof(double));}
	if (DATA->NBEunc==0){DATA->NBEunc=calloc(DATA->nodays,sizeof(double));}
	if (DATA->CH4==0){DATA->CH4=calloc(DATA->nodays,sizeof(double));}



	//////Try alternative form
	
	//READ OBS field


	/*What happens:
	- DATA->EWT is populated with 1XN values IF values exist in netcf
	- DATA->EWT is populated with 1XN -9999 values IF either (a) that's what's already there OR (b) single (scaler) fill value is in place of 1XN array (i.e. netcdf "empty" field), OR (c) field doesn't exist */
	/*N - number of points*/


printf("About to declare obs indices\n");

	DATA->ngpp=0;
	DATA->nlai=0;
	DATA->nnbe=0;
	DATA->nabgb=0;
	DATA->net=0;
	DATA->newt=0;
	DATA->nband1=0;
	DATA->nband2=0;
	DATA->nband3=0;
	DATA->nband4=0;
	DATA->nsom=0;
	DATA->nnbeunc=0;
	DATA->nch4=0; /*shuang*/


printf("Done with dynamic memory declaration\n");

return 0;
};

int CARDAMOM_READ_BINARY_DATA(char *ncfilename,DATA *DATA)
{

    int n,m;
    
	  //Step 1.Data file is of the newer NetCDF format
	  CARDAMOM_READ_NETCDF_DATA(ncfilename, &(DATA->ncdf_data));

	//Step 3. Cast into existing data structure
	DATA->ID=DATA->ncdf_data.ID;
	/*Read to get info on NOMET*/
        memcpy(DATA->parpriors,DATA->ncdf_data.PARPRIORS.values,DATA->ncdf_data.PARPRIORS.length*sizeof(double));
        memcpy(DATA->parpriorunc,DATA->ncdf_data.PARPRIORUNC.values,DATA->ncdf_data.PARPRIORS.length*sizeof(double));
        memcpy(DATA->otherpriors,DATA->ncdf_data.OTHERPRIORS.values,DATA->ncdf_data.PARPRIORS.length*sizeof(double));
        memcpy(DATA->otherpriorunc,DATA->ncdf_data.OTHERPRIORSUNC.values,DATA->ncdf_data.PARPRIORS.length*sizeof(double));



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



	/*Default values*/

	if (DATA->ncdf_data.NBE.Uncertainty<0){DATA->ncdf_data.NBE.Uncertainty=0.5;}
	if (DATA->ncdf_data.GPP.Uncertainty<0){DATA->ncdf_data.GPP.Uncertainty=2;}
	//if (DATA->ncdf_data.ET.Uncertainty<0){DATA->ncdf_data.ET.Uncertainty=2;}
	if (DATA->ncdf_data.CH4.Uncertainty<0){DATA->ncdf_data.CH4.Uncertainty=0.5;}
	if (DATA->ncdf_data.EWT.Uncertainty<0){DATA->ncdf_data.EWT.Uncertainty=50;}


// if (DATA->ncdf_data.ET.Uncertainty_Threshold<0){DATA->ncdf_data.ET.Uncertainty_Threshold=0;}
 if (DATA->ncdf_data.GPP.Uncertainty_Threshold<0){DATA->ncdf_data.GPP.Uncertainty_Threshold=0;}
if (DATA->ncdf_data.CH4.Uncertainty_Threshold<0){DATA->ncdf_data.CH4.Uncertainty_Threshold=1e-5;}; /*TO ADDRESS: other fields use "0" threshold as default*/

		
	DATA->nbe_annual_unc=DATA->ncdf_data.NBE.Annual_Uncertainty;
        //DATA->et_annual_unc=DATA->ncdf_data.ET.Annual_Uncertainty;
        DATA->nbe_obs_unc=DATA->ncdf_data.NBE.Uncertainty;
        //DATA->et_obs_unc=DATA->ncdf_data.ET.Uncertainty;
        DATA->ewt_annual_unc=DATA->ncdf_data.EWT.Annual_Uncertainty;
        DATA->ewt_obs_unc=DATA->ncdf_data.EWT.Uncertainty;
        DATA->gpp_annual_unc=DATA->ncdf_data.GPP.Annual_Uncertainty;
        DATA->gpp_obs_unc=DATA->ncdf_data.GPP.Uncertainty;
        DATA->ch4_annual_unc=DATA->ncdf_data.CH4.Annual_Uncertainty;
        DATA->ch4_obs_unc=DATA->ncdf_data.CH4.Uncertainty;
	



       //DATA->et_obs_threshold=DATA->ncdf_data.ET.Uncertainty_Threshold;
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









/*Placeholder: read netcdf fields here*/


printf("***About to do dynamic allocation***\n");


	DYNAMIC_DATA_MEMORY_ALLOCATION(DATA);

printf("***Dynamic allocation done!***\n");


//memcpy(DATA->parpriors,DATA->ncdf_data.PARPRIORS.values,50*sizeof(double));
   memcpy(DATA->GPP,DATA->ncdf_data.GPP.values,DATA->nodays*sizeof(double));
   memcpy(DATA->LAI,DATA->ncdf_data.LAI.values,DATA->nodays*sizeof(double));
   memcpy(DATA->NBE,DATA->ncdf_data.NBE.values,DATA->nodays*sizeof(double));
   memcpy(DATA->ABGB,DATA->ncdf_data.ABGB.values,DATA->nodays*sizeof(double));
   memcpy(DATA->ET,DATA->ncdf_data.ET.values,DATA->nodays*sizeof(double));
   memcpy(DATA->EWT,DATA->ncdf_data.EWT.values,DATA->nodays*sizeof(double));
   /*memcpy(DATA->BAND1,DATA->ncdf_data.BAND1.values,DATA->nodays*sizeof(double));
   memcpy(DATA->BAND2,DATA->ncdf_data.BAND2.values,DATA->nodays*sizeof(double));
   memcpy(DATA->BAND3,DATA->ncdf_data.BAND3.values,DATA->nodays*sizeof(double));
   memcpy(DATA->BAND4,DATA->ncdf_data.BAND4.values,DATA->nodays*sizeof(double));
   */
   memcpy(DATA->SOM,DATA->ncdf_data.SOM.values,DATA->nodays*sizeof(double));
  // memcpy(DATA->NBEunc,DATA->ncdf_data.NBE.values,DATA.nodays*sizeof(double));
   memcpy(DATA->CH4,DATA->ncdf_data.CH4.values,DATA->nodays*sizeof(double));

DATA->noobs=12;

printf("***Memcopy from netcdf done***\n");

for (n=0;n<DATA->nodays;n++){
		if (DATA->GPP[n]>-9998){DATA->ngpp=DATA->ngpp+1;}
                if (DATA->LAI[n]>-9998){DATA->nlai=DATA->nlai+1;}
                if (DATA->NBE[n]>-9998){DATA->nnbe=DATA->nnbe+1;}
                if (DATA->ABGB[n]>-9998){DATA->nabgb=DATA->nabgb+1;}
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









	DATA->deltat=DATA->MET[DATA->nomet]-DATA->MET[0];



	if (DATA->ngpp>0){DATA->gpppts=calloc(DATA->ngpp,sizeof(int));}
	if (DATA->nlai>0){DATA->laipts=calloc(DATA->nlai,sizeof(int));}
	if (DATA->nnbe>0){DATA->nbepts=calloc(DATA->nnbe,sizeof(int));}
	if (DATA->nabgb>0){DATA->abgbpts=calloc(DATA->nabgb,sizeof(int));}
	if (DATA->net>0){DATA->etpts=calloc(DATA->net,sizeof(int));}
	if (DATA->newt>0){DATA->ewtpts=calloc(DATA->newt,sizeof(int));}
	/*if (DATA->nband1>0){DATA->band1pts=calloc(DATA->nband1,sizeof(int));}
	if (DATA->nband2>0){DATA->band2pts=calloc(DATA->nband2,sizeof(int));}
	if (DATA->nband3>0){DATA->band3pts=calloc(DATA->nband3,sizeof(int));}
	if (DATA->nband4>0){DATA->band4pts=calloc(DATA->nband4,sizeof(int));}
	*/
	if (DATA->nsom>0){DATA->sompts=calloc(DATA->nsom,sizeof(int));}
	if (DATA->nnbeunc>0){DATA->nbeuncpts=calloc(DATA->nnbeunc,sizeof(int));}
	if (DATA->nch4>0){DATA->ch4pts=calloc(DATA->nch4,sizeof(int));}       /*shuang*/

	/*Deriving laipts, gpppts, nbepts*/
	int c;
	c=0;for (n=0;n<DATA->nodays;n++){if (DATA->GPP[n]>-9998){DATA->gpppts[c]=n;c=c+1;}};
	c=0;for (n=0;n<DATA->nodays;n++){if (DATA->LAI[n]>-9998){DATA->laipts[c]=n;c=c+1;}};
	c=0;for (n=0;n<DATA->nodays;n++){if (DATA->NBE[n]>-9998){DATA->nbepts[c]=n;c=c+1;}};
	if (DATA->noobs>3){c=0;for (n=0;n<DATA->nodays;n++){printf("%2.2f\n",DATA->ABGB[n]);if (DATA->ABGB[n]>-9998){DATA->abgbpts[c]=n;c=c+1;}}}

	//if (DATA->noobs>4){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->ET[n]>-9998){DATA->etpts[c]=n;c=c+1;}}}

	if (DATA->noobs>5){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->EWT[n]>-9998){DATA->ewtpts[c]=n;c=c+1;}}}
/*
	if (DATA->noobs>6){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->BAND1[n]>-9998){DATA->band1pts[c]=n;c=c+1;}}}

	if (DATA->noobs>7){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->BAND2[n]>-9998){DATA->band2pts[c]=n;c=c+1;}}}

	if (DATA->noobs>8){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->BAND3[n]>-9998){DATA->band3pts[c]=n;c=c+1;}}}

	if (DATA->noobs>9){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->BAND4[n]>-9998){DATA->band4pts[c]=n;c=c+1;}}}
*/
	if (DATA->noobs>10){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->SOM[n]>-9998){DATA->sompts[c]=n;c=c+1;}}}

	/*if (DATA->noobs>11){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->NBEunc[n]>-9998){DATA->nbeuncpts[c]=n;c=c+1;}}}*/

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
CDATA->NBE=0;
CDATA->ABGB=0;
CDATA->ET=0;
CDATA->EWT=0;
CDATA->BAND1=0;
CDATA->BAND2=0;
CDATA->BAND3=0;
CDATA->BAND4=0;
CDATA->SOM=0;
CDATA->NBEunc=0;
CDATA->CH4=0; /*shuang*/


CDATA->assemble_model=0;

return 0;

}


int FREE_DATA_STRUCT(DATA DATA){


if (DATA.ngpp>0){free(DATA.gpppts);}
if (DATA.nlai>0){free(DATA.laipts);}
if (DATA.nnbe>0){free(DATA.nbepts);}
if (DATA.nabgb>0){free(DATA.abgbpts);}
if (DATA.net>0){free(DATA.etpts);}
if (DATA.newt>0){free(DATA.ewtpts);}
if (DATA.nband1>0){free(DATA.band1pts);}
if (DATA.nband2>0){free(DATA.band2pts);}
if (DATA.nband3>0){free(DATA.band3pts);}
if (DATA.nband4>0){free(DATA.band4pts);}
if (DATA.nsom>0){free(DATA.sompts);}
if (DATA.nnbeunc>0){free(DATA.nbeuncpts);}
if (DATA.nch4>0){free(DATA.ch4pts);} /*shuang*/

free(DATA.MET);
free(DATA.LAI);
free(DATA.NBE);
free(DATA.ABGB);
free(DATA.ET);
free(DATA.GPP);
free(DATA.EWT);
free(DATA.BAND4);
free(DATA.BAND3);
free(DATA.BAND2);
free(DATA.BAND1);
free(DATA.SOM);
free(DATA.NBEunc);
free(DATA.CH4);/*shuang*/

//Cost function obsetvations
free(DATA.M_LAI);
free(DATA.M_GPP);
free(DATA.M_ET);
free(DATA.M_NBE);
free(DATA.M_EWT);
free(DATA.M_CH4);
free(DATA.M_ABGB_t0);
free(DATA.M_ABGB);
free(DATA.M_SOM);
free(DATA.M_MGPP);
free(DATA.M_MFIRE);




free(DATA.M_PARS);
free(DATA.M_FLUXES);
free(DATA.M_POOLS);
free(DATA.M_P);
free(DATA.M_EDCD);

free(DATA.parmin);
free(DATA.parmax);
free(DATA.parname);
free(DATA.M_leo);

return 0;

}
