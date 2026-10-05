import sys
import os
import numpy as np
import netCDF4 as nc
import subprocess
import time
import shutil

def parse_requirements(filepath):
    """Parses the requirements text file into a dictionary."""
    requirements = {}
    with open(filepath, 'r') as f:
        for line in f:
            line = line.strip()
            # Skip comments and empty lines
            if not line or line.startswith('//'):
                continue

            parts = [p.strip() for p in line.split(':')]
            if len(parts) == 5:
                field, rng_str, req_it, req_finite, responds = parts

                # Parse range [min, max]
                rng_str = rng_str.strip('[]')
                min_s, max_s = [val.strip() for val in rng_str.split(',')]

                def parse_val(v):
                    if v == '-INF': return -np.inf
                    if v == 'INF': return np.inf
                    return float(v)

                requirements[field] = {
                    'min': parse_val(min_s),
                    'max': parse_val(max_s),
                    'req_it': req_it,
                    'req_finite': req_finite,
                    'responds': responds
                }
    return requirements

def is_field_required_by_model(model_id, req_string):
    """Check if a field is required by a specific model ID."""
    if req_string == 'NONE':
        return False
    if req_string == 'ALL':
        return True

    # Handle "ALL -[ID]" format
    if req_string.startswith('ALL -'):
        excluded = req_string.replace('ALL -', '').strip('[]').split(',')
        excluded = [x.strip() for x in excluded]
        return str(model_id) not in excluded

    # Handle "[ID_1],[ID_2],..." format
    required_ids = req_string.strip('[]').split(',')
    required_ids = [x.strip() for x in required_ids]
    return str(model_id) in required_ids

def create_minimal_test_file(input_nc_path, output_nc_path, requirements, model_id):
    """Create a test file with non-required OBSERVATION variables removed entirely and EDC=0."""
    # Read input file
    ds_in = nc.Dataset(input_nc_path, 'r')

    # Create output file
    ds_out = nc.Dataset(output_nc_path, 'w', format='NETCDF4')

    print(f"  Creating minimal test configuration for model ID {model_id}...")

    # List of observation variables (not coordinates or drivers)
    observation_vars = ['LAI', 'ABGB', 'SCF', 'NBE', 'H', 'LE', 'GPP', 'ET', 'DOM', 'CH4',
                       'SWE', 'EWT', 'SIF', 'ROFF', 'FIR', 'PEQ_iniSOM', 'PEQ_CUE', 'YIELD']

    # Determine which observation variables to exclude
    vars_to_exclude = []
    for var_name in observation_vars:
        if var_name in requirements and var_name in ds_in.variables:
            req_string = requirements[var_name]['req_it']
            if not is_field_required_by_model(model_id, req_string):
                vars_to_exclude.append(var_name)

    # Copy dimensions
    for name, dimension in ds_in.dimensions.items():
        ds_out.createDimension(name, len(dimension) if not dimension.isunlimited() else None)

    # Copy variables, excluding observation variables
    for name, variable in ds_in.variables.items():
        if name in vars_to_exclude:
            print(f"    Removed {name} (not required by model {model_id})")
            continue

        # Create variable
        outVar = ds_out.createVariable(name, variable.datatype, variable.dimensions)

        # Copy attributes
        outVar.setncatts({k: variable.getncattr(k) for k in variable.ncattrs()})

        # Copy data
        outVar[:] = variable[:]

    # Set EDC to 0
    if 'EDC' in ds_out.variables:
        ds_out.variables['EDC'][:] = 0.0
        print(f"    Set EDC to 0")

    # Modify MCMCID attributes
    if 'MCMCID' in ds_out.variables:
        ds_out.variables['MCMCID'].nITERATIONS = 1
        ds_out.variables['MCMCID'].nSAMPLES = 1
        ds_out.variables['MCMCID'].nPRINT = 1
        print(f"    Set MCMCID: nITERATIONS=1, nSAMPLES=1, nPRINT=1")

    ds_in.close()
    ds_out.close()
    print(f"  Test file created: {output_nc_path}")

