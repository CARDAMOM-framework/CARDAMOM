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

#
# Note: CALLMIP already provides most of these drivers. CARDAMOM team only needs surface skin temp 
#
#
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
    "surface_thermal_radiation_downwards"
]

all_quantities = hourly_quantities + monthly_quantities
all_hours = [f"{str(h).zfill(2)}:00" for h in range(24)]

# Dynamically calculate the Continental Box covering ALL sites
max_lat = max([s["lat"] for s in SITES]) + pad
min_lat = min([s["lat"] for s in SITES]) - pad
min_lon = min([s["lon"] for s in SITES]) - pad
max_lon = max([s["lon"] for s in SITES]) + pad
regional_area = [max_lat, min_lon, min_lat, max_lon] 

client = cdsapi.Client()

def DOWNLOAD_AND_SLICE_SINGLE_VAR(q, m, yr):
    month_str = str(m).zfill(2)
    yr_str = str(yr)
    
    # Check if this variable needs hourly diurnal data or a flat monthly mean
    if q in hourly_quantities:
        product_type = "monthly_averaged_reanalysis_by_hour_of_day"
        req_time = all_hours
    else:
        product_type = "monthly_averaged_reanalysis"
        req_time = ["00:00"]
        
    bulk_file = f"BULK_{q}_{month_str}{yr_str}.nc"
    
    # --- 1. CHECK IF FILES ALREADY EXIST ---
    all_sites_exist = True
    for site in SITES:
        site_file = f"DATA/CALLMIP/ECMWF_PHASE1b_DRIVERS/{site['name']}_ECMWF_CARDAMOM_DRIVER_{q}_{month_str}{yr_str}.nc"
        if not os.path.exists(site_file):
            all_sites_exist = False
            break
            
    if all_sites_exist:
        print(f"[{month_str}/{yr_str}] {q} ... already sliced for all sites.")
        return

    # --- 2. DOWNLOAD BULK FILE ---
    request = {
        "product_type": [product_type],
        "variable": [q],
        "year": [yr_str],
        "month": [month_str],
        "time": req_time,
        "data_format": data_format,
        "area": regional_area 
    }
    
    print(f"\n[{month_str}/{yr_str}] Downloading {q}...")
    try:
        client.retrieve(dataset, request).download(bulk_file)
    except Exception as e:
        print(f"Failed to download {bulk_file}: {e}")
        return

   # --- 3. SLICE AND SAVE SITES LOCALLY ---
    print(f"  -> Slicing for individual sites...")
    try:
        ds = xr.open_dataset(bulk_file)
        
        # --- NEW: Ensure the output directory exists before saving ---
        out_dir = "DATA/CALLMIP/ECMWF_PHASE1b_DRIVERS"
        os.makedirs(out_dir, exist_ok=True) 
        
        for site in SITES:
            # FIX: Added the folder path here to match Step 1!
            site_file = f"{out_dir}/{site['name']}_ECMWF_CARDAMOM_DRIVER_{q}_{month_str}{yr_str}.nc"
            
            if not os.path.exists(site_file):
                site_ds = ds.sel(latitude=site["lat"], longitude=site["lon"], method="nearest")
                site_ds.to_netcdf(site_file)
                
        ds.close()
    except Exception as e:
        print(f"Failed to slice {bulk_file}: {e}")
        return

    # --- 4. CLEAN UP BULK FILE ---
    if os.path.exists(bulk_file):
        os.remove(bulk_file)


# --- MAIN EXECUTION ---
print(f"Calculated Bounding Box: {regional_area}")
for yr in range(2001, 2025): 
    for m in range(1, 13):
        for q in all_quantities:
            DOWNLOAD_AND_SLICE_SINGLE_VAR(q, m, yr)
