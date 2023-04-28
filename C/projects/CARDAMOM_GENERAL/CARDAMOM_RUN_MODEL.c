#include "math.h"
#include <stdio.h>
#include "CARDAMOM_READ_BINARY_DATA.c"
#include <netcdf.h>


/* Handle netCDF library errors by printing an error message and exiting with a
 * non-zero status.*/
#define ERREXITCODE 2
#define NCDFERR(e) {printf("Error in %s at %d: %s\n", __FILE__, __LINE__, nc_strerror(e));}
#define FILE_NAME_MAX_LEN 1000

//This is a tiny macro used to do error handeling for netCDF methods with the standard format of returning a result code.
//It helps make the code look less insane by allowing one-line calls
//This as a macro because we want the __FILE__ and __LINE__ macros to work
#define FAILONERROR(ncretval) if (ncretval != NC_NOERR) {NCDFERR(ncretval); exit(ERREXITCODE);}
//This variant will not die, but will still thow a message at the user about the problem.
#define WARNONERROR(ncretval) if (ncretval != NC_NOERR) {NCDFERR(ncretval);}





//This scans the string and removes all instances of the string toFind, and replaces them with the single char toReplace.
void str_inplace_replace(char * str, const char * toFind, const char toReplace){
  //Yeah this implementation is N^2... but we have a small fixed max N, so don't @ me.
  char* substring=str;
  while ((substring = strstr(substring, toFind))){
    memmove(substring, substring+1,strlen(substring)); //Memmove needed due to overlapping strings. Move the whole string, with null term
    substring[0]=toReplace; //put in the filler char
  }

}



/*syntax CARDAMOM_READ_BINARY_CARDADATA(char *filename,CARDADATA *CARDADATA)*/

