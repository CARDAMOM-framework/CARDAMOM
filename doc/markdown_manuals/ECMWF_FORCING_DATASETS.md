# ECMWF forcing datasets

##Overview 

Meteorological forcing datasets a re needed to run CARDAMOM. This page currently describes how to acquire the ECMWF ERA5 datasets needed to run DALEC 1100.

##Step 1. install ECMWF API

https://confluence.ecmwf.int/display/CKB/How+to+download+ERA5


##Step 2. Test to make sure you can download test file

Example file is here:
https://github.com/CARDAMOM-framework/CARDAMOM/blob/main/PYTHON/ECMWF/ERA5_TEST_DOWNLOAD_API.py

you can test this locally:

>> python3 /local/path/to/CARDAMOM/PYTHON/ECMWF/ERA5_TEST_DOWNLOAD_API.py

The outcome is a netcdf file in your local path with a global January 2001 monthly average 2m air temperature map


##Step 3. Format or source all python files needed to acquire data

The list of drivers needed for CARDAMOM:
- Total downward radiation
- Total precip
- Total snow cover fraction
- 2m Dewpoint temperature min max
- Surface Skin Temperature
- 2m temperayure min max


[CONTINUE FROM HERE]


##General notes for using ECMWF for CARDAMOM:
- Min & Max values are derived from monthly means averaged by hour product, named "Monthly averaged reanalysis by hour of day"
- Rest of values derived derived from monthly means product, named "Monthly averaged reanalysis"
- 

##Additional resources

ERA5 1940-present online GUI for selecting and downloading data
https://cds.climate.copernicus.eu/datasets/reanalysis-era5-single-levels-monthly-means?tab=download

ERA5 interpolation and grid definitions here:
https://confluence.ecmwf.int/display/CKB/ERA5%3A+What+is+the+spatial+reference





