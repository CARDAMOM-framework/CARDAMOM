import xarray as xr
import numpy as np
import warnings 
import netCDF4
from netCDF4 import Dataset

#Set constant
EARTH_RADIUS_METERS = 6.371e6
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
    
def initialize_regridding(DataHD): #Data formatting rules apply!! 
    #Takes in a half-degree (only!) xarray.DataArray gridded variable in the form DataHD.var_name, and returns it regridded to the irregular GeosChem 4x5 grid
    #input lat/lon dimensions must be called "latitude" and "longitude" 
    #input lat/lon dimensions must be given in degrees (i.e. -179.75, -179.25, etc., no numerical index like 0, 1, 2, etc.)
    #It is assumed that half degree coordinates are pixel centers
    #It is assumed that input half-degree per-area data are defined relatieve to whole pixel area; 
    #In contrast, Output 4x5 per-area data will be defined relative to the area of the mask of contributing half-degree pixels; approximately per pixel land-only area     

    req_dims = ['latitude', 'longitude']
    for d in req_dims:
        if d not in DataHD.dims:
            raise ValueError(f"Input data must have dimension '{d}'")

    other_dims = [d for d in DataHD.dims if d not in req_dims]
    other_coords = {d: DataHD.coords[d] for d in other_dims}

    var_name = DataHD.name
    if var_name is None:
        raise ValueError("Input DataArray must have a name (e.g., `DataHD.name`).")

    lon_res = 5.0
    lat_res = 4.0
    lon_centers = np.arange(-180, 180, 5) 
    lon_edges = np.concatenate(([lon_centers[0] - lon_res / 2], lon_centers + lon_res / 2))
    lat_centers = np.array([-89.] + list(np.arange(-86, 86+1, 4)) + [89.])
    core_edges_4deg = np.arange(-88, 88 + 4, 4)
    lat_edges = np.concatenate(([-90.0], core_edges_4deg, [90.0]))
    Areas4x5=_calculate_spherical_grid_areas(lat_edges, lon_edges, R=EARTH_RADIUS_METERS)
    
    Areas4x52 = xr.Dataset(
        data_vars=dict(
            Areas4x5=(['latitude', 'longitude'], Areas4x5)
        ),
        coords=dict(
            latitude=lat_centers,
            longitude=lon_centers
        )
    )
    
    lon_res = 0.5
    lat_res = 0.5
    lon_centers = DataHD.longitude.data 
    lon_edges = np.concatenate(([lon_centers[0] - lon_res / 2], lon_centers + lon_res / 2))
    lat_centers = DataHD.latitude.data
    lat_edges = np.concatenate(([lat_centers[0] - lat_res / 2], lat_centers + lat_res / 2))
    
    AreasHD=_calculate_spherical_grid_areas(lat_edges, lon_edges, R=EARTH_RADIUS_METERS)
    
    AreasHD2 = xr.Dataset(
        data_vars=dict(
            AreasHD=(['latitude', 'longitude'], AreasHD)
        ),
        coords=dict(
            latitude=DataHD.latitude.data,
            longitude=DataHD.longitude.data
        )
    )
    
    target_lat = Areas4x52.latitude
    target_lon = Areas4x52.longitude
    target_area = Areas4x52.Areas4x5
    coords = {'latitude': target_lat, 'longitude': target_lon}
    coords.update(other_coords)
    shape = (len(target_lat), len(target_lon)) + tuple(len(other_coords[d]) for d in other_dims)
    
    
    # Create an nD DataArray filled with NaNs: (other_dims (e.g. time), latitude, longitude)

    data_regridded = xr.DataArray(
        data=np.full(shape, np.nan),
        coords=coords,
        dims=['latitude', 'longitude'] + other_dims
    )

    contributing_area = xr.full_like(target_area, fill_value=np.nan)

    # Create dataset to hold both variables
    data4x5 = xr.Dataset({
        var_name: data_regridded,
        f'contributing_area_{var_name}': contributing_area
    })

    return data4x5, Areas4x52,AreasHD2

