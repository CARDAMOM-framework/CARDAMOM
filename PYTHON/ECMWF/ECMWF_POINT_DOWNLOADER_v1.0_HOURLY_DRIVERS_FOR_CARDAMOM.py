import cdsapi

# --- USER CONFIGURATION ---
TARGET_LAT = 55.4859
TARGET_LON = 11.6446

# We use the minimum safe pad (0.25) to prevent the MARS server from crashing.
# This will download a tiny grid. You can just read the first spatial index 
# (e.g., data[time, 0, 0]) when you ingest this into CARDAMOM.
pad = 0.25
padded_area = [
    TARGET_LAT + pad, # North
    TARGET_LON - pad, # West
    TARGET_LAT - pad, # South
    TARGET_LON + pad  # East
]

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

def DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(m, yr):
    # Reverting to the standard, complete dataset
    dataset = "reanalysis-era5-single-levels"
    
    for q in all_quantities:
        request = {
            "product_type": ["reanalysis"],
            "variable": [q],
            "year": [str(yr)],
            "month": [str(m).zfill(2)],
            "day": [str(d).zfill(2) for d in range(1, 32)],
            "time": [f"{str(h).zfill(2)}:00" for h in range(24)],
            "data_format": data_format,
            "area": padded_area 
        }

        file = f"ECMWF_CARDAMOM_DRIVER_{q}_{str(m).zfill(2)}{yr}.nc"
        print(f"Downloading {file}...")
        
        client = cdsapi.Client()
        client.retrieve(dataset, request).download(file)

# --- MAIN EXECUTION ---
# Example for downloading months & years of data
for m in range(1, 13):  # downloads months 1-12
    for yr in range(2001, 2025): # downloads years 2001-2024
        DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(m, yr)
