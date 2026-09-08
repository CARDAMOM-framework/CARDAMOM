// Standalone test for DALEC_1100 - avoids full CARDAMOM framework
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <netcdf.h>

// Include only the minimal required structures
#include "../../../CARDAMOM_GENERAL/GLOBAL_CONSTANTS.c"
#include "../../../CARDAMOM_GENERAL/CARDAMOM_MODULE_IDX.c"
#include "../../../CARDAMOM_GENERAL/CARDAMOM_NETCDF_DATA_STRUCTURE.c"
#include "../../../CARDAMOM_GENERAL/CARDAMOM_DATA_STRUCTURE.c"

// Include DALEC structures and model ONLY (not the full framework)
#include "DALEC_1100.c"

int main() {
    printf("====================================================\n");
    printf(" Running STANDALONE TEST_DALEC_1100\n");
    printf("====================================================\n");

    int retval, ncid_cbf, ncid_cbr, ncid_out;
    int time_dimid;
    size_t n_time;

    // Read CBF file
    char *cbf_path = "../../../../../../CARDAMOM-DEV/DATA/CARDAMOM_TEST_DRIVERS_APR26.cbf.nc";
    printf("Opening CBF file...\n");
    if ((retval = nc_open(cbf_path, NC_NOWRITE, &ncid_cbf))) {
        printf("Error opening CBF file: %s\n", nc_strerror(retval));
        return 1;
    }

    nc_inq_dimid(ncid_cbf, "time", &time_dimid);
    nc_inq_dimlen(ncid_cbf, time_dimid, &n_time);
    printf("Time steps: %zu\n", n_time);

    // Initialize DATA
    DATA my_data;
    memset(&my_data, 0, sizeof(DATA));
    my_data.ncdf_data.TIME_INDEX.length = n_time;
    my_data.ncdf_data.Ntimesteps = n_time;

    // Read variables
    #define READ_VAR(name, field) { \
        field.length = n_time; \
        field.values = malloc(n_time * sizeof(double)); \
        int v_id; \
        if (nc_inq_varid(ncid_cbf, name, &v_id) == NC_NOERR) { \
            nc_get_var_double(ncid_cbf, v_id, field.values); \
        } \
    }

    printf("Reading drivers...\n");
    READ_VAR("time", my_data.ncdf_data.TIME_INDEX);
    READ_VAR("SSRD", my_data.ncdf_data.SSRD);
    READ_VAR("T2M_MIN", my_data.ncdf_data.T2M_MIN);
    READ_VAR("T2M_MAX", my_data.ncdf_data.T2M_MAX);
    READ_VAR("CO2", my_data.ncdf_data.CO2);
    READ_VAR("DOY", my_data.ncdf_data.DOY);
    READ_VAR("TOTAL_PREC", my_data.ncdf_data.TOTAL_PREC);
    READ_VAR("VPD", my_data.ncdf_data.VPD);
    READ_VAR("BURNED_AREA", my_data.ncdf_data.BURNED_AREA);
    READ_VAR("SNOWFALL", my_data.ncdf_data.SNOWFALL);
    READ_VAR("SKT", my_data.ncdf_data.SKT);
    READ_VAR("STRD", my_data.ncdf_data.STRD);
    READ_VAR("DISTURBANCE_FLUX", my_data.ncdf_data.DISTURBANCE_FLUX);
    READ_VAR("YIELD", my_data.ncdf_data.YIELD);

    int lat_id;
    if (nc_inq_varid(ncid_cbf, "LAT", &lat_id) == NC_NOERR) {
        nc_get_var_double(ncid_cbf, lat_id, &my_data.ncdf_data.LAT);
    }
    nc_close(ncid_cbf);

    // Read parameters
    char *cbr_path = "../../../../../../DUMPFILES/CARDAMOM_TEST_PARAMETERS_APR26.cbr.nc";
    printf("Reading parameters...\n");
    if ((retval = nc_open(cbr_path, NC_NOWRITE, &ncid_cbr))) {
        printf("Error opening CBR file: %s\n", nc_strerror(retval));
        return 1;
    }

    double pars[89];
    int par_id;
    nc_inq_varid(ncid_cbr, "Parameters", &par_id);
    nc_get_var_double(ncid_cbr, par_id, pars);
    nc_close(ncid_cbr);

    // Setup model
    printf("Configuring model...\n");
    DALEC model;
    memset(&model, 0, sizeof(DALEC));
    my_data.MODEL = &model;

    DALEC_1100_MODCONFIG(&model, &my_data);

    my_data.nofluxes = model.nofluxes;
    my_data.nopools = model.nopools;
    my_data.nopars = model.nopars;

    printf("nopools=%d, nofluxes=%d, nopars=%d\n",
           model.nopools, model.nofluxes, model.nopars);

    // Allocate outputs
    my_data.M_FLUXES = calloc(n_time * model.nofluxes, sizeof(double));
    my_data.M_POOLS = calloc((n_time + 1) * model.nopools, sizeof(double));

    // Run model
    printf("Executing DALEC_1100...\n");
    DALEC_1100(my_data, pars);
    printf("Model completed!\n");

    // Write output
    char *out_path = "TEST_DALEC_1100.output.nc";
    printf("Writing output...\n");

    if ((retval = nc_create(out_path, NC_CLOBBER | NC_NETCDF4, &ncid_out))) {
        printf("Error creating output: %s\n", nc_strerror(retval));
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
    int dimids_pools[3] = {dim_sample, dim_time_pools, dim_pool};

    nc_def_var(ncid_out, "FLUXES", NC_DOUBLE, 3, dimids_fluxes, &var_fluxes);
    nc_def_var(ncid_out, "POOLS", NC_DOUBLE, 3, dimids_pools, &var_pools);
    nc_enddef(ncid_out);

    nc_put_var_double(ncid_out, var_fluxes, my_data.M_FLUXES);
    nc_put_var_double(ncid_out, var_pools, my_data.M_POOLS);
    nc_close(ncid_out);

    printf("====================================================\n");
    printf(" SUCCESS: Output written to %s\n", out_path);
    printf("====================================================\n");

    return 0;
}
