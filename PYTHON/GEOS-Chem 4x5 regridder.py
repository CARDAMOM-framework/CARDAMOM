import os
import sys
import string
import subprocess
import xarray as xr

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import netCDF4
from netCDF4 import MFDataset
from netCDF4 import Dataset
from IPython.display import display, Math, Latex
import pandas as pd
from matplotlib import cm
import datetime
from datetime import datetime, timedelta
from operator import * 
from matplotlib.dates import DateFormatter
import scipy.io as sp

import cartopy
import cartopy.crs as ccrs
import cartopy.feature as cfeature

import rasterio
from rasterio.features import rasterize
from rasterio.warp import reproject
from rasterio.transform import from_bounds, Affine
from rasterio.enums import Resampling
import warnings
from shapely.geometry import Polygon
from shapely.geometry import mapping
import geopandas as gpd


EARTH_RADIUS_METERS = 6.371e6 # Standard Earth radius in meters; change here for different area units

def _calculate_spherical_grid_areas(lat_edges, lon_edges, R=EARTH_RADIUS_METERS):
    """
    Calculates the area of each grid cell on a sphere.
    Formula: R^2 * (lon2 - lon1) * (sin(lat2) - sin(lat1))
    Lat/Lon edges should be in degrees. Output area in m^2.
    """
    lon_rad = np.deg2rad(lon_edges)
    lat_rad = np.deg2rad(lat_edges)

    d_lon = np.diff(lon_rad)[np.newaxis, :]
    sin_lat2_minus_sin_lat1 = np.diff(np.sin(lat_rad))[:, np.newaxis]

    areas = (R**2) * d_lon * sin_lat2_minus_sin_lat1
    return areas

def get_geoschem_grid_info(gcres_str):
    """
    Defines GEOS-Chem grid edges, centers, and cell areas based on your custom 4x5 definition.
    Returns a dictionary with 'lat_edges', 'lon_edges', 'lat_centers', 'lon_centers', 'area'.
    """
    if gcres_str == '4x5':
        lon_res = 5.0
        lat_res = 4.0
             # longitude centers (72 points)
        lon_centers = np.arange(-180, 180, 5) 

        # Derive edges from these centers.
        lon_edges = np.concatenate(([lon_centers[0] - lon_res / 2], lon_centers + lon_res / 2))
        # This generates: [-182.5, -177.5, ..., 172.5, 177.5] (73 points)

        # Verify the number of edges: should be len(lon_centers) + 1
        if len(lon_edges) != len(lon_centers) + 1:
            warnings.warn(f"Calculated lon_edges ({len(lon_edges)}) does not match expected "
                          f"number for lon_centers ({len(lon_centers)} + 1). "
                          "This might indicate an issue in grid definition.")

        # Latitude definition (from previous correction, kept the same)
        lat_centers = np.array([-89.] + list(np.arange(-86, 86+1, 4)) + [89.])
        core_edges_4deg = np.arange(-88, 88 + 4, 4)
        lat_edges = np.concatenate(([-90.0], core_edges_4deg, [90.0]))
        if len(lat_edges) != len(lat_centers) + 1:
            warnings.warn(f"Calculated lat_edges ({len(lat_edges)}) does not match expected "
                          f"number for lat_centers ({len(lat_centers)} + 1). "
                          "This might indicate an issue in grid definition.")


    elif gcres_str == '2x2.5':
        lon_res = 2.5
        lat_res = 2.0
        
        lon_centers = np.arange(-180, 180, 2.5)
        lon_edges = np.concatenate(([lon_centers[0] - lon_res / 2], lon_centers + lon_res / 2))

        lat_edges = np.arange(-90, 90.001, 2) # 91 edges -> 90 cells
        lat_centers = (lat_edges[:-1] + lat_edges[1:]) / 2
    else:
        raise ValueError(f"Unsupported GEOS-Chem resolution: {gcres_str}. Use '4x5' or '2x2.5'.")

    area = _calculate_spherical_grid_areas(lat_edges, lon_edges)

    return {
        'lat_edges': lat_edges,
        'lon_edges': lon_edges,
        'lat_centers': lat_centers,
        'lon_centers': lon_centers, # This will now be your exact desired array
        'area': area,
    }

    # Assuming _calculate_spherical_grid_areas and get_geoschem_grid_info are already defined

