import cdsapi

# --- USER CONFIGURATION ---
# Define single user-defined point (Latitude, Longitude)
# Setting all values to the same coordinate snaps the download to that single point.
TARGET_LAT =  	55.4859  # Replace with your desired latitude
TARGET_LON = 11.6446 # Replace with your desired longitude
point_area_definition = [TARGET_LAT, TARGET_LON, TARGET_LAT, TARGET_LON]

data_format = "netcdf"

# Combined quantities for high-frequency extraction
# Note: Added 'surface_thermal_radiation_downwards' (STRD) for v1.1
all_quantities = [
    "2m_temperature",
    "2m_dewpoint_temperature",
    "total_precipitation",
    "skin_temperature",
    "surface_solar_radiation_downwards",
    "snowfall",
    "surface_thermal_radiation_downwards"
]

# We must explicitly request all days of the month for the un-averaged hourly dataset
days_of_month = [str(d).zfill(2) for d in range(1, 32)]
# All 24 hours in 'HH:00' format
hours_of_day = [f"{str(h).zfill(2)}:00" for h in range(24)]

def DOWNLOAD_ECMWF_HOURLY_DRIVERS_FOR_CARDAMOM(m, yr):
    dataset = "reanalysis-era5-single-levels"
    
    for q in all_quantities:
        request = {
            "product_type": ["reanalysis"],
            "variable": q,
            "year": str(yr),
            "month": str(m).zfill(2),
            "day": days_of_month,       # The API handles months with < 31 days automatically
            "time": hours_of_day,       
            "data_format": data_format,
            "area": point_area_definition # Extracts our single user-defined point
        }

        # Formats file title
        file_name = f"ECMWF_CARDAMOM_DRIVER_{q}_{str(m).zfill(2)}_{yr}.nc" 
        
        print(f"Downloading {q} for {m}/{yr} at point [{TARGET_LAT}, {TARGET_LON}]...")
        client = cdsapi.Client()
        
        # In newer CDS API versions, the target file is cleanly passed as the third argument
        client.retrieve(dataset, request, file_name)

# --- MAIN EXECUTION ---
# Example for downloading months & years of data
for yr in range(2001, 2025): # downloads years 2001-2024
    for m in range(1, 13):   # downloads months 1-12
        DOWNLOAD_ECMWF_HOURLY_DRIVERS_FOR_CARDAMOM(m, yr)
