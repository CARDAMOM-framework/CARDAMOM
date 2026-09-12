# CARDAMOM Subset Output Feature

## Overview

The subset output feature allows you to specify which fluxes and/or pools to output from `CARDAMOM_RUN_MODEL`, reducing output file size and improving performance for large ensemble runs.

## Quick Start

Add a global attribute to your NetCDF input file (`.cbf.nc`) specifying which variables to output:

### MATLAB
```matlab
% Specify which fluxes to output (comma-delimited string)
ncwriteatt('your_file.cbf.nc', '/', 'FLUXES_SUBSET', 'GPP,rh_co2,ets');

% Specify which pools to output
ncwriteatt('your_file.cbf.nc', '/', 'POOLS_SUBSET', 'C_lab,C_fol,C_som');
```

### Python
```python
import netCDF4 as nc

ncfile = nc.Dataset('your_file.cbf.nc', 'a')
ncfile.FLUXES_SUBSET = "GPP,rh_co2,ets"
ncfile.POOLS_SUBSET = "C_lab,C_fol,C_som"
ncfile.close()
```

### Command Line (ncatted)
```bash
ncatted -a FLUXES_SUBSET,global,c,c,"GPP,rh_co2,ets" your_file.cbf.nc
ncatted -a POOLS_SUBSET,global,c,c,"C_lab,C_fol,C_som" your_file.cbf.nc
```

## Usage Details

### Attribute Format
- **Attribute name**: `FLUXES_SUBSET` or `POOLS_SUBSET` (global attributes)
- **Format**: Comma-delimited string of variable abbreviations
- **Case-sensitive**: Use exact abbreviation names (e.g., `GPP` not `gpp`)
- **Whitespace**: Spaces around commas are automatically trimmed

### Variable Names
Use the **abbreviations** defined in your model's `DALEC_####_NC_INFO.c` file:

**Common Flux Abbreviations:**
- `GPP` - Gross Primary Productivity
- `rh_co2` - Heterotrophic Respiration CO2
- `ets` - Evapotranspiration
- `nbe` - Net Biosphere Exchange
- `resp_auto` - Autotrophic Respiration
- `lab_prod`, `foliar_prod`, `root_prod`, `wood_prod`
- Fire fluxes: `f_total`, `f_lab`, `f_fol`, `f_roo`, etc.

**Common Pool Abbreviations:**
- `C_lab`, `C_fol`, `C_roo`, `C_woo` - Carbon pools
- `C_cwd`, `C_lit`, `C_som` - Decomposition pools
- `H2O_LY1`, `H2O_LY2`, `H2O_LY3` - Water pools
- `D_LAI`, `D_SCF` - Diagnostic pools

### Examples

#### Example 1: Carbon cycle analysis
Output only carbon fluxes and pools:
```matlab
ncwriteatt(file, '/', 'FLUXES_SUBSET', 'GPP,resp_auto,rh_co2,nbe');
ncwriteatt(file, '/', 'POOLS_SUBSET', 'C_lab,C_fol,C_roo,C_woo,C_som');
```

#### Example 2: Water cycle analysis
Output only water-related variables:
```matlab
ncwriteatt(file, '/', 'FLUXES_SUBSET', 'ets,q_ly1,q_ly2');
ncwriteatt(file, '/', 'POOLS_SUBSET', 'H2O_LY1,H2O_LY2,H2O_LY3');
```

#### Example 3: Single variable output
Output only GPP:
```matlab
ncwriteatt(file, '/', 'FLUXES_SUBSET', 'GPP');
```

## Behavior

### When Subset is Specified
- Only the specified fluxes/pools are written to the output file
- Output file size is proportionally reduced
- NetCDF dimensions are adjusted (`Flux = N` instead of total count)
- Attributes map subset indices to original flux/pool names

### When Subset is NOT Specified
- All fluxes and pools are output (default behavior)
- Backward compatible with existing workflows

### Performance Impact
- **File size reduction**: Proportional to subset size (e.g., 3/100 fluxes → ~74% smaller)
- **Execution speed**: No significant change (bulk write operations maintained)
- **Memory usage**: Minimal overhead (temporary buffers during write)

## Troubleshooting

### Variable not found warning
```
Warning: Subset name 'gpp' not found in abbreviations
```
**Solution**: Check abbreviation spelling and case. Use exact names from `DALEC_####_NC_INFO.c`

### Wrong variable appears
If you get a different variable than expected, verify the abbreviation:
```matlab
% Check what abbreviations are available in output file
info = ncinfo('output.cbr.nc');
fluxes_var = info.Variables(strcmp({info.Variables.Name}, 'FLUXES'));
disp(fluxes_var.Attributes);  % Shows FLUX-XXX mappings
```

### Attribute not being read
Ensure attribute is global (not variable-specific):
```matlab
% Correct - global attribute:
ncwriteatt(file, '/', 'FLUXES_SUBSET', 'GPP,nbe');

% Wrong - variable attribute:
ncwriteatt(file, 'time', 'FLUXES_SUBSET', 'GPP,nbe');  % Don't do this
```

## File Size Savings

Example with 4000 samples, 216 timesteps:

| Configuration | Fluxes | Pools | File Size | Savings |
|--------------|--------|-------|-----------|---------|
| Full output | 100 | 30 | 862 MB | - |
| Subset (3,30) | 3 | 30 | 223 MB | 74% |
| Subset (10,5) | 10 | 5 | ~120 MB | 86% |

## Implementation Notes

- Subset specification added in CARDAMOM v2.1.6c (September 2026)
- Compatible with all DALEC models
- Uses global NetCDF attributes (compatible with all NetCDF formats)
- Variable abbreviations must match those in model's NC_INFO file

## See Also

- `DALEC_####_NC_INFO.c` - Variable abbreviation definitions
- `CARDAMOM_RUN_MODEL.c` - Implementation code
- Model-specific documentation for variable definitions
