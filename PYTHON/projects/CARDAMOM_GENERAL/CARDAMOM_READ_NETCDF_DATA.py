import numpy as np
import jax.numpy as jnp
from netCDF4 import Dataset

def CARDAMOM_READ_NETCDF_DATA(nc_filepath):
    """
    Reads CARDAMOM NetCDF driver (.cbf.nc) file and formats forcings and initial states
    into JAX-compatible arrays for DALEC_1100.
    """
    with Dataset(nc_filepath, 'r') as nc:
        # Extract scalar properties
        lat = float(nc.variables['LAT'][0]) if nc.variables['LAT'].ndim > 0 else float(nc.variables['LAT'][:])
        
        # Determine simulation length from time variable
        time_var = nc.variables['time'][:]
        n_steps = len(time_var)
        
        # Safely extract time series variables (filling defaults if missing)
        def get_var(var_name, default_val=0.0):
            if var_name in nc.variables:
                arr = np.array(nc.variables[var_name][:])
                # Replace NetCDF fill values (-9999) with default_val
                arr = np.where(arr == -9999.0, default_val, arr)
                return arr
            else:
                return np.full(n_steps, default_val)

        # 1. Meteorological and Disturbance Forcings
        ssrd = get_var('SSRD')             # Surface shortwave radiation [MJ/m2/day]
        t2m_min = get_var('T2M_MIN')       # Min temperature [C]
        t2m_max = get_var('T2M_MAX')       # Max temperature [C]
        co2 = get_var('CO2', 400.0)        # CO2 concentration [ppm]
        prec = get_var('TOTAL_PREC')       # Precipitation [mm/day]
        vpd = get_var('VPD')               # Vapor Pressure Deficit [hPa]
        burned_area = get_var('BURNED_AREA')# Burned Area [m2/m2]
        snowfall = get_var('SNOWFALL')     # Snowfall [mm/day]
        skt = get_var('SKT')               # Surface Skin Temp [C]
        strd = get_var('STRD')             # Thermal downward radiation [MJ/m2/day]
        disturbance = get_var('DISTURBANCE_FLUX') # Non-fire disturbance flux
        yield_var = get_var('YIELD')       # Crop Yield
        doy = get_var('DOY')               # Day of Year

        # Construct 14-column forcing matrix matching dalec_1100_step order:
        # [SSRD, T2M_MIN, T2M_MAX, CO2, PREC, VPD, BURNED_AREA, SNOWFALL, SKT, STRD, DIST, YIELD, DOY, LAT]
        lat_array = np.full(n_steps, lat)
        
        forcings_matrix = np.column_stack([
            ssrd, t2m_min, t2m_max, co2, prec, vpd, burned_area,
            snowfall, skt, strd, disturbance, yield_var, doy, lat_array
        ])

        # 2. Observation Targets (for MLF calculation)
        obs_dict = {}
        obs_unc = {}
        
        timeseries_obs = ['ABGB', 'LAI', 'SCF', 'NBE', 'H', 'LE']
        for obs_name in timeseries_obs:
            if obs_name in nc.variables:
                var = nc.variables[obs_name]
                data = np.array(var[:])
                obs_dict[obs_name] = jnp.array(data)
                
                # Check for single_unc or default
                unc = getattr(var, 'single_unc', 1.0)
                obs_unc[obs_name] = float(unc)

    return jnp.array(forcings_matrix), lat, obs_dict, obs_unc
