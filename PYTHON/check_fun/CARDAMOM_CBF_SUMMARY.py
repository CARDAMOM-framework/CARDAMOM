#!/usr/bin/env python3
"""
CARDAMOM_CBF_SUMMARY.py

Summary and diagnostic visualization script for CARDAMOM driver/input NetCDF files (.cbf.nc).
"""

import argparse
import math
import os
import sys
import numpy as np
import xarray as xr
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

# Name of the time dimension. CARDAMOM files use either "time_dim" or "time";
# load_file() detects which one this file uses and updates this value.
TIME_DIM = "time_dim"

# Known observation variable names per CARDAMOM specification
OBS_VARS = {
    "GPP", "LAI", "NBE", "ET", "LE", "H", "ABGB",
    "SCF", "SIF", "CH4", "ROFF", "EWT", "SWE", "FIR", "DOM"
}


# Readable versions of the raw unit strings found in CARDAMOM files.
# Add new entries here as you meet them; unknown units are shown unchanged.
UNIT_LABELS = {
    "deg C": "°C", "degC": "°C", "C": "°C", "degrees Celsius": "°C",
    "MJ m**-2 day**-1": "MJ/m²/day", "MJ m-2 day-1": "MJ/m²/day",
    "mm day-1": "mm/day", "g C m-2 day-1": "gC/m²/day", "g C m-2": "gC/m²",
    "m2 m-2 pixel land area": "m²/m²", "g C m-2 month-1": "gC/m²/month",
    "CO2 [ppm]": "ppm",
    "VPD [hPa]": "hPa",
    "gC/m2/day": "gC/m²/day", "gC m-2 day-1": "gC/m²/day",
    "gC/m2": "gC/m²", "m2/m2": "m²/m²", "m2 m-2": "m²/m²",
}


# Attribute names CARDAMOM reads as observation uncertainty
# (see READ_NETCDF_TIMESERIES_OBS_FIELDS in CARDAMOM_LIKELIHOOD_FUNCTION.c).
UNC_ATTRS = ("single_unc", "single_monthly_unc", "single_annual_unc",
             "single_decadal_unc", "single_mean_unc", "structural_unc")

# Single-value constraints (not time series) start with these prefixes.
CONSTRAINT_PREFIXES = ("Mean_", "PEQ_")


def has_uncertainty(ds, var):
    """True if an observation has an uncertainty CARDAMOM can use:
    a '<VAR>unc' variable or any of the UNC_ATTRS attributes."""
    return f"{var}unc" in ds or any(a in ds[var].attrs for a in UNC_ATTRS)


def clean_units(units):
    """Return a readable unit label, e.g. 'MJ m**-2 day**-1' -> 'MJ/m²/day'."""
    return UNIT_LABELS.get(str(units).strip(), str(units))


def shorten_middle(text, max_len=40):
    """Shorten long text by cutting out the middle: 'abcdef...uvwxyz'."""
    if len(text) <= max_len:
        return text
    keep = (max_len - 1) // 2
    return text[:keep] + "…" + text[-keep:]


def wrap_lines(lines, width=45):
    """Wrap each text line to a maximum width, indenting continuation lines."""
    import textwrap
    wrapped = []
    for line in lines:
        wrapped.extend(textwrap.wrap(line, width=width, subsequent_indent="  ") or [""])
    return wrapped


def fmt_number(value):
    """Show whole numbers as integers with thousands separators (1000000.0 -> 1,000,000)."""
    try:
        f = float(value)
    except (TypeError, ValueError):
        return str(value)
    return f"{int(f):,}" if f.is_integer() else f"{f:g}"


def half_timestep(times):
    """Half the typical spacing between timesteps, used to widen gap shading."""
    if len(times) > 1:
        return np.median(np.diff(times)) / 2
    return np.timedelta64(15, "D") if np.issubdtype(times.dtype, np.datetime64) else 0.5