def regridding05x05to4x5(DataHD):#Data formatting rules apply!! 
    #Takes in a half-degree (only!) xarray.DataArray gridded variable in the form DataHD.var_name, and returns it regridded to the irregular GeosChem 4x5 grid
    #input lat/lon dimensions must be called "latitude" and "longitude" 
    #input lat/lon dimensions must be given in degrees (i.e. -179.75, -179.25, etc., no numerical index like 0, 1, 2, etc.)
    #It is assumed that half degree coordinates are pixel centers
    #It is assumed that input half-degree per-area data are defined relative to whole pixel area; 
    #In contrast, Output 4x5 per-area data will be defined relative to the area of the mask of contributing half-degree pixels; approximately per pixel land-only area     
    
    data4x5, Areas4x52,AreasHD2 = initialize_regridding(DataHD)
    non_spatial_dims = [d for d in DataHD.dims if d not in ['latitude', 'longitude']]
    subset_two_poles=(DataHD*AreasHD2.AreasHD)[dict(latitude=np.concatenate([np.arange(4), np.arange(356,360)]))]
    subset_middle=(DataHD*AreasHD2.AreasHD)[dict(latitude=np.arange(4,356))]

    TotalmassHD=DataHD*AreasHD2.AreasHD
    
    for i in subset_two_poles.latitude[::4].data: 
        lat_idx=np.where(subset_two_poles.latitude==i)[0][0]
        #print(i,lat_idx)
        lat_range=subset_two_poles.latitude[np.arange(lat_idx,lat_idx+4)]
        #print(lat_range.data)
    
        for j in subset_two_poles.longitude[5:][::10].data:
            if j == subset_two_poles.longitude[5:][::10][-1]:
                #print('wrapping longitude:')
                lon_range=subset_two_poles.longitude.data[np.r_[0:5, 715:720]]  
                lon4x5=-180.0
                #print(lon4x5)
            else:
                lon_idx=np.where(subset_two_poles.longitude==j)[0][0]
                # print(j,lon_idx)
                lon_range=subset_two_poles.longitude[np.arange(lon_idx,lon_idx+10)]
                #print(lon_range.data)
                lon4x5=lon_range.data.mean()
                #print(lon4x5)
    
            #Select data
            data_temp=DataHD[dict(latitude=np.where(DataHD.latitude.isin(lat_range))[0],
                                      longitude=np.where(DataHD.longitude.isin(lon_range))[0])].astype('float64')
            areas_temp=AreasHD2.AreasHD[dict(latitude=np.where(DataHD.latitude.isin(lat_range))[0],
                                      longitude=np.where(DataHD.longitude.isin(lon_range))[0])].astype('float64')
            datasum=(data_temp*areas_temp).sum('latitude').sum('longitude')
            #print(areas_temp.sum('latitude').sum('longitude').data)
            areasum=areas_temp.where(~np.isnan(data_temp.mean(dim=non_spatial_dims))).sum('latitude').sum('longitude').data
            #print(areasum)
    
            if areasum == 0:
                # In case of no data:
                regridded_value = datasum * np.nan
            else:
                regridded_value = datasum / areasum # This division results in a scalar or DataArray

            # Set up flexible xarray assignment (step 1)
            var_name = list(data4x5.data_vars)[0]
            contributing_area_name = list(data4x5.data_vars)[1]
            
            lat4x5=lat_range.data.mean()
            #print(lat4x5)
            lat4x5_idx=np.where(data4x5.latitude==lat4x5)[0][0]
            lon4x5_idx=np.where(data4x5.longitude==lon4x5)[0][0]
            #print(lat4x5_idx,lon4x5_idx)
            
            # Set up flexible xarray assignment (step 2)
            loc_dict = {dim: data4x5.coords[dim] for dim in non_spatial_dims} # Get actual coords from data4x5
            loc_dict['latitude'] = lat4x5
            loc_dict['longitude'] = lon4x5

            # Assign directly. Xarray handles the dimensionality of 'regridded_value' correctly.
            data4x5[list(data4x5.data_vars)[0]].loc[loc_dict] = regridded_value
            data4x5[list(data4x5.data_vars)[1]][dict(latitude=lat4x5_idx, longitude=lon4x5_idx)] = areasum
    
    for i in subset_middle.latitude[::8].data: 
        lat_idx=np.where(subset_middle.latitude==i)[0][0]
        #print(i,lat_idx)
        lat_range=subset_middle.latitude[np.arange(lat_idx,lat_idx+8)]
        #print(lat_range.data)
    
        for j in subset_middle.longitude[5:][::10].data:
            if j == subset_middle.longitude[5:][::10][-1]:
                #print('wrapping longitude:')
                lon_range=subset_middle.longitude.data[np.r_[0:5, 715:720]]  
                lon4x5=-180.0
                #print(lon4x5)
            else:
                lon_idx=np.where(subset_middle.longitude==j)[0][0]
                #print(j,lon_idx)
                lon_range=subset_middle.longitude[np.arange(lon_idx,lon_idx+10)]
                #print(lon_range.data)
                lon4x5=lon_range.data.mean()
                #print(lon4x5)
    
            #Select data
            data_temp=DataHD[dict(latitude=np.where(DataHD.latitude.isin(lat_range))[0],
                                      longitude=np.where(DataHD.longitude.isin(lon_range))[0])].astype('float64')
            areas_temp=AreasHD2.AreasHD[dict(latitude=np.where(DataHD.latitude.isin(lat_range))[0],
                                      longitude=np.where(DataHD.longitude.isin(lon_range))[0])].astype('float64')
            datasum=(data_temp*areas_temp).sum('latitude').sum('longitude')
            #print(areas_temp.sum('latitude').sum('longitude').data)
            areasum=areas_temp.where(~np.isnan(data_temp.mean(dim=non_spatial_dims))).sum('latitude').sum('longitude').data
            #print(areasum)

            if areasum == 0:
                # In case of no data:
                regridded_value = datasum * np.nan
            else:
                regridded_value = datasum / areasum # This division results in a scalar or DataArray

            # Set up flexible xarray assignment (step 1)
            var_name = list(data4x5.data_vars)[0]
            contributing_area_name = list(data4x5.data_vars)[1]
            
            lat4x5=lat_range.data.mean()
            #print(lat4x5)
            lat4x5_idx=np.where(data4x5.latitude==lat4x5)[0][0]
            lon4x5_idx=np.where(data4x5.longitude==lon4x5)[0][0]
            #print(lat4x5_idx,lon4x5_idx)
            
            # Set up flexible xarray assignment (step 2)
            loc_dict = {dim: data4x5.coords[dim] for dim in non_spatial_dims} # Get actual coords from data4x5
            loc_dict['latitude'] = lat4x5
            loc_dict['longitude'] = lon4x5

            # Assign directly. Xarray handles the dimensionality of 'regridded_value' correctly.
            data4x5[list(data4x5.data_vars)[0]].loc[loc_dict] = regridded_value
            data4x5[list(data4x5.data_vars)[1]][dict(latitude=lat4x5_idx, longitude=lon4x5_idx)] = areasum

    print('Mass conservation checks: ')
    print('HD mass computed='+str(TotalmassHD.sum('latitude').sum('longitude').sum(dim=non_spatial_dims).data))
    print('4x5 mass='+str((data4x5[list(data4x5.data_vars)[0]]*data4x5[list(data4x5.data_vars)[1]]).sum('latitude').sum('longitude').sum(dim=non_spatial_dims).data))
    
    return(data4x5)