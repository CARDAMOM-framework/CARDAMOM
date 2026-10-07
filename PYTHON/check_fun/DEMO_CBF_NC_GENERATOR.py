import os
import sys
import numpy as np
import netCDF4 as nc

def generate_cbf_nc():
    # Define paths based on script location (CARDAMOM/PYTHON/check_fun/)
    script_dir = os.path.dirname(os.path.abspath(__file__))
    repo_root = os.path.normpath(os.path.join(script_dir, '../..'))
    txt_dir = os.path.join(repo_root, 'DATA/TEXT_FILE_DRIVERS')
    output_filename = os.path.join(repo_root, 'CARDAMOM_DEMO_INPUT_FILE.cbf.nc')
    
    if not os.path.exists(txt_dir):
        print(f"🛑 FATAL ERROR: Driver directory not found at {txt_dir}")
        sys.exit(1)

    # Define the exact file structure, dimensions, and attributes based on the ncdump
    time_dim_size = 216
    
    # Dictionary structure: 
    # 'VAR_NAME': ('dimension_tuple', fill_value, {'attr_name': 'attr_value'})
    nc_structure = {
        'LAT': ((), None, {'units': 'degrees'}),
        'time': (('time_dim',), -9999., {'units': 'days since 2001-01-01', 'calendar': 'proleptic_gregorian'}),
        'LON': ((), None, {'units': 'degrees'}),
        'T2M_MAX': (('time_dim',), -9999., {'units': 'deg C', 'variable_info': 'ERA5 0.5 degree dataset: monthly average daily maximum temperature', 'coordinates': 'LAT LON'}),
        'T2M_MIN': (('time_dim',), -9999., {'units': 'deg C', 'variable_info': 'ERA5 0.5 degree dataset: monthly average daily minimum temperature', 'coordinates': 'LAT LON'}),
        'SNOWFALL': (('time_dim',), -9999., {'units': 'mm/day', 'variable_info': 'ERA5 0.5 degree dataset: monthly total snow fall', 'coordinates': 'LAT LON'}),
        'SSRD': (('time_dim',), -9999., {'units': 'MJ m**-2 day**-1', 'variable_info': 'ERA5 0.5 degree dataset: monthly average shortwave  downward radiation', 'coordinates': 'LAT LON'}),
        'STRD': (('time_dim',), -9999., {'units': 'MJ m**-2 day**-1', 'variable_info': 'ERA5 0.5 degree dataset: monthly average thermal downward radiation', 'coordinates': 'LAT LON'}),
        'SKT': (('time_dim',), -9999., {'units': 'C', 'variable_info': 'ERA5 0.5 degree dataset: monthly average surface skin temp', 'coordinates': 'LAT LON'}),
        'VPD': (('time_dim',), -9999., {'units': 'VPD [hPa]', 'variable_info': 'ERA5 0.5 degree dataset: monthly average shortwave solar downward radiation', 'coordinates': 'LAT LON'}),
        'TOTAL_PREC': (('time_dim',), -9999., {'units': 'mm/day', 'variable_info': 'ERA5 0.5 degree dataset: monthly average shortwave solar downward radiation', 'coordinates': 'LAT LON'}),
        'BURNED_AREA': (('time_dim',), -9999., {'units': 'Burned area [m2/m2]', 'variable_info': 'GFEDv4.1s burned area at 0.5 degrees', 'coordinates': 'LAT LON'}),
        'CO2': (('time_dim',), -9999., {'units': 'CO2 [ppm]', 'variable_info': 'NOAA global mean surface atmospheric CO2 (not spatially resolved, mean replicated everywhere)', 'coordinates': 'LAT LON'}),
        'DISTURBANCE_FLUX': (('time_dim',), -9999., {'description': 'Non-fire disturbance flux', 'coordinates': 'LAT LON'}),
        'LAI': (('time_dim',), -9999., {'units': 'm2 m-2', 'variable_info': 'This data was gridded at JPL  LAI=DATASCRIPT_GRID_MODIS_LAI_APR17(res); contact AABLOOM abloom@jpl.nasa.gov', 'opt_unc_type': 1, 'single_unc': 1.5, 'coordinates': 'LAT LON'}),
        'ABGB': (('time_dim',), -9999., {'units': 'gC m-2', 'description': 'Above- and below-ground biomass', 'opt_unc_type': 1, 'single_annual_unc': 1.5, 'opt_filter': 3., 'coordinates': 'LAT LON'}),
        'SCF': (('time_dim',), -9999., {'units': '1', 'description': 'Snow cover fraction (0 = no snow, 1 = complete coverage)', 'opt_unc_type': 0, 'single_unc': 0.2, 'min_threshold': 0.02, 'coordinates': 'LAT LON'}),
        'PEQ_iniSOM': ((), None, {'units': 'gC m-2', 'variable_info': 'Gridded HWSD soil C at 360x720 grid', 'opt_unc_type': 1, 'unc': 1.5, 'coordinates': 'LAT LON'}),
        'YIELD': (('time_dim',), -9999., {'coordinates': 'LAT LON'}),
        'ID': ((), None, {'coordinates': 'LAT LON'}),
        'EDC': ((), None, {'coordinates': 'LAT LON'}),
        'MCMCID': ((), None, {'nITERATIONS': 500000., 'nSAMPLES': 10., 'nPRINT': 1000., 'seed_number': 0., 'nSAMPLES_EDC_SEARCH': 400., 'coordinates': 'LAT LON'}),
        'DOY': (('time_dim',), -9999., {'coordinates': 'LAT LON'}),
        'PEQ_CUE': ((), None, {'unc': 0.25, 'opt_unc_type': 0., 'coordinates': 'LAT LON'}),
        'NBE': (('time_dim',), -9999., {'opt_filter': 0., 'opt_unc_type': 0., 'single_unc': 1.}),
        'H': (('time_dim',), -9999., {'opt_filter': 0., 'opt_unc_type': 0., 'single_unc': 35.157567844103}),
        'LE': (('time_dim',), -9999., {'opt_filter': 0., 'opt_unc_type': 0., 'single_unc': 33.5804637921207, 'min_threshold': 0.1})
    }

    print(f"Creating {output_filename}...")
    ds = nc.Dataset(output_filename, 'w', format='NETCDF4')
    
    # Create dimension
    ds.createDimension('time_dim', time_dim_size)

    # Loop through required variables, read from txt, and construct NC
    for var_name, (dims, fill_val, attrs) in nc_structure.items():
        txt_path = os.path.join(txt_dir, f"{var_name}.txt")
        
        # Initialize netCDF variable
        if fill_val is not None:
            nc_var = ds.createVariable(var_name, 'f8', dims, fill_value=fill_val)
        else:
            nc_var = ds.createVariable(var_name, 'f8', dims)
            
        # Assign attributes
        for attr_name, attr_val in attrs.items():
            setattr(nc_var, attr_name, attr_val)

        # Load data if text file exists, else populate with fill value
        if os.path.exists(txt_path):
            try:
                data = np.loadtxt(txt_path)
                
                # Check if dimensionality matches expectations
                if dims == ('time_dim',) and data.size != time_dim_size:
                    print(f"⚠️ WARNING: {var_name}.txt size ({data.size}) does not match time_dim ({time_dim_size}). Padding/Truncating...")
                    formatted_data = np.full(time_dim_size, fill_val if fill_val else np.nan)
                    min_len = min(data.size, time_dim_size)
                    formatted_data[:min_len] = data[:min_len]
                    nc_var[:] = formatted_data
                elif dims == ():
                    # Scalar
                    nc_var[:] = data.item() if data.size == 1 else data[0]
                else:
                    nc_var[:] = data
                    
            except Exception as e:
                print(f"❌ ERROR: Failed to read or assign data for {var_name}.txt. Details: {e}")
        else:
            print(f"⚠️ WARNING: {var_name}.txt not found in {txt_dir}. Initializing with fill values.")
            if dims == ('time_dim',):
                nc_var[:] = np.full(time_dim_size, fill_val if fill_val else np.nan)
            elif dims == ():
                # Provide a fallback scalar if no file and no fill_value
                nc_var[:] = fill_val if fill_val else 0.0

    ds.close()
    print(f"✅ Successfully generated {output_filename}")

if __name__ == "__main__":
    generate_cbf_nc()


