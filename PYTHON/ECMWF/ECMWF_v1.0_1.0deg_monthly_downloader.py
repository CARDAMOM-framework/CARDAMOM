import cdsapi
#cdsapi instructions available here: https://confluence.ecmwf.int/display/CKB/How+to+install+and+use+CDS+API+on+macOS 

# variable names are available here and at related links: https://confluence.ecmwf.int/pages/viewpage.action?pageId=536218894 
global_area_definition=[-89.75, -179.75, 89.75, 179.75];
global_grid_definition=["0.5/0.5"];
data_format="netcdf";
download_format="unarchived";
hourly_quantities=["2m_temperature","2m_dewpoint_temperature"];
monthly_quantities=["total_precipitation","skin_temperature","surface_solar_radiation_downwards","snowfall"];
#Add STRD to list for next iteration (version 1.1)

def DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(m, yr):

  #Step 1. Download all monthly averages by hour


  for q in hourly_quantities:
        dataset = "reanalysis-era5-single-levels-monthly-means"
        request = {
        "product_type": ["monthly_averaged_reanalysis_by_hour_of_day"],
        "variable": q,
        "year": yr,
        "month": m,
         "time": [
        "00:00", "01:00", "02:00",
        "03:00", "04:00", "05:00",
        "06:00", "07:00", "08:00",
        "09:00", "10:00", "11:00",
        "12:00", "13:00", "14:00",
        "15:00", "16:00", "17:00",
        "18:00", "19:00", "20:00",
        "21:00", "22:00", "23:00"
    ],
        "data_format": data_format,
        "grid": global_grid_definition,
        "download_format": download_format,
        "area": global_area_definition
        }

        file = f"ECMWF_CARDAMOM_DRIVER_{q}_{str(m).zfill(2)}{yr}.nc" #formats file title
        client = cdsapi.Client()
        client.retrieve(dataset, request).download(file)


    #Step 1. Download all monthly averages 


  
  for q in  monthly_quantities:
        dataset = "reanalysis-era5-single-levels-monthly-means"
        request = {
        "product_type": ["monthly_averaged_reanalysis"],
        "variable": q,
        "year": yr,
        "month": m,
        "time": ["00:00","01:00"],
        "data_format": data_format,
        "grid": global_grid_definition,
        "download_format": download_format,
        "area": global_area_definition
        }

        file = f"ECMWF_CARDAMOM_DRIVER_{q}_{str(m).zfill(2)}{yr}.nc" #formats file title
        client = cdsapi.Client()
        client.retrieve(dataset, request).download(file)




#Main code
#Example for dowloading months & years of data

for m in list(range(1, 13)):  #downloads months 1-12
  for yr in list(range(2001, 2025)): #downloads years 2001-2024
    DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(m, yr);
