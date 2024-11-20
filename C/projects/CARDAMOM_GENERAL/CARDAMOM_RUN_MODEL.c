#include "../../auxi_fun/okcheck.c"
#include "math.h"
#include <stdio.h>
#include "CARDAMOM_READ_BINARY_DATA.c"
#include <netcdf.h>
#include <time.h>

#include <stdlib.h>
#include <unistd.h>

#include <search.h>

//Maximum length of metadata strings that will be allowed when writing to netcdf.
#define METADATA_MAX_LEN 100 

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


//Typesafe min macro without double evaluation
#define min(a,b) ({ __typeof__ (a) _a = (a);  __typeof__ (b) _b = (b);  _a < _b ? _a : _b; })


//This scans the string and removes all instances of the string toFind, and replaces them with the single char toReplace.
void str_inplace_replace(char * str, const char * toFind, const char toReplace){
  //Yeah this implementation is N^2... but we have a small fixed max N, so don't @ me.
  char* substring=str;
  while ((substring = strstr(substring, toFind))){
    memmove(substring, substring+1,strlen(substring)); //Memmove needed due to overlapping strings. Move the whole string, with null term
    substring[0]=toReplace; //put in the filler char
  }

}

//string based comparison function, for searching and sorting arrays of strings. 
int comp (const void * elem1, const void * elem2) 
{
  return strcmp(*(const char**)elem1,*(const char**)elem2);
}

//checks if the sorted list of strings list with length listLen contains the string str
int sortedListContains(char** list,size_t listLen,  const char* str){
  char* result = bsearch(str, list, listLen, sizeof(char*), comp);
  if (result!= NULL){
    return 1;
  }
  return 0;
}

//Function that uses strtok to return a correctly sized array of pointers of all the tokens. The array is itself allocated dynamically by this method.
char** break_string_dynamically(char* inString, const char* delimiters, size_t* count){
  char* inStrCopy = calloc(strlen(inString), sizeof(char));
  strcpy(inStrCopy, inString);
  *count = 0;
  //Run the method on the copy to pre-check the length of the ending array
  char* saveptr = NULL;
  char* tok = strtok_r (inStrCopy,delimiters, &saveptr);
  while (tok != NULL)
  {
    (*count)++;
    tok = strtok_r (NULL,delimiters, &saveptr);
  }
  free(inStrCopy);

  //Now we can get the real output tokens!
  char** outTokens = calloc(*count, sizeof(char*));
  saveptr = NULL;
  tok = strtok_r (inString,delimiters, &saveptr);
  int idx = 0;
  while (tok != NULL)
  {
    outTokens[idx] = tok;
    idx++;
    tok = strtok_r (NULL,delimiters, &saveptr);
  }
  return outTokens;

}


/*syntax CARDAMOM_READ_BINARY_CARDADATA(char *filename,CARDADATA *CARDADATA)*/

