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
    lines.append(f"File: {fname}")
    
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
        units = ds["T2M_MAX"].attrs.get("units", "°C")
        lines.append(f"Mean Temp: {t_mean:.2f} {units}")
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
            lines.append(f"CO2: {co2_valid[0]:.1f} → {co2_valid[-1]:.1f} {units}")
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
            lines.append(f"  {key}: {attrs.get(key, 'Not Set')}")
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


def build_warnings(ds, groups, requirements):
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
                has_unc = (f"{var}unc" in ds) or ("single_unc" in ds[var].attrs)
                if not has_unc:
                    warnings.append(f"Observation '{var}' has no uncertainty")
                    
    # 3. Classified variable warnings (all zeros, constant, all missing)
    for v in groups["all_missing"]:
        warnings.append(f"Variable '{v}' is all missing (NaNs)")
    for v in groups["all_zeros"]:
        warnings.append(f"Variable '{v}' is all zeros")
    for v in groups["constant"]:
        warnings.append(f"Variable '{v}' is constant non-zero")
        
    # 4. Variables with > 10% missing
    n_times = ds.sizes.get(TIME_DIM, 0)
    if n_times > 0:
        for v in groups["has_data"]:
            da = ds[v]
            missing_count, _ = find_gaps(da)
            frac_missing = missing_count / float(n_times)
            if frac_missing > 0.10:
                warnings.append(f"Variable '{v}' has {frac_missing * 100:.1f}% missing values")
                
    if not warnings:
        warnings.append("No file anomalies or missing constraints detected.")
        
    return warnings


def plot_text_panel(ax, title, lines, highlight=False):
    """Draw a titled panel of text with hidden axes; draws in red if highlight is True."""
    ax.axis("off")
    color = "#b30000" if highlight else "#222222"
    edgecolor = "#b30000" if highlight else "#cccccc"
    facecolor = "#fff2f2" if highlight else "#f9f9f9"
    
    # Outer box
    bbox = dict(boxstyle="square,pad=0.6", facecolor=facecolor, edgecolor=edgecolor, lw=1.2)
    full_text = "\n".join(lines)
    ax.text(
        0.05, 0.90, full_text,
        transform=ax.transAxes,
        fontsize=8.5,
        verticalalignment="top",
        fontfamily="monospace",
        color=color,
        bbox=bbox
    )
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
    ax.set_yticklabels(variables, fontsize=7.5)
    ax.set_ylim(n_vars, 0)
    ax.grid(False)
    ax.set_title("Data Coverage Strip (Green = Available, Red = Missing)", fontsize=9, fontweight="bold", pad=4)


def plot_timeseries(ax, ds, var):
    """Plot variable time-series with small dots, a thin line, and light-red gap shading."""
    da = ds[var]
    vals = da.values.astype(float).flatten()
    vals[vals == -9999] = np.nan
    
    if "time" in ds:
        times = ds["time"].values
    else:
        times = np.arange(len(vals))
        
    units = da.attrs.get("units", "")
    ax.plot(times, vals, marker=".", markersize=2.5, linewidth=0.6, color="#1f77b4")
    
    # Shade missing runs
    _, gaps = find_gaps(da)
    for start_t, end_t in gaps:
        ax.axvspan(start_t, end_t, color="#ffcccc", alpha=0.6, lw=0)
        
    ax.set_title(var, fontsize=8.5, fontweight="bold", pad=2)
    ax.set_ylabel(units, fontsize=7.5)
    ax.tick_params(axis="both", labelsize=7)
    
    if np.issubdtype(times.dtype, np.datetime64):
        # AutoDateLocator picks a sensible tick spacing; ConciseDateFormatter avoids repeated labels
        locator = mdates.AutoDateLocator(minticks=3, maxticks=7)
        ax.xaxis.set_major_locator(locator)
        ax.xaxis.set_major_formatter(mdates.ConciseDateFormatter(locator))


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
        
    units = ds["T2M_MAX"].attrs.get("units", "°C")
    ax.plot(times, tmax, marker=".", markersize=2, linewidth=0.5, color="#d95f02", label="T2M_MAX")
    ax.plot(times, tmin, marker=".", markersize=2, linewidth=0.5, color="#2b83ba", label="T2M_MIN")
    
    # Shading missing runs
    _, gaps_min = find_gaps(ds["T2M_MIN"])
    for start_t, end_t in gaps_min:
        ax.axvspan(start_t, end_t, color="#ffcccc", alpha=0.5, lw=0)
        
    ax.set_title("Temperature (T2M_MAX & T2M_MIN)", fontsize=8.5, fontweight="bold", pad=2)
    ax.set_ylabel(units, fontsize=7.5)
    ax.legend(loc="upper right", fontsize=6.5)
    ax.tick_params(axis="both", labelsize=7)
    
    if np.issubdtype(times.dtype, np.datetime64):
        # AutoDateLocator picks a sensible tick spacing; ConciseDateFormatter avoids repeated labels
        locator = mdates.AutoDateLocator(minticks=3, maxticks=7)
        ax.xaxis.set_major_locator(locator)
        ax.xaxis.set_major_formatter(mdates.ConciseDateFormatter(locator))


