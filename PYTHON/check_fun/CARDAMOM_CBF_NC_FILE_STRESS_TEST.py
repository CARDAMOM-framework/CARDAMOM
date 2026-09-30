import sys
import os
import numpy as np
import netCDF4 as nc

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

    check_nc_file(nc_file_path, parse_requirements(req_file_path))
