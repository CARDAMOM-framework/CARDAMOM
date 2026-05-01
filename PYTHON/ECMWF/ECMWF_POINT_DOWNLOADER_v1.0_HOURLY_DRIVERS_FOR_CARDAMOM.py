import cdsapi
import xarray as xr
import os
import glob

# --- USER CONFIGURATION ---
TARGET_LAT = 55.4859  
TARGET_LON = 11.6446 

# 1. PAD THE BOUNDING BOX 
# ERA5 native resolution is ~0.25 degrees. We pad the box to ensure 
# the MARS server finds data and doesn't throw an "empty crop" error.
pad = 0.25
point_area_definition = [
    TARGET_LAT + pad, # North
    TARGET_LON - pad, # West
    TARGET_LAT - pad, # South
    TARGET_LON + pad  # East
]

data_format = "netcdf"

# All quantities for the CARDAMOM driver
all_quantities = [
    "2m_temperature",
    "2m_dewpoint_temperature",
    "total_precipitation",
    "skin_temperature",
    "surface_solar_radiation_downwards",
    "snowfall",
    "surface_thermal_radiation_downwards"
]

months_of_year = [str(m).zfill(2) for m in range(1, 13)]
days_of_month = [str(d).zfill(2) for d in range(1, 32)]
hours_of_day = [f"{str(h).zfill(2)}:00" for h in range(24)]

def DOWNLOAD_ECMWF_YEARLY_CHUNK(yr):
    """Downloads a full year of hourly data for a small bounding box."""
    dataset = "reanalysis-era5-single-levels"
    
    request = {
        "product_type": ["reanalysis"],
        "variable": all_quantities,
        "year": str(yr),
        "month": months_of_year,
        "day": days_of_month,       
        "time": hours_of_day,       
        "data_format": data_format,
        "area": point_area_definition 
    }

    temp_file_name = f"TEMP_DRIVER_{yr}.nc" 
    
    print(f"Downloading hourly data for {yr} around [{TARGET_LAT}, {TARGET_LON}]...")
    client = cdsapi.Client()
    client.retrieve(dataset, request, temp_file_name)
    return temp_file_name

# --- MAIN EXECUTION ---
if __name__ == "__main__":
    start_year = 2001
    end_year = 2025
    master_file_name = f"ECMWF_CARDAMOM_DRIVER_MASTER_{start_year}_{end_year}.nc"

    # Step 1: Download yearly chunks
    for yr in range(start_year, end_year + 1): 
        if not os.path.exists(f"TEMP_DRIVER_{yr}.nc"):
            DOWNLOAD_ECMWF_YEARLY_CHUNK(yr)
        else:
            print(f"TEMP_DRIVER_{yr}.nc already exists. Skipping download.")

    # Step 2: Merge the chunks and SNAP to the exact point
    print("\nAll downloads complete. Merging and extracting nearest point...")
    
    temp_files = sorted(glob.glob("TEMP_DRIVER_*.nc"))
    
    with xr.open_mfdataset(temp_files, combine='by_coords') as ds:
        # 2. SNAP LOCALLY
        # This isolates the single closest grid node to your exact coordinates
        ds_point = ds.sel(latitude=TARGET_LAT, longitude=TARGET_LON, method='nearest')
        
        # Save the isolated point data to the master file
        ds_point.to_netcdf(master_file_name)

    print(f"Master file saved as: {master_file_name}")

    # Step 3: Clean up temporary files
    print("Cleaning up temporary yearly files...")
    for file in temp_files:
        os.remove(file)
        
    print("Done! You now have a single, continuous hourly driver file for your exact point.")
