import os
import cdsapi

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

# All variables grouped together
all_quantities = [
    "2m_temperature",
    "2m_dewpoint_temperature",
    "total_precipitation",
    "skin_temperature",
    "surface_solar_radiation_downwards",
    "snowfall",
    "surface_thermal_radiation_downwards"
]

# All months grouped together
all_months = [str(m).zfill(2) for m in range(1, 13)]

client = cdsapi.Client()

def DOWNLOAD_ECMWF_YEARLY_DRIVERS(site_name, lat, lon, yr):
    dataset = "reanalysis-era5-single-levels"
    
    padded_area = [
        lat + pad, # North
        lon - pad, # West
        lat - pad, # South
        lon + pad  # East
    ]

    # New filename format: One file per year containing all vars and months
    file = f"{site_name}_ECMWF_CARDAMOM_DRIVER_ALL_VARS_{yr}.nc"
    
    if os.path.exists(file):
        print(f"{file} ... already downloaded")
        return

    request = {
        "product_type": ["reanalysis"],
        "variable": all_quantities, # Request all 7 vars at once
        "year": [str(yr)],
        "month": all_months,        # Request all 12 months at once
        "day": [str(d).zfill(2) for d in range(1, 32)],
        "time": [f"{str(h).zfill(2)}:00" for h in range(24)],
        "data_format": data_format,
        "area": padded_area 
    }

    print(f"Downloading {file}...")
    try:
        client.retrieve(dataset, request).download(file)
    except Exception as e:
        print(f"Failed to download {file}: {e}")

# --- MAIN EXECUTION ---
# Loop over sites, then years (Months and Vars are now handled inside the API call)
for site in SITES:
    print(f"\n==============================================")
    print(f"--- Processing Site: {site['name']} ---")
    print(f"==============================================")
    for yr in range(2001, 2025): 
        DOWNLOAD_ECMWF_YEARLY_DRIVERS(site["name"], site["lat"], site["lon"], yr)
