# Pull Request Information

## Create PR at:
https://github.com/CARDAMOM-framework/CARDAMOM/pull/new/SUBSET_TEST_SEP26

## PR Title:
Add optional FLUXES_SUBSET and POOLS_SUBSET output filtering

## PR Description:

```markdown
## Overview
Implements optional output subsetting for `CARDAMOM_RUN_MODEL` to allow users to specify which fluxes and pools to output, reducing file size and improving efficiency for large ensemble runs.

## Key Features
- Specify output subset via global NetCDF attributes: `FLUXES_SUBSET` and `POOLS_SUBSET`
- Simple comma-delimited format: `"GPP,rh_co2,ets"`
- Easy to add in MATLAB: `ncwriteatt(file, '/', 'FLUXES_SUBSET', 'GPP,rh_co2,ets')`
- Backward compatible: outputs all variables if no subset specified
- No macros, portable C code with standard NetCDF calls

## Performance Results
- **File size reduction**: 74% smaller for 3/100 flux subset (862MB → 223MB)
- **Execution speed**: No performance impact (1-2 seconds maintained)
- **Memory**: Minimal overhead with temporary buffers during write

## Implementation Details
- Added subset fields to `NETCDF_DATA` structure
- Created `ncdf_read_string_array()` for comma-delimited attribute parsing
- Added `build_subset_indices()` to map abbreviation names to indices
- Modified NetCDF output dimensions and data writing to use subset counts
- Added proper memory cleanup

## Files Changed
- `CARDAMOM_NETCDF_DATA_STRUCTURE.c` - Added subset tracking fields
- `NETCDF_AUXILLIARY_FUNCTIONS.c` - Added string parsing function
- `CARDAMOM_READ_NETCDF_DATA.c` - Read subset attributes from input
- `CARDAMOM_RUN_MODEL.c` - Modified output writing for subsets
- `CARDAMOM_SUBSET_OUTPUT_GUIDE.md` - User documentation

## Testing
✅ Compiles successfully without warnings
✅ Tested with 3-flux subset: correct output dimensions and file size
✅ Verified backward compatibility with no subset specified
✅ Performance maintained at normal speed

## Usage Example
```matlab
% MATLAB
ncwriteatt('input.cbf.nc', '/', 'FLUXES_SUBSET', 'GPP,rh_co2,ets');
ncwriteatt('input.cbf.nc', '/', 'POOLS_SUBSET', 'C_lab,C_fol,C_som');
```

See `CARDAMOM_SUBSET_OUTPUT_GUIDE.md` for complete documentation.

## Commits
- 86bcc591: Initial implementation (+164, -17)
- a41f8c12: Compilation fix (+5, -5)
- d35bd106: Performance optimization (+15, -11)
- 1ccb67d5: 2D array support (+60, -16)
- 8562f052: Comma-delimited attributes + documentation (+186, -60)

Total: +430 insertions, -109 deletions across 6 files
```

## Reviewers to Tag:
- Tag relevant CARDAMOM team members
- Request review from code maintainers

## Labels to Add:
- `enhancement`
- `performance`
- `documentation`
