#!/usr/bin/env python
"""\
This script converts monthly 0.05-degree (CMG) MOD10 HDF files 
downloaded from https://nsidc.org/data/mod10cm/versions/61
into annual 0.5-degree NetCDF files compatible with the CARDAMOM-MAPS conventions

Written by Paul Levine https://github.com/alkalifly

Usage: run modis_scf_conversion.py without any arguments 
after setting input_dir and output_dir paths on lines 19 and 20
"""

import glob
import numpy as np
import xarray as xr
import xesmf as xe
from pyhdf.SD import SD, SDC

input_dir = '/path/to/directory/with/modis/files'
output_dir = '/path/to/directory/with/output/files'

first_year = 2001  # first complete year of MODIS data
last_year = 2021   # last complete year of MODIS data

lats = np.arange(-90,90,0.5)+0.25
lons = np.arange(-180,180,0.5)+0.25
yearly = xr.DataArray(np.nan*np.ones((12,lons.size,lats.size)),
	coords=[np.arange(12)+1,lons,lats],dims=['time','longitude','latitude'],name='data')
out_grid = xr.Dataset(
    {
        "lat": (["lat"], lats),
        "lon": (["lon"], lons),
    }
)

mod_lats = np.arange(90,-90,-0.05)-0.025
mod_lons = np.arange(-180,180,0.05)+0.025
mod_darr = xr.DataArray(np.nan*np.ones((12,mod_lats.size,mod_lons.size)),
	coords=[np.arange(12)+1,mod_lats,mod_lons],dims=['time','lat','lon'],name='modis')
regridder = xe.Regridder(mod_darr, out_grid, "bilinear") #choose conservative algorithm for conserved quantities

for year in range(first_year,last_year+1):
	hdf_files = sorted(glob.glob('{}/MOD10CM.A{}*hdf'.format(input_dir,year)))
	for m,hdf_file in enumerate(hdf_files):
		mod_darr[m,:,:] = SD(hdf_file, SDC.READ).select('Snow_Cover_Monthly_CMG')[:,:]
	mod_darr = mod_darr.where(mod_darr <= 100)/100
	out = regridder(mod_darr).rename('data').rename({'lat':'latitude','lon':'longitude'})
	out.attrs = {'units':1,'description':'Snow cover fraction (0 = no snow, 1 = complete coverage)'}
	out.encoding = {'_FillValue':np.nan}
	out.latitude.attrs = {'units':'degrees'}
	out.longitude.attrs = {'units':'degrees'}
	out.time.attrs = {'units':'Months since December of previous year'}
	out.to_netcdf('/CARDAMOM-MAPS_05deg_MODIS_SCF_{}.nc'.format(year))
