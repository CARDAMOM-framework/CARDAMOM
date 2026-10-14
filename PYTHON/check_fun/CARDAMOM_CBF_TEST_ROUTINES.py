"""
Test routines for CARDAMOM CBF NC file stress testing.
Each function runs a specific test and returns success status and warnings.
"""

import os
import numpy as np
import netCDF4 as nc
import subprocess


def run_test1(nc_file_path, test_cbf, test_cbr, cardamom_exe, cardamom_run_model_exe,
              file_prefix=""):
    """
    TEST 1: No observations, EDC=0, minimal MCMC settings
    Returns: (success, has_warnings)
    """
    from CARDAMOM_CBF_NC_FILE_STRESS_TEST import run_cardamom_test

    print("\n" + "="*70)
    print("TEST 1: Short run with no obs and EDC=0")
    print("="*70)
    print("Expectation: Successful run and output within 1-2 minutes (will time out after 120 seconds)")

    # Run the test
    success, elapsed, stdout, stderr = run_cardamom_test(test_cbf, test_cbr, cardamom_exe, timeout=120)
    if success:
        print(f"✅ TEST 1 SUCCESSFUL: short run with no OBS and EDC=0 completed in {elapsed:.1f} seconds")
        if os.path.exists(test_cbr):
            print(f"   Output file created: {test_cbr}")
    else:
        print(f"❌ TEST 1 FAILED after {elapsed:.1f} seconds")
        if "TIMEOUT" in stderr:
            print(f"   Error: Run did not complete within 120 seconds")
        else:
            print(f"   Error details:")
            if stderr:
                print(f"   STDERR: {stderr[:500]}")
            if stdout:
                print(f"   STDOUT: {stdout[:500]}")

    # Clean up temporary cbrSTART file
    cbr_start_file = test_cbr + "START"
    if os.path.exists(cbr_start_file):
        os.remove(cbr_start_file)
        print(f"   Cleaned up temporary file: {cbr_start_file}")

    has_warnings = False

    # Unit verification test: Run model forward and check likelihood
    if success and os.path.exists(cardamom_run_model_exe):
        print("\n" + "-"*70)
        print("UNIT VERIFICATION: Forward model run with no observations")
        print("-"*70)

        test1_output_nc = f"{file_prefix}_TEST1.output.nc" if file_prefix else "TEST1.output.nc"

        # Run forward model
        try:
            cmd = [cardamom_run_model_exe, test_cbf, test_cbr, test1_output_nc]
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)

            if result.returncode == 0 and os.path.exists(test1_output_nc):
                print(f"✅ SUCCESS: executed CARDAMOM_RUN_MODEL.exe and obtained output file")

                # Read and check likelihood from output file
                try:
                    ds_out = nc.Dataset(test1_output_nc, 'r')
                    likelihood_found = False
                    likelihood_val = None

                    if 'LIKELIHOODS' in ds_out.variables:
                        likelihoods = ds_out.variables['LIKELIHOODS'][:]
                        if likelihoods.ndim >= 2:
                            likelihood_val = float(np.sum(likelihoods[0, :]))
                        else:
                            likelihood_val = float(np.sum(likelihoods[:]))
                        likelihood_found = True
                    elif 'PROB' in ds_out.variables:
                        prob = ds_out.variables['PROB'][:]
                        if prob.ndim >= 2:
                            likelihood_val = float(prob[0, 0])
                        else:
                            likelihood_val = float(prob[0])
                        likelihood_found = True
                    elif 'P' in ds_out.variables:
                        likelihood_val = float(ds_out.variables['P'][:])
                        likelihood_found = True

                    ds_out.close()

                    if likelihood_found:
                        if likelihood_val == 0.0:
                            print(f"✅ SUCCESS: likelihood = 0 for no-data run")
                        else:
                            print(f"⚠️  WARNING: likelihood = {likelihood_val} (expected 0)")
                            has_warnings = True
                    else:
                        print(f"ℹ️  Note: No likelihood variable found in output file")
                        print(f"   Expected likelihood = 0 for no-observation run")
                except Exception as e:
                    print(f"⚠️  Could not read likelihood from output: {e}")
                    has_warnings = True
            else:
                print(f"⚠️  Forward model run failed or no output file created")
                if result.stderr:
                    print(f"   Error: {result.stderr[:200]}")
                has_warnings = True
        except subprocess.TimeoutExpired:
            print(f"⚠️  Forward model run timed out after 30 seconds")
            has_warnings = True
        except Exception as e:
            print(f"⚠️  Forward model run error: {e}")
            has_warnings = True

    print("\n" + "="*70)
    return success, has_warnings