def shade_gaps(ax, times, gaps):
    """Shade each missing-data run in light red, padded by half a timestep each side
    so that even a single missing timestep is visible."""
    pad = half_timestep(times)
    for start_t, end_t in gaps:
        ax.axvspan(start_t - pad, end_t + pad, color="#ffcccc", alpha=0.7, lw=0, zorder=0)


def marker_size(n_points):
    """Smaller dots for long records so they don't merge into a thick band."""
    return 3 if n_points <= 60 else (2 if n_points <= 150 else 1.2)


def set_time_axis(ax, times):
    """Year-based x-axis ticks without repeated labels."""
    if np.issubdtype(times.dtype, np.datetime64):
        locator = mdates.AutoDateLocator(minticks=3, maxticks=7)
        ax.xaxis.set_major_locator(locator)
        ax.xaxis.set_major_formatter(mdates.ConciseDateFormatter(locator))


def load_file(path):
    """Open a NetCDF file with xarray with error handling."""
    if not os.path.exists(path):
        print(f"Error: File not found: {path}", file=sys.stderr)
        sys.exit(1)
    try:
        ds = xr.open_dataset(path)
        # 'time' is stored as an ordinary variable along 'time_dim'.
        # Make it a coordinate so it isn't treated as data and so
        # time-based tools (gap dates, groupby("time.month")) work.
        if "time" in ds.variables:
            ds = ds.set_coords("time")
            # Use whatever dimension the 'time' variable runs along
            global TIME_DIM
            TIME_DIM = ds["time"].dims[0]
        return ds
    except Exception as e:
        print(f"Error opening NetCDF file '{path}': {e}", file=sys.stderr)
        sys.exit(1)


def classify_variables(ds):
    """Split time-varying variables into has_data, all_zeros, constant, and all_missing."""
    groups = {
        "has_data": [],
        "all_zeros": [],
        "constant": [],
        "all_missing": []
    }
    
    for var_name, da in ds.data_vars.items():
        if TIME_DIM not in da.dims or var_name == "DOY":
            continue
        
        # Extract values, treat -9999 as NaN
        vals = da.values.astype(float).flatten()
        vals[vals == -9999] = np.nan
        valid_vals = vals[~np.isnan(vals)]
        
        if valid_vals.size == 0:
            groups["all_missing"].append(var_name)
        elif np.all(valid_vals == 0):
            groups["all_zeros"].append(var_name)
        elif np.all(valid_vals == valid_vals[0]):
            groups["constant"].append(var_name)
        else:
            groups["has_data"].append(var_name)
            
    return groups


def find_gaps(da):
    """Return number of missing timesteps and list of (start_date, end_date) for missing runs."""
    vals = da.values.astype(float).flatten()
    is_missing = np.isnan(vals) | (vals == -9999)
    missing_count = int(np.sum(is_missing))
    
    # Identify time coordinates
    if "time" in da.coords:
        times = da["time"].values
    elif "time" in da.dims:
        times = da[da.dims[0]].values
    else:
        times = np.arange(len(vals))
        
    gaps = []
    in_gap = False
    start_idx = 0
    
    for i, miss in enumerate(is_missing):
        if miss and not in_gap:
            in_gap = True
            start_idx = i
        elif not miss and in_gap:
            in_gap = False
            gaps.append((times[start_idx], times[i - 1]))
    if in_gap:
        gaps.append((times[start_idx], times[-1]))
        
    return missing_count, gaps


