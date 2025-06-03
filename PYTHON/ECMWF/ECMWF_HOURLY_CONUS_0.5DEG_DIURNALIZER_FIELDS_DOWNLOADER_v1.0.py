import cdsapi;

global_area_definition=[60, -130, 20, -50];
global_grid_definition=["0.5/0.5"];
data_format="netcdf";
download_format="unarchived";
hourly_quantities=["2m_temperature","2m_dewpoint_temperature"];
monthly_quantities=["total_precipitation","skin_temperature","surface_solar_radiation_downwards","snowfall"];


def DOWNLOAD_ECMWF_HOURLY_DRIVERS_FOR_CARDAMOM(m, yr):

  #Step 1. Download all monthly averages by hour

    dataset = "reanalysis-era5-single-levels"
    request = {
    "product_type": ["reanalysis"],
    "variable": [
        ""skin_temperature"
        "surface_solar_radiation_downwards"
    ],
    "year": yr,
    "month": m,
    "day": [
        "01", "02", "03",
        "04", "05", "06",
        "07", "08", "09",
        "10", "11", "12",
        "13", "14", "15",
        "16", "17", "18",
        "19", "20", "21",
        "22", "23", "24",
        "25", "26", "27",
        "28", "29", "30",
        "31"
    ],
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

    file="ECMWF_CARDAMOM_HOURLY_DRIVER_TEMP_RAD"+m+yr+".nc"
    client = cdsapi.Client()
    client.retrieve(dataset, request).download(file)


#Main code

DOWNLOAD_ECMWF_HOURLY_DRIVERS_FOR_CARDAMOM("01", "2001");