def run_test2a(test2b_cbf, test_cbr, cardamom_run_model_exe, file_prefix=""):
    """
    TEST 2a: Forward model run with TEST2b input + TEST1 parameters
    Returns: (success, has_warnings)
    """
    print("\n" + "="*70)
    print("TEST 2a: Forward model with TEST2b input + TEST1 parameters")
    print("="*70)

    if not os.path.exists(cardamom_run_model_exe):
        print(f"⚠️  Skipping: CARDAMOM_RUN_MODEL.exe not found")
        return True, True  # Not a failure, but has a warning

    test2a_output = f"{file_prefix}_TEST2a.output.nc" if file_prefix else "TEST2a.output.nc"
    has_warnings = False

    # Run forward model: TEST2b.cbf.nc + TEST1.cbr.nc -> TEST2a.output.nc
    try:
        cmd = [cardamom_run_model_exe, test2b_cbf, test_cbr, test2a_output]
        print(f"  Running: {' '.join(cmd)}")
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)

        if result.returncode == 0 and os.path.exists(test2a_output):
            print(f"✅ TEST 2a SUCCESSFUL: Forward model run completed")
            print(f"   Output file created: {test2a_output}")

            # Analyze LIKELIHOODS
            try:
                ds_test2a = nc.Dataset(test2a_output, 'r')

                if 'LIKELIHOODS' in ds_test2a.variables:
                    likelihoods = ds_test2a.variables['LIKELIHOODS'][:]

                    # Likelihood names from DALEC_ALL_LIKELIHOOD.c
                    likelihood_names = [
                        "ABGB", "CH4", "DOM", "ET", "LE", "H", "EWT", "GPP", "SIF", "LAI",
                        "NBE", "ROFF", "SCF", "FIR", "SWE", "Mean_ABGB", "Mean_FIR", "Mean_GPP",
                        "Mean_LAI", "PEQ_Cefficiency", "PEQ_CUE", "PEQ_NBEmrg", "PEQ_iniSnow",
                        "PEQ_iniSOM", "PEQ_C3frac", "PEQ_Vcmax25", "PEQ_LCMA", "PEQ_clumping",
                        "PEQ_r_ch4", "PEQ_S_fv", "PEQ_rhch4_rhco2"
                    ]

                    if likelihoods.ndim >= 2:
                        n_types = likelihoods.shape[1]

                        finite_counts = []
                        for i in range(n_types):
                            count = np.sum(np.isfinite(likelihoods[:, i]))
                            finite_counts.append(count)

                        # Build compact output showing variable (count)
                        likelihood_strs = []
                        zero_finite_names = []
                        for i, count in enumerate(finite_counts):
                            label = likelihood_names[i] if i < len(likelihood_names) else f"Type_{i+1}"
                            likelihood_strs.append(f"{label} ({count})")
                            if count == 0:
                                zero_finite_names.append(label)

                        # Print in compact single-line format
                        likelihood_line = ', '.join(likelihood_strs)
                        print(f"\n   Finite likelihood analysis ({likelihoods.shape[0]} samples, {n_types} types): {likelihood_line}")

                        # Warning for zero-finite types
                        if zero_finite_names:
                            print(f"   ⚠️  WARNING: {len(zero_finite_names)} type(s) have 0 finite values: {', '.join(zero_finite_names)}")
                            has_warnings = True
                    else:
                        print(f"\n   LIKELIHOODS has unexpected shape: {likelihoods.shape}")
                else:
                    print(f"\n   ℹ️  Note: LIKELIHOODS variable not found in output file")

                ds_test2a.close()
            except Exception as e:
                print(f"\n   ⚠️  Could not analyze LIKELIHOODS: {e}")
                has_warnings = True

            return True, has_warnings
        else:
            print(f"❌ TEST 2a FAILED")
            if result.stderr:
                print(f"   STDERR: {result.stderr[:200]}")
            return False, False
    except subprocess.TimeoutExpired:
        print(f"❌ TEST 2a FAILED: Timeout after 30 seconds")
        return False, False
    except Exception as e:
        print(f"❌ TEST 2a FAILED: {e}")
        return False, False


def run_test2b(test2b_cbf, test2b_cbr, cardamom_exe):
    """
    TEST 2b: MCMC with observations, EDC=0, minimal MCMC settings
    Returns: (success, has_warnings)
    """
    from CARDAMOM_CBF_NC_FILE_STRESS_TEST import run_cardamom_test

    print("\n" + "="*70)
    print("TEST 2b: Short run with OBS and EDC=0")
    print("="*70)
    print("Expectation: Successful run and output within 1-2 minutes (will time out after 120 seconds)")

    # Run the test
    success2b, elapsed2b, stdout2b, stderr2b = run_cardamom_test(test2b_cbf, test2b_cbr, cardamom_exe, timeout=120)
    if success2b:
        print(f"✅ TEST 2b SUCCESSFUL: short run with OBS and EDC=0 completed in {elapsed2b:.1f} seconds")
        if os.path.exists(test2b_cbr):
            print(f"   Output file created: {test2b_cbr}")
    else:
        print(f"❌ TEST 2b FAILED after {elapsed2b:.1f} seconds")
        if "TIMEOUT" in stderr2b:
            print(f"   Error: Run did not complete within 120 seconds")
        else:
            print(f"   Error details:")
            if stderr2b:
                print(f"   STDERR: {stderr2b[:500]}")
            if stdout2b:
                print(f"   STDOUT: {stdout2b[:500]}")

    # Clean up temporary cbrSTART file
    cbr_start_file2b = test2b_cbr + "START"
    if os.path.exists(cbr_start_file2b):
        os.remove(cbr_start_file2b)
        print(f"   Cleaned up temporary file: {cbr_start_file2b}")

    print("\n" + "="*70)
    return success2b, False