def file_overview(ds):
    """Extract metadata: filename, coordinates, model ID, timesteps, and temporal resolution."""
    lines = []
    fname = os.path.basename(ds.encoding.get("source", "Unknown file"))
    lines.append(f"File: {shorten_middle(fname, 40)}")
    
    # Lat/Lon
    lat = float(ds["LAT"].values) if "LAT" in ds else np.nan
    lon = float(ds["LON"].values) if "LON" in ds else np.nan
    lines.append(f"Lat / Lon: {lat:.3f}° / {lon:.3f}°")
    
    # Model ID
    model_id = int(ds["ID"].values) if "ID" in ds else "Unknown"
    lines.append(f"DALEC Model ID: {model_id}")
    
    # Timestep & Temporal resolution
    if "time" in ds:
        times = ds["time"].values
        n_times = len(times)
        lines.append(f"Timesteps: {n_times}")
        start_str = str(np.datetime_as_string(times[0], unit='D')) if np.issubdtype(times.dtype, np.datetime64) else str(times[0])
        end_str = str(np.datetime_as_string(times[-1], unit='D')) if np.issubdtype(times.dtype, np.datetime64) else str(times[-1])
        lines.append(f"Range: {start_str} to {end_str}")
        
        # Check median delta
        if len(times) > 1 and np.issubdtype(times.dtype, np.datetime64):
            deltas = np.diff(times).astype("timedelta64[D]").astype(float)
            median_delta = np.median(deltas)
            res = "Daily" if median_delta <= 5 else "Monthly"
            lines.append(f"Resolution: ~{res} (median step {median_delta:.1f}d)")
    else:
        lines.append(f"Timesteps: {ds.sizes.get('time_dim', 0)}")
        
    return lines


def climate_snapshot(ds):
    """Calculate mean temperature, annual precipitation, snowfall ratio, and CO2 change."""
    lines = []
    
    # Temperature: mean of T2M_MIN and T2M_MAX
    if "T2M_MIN" in ds and "T2M_MAX" in ds:
        tmin = ds["T2M_MIN"].values.astype(float)
        tmax = ds["T2M_MAX"].values.astype(float)
        tmin[tmin == -9999] = np.nan
        tmax[tmax == -9999] = np.nan
        t_mean = np.nanmean((tmin + tmax) / 2.0)
        units = clean_units(ds["T2M_MAX"].attrs.get("units", "°C"))
        lines.append(f"Mean Temp (avg of min & max): {t_mean:.1f} {clean_units(units)}")
    else:
        lines.append("Mean Temp: N/A")
        
    # Precipitation: TOTAL_PREC
    if "TOTAL_PREC" in ds:
        prec = ds["TOTAL_PREC"].values.astype(float)
        prec[prec == -9999] = np.nan
        units = ds["TOTAL_PREC"].attrs.get("units", "")
        p_mean = np.nanmean(prec)
        if "day" in units.lower() or units.strip() == "mm":
            annual_p = p_mean * 365.25
            lines.append(f"Mean Ann. Precip: {annual_p:.1f} mm/yr (from {units})")
        else:
            lines.append(f"Mean Precip: {p_mean:.2f} {units}")
    else:
        lines.append("Mean Ann. Precip: N/A")
        
    # Snowfall fraction
    if "SNOWFALL" in ds and "TOTAL_PREC" in ds:
        snow = ds["SNOWFALL"].values.astype(float)
        prec = ds["TOTAL_PREC"].values.astype(float)
        snow[snow == -9999] = np.nan
        prec[prec == -9999] = np.nan
        tot_snow = np.nansum(snow)
        tot_prec = np.nansum(prec)
        frac = (tot_snow / tot_prec * 100) if tot_prec > 0 else 0.0
        lines.append(f"Snowfall Fraction: {frac:.1f}%")
    else:
        lines.append("Snowfall Fraction: N/A")
        
    # CO2 first -> last
    if "CO2" in ds:
        co2 = ds["CO2"].values.astype(float).flatten()
        co2_valid = co2[(~np.isnan(co2)) & (co2 != -9999)]
        if co2_valid.size > 0:
            units = ds["CO2"].attrs.get("units", "ppm")
            lines.append(f"CO2: {co2_valid[0]:.1f} → {co2_valid[-1]:.1f} {clean_units(units)}")
        else:
            lines.append("CO2: No valid values")
    else:
        lines.append("CO2: Not present")
        
    return lines