/* the gateway function */
/*NOTE: this function can be used with any valid model ID (stored in CARDADATA.ID)*/
int main(int argc, char *argv[])
{
  printf("In 'main'\n");


  char metfile[FILE_NAME_MAX_LEN];
  char parfile[FILE_NAME_MAX_LEN];
  char ncdffile[FILE_NAME_MAX_LEN];

//These are the lists of fluxes, pools, and pars that we are wanting to output. 
//if the count var for the list is 0, then ALL variables will be output (this happens later)
  size_t fluxListCount = 0;
  char** fluxListToOutput;
  size_t poolListCount = 0;
  char** poolListToOutput;
  size_t parListCount = 0;
  char** parListToOutput;
  //this string is a list of legal delimiters between variables, to be passed into strtok. See strtok docs for more info 
  const char* delimiters = " ,";
  
  int opt;
  while ((opt = getopt(argc, argv, "f:p:a:")) != -1) {
      switch (opt) {
      case 'f': fluxListToOutput=break_string_dynamically(optarg, delimiters, &fluxListCount); break;
      case 'p': poolListToOutput=break_string_dynamically(optarg, delimiters, &poolListCount); break;
      case 'a': parListToOutput=break_string_dynamically(optarg, delimiters, &parListCount); break;
      default:
          fprintf(stderr, "Usage: %s [-f fluxes] [-p pools] [-a pars] [metfile] [parfile] [ncdffile]\n", argv[0]);
          exit(EXIT_FAILURE);
      }
  }
  // Now optind (declared extern int by <unistd.h>) is the index of the first non-option argument.
  // If it is >= argc, there were no non-option arguments.

  //Sort our output lists for faster access later
  //This is important! future string searching functions rely upon a sorted list
  qsort(fluxListToOutput,fluxListCount, sizeof(*fluxListToOutput), comp);
  qsort(poolListToOutput,poolListCount, sizeof(*poolListToOutput), comp);
  qsort(parListToOutput,parListCount, sizeof(*parListToOutput), comp);


//TODO: remove me!
printf("Parsing fluxes: ");
for (int i = 0; i<fluxListCount; i++){
printf("%s;", fluxListToOutput[i] );
}
printf("\n");


/*storing command line inputs as 2 files*/
strncpy(metfile,argv[optind],FILE_NAME_MAX_LEN-1);
strncpy(parfile,argv[optind+1],FILE_NAME_MAX_LEN-1);

if (argc-optind>2){strncpy(ncdffile,argv[optind+2],FILE_NAME_MAX_LEN-1);}
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


//Add mandatory null terminators just in case we used every char
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
okcheck(OK,"CHECK: DATA structure successfully initialized");
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


int cbrId = 0; //This is the netcdf id num for the cbfcbr file
int cbrParsVarID; // Variable ID number
int cbrParamDimID,cbrSampleDimID; //Dim ID numbers, must be populated each invocation
FAILONERROR(nc_open(parfile,NC_NOWRITE, &cbrId ));
FAILONERROR(nc_inq_dimid(cbrId,"Parameter",&cbrParamDimID));
FAILONERROR(nc_inq_dimid(cbrId,"Sample", &cbrSampleDimID));
FAILONERROR(nc_inq_varid(cbrId,"Parameters",&cbrParsVarID));

/*number of parameter vectors to run*/
size_t N;
FAILONERROR(nc_inq_dimlen(cbrId,cbrSampleDimID, &N));


/*comments for display*/
int verbal=1;
if (verbal==1){printf("number of parvecs to run = %d\n",(int)N);}
/*declaring pars file*/
double *pars=calloc(CARDADATA.nopars,sizeof(double));
/*parameter file will be closed later - in the meantime parameters will simply be read N times*/





/*STEP 3 - declaring flux, pool and edc files*/
/*note - these should be input at command line as variables*/



/*STEP 3.1 - create netCDF output file*/
int ncid = 0; //This is the netcdf id num
int ncretval = 0; //This is a reused variable for the return value of ncdf methods.
ncretval = nc_create(ncdffile,NC_CLOBBER|NC_64BIT_OFFSET, &ncid );
if (ncretval != NC_NOERR){
  //If nc_create did anything but return no error, then fail
  ERR(ncretval);
}
int Ntimesteps=CARDADATA.ncdf_data.Ntimesteps;


/*STEP 3.2 - create netCDF output dimensions*/
int sampleDimID,  timePoolsDimID,timeFluxesDimID, probIdxDimID,edcIdxDimID, noLikelihoodsDimID;
FAILONERROR(nc_def_dim(ncid,"Sample",N,&sampleDimID));


int poolDimID;
FAILONERROR(nc_def_dim(ncid,"Pool",CARDADATA.nopools,&poolDimID ));
int fluxDimID;
FAILONERROR(nc_def_dim(ncid,"Flux",CARDADATA.nofluxes,&fluxDimID ));
int noParsDimID;
FAILONERROR(nc_def_dim(ncid,"Parameter",CARDADATA.nopars,&noParsDimID ));


//NOTE: this was going to be the NC_UNLIMITED dimension, however due to concerns with support for netcdf classic, it is now fixed, and split into two
FAILONERROR(nc_def_dim(ncid,"Time_pools",Ntimesteps+1,&timePoolsDimID));
FAILONERROR(nc_def_dim(ncid,"Time_fluxes",Ntimesteps,&timeFluxesDimID));
//Hard coded to 1
const size_t probIdxLen=1;
FAILONERROR(nc_def_dim(ncid,"Probability Index",probIdxLen,&probIdxDimID ));
//Hard coded to 100
FAILONERROR(nc_def_dim(ncid,"EDC Index",CARDADATA.noedcs,&edcIdxDimID ));

FAILONERROR(nc_def_dim(ncid,"Likelihood Index",CARDADATA.nolikelihoods,&noLikelihoodsDimID ));
int chidDimID; //This is the charicter position dimension. It is used for strings, and fixed to a max of 200 chars

FAILONERROR(nc_def_dim(ncid,"chid",METADATA_MAX_LEN,&chidDimID ));


//size_t chunkSize = (size_t) Ntimesteps * sizeof(double);
/*STEP 3.3 - create netCDF variables in preparation for writing them later*/
int edcsVarID, pVarID, likelihoodsVarID;

//FLUXES DEFINITION
int fluxesVarID, fluxesNameVarID,fluxesDescriptionVarID, fluxesUnitVarID;
//This is the int array that gets re-used for each flux, letting the nc_def_var function know that each flux needs to be based on the sample and time dimensions 
int fluxes_dems[] = {sampleDimID,timeFluxesDimID,fluxDimID};
int fluxes_meta_dems[] = {fluxDimID, chidDimID};

FAILONERROR(nc_def_var(	ncid,"FLUXES" , NC_DOUBLE, 3, fluxes_dems, &(fluxesVarID) ));

//Create each flux's mapping as an attribute
struct FLUX_META_STRUCT fluxInfo = ((DALEC *)CARDADATA.MODEL)->FLUX_META;
for(int i = 0; i < CARDADATA.nofluxes; i++){
  const char* ncVarAbbreviation =(const char *) calloc(sizeof(char), METADATA_MAX_LEN );//WARNING: DO NOT FREE THIS ARRAY! Netcdf libs require a const char*, so whatever is inside the string should not change or be freed!

  if (fluxInfo.ABBREVIATION != NULL && fluxInfo.ABBREVIATION[i] != NULL){
    snprintf( (char *) ncVarAbbreviation,METADATA_MAX_LEN-1,"%s", fluxInfo.ABBREVIATION[i] );//Write to it once, overriding the const qualifier so it is set
  } else {
    //just make up an abbrev
    snprintf( (char *) ncVarAbbreviation,METADATA_MAX_LEN-1,"FLUX_%d", i);//Write to it once, overriding the const qualifier so it is set
    printf("ERROR in %s at %d: Flux ID %d has no defined ABBREVIATION in it's FLUX_META. Add it to your DALEC_####_NC_INFO.c file! This flux will be called %s until you do!\n", __FILE__, __LINE__,i,ncVarAbbreviation);

  }
  //FAILONERROR(nc_def_var(	ncid,ncVarAbbreviation , NC_DOUBLE, 2, fluxes_dems, &(fluxesVarID[i]) ));
  WARNONERROR(nc_put_att_int	(	ncid,fluxesVarID,ncVarAbbreviation,NC_INT,1,&i));
}
//metadata vars
FAILONERROR(nc_def_var(	ncid,"FLUX_NAMES" , NC_CHAR, 2, fluxes_meta_dems, &(fluxesNameVarID) ));
FAILONERROR(nc_def_var(	ncid,"FLUX_DESCRIPTIONS" , NC_CHAR, 2, fluxes_meta_dems, &(fluxesDescriptionVarID) ));
FAILONERROR(nc_def_var(	ncid,"FLUX_UNITS" , NC_CHAR, 2, fluxes_meta_dems, &(fluxesUnitVarID) ));


//POOLS DEFINITION
//Create each pool variable as its own var inside 
int poolsVarID,poolsNameVarID,poolsDescriptionVarID, poolsUnitVarID;
struct POOLS_META_STRUCT poolsInfo = ((DALEC *)CARDADATA.MODEL)->POOLS_META;
int pools_dems[] = {sampleDimID,timePoolsDimID, poolDimID}; //poolsDimId was last in the order
int pools_meta_dems[] = {poolDimID, chidDimID};

FAILONERROR(nc_def_var(	ncid,"POOLS" , NC_DOUBLE, 3, pools_dems, &(poolsVarID) ));


for(int i = 0; i < CARDADATA.nopools; i++){
  const char* ncVarAbbreviation =(const char *) calloc(sizeof(char), METADATA_MAX_LEN );//WARNING: DO NOT FREE THIS ARRAY! Netcdf libs require a const char*, so whatever is inside the string should not change or be freed!
  if (poolsInfo.ABBREVIATION != NULL && poolsInfo.ABBREVIATION[i] != NULL ){
    snprintf( (char *) ncVarAbbreviation,METADATA_MAX_LEN-1,"%s", poolsInfo.ABBREVIATION[i] );//Write to it once, overriding the const qualifier so it is set
  } else {
    //just make up a name
    snprintf( (char *) ncVarAbbreviation,METADATA_MAX_LEN-1,"POOL_%d", i);//Write to it once, overriding the const qualifier so it is set
    printf("ERROR in %s at %d: pool ID %d has no defined ABBREVIATION in it's POOLS_META. Add it to your DALEC_####_NC_INFO.c file! This pool will be called %s until you do!\n", __FILE__, __LINE__,i,ncVarAbbreviation);

  }
  WARNONERROR(nc_put_att_int	(	ncid,poolsVarID,ncVarAbbreviation,NC_INT,1,&i));


  /*if (poolsInfo.NAME != NULL && poolsInfo.NAME[i] != NULL){
    WARNONERROR(nc_put_att_text	(	ncid,poolsVarID[i],"Name",strlen(poolsInfo.NAME[i]),poolsInfo.NAME[i]));
  }
  if (poolsInfo.DESCRIPTION != NULL && poolsInfo.DESCRIPTION[i] != NULL){
    WARNONERROR(nc_put_att_text	(	ncid,poolsVarID[i],"Description",strlen(poolsInfo.DESCRIPTION[i]),poolsInfo.DESCRIPTION[i]));
  }
  if (poolsInfo.UNITS != NULL && poolsInfo.UNITS[i] != NULL){
    WARNONERROR(nc_put_att_text	(	ncid,poolsVarID[i],"Units",strlen(poolsInfo.UNITS[i]),poolsInfo.UNITS[i]));
  }*/
  
}
//metadata vars
FAILONERROR(nc_def_var(	ncid,"POOL_NAMES" , NC_CHAR, 2, pools_meta_dems, &(poolsNameVarID) ));
FAILONERROR(nc_def_var(	ncid,"POOL_DESCRIPTIONS" , NC_CHAR, 2, pools_meta_dems, &(poolsDescriptionVarID) ));
FAILONERROR(nc_def_var(	ncid,"POOL_UNITS" , NC_CHAR, 2, pools_meta_dems, &(poolsUnitVarID) ));



int prob_dems[] = {sampleDimID, probIdxDimID};
FAILONERROR(nc_def_var(	ncid,"PROB" , NC_DOUBLE, 2, prob_dems, &pVarID ));

//PARS DEFINITION
//Create each paramater variable as its own var inside 
int parsVarID, parsNameVarID,parsDescriptionVarID, parsUnitVarID;
struct PARS_META_STRUCT parsInfo = ((DALEC *)CARDADATA.MODEL)->PARS_META;
int pars_dems[] = {sampleDimID, noParsDimID}; //noParsDimID was last in the order
int pars_meta_dems[] = {noParsDimID, chidDimID};

FAILONERROR(nc_def_var(	ncid, "PARS", NC_DOUBLE, 2, pars_dems, &(parsVarID) ));



for(int i = 0; i < CARDADATA.nopars; i++){
  const char* ncVarAbbreviation =(const char *) calloc(sizeof(char), METADATA_MAX_LEN );//WARNING: DO NOT FREE THIS ARRAY! Netcdf libs require a const char*, so whatever is inside the string should not change or be freed!
  if (parsInfo.ABBREVIATION != NULL && parsInfo.ABBREVIATION[i] != NULL){
    snprintf( (char *) ncVarAbbreviation,METADATA_MAX_LEN-1,"%s", parsInfo.ABBREVIATION[i] );//Write to it once, overriding the const qualifier so it is set
  } else {
    //just make up a name
    snprintf( (char *) ncVarAbbreviation,METADATA_MAX_LEN-1,"PAR_%d", i); //Write to it once, overriding the const qualifier so it is set
    printf("ERROR in %s at %d: paramater ID %d has no defined ABBREVIATION in it's PARS_META. Add it to your DALEC_####_NC_INFO.c file! This paramater will be called %s until you do!\n", __FILE__, __LINE__,i,ncVarAbbreviation );
  }
  
  WARNONERROR(nc_put_att_int	(	ncid,parsVarID,ncVarAbbreviation,NC_INT,1,&i));
  /*
  if (parsInfo.NAME != NULL && parsInfo.NAME[i] != NULL){
    WARNONERROR(nc_put_att_text	(	ncid,parsVarID[i],"Name",strlen(parsInfo.NAME[i]),parsInfo.NAME[i]));
  }
  if (parsInfo.DESCRIPTION != NULL && parsInfo.DESCRIPTION[i] != NULL){
    WARNONERROR(nc_put_att_text	(	ncid,parsVarID[i],"Description",strlen(parsInfo.DESCRIPTION[i]),parsInfo.DESCRIPTION[i]));
  }
  if (parsInfo.UNITS != NULL && parsInfo.UNITS[i] != NULL){
    WARNONERROR(nc_put_att_text	(	ncid,parsVarID[i],"Units",strlen(parsInfo.UNITS[i]),parsInfo.UNITS[i]));
  }*/

}
//metadata vars
FAILONERROR(nc_def_var(	ncid,"PAR_NAMES" , NC_CHAR, 2, pars_meta_dems, &(parsNameVarID) ));
FAILONERROR(nc_def_var(	ncid,"PAR_DESCRIPTIONS" , NC_CHAR, 2, pars_meta_dems, &(parsDescriptionVarID) ));
FAILONERROR(nc_def_var(	ncid,"PAR_UNITS" , NC_CHAR, 2, pars_meta_dems, &(parsUnitVarID) ));


int likelihoods_dems[] = {sampleDimID, noLikelihoodsDimID};
FAILONERROR(nc_def_var(	ncid,"LIKELIHOODS" , NC_DOUBLE, 2, likelihoods_dems, &likelihoodsVarID ));

int edcs_dems[] = {sampleDimID, edcIdxDimID };
FAILONERROR(nc_def_var(	ncid,"EDCs" , NC_DOUBLE, 2, edcs_dems, &edcsVarID ));


//End NetCDF definition phase, in order to allow for writting
nc_enddef(ncid);


double         cpu_time_used=0;
/*declaring loop variable n*/
int n, nn;
/*STEP 4 - RUNNING CARDADATA.MLF N TIMES*/
for (n=0;n<N;n++){

/*step 4.1 - reading parameter vector from cbr file*/
//BE ADVISED YE WHO READS THIS CODE: This is from the CBR file represented by cbrId, so a totally seperate file from all the writing
FAILONERROR(nc_get_vara_double(cbrId,cbrParsVarID,(const size_t[]){n,0}, (const size_t[]){1,CARDADATA.nopars},pars));


/*step 4.2 - running MODEL_LIKELIHOOD_FUNCTION (e.g. DALEC_CDEA_MLF) with parameter vector from file*/
/*This is stored in CARDADATA.MLF*/
/*Setting EDCDIAG = 1 to ensure full model run*/
CARDADATA.ncdf_data.EDCDIAG=1;

clock_t    start = clock();//Start timer

CARDADATA.MLF(CARDADATA,pars);
clock_t    end = clock();//End timer

 cpu_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;



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


FAILONERROR(nc_put_vara_double(ncid,fluxesVarID,(const size_t []){n,0,0}, (const size_t[]){1,Ntimesteps,CARDADATA.nofluxes}, CARDADATA.M_FLUXES));
FAILONERROR(nc_put_vara_double(ncid,poolsVarID,(const size_t []){n,0,0}, (const size_t[]){1,Ntimesteps+1,CARDADATA.nopools}, CARDADATA.M_POOLS));
FAILONERROR(nc_put_vara_double(ncid,parsVarID,(const size_t[]){n,0}, (const size_t[]){1,CARDADATA.nopars}, pars));

//Insert Fluxes metadata
for(int i = 0; i < CARDADATA.nofluxes; i++){
  if (fluxInfo.NAME != NULL && fluxInfo.NAME[i] != NULL){
    //"Name"
    WARNONERROR(nc_put_vara_text	(	ncid,fluxesNameVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(fluxInfo.NAME[i]))},(const char *)fluxInfo.NAME[i]));
  }
  if (fluxInfo.DESCRIPTION != NULL && fluxInfo.DESCRIPTION[i] != NULL){
    //"Description"
    WARNONERROR(nc_put_vara_text	(	ncid,fluxesDescriptionVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(fluxInfo.DESCRIPTION[i]))},(const char *)fluxInfo.DESCRIPTION[i]));
  }
  if (fluxInfo.UNITS != NULL && fluxInfo.UNITS[i] != NULL){
    //"Units"
    WARNONERROR(nc_put_vara_text	(	ncid,fluxesUnitVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(fluxInfo.UNITS[i]))},(const char *)fluxInfo.UNITS[i]));
  }
}

