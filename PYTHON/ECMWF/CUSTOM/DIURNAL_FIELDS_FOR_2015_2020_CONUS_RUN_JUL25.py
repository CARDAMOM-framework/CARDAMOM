import cdsapi
import os

global_area_definition=[60, -130, 20, -50]
global_grid_definition=["0.5/0.5"]
data_format="netcdf"
download_format="unarchived"

# Map full variable names to their abbreviations
variable_map = {
    "skin_temperature": "SKT",
    "surface_solar_radiation_downwards": "SSRD"
}

def DOWNLOAD_ECMWF_HOURLY_DRIVERS_FOR_CARDAMOM(m, yr):
    # Iterate over the variable map
    for full_var_name, abbr_var_name in variable_map.items():
        file = f"ECMWF_CARDAMOM_HOURLY_DRIVER_{abbr_var_name}_{m}{yr}.nc"
        
        if os.path.exists(file):
            print(f"File '{file}' already exists. Skipping download.")
            continue

        dataset = "reanalysis-era5-single-levels"
        request = {
            "product_type": ["reanalysis"],
            "variable": full_var_name,  # Use the full variable name for the API request
            "year": yr,
            "month": m,
            "day": [
                "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12",
                "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24",
                "25", "26", "27", "28", "29", "30", "31"
            ],
            "time": [
                "00:00", "01:00", "02:00", "03:00", "04:00", "05:00", "06:00", "07:00", "08:00",
                "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00",
                "18:00", "19:00", "20:00", "21:00", "22:00", "23:00"
            ],
            "data_format": data_format,
            "grid": global_grid_definition,
            "download_format": download_format,
            "area": global_area_definition
        }
        
        client = cdsapi.Client()
        client.retrieve(dataset, request).download(file)


#Main code

for year in range(2015, 2021):
    for month in range(1, 13):
        year_str = str(year)
        month_str = str(month).zfill(2)
        DOWNLOAD_ECMWF_HOURLY_DRIVERS_FOR_CARDAMOM(month_str, year_str)
