import netCDF4 as nc
import numpy as np

# --- Data-loading Subroutines ---
# Each function provides the data for a single variable, truncated to one decimal point.

def truncate(data, decimals=1):
    """Truncates a numpy array to a certain number of decimals."""
    if isinstance(data, np.ndarray):
        factor = 10.0 ** decimals
        return np.trunc(data * factor) / factor
    else: # For single numbers
        factor = 10.0 ** decimals
        return int(data * factor) / factor

def get_time_data():
    data = np.array([15.21, 45.65, 76.09, 106.53, 136.96, 167.40, 197.84, 228.28, 258.71, 289.15, 319.59, 350.03, 380.21, 410.65, 441.09, 471.53, 501.96, 532.40, 562.84, 593.28, 623.71, 654.15, 684.59, 715.03, 745.21, 775.65, 806.09, 836.53, 866.96, 897.40, 927.84, 958.28, 988.71, 1019.15, 1049.59, 1080.03, 1110.21, 1140.65, 1171.09, 1201.53, 1231.96, 1262.40, 1292.84, 1323.28, 1353.71, 1384.15, 1414.59, 1445.03, 1476.21, 1506.65, 1537.09, 1567.53, 1597.96, 1628.40, 1658.84, 1689.28, 1719.71, 1750.15, 1780.59, 1811.03, 1841.21, 1871.65, 1902.09, 1932.53, 1962.96, 1993.40, 2023.84, 2054.28, 2084.71, 2115.15, 2145.59, 2176.03, 2206.21, 2236.65, 2267.09, 2297.53, 2327.96, 2358.40, 2388.84, 2419.28, 2449.71, 2480.15, 2510.59, 2541.03, 2571.21, 2601.65, 2632.09, 2662.53, 2692.96, 2723.40, 2753.84, 2784.28, 2814.71, 2845.15, 2875.59, 2906.03, 2937.21, 2967.65, 2998.09, 3028.53, 3058.96, 3089.40, 3119.84, 3150.28, 3180.71, 3211.15, 3241.59, 3272.03, 3302.21, 3332.65, 3363.09, 3393.53, 3423.96, 3454.40, 3484.84, 3515.28, 3545.71, 3576.15, 3606.59, 3637.03, 3667.21, 3697.65, 3728.09, 3758.53, 3788.96, 3819.40, 3849.84, 3880.28, 3910.71, 3941.15, 3971.59, 4002.03, 4032.21, 4062.65, 4093.09, 4123.53, 4153.96, 4184.40, 4214.84, 4245.28, 4275.71, 4306.15, 4336.59, 4367.03, 4398.21, 4428.65, 4459.09, 4489.53, 4519.96, 4550.40, 4580.84, 4611.28, 4641.71, 4672.15, 4702.59, 4733.03, 4763.21, 4793.65, 4824.09, 4854.53, 4884.96, 4915.40, 4945.84, 4976.28, 5006.71, 5037.15, 5067.59, 5098.03, 5128.21, 5158.65, 5189.09, 5219.53, 5249.96, 5280.40, 5310.84, 5341.28, 5371.71, 5402.15, 5432.59, 5463.03, 5493.21, 5523.65, 5554.09, 5584.53, 5614.96, 5645.40, 5675.84, 5706.28, 5736.71, 5767.15, 5797.59, 5828.03, 5859.21, 5889.65, 5920.09, 5950.53, 5980.96, 6011.40, 6041.84, 6072.28, 6102.71, 6133.15, 6163.59, 6194.03, 6224.21, 6254.65, 6285.09, 6315.53, 6345.96, 6376.40, 6406.84, 6437.28, 6467.71, 6498.15, 6528.59, 6559.03, 6589.21, 6619.65, 6650.09, 6680.53, 6710.96, 6741.40, 6771.84, 6802.28, 6832.71, 6863.15, 6893.59, 6924.03, 6954.21, 6984.65, 7015.09, 7045.53, 7075.96, 7106.40, 7136.84, 7167.28, 7197.71, 7228.15, 7258.59, 7289.03], np.float64)
    return truncate(data)