//Insert Pools metadata
for(int i = 0; i < CARDADATA.nopools; i++){
  if (poolsInfo.NAME != NULL && poolsInfo.NAME[i] != NULL){
    //"Name"
    WARNONERROR(nc_put_vara_text	(	ncid,poolsNameVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(poolsInfo.NAME[i]))},(const char *)poolsInfo.NAME[i]));
  }
  if (poolsInfo.DESCRIPTION != NULL && poolsInfo.DESCRIPTION[i] != NULL){
    //"Description"
    WARNONERROR(nc_put_vara_text	(	ncid,poolsDescriptionVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(poolsInfo.DESCRIPTION[i]))},(const char *)poolsInfo.DESCRIPTION[i]));
  }
  if (poolsInfo.UNITS != NULL && poolsInfo.UNITS[i] != NULL){
    //"Units"
    WARNONERROR(nc_put_vara_text	(	ncid,poolsUnitVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(poolsInfo.UNITS[i]))},(const char *)poolsInfo.UNITS[i]));
  }
}

//Insert Pars metadata
for(int i = 0; i < CARDADATA.nopars; i++){
  if (parsInfo.NAME != NULL && parsInfo.NAME[i] != NULL){
    //"Name"
    WARNONERROR(nc_put_vara_text	(	ncid,parsNameVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(parsInfo.NAME[i]))},(const char *)parsInfo.NAME[i]));
  }
  if (parsInfo.DESCRIPTION != NULL && parsInfo.DESCRIPTION[i] != NULL){
    //"Description"
    WARNONERROR(nc_put_vara_text	(	ncid,parsDescriptionVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(parsInfo.DESCRIPTION[i]))},(const char *)parsInfo.DESCRIPTION[i]));
  }
  if (parsInfo.UNITS != NULL && parsInfo.UNITS[i] != NULL){
    //"Units"
    WARNONERROR(nc_put_vara_text	(	ncid,parsUnitVarID,(const size_t[]){i,0},(const size_t[]){1,min(METADATA_MAX_LEN-1,strlen(parsInfo.UNITS[i]))},(const char *)parsInfo.UNITS[i]));
  }
}



