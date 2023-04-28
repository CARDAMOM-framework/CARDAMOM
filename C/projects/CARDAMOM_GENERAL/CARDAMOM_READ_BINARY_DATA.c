/*INCLUDING THE PARAMETER_INFO structure*/
/*Note: it remains unclear as to where this structure should be defined*/




#include "GLOBAL_CONSTANTS.c"



#pragma once
#include "../../auxi_fun/filediag.c"
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
printf("MODEL ID = %d\n",(int)DATA->ncdf_data.ID);
printf("No days = %d\n",DATA->ncdf_data.Ntimesteps);
printf("Mean Snow = %f\n",DATA->ncdf_data.SNOWFALL.reference_mean);
printf("Mean Temp = %f\n",DATA->ncdf_data.T2M_MIN.reference_mean);
printf("Mean Prec = %f\n",DATA->ncdf_data.TOTAL_PREC.reference_mean);
printf("Latitude = %f\n",DATA->ncdf_data.LAT);

printf("Ecological & Dynamic Constraints options\n");
printf("EDC likelihood option = %d\n",DATA->ncdf_data.EDC);
printf("EDC diagnostics option = %d\n",DATA->ncdf_data.EDCDIAG);


printf("*****END OF CBF FILE SUMMARY***");



return 0;}

int DYNAMIC_DATA_MEMORY_ALLOCATION(DATA * DATA){
	
int Ntimesteps=DATA->ncdf_data.Ntimesteps;
DATA->M_ABGB=calloc(Ntimesteps,sizeof(double));
DATA->M_CH4=calloc(Ntimesteps,sizeof(double));
DATA->M_CWOO=calloc(Ntimesteps,sizeof(double));
DATA->M_DOM=calloc(Ntimesteps,sizeof(double));
DATA->M_ET=calloc(Ntimesteps,sizeof(double));
DATA->M_EWT=calloc(Ntimesteps,sizeof(double));
DATA->M_GPP=calloc(Ntimesteps,sizeof(double));
DATA->M_SIF=calloc(Ntimesteps,sizeof(double));
DATA->M_LAI=calloc(Ntimesteps,sizeof(double));//Warning, LAI larger than this!
DATA->M_NBE=calloc(Ntimesteps,sizeof(double));
DATA->M_ROFF=calloc(Ntimesteps,sizeof(double));
DATA->M_SCF=calloc(Ntimesteps,sizeof(double));//Warning, LAI larger than this!




    


//Model variables 
	DATA->M_FLUXES=calloc(Ntimesteps*DATA->nofluxes,sizeof(double));
	DATA->M_POOLS=calloc((Ntimesteps+1)*DATA->nopools,sizeof(double));
   
	int noedc=100, noprob=1;
	DATA->M_EDCD=calloc(noedc,sizeof(int));
	DATA->M_P=calloc(noprob,sizeof(double));
	DATA->M_PARS=calloc(DATA->nopars,sizeof(double));
    DATA->M_LIKELIHOODS=calloc(DATA->nolikelihoods,sizeof(double));
	DATA->M_EDCs=calloc(DATA->noedcs,sizeof(double));



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





	CARDAMOM_MODEL_LIBRARY(DATA);
        /*This will eventually be obsolete*/






	/*NEXT STEPS: figure out how to ceclafre MET and other memory*/
	// READ GPP, LAI, etc.:









/*Placeholder: read netcdf fields here*/


printf("***About to do dynamic allocation***\n");


	DYNAMIC_DATA_MEMORY_ALLOCATION(DATA);

    
    

	CARDAMOM_DATA_CHECKS(DATA);


printf("Done reading all data...\n");

return 0;



}



int FREE_DATA_STRUCT(DATA DATA){


//Cost function observations
free(DATA.M_ABGB);
free(DATA.M_CH4);
free(DATA.M_DOM);
free(DATA.M_ET);
free(DATA.M_ROFF);
free(DATA.M_EWT);
free(DATA.M_GPP);
free(DATA.M_SIF);
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