def get_t2m_max_data():
    data = np.array([27.93, 28.48, 28.58, 28.10, 27.26, 25.58, 26.41, 27.33, 28.02, 28.84, 28.93, 28.00, 29.55, 29.16, 28.48, 27.80, 27.30, 26.16, 26.30, 27.42, 28.42, 28.59, 27.92, 28.36, 30.04, 29.08, 28.52, 27.75, 26.67, 26.88, 26.34, 26.98, 27.90, 28.55, 28.55, 28.33, 30.38, 30.26, 28.50, 28.91, 26.84, 26.20, 26.60, 27.16, 28.21, 28.85, 28.40, 28.90, 29.52, 28.78, 29.34, 28.07, 28.17, 27.18, 27.10, 28.50, 28.72, 29.10, 28.97, 28.89, 28.28, 29.26, 28.55, 28.17, 26.60, 27.16, 26.91, 28.43, 28.87, 29.42, 28.56, 28.24, 29.02, 32.52, 28.62, 28.10, 27.21, 26.10, 27.38, 27.96, 28.58, 28.68, 28.51, 28.83, 28.95, 28.83, 28.97, 28.60, 26.88, 26.53, 26.99, 28.60, 28.64, 28.93, 28.48, 29.22, 27.84, 28.48, 28.32, 27.99, 28.00, 26.42, 27.04, 28.13, 29.15, 29.19, 29.44, 29.13, 30.28, 29.97, 29.93, 28.64, 27.97, 27.26, 27.29, 28.40, 29.54, 29.57, 28.51, 28.84, 29.18, 29.07, 28.71, 27.79, 26.67, 26.84, 27.33, 28.46, 28.72, 29.06, 28.59, 27.89, 28.49, 28.14, 27.18, 28.22, 27.05, 27.11, 26.46, 27.92, 28.87, 28.97, 29.28, 29.01, 29.22, 28.18, 28.53, 28.50, 27.53, 27.50, 26.41, 26.85, 28.49, 29.11, 28.49, 28.69, 28.88, 28.90, 28.47, 27.73, 27.60, 26.39, 26.18, 27.64, 29.09, 28.44, 28.30, 28.73, 27.41, 29.09, 28.08, 27.76, 27.38, 26.41, 27.07, 28.37, 30.34, 29.80, 29.00, 28.73, 32.09, 29.11, 29.06, 28.44, 27.56, 26.59, 26.76, 28.34, 28.67, 29.13, 29.24, 27.97, 28.23, 29.44, 28.41, 28.49, 28.59, 27.66, 27.13, 29.66, 29.47, 29.08, 28.76, 29.48, 28.29, 30.35, 29.23, 27.81, 27.10, 26.33, 27.06, 27.83, 29.39, 28.74, 28.57, 28.07, 28.12, 28.87, 28.53, 28.99, 28.01, 26.76, 27.01, 28.21, 29.97, 28.73, 28.86, 28.16, 30.27, 30.25, 30.05, 28.84, 27.44, 27.93, 27.81, 28.44, 29.51, 29.86, 29.03, 28.42], np.float64)
    return truncate(data)