def plot_seasonal_cycle(ax, ds, var):
    """Plot calendar month seasonal cycle (Jan-Dec) using groupby('time.month')."""
    da = ds[var]
    da_clean = da.where(da != -9999)
    
    if "time" in da_clean.coords and np.issubdtype(da_clean["time"].dtype, np.datetime64):
        monthly = da_clean.groupby("time.month").mean(skipna=True)
        months = monthly["month"].values
        means = monthly.values
        ax.plot(months, means, marker="o", markersize=3.5, linewidth=1.2, color="#2ca02c")
        ax.set_xticks(range(1, 13))
        ax.set_xticklabels(["J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D"], fontsize=7)
    else:
        ax.text(0.5, 0.5, "No Datetime Index", transform=ax.transAxes, ha="center", fontsize=8)
        
    ax.set_title(f"Seasonal: {var}", fontsize=8.5, fontweight="bold", pad=2)
    ax.set_ylabel(da.attrs.get("units", ""), fontsize=7.5)
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
    
    # Build text sections
    txt_overview = file_overview(ds)
    txt_climate = climate_snapshot(ds)
    txt_settings = run_settings(ds)
    txt_warnings = build_warnings(ds, groups, requirements)
    
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
    print("=" * 60)
    
    # -------------------------------------------------------------
    # Figure Layout Setup
    # -------------------------------------------------------------
    # Grid breakdown:
    # Row 0: 4 text panels
    # Row 1: 1 coverage panel across all columns
    # Rows 2 to 2 + ts_rows - 1: Time series panels
    # Last row: 4 seasonal panels
    
    ts_vars = [v for v in groups["has_data"] if v not in ("T2M_MIN", "T2M_MAX")]
    total_ts = 1 + len(ts_vars)  # 1 slot for combined temperature
    cols = 4
    ts_rows = math.ceil(total_ts / cols)
    total_rows = 2 + ts_rows + 1
    
    # Calculate adaptive figure height
    fig_height = max(11.0, 2.5 + 2.0 + (ts_rows * 1.8) + 2.2)
    fig = plt.figure(figsize=(14, fig_height))
    
    # Height ratios: text row, coverage strip, time series rows, seasonal row
    height_ratios = [1.8, 1.8] + [1.5] * ts_rows + [1.6]
    gs = fig.add_gridspec(total_rows, cols, height_ratios=height_ratios, hspace=0.45, wspace=0.3)
    
    # 1. Text Panels
    ax_txt0 = fig.add_subplot(gs[0, 0])
    plot_text_panel(ax_txt0, "File Overview", txt_overview)
    
    ax_txt1 = fig.add_subplot(gs[0, 1])
    plot_text_panel(ax_txt1, "Climate Snapshot", txt_climate)
    
    ax_txt2 = fig.add_subplot(gs[0, 2])
    plot_text_panel(ax_txt2, "Run Settings", txt_settings)
    
    ax_txt3 = fig.add_subplot(gs[0, 3])
    is_alert = any("Missing" in w or "has 0" in w or "all missing" in w for w in txt_warnings)
    plot_text_panel(ax_txt3, "Warnings & Anomalies", txt_warnings, highlight=is_alert)
    
    # 2. Coverage Strip
    ax_cov = fig.add_subplot(gs[1, :])
    all_time_vars = sorted([v for v in ds.data_vars if TIME_DIM in ds[v].dims and v != "DOY"])
    plot_coverage(ax_cov, ds, all_time_vars)
    
    # 3. Time Series Panels
    curr_idx = 0
    # Combined temperature panel
    row_idx = 2 + (curr_idx // cols)
    col_idx = curr_idx % cols
    ax_temp = fig.add_subplot(gs[row_idx, col_idx])
    plot_temperature(ax_temp, ds)
    curr_idx += 1
    
    # Remaining time series
    for var in ts_vars:
        row_idx = 2 + (curr_idx // cols)
        col_idx = curr_idx % cols
        ax_ts = fig.add_subplot(gs[row_idx, col_idx])
        plot_timeseries(ax_ts, ds, var)
        curr_idx += 1
        
    # Blank out any empty slots in time-series grid
    while curr_idx < ts_rows * cols:
        row_idx = 2 + (curr_idx // cols)
        col_idx = curr_idx % cols
        ax_blank = fig.add_subplot(gs[row_idx, col_idx])
        ax_blank.axis("off")
        curr_idx += 1
        
    # 4. Seasonal Cycle Panels: Temp, Precip, first 2 valid observations
    seasonal_row = total_rows - 1
    
    # Temperature Seasonal
    ax_s0 = fig.add_subplot(gs[seasonal_row, 0])
    temp_var = "T2M_MAX" if "T2M_MAX" in ds else ("T2M_MIN" if "T2M_MIN" in ds else None)
    if temp_var:
        plot_seasonal_cycle(ax_s0, ds, temp_var)
    else:
        ax_s0.axis("off")
        
    # Precip Seasonal
    ax_s1 = fig.add_subplot(gs[seasonal_row, 1])
    if "TOTAL_PREC" in ds:
        plot_seasonal_cycle(ax_s1, ds, "TOTAL_PREC")
    else:
        ax_s1.axis("off")
        
    # First 2 valid observations
    valid_obs = [v for v in groups["has_data"] if v in OBS_VARS]
    for i in range(2):
        ax_obs = fig.add_subplot(gs[seasonal_row, 2 + i])
        if i < len(valid_obs):
            plot_seasonal_cycle(ax_obs, ds, valid_obs[i])
        else:
            ax_obs.axis("off")
            
    # Save output
    base_name = os.path.splitext(os.path.basename(args.path))[0]
    if base_name.endswith(".cbf"):
        base_name = os.path.splitext(base_name)[0]
    out_filename = f"{base_name}_summary.png"
    out_path = os.path.join(args.output, out_filename)
    
    os.makedirs(args.output, exist_ok=True)
    plt.savefig(out_path, dpi=200, bbox_inches="tight")
    print(f"Summary visual saved to: {out_path}")
    
    if not args.no_show:
        plt.show()
    plt.close()


if __name__ == "__main__":
    main()