def create_obs_test_file(input_nc_path, output_nc_path, edc_value=0.0):
    """Create a test file with observations and specified EDC value with minimal MCMC settings."""
    # Copy the input file to output
    shutil.copy2(input_nc_path, output_nc_path)

    # Open in write mode
    ds = nc.Dataset(output_nc_path, 'r+')

    print(f"  Creating test configuration with observations...")

    # Set EDC to specified value
    if 'EDC' in ds.variables:
        ds.variables['EDC'][:] = edc_value
        print(f"    Set EDC to {edc_value}")

    # Modify MCMCID attributes
    if 'MCMCID' in ds.variables:
        ds.variables['MCMCID'].nITERATIONS = 1
        ds.variables['MCMCID'].nSAMPLES = 1
        ds.variables['MCMCID'].nPRINT = 1
        print(f"    Set MCMCID: nITERATIONS=1, nSAMPLES=1, nPRINT=1")
        print(f"    Kept all observation data from input file")

    ds.close()
    print(f"  Test file created: {output_nc_path}")

def create_shortened_test_file(input_nc_path, output_nc_path, requirements, model_id, max_timesteps=24):
    """Create a test file with time-varying quantities shortened to max_timesteps and observations removed."""
    ds_in = nc.Dataset(input_nc_path, 'r')

    # Find the time dimension name
    time_dim_name = None
    for dim_name in ds_in.dimensions.keys():
        if 'time' in dim_name.lower():
            time_dim_name = dim_name
            break

    if time_dim_name is None:
        ds_in.close()
        raise ValueError("Could not find time dimension in input file")

    original_length = len(ds_in.dimensions[time_dim_name])
    new_length = min(max_timesteps, original_length)

    print(f"  Creating shortened test file (timesteps: {original_length} → {new_length})...")

    # List of observation variables to exclude
    observation_vars = ['LAI', 'ABGB', 'SCF', 'NBE', 'H', 'LE', 'GPP', 'ET', 'DOM', 'CH4',
                       'SWE', 'EWT', 'SIF', 'ROFF', 'FIR', 'PEQ_iniSOM', 'PEQ_CUE', 'YIELD']

    # Determine which observation variables to exclude
    vars_to_exclude = []
    for var_name in observation_vars:
        if var_name in requirements and var_name in ds_in.variables:
            req_string = requirements[var_name]['req_it']
            if not is_field_required_by_model(model_id, req_string):
                vars_to_exclude.append(var_name)

    # Create new file
    ds_out = nc.Dataset(output_nc_path, 'w', format='NETCDF4')

    # Copy dimensions
    for name, dimension in ds_in.dimensions.items():
        if name == time_dim_name:
            ds_out.createDimension(name, new_length)
        else:
            ds_out.createDimension(name, len(dimension) if not dimension.isunlimited() else None)

    # Copy variables (excluding observations)
    for name, variable in ds_in.variables.items():
        if name in vars_to_exclude:
            print(f"    Removed {name} (not required by model {model_id})")
            continue

        # Create variable
        outVar = ds_out.createVariable(name, variable.datatype, variable.dimensions)

        # Copy attributes
        outVar.setncatts({k: variable.getncattr(k) for k in variable.ncattrs()})

        # Copy data
        if time_dim_name in variable.dimensions:
            # This is a time-varying variable - truncate it
            time_axis = variable.dimensions.index(time_dim_name)
            if time_axis == 0:
                outVar[:] = variable[:new_length]
            else:
                # Handle other dimensions (though most should be time-first)
                outVar[:] = variable[:]
            print(f"    Shortened {name} from {variable.shape} to {outVar.shape}")
        else:
            # Not time-varying - copy as-is
            outVar[:] = variable[:]

    ds_in.close()

    # Set EDC to 1 and minimal MCMC settings
    if 'EDC' in ds_out.variables:
        ds_out.variables['EDC'][:] = 1.0
        print(f"    Set EDC to 1.0")

    if 'MCMCID' in ds_out.variables:
        ds_out.variables['MCMCID'].nITERATIONS = 1
        ds_out.variables['MCMCID'].nSAMPLES = 1
        ds_out.variables['MCMCID'].nPRINT = 1
        print(f"    Set MCMCID: nITERATIONS=1, nSAMPLES=1, nPRINT=1")

    ds_out.close()
    print(f"  Test file created: {output_nc_path}")

