#!/usr/bin/env python3
"""
Diagnostic script to check why likelihood might return -inf for EDC-passing parameters.
"""
import sys
import numpy as np
import netCDF4 as nc

def check_observations(nc_path):
    """Check observation data availability and quality."""
    print(f"\n{'='*70}")
    print(f"OBSERVATION DATA DIAGNOSTIC: {nc_path}")
    print(f"{'='*70}\n")

    ds = nc.Dataset(nc_path, 'r')

    # List of common observation variables
    obs_vars = ['LAI', 'ABGB', 'SCF', 'NBE', 'H', 'LE', 'GPP']

    for var_name in obs_vars:
        if var_name not in ds.variables:
            continue

        data = ds.variables[var_name][:]

        # Handle masked arrays
        if np.ma.is_masked(data):
            valid_data = data.compressed()
        else:
            valid_data = data[~np.isnan(data)]
            valid_data = valid_data[valid_data != -9999.0]

        total_points = len(data) if data.ndim == 1 else data.size
        valid_points = len(valid_data)
        valid_pct = 100.0 * valid_points / total_points if total_points > 0 else 0

        print(f"{var_name}:")
        print(f"  Total timesteps: {total_points}")
        print(f"  Valid observations: {valid_points} ({valid_pct:.1f}%)")
        print(f"  Missing/invalid: {total_points - valid_points} ({100-valid_pct:.1f}%)")

        if valid_points > 0:
            print(f"  Range: [{np.min(valid_data):.3f}, {np.max(valid_data):.3f}]")

            # Check for uncertainty metadata
            if hasattr(ds.variables[var_name], 'opt_unc_type'):
                unc_type = ds.variables[var_name].opt_unc_type
                print(f"  Uncertainty type: {unc_type}")
                if hasattr(ds.variables[var_name], 'single_unc'):
                    single_unc = ds.variables[var_name].single_unc
                    print(f"  Single uncertainty: {single_unc}")
                if hasattr(ds.variables[var_name], 'single_annual_unc'):
                    single_unc = ds.variables[var_name].single_annual_unc
                    print(f"  Single annual uncertainty: {single_unc}")
        else:
            print(f"  ⚠️  NO VALID DATA - likelihood calculation may fail!")

        print()

    # Check EDC value
    if 'EDC' in ds.variables:
        edc = float(ds.variables['EDC'][:])
        print(f"EDC value: {edc}")
        if edc == 0:
            print("  → EDC=0: constraints disabled, any parameter set should work")
        else:
            print("  → EDC=1: constraints enabled, parameter space is restricted")

    ds.close()

    print(f"\n{'='*70}")
    print("SUMMARY:")
    print(f"{'='*70}")
    print("\nIf most observations show 0% valid data, the likelihood function")
    print("will return -inf because there's no data to fit against.")
    print("\nThis is EXPECTED behavior when running with test files that have")
    print("observations removed (like TEST_1_NO_OBS_NO_EDCs.cbf.nc).")
    print(f"\n{'='*70}\n")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 diagnose_likelihood_issue.py <cbf_file.nc>")
        sys.exit(1)

    check_observations(sys.argv[1])