def run_settings(ds):
    """Retrieve MCMC and EDC algorithm configuration settings."""
    lines = []
    
    if "MCMCID" in ds:
        mcmc_val = int(ds["MCMCID"].values)
        lines.append(f"MCMC ID: {mcmc_val}")
        attrs = ds["MCMCID"].attrs
        for key in ["nITERATIONS", "nSAMPLES", "seed_number", "fADAPT"]:
            val = attrs.get(key, None)
            lines.append(f"  {key}: {fmt_number(val) if val is not None else 'Not set'}")
    else:
        lines.append("MCMCID: Not found")
        
    if "EDC" in ds:
        edc_val = int(ds["EDC"].values)
        lines.append(f"EDC: {'ON (1)' if edc_val == 1 else 'OFF (0)'}")
    else:
        lines.append("EDC: Not found")
        
    return lines


def _parse_dalec_requirements(req_path, model_id):
    """Helper to parse DALEC_MODEL_FIELD_REQUIREMENTS.txt for required fields."""
    if not os.path.exists(req_path):
        return None
    required_fields = set()
    model_str = str(model_id).strip()
    
    with open(req_path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("//"):
                continue
            parts = line.split(":")
            if len(parts) >= 3:
                field = parts[0].strip()
                req_by = [m.strip() for m in parts[2].split(",")]
                if "ALL" in req_by or model_str in req_by:
                    required_fields.add(field)
    return required_fields


def _all_listed_fields(req_path):
    """Every field named in DALEC_MODEL_FIELD_REQUIREMENTS.txt (required or not).
    Returns an empty set if the file isn't found."""
    fields = set()
    if os.path.exists(req_path):
        with open(req_path) as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith("//") and ":" in line:
                    fields.add(line.split(":")[0].strip())
    return fields


def find_additional_vars(ds, listed_fields):
    """Variables CARDAMOM doesn't use: not in the requirements file, not a known
    observation, not an uncertainty or single-value constraint.
    Returns a list of (name, description) for an 'Additional data' list."""
    known = set(listed_fields) | OBS_VARS | {"time", "DOY", "ID", "LAT", "LON", "EDC", "MCMCID"}
    extra = []
    for name in sorted(ds.data_vars):
        if (name in known or name.endswith("unc")
                or name.startswith(CONSTRAINT_PREFIXES)):
            continue
        desc = ds[name].attrs.get("description", "")
        extra.append((name, desc))
    return extra


def build_warnings(ds, groups, requirements, additional=()):
    """Generate warnings for missing requirements, missing observations, or gaps."""
    warnings = []
    
    # 1. Missing DALEC required fields
    if requirements is None:
        warnings.append("[NOTE] DALEC_MODEL_FIELD_REQUIREMENTS.txt not found. Skipped.")
    else:
        for field in sorted(requirements):
            if field not in ds:
                warnings.append(f"Missing required field: {field}")
                
    # 2. Check observations (0 valid points, missing uncertainty)
    time_vars = [v for v, da in ds.data_vars.items() if TIME_DIM in da.dims and v != "DOY"]
    for var in time_vars:
        if var in OBS_VARS:
            vals = ds[var].values.astype(float).flatten()
            valid_pts = np.sum((~np.isnan(vals)) & (vals != -9999))
            if valid_pts == 0:
                warnings.append(f"Observation '{var}' has 0 valid data points")
            else:
                if not has_uncertainty(ds, var):
                    warnings.append(f"Observation '{var}' has no uncertainty")
                    
    # 3. Classified variable warnings (all zeros, constant, all missing).
    #    Additional (unused) variables are listed separately, not warned about.
    skip = {name for name, _ in additional}
    for v in [v for v in groups["all_missing"] if v not in skip]:
        warnings.append(f"Variable '{v}' is all missing (NaNs)")
    for v in [v for v in groups["all_zeros"] if v not in skip]:
        warnings.append(f"Variable '{v}' is all zeros")
    for v in [v for v in groups["constant"] if v not in skip]:
        if v.endswith("unc"):
            continue  # a constant uncertainty (e.g. NBEunc = 1 everywhere) is normal
        warnings.append(f"Variable '{v}' is constant non-zero")
        
    # 4. Variables with > 10% missing
    n_times = ds.sizes.get(TIME_DIM, 0)
    if n_times > 0:
        for v in [v for v in groups["has_data"] if v not in skip]:
            da = ds[v]
            missing_count, _ = find_gaps(da)
            frac_missing = missing_count / float(n_times)
            if frac_missing > 0.10:
                warnings.append(f"Variable '{v}' has {frac_missing * 100:.1f}% missing values")
                
    if not warnings:
        warnings.append("No file anomalies or missing constraints detected.")
        
    return warnings


def plot_text_panel(ax, title, lines, highlight=False, ncols=1, wrap=45):
    """Draw a titled box of text lines that fills the axis.
    highlight=True draws it in red; ncols splits long lists into columns."""
    ax.set_xticks([]); ax.set_yticks([])
    color = "#b30000" if highlight else "#222222"
    edgecolor = "#b30000" if highlight else "#cccccc"
    ax.set_facecolor("#fff2f2" if highlight else "#f7f7f7")
    for spine in ax.spines.values():
        spine.set_edgecolor(edgecolor)
        spine.set_linewidth(1.2)

    lines = wrap_lines(lines, width=wrap)
    per_col = math.ceil(len(lines) / ncols)
    for c in range(ncols):
        chunk = lines[c * per_col:(c + 1) * per_col]
        ax.text(0.02 + c / ncols, 0.93, "\n".join(chunk),
                transform=ax.transAxes, fontsize=8, va="top",
                fontfamily="monospace", color=color, linespacing=1.4)
    ax.set_title(title, fontsize=10, fontweight="bold", color=color, loc="left", pad=4)


def plot_coverage(ax, ds, variables):
    """Plot horizontal data coverage matrix: green for valid data, light red for missing."""
    if "time" in ds:
        times = ds["time"].values
    else:
        times = np.arange(ds.sizes.get(TIME_DIM, 1))
        
    n_vars = len(variables)
    n_times = len(times)
    
    matrix = np.zeros((n_vars, n_times))
    for i, var in enumerate(variables):
        vals = ds[var].values.astype(float).flatten()
        valid = (~np.isnan(vals)) & (vals != -9999)
        matrix[i, :] = valid.astype(int)
        
    # Colormap: 0 = missing (#ffbaba, light red), 1 = present (#99df99, soft green)
    from matplotlib.colors import ListedColormap
    cmap = ListedColormap(["#f7b0b0", "#8ad690"])
    
    # imshow draws the matrix as one image stretched across the time range
    if np.issubdtype(times.dtype, np.datetime64):
        x0, x1 = mdates.date2num(times[0]), mdates.date2num(times[-1])
    else:
        x0, x1 = 0, n_times - 1
    ax.imshow(matrix, aspect="auto", cmap=cmap, vmin=0, vmax=1,
              interpolation="nearest", extent=[x0, x1, n_vars, 0])
    if np.issubdtype(times.dtype, np.datetime64):
        ax.xaxis_date()
        ax.xaxis.set_major_locator(mdates.YearLocator())
        ax.xaxis.set_major_formatter(mdates.DateFormatter("%Y"))

    ax.set_yticks(np.arange(n_vars) + 0.5)
    ax.set_yticklabels(variables, fontsize=8)
    # thin white lines between rows so each variable reads as its own strip
    for i in range(1, n_vars):
        ax.axhline(i, color="white", lw=0.8)
    ax.set_ylim(n_vars, 0)
    ax.grid(False)
    ax.set_title("Data coverage (green = data, red = missing)", fontsize=10, fontweight="bold", loc="left", pad=4)


def plot_timeseries(ax, ds, var):
    """Plot variable time-series with small dots, a thin line, and light-red gap shading."""
    da = ds[var]
    vals = da.values.astype(float).flatten()
    vals[vals == -9999] = np.nan
    
    if "time" in ds:
        times = ds["time"].values
    else:
        times = np.arange(len(vals))
        
    units = clean_units(da.attrs.get("units", ""))
    ax.plot(times, vals, marker=".", markersize=marker_size(len(vals)), linewidth=0.6, color="#1f77b4")

    # Shade missing runs (padded so single missing steps show)
    _, gaps = find_gaps(da)
    shade_gaps(ax, times, gaps)

    ax.set_title(var, fontsize=8.5, fontweight="bold", pad=2)
    ax.set_ylabel(units, fontsize=7.5)
    ax.tick_params(axis="both", labelsize=7)
    
    set_time_axis(ax, times)


def plot_temperature(ax, ds):
    """Plot T2M_MAX and T2M_MIN on one combined panel with a legend."""
    tmin = ds["T2M_MIN"].values.astype(float).flatten()
    tmax = ds["T2M_MAX"].values.astype(float).flatten()
    tmin[tmin == -9999] = np.nan
    tmax[tmax == -9999] = np.nan
    
    if "time" in ds:
        times = ds["time"].values
    else:
        times = np.arange(len(tmin))
        
    units = clean_units(ds["T2M_MAX"].attrs.get("units", "°C"))
    ms = marker_size(len(tmax))
    ax.plot(times, tmax, marker=".", markersize=ms, linewidth=0.5, color="#d95f02", label="T2M_MAX")
    ax.plot(times, tmin, marker=".", markersize=ms, linewidth=0.5, color="#2b83ba", label="T2M_MIN")
    
    # Shading missing runs
    _, gaps_min = find_gaps(ds["T2M_MIN"])
    shade_gaps(ax, times, gaps_min)

    ax.set_title("Temperature (T2M_MAX & T2M_MIN)", fontsize=8.5, fontweight="bold", pad=2)
    ax.set_ylabel(units, fontsize=7.5)
    # Legend sits above the plot area so it never hides data
    ax.legend(loc="lower right", bbox_to_anchor=(1.0, 1.0), ncol=2, fontsize=6.5, frameon=False)
    ax.tick_params(axis="both", labelsize=7)
    set_time_axis(ax, times)


def plot_seasonal_cycle(ax, ds, variables, title=None):
    """Plot the mean for each calendar month (Jan-Dec) for one variable or a list of
    variables, using groupby('time.month'). Several variables share one panel."""
    if isinstance(variables, str):
        variables = [variables]
    colors = ["#d95f02", "#2b83ba", "#2ca02c", "#7570b3"]
    units = ""
    for i, var in enumerate(variables):
        da = ds[var].where(ds[var] != -9999)
        if "time" not in da.coords or not np.issubdtype(da["time"].dtype, np.datetime64):
            ax.text(0.5, 0.5, "No dates in file", transform=ax.transAxes, ha="center", fontsize=8)
            return
        monthly = da.groupby("time.month").mean(skipna=True)
        ax.plot(monthly["month"].values, monthly.values, marker="o", markersize=3.5,
                linewidth=1.2, color=colors[i % len(colors)], label=var)
        units = clean_units(da.attrs.get("units", ""))

    ax.set_xticks(range(1, 13))
    ax.set_xticklabels(["J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D"], fontsize=7)
    if len(variables) > 1:
        ax.legend(fontsize=6.5)
    ax.set_title(title or f"Typical year: {variables[0]}", fontsize=8.5, fontweight="bold", pad=2)
    ax.set_ylabel(units, fontsize=7.5)
    ax.tick_params(axis="both", labelsize=7)
    ax.grid(True, linestyle=":", alpha=0.5)


def main():
    parser = argparse.ArgumentParser(description="Generate diagnostic summary for CARDAMOM CBF NetCDF file.")
    parser.add_argument("path", help="Path to input .cbf.nc NetCDF file")
    parser.add_argument("--no-show", action="store_true", help="Save plot without opening display window")
    parser.add_argument("--output", default=".", help="Directory to save the generated PNG summary")
    args = parser.parse_args()

    ds = load_file(args.path)
    
    # Classification & Gaps
    groups = classify_variables(ds)
    model_id = int(ds["ID"].values) if "ID" in ds else "Unknown"
    
    # DALEC Requirements
    req_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), "DALEC_MODEL_FIELD_REQUIREMENTS.txt")
    requirements = _parse_dalec_requirements(req_file, model_id)
    additional = find_additional_vars(ds, _all_listed_fields(req_file))
    additional_names = {name for name, _ in additional}
    
    # Build text sections
    txt_overview = file_overview(ds)
    txt_climate = climate_snapshot(ds)
    txt_settings = run_settings(ds)
    txt_warnings = build_warnings(ds, groups, requirements, additional)
    txt_additional = [f"{n}: {d}" if d else n for n, d in additional] or ["None"]
    
    # Print panels to terminal
    print("=" * 60)
    print("FILE OVERVIEW:")
    print("\n".join(txt_overview))
    print("-" * 60)
    print("CLIMATE SNAPSHOT:")
    print("\n".join(txt_climate))
    print("-" * 60)
    print("RUN SETTINGS:")
    print("\n".join(txt_settings))
    print("-" * 60)
    print("DIAGNOSTICS & WARNINGS:")
    print("\n".join(txt_warnings))
    print("-" * 60)
    print("ADDITIONAL DATA (in file, not used by CARDAMOM):")
    print("\n".join(txt_additional))
    print("=" * 60)
    
    # -------------------------------------------------------------
    # LAYOUT: landscape pages
    #   Page 1  "At a glance": text panels, warnings, coverage strip,
    #                          typical-year cycles  -> answers "should I use this file?"
    #   Page 2+ "Time series": 6 wide panels per page (2 columns x 3 rows)
    # Saved as one PNG per page plus a single multi-page PDF.
    # -------------------------------------------------------------
    is_unc = lambda v: v.endswith("unc")  # uncertainty variables aren't plotted separately

    ts_vars = [v for v in groups["has_data"]
               if v not in ("T2M_MIN", "T2M_MAX") and not is_unc(v)
               and v not in additional_names]
    has_temp = "T2M_MIN" in ds and "T2M_MAX" in ds
    cov_vars = sorted(v for v in ds.data_vars
                      if TIME_DIM in ds[v].dims and v != "DOY" and not is_unc(v)
                      and v not in additional_names)

    base_name = os.path.splitext(os.path.basename(args.path))[0]
    if base_name.endswith(".cbf"):
        base_name = os.path.splitext(base_name)[0]

    figures = [build_overview_page(ds, groups, base_name, txt_overview, txt_climate,
                                   txt_settings, txt_warnings, txt_additional, cov_vars)]
    figures += build_timeseries_pages(ds, base_name, ts_vars, has_temp)

    save_pages(figures, base_name, args.output)

    if not args.no_show:
        plt.show()
    plt.close("all")