//write edcd
FAILONERROR(nc_put_vara_double(ncid,edcsVarID,(const size_t[]){n,0}, (const size_t[]){1,CARDADATA.noedcs}, CARDADATA.M_EDCs));
//write M_P
FAILONERROR(nc_put_vara_double(ncid,pVarID,(const size_t[]){n,0}, (const size_t[]){1,probIdxLen}, CARDADATA.M_P));
//Write Likelihoods
FAILONERROR(nc_put_vara_double(ncid,likelihoodsVarID,(const size_t[]){n,0}, (const size_t[]){1,CARDADATA.nolikelihoods}, CARDADATA.M_LIKELIHOODS));


//for (n=0;n<CARDADATA.nolikelihoods;n++){printf("%5.4f  ",CARDADATA.M_LIKELIHOODS[n]);}
//printf("\n");


//write attributes to netCDF file



/*STEP 4 completed*/
}


/*STEP 5 - close all files*/
FAILONERROR(nc_close(cbrId));

FAILONERROR(nc_close(ncid));

/*Step 6: Free memory*/
/*exhaustive list of all malloc/calloc used fields*/
free(pars);
FREE_DATA_STRUCT(CARDADATA);

printf("**********\n");
    printf("Average time used per forward model run = %6.4f milliseconds\n", 1000*cpu_time_used/(double)N);
printf("**********\n");


return 0;

}