def GEOSChem_regular_grid_to_GC_simplified(FIN: xr.DataArray, gcres_str: str = '4x5') -> xr.DataArray:
    """
    Regrids an input xarray DataArray (FIN) from a regular lat/lon grid
    to a GEOS-Chem grid (e.g., 4x5 or 2x2.5) using conservative remapping (summing).

    Args:
        FIN (xr.DataArray): Input DataArray with 'latitude' and 'longitude' dimensions.
                            Assumed to be in g/m^2 (mass per unit area).
        gcres_str (str): GEOS-Chem resolution string, e.g., '4x5' or '2x2.5'.

    Returns:
        xr.DataArray: Regridded DataArray in g/m^2.
    """
        # 1. Get output grid information
    gc_grid = get_geoschem_grid_info(gcres_str)

        # 2. Extract input grid information 
    input_lat_res = abs(float(FIN.latitude.diff('latitude')[0]))
    input_lon_res = abs(float(FIN.longitude.diff('longitude')[0]))
            # Derive input edges from centers for transform
    input_lat_edges = np.concatenate((FIN.latitude.values - input_lat_res / 2, [FIN.latitude.values[-1] + input_lat_res / 2]))
    input_lon_edges = np.concatenate((FIN.longitude.values - input_lon_res / 2, [FIN.longitude.values[-1] + input_lon_res / 2]))
            # Calculate input grid areas (for mass conversion)
    input_area = _calculate_spherical_grid_areas(input_lat_edges, input_lon_edges)
    input_area_broadcasted = input_area[tuple(slice(None) if i < 2 else np.newaxis for i in range(len(FIN.shape)))]

            # Make transform object for `rasterio.reproject`
    input_transform = from_bounds(
        west=input_lon_edges[0],
        south=input_lat_edges[0], # Assuming southern-most edge is at index 0 (e.g. -90)
        east=input_lon_edges[-1],
        north=input_lat_edges[-1], # Assuming northern-most edge is at index -1 (e.g. 90)
        width=FIN.sizes['longitude'],
        height=FIN.sizes['latitude'])

        # 3. Prepare output grid information
    output_shape = (len(gc_grid['lat_centers']), len(gc_grid['lon_centers']))

            # Make `output_transform` for `rasterio.reproject`
    output_transform = from_bounds(
        west=gc_grid['lon_edges'][0] ,
        south=gc_grid['lat_edges'][0],
        east=gc_grid['lon_edges'][-1],
        north=gc_grid['lat_edges'][-1],
        width=output_shape[1],
        height=output_shape[0])

            # Initialize output DataArray to store regridded mass (not per area yet); The output will store total mass in each coarse grid cell.
    regridded_mass_data = np.zeros(FIN.shape[:2] + output_shape, dtype=FIN.dtype) # (lat_in, lon_in, lat_out, lon_out) if all dims, but no, (time, species, lat_out, lon_out)
            # Adjust regridded_mass_data shape for higher dimensions (time, anything else, etc.)
            # (The shape should be (output_lat_cells, output_lon_cells) plus original extra dims)
    regridded_mass_data_shape = output_shape + FIN.shape[2:]
    regridded_mass_data = np.zeros(regridded_mass_data_shape, dtype=FIN.dtype)


        # 4. Convert per-area units (g/m^2) to total mass (g) for each input cell
            # IMPORTANT: Convert NaNs to 0.0. These 0.0s will be treated as actual data (zero mass), unless src_nodata is explicitly set to 0.0 in reproject.
            # For mass, we want 0s to contribute 0 mass.
    mass_data = np.nan_to_num(FIN.values, nan=0.0) * input_area_broadcasted

    lat_dim_idx = FIN.dims.index('latitude')
    lon_dim_idx = FIN.dims.index('longitude')
            # Define not_nan_mask here, before the if condition; IMPORTANT, otherwise you wind up dividing by the whole broadcast area and dilute values on the coasts
    not_nan_mask = ~np.isnan(FIN.values)
    
    if FIN.ndim > 2:
            # Move spatial dimensions to front for consistent processing
            # Example: if FIN is (time, species, lat, lon), make it (lat, lon, time, species)
        reordered_mask = np.moveaxis(not_nan_mask, (lat_dim_idx, lon_dim_idx), (0, 1))
        spatial_data_mask = np.any(reordered_mask, axis=tuple(range(2, FIN.ndim)))
    else:
        spatial_data_mask = not_nan_mask

            # Create the 2D source area grid for reprojecting areas: Where spatial_data_mask is False (all NaNs in FIN for that lat/lon), set area to 0.0. Otherwise, use the actual input_area.
    source_area_for_reproject = np.where(spatial_data_mask, input_area, 0.0)
            # Ensure it's the correct dtype for reproject
    source_area_for_reproject = source_area_for_reproject.astype(FIN.dtype) # or float64 for areas
            # Initialize output array for summed areas (2D)
    regridded_contributing_area = np.zeros(output_shape, dtype=FIN.dtype) # Use FIN.dtype or float64

            # *****Regrid the source areas to the target grid here*****
    reproject(
        source=source_area_for_reproject, # Pass the 2D source area directly
        src_transform=input_transform,
        src_crs="EPSG:4326",
        destination=regridded_contributing_area,
        dst_transform=output_transform,
        dst_crs="EPSG:4326",
        resampling=Resampling.sum, # Sum the areas
        src_nodata=0.0, # IMPORTANT: areas with value=0.0 (where FIN was NaN across all slices) are ignored
        num_threads=4)
            # Ensure regridded_contributing_area doesn't have tiny negative values from floating point
    regridded_contributing_area = np.maximum(regridded_contributing_area, 0.0)

            # Broadcast regridded_contributing_area to match the full output shape for final division
    regridded_contributing_area_broadcasted = regridded_contributing_area[
        tuple(slice(None) if i < 2 else np.newaxis for i in range(len(regridded_mass_data.shape)))]

            # Store total input mass for scaling factor, before any regridding.
    sf_apply = 0
    if np.all(np.isfinite(FIN.values)) and np.all(FIN.values >= 0): # Check if input data is clean for sf calculation
        sf_apply = 1
            # Calculate total input mass based on original FIN values where they exist.
            # This is `FIN.values * input_area` where FIN is not NaN.
        total_input_mass_original = np.nansum(FIN.values * input_area_broadcasted)

        # 5. Loop over the additional dimensions (time, species, etc.) for regridding mass
            # Get indices for extra dimensions (e.g., time, species)
    extra_dims_shape = FIN.shape[2:]
    for idx_tuple in np.ndindex(extra_dims_shape):
        current_input_slice_index = (Ellipsis,) + idx_tuple
        input_2d_mass_array = mass_data[current_input_slice_index] # This array now has 0s where original FIN had NaNs
        output_2d_regridded_mass = np.zeros(output_shape, dtype=mass_data.dtype)

        reproject(
            source=input_2d_mass_array,
            src_transform=input_transform,
            src_crs="EPSG:4326",
            destination=output_2d_regridded_mass,
            dst_transform=output_transform,
            dst_crs="EPSG:4326",
            resampling=Resampling.sum,
            # KEEP src_nodata=0.0 here for the mass array, IF 0 means "no data/no mass contribution"
            # If 0 can represent actual zero mass, then remove src_nodata=0.0 here.
            num_threads=4)
        current_output_slice_index = tuple(slice(None) for _ in output_shape) + idx_tuple
        regridded_mass_data[current_output_slice_index] = output_2d_regridded_mass

    # 6. Convert back to per-area units (g/m^2) -- NOW DIVIDE BY CONTRIBUTING AREA
    # Handle division by zero for cells where no data contributed (e.g., pure ocean cells)
    # These will result in NaN, which is appropriate for unpopulated cells.
    final_regridded_data = np.divide(regridded_mass_data, regridded_contributing_area_broadcasted,
                                     out=np.full_like(regridded_mass_data, np.nan),
                                     where=regridded_contributing_area_broadcasted != 0)

    # 7. Apply scale factor if applicable
    SF = 1.0
    if sf_apply == 1:
        # Sum of output mass over contributing areas only
        total_output_mass_before_sf = np.nansum(final_regridded_data * regridded_contributing_area_broadcasted)
        
        if total_input_mass_original > 0 and total_output_mass_before_sf > 0:
            SF = total_input_mass_original / total_output_mass_before_sf
        elif total_input_mass_original == 0 and total_output_mass_before_sf == 0:
            SF = 1.0
        elif total_input_mass_original > 0 and total_output_mass_before_sf == 0:
            warnings.warn("Input mass is non-zero, but output mass (over contributing areas) is zero. Scaling factor will be undefined.")
            SF = np.nan
            
    final_regridded_data_scaled = final_regridded_data * SF

    # 8. Create xarray DataArray for output
    FOUT = xr.DataArray(
        final_regridded_data_scaled,
        coords={
            'latitude': gc_grid['lat_centers'],
            'longitude': gc_grid['lon_centers'],
            **{dim: FIN.coords[dim].values for dim in FIN.dims[2:]} # Include other original dims
        },
        dims=['latitude', 'longitude'] + list(FIN.dims[2:]),
        name=FIN.name + '_regridded' if FIN.name else 'regridded_data',
        attrs=FIN.attrs # Inherit attributes from input
    )
    # Add attributes to indicate regridding
    FOUT.attrs['regridding_method'] = 'Conservative (sum) with contributing area normalization'
    FOUT.attrs['original_resolution'] = f'{input_lat_res}x{input_lon_res}'
    FOUT.attrs['target_resolution'] = gcres_str

    return FOUT

#######Example of use###############

# Total_prec=xr.load_dataset('/Users/bilir/Desktop/CardamomScratch/MET_DRIVERS_05x05/ERA5_TOTAL_PREC_2001-2024.nc').transpose('latitude','longitude','time')
# Total_prec4x5=GEOSChem_regular_grid_to_GC_simplified(Total_prec.PREC)

# fig, ax = plt.subplots(figsize=(20,25), subplot_kw={'projection': ccrs.PlateCarree()})
# ax.coastlines()

# data=(Total_prec4x5).mean('time')

# C = ax.pcolormesh(data.longitude, data.latitude, data, transform=ccrs.PlateCarree())
# C.set_clim(vmin=0,vmax=10)
# plt.colorbar(C,shrink=.15,label='mm day-1')
# plt.title('new Precip, new regridder  2001-2023 mean',fontsize=24)