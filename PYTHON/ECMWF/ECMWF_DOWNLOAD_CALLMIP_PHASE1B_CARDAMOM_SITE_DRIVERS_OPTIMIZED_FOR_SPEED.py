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
q = "skin_temperature"

# Request 1984 to 2025
all_years = [str(yr) for yr in range(1984, 2026)]
all_months = [str(m).zfill(2) for m in range(1, 13)]

# Dynamically calculate the Continental Box
max_lat = max([s["lat"] for s in SITES]) + pad
min_lat = min([s["lat"] for s in SITES]) - pad
min_lon = min([s["lon"] for s in SITES]) - pad
max_lon = max([s["lon"] for s in SITES]) + pad
regional_area = [max_lat, min_lon, min_lat, max_lon] 

out_dir = "DATA/CALLMIP/ECMWF_PHASE1b_DRIVERS"
os.makedirs(out_dir, exist_ok=True)

bulk_file = f"BULK_{q}_1984_2025.nc"
client = cdsapi.Client()

# --- 1. DOWNLOAD BULK FILE (ONE REQUEST) ---
request = {
    "product_type": ["monthly_averaged_reanalysis"],
    "variable": [q],
    "year": all_years,
    "month": all_months,
    "time": ["00:00"],
    "data_format": data_format,
    "area": regional_area 
}

if not os.path.exists(bulk_file):
    print(f"Submitting 1 master request for 1984-2025 {q}...")
    try:
        client.retrieve(dataset, request).download(bulk_file)
    except Exception as e:
        print(f"Failed to download {bulk_file}: {e}")
        exit()
else:
    print(f"{bulk_file} already exists. Skipping download.")

# --- 2. SLICE AND SAVE LOCAL SITES ---
print("Slicing full timeseries for all sites...")
try:
    ds = xr.open_dataset(bulk_file)
    
    for site in SITES:
        # Define output file for the continuous timeseries
        site_file = f"{out_dir}/{site['name']}_ECMWF_CARDAMOM_DRIVER_{q}_1984_2025.nc"
        
        if not os.path.exists(site_file):
            # Extract the entire 42-year timeseries for this specific site and save immediately
            site_ds_full = ds.sel(latitude=site["lat"], longitude=site["lon"], method="nearest")
            site_ds_full.to_netcdf(site_file)
                    
    ds.close()
    print(f"Successfully sliced all files to {out_dir}")
except Exception as e:
    print(f"Failed during local slicing: {e}")

# --- 3. CLEANUP ---
if os.path.exists(bulk_file):
    os.remove(bulk_file)
    print("Cleaned up bulk file.")
