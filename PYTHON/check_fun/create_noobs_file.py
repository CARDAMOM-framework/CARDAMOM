#!/usr/bin/env python3
"""
Simple script to replicate MATLAB behavior:
Set observation variables to NaN (multiply by NaN) and write out.
"""
import sys
import netCDF4 as nc
import numpy as np

def create_noobs_file(input_file, output_file):
    """
    Replicate MATLAB approach:
    CBF.ABGB.values=CBF.ABGB.values*NaN;
    CBF.LAI.values=CBF.LAI.values*NaN;
    etc.
    """
    # Read input file
    ds_in = nc.Dataset(input_file, 'r')

    # Create output file (copy structure)
    ds_out = nc.Dataset(output_file, 'w', format='NETCDF4')

    # Copy dimensions
    for name, dimension in ds_in.dimensions.items():
        ds_out.createDimension(name, len(dimension) if not dimension.isunlimited() else None)

    # Observation variables to set to NaN
    obs_vars = ['ABGB', 'LAI', 'SCF', 'NBE', 'H', 'LE']

    # Copy all variables
    for name, variable in ds_in.variables.items():
        # Create variable with same attributes
        outVar = ds_out.createVariable(name, variable.datatype, variable.dimensions)

        # Copy attributes
        outVar.setncatts({k: variable.getncattr(k) for k in variable.ncattrs()})

        # Copy data, but multiply obs vars by NaN
        data = variable[:]
        if name in obs_vars:
            outVar[:] = data * np.nan  # Exactly like MATLAB
            print(f"Set {name} to NaN (data * NaN)")
        else:
            outVar[:] = data

    ds_in.close()
    ds_out.close()
    print(f"\nCreated: {output_file}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 create_noobs_file.py input.cbf.nc output.cbf.nc")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    create_noobs_file(input_file, output_file)
