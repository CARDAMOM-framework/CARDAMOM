import cdsapi

# --- USER CONFIGURATION ---
TARGET_LAT = 55.4859
TARGET_LON = 11.6446

data_format = "netcdf"

# All quantities consolidated into one list for hourly extraction
all_quantities = [
    "2m_temperature",
    "2m_dewpoint_temperature",
    "total_precipitation",
    "skin_temperature",
    "surface_solar_radiation_downwards",
    "snowfall",
    "surface_thermal_radiation_downwards" # STRD added
]

def DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(m, yr):
    # Using the specialized timeseries dataset to avoid MARS area errors
    dataset = "reanalysis-era5-single-levels-timeseries"
    
    for q in all_quantities:
        request = {
            "variable": [q],
            "year": [str(yr)],
            "month": [str(m).zfill(2)],
            "day": [str(d).zfill(2) for d in range(1, 32)],
            "time": [
                "00:00", "01:00", "02:00", "03:00", "04:00", "05:00",
                "06:00", "07:00", "08:00", "09:00", "10:00", "11:00",
                "12:00", "13:00", "14:00", "15:00", "16:00", "17:00",
                "18:00", "19:00", "20:00", "21:00", "22:00", "23:00"
            ],
            "data_format": data_format,
            # 'location' replaces 'area' for this specific dataset to get exactly 1 point
            "location": {
                "latitude": TARGET_LAT,
                "longitude": TARGET_LON
            }
        }

        file = f"ECMWF_CARDAMOM_DRIVER_{q}_{str(m).zfill(2)}{yr}.nc" # formats file title
        print(f"Downloading {file}...")
        
        client = cdsapi.Client()
        client.retrieve(dataset, request).download(file)


#Main code
#Example for downloading months & years of data

for m in list(range(1, 13)):  #downloads months 1-12
    for yr in list(range(2001, 2025)): #downloads years 2001-2024
        DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(m, yr)
