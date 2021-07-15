#include "../../auxi_fun/okcheck.c"
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
//Add manditory null terminators just in case we used every char
metfile[FILE_NAME_MAX_LEN-1]=0;
parfile[FILE_NAME_MAX_LEN-1]=0;

int ncFilenameLen=strlen(parfile)-4;
if (ncFilenameLen > FILE_NAME_MAX_LEN-8){
  printf("Error on input validation, parfile name too long to create netCDF file name from.\n");
  exit(1);
}
char ncdffile[FILE_NAME_MAX_LEN]; strncpy(ncdffile, parfile,ncFilenameLen);
strncpy(ncdffile+ncFilenameLen, ".nc.cbr",ncFilenameLen+1); //Write an extra char so strncpy adds a null
str_inplace_replace(ncdffile, "//", '/'); //Remove double-slashes
str_inplace_replace(ncdffile, ":", '-'); //Remove colons





/*declaring data structure*/
DATA CARDADATA;

/*if (&CARDADATA==0){printf("CARDADATA not assigned, expect error!\n");}
*//*Initialize data structure*/
int OK;
printf("About to read data structure...");
OK=INITIALIZE_DATA_STRUCT(&CARDADATA);
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

printf("Line 123...\n");
char fluxfile[1000];
char poolfile[1000];
char edcdfile[1000];
char probfile[1000];

if (argc-1>2){
strcpy(fluxfile,files[3]);
strcpy(poolfile,files[4]);
strcpy(edcdfile,files[5]);
strcpy(probfile,files[6]);}
else{
strcpy(fluxfile,"tempcardafluxfile.bin");
strcpy(poolfile,"tempcardapoolfile.bin");
strcpy(edcdfile,"tempcardaedcdfile.bin");
strcpy(probfile,"tempcardaprobfile.bin");}


FILE *fdf=fopen(fluxfile,"wb");
filediag(fdf,fluxfile);
FILE *fdp=fopen(poolfile,"wb");
filediag(fdp,poolfile);
FILE *fde=fopen(edcdfile,"wb");
filediag(fde,edcdfile);
FILE *fdpro=fopen(probfile,"wb");
filediag(fdpro,probfile);

printf("Line 150...\n");
/*STEP 3.1 - create netCDF output file*/
int ncid = 0; //This is the netcdf id num
int ncretval = 0; //This is a reused variable for the return value of ncdf methods.
ncretval = nc_create(ncdffile,NC_NOCLOBBER, &ncid );
if (ncretval != NC_NOERR){
  //If nc_create did anything but return no error, then fail
  ERR(ncretval);
}


/*STEP 3.2 - create netCDF output dimensions*/
int sampleDimID, poolDimID, fluxDimID, timePoolsDimID,timeFluxesDimID, probIdxDimID,edcIdxDimID, noParsDimID;
FAILONERROR(nc_def_dim(ncid,"Sample",N,&sampleDimID));
FAILONERROR(nc_def_dim(ncid,"Pool",CARDADATA.nopools,&poolDimID ));
FAILONERROR(nc_def_dim(ncid,"Flux",CARDADATA.nofluxes,&fluxDimID ));
//NOTE: this was going to be the NC_UNLIMITED dimension, however due to concerns with support for netcdf classic, it is now fixed, and split into two
FAILONERROR(nc_def_dim(ncid,"Time_pools",CARDADATA.nodays+1,&timePoolsDimID));
FAILONERROR(nc_def_dim(ncid,"Time_fluxes",CARDADATA.nodays,&timeFluxesDimID));
//Hard coded to 1
const size_t probIdxLen=1;
FAILONERROR(nc_def_dim(ncid,"Probability Index",probIdxLen,&probIdxDimID ));
//Hard coded to 100
const size_t edcIdxLen=100;
FAILONERROR(nc_def_dim(ncid,"EDC Index",edcIdxLen,&edcIdxDimID ));

FAILONERROR(nc_def_dim(ncid,"Parameter",CARDADATA.nopars,&noParsDimID ));


/*STEP 3.3 - create netCDF variables in preperation for writting them later*/
int fluxesVarID, poolsVarID, edcdVarID, pVarID, parsVarId;