def run_cardamom_test(input_cbf, output_cbr, cardamom_exe, timeout=120, check_initiation_only=False):
    """Run CARDAMOM_RUN_MDF.exe with timeout.

    If check_initiation_only=True, terminates after timeout and returns success if no crash occurred.
    """
    try:
        cmd = [cardamom_exe, input_cbf, output_cbr]
        print(f"  Running: {' '.join(cmd)}")

        start_time = time.time()
        process = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        # Wait for process with timeout
        try:
            stdout, stderr = process.communicate(timeout=timeout)
            elapsed = time.time() - start_time

            if process.returncode == 0:
                return True, elapsed, stdout, stderr
            else:
                return False, elapsed, stdout, stderr

        except subprocess.TimeoutExpired:
            # For initiation check, timeout means it's still running (good!)
            if check_initiation_only:
                process.kill()
                stdout, stderr = process.communicate()
                elapsed = time.time() - start_time
                return True, elapsed, stdout, f"Still running after {timeout}s (expected for EDC search)"
            else:
                process.kill()
                stdout, stderr = process.communicate()
                elapsed = time.time() - start_time
                return False, elapsed, stdout, f"TIMEOUT after {timeout}s\n{stderr}"

    except Exception as e:
        return False, 0, "", str(e)

def check_time_varying_lengths(nc_path):
    """Check that all time-varying quantities have the same length."""
    nc_filename = os.path.basename(nc_path)

    try:
        ds = nc.Dataset(nc_path, 'r')
    except Exception as e:
        print(f"🛑 FATAL ERROR: Cannot open {nc_filename}. Details: {e}")
        sys.exit(1)

    # Find the time dimension
    time_dim_name = None
    for dim_name in ds.dimensions.keys():
        if 'time' in dim_name.lower():
            time_dim_name = dim_name
            break

    if time_dim_name is None:
        ds.close()
        print(f"⚠️  WARNING: No time dimension found in {nc_filename}")
        return True  # Not necessarily an error if no time dimension exists

    time_dim_length = len(ds.dimensions[time_dim_name])

    # Collect all time-varying variables and their lengths
    time_varying_vars = {}
    for var_name, var in ds.variables.items():
        if time_dim_name in var.dimensions:
            # Get the length along the time dimension
            time_axis = var.dimensions.index(time_dim_name)
            var_length = var.shape[time_axis]
            time_varying_vars[var_name] = var_length

    ds.close()

    # Check if all time-varying variables have the same length
    if not time_varying_vars:
        print(f"ℹ️  INFO: No time-varying variables found in {nc_filename}")
        return True

    unique_lengths = set(time_varying_vars.values())

    if len(unique_lengths) == 1:
        expected_length = list(unique_lengths)[0]
        print(f"✅ SUCCESS: All time-varying quantities have consistent length ({expected_length} timesteps)")
        return True
    else:
        print(f"❌ ERROR: Time-varying quantities have inconsistent lengths!")
        print(f"   Expected length from time dimension: {time_dim_length}")
        # Group variables by length
        length_groups = {}
        for var_name, var_length in time_varying_vars.items():
            if var_length not in length_groups:
                length_groups[var_length] = []
            length_groups[var_length].append(var_name)

        for length, var_list in sorted(length_groups.items()):
            print(f"   Length {length}: {', '.join(var_list)}")
        return False

