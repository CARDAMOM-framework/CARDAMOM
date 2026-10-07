// Step 0. Header Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <netcdf.h>

// Include core dependencies in order
#include "../../../CARDAMOM_GENERAL/GLOBAL_CONSTANTS.c"
#include "../../../CARDAMOM_GENERAL/CARDAMOM_MODULE_IDX.c"
#include "../../../CARDAMOM_GENERAL/CARDAMOM_NETCDF_DATA_STRUCTURE.c"
#include "../../../CARDAMOM_GENERAL/CARDAMOM_DATA_STRUCTURE.c"

// Include DALEC_1100
#include "DALEC_1100.c"

int main() {
    int retval, ncid_cbf, ncid_cbr, ncid_out;
    int time_dimid;
    size_t n_time;

    printf("====================================================\n");
    printf(" Running TEST_DALEC_1100 (JAX Validation Harness)\n");
    printf("====================================================\n");

    // -------------------------------------------------------------------------
    // Step 1. Read Drivers from .cbf.nc
    // -------------------------------------------------------------------------
    char *cbf_path = "../../../../../../CARDAMOM-DEV/DATA/CARDAMOM_TEST_DRIVERS_APR26.cbf.nc";
    printf("Opening CBF file: %s\n", cbf_path);
    if ((retval = nc_open(cbf_path, NC_NOWRITE, &ncid_cbf))) {
        printf("Error opening CBF file: %s\n", nc_strerror(retval));
        return 1;
    }
    printf("CBF file opened successfully\n");

    printf("Reading time dimension...\n");
    nc_inq_dimid(ncid_cbf, "time", &time_dimid);
    nc_inq_dimlen(ncid_cbf, time_dimid, &n_time);
    printf("Time dimension length: %zu\n", n_time);

    printf("Initializing DATA structure...\n");
    DATA my_data;
    memset(&my_data, 0, sizeof(DATA));
    my_data.ncdf_data.TIME_INDEX.length = n_time;
    my_data.ncdf_data.Ntimesteps = n_time;
    printf("DATA structure initialized\n");

    // Helper macro to allocate and read double arrays
    #define READ_NCDF_ARR(ncid, name, field, len) { \
        field.length = len; \
        field.values = malloc(len * sizeof(double)); \
        int v_id; \
        if (nc_inq_varid(ncid, name, &v_id) == NC_NOERR) { \
            nc_get_var_double(ncid, v_id, field.values); \
        } else { \
            printf("Error reading variable %s\n", name); \
        } \
    }

    printf("Reading driver variables...\n");
    READ_NCDF_ARR(ncid_cbf, "time",             my_data.ncdf_data.TIME_INDEX,       n_time);
    READ_NCDF_ARR(ncid_cbf, "SSRD",             my_data.ncdf_data.SSRD,             n_time);
    READ_NCDF_ARR(ncid_cbf, "T2M_MIN",          my_data.ncdf_data.T2M_MIN,          n_time);
    READ_NCDF_ARR(ncid_cbf, "T2M_MAX",          my_data.ncdf_data.T2M_MAX,          n_time);
    READ_NCDF_ARR(ncid_cbf, "CO2",              my_data.ncdf_data.CO2,              n_time);
    READ_NCDF_ARR(ncid_cbf, "DOY",              my_data.ncdf_data.DOY,              n_time);
    READ_NCDF_ARR(ncid_cbf, "TOTAL_PREC",       my_data.ncdf_data.TOTAL_PREC,       n_time);
    READ_NCDF_ARR(ncid_cbf, "VPD",              my_data.ncdf_data.VPD,              n_time);
    READ_NCDF_ARR(ncid_cbf, "BURNED_AREA",      my_data.ncdf_data.BURNED_AREA,      n_time);
    READ_NCDF_ARR(ncid_cbf, "SNOWFALL",         my_data.ncdf_data.SNOWFALL,         n_time);
    READ_NCDF_ARR(ncid_cbf, "SKT",              my_data.ncdf_data.SKT,              n_time);
    READ_NCDF_ARR(ncid_cbf, "STRD",             my_data.ncdf_data.STRD,             n_time);
    READ_NCDF_ARR(ncid_cbf, "DISTURBANCE_FLUX", my_data.ncdf_data.DISTURBANCE_FLUX, n_time);
    READ_NCDF_ARR(ncid_cbf, "YIELD",            my_data.ncdf_data.YIELD,            n_time);
    printf("Driver variables read successfully\n");


    printf("Reading latitude...\n");
    int lat_id;
    if (nc_inq_varid(ncid_cbf, "LAT", &lat_id) == NC_NOERR) {
        nc_get_var_double(ncid_cbf, lat_id, &my_data.ncdf_data.LAT);
        printf("LAT = %f\n", my_data.ncdf_data.LAT);
    }
    nc_close(ncid_cbf);
    printf("CBF file closed\n");

    // -------------------------------------------------------------------------
    // Step 2. Read Parameters from .cbr.nc
    // -------------------------------------------------------------------------
    char *cbr_path = "../../../../../../DUMPFILES/CARDAMOM_TEST_PARAMETERS_APR26.cbr.nc";
    printf("Opening CBR file: %s\n", cbr_path);
    if ((retval = nc_open(cbr_path, NC_NOWRITE, &ncid_cbr))) {
        printf("Error opening CBR file: %s\n", nc_strerror(retval));
        return 1;
    }

    double pars[89];
    int par_id;
    printf("Reading parameters...\n");
    nc_inq_varid(ncid_cbr, "Parameters", &par_id);
    nc_get_var_double(ncid_cbr, par_id, pars);
    nc_close(ncid_cbr);
    printf("Parameters read successfully\n");

    // -------------------------------------------------------------------------
    // Step 3. Configure & Execute DALEC_1100
    // -------------------------------------------------------------------------
    printf("Initializing DALEC model structure...\n");
    DALEC model;
    memset(&model, 0, sizeof(DALEC));
    my_data.MODEL = &model;

    printf("Calling DALEC_1100_MODCONFIG...\n");
    DALEC_1100_MODCONFIG(&model, &my_data);
    printf("DALEC_1100_MODCONFIG completed\n");

    // Copy model dimensions to DATA structure
    my_data.nofluxes = model.nofluxes;
    my_data.nopools = model.nopools;
    my_data.nopars = model.nopars;
    printf("Model dimensions: nopools=%d, nofluxes=%d, nopars=%d\n",
           model.nopools, model.nofluxes, model.nopars);

    printf("Allocating memory for fluxes and pools...\n");
    my_data.M_FLUXES = calloc(n_time * model.nofluxes, sizeof(double));
    my_data.M_POOLS  = calloc((n_time + 1) * model.nopools, sizeof(double));
    if (!my_data.M_FLUXES || !my_data.M_POOLS) {
        printf("Error: Failed to allocate memory\n");
        return 1;
    }
    printf("Memory allocated successfully\n");

    printf("Executing C model run (%zu timesteps)...\n", n_time);
    DALEC_1100(my_data, pars);
    printf("Model execution completed\n");

    // -------------------------------------------------------------------------
    // Step 4. Write Verification Outputs (TEST_DALEC_1100.output.nc)
    // -------------------------------------------------------------------------
    char *out_path = "TEST_DALEC_1100.output.nc";
    if ((retval = nc_create(out_path, NC_CLOBBER | NC_NETCDF4, &ncid_out))) {
        printf("Error creating output NetCDF file: %s\n", nc_strerror(retval));
        return 1;
    }

    int dim_sample, dim_pool, dim_flux, dim_time_pools, dim_time_fluxes;
    nc_def_dim(ncid_out, "Sample", 1, &dim_sample);
    nc_def_dim(ncid_out, "Pool", model.nopools, &dim_pool);
    nc_def_dim(ncid_out, "Flux", model.nofluxes, &dim_flux);
    nc_def_dim(ncid_out, "Time_pools", n_time + 1, &dim_time_pools);
    nc_def_dim(ncid_out, "Time_fluxes", n_time, &dim_time_fluxes);

    int var_fluxes, var_pools;
    int dimids_fluxes[3] = {dim_sample, dim_time_fluxes, dim_flux};
    int dimids_pools[3]  = {dim_sample, dim_time_pools, dim_pool};

    nc_def_var(ncid_out, "FLUXES", NC_DOUBLE, 3, dimids_fluxes, &var_fluxes);
    nc_def_var(ncid_out, "POOLS",  NC_DOUBLE, 3, dimids_pools,  &var_pools);
    nc_enddef(ncid_out);

    nc_put_var_double(ncid_out, var_fluxes, my_data.M_FLUXES);
    nc_put_var_double(ncid_out, var_pools,  my_data.M_POOLS);
    nc_close(ncid_out);

    printf("Output successfully written to %s\n", out_path);
    printf("====================================================\n");

    return 0;
}