def build_overview_page(ds, groups, base_name, txt_overview, txt_climate,
                        txt_settings, txt_warnings, txt_additional, cov_vars):
    """Page 1, 'At a glance' (landscape):
         row 0 : overview (2 cols) | climate | run settings | warnings + additional (right column)
         row 1 : coverage strip (4 cols)                     | (warnings continue)
         row 2 : typical-year panels (up to 5 across)
    """
    cov_height = max(3.0, 0.6 + 0.22 * len(cov_vars))   # grows with number of variables
    heights = [1.8, cov_height, 2.3]
    fig = plt.figure(figsize=(18, sum(heights) + 1.6))
    fig.suptitle(f"CARDAMOM input summary: {base_name}  |  page 1: at a glance",
                 fontsize=13, fontweight="bold", x=0.01, ha="left")
    gs = fig.add_gridspec(3, 5, height_ratios=heights, width_ratios=[1, 1, 1, 1, 1.35],
                          hspace=0.45, wspace=0.3, top=0.93)

    # Text panels
    plot_text_panel(fig.add_subplot(gs[0, 0:2]), "File overview", txt_overview, wrap=70)
    plot_text_panel(fig.add_subplot(gs[0, 2]), "Climate snapshot", txt_climate, wrap=29)
    plot_text_panel(fig.add_subplot(gs[0, 3]), "Run settings", txt_settings, wrap=29)

    # Right column: warnings on top, additional (unused) data below
    w_lines = len(wrap_lines(txt_warnings, 42))
    a_lines = len(wrap_lines(txt_additional, 42))
    right = gs[0:2, 4].subgridspec(2, 1, height_ratios=[w_lines + 2, a_lines + 2], hspace=0.35)
    is_alert = any("Missing" in w or "has 0" in w or "all missing" in w for w in txt_warnings)
    plot_text_panel(fig.add_subplot(right[0]), "Warnings", txt_warnings,
                    highlight=is_alert, wrap=42)
    plot_text_panel(fig.add_subplot(right[1]), "Additional data (not used by CARDAMOM)",
                    txt_additional, wrap=42)

    # Coverage strip
    plot_coverage(fig.add_subplot(gs[1, 0:4]), ds, cov_vars)

    # Typical-year panels: temperature, precipitation, then observations with a seasonal cycle
    seasonal = []
    if "T2M_MIN" in ds and "T2M_MAX" in ds:
        seasonal.append((["T2M_MAX", "T2M_MIN"], "Typical year: temperature"))
    if "TOTAL_PREC" in groups["has_data"]:
        seasonal.append(("TOTAL_PREC", None))
    preferred = ["GPP", "LAI", "NBE", "ET", "SIF", "SCF", "LE", "H"]
    obs_with_data = [v for v in groups["has_data"] if v in OBS_VARS]
    obs_with_data.sort(key=lambda v: preferred.index(v) if v in preferred else len(preferred))
    for v in obs_with_data[:5 - len(seasonal)]:
        seasonal.append((v, None))
    for i, (vars_, title) in enumerate(seasonal[:5]):
        plot_seasonal_cycle(fig.add_subplot(gs[2, i]), ds, vars_, title)

    return fig