def check_nc_file(nc_path, requirements):
    nc_filename = os.path.basename(nc_path)

    try:
        ds = nc.Dataset(nc_path, 'r')
    except Exception as e:
        print(f"🛑 FATAL ERROR: Cannot open {nc_filename}. Details: {e}")
        sys.exit(1)

    nc_variables = ds.variables.keys()

    for field, rules in requirements.items():
        # Check presence
        if field not in nc_variables:
            if rules['req_it'] != 'NONE':
                print(f"🛑 FATAL ERROR: {field} missing from {nc_filename}, CARDAMOM execution will fail")
            continue

        # Extract data and filter out fill values/NaNs for range checking
        data = ds.variables[field][:]
        
        # Handle masked arrays natively (netCDF4 does this for _FillValue)
        if np.ma.is_masked(data):
            valid_data = data.compressed()
        else:
            valid_data = data[~np.isnan(data)]
            # Manual fallback for standard missing values if not masked properly
            valid_data = valid_data[valid_data != -9999.0]

        if len(valid_data) == 0:
            if rules['req_finite'] != 'NONE':
                print(f"❌ ERROR: {field} contains only unphysical/missing values, expect erroneous DALEC outputs")
            continue

        # Check bounds
        min_val, max_val = rules['min'], rules['max']
        out_of_bounds = np.any((valid_data < min_val) | (valid_data > max_val))

        if out_of_bounds:
            if rules['req_it'] != 'NONE':
                print(f"❌ ERROR: {field} contains unphysical values, expect erroneous DALEC outputs")
            elif rules['responds'] != 'NONE':
                print(f"⚠️ WARNING: {field} values are out of range: DALEC model will run but cost function may fail")
            else:
                print(f"🔶 CAUTION: {field} values are out of range: these bear no consequence but you may need to check workflow outside CARDAMOM runtime context")
        else:
            print(f"✅ {field} complies with CARDAMOM DALEC model requirements")

    ds.close()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 CARDAMOM_CBF_NC_FILE_STRESS_TEST.py <your_cbf_nc_file.cbf.nc>")
        sys.exit(1)

    nc_file_path = sys.argv[1]

    # Assume the requirements file is in the same directory as the script
    script_dir = os.path.dirname(os.path.abspath(__file__))
    req_file_path = os.path.join(script_dir, "DALEC_MODEL_FIELD_REQUIREMENTS.txt")

    if not os.path.exists(req_file_path):
        print(f"🛑 FATAL ERROR: Requirements file missing at {req_file_path}")
        sys.exit(1)

    requirements = parse_requirements(req_file_path)

    # Check time-varying lengths consistency
    print("\n" + "="*70)
    print("TIME-VARYING QUANTITIES LENGTH CHECK")
    print("="*70)
    time_check_passed = check_time_varying_lengths(nc_file_path)
    if not time_check_passed:
        print("\n⚠️  WARNING: Inconsistent time-varying lengths may cause CARDAMOM execution issues")

    # Original validation check
    print("\n" + "="*70)
    print("ORIGINAL FILE VALIDATION")
    print("="*70)
    check_nc_file(nc_file_path, requirements)

    # TEST 1: No observations, EDC=0, minimal MCMC settings
    print("\n" + "="*70)
    print("TEST 1: Short run with no obs and EDC=0")
    print("="*70)
    print("Expectation: Successful run and output within 1-2 minutes")
    print()

    # Read model ID and original EDC value from input file
    try:
        ds = nc.Dataset(nc_file_path, 'r')
        model_id = int(ds.variables['ID'][:])
        original_edc = float(ds.variables['EDC'][:]) if 'EDC' in ds.variables else 1.0
        ds.close()
        print(f"Model ID: {model_id}")
        print(f"Original EDC value: {original_edc}")
    except Exception as e:
        print(f"🛑 FATAL ERROR: Cannot read model ID from {nc_file_path}. Details: {e}")
        sys.exit(1)

    # Find CARDAMOM executable
    cardamom_exe = "/Users/abloom/CARDAMOM/C/projects/CARDAMOM_MDF/CARDAMOM_RUN_MDF_pcg32.exe"
    if not os.path.exists(cardamom_exe):
        print(f"🛑 FATAL ERROR: CARDAMOM executable not found at {cardamom_exe}")
        sys.exit(1)

    # Create test files in current working directory
    test_cbf = "TEST1.cbf.nc"
    test_cbr = "TEST1.cbr.nc"

    create_minimal_test_file(nc_file_path, test_cbf, requirements, model_id)

    # Run the test
    print()
    success, elapsed, stdout, stderr = run_cardamom_test(test_cbf, test_cbr, cardamom_exe, timeout=120)

    print()
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

    # Unit verification test: Run model forward and check likelihood
    if success:
        print("\n" + "-"*70)
        print("UNIT VERIFICATION: Forward model run with no observations")
        print("-"*70)

        cardamom_run_model_exe = "/Users/abloom/CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.exe"
        if not os.path.exists(cardamom_run_model_exe):
            print(f"⚠️  Skipping verification: CARDAMOM_RUN_MODEL.exe not found")
        else:
            test1_output_nc = "TEST1.output.nc"

            # Run forward model
            try:
                cmd = [cardamom_run_model_exe, test_cbf, test_cbr, test1_output_nc]
                result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)

                if result.returncode == 0 and os.path.exists(test1_output_nc):
                    print(f"✅ SUCCESS: executed CARDAMOM_RUN_MODEL.exe and obtained output file")

                    # Read and check likelihood from output file
                    try:
                        ds_out = nc.Dataset(test1_output_nc, 'r')
                        # The likelihood is stored in the output file
                        likelihood_found = False
                        likelihood_val = None

                        if 'LIKELIHOODS' in ds_out.variables:
                            # LIKELIHOODS is typically (Sample, Likelihood Index)
                            # Sum all likelihood components to get total likelihood
                            likelihoods = ds_out.variables['LIKELIHOODS'][:]
                            # Get first sample's total likelihood (sum across likelihood indices)
                            if likelihoods.ndim >= 2:
                                likelihood_val = float(np.sum(likelihoods[0, :]))
                            else:
                                likelihood_val = float(np.sum(likelihoods[:]))
                            likelihood_found = True
                        elif 'PROB' in ds_out.variables:
                            # PROB might contain total probability
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
                        else:
                            # If no likelihood in output, calculate it manually
                            # For TEST 1 with no observations, likelihood should be 0
                            print(f"ℹ️  Note: No likelihood variable found in output file")
                            print(f"   Expected likelihood = 0 for no-observation run")
                    except Exception as e:
                        print(f"⚠️  Could not read likelihood from output: {e}")
                else:
                    print(f"⚠️  Forward model run failed or no output file created")
                    if result.stderr:
                        print(f"   Error: {result.stderr[:200]}")
            except subprocess.TimeoutExpired:
                print(f"⚠️  Forward model run timed out after 30 seconds")
            except Exception as e:
                print(f"⚠️  Forward model run error: {e}")

    print("\n" + "="*70)

    # Create TEST2b input file (with observations) for use in TEST2a and TEST2b
    test2b_cbf = "TEST2b.cbf.nc"
    test2b_cbr = "TEST2b.cbr.nc"

    print("\nCreating TEST2b input file (with observations)...")
    create_obs_test_file(nc_file_path, test2b_cbf)
    print(f"  Created: {test2b_cbf}")

    # TEST 2a: Forward model run with TEST2b input + TEST1 parameters
    if success:
        print("\n" + "="*70)
        print("TEST 2a: Forward model with TEST2b input + TEST1 parameters")
        print("="*70)

        cardamom_run_model_exe = "/Users/abloom/CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.exe"
        if not os.path.exists(cardamom_run_model_exe):
            print(f"⚠️  Skipping: CARDAMOM_RUN_MODEL.exe not found")
        else:
            test2a_output = "TEST2a.output.nc"

            # Run forward model: TEST2b.cbf.nc + TEST1.cbr.nc -> TEST2a.output.nc
            try:
                cmd = [cardamom_run_model_exe, test2b_cbf, test_cbr, test2a_output]
                print(f"  Running: {' '.join(cmd)}")
                result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)

                if result.returncode == 0 and os.path.exists(test2a_output):
                    print(f"✅ TEST 2a SUCCESSFUL: Forward model run completed")
                    print(f"   Output file created: {test2a_output}")

                    # Analyze LIKELIHOODS: count finite values per likelihood type
                    try:
                        ds_test2a = nc.Dataset(test2a_output, 'r')

                        if 'LIKELIHOODS' in ds_test2a.variables:
                            likelihoods = ds_test2a.variables['LIKELIHOODS'][:]

                            # Likelihood names from DALEC_ALL_LIKELIHOOD.c (lines 22-52)
                            # Order corresponds to LIKELIHOOD_INDICES struct (indices 0-30)
                            likelihood_names = [
                                "ABGB",             # 0
                                "CH4",              # 1
                                "DOM",              # 2
                                "ET",               # 3
                                "LE",               # 4
                                "H",                # 5
                                "EWT",              # 6
                                "GPP",              # 7
                                "SIF",              # 8
                                "LAI",              # 9
                                "NBE",              # 10
                                "ROFF",             # 11
                                "SCF",              # 12
                                "FIR",              # 13
                                "SWE",              # 14
                                "Mean_ABGB",        # 15
                                "Mean_FIR",         # 16
                                "Mean_GPP",         # 17
                                "Mean_LAI",         # 18
                                "PEQ_Cefficiency",  # 19
                                "PEQ_CUE",          # 20
                                "PEQ_NBEmrg",       # 21
                                "PEQ_iniSnow",      # 22
                                "PEQ_iniSOM",       # 23
                                "PEQ_C3frac",       # 24
                                "PEQ_Vcmax25",      # 25
                                "PEQ_LCMA",         # 26
                                "PEQ_clumping",     # 27
                                "PEQ_r_ch4",        # 28
                                "PEQ_S_fv",         # 29
                                "PEQ_rhch4_rhco2"   # 30
                            ]

                            # Count finite values per likelihood type (column)
                            # likelihoods shape is typically (nSamples, nLikelihoodTypes)
                            if likelihoods.ndim >= 2:
                                n_types = likelihoods.shape[1]
                                print(f"\n   LIKELIHOODS Analysis ({likelihoods.shape[0]} samples, {n_types} types):")

                                finite_counts = []
                                for i in range(n_types):
                                    count = np.sum(np.isfinite(likelihoods[:, i]))
                                    finite_counts.append(count)

                                # Print counts with labels
                                for i, count in enumerate(finite_counts):
                                    label = likelihood_names[i] if i < len(likelihood_names) else f"Type_{i+1}"
                                    status = "⚠️" if count == 0 else " "
                                    print(f"   {status} [{i+1:2d}] {label:30s}: {count:4d} finite values")

                                # Summary of zero-finite types
                                zero_finite = []
                                zero_finite_names = []
                                for i, count in enumerate(finite_counts):
                                    if count == 0:
                                        zero_finite.append(i+1)
                                        name = likelihood_names[i] if i < len(likelihood_names) else f"Type_{i+1}"
                                        zero_finite_names.append(name)

                                if zero_finite:
                                    print(f"\n   ⚠️  WARNING: {len(zero_finite)} likelihood type(s) have 0 finite values:")
                                    for idx, name in zip(zero_finite, zero_finite_names):
                                        print(f"      [{idx}] {name}")
                            else:
                                print(f"\n   LIKELIHOODS has unexpected shape: {likelihoods.shape}")
                        else:
                            print(f"\n   ℹ️  Note: LIKELIHOODS variable not found in output file")

                        ds_test2a.close()
                    except Exception as e:
                        print(f"\n   ⚠️  Could not analyze LIKELIHOODS: {e}")
                else:
                    print(f"❌ TEST 2a FAILED")
                    if result.stderr:
                        print(f"   STDERR: {result.stderr[:200]}")
            except subprocess.TimeoutExpired:
                print(f"❌ TEST 2a FAILED: Timeout after 30 seconds")
            except Exception as e:
                print(f"❌ TEST 2a FAILED: {e}")

    # TEST 2b: MCMC with observations, EDC=0, minimal MCMC settings
    print("\n" + "="*70)
    print("TEST 2b: Short run with OBS and EDC=0")
    print("="*70)
    print("Expectation: Successful run and output within 1-2 minutes")
    print()

    # Run the test
    print()
    success2b, elapsed2b, stdout2b, stderr2b = run_cardamom_test(test2b_cbf, test2b_cbr, cardamom_exe, timeout=120)

    print()
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

    # Ask user if they want to run TEST3 (time-consuming)
    print("\nTEST 3a and 3b are time-consuming (10 seconds each for EDC search initiation).")
    run_test3 = input("Do you want to perform TEST 3a and 3b? (y/n): ").strip().lower()

    if run_test3 != 'y':
        print("Skipping TEST 3a and 3b")
        print("\n" + "="*70)
        sys.exit(0)

    # TEST 3a: No observations, EDC=1, check initiation only (skip if original EDC=0)
    print("\nTEST 3a: EDC search initiation check with no OBS and EDC=1")
    print("="*70)

    if original_edc == 0.0:
        print("ℹ️  Info: Test 3a was omitted, since input file EDC value was set to zero")
        print("\n" + "="*70)
    else:
        print("Expectation: EDC search initiates successfully (10 second check)")
        print()

        # Create test files
        test3_cbf = "TEST3a.cbf.nc"
        test3_cbr = "TEST3a.cbr.nc"

        # Create minimal test file (no observations) but with EDC=1
        create_minimal_test_file(nc_file_path, test3_cbf, requirements, model_id)
        # Now set EDC to 1 (create_minimal_test_file sets it to 0)
        ds = nc.Dataset(test3_cbf, 'r+')
        ds.variables['EDC'][:] = 1.0
        ds.close()
        print(f"    Updated EDC to 1.0 for TEST 3a")

        # Run the test with 10 second timeout, check initiation only
        print()
        success3, elapsed3, stdout3, stderr3 = run_cardamom_test(test3_cbf, test3_cbr, cardamom_exe, timeout=10, check_initiation_only=True)

        print()
        if success3:
            print(f"✅ TEST 3a SUCCESSFUL: EDC search with no OBS successfully initiated (ran for {elapsed3:.1f} seconds without crash)")
        else:
            print(f"❌ TEST 3a FAILED after {elapsed3:.1f} seconds")
            print(f"   Error: Code crashed during EDC search initiation with no OBS")
            if stderr3 and "TIMEOUT" not in stderr3:
                print(f"   Error details:")
                print(f"   STDERR: {stderr3[:500]}")
            if stdout3:
                print(f"   STDOUT: {stdout3[:500]}")

        # Clean up temporary cbrSTART file
        cbr_start_file3 = test3_cbr + "START"
        if os.path.exists(cbr_start_file3):
            os.remove(cbr_start_file3)
            print(f"   Cleaned up temporary file: {cbr_start_file3}")

        # Clean up incomplete cbr file since we terminated early
        if os.path.exists(test3_cbr):
            os.remove(test3_cbr)
            print(f"   Cleaned up incomplete output file: {test3_cbr}")

        print("\n" + "="*70)

    # TEST 3b: Shortened time series (24 timesteps), no observations, EDC=1, check initiation
    print("\nTEST 3b: EDC search with shortened time series and no OBS (24 timesteps)")
    print("="*70)
    print("Expectation: EDC search initiates successfully (10 second check)")
    print()

    # Create shortened test file
    test3b_cbf = "TEST3b.cbf.nc"
    test3b_cbr = "TEST3b.cbr.nc"

    try:
        create_shortened_test_file(nc_file_path, test3b_cbf, requirements, model_id, max_timesteps=24)

        # Run the test with 10 second timeout, check initiation only
        print()
        success3b, elapsed3b, stdout3b, stderr3b = run_cardamom_test(test3b_cbf, test3b_cbr, cardamom_exe, timeout=10, check_initiation_only=True)

        print()
        if success3b:
            print(f"✅ TEST 3b SUCCESSFUL: EDC search with shortened time series and no OBS initiated (ran for {elapsed3b:.1f} seconds without crash)")
        else:
            print(f"❌ TEST 3b FAILED after {elapsed3b:.1f} seconds")
            print(f"   Error: Code crashed during EDC search with shortened time series and no OBS")
            if stderr3b and "TIMEOUT" not in stderr3b:
                print(f"   Error details:")
                print(f"   STDERR: {stderr3b[:500]}")
            if stdout3b:
                print(f"   STDOUT: {stdout3b[:500]}")

        # Clean up temporary cbrSTART file
        cbr_start_file3b = test3b_cbr + "START"
        if os.path.exists(cbr_start_file3b):
            os.remove(cbr_start_file3b)
            print(f"   Cleaned up temporary file: {cbr_start_file3b}")

        # Clean up incomplete cbr file since we terminated early
        if os.path.exists(test3b_cbr):
            os.remove(test3b_cbr)
            print(f"   Cleaned up incomplete output file: {test3b_cbr}")

    except Exception as e:
        print(f"❌ TEST 3b FAILED: Could not create shortened test file")
        print(f"   Error: {e}")

    print("\n" + "="*70)