def get_t2m_min_data():
    data = np.array([21.47, 21.87, 22.18, 22.14, 22.03, 20.83, 21.04, 21.37, 21.61, 22.37, 22.40, 22.50, 22.57, 22.65, 22.42, 22.32, 22.19, 21.30, 21.32, 21.48, 21.96, 22.17, 22.07, 22.44, 22.81, 22.83, 22.53, 22.35, 21.93, 21.61, 21.10, 21.32, 21.74, 22.35, 22.42, 22.38, 22.75, 22.92, 22.55, 22.72, 21.98, 21.12, 21.06, 21.18, 21.72, 22.41, 22.47, 22.48, 22.85, 22.84, 22.92, 22.70, 22.38, 21.82, 21.33, 21.89, 22.16, 22.51, 22.63, 22.44, 22.31, 22.60, 22.46, 22.39, 21.67, 21.56, 21.53, 21.79, 22.03, 22.79, 22.46, 22.50, 22.88, 23.30, 22.63, 22.51, 22.06, 21.33, 21.42, 21.62, 21.85, 22.29, 22.39, 22.45, 22.25, 22.38, 22.16, 22.37, 21.80, 21.41, 21.21, 21.88, 22.07, 22.45, 22.68, 22.62, 22.31, 22.35, 22.40, 22.41, 22.18, 21.60, 21.49, 21.99, 22.42, 22.72, 23.25, 22.85, 22.94, 23.55, 23.41, 23.03, 22.83, 22.07, 21.63, 21.98, 22.55, 22.79, 22.60, 22.46, 22.28, 22.51, 22.51, 22.20, 22.11, 21.72, 21.52, 21.84, 22.13, 22.60, 22.61, 22.41, 22.30, 22.13, 22.00, 22.28, 21.92, 21.52, 21.24, 21.49, 21.92, 22.49, 22.94, 22.75, 22.83, 22.68, 22.64, 22.40, 22.31, 21.82, 21.09, 21.40, 21.97, 22.36, 22.45, 22.47, 22.59, 22.76, 22.46, 22.43, 22.29, 21.78, 21.20, 21.53, 22.18, 22.35, 22.65, 22.50, 22.17, 22.58, 22.65, 22.29, 22.28, 21.69, 21.68, 22.20, 22.78, 22.96, 22.95, 22.75, 23.89, 23.57, 23.52, 23.07, 22.57, 21.64, 21.47, 22.04, 22.13, 22.83, 22.96, 22.57, 22.29, 22.83, 22.61, 22.71, 22.60, 22.09, 21.39, 22.23, 22.53, 22.60, 22.79, 22.84, 22.19, 23.12, 22.84, 22.29, 22.06, 21.35, 21.43, 21.40, 22.35, 22.81, 22.90, 22.31, 22.52, 22.92, 22.91, 22.96, 22.61, 22.04, 21.47, 21.50, 22.62, 22.34, 22.78, 22.79, 22.97, 23.36, 23.43, 22.89, 22.54, 22.23, 21.84, 22.05, 22.47, 22.77, 22.76, 22.47], np.float64)
    return truncate(data)

def get_snowfall_data():
    data = np.array([0, 0, 0, 0, 0, 0, 0, 0, 8.6e-16, 0, 1.7e-15, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 8.6e-16, 0, 0, 0, 0, 8.6e-16, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 8.6e-16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 0, 8.6e-16, 0, 0, 0, 0, 0, 8.6e-16, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 8.6e-16, 0, 0, 1.7e-15, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.7e-15, 0, 0, 0, 0, 0, 0, 8.6e-16, 1.7e-15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], np.float64)
    return truncate(data)

# ... The other get_*_data() functions would follow here ...
# For brevity, only the structure is shown. The full list is very long.

# --- Main Structure-Building Function ---