/* the gateway function */
/*NOTE: this function can be used with any valid model ID (stored in CARDADATA.ID)*/
int main(int argc, char *files[])
{
    printf("In 'main'\n");

/*declaring loop variable n*/
int n, nn;

/*storing command line inputs as 2 files*/
char metfile[FILE_NAME_MAX_LEN];strncpy(metfile,files[1],FILE_NAME_MAX_LEN-1);
char parfile[FILE_NAME_MAX_LEN];strncpy(parfile,files[2],FILE_NAME_MAX_LEN-1);
char ncdffile[FILE_NAME_MAX_LEN];
if (argc-1>2){strncpy(ncdffile,files[3],FILE_NAME_MAX_LEN-1);}
else{
  int ncFilenameLen=strlen(parfile)-4;
  if (ncFilenameLen > FILE_NAME_MAX_LEN-8){
    printf("Error on input validation, parfile name too long to create netCDF file name from.\n");
    exit(1);
  }
  strncpy(ncdffile, parfile,ncFilenameLen);
  strncpy(ncdffile+ncFilenameLen, ".cbr.nc",ncFilenameLen+1); //Write an extra char so strncpy adds a null
  str_inplace_replace(ncdffile, "//", '/'); //Remove double-slashes
  str_inplace_replace(ncdffile, ":", '-'); //Remove colons
}

//Add manditory null terminators just in case we used every char
metfile[FILE_NAME_MAX_LEN-1]=0;
parfile[FILE_NAME_MAX_LEN-1]=0;
ncdffile[FILE_NAME_MAX_LEN-1]=0;



/*declaring data structure*/
DATA CARDADATA;

/*if (&CARDADATA==0){printf("CARDADATA not assigned, expect error!\n");}
*//*Initialize data structure*/
int OK;
printf("About to read data structure...");
//OK=INITIALIZE_DATA_STRUCT(&CARDADATA);
printf("Done with initializing data structure!\n");
/*step 1 - read the met file & initialize model fields here*/
/*NOTE: metfile contains information on which CARDAMOM model is being run*/
/*therefore this function is transferable to any model type*/
/*NOTE: parameter minimum and maximum values also loaded as a function of CARDADATA.ID by
the CARDAMOM_MODEL_LIBRARY.c function*/

printf("About to read cbf data...");
CARDAMOM_READ_BINARY_DATA(metfile,&CARDADATA);
printf("done with reading cbf data!\n");
//CARDAMOM_READ_NETCDF_DATA("DEFAULT/FILE/NETCDF", &(CARDADATA.ncdf_data));


/*step 2 - read (first time) parameter file here*/

FILE *fd;
/*opening file*/
fd=fopen(parfile,"rb");
/*checking that this has been done successfully*/
filediag(fd,parfile);

/*identifying number of elements in file*/
fseek(fd,0,SEEK_END);
/*number of parameter vectors to run*/
int N=ftell(fd)/(sizeof(double)*(CARDADATA.nopars));

/*comments for display*/
int verbal=1;
if (verbal==1){printf("number of parvecs to run = %d\n",N);}

/*resetting file reader to start*/
fseek(fd,0,SEEK_SET);
/*parameter file will be closed later - in the meantime parameters will simply be read N times*/

/*declaring pars file*/
double *pars=calloc(CARDADATA.nopars,sizeof(double));





/*STEP 3 - declaring flux, pool and edc files*/
/*note - these should be input at command line as variables*/



/*STEP 3.1 - create netCDF output file*/
int ncid = 0; //This is the netcdf id num
int ncretval = 0; //This is a reused variable for the return value of ncdf methods.
ncretval = nc_create(ncdffile,NC_CLOBBER, &ncid );
if (ncretval != NC_NOERR){
  //If nc_create did anything but return no error, then fail
  ERR(ncretval);
}
int Ntimesteps=CARDADATA.ncdf_data.Ntimesteps;


/*STEP 3.2 - create netCDF output dimensions*/
int sampleDimID, poolDimID, fluxDimID, timePoolsDimID,timeFluxesDimID, probIdxDimID,edcIdxDimID, noParsDimID, noLikelihoodsDimID;
FAILONERROR(nc_def_dim(ncid,"Sample",N,&sampleDimID));
FAILONERROR(nc_def_dim(ncid,"Pool",CARDADATA.nopools,&poolDimID ));
FAILONERROR(nc_def_dim(ncid,"Flux",CARDADATA.nofluxes,&fluxDimID ));
//NOTE: this was going to be the NC_UNLIMITED dimension, however due to concerns with support for netcdf classic, it is now fixed, and split into two
FAILONERROR(nc_def_dim(ncid,"Time_pools",Ntimesteps+1,&timePoolsDimID));
FAILONERROR(nc_def_dim(ncid,"Time_fluxes",Ntimesteps,&timeFluxesDimID));
//Hard coded to 1
const size_t probIdxLen=1;
FAILONERROR(nc_def_dim(ncid,"Probability Index",probIdxLen,&probIdxDimID ));
//Hard coded to 100
const size_t edcIdxLen=100;
FAILONERROR(nc_def_dim(ncid,"EDC Index",edcIdxLen,&edcIdxDimID ));

FAILONERROR(nc_def_dim(ncid,"Parameter",CARDADATA.nopars,&noParsDimID ));
FAILONERROR(nc_def_dim(ncid,"Likelihood Index",CARDADATA.nolikelihoods,&noLikelihoodsDimID ));



/*STEP 3.3 - create netCDF variables in preperation for writting them later*/
int fluxesVarID, poolsVarID, edcdVarID, pVarID, parsVarID, likelihoodsVarID;

int fluxes_dems[] = {sampleDimID,timeFluxesDimID,fluxDimID};
FAILONERROR(nc_def_var(	ncid,"FLUXES" , NC_DOUBLE, 3, fluxes_dems, &fluxesVarID ));
//EXAMPLE ATTRIBUTES
char fluxesLowercaseName[]="fluxes";
WARNONERROR(nc_put_att_text	(	ncid,fluxesVarID,"example_lowercase_name",strlen(fluxesLowercaseName),fluxesLowercaseName));
//This is an example of an array of doubles. Yes, you do need to specify NC_DOUBLE even though we used the type-safe method nc_put_att_double
WARNONERROR(nc_put_att_double	(	ncid,fluxesVarID,"example_doubles",NC_DOUBLE,4,(double[]){12.44, 441.0, 3.14159265, 0.0}));



int pools_dems[] = {sampleDimID,timePoolsDimID,poolDimID};
FAILONERROR(nc_def_var(	ncid,"POOLS" , NC_DOUBLE, 3, pools_dems, &poolsVarID ));

int edcd_dems[] = {sampleDimID, edcIdxDimID };
FAILONERROR(nc_def_var(	ncid,"EDCD" , NC_INT, 2, edcd_dems, &edcdVarID ));

int prob_dems[] = {sampleDimID, probIdxDimID};
FAILONERROR(nc_def_var(	ncid,"PROB" , NC_DOUBLE, 2, prob_dems, &pVarID ));

int pars_dems[] = {sampleDimID, noParsDimID};
FAILONERROR(nc_def_var(	ncid,"PARS" , NC_DOUBLE, 2, pars_dems, &parsVarID ));

int likelihoods_dems[] = {sampleDimID, noLikelihoodsDimID};
FAILONERROR(nc_def_var(	ncid,"LIKELIHOODS" , NC_DOUBLE, 2, likelihoods_dems, &likelihoodsVarID ));


//End NetCDF definition phase, in order to allow for writting
nc_enddef(ncid);



/*STEP 4 - RUNNING CARDADATA.MLF N TIMES*/
for (n=0;n<N;n++){

/*step 4.1 - reading parameter vector from file*/
fread(pars,sizeof(double),CARDADATA.nopars,fd);


/*step 4.2 - running MODEL_LIKELIHOOD_FUNCTION (e.g. DALEC_CDEA_MLF) with parameter vector from file*/
/*This is stored in CARDADATA.MLF*/
/*Setting EDCDIAG = 1 to ensure full model run*/
CARDADATA.ncdf_data.EDCDIAG=1;


CARDADATA.MLF(CARDADATA,pars);


// printf("DATA.M_P[0] = %2.2f\n",CARDADATA.M_P[0]);
// printf("DATA.LAI.values[0] = %2.2f\n",CARDADATA.ncdf_data.LAI.values[0]);
// printf("DATA.LAI.values[24] = %2.2f\n",CARDADATA.ncdf_data.LAI.values[24]);
// printf("DATA.M_POOLS[1] = %2.2f\n",CARDADATA.M_POOLS[1]);
// printf("CARDADATA.ncdf_data.LAI.opt_unc_type=%i\n",CARDADATA.ncdf_data.LAI.opt_unc_type);
// printf("CARDADATA.ncdf_data.LAI.opt_normalization=%i\n",CARDADATA.ncdf_data.LAI.opt_normalization);
// printf("CARDADATA.ncdf_data.LAI.opt_filter=%i\n",CARDADATA.ncdf_data.LAI.opt_filter);
// printf("CARDADATA.ncdf_data.LAI.min_threshold=%2.2f\n",CARDADATA.ncdf_data.LAI.min_threshold);
// printf("CARDADATA.ncdf_data.LAI.single_monthly_unc=%2.2f\n",CARDADATA.ncdf_data.LAI.single_monthly_unc);
// printf("CARDADATA.ncdf_data.LAI.single_annual_unc=%2.2f\n",CARDADATA.ncdf_data.LAI.single_annual_unc);
// printf("CARDADATA.ncdf_data.LAI.single_mean_unc=%2.2f\n",CARDADATA.ncdf_data.LAI.single_mean_unc);
// printf("CARDADATA.ncdf_data.LAI.single_unc=%2.2f\n",CARDADATA.ncdf_data.LAI.single_unc);
// printf("CARDADATA.ncdf_data.LAI.structural_unc=%2.2f\n",CARDADATA.ncdf_data.LAI.structural_unc);
// printf("CARDADATA.ncdf_data.LAI.unc[24]=%f\n",CARDADATA.ncdf_data.LAI.unc[24]);
// printf("CARDADATA.ncdf_data.LAI.length=%i\n",(int)CARDADATA.ncdf_data.LAI.length);
// printf("CARDADATA.ncdf_data.LAI.unc_length=%i\n",(int)CARDADATA.ncdf_data.LAI.unc_length);
// printf("CARDADATA.ncdf_data.LAI.valid_obs_length=%i\n",(int)CARDADATA.ncdf_data.LAI.valid_obs_length);
// 

// for (nn=0;nn<192;nn++){printf("%2.3f ",CARDADATA.M_POOLS[1+nn*9]);}
// printf("\n");
// for (nn=0;nn<54;nn++){printf("%2.3f ",CARDADATA.M_PARS[nn]);}
// printf("\n");


// double * values;//Timeseries of observation values
// //**********Variable attributes*************
// int opt_unc_type;//(0 = absolute sigma, 1 = uncertainty factor, 2 = sigma as fraction of value)
// int opt_normalization;//(0 = none, 1 = remove mean, 2 = divide by mean)
// int opt_filter;//(0 = no filter, 1 = mean only, 2==annual mean & monthly anomaly, 3 = seasonal cycle & inter-annual anomalies). 
// double min_threshold;//Minimum value threshold: model and/or data will be rounded up to this value (default = -inf)
// double single_monthly_unc;//Fields to be used only with Filter=2 AND opt_unc_type=0;
// double single_annual_unc;//Fields to be used only with Filter=2 AND opt_unc_type=0;
// double single_mean_unc;//Fields to be used only with Filter = 1;
// double single_unc;//
// double structural_unc;//this gets added to uncertainty in quadrature.
// //Auxiliary uncertainty variable, separate from timeseries variable
// double * unc;//Timeseries of uncertainty values
// //expand as needed
// size_t length;//
// size_t unc_length;//
// int valid_obs_length;//number of non-empty obs
// int * valid_obs_indices;//indices of non-empty obs

/*step 4.4 - writing DALEC fluxes and pools to netCDF file*/
//(with N (Number of samples) being another dimension, applied to all vars)

//Write fluxes
FAILONERROR(nc_put_vara_double(ncid,fluxesVarID,(const size_t []){n,0,0}, (const size_t[]){1,Ntimesteps,CARDADATA.nofluxes}, CARDADATA.M_FLUXES));
//Write pools
FAILONERROR(nc_put_vara_double(ncid,poolsVarID,(const size_t []){n,0,0}, (const size_t[]){1,Ntimesteps+1,CARDADATA.nopools}, CARDADATA.M_POOLS));
//write edcd
FAILONERROR(nc_put_vara_int(ncid,edcdVarID,(const size_t[]){n,0}, (const size_t[]){1,edcIdxLen}, CARDADATA.M_EDCD));
//write M_P
FAILONERROR(nc_put_vara_double(ncid,pVarID,(const size_t[]){n,0}, (const size_t[]){1,probIdxLen}, CARDADATA.M_P));
//write Pars
FAILONERROR(nc_put_vara_double(ncid,parsVarID,(const size_t[]){n,0}, (const size_t[]){1,CARDADATA.nopars}, pars));
//Write Likelihoods
FAILONERROR(nc_put_vara_double(ncid,likelihoodsVarID,(const size_t[]){n,0}, (const size_t[]){1,CARDADATA.nolikelihoods}, CARDADATA.M_LIKELIHOODS));


//for (n=0;n<CARDADATA.nolikelihoods;n++){printf("%5.4f  ",CARDADATA.M_LIKELIHOODS[n]);}
//printf("\n");


//write attributes to netCDF file



/*STEP 4 completed*/
}


/*STEP 5 - close all files*/
fclose(fd);

FAILONERROR(nc_close(ncid));

/*Step 6: Free memory*/
/*exhaustive list of all malloc/calloc used fields*/
free(pars);
FREE_DATA_STRUCT(CARDADATA);





return 0;

}
