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

printf("Ecological & Dynamic Constraints options\n");
printf("EDC likelihood option = %d\n",DATA->EDC);
printf("EDC diagnostics option = %d\n",DATA->EDCDIAG);


printf("*****END OF CBF FILE SUMMARY***");



return 0;}

int DYNAMIC_DATA_MEMORY_ALLOCATION(DATA * DATA){
	
DATA->M_ABGB=calloc(DATA->nodays,sizeof(double));
DATA->M_CH4=calloc(DATA->nodays,sizeof(double));
DATA->M_DOM=calloc(DATA->nodays,sizeof(double));
DATA->M_EWT=calloc(DATA->nodays,sizeof(double));
DATA->M_ET=calloc(DATA->nodays,sizeof(double));
DATA->M_LAI=calloc(DATA->nodays,sizeof(double));
DATA->M_GPP=calloc(DATA->nodays,sizeof(double));
DATA->M_NBE=calloc(DATA->nodays,sizeof(double));

    


//Model variables 
	DATA->M_FLUXES=calloc(DATA->nodays*DATA->nofluxes,sizeof(double));
	DATA->M_POOLS=calloc((DATA->nodays+1)*DATA->nopools,sizeof(double));
	int noedc=100, noprob=1;
	DATA->M_EDCD=calloc(noedc,sizeof(int));
	DATA->M_P=calloc(noprob,sizeof(double));

	DATA->M_PARS=calloc(DATA->nopars,sizeof(double));



printf("Done with dynamic memory declaration\n");

return 0;
};

int CARDAMOM_READ_BINARY_DATA(char *ncfilename,DATA *DATA)
{

    int n,m;
    
    
    
    
    


    
	  //Step 1.Data file is of the newer NetCDF format
	  CARDAMOM_READ_NETCDF_DATA(ncfilename, &(DATA->ncdf_data));

printf("Successfully read NETCDF data...\n");



//Pre-process (to accelerate model)



printf("DATA->ncdf_data.ET.opt_unc_type = %i\n",DATA->ncdf_data.ET.opt_unc_type);
printf("DATA->ncdf_data.ET.opt_filter = %i\n",DATA->ncdf_data.ET.opt_filter);
printf("DATA->ncdf_data.ET.single_unc = %2.2f\n",DATA->ncdf_data.ET.single_unc);
printf("DATA->ncdf_data.ET.min_threshold = %2.2f\n", DATA->ncdf_data.ET.min_threshold);
printf("Successfully pre-processed ET...\n");

// 
// 
// //Step 2. Default values
// DATA->ncdf_data->ET.obs_unc_type=1;
// DATA->ncdf_data->ET.single_unc=2;
// DATA->ncdf_data->ET.threshold=0;
// //Pre-process (to accelerate model)
// OBS_STRUCT_PREPROCESS(DATA->ncdf_data->ET);





//Next step. Test all data 


		
	//Step 3. Cast into existing data structure
	DATA->ID=DATA->ncdf_data.ID;
	/*Read to get info on NOMET*/


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



/*Populate with mean fields*/
DATA->meantemp = DATA->ncdf_data.T2M_MAX.reference_mean/2 + DATA->ncdf_data.T2M_MIN.reference_mean/2;
DATA->meanrad = DATA->ncdf_data.SSRD.reference_mean;
DATA->meanprec = DATA->ncdf_data.TOTAL_PREC.reference_mean;






	CARDAMOM_DATA_CHECKS(DATA);


printf("Done reading all data...\n");

return 0;



}



int FREE_DATA_STRUCT(DATA DATA){


//Cost function obsetvations
free(DATA.M_ABGB);
free(DATA.M_CH4);
free(DATA.M_DOM);
free(DATA.M_ET);
free(DATA.M_EWT);
free(DATA.M_GPP);
free(DATA.M_LAI);
free(DATA.M_NBE);






free(DATA.M_PARS);
free(DATA.M_FLUXES);
free(DATA.M_POOLS);
free(DATA.M_P);
free(DATA.M_EDCD);

free(DATA.parmin);
free(DATA.parmax);
free(DATA.parname);

return 0;

}
