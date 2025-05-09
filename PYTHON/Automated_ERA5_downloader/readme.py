Depending on the hourly, daily, or monthly data set you want to use, you can choose using different .py files.
 
For example daily download, you can use the ‘ERA5_downloader_daily_example.py’ script, and set all of user defined properties through terminal command when executing the python script.
 
Example command in terminal:
python ERA5_downloader_daily_example.py --output_dir /Your/output/dir/ --variables 2m_temperature --start_year 1993 --end_year 1993 --months 01 02 --days 01 02 --daily_statistics daily_max
 
Here’s a list of external variables use can define and their default values.
parser = argparse.ArgumentParser(description='Download ERA5 data using cdsapi with controlled concurrency.')
    parser.add_argument('--output_dir', type=str, required=True,
                        help='Directory to save downloaded .nc files')
    parser.add_argument('--variables', type=str, nargs='+', default=['2m_temperature'],
                        help='Variables to download from ERA5 (default: 2m_temperature)')
    parser.add_argument('--start_year', type=int, default=1961, help='Start year for download (inclusive)')
    parser.add_argument('--end_year', type=int, default=1989, help='End year for download (inclusive)')
    parser.add_argument('--max_workers', type=int, default=4,
                        help='Maximum number of concurrent downloads (default: 4)')
    parser.add_argument('--months', type=str, nargs='+',default=[str(i).zfill(2) for i in range(1, 13)],
                        help="Months to download (default: all months)")
    parser.add_argument('--days', type=str, nargs='+',default=[str(i).zfill(2) for i in range(1, 32)],
                        help="Days to download (default: all days)")
    parser.add_argument('--hours', type=str,nargs='+',default=[f'{i:02d}:00' for i in range(24)],
                        help="Hours to download (default: all hours)")
    parser.add_argument('--dataset', type=str, default='reanalysis-era5-single-levels',
                        help='derived-era5-single-levels-daily-statistics')
    parser.add_argument('--daily_statistics',type=str,default=['daily_mean'], help='select mean min max or sum')
   
Reason I used separate scripts is because if we use monthly dataset but have a default ‘—‘--hours’ or ‘--days’ option, it will confuse the API and return error.
