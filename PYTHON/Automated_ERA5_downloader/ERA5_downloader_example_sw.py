#!/usr/bin/env python
# coding: utf-8
 
# In[ ]:
 
 
# quick steps to use this script:
# 1. Navigate with keyword 'optional' to switch setups upon your need
# 2. After your edits, rename the file if needed
 
# 3. Convert .ipynb to .py: jupyter nbconvert --to script ERA5_downloader_2mTemp.ipynb (navigate to file dir first)
 
# 4. In your terminal, run: 
#    python ERA5_downloader_2mTemp.py --output_dir /Volumes/easystore/ERA5_single_level_2mTemp/ --variables 2m_temperature --start_year 1961 --end_year 1990 --max_workers 6
 
#    where --outputdir is destination to download the data, --variable is ERA5 climate variables of interest and can be single or multiple, 
#    the script will submit a request and save in one .nc file for each year between the --start_year and --end_year 
#    --max_workers is the number of requests to the server submitted at a time.
#    the script will automatically search for years that already exist in your destination directory, 
#    avoid having to download the same file again, if the previous API call got intterupted and have to run the script another time.
 
import cdsapi
import concurrent.futures
import os
import argparse
import logging
from functools import wraps
import time
 
# Configure logging
logging.basicConfig(
    filename='era5_download_2mTemp_6_34.log',
    level=logging.INFO,
    format='%(asctime)s %(levelname)s:%(message)s'
)
 
# Retry decorator with exponential backoff
def retry(max_retries=5, initial_delay=60, backoff_factor=2):
    def decorator(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            delay = initial_delay
            for attempt in range(1, max_retries + 1):
                try:
                    return func(*args, **kwargs)
                except Exception as e:
                    logging.warning(f"Attempt {attempt} failed for {args}: {e}")
                    if attempt == max_retries:
                        logging.error(f"All {max_retries} attempts failed for {args}")
                        raise
                    logging.info(f"Retrying in {delay} seconds...")
                    time.sleep(delay)
                    delay *= backoff_factor
        return wrapper
    return decorator
 
# Function to verify the integrity of a NetCDF file using xarray
def verify_netcdf(file_path):
    import xarray as xr
    try:
        ds = xr.open_dataset(file_path)
        ds.close()
        logging.info(f"Verification successful for {file_path}.")
        return True
    except Exception as e:
        logging.error(f"Verification failed for {file_path}: {e}")
        return False
 
# Function to download data for a specific year with retries
@retry(max_retries=5, initial_delay=60, backoff_factor=2)
def download_year(year, client, output_dir, variables):
    # Construct the output file name based on variables
    if len(variables) == 1:
        vars_str = variables[0]
    else:
        vars_str = '_'.join(variables)
    
    file_name = f'era5_{"_".join(variables)}_{year}.nc'
    file_path = os.path.join(output_dir, file_name)
    
    # Check if file already exists and is valid
    if os.path.exists(file_path):
        if verify_netcdf(file_path):
            print(f"File for year {year} with variables {variables} already exists and is valid. Skipping download.")
            logging.info(f"File for year {year} with variables {variables} already exists and is valid. Skipping download.")
            return
        else:
            print(f"File for year {year} with variables {variables} is corrupted or incomplete. Deleting and re-downloading.")
            logging.info(f"File for year {year} with variables {variables} is corrupted or incomplete. Deleting and re-downloading.")
            os.remove(file_path)
    
    try:
        print(f"Starting download for year {year} with variables {variables}...")
        logging.info(f"Starting download for year {year} with variables {variables}.")
        
        client.retrieve(
            'derived-era5-single-levels-daily-statistics',                       # optional 'derived-era5-single-levels-daily-statistics' 'derived-era5-land-daily-statistics'
            {
                'product_type': 'reanalysis',
                'variable': variables,  # List of variables
                'year': str(year),
                'month': [str(i).zfill(2) for i in range(1, 13)],  # optional Months from January to December 1-13
                'day': [str(i).zfill(2) for i in range(1, 32)],    # optional All days in a month 1-32
                "daily_statistic": 'daily_maximum',                # optional switch to daily_maximum
                "time_zone": "utc+00:00",                          # optional switch to different time zones
                "frequency": "1_hourly",                           # optional switch to '3_hourly' or '6_hourly'
                'format': 'netcdf',  # Download format (NetCDF)
                # "area": [30, -180, -30, 90]                        # optional remove for global, toggle for different areas [N, W, S, E]
            },
            file_path  # Output file name for each year and variable(s)
        )
        
        # Verify download
        if verify_netcdf(file_path):
            print(f"Finished downloading data for year {year} with variables {variables}.")
            logging.info(f"Finished downloading data for year {year} with variables {variables}.")
        else:
            print(f"Download for year {year} with variables {variables} incomplete. Will retry.")
            logging.warning(f"Download for year {year} with variables {variables} incomplete. Will retry.")
            raise Exception("Incomplete download.")
    except Exception as e:
        print(f"Error downloading data for year {year} with variables {variables}: {e}")
        logging.error(f"Error downloading data for year {year} with variables {variables}: {e}")
        raise
 
def main():
    # Set up argument parser
    parser = argparse.ArgumentParser(description='Download ERA5 data using cdsapi with controlled concurrency.')
    parser.add_argument('--output_dir', type=str, required=True,
                        help='Directory to save downloaded .nc files')
    parser.add_argument('--variables', type=str, nargs='+', default=['2m_temperature'],
                        help='Variables to download from ERA5 (default: 2m_temperature)')
    parser.add_argument('--start_year', type=int, default=1961, help='Start year for download (inclusive)')
    parser.add_argument('--end_year', type=int, default=1989, help='End year for download (inclusive)')
    parser.add_argument('--max_workers', type=int, default=6,
                        help='Maximum number of concurrent downloads (default: 4)')
    
    args = parser.parse_args()
    
    # Ensure output directory exists
    if not os.path.isdir(args.output_dir):
        try:
            os.makedirs(args.output_dir, exist_ok=True)
            print(f"Created directory: {args.output_dir}")
            logging.info(f"Created directory: {args.output_dir}")
        except Exception as e:
            print(f"Failed to create directory {args.output_dir}: {e}")
            logging.error(f"Failed to create directory {args.output_dir}: {e}")
            return
    
    # Initialize CDS API client
    try:
        client = cdsapi.Client()
    except Exception as e:
        print(f"Failed to initialize CDS API client: {e}")
        logging.error(f"Failed to initialize CDS API client: {e}")
        return
    
    # Define the range of years
    years = list(range(args.start_year, args.end_year + 1))
    
    # Use ThreadPoolExecutor to manage concurrency
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.max_workers) as executor:
        # Submit all download tasks
        futures = [
            executor.submit(download_year, year, client, args.output_dir, args.variables) for year in years
        ]
        for future in concurrent.futures.as_completed(futures):
            try:
                future.result()
            except Exception as e:
                print(f"Download failed: {e}")
                logging.error(f"Download failed: {e}")
 
if __name__ == "__main__":
    main()