def create_cardamom_structure():
    fill_value = -9999.0
    
    cardamom_data = {
        'time': {
            'dims': ('time_dim',), 'data': get_time_data(),
            'attrs': {'_FillValue': fill_value, 'units': 'days since 2001-01-01', 'calendar': 'proleptic_gregorian'}
        },
        'T2M_MAX': {
            'dims': ('time_dim',), 'data': get_t2m_max_data(),
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(28.31118)}
        },
        'T2M_MIN': {
            'dims': ('time_dim',), 'data': get_t2m_min_data(),
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(22.28568)}
        },
        'SNOWFALL': {
            'dims': ('time_dim',), 'data': get_snowfall_data(),
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(-1.0842e-16)}
        },
        'SSRD': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(16.3549)}
        },
        'STRD': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(35.7737)}
        },
        'SKT': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(24.8919)}
        },
        'VPD': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(9.4804)}
        },
        'BURNED_AREA': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(0.00028)}
        },
        'CO2': {
            'dims': ('time_dim',), 'data': get_co2_data(),
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(390.6323)}
        },
        'DISTURBANCE_FLUX': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(2.9971)}
        },
        'TOTAL_PREC': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'reference_mean': truncate(8.8815)}
        },
        'GPP': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'opt_filter': 0.0, 'opt_unc_type': 2.0, 'single_unc': 0.1}
        },
        'EWT': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'opt_unc_type': 0.0, 'opt_filter': 0.0, 'single_unc': 50.0, 'opt_normalization': 1.0}
        },
        'LAI': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'opt_unc_type': 1.0, 'opt_filter': 3.0, 'single_annual_unc': 1.5}
        },
        'SCF': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'opt_unc_type': 0.0, 'opt_filter': 0.0, 'single_unc': 0.2, 'min_threshold': 0.02}
        },
        'ABGB': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'opt_unc_type': 1.0, 'opt_filter': 3.0, 'single_annual_unc': 1.5}
        },
        'Mean_FIR': {'dims': (), 'data': truncate(0.0085), 'attrs': {'opt_unc_type': 1.0, 'unc': 1.5}},
        'PEQ_iniSOM': {'dims': (), 'data': truncate(12270.4278), 'attrs': {'opt_unc_type': 1.0, 'unc': 1.5}},
        'PEQ_CUE': {'dims': (), 'data': 0.5, 'attrs': {'opt_unc_type': 0, 'unc': 0.2}},
        'ROFF': {
            'dims': ('time_dim',), 'data': np.full(240, np.nan), # Placeholder
            'attrs': {'_FillValue': fill_value, 'opt_unc_type': 1.0, 'opt_filter': 0.0, 'single_unc': 1.5, 'min_threshold': -np.inf}
        },
        'LAI_ts': {'dims': ('time_dim',), 'data': np.full(240, np.nan), 'attrs': {'_FillValue': fill_value}},
        'ET_ts': {'dims': ('time_dim',), 'data': np.full(240, np.nan), 'attrs': {'_FillValue': fill_value}},
        'YIELD': {'dims': ('time_dim',), 'data': np.full(240, 0.0), 'attrs': {'_FillValue': fill_value}},
        'LAT': {'dims': (), 'data': truncate(-0.0647), 'attrs': {}},
        'DOY': {'dims': ('time_dim',), 'data': np.full(240, np.nan), 'attrs': {'_FillValue': fill_value}},
        'ID': {'dims': (), 'data': 1100.0, 'attrs': {}},
        'EDC': {'dims': (), 'data': 1.0, 'attrs': {}},
        'MCMCID': {
            'dims': (), 'data': 3.0,
            'attrs': {'nITERATIONS': 500000.0, 'nSAMPLES': 10.0, 'nPRINT': 1000.0, 'seed_number': 0.0}
        },
        'GPPunc': {'dims': ('time_dim',), 'data': np.full(240, np.nan), 'attrs': {'_FillValue': fill_value}},
    }
    return cardamom_data

# --- NetCDF Writer Function ---

def write_struct_to_netcdf(data_struct, filename="cardamom_output.nc"):
    """
    Writes the entire data structure to a NetCDF file.
    """
    with nc.Dataset(filename, 'w', format='NETCDF4') as rootgrp:
        print(f"Creating NetCDF file: {filename}")
        
        rootgrp.createDimension("time_dim", 240)
        
        for var_name, var_details in data_struct.items():
            variable = rootgrp.createVariable(var_name, 'f8', var_details['dims'])
            variable.setncatts(var_details['attrs'])
            print(f"  - Writing variable: {var_name}")
            variable[:] = var_details['data']
            
    print("\nFile writing complete!")

# --- Main execution block ---
if __name__ == "__main__":
    STRUCT = create_cardamom_structure()
    write_struct_to_netcdf(STRUCT, "AMAZON_H2O_BALANCE_BASIN01.cbf.nc")
    print("\nScript finished. You can verify the output with 'ncdump AMAZON_H2O_BALANCE_BASIN01.cbf.nc'")