int fluxes_dems[] = {sampleDimID,timeFluxesDimID,fluxDimID};
FAILONERROR(nc_def_var(	ncid,"FLUXES" , NC_DOUBLE, 3, fluxes_dems, &fluxesVarID ));
//EXAMPLE ATTRIBUTES
char fluxesLowercaseName[]="fluxes";
WARNONERROR(nc_put_att_text	(	ncid,fluxesVarID,"example_lowercase_name",strlen(fluxesLowercaseName),fluxesLowercaseName));
//This is an example of an array of doubles. Yes, you do need to specify NC_DOUBLE even though we used the type-safe method nc_put_att_double
WARNONERROR(nc_put_att_double	(	ncid,fluxesVarID,"example_doubles",NC_DOUBLE,4,(double[]){12.44, 441.0, 3.14159265, 0.0}));

printf("Line 190...\n");


int pools_dems[] = {sampleDimID,timePoolsDimID,poolDimID};
FAILONERROR(nc_def_var(	ncid,"POOLS" , NC_DOUBLE, 3, pools_dems, &poolsVarID ));

int edcd_dems[] = {sampleDimID, edcIdxDimID };
FAILONERROR(nc_def_var(	ncid,"EDCD" , NC_INT, 2, edcd_dems, &edcdVarID ));

int prob_dems[] = {sampleDimID, probIdxDimID};
FAILONERROR(nc_def_var(	ncid,"PROB" , NC_DOUBLE, 2, prob_dems, &pVarID ));

int pars_dems[] = {sampleDimID, noParsDimID};
FAILONERROR(nc_def_var(	ncid,"PARS" , NC_DOUBLE, 2, pars_dems, &parsVarId ));


printf("Line 206...\n");
//End NetCDF definition phase, in order to allow for writting
nc_enddef(ncid);



/*STEP 4 - RUNNING CARDADATA.MLF N TIMES*/
for (n=0;n<N;n++){

/*step 4.1 - reading parameter vector from file*/
fread(pars,sizeof(double),CARDADATA.nopars,fd);


/*step 4.2 - running MODEL_LIKELIHOOD_FUNCTION (e.g. DALEC_CDEA_MLF) with parameter vector from file*/
/*This is stored in CARDADATA.MLF*/
/*Setting EDCDIAG = 1 to ensure full model run*/
CARDADATA.EDCDIAG=1;

printf("Line 224...\n");
CARDADATA.MLF(CARDADATA,pars);
printf("Line 226...\n");
/*step 4.3 - writing DALEC fluxes and pools to file*/
fwrite(CARDADATA.M_FLUXES,sizeof(double),CARDADATA.nodays*CARDADATA.nofluxes,fdf);
fwrite(CARDADATA.M_POOLS,sizeof(double),(CARDADATA.nodays+1)*CARDADATA.nopools,fdp);
fwrite(CARDADATA.M_EDCD,sizeof(int),100,fde);
fwrite(CARDADATA.M_P,sizeof(double),1,fdpro);
/*writing as double - platform issues related to "int"*/
/*double MEDCD[100];for (nn=0;nn<100;nn++){MEDCD[nn]=(double)CARDADATA.M_EDCD[nn];};fwrite(MEDCD,sizeof(double),100,fde);
*/
printf("Line 235...\n");
/*step 4.4 - writing DALEC fluxes and pools to netCDF file*/
//(with N (Number of samples) being another dimension, applied to all vars)

//Write fluxes
FAILONERROR(nc_put_vara_double(ncid,fluxesVarID,(const size_t []){n,0,0}, (const size_t[]){1,CARDADATA.nodays,CARDADATA.nofluxes}, CARDADATA.M_FLUXES));
//Write pools
FAILONERROR(nc_put_vara_double(ncid,poolsVarID,(const size_t []){n,0,0}, (const size_t[]){1,CARDADATA.nodays+1,CARDADATA.nopools}, CARDADATA.M_POOLS));
//write edcd
FAILONERROR(nc_put_vara_int(ncid,edcdVarID,(const size_t[]){n,0}, (const size_t[]){1,edcIdxLen}, CARDADATA.M_EDCD));
//write M_P
FAILONERROR(nc_put_vara_double(ncid,pVarID,(const size_t[]){n,0}, (const size_t[]){1,probIdxLen}, CARDADATA.M_P));
//write Pars
FAILONERROR(nc_put_vara_double(ncid,parsVarId,(const size_t[]){n,0}, (const size_t[]){1,CARDADATA.nopars}, pars));


//write attributes to netCDF file



/*STEP 4 completed*/
}


/*STEP 5 - close all files*/
fclose(fdf);
fclose(fdp);
fclose(fde);
fclose(fdpro);
fclose(fd);

FAILONERROR(nc_close(ncid));

/*Step 6: Free memory*/
/*exhaustive list of all malloc/calloc used fields*/
free(pars);
FREE_DATA_STRUCT(CARDADATA);





return 0;

}
