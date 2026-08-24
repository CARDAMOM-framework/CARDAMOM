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

all_quantities = [
    "2m_temperature",
    "2m_dewpoint_temperature",
    "total_precipitation",
    "skin_temperature",
    "surface_solar_radiation_downwards",
    "snowfall",
    "surface_thermal_radiation_downwards"
]

# Dynamically calculate the bounding box covering ALL sites
max_lat = max([s["lat"] for s in SITES]) + pad
min_lat = min([s["lat"] for s in SITES]) - pad
min_lon = min([s["lon"] for s in SITES]) - pad
max_lon = max([s["lon"] for s in SITES]) + pad

regional_area = [max_lat, min_lon, min_lat, max_lon] # North, West, South, East

client = cdsapi.Client()

def DOWNLOAD_AND_SLICE_MONTHLY(m, yr):
    dataset = "reanalysis-era5-single-levels"
    month_str = str(m).zfill(2)
    bulk_file = f"BULK_REGIONAL_{month_str}_{yr}.nc"
    
    # --- 1. DOWNLOAD THE BULK FILE ---
    request = {
        "product_type": ["reanalysis"],
        "variable": all_quantities,
        "year": [str(yr)],
        "month": [month_str],
        "day": [str(d).zfill(2) for d in range(1, 32)],
        "time": [f"{str(h).zfill(2)}:00" for h in range(24)],
        "data_format": data_format,
        "area": regional_area 
    }

    print(f"\nDownloading bulk file {bulk_file}...")
    try:
        if not os.path.exists(bulk_file):
            client.retrieve(dataset, request).download(bulk_file)
        else:
            print(f"{bulk_file} already exists, skipping download.")
    except Exception as e:
        print(f"Failed to download {bulk_file}: {e}")
        return

    # --- 2. SLICE AND SAVE SITES LOCALLY ---
    print(f"Slicing bulk file for all sites...")
    try:
        # Open the bulk dataset
        ds = xr.open_dataset(bulk_file)
        
        for site in SITES:
            site_file = f"{site['name']}_ECMWF_CARDAMOM_DRIVER_ALL_VARS_{month_str}{yr}.nc"
            
            if os.path.exists(site_file):
                continue
                
            # Extract the nearest neighbor grid point for the site
            site_ds = ds.sel(latitude=site["lat"], longitude=site["lon"], method="nearest")
            
            # Save to its own file
            site_ds.to_netcdf(site_file)
            print(f"  -> Saved {site_file}")
            
        # Close the dataset so we can delete the file
        ds.close()
        
    except Exception as e:
        print(f"Failed to process {bulk_file}: {e}")
        return

    # --- 3. DISCARD BULK FILE ---
    print(f"Cleaning up {bulk_file} to save space...")
    os.remove(bulk_file)


# --- MAIN EXECUTION ---
print(f"Calculated Bounding Box: {regional_area}")
for yr in range(2001, 2025): 
    for m in range(1, 13):
        DOWNLOAD_AND_SLICE_MONTHLY(m, yr)
