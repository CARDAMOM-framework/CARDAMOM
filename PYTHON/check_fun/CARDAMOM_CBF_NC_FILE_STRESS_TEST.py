import sys
import os
import numpy as np
import netCDF4 as nc
import subprocess
import time
import shutil
import glob
from CARDAMOM_CBF_TEST_ROUTINES import run_test1, run_test2a, run_test2b

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

def get_vars_to_exclude(ds_in, requirements, model_id):
    """List the variables to strip from a test file for a given model ID.

    The candidate set is read straight from DALEC_MODEL_FIELD_REQUIREMENTS.txt:
    any field that file lists as not required by this model ID (observations,
    optional constraints, optional metadata) is dropped. Fields absent from the
    requirements file are non-consequential and are left in place.
    """
    vars_to_exclude = []
    for var_name, rules in requirements.items():
        if var_name not in ds_in.variables:
            continue
        if not is_field_required_by_model(model_id, rules['req_it']):
            vars_to_exclude.append(var_name)
    return vars_to_exclude

def create_minimal_test_file(input_nc_path, output_nc_path, requirements, model_id):
    """Create a test file with non-required OBSERVATION variables removed entirely and EDC=0."""
    # Read input file
    ds_in = nc.Dataset(input_nc_path, 'r')

    # Create output file
    ds_out = nc.Dataset(output_nc_path, 'w', format='NETCDF4')

    print(f"  Creating minimal test configuration for model ID {model_id}...")

    # Determine which variables to exclude
    vars_to_exclude = get_vars_to_exclude(ds_in, requirements, model_id)

    # Copy dimensions
    for name, dimension in ds_in.dimensions.items():
        ds_out.createDimension(name, len(dimension) if not dimension.isunlimited() else None)

    # Track removed variables for compact reporting
    removed_vars = []

    # Copy variables, excluding observation variables
    for name, variable in ds_in.variables.items():
        if name in vars_to_exclude:
            removed_vars.append(name)
            continue

        # Create variable
        outVar = ds_out.createVariable(name, variable.datatype, variable.dimensions)

        # Copy attributes
        outVar.setncatts({k: variable.getncattr(k) for k in variable.ncattrs()})

        # Copy data
        outVar[:] = variable[:]

    # Print removed variables in compact format
    if removed_vars:
        vars_str = ', '.join(removed_vars)
        print(f"    Removed {len(removed_vars)} variables not required by model {model_id}:")
        print(f"    {vars_str}")

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

    # Determine which variables to exclude
    vars_to_exclude = get_vars_to_exclude(ds_in, requirements, model_id)

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

    # TEST 3b is an EDC-search initiation check rather than a short MDF run, so it
    # uses production-scale MCMC settings instead of the minimal ones used elsewhere.
    if 'MCMCID' in ds_out.variables:
        ds_out.variables['MCMCID'].nITERATIONS = 100000
        ds_out.variables['MCMCID'].nSAMPLES = 20
        ds_out.variables['MCMCID'].nPRINT = 1000
        print(f"    Set MCMCID: nITERATIONS=100000, nSAMPLES=20, nPRINT=1000")

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

