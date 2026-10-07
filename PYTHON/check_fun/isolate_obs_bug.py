#!/usr/bin/env python3
"""
Isolate which observation variable causes FLUXES to differ in forward model run.
This is a bug investigation - observations should NOT affect forward model dynamics.
"""
import sys
import os
import shutil
import netCDF4 as nc
import numpy as np
import subprocess

def create_test_with_single_obs(base_cbf, output_cbf, obs_var_to_keep):
    """Create a test file with only one observation variable retained, all others set to -9999."""
    shutil.copy2(base_cbf, output_cbf)

    ds = nc.Dataset(output_cbf, 'r+')

    # List of observation variables
    obs_vars = ['LAI', 'ABGB', 'SCF', 'NBE', 'H', 'LE', 'GPP', 'ET', 'DOM', 'CH4',
                'SWE', 'EWT', 'SIF', 'ROFF', 'FIR']

    # Set all obs to -9999 except the one we want to keep
    for var in obs_vars:
        if var in ds.variables:
            if var != obs_var_to_keep:
                ds.variables[var][:] = -9999.0

    ds.close()
    print(f"  Created {output_cbf} with only {obs_var_to_keep} observation")

def run_forward_model(cbf_file, cbr_file, output_file, exe_path):
    """Run CARDAMOM_RUN_MODEL.exe and return success status."""
    try:
        cmd = [exe_path, cbf_file, cbr_file, output_file]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        return result.returncode == 0 and os.path.exists(output_file)
    except Exception as e:
        print(f"    Error running model: {e}")
        return False

def compare_fluxes(output1, output2):
    """Compare FLUXES from two output files, return max difference and location."""
    try:
        ds1 = nc.Dataset(output1, 'r')
        ds2 = nc.Dataset(output2, 'r')

        fluxes1 = ds1.variables['FLUXES'][:]
        fluxes2 = ds2.variables['FLUXES'][:]

        # Compute absolute difference
        diff = np.abs(fluxes1 - fluxes2)

        # Ignore NaN differences
        diff_clean = np.where(np.isnan(diff), 0, diff)

        max_diff = np.max(diff_clean)
        max_loc = np.unravel_index(np.argmax(diff_clean), diff_clean.shape)

        ds1.close()
        ds2.close()

        return max_diff, max_loc, fluxes1[max_loc], fluxes2[max_loc]

    except Exception as e:
        print(f"    Error comparing: {e}")
        return None, None, None, None

if __name__ == "__main__":
    print("="*70)
    print("ISOLATING OBSERVATION VARIABLE CAUSING FLUX MISMATCH")
    print("="*70)

    # Paths
    test1_cbf = "TEST_1_NO_OBS_NO_EDCs.cbf.nc"
    test2_cbf = "TEST_2_OBS_NO_EDCs.cbf.nc"
    test1_cbr = "TEST_1_NO_OBS_NO_EDCs.cbr.nc"
    cardamom_exe = "/Users/abloom/CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.exe"

    if not all([os.path.exists(f) for f in [test1_cbf, test2_cbf, test1_cbr, cardamom_exe]]):
        print("ERROR: Required files not found")
        sys.exit(1)

    # Baseline: run with no observations
    baseline_output = "baseline_no_obs.nc"
    print("\nRunning baseline (no observations)...")
    if not run_forward_model(test1_cbf, test1_cbr, baseline_output, cardamom_exe):
        print("ERROR: Baseline run failed")
        sys.exit(1)
    print("  ✓ Baseline complete")

    # Test each observation variable individually
    obs_vars = ['LAI', 'ABGB', 'SCF', 'NBE', 'H', 'LE']

    print("\nTesting each observation variable individually:")
    print("-"*70)

    results = {}

    for obs_var in obs_vars:
        print(f"\nTesting {obs_var}...")

        # Create test file with only this observation
        test_cbf = f"TEST_ONLY_{obs_var}.cbf.nc"
        test_output = f"output_only_{obs_var}.nc"

        create_test_with_single_obs(test2_cbf, test_cbf, obs_var)

        # Run forward model
        if not run_forward_model(test_cbf, test1_cbr, test_output, cardamom_exe):
            print(f"  ✗ Forward model run failed")
            results[obs_var] = None
            continue

        # Compare with baseline
        max_diff, max_loc, val1, val2 = compare_fluxes(baseline_output, test_output)

        if max_diff is None:
            print(f"  ✗ Comparison failed")
            results[obs_var] = None
        elif max_diff < 1e-10:
            print(f"  ✓ No difference (max diff: {max_diff:.2e})")
            results[obs_var] = 0.0
        else:
            print(f"  ⚠️  DIFFERENCE DETECTED!")
            print(f"     Max diff: {max_diff:.6f}")
            print(f"     Location: flux[{max_loc[0]}, {max_loc[1]}, {max_loc[2]}]")
            print(f"     Baseline value: {val1:.6f}")
            print(f"     With {obs_var}: {val2:.6f}")
            results[obs_var] = max_diff

    # Summary
    print("\n" + "="*70)
    print("SUMMARY")
    print("="*70)

    culprits = [var for var, diff in results.items() if diff is not None and diff > 1e-10]

    if culprits:
        print(f"\n⚠️  BUG FOUND: The following observation variables affect forward model output:")
        for var in culprits:
            print(f"   - {var}: max difference = {results[var]:.6f}")
        print("\nThis is a BUG - observations should only affect likelihood, not model dynamics!")
    else:
        print("\n✓ No individual observation variable causes the mismatch.")
        print("  The difference may be due to interaction between multiple observations.")

    print("\n" + "="*70)
