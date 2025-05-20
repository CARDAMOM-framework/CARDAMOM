global_area_definition=[-89.75, -179.75, 89.75, 179.75];
global_grid_definition=["0.5/0.5"];

def DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(m, yr)



  #Step 1. Download all monthly averages by hour

  quantities=["2m_temperature","2m_dewpoint_temperature"];


  for q in quantities:
        dataset = "reanalysis-era5-single-levels-monthly-means"
        request = {
        #"product_type": ["monthly_averaged_reanalysis"],
        "product_type": ["monthly_averaged_reanalysis_by_hour_of_day"],
        "variable": q,
        "year": yr,
        "month": m,
        "time": ["00:00","01:00"],
        "data_format": "netcdf",
        "grid": global_grid_definition,
        "download_format": "unarchived",
        "area": global_area_definition
        }

        file="ECMWF_TEST_DOWNLOAD_MAX_FILE"+q+m+yr+".nc"
        client = cdsapi.Client()
        client.retrieve(dataset, request).download(file)



#Main code

DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(1, 2001)