def build_timeseries_pages(ds, base_name, ts_vars, has_temp, per_page=6):
    """Pages 2+: wide time-series panels, 2 columns x 3 rows per landscape page."""
    panels = (["__TEMPERATURE__"] if has_temp else []) + list(ts_vars)
    n_pages = max(1, math.ceil(len(panels) / per_page))
    figures = []
    for p in range(n_pages):
        chunk = panels[p * per_page:(p + 1) * per_page]
        if not chunk:
            break
        fig = plt.figure(figsize=(18, 10))
        fig.suptitle(f"CARDAMOM input summary: {base_name}  |  page {p + 2}: "
                     f"time series ({p + 1} of {n_pages})",
                     fontsize=13, fontweight="bold", x=0.01, ha="left")
        gs = fig.add_gridspec(3, 2, hspace=0.45, wspace=0.18, top=0.92)
        for i, var in enumerate(chunk):
            ax = fig.add_subplot(gs[i // 2, i % 2])
            if var == "__TEMPERATURE__":
                plot_temperature(ax, ds)
            else:
                plot_timeseries(ax, ds, var)
        figures.append(fig)
    return figures


def save_pages(figures, base_name, out_dir):
    """Save each page as its own PNG, plus one multi-page PDF of all pages."""
    from matplotlib.backends.backend_pdf import PdfPages
    os.makedirs(out_dir, exist_ok=True)
    pdf_path = os.path.join(out_dir, f"{base_name}_summary.pdf")
    with PdfPages(pdf_path) as pdf:
        for i, fig in enumerate(figures, start=1):
            png_path = os.path.join(out_dir, f"{base_name}_summary_p{i}.png")
            fig.savefig(png_path, dpi=150, bbox_inches="tight")
            pdf.savefig(fig, bbox_inches="tight")
            print(f"Saved page {i}: {png_path}")
    print(f"Saved all pages as PDF: {pdf_path}")


if __name__ == "__main__":
    main()