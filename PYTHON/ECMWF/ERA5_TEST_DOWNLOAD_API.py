import cdsapi

dataset = "reanalysis-era5-single-levels-monthly-means"
request = {
    "product_type": ["monthly_averaged_reanalysis"],
    "variable": ["2m_temperature"],
    "year": ["2001"],
    "month": ["01"],
    "time": ["00:00"],
    "data_format": "netcdf",
    "grid": "0.25/0.25",
    "download_format": "unarchived",
    "area": [90, -180, -90, 180]
}
file = "TEST_ECMWF_OUTPUT_FILE.nc"
client = cdsapi.Client()
client.retrieve(dataset, request).download(file)
