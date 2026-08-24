import os
import cdsapi
import xarray as xr

# --- USER CONFIGURATION ---
SITES = [
    {"name": "CA-Qfo", "lat": 49.6925, "lon": -74.3421},
    {"name": "CH-Dav", "lat": 46.8153, "lon": 9.8559},
    {"name": "DE-Gri", "lat": 50.9495, "lon": 13.5125},
    {"name": "DE-Hai", "lat": 51.0792, "lon": 10.4530},
    {"name": "DE-Tha", "lat": 50.9636, "lon": 13.5669},
    {"name": "DK-Sor", "lat": 55.4859, "lon": 11.6446},
    {"name": "FI-Hyy", "lat": 61.8475, "lon": 24.2950},
    {"name": "FR-Pue", "lat": 43.7414, "lon": 3.5958},
    {"name": "IT-Lav", "lat": 45.9562, "lon": 11.2813},
    {"name": "IT-MBo", "lat": 46.0147, "lon": 11.0458},
    {"name": "IT-Noe", "lat": 40.6062, "lon": 8.1512},
    {"name": "NL-Loo", "lat": 52.1666, "lon": 5.7436},
    {"name": "RU-Fyo", "lat": 56.4615, "lon": 32.9221},
    {"name": "US-MMS", "lat": 39.3232, "lon": -86.4131},
    {"name": "US-NR1", "lat": 40.0329, "lon": -105.5464},
    {"name": "US-SRG", "lat": 31.7894, "lon": -110.8277},
    {"name": "US-SRM", "lat": 31.8214, "lon": -110.8660},
    {"name": "US-Ton", "lat": 38.4316, "lon": -120.9660},
    {"name": "US-Var", "lat": 38.4133, "lon": -120.9507},
    {"name": "US-Whs", "lat": 31.7438, "lon": -110.0522},
    {"name": "US-Wkg", "lat": 31.7365, "lon": -109.9419}
]

pad = 0.25
data_format = "netcdf"
dataset = "reanalysis-era5-single-levels-monthly-means"

hourly_quantities = [
    "2m_temperature", 
    "2m_dewpoint_temperature"
]
monthly_quantities = [
    "total_precipitation", 
    "skin_temperature", 
    "surface_solar_radiation_downwards", 
    "snowfall",
    "surface_thermal_radiation_downwards" # Added STRD here
]

all_months = [str(m).zfill(2) for m in range(1, 13)]
all_hours = [f"{str(h).zfill(2)}:00" for h in range(24)]

# Dynamically calculate the Continental Box covering ALL sites
max_lat = max([s["lat"] for s in SITES]) + pad
min_lat = min([s["lat"] for s in SITES]) - pad
min_lon = min([s["lon"] for s in SITES]) - pad
max_lon = max([s["lon"] for s in SITES]) + pad
regional_area = [max_lat, min_lon, min_lat, max_lon] 

client = cdsapi.Client()

def DOWNLOAD_AND_SLICE_YEARLY(yr):
    yr_str = str(yr)
    
    # Define temporary bulk filenames
    bulk_hourly = f"BULK_REGIONAL_HOURLY_{yr_str}.nc"
    bulk_monthly = f"BULK_REGIONAL_MONTHLY_{yr_str}.nc"
    
    # --- 1. DOWNLOAD BULK HOURLY AVERAGES (2m Temp, Dewpoint) ---
    req_hourly = {
        "product_type": ["monthly_averaged_reanalysis_by_hour_of_day"],
        "variable": hourly_quantities,
        "year": [yr_str],
        "month": all_months,
        "time": all_hours,
        "data_format": data_format,
        "area": regional_area 
    }
    
    if not os.path.exists(bulk_hourly):
        print(f"\nDownloading {bulk_hourly}...")
        client.retrieve(dataset, req_hourly).download(bulk_hourly)

    # --- 2. DOWNLOAD BULK MONTHLY AVERAGES (Precip, Rad, Snow, etc.) ---
    req_monthly = {
        "product_type": ["monthly_averaged_reanalysis"],
        "variable": monthly_quantities,
        "year": [yr_str],
        "month": all_months,
        "time": ["00:00"], # Required dummy time for flat monthly means
        "data_format": data_format,
        "area": regional_area 
    }
    
    if not os.path.exists(bulk_monthly):
        print(f"Downloading {bulk_monthly}...")
        client.retrieve(dataset, req_monthly).download(bulk_monthly)

    # --- 3. SLICE AND SAVE SITES LOCALLY ---
    print(f"Slicing bulk files for {yr_str} at all sites...")
    try:
        ds_hourly = xr.open_dataset(bulk_hourly)
        ds_monthly = xr.open_dataset(bulk_monthly)
        
        for site in SITES:
            site_file_h = f"{site['name']}_ECMWF_HOURLY_MEANS_{yr_str}.nc"
            site_file_m = f"{site['name']}_ECMWF_MONTHLY_MEANS_{yr_str}.nc"
            
            # Slice Hourly Data
            if not os.path.exists(site_file_h):
                site_ds_h = ds_hourly.sel(latitude=site["lat"], longitude=site["lon"], method="nearest")
                site_ds_h.to_netcdf(site_file_h)
            
            # Slice Monthly Data
            if not os.path.exists(site_file_m):
                site_ds_m = ds_monthly.sel(latitude=site["lat"], longitude=site["lon"], method="nearest")
                site_ds_m.to_netcdf(site_file_m)
                
            print(f"  -> Saved data for {site['name']}")
            
        ds_hourly.close()
        ds_monthly.close()
        
    except Exception as e:
        print(f"Failed to slice data for {yr_str}: {e}")
        return

    # --- 4. CLEAN UP BULK FILES ---
    print(f"Cleaning up bulk files for {yr_str}...")
    os.remove(bulk_hourly)
    os.remove(bulk_monthly)


# --- MAIN EXECUTION ---
print(f"Calculated Bounding Box: {regional_area}")
for yr in range(2001, 2025): 
    DOWNLOAD_AND_SLICE_YEARLY(yr)