def run_stress_tests_on_file(nc_file_path, requirements, cardamom_exe, cardamom_run_model_exe,
                             model_id, original_edc, file_prefix="", skip_test3=False):
    """
    Orchestrator function to run stress tests (TEST 1, TEST 2a, TEST 2b) on a single file.

    Parameters:
    -----------
    nc_file_path : str
        Path to the input .cbf.nc file to test
    requirements : dict
        Parsed requirements from DALEC_MODEL_FIELD_REQUIREMENTS.txt
    cardamom_exe : str
        Path to CARDAMOM_MDF.exe
    cardamom_run_model_exe : str
        Path to CARDAMOM_RUN_MODEL.exe
    model_id : int
        Model ID from the input file
    original_edc : float
        Original EDC value from the input file
    file_prefix : str, optional
        Prefix for output files (default: "")
    skip_test3 : bool, optional
        Whether to skip TEST 3 (time-consuming EDC search tests) (default: False)

    Returns:
    --------
    dict
        {'passed': bool, 'has_warnings': bool}
    """
    overall_success = True
    has_warnings = False

    # Generate test file names with optional prefix
    if file_prefix:
        test1_cbf = f"{file_prefix}_TEST1.cbf.nc"
        test1_cbr = f"{file_prefix}_TEST1.cbr.nc"
        test2b_cbf = f"{file_prefix}_TEST2b.cbf.nc"
        test2b_cbr = f"{file_prefix}_TEST2b.cbr.nc"
    else:
        test1_cbf = "TEST1.cbf.nc"
        test1_cbr = "TEST1.cbr.nc"
        test2b_cbf = "TEST2b.cbf.nc"
        test2b_cbr = "TEST2b.cbr.nc"

    # Create TEST1 input file (no observations, EDC=0, minimal MCMC)
    print("Creating TEST1 input file (no observations, EDC=0)...")
    create_minimal_test_file(nc_file_path, test1_cbf, requirements, model_id)
    print(f"  Created: {test1_cbf}")

    # Run TEST 1
    test1_success, test1_warnings = run_test1(
        nc_file_path, test1_cbf, test1_cbr, cardamom_exe, cardamom_run_model_exe, file_prefix
    )
    overall_success = overall_success and test1_success
    has_warnings = has_warnings or test1_warnings

    # Create TEST2b input file (with observations, EDC=0, minimal MCMC)
    print("Creating TEST2b input file (with observations, EDC=0)...")
    create_obs_test_file(nc_file_path, test2b_cbf, edc_value=0.0)
    print(f"  Created: {test2b_cbf}")

    # Run TEST 2a (forward model with TEST2b input + TEST1 parameters)
    if test1_success:
        test2a_success, test2a_warnings = run_test2a(
            test2b_cbf, test1_cbr, cardamom_run_model_exe, file_prefix
        )
        overall_success = overall_success and test2a_success
        has_warnings = has_warnings or test2a_warnings

    # Run TEST 2b (MCMC with observations)
    test2b_success, test2b_warnings = run_test2b(test2b_cbf, test2b_cbr, cardamom_exe)
    overall_success = overall_success and test2b_success
    has_warnings = has_warnings or test2b_warnings

    # TEST 3 prompting (only in single-file mode)
    if not skip_test3:
        print("TEST 3a and 3b are time-consuming (10 seconds each for EDC search initiation).")
        run_test3 = input("Do you want to perform TEST 3a and 3b? (y/n): ").strip().lower()

        if run_test3 == 'y':
            print("ℹ️  Note: TEST 3 implementation coming soon")
            # TODO: Implement TEST 3a and 3b
            # These tests involve EDC search initiation checks
        else:
            print("Skipping TEST 3a and 3b")

    return {'passed': overall_success, 'has_warnings': has_warnings}


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

    # Flag any field in the file that the requirements list does not know about.
    # These are not range-checked, and are not stripped out of the no-observation
    # test configurations, so an unlisted observation will silently constrain
    # TEST 1 / TEST 3 runs.
    for field in nc_variables:
        if field not in requirements:
            print(f"⚠️ WARNING: {field} is not listed in DALEC_MODEL_FIELD_REQUIREMENTS.txt, it will be treated as a non-consequential field")

    ds.close()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 CARDAMOM_CBF_NC_FILE_STRESS_TEST.py <your_cbf_nc_file.cbf.nc>")
        print("       python3 CARDAMOM_CBF_NC_FILE_STRESS_TEST.py <pattern*.cbf.nc>")
        print("       python3 CARDAMOM_CBF_NC_FILE_STRESS_TEST.py file1.cbf.nc file2.cbf.nc ...")
        sys.exit(1)

    # Collect all file paths from command line arguments
    # The shell may expand globs before passing to Python, so we need to handle both:
    # 1. Multiple files passed directly (shell expanded glob)
    # 2. A single glob pattern that needs expansion
    nc_file_paths = []

    for arg in sys.argv[1:]:
        # Try to expand as glob pattern
        expanded = glob.glob(arg)
        if expanded:
            nc_file_paths.extend(expanded)
        else:
            # Not a glob pattern, treat as literal path
            nc_file_paths.append(arg)

    # Filter out test-generated files (TEST1, TEST2a, TEST2b, TEST3a, TEST3b, file*_TEST*)
    test_file_patterns = ['TEST1', 'TEST2a', 'TEST2b', 'TEST3a', 'TEST3b', 'file1_', 'file2_', 'file3_', 'file4_', 'file5_', 'file6_', 'file7_', 'file8_', 'file9_']
    nc_file_paths = [
        path for path in nc_file_paths
        if not any(pattern in os.path.basename(path) for pattern in test_file_patterns)
    ]

    if not nc_file_paths:
        print("No valid input files found (test-generated files are excluded)")
        sys.exit(1)

    # Remove duplicates while preserving order
    seen = set()
    nc_file_paths = [x for x in nc_file_paths if not (x in seen or seen.add(x))]

    multi_file_mode = len(nc_file_paths) > 1

    # Assume the requirements file is in the same directory as the script
    script_dir = os.path.dirname(os.path.abspath(__file__))
    req_file_path = os.path.join(script_dir, "DALEC_MODEL_FIELD_REQUIREMENTS.txt")

    # Locate the executables relative to the repository root (PYTHON/check_fun/ -> repo root),
    # so the stress test works for any checkout rather than one hardcoded home directory.
    repo_root = os.path.abspath(os.path.join(script_dir, "..", ".."))
    cardamom_exe = os.path.join(repo_root, "C", "projects", "CARDAMOM_MDF", "CARDAMOM_MDF.exe")
    cardamom_run_model_exe = os.path.join(repo_root, "C", "projects", "CARDAMOM_GENERAL", "CARDAMOM_RUN_MODEL.exe")

    if not os.path.exists(req_file_path):
        print(f"🛑 FATAL ERROR: Requirements file missing at {req_file_path}")
        sys.exit(1)

    if not os.path.exists(cardamom_exe):
        print(f"🛑 FATAL ERROR: CARDAMOM executable not found at {cardamom_exe}")
        sys.exit(1)

    requirements = parse_requirements(req_file_path)

    # Track results for multi-file summary
    file_results = {}  # {filename: result_dict from run_stress_tests_on_file}

    # Determine skip_test3 based on mode
    # In multi-file mode, skip TEST 3 automatically; in single-file mode, prompt user
    if multi_file_mode:
        skip_test3 = True
        print("\n" + "="*70)
        print(f"MULTI-FILE MODE: Processing {len(nc_file_paths)} files")
        print("TEST 3a and 3b will be skipped automatically in multi-file mode")
        print("="*70)
    else:
        skip_test3 = False

    # Loop through each file
    for file_idx, nc_file_path in enumerate(nc_file_paths):
        nc_filename = os.path.basename(nc_file_path)

        # Generate unique prefix for this file's test files
        file_prefix = f"file{file_idx + 1}_" if multi_file_mode else ""

        # Always print header showing which file we're testing
        print("\n" + "="*70)
        if multi_file_mode:
            print(f"Testing [{nc_filename}] (File {file_idx + 1}/{len(nc_file_paths)})")
        else:
            print(f"Testing [{nc_filename}]")
        print("="*70)

        # Check time-varying lengths consistency
        print("="*70)
        print("TIME-VARYING QUANTITIES LENGTH CHECK")
        print("="*70)
        time_check_passed = check_time_varying_lengths(nc_file_path)
        if not time_check_passed:
            print("⚠️  WARNING: Inconsistent time-varying lengths may cause CARDAMOM execution issues")

        # Original validation check
        print("="*70)
        print("ORIGINAL FILE VALIDATION")
        print("="*70)
        check_nc_file(nc_file_path, requirements)

        # Read model ID and original EDC from the file
        try:
            ds = nc.Dataset(nc_file_path, 'r')
            model_id = int(ds.variables['ID'][:])
            original_edc = float(ds.variables['EDC'][:]) if 'EDC' in ds.variables else 1.0

            # Check and handle nSAMPLES_EDC_SEARCH
            nsamples_edc_search_exists = False
            nsamples_edc_search = None
            if 'MCMCID' in ds.variables and hasattr(ds.variables['MCMCID'], 'nSAMPLES_EDC_SEARCH'):
                nsamples_edc_search = ds.variables['MCMCID'].nSAMPLES_EDC_SEARCH
                nsamples_edc_search_exists = True

            ds.close()

            print(f"Model ID: {model_id}")
            print(f"Original EDC value: {original_edc}")

            # Handle nSAMPLES_EDC_SEARCH warnings and caps
            has_edc_warning = False
            if not nsamples_edc_search_exists:
                print(f"ℹ️  Info: nSAMPLES_EDC_SEARCH not explicitly set, setting to nSAMPLES_EDC_SEARCH = 1000 for the following tests.")
                print(f"   You may want to explicitly set nSAMPLES_EDC_SEARCH if EDC search is unnecessarily long")
                nsamples_edc_search = 1000
                has_edc_warning = True

                # Set the default value in the input file
                ds = nc.Dataset(nc_file_path, 'r+')
                if 'MCMCID' in ds.variables:
                    ds.variables['MCMCID'].nSAMPLES_EDC_SEARCH = nsamples_edc_search
                ds.close()
                print(f"   Set nSAMPLES_EDC_SEARCH = {nsamples_edc_search} for all subsequent tests")

            elif nsamples_edc_search > 1000:
                print(f"⚠️  WARNING: nSAMPLES_EDC_SEARCH > 1000, setting nSAMPLES_EDC_SEARCH = 1000 for testing purposes,")
                print(f"   consider setting nSAMPLES_EDC_SEARCH = 1000 in your cbf.nc file unless your algorithm setup requires otherwise")
                nsamples_edc_search = 1000
                has_edc_warning = True

                # Update the input file with capped value
                ds = nc.Dataset(nc_file_path, 'r+')
                ds.variables['MCMCID'].nSAMPLES_EDC_SEARCH = nsamples_edc_search
                ds.close()
                print(f"   Set nSAMPLES_EDC_SEARCH = {nsamples_edc_search} for all subsequent tests")

        except Exception as e:
            print(f"🛑 ERROR reading metadata from {nc_filename}: {e}")
            file_results[nc_filename] = {
                'passed': False,
                'has_warnings': False,
                'error': str(e)
            }
            # Continue to next file in multi-file mode
            if multi_file_mode:
                continue
            else:
                sys.exit(1)

        # Run stress tests on this file
        try:
            result = run_stress_tests_on_file(
                nc_file_path=nc_file_path,
                requirements=requirements,
                cardamom_exe=cardamom_exe,
                cardamom_run_model_exe=cardamom_run_model_exe,
                model_id=model_id,
                original_edc=original_edc,
                skip_test3=skip_test3,
                file_prefix=file_prefix
            )
            # Merge EDC warning into result
            if has_edc_warning:
                result['has_warnings'] = True
            file_results[nc_filename] = result
        except Exception as e:
            print(f"🛑 ERROR processing {nc_filename}: {e}")
            file_results[nc_filename] = {
                'passed': False,
                'has_warnings': False,
                'error': str(e)
            }
            # Continue to next file in multi-file mode
            if multi_file_mode:
                continue
            else:
                sys.exit(1)

    # Print multi-file summary if needed
    if multi_file_mode:
        from CARDAMOM_CBF_SUMMARY import print_multi_file_summary
        print("\n" + "="*70)
        print("MULTI-FILE SUMMARY")
        print("="*70)
        print_multi_file_summary(file_results)

        # Clean up test files in multi-file mode
        print("\nCleaning up test files...")
        test_patterns = ['TEST1.*', 'TEST2a.*', 'TEST2b.*', 'TEST3a.*', 'TEST3b.*', 'file*_TEST*']
        files_removed = 0
        for pattern in test_patterns:
            for test_file in glob.glob(pattern):
                try:
                    os.remove(test_file)
                    files_removed += 1
                except Exception as e:
                    print(f"  Warning: Could not remove {test_file}: {e}")
        print(f"  Removed {files_removed} test file(s)")
