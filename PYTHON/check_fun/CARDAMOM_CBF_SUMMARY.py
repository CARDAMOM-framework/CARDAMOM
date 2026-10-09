#!/usr/bin/env python3
"""
CARDAMOM_CBF_SUMMARY.py  (v4)

Quick visual sanity check of CARDAMOM driver/input files (.cbf.nc).
Run it BEFORE submitting a run, or on a file someone says "doesn't work".

What you get
------------
1. Executive summary (window 1): one landscape page.
     - header: file overview, climate snapshot, warnings, location map
     - one "lane" per variable: coverage (green = data, red = missing) with the
       data drawn inside it, then % present, units, mean / min / max,
       a mini seasonal cycle, and the observation uncertainty attributes.
       Forcings (teal) on top, observations (purple) below.
2. All time series on one page (window 2).
3. Log PDF ({file}_log.pdf): the information dump. Pages 1-2 are the two
   windows above, then one full page per variable (time series, typical year,
   every attribute, list of gaps), then constraints and unused variables.

Usage
-----
    python3 CARDAMOM_CBF_SUMMARY.py FILE.cbf.nc               # windows + log PDF
    python3 CARDAMOM_CBF_SUMMARY.py FILE.cbf.nc --save        # summary PDF, no windows
    python3 CARDAMOM_CBF_SUMMARY.py DIR_OR_FILES... --save    # batch: one summary per file
    options: --no-log   skip the log PDF
             --no-show  don't open windows
             --output D folder for saved files (default: current folder)

The script reads DALEC_MODEL_FIELD_REQUIREMENTS.txt from its own folder
(FIELD:[min,max]:REQUIRED_BY:FINITE:RESPONDS[:UNITS]). The optional 6th
column, units, is used to check the units in the file.
"""

import argparse
import glob
import math
import os
import sys
import textwrap

import numpy as np
import xarray as xr
import matplotlib

# When nothing is shown on screen, use the non-interactive backend so the
# script also works over ssh / in batch jobs without a display.
if any(flag in sys.argv for flag in ("--save", "--no-show")):
    matplotlib.use("Agg")

import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from matplotlib.backends.backend_pdf import PdfPages

VERSION = "v4d (2026-10-09)"

# =============================================================================
# CONSTANTS
# =============================================================================

# Name of the time dimension. CARDAMOM files use either "time_dim" or "time";
# load_file() detects which one this file uses and updates this value.
TIME_DIM = "time_dim"

# Observation (data constraint) variables CARDAMOM's likelihood code can read.
# Order = order of the observation lanes (seasonal ones first).
OBS_ORDER = ["GPP", "LAI", "NBE", "ABGB", "ET", "LE", "H", "SIF", "SCF",
             "SWE", "EWT", "ROFF", "CH4", "FIR", "DOM"]
OBS_VARS = set(OBS_ORDER)

# Metadata / bookkeeping variables: never plotted as lanes.
META_VARS = {"time", "DOY", "ID", "LAT", "LON", "EDC", "MCMCID"}

# Readable versions of the raw unit strings found in CARDAMOM files.
# Add new entries here as you meet them; unknown units are shown unchanged.
UNIT_LABELS = {
    "deg C": "°C", "degC": "°C", "C": "°C", "degrees Celsius": "°C",
    "MJ m**-2 day**-1": "MJ/m²/day", "MJ m-2 day-1": "MJ/m²/day",
    "mm day-1": "mm/day", "g C m-2 day-1": "gC/m²/day", "g C m-2": "gC/m²",
    "m2 m-2 pixel land area": "m²/m²", "g C m-2 month-1": "gC/m²/month",
    "CO2 [ppm]": "ppm", "VPD [hPa]": "hPa",
    "gC/m2/day": "gC/m²/day", "gC m-2 day-1": "gC/m²/day",
    "gC/m2": "gC/m²", "m2/m2": "m²/m²", "m2 m-2": "m²/m²",
}

# Uncertainty attributes CARDAMOM reads (READ_NETCDF_TIMESERIES_OBS_FIELDS in
# CARDAMOM_LIKELIHOOD_FUNCTION.c), with the short label used in the table.
UNC_ATTRS = {
    "single_unc": "",
    "single_monthly_unc": "mo ",
    "single_annual_unc": "yr ",
    "single_decadal_unc": "dec ",
    "single_mean_unc": "mean ",
    "structural_unc": "str ",
}
# The other observation settings, shown as their own columns.
OBS_OPT_ATTRS = {"opt_unc_type": "type", "opt_filter": "filt",
                 "opt_normalization": "norm", "min_threshold": "thr"}

# Single-value constraints (not time series) start with these prefixes.
CONSTRAINT_PREFIXES = ("Mean_", "PEQ_")

# Temperature fields: a mean above 100 almost certainly means Kelvin.
TEMP_VARS = ("T2M_MIN", "T2M_MAX", "SKT")

# "Plausible" ranges. Looser physical checks than DALEC's hard [min,max]:
# values outside these won't crash CARDAMOM, but suggest wrong units.
SANITY_RANGES = {
    "CO2": (250, 600),          # ppm
    "VPD": (0, 100),            # hPa (values in Pa would be ~100x larger)
    "TOTAL_PREC": (0, 50),      # mm/day as a monthly mean (mm/month would be ~30x larger)
    "SNOWFALL": (0, 50),        # mm/day
    "SSRD": (0, 45),            # MJ/m²/day (top-of-atmosphere max is ~45)
    "STRD": (0, 60),            # MJ/m²/day
    "LAI": (0, 15),             # m²/m²
}

# Colours (one place, so the whole figure stays consistent)
C_FORCING = "#0F6E56"      # teal: forcing lanes
C_OBS = "#534AB7"          # purple: observation lanes
C_PRESENT = "#dff2e1"      # pale green: data present
C_MISSING = "#f6b9b9"      # pale red: data missing
C_ALERT = "#b30000"        # red text: problems
C_IGNORED = "#a86400"      # amber text: set, but CARDAMOM ignores it here
C_MUTED = "#777777"


# =============================================================================
# SMALL HELPERS
# =============================================================================

def clean_units(units):
    """Return a readable unit label, e.g. 'MJ m**-2 day**-1' -> 'MJ/m²/day'."""
    return UNIT_LABELS.get(str(units).strip(), str(units).strip())


def shorten_middle(text, max_len=40):
    """Shorten long text by cutting out the middle: 'abcdef…uvwxyz'."""
    if len(text) <= max_len:
        return text
    keep = (max_len - 1) // 2
    return text[:keep] + "…" + text[-keep:]


def wrap_lines(lines, width=45):
    """Wrap each text line to a maximum width, indenting continuation lines."""
    wrapped = []
    for line in lines:
        wrapped.extend(textwrap.wrap(line, width=width, subsequent_indent="  ") or [""])
    return wrapped


def fmt_number(value):
    """Whole numbers as integers with thousands separators (1000000.0 -> 1,000,000)."""
    try:
        f = float(value)
    except (TypeError, ValueError):
        return str(value)
    return f"{int(f):,}" if f.is_integer() else f"{f:g}"


def fmt_stat(x):
    """Compact number for the stats table: 3 significant figures,
    scientific notation for very large or very small values."""
    try:
        x = float(x)
    except (TypeError, ValueError):
        return str(x)
    if not np.isfinite(x):
        return "–"
    if x != 0 and (abs(x) >= 1e5 or abs(x) < 1e-2):
        return f"{x:.1e}"
    if abs(x) >= 1000:
        return f"{x:.0f}"
    return f"{x:.3g}"


def fmt_bound(x):
    """Format a [min,max] bound from the requirements file (±INF -> ±inf)."""
    return "inf" if x == np.inf else "-inf" if x == -np.inf else f"{x:g}"


def values_of(da):
    """Values of a variable as a flat float array, with -9999 treated as missing."""
    vals = np.asarray(da.values, dtype=float).flatten()
    vals[vals == -9999] = np.nan
    return vals


def get_times(ds):
    """The time axis: real dates if the file has a 'time' variable,
    otherwise just the step index 0, 1, 2, ..."""
    if "time" in ds.variables:
        return ds["time"].values
    return np.arange(ds.sizes.get(TIME_DIM, 0))


def is_dates(times):
    return np.issubdtype(np.asarray(times).dtype, np.datetime64)


def half_timestep(times):
    """Half the typical spacing between timesteps, used to widen gap shading."""
    if len(times) > 1:
        return np.median(np.diff(times)) / 2
    return np.timedelta64(15, "D") if is_dates(times) else 0.5


def shade_gaps(ax, times, gaps, color="#ffcccc", alpha=0.7):
    """Shade each missing-data run, padded by half a timestep each side
    so that even a single missing timestep is visible."""
    pad = half_timestep(times)
    for start_t, end_t in gaps:
        ax.axvspan(start_t - pad, end_t + pad, color=color, alpha=alpha, lw=0, zorder=0)


def marker_size(n_points):
    """Smaller dots for long records so they don't merge into a thick band."""
    return 3 if n_points <= 60 else (2 if n_points <= 150 else 1.2)


def set_time_axis(ax, times):
    """Year-based x-axis ticks without repeated labels."""
    if is_dates(times):
        locator = mdates.AutoDateLocator(minticks=3, maxticks=7)
        ax.xaxis.set_major_locator(locator)
        ax.xaxis.set_major_formatter(mdates.ConciseDateFormatter(locator))


def is_time_series(ds, var):
    return TIME_DIM in ds[var].dims


# =============================================================================
# LOADING AND TIME-AXIS CHECKS
# =============================================================================

def load_file(path):
    """Open a NetCDF file with xarray. Returns (ds, notes)."""
    global TIME_DIM
    if not os.path.exists(path):
        print(f"Error: File not found: {path}", file=sys.stderr)
        sys.exit(1)
    try:
        ds = xr.open_dataset(path)
    except Exception as e:
        print(f"Error opening NetCDF file '{path}': {e}", file=sys.stderr)
        sys.exit(1)

    notes = []
    if "time" in ds.variables:
        # 'time' is stored as an ordinary variable. Make it a coordinate so it
        # isn't treated as data and so time tools (groupby("time.month")) work.
        ds = ds.set_coords("time")
        TIME_DIM = ds["time"].dims[0]
    else:
        # No time variable: fall back to a dimension with 'time' in its name.
        time_dims = [d for d in ds.dims if "time" in d.lower()]
        TIME_DIM = time_dims[0] if time_dims else (list(ds.dims)[0] if ds.dims else "time")
        notes.append("No 'time' variable: x-axes show the step index, no typical year")
    return ds, notes


def step_lengths_days(times):
    """Length of each timestep in days, as floats.
    NOTE: dividing by np.timedelta64(1, 'D') keeps fractions of a day.
    The old version used .astype('timedelta64[D]'), which truncates to whole
    days. That's why it reported 30.0 d for steps that are really 30.4375 d."""
    d = np.diff(times)
    if is_dates(times):
        return d / np.timedelta64(1, "D")
    return d.astype(float)


def check_time_axis(ds):
    """Check the time axis is sorted, has no duplicates and has equal steps.
    Unsorted / duplicate times are fixed in the returned dataset so the plots
    are right, and reported as warnings so the file itself gets fixed.
    Returns (ds, step_line, warnings)."""
    warnings = []
    if "time" not in ds.variables:
        return ds, f"Steps: {ds.sizes.get(TIME_DIM, 0)} (no dates)", []

    times = ds["time"].values
    n = len(times)
    if n < 2:
        return ds, f"Steps: {n}", ["Only one timestep in the file"]

    # 1. Sorted? (negative steps = time goes backwards somewhere)
    n_back = int(np.sum(step_lengths_days(times) < 0))
    if n_back:
        warnings.append(f"Time not sorted: goes backwards {n_back}x (sorted for plots)")
        ds = ds.sortby("time")

    # 2. Duplicates? Keep the first copy of each timestamp for plotting.
    unique_t, first_idx = np.unique(ds["time"].values, return_index=True)
    n_dup = n - len(unique_t)
    if n_dup:
        warnings.append(f"{n_dup} duplicate timestamps (first copy kept for plots)")
        ds = ds.isel({TIME_DIM: np.sort(first_idx)})

    # 3. Step lengths. Round to 4 decimals so float noise doesn't count as a new step.
    times = ds["time"].values
    steps = np.round(step_lengths_days(times), 4)
    if len(steps) == 0:
        return ds, "Steps: 1", warnings + ["Only one unique timestep"]
    lengths, counts = np.unique(steps, return_counts=True)
    order = np.argsort(counts)[::-1]
    lengths, counts = lengths[order], counts[order]
    median = np.median(steps)
    res = ("daily" if median <= 1.5 else "monthly" if 27 <= median <= 32
           else "yearly" if 360 <= median <= 367 else f"{median:g}-day")

    if len(lengths) == 1:
        step_line = f"Step: {lengths[0]:g} d, constant ({res})"
    else:
        parts = ", ".join(f"{L:g} d ×{c}" for L, c in zip(lengths[:3], counts[:3]))
        step_line = f"Step ({res}): {parts}"
        where = ""
        if is_dates(times):
            # Are all the odd-length steps at the Dec -> Jan boundary?
            odd = np.where(steps != lengths[0])[0]
            next_month = (times[1:][odd].astype("datetime64[M]").astype(int) % 12) + 1
            if np.all(next_month == 1):
                where = " (odd steps all at Dec→Jan)"
        warnings.append(f"Time step not constant: {parts}{where}")
        if lengths.max() > 2 * lengths.min():
            warnings.append("Mixed step lengths (e.g. daily and monthly in one file)")
    return ds, step_line, warnings


# =============================================================================
# REQUIREMENTS FILE
# =============================================================================

def parse_requirements(req_path):
    """Read DALEC_MODEL_FIELD_REQUIREMENTS.txt into an ordered dict:
        field -> {min, max, req_by, finite, responds, units}
    Line format: FIELD:[min,max]:REQUIRED_BY:FINITE:RESPONDS[:UNITS]
    Returns None if the file isn't found."""
    if not os.path.exists(req_path):
        return None

    def bound(s):
        s = s.strip().upper()
        return -np.inf if s == "-INF" else np.inf if s == "INF" else float(s)

    reqs = {}
    with open(req_path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("//"):
                continue
            parts = [p.strip() for p in line.split(":")]
            if len(parts) < 5:
                continue
            lo, hi = parts[1].strip("[]").split(",")
            reqs[parts[0]] = {"min": bound(lo), "max": bound(hi), "req_by": parts[2],
                              "finite": parts[3], "responds": parts[4],
                              "units": parts[5] if len(parts) > 5 else ""}
    return reqs


def model_requires(model_id, rule):
    """Does model `model_id` require a field, given its REQUIRED_BY rule?
    Rules: 'NONE', 'ALL', 'ALL -[ID],...', or '[ID_1],[ID_2],...'."""
    rule = rule.strip()
    ids = lambda s: [x.strip() for x in s.replace("[", "").replace("]", "").split(",") if x.strip()]
    if rule == "NONE":
        return False
    if rule == "ALL":
        return True
    if rule.startswith("ALL -"):
        return str(model_id) not in ids(rule[5:])
    return str(model_id) in ids(rule)


# =============================================================================
# ANALYSIS
# =============================================================================

def classify_variables(ds):
    """Split time-varying variables into has_data, all_zeros, constant and all_missing."""
    groups = {"has_data": [], "all_zeros": [], "constant": [], "all_missing": []}
    for var_name, da in ds.data_vars.items():
        if TIME_DIM not in da.dims or var_name == "DOY":
            continue
        vals = values_of(da)
        valid = vals[~np.isnan(vals)]
        if valid.size == 0:
            groups["all_missing"].append(var_name)
        elif np.all(valid == 0):
            groups["all_zeros"].append(var_name)
        elif np.all(valid == valid[0]):
            groups["constant"].append(var_name)
        else:
            groups["has_data"].append(var_name)
    return groups


def find_gaps(da):
    """Return (number of missing steps, list of (start, end) times of each missing run)."""
    vals = values_of(da)
    is_missing = np.isnan(vals)
    times = da["time"].values if "time" in da.coords else np.arange(len(vals))
    gaps, start = [], None
    for i, miss in enumerate(is_missing):
        if miss and start is None:
            start = i
        elif not miss and start is not None:
            gaps.append((times[start], times[i - 1]))
            start = None
    if start is not None:
        gaps.append((times[start], times[-1]))
    return int(is_missing.sum()), gaps


def find_additional_vars(ds, listed_fields):
    """Variables CARDAMOM doesn't use: not in the requirements file, not a known
    observation, not metadata, not an uncertainty or single-value constraint.
    Returns a list of (name, description)."""
    known = set(listed_fields) | OBS_VARS | META_VARS
    extra = []
    # ds.variables (not ds.data_vars) so coordinates like 'latitude' are listed too
    for name in sorted(ds.variables):
        if name in ds.dims:          # dimension index variables, not real data
            continue
        if name in known or name.endswith("unc") or name.startswith(CONSTRAINT_PREFIXES):
            continue
        extra.append((name, ds[name].attrs.get("description", "")))
    return extra


def split_lanes(ds, reqs, additional_names):
    """Pick the variables that get a lane, split into (forcings, observations).
    Forcings follow the requirements-file order; observations follow OBS_ORDER."""
    candidates = [v for v in ds.data_vars
                  if is_time_series(ds, v) and v not in META_VARS
                  and not v.endswith("unc") and v not in additional_names]
    obs = sorted((v for v in candidates if v in OBS_VARS), key=OBS_ORDER.index)
    req_order = list(reqs) if reqs else []
    rank = lambda v: (req_order.index(v) if v in req_order else len(req_order), v)
    forcings = sorted((v for v in candidates if v not in OBS_VARS), key=rank)
    return forcings, obs


def has_uncertainty(ds, var):
    """True if an observation has an uncertainty CARDAMOM can use:
    a '<VAR>unc' variable or any of the UNC_ATTRS attributes."""
    return f"{var}unc" in ds or any(a in ds[var].attrs for a in UNC_ATTRS)


def file_overview(ds, step_line):
    """Header text: file, location, model, MCMC settings, time range, constraints."""
    lines = []
    fname = os.path.basename(ds.encoding.get("source", "Unknown file"))
    lines.append(f"File: {shorten_middle(fname, 60)}")
    lat = float(ds["LAT"].values) if "LAT" in ds else np.nan
    lon = float(ds["LON"].values) if "LON" in ds else np.nan
    model_id = int(ds["ID"].values) if "ID" in ds else "?"
    lines.append(f"Lat/Lon: {lat:.2f}, {lon:.2f}   |   DALEC model ID: {model_id}")

    # Run settings, squeezed onto one or two lines
    if "MCMCID" in ds:
        a = ds["MCMCID"].attrs
        mc = f"MCMC {int(ds['MCMCID'].values)}: " + ", ".join(
            f"{k} {fmt_number(a[k])}" for k in ("nITERATIONS", "nSAMPLES", "fADAPT") if k in a)
    else:
        mc = "MCMCID: not found"
    edc = ("EDC on" if int(ds["EDC"].values) == 1 else "EDC off") if "EDC" in ds else "EDC: not found"
    lines.append(f"{mc}   |   {edc}")

    times = get_times(ds)
    if len(times) and is_dates(times):
        lines.append(f"Range: {np.datetime_as_string(times[0], unit='D')} to "
                     f"{np.datetime_as_string(times[-1], unit='D')} ({len(times)} steps)")
    else:
        lines.append(f"Timesteps: {len(times)}")
    lines.append(step_line)

    # Single-value constraints (PEQ_*, Mean_*): value ± uncertainty
    cons = []
    for v in sorted(ds.data_vars):
        if v.startswith(CONSTRAINT_PREFIXES) and ds[v].size == 1:
            unc = ds[v].attrs.get("unc")
            cons.append(f"{v} {fmt_stat(ds[v].values)}" + (f" (unc {fmt_stat(unc)})" if unc is not None else ""))
    lines.append("Constraints: " + (", ".join(cons) if cons else "none"))
    return lines


def climate_snapshot(ds):
    """Mean temperature, annual precipitation, snowfall fraction, CO2 change."""
    lines = []
    if "T2M_MIN" in ds and "T2M_MAX" in ds:
        t_mean = np.nanmean((values_of(ds["T2M_MIN"]) + values_of(ds["T2M_MAX"])) / 2)
        lines.append(f"Mean temp: {t_mean:.1f} {clean_units(ds['T2M_MAX'].attrs.get('units', '°C'))}")
    else:
        lines.append("Mean temp: N/A")

    if "TOTAL_PREC" in ds:
        units = ds["TOTAL_PREC"].attrs.get("units", "")
        p_mean = np.nanmean(values_of(ds["TOTAL_PREC"]))
        if "day" in units.lower() or units.strip() == "mm":
            lines.append(f"Precip: {p_mean * 365.25:.0f} mm/yr")
        else:
            lines.append(f"Precip: {p_mean:.2f} {units}")
    else:
        lines.append("Precip: N/A")

    if "SNOWFALL" in ds and "TOTAL_PREC" in ds:
        tot_p = np.nansum(values_of(ds["TOTAL_PREC"]))
        frac = np.nansum(values_of(ds["SNOWFALL"])) / tot_p * 100 if tot_p > 0 else 0.0
        lines.append(f"Snowfall fraction: {frac:.0f}%")

    if "CO2" in ds:
        co2 = values_of(ds["CO2"])
        co2 = co2[~np.isnan(co2)]
        lines.append(f"CO2: {co2[0]:.0f} → {co2[-1]:.0f} ppm" if co2.size else "CO2: no valid values")
    else:
        lines.append("CO2: not present")
    return lines


def build_warnings(ds, groups, required, additional=(), forcings=()):
    """Missing required fields, observations without data or uncertainty,
    all-zero / constant / all-missing variables, forcings with gaps."""
    warnings = []
    if required is None:
        warnings.append("DALEC_MODEL_FIELD_REQUIREMENTS.txt not found: required fields not checked")
    else:
        for field in sorted(required):
            if field not in ds:
                warnings.append(f"MISSING required field: {field}")

    for var in ds.data_vars:
        if var in OBS_VARS and is_time_series(ds, var):
            if np.all(np.isnan(values_of(ds[var]))):
                warnings.append(f"Observation {var} has 0 valid data points")
            elif not has_uncertainty(ds, var):
                warnings.append(f"Observation {var} has no uncertainty")

    skip = {name for name, _ in additional}
    for v in groups["all_missing"]:
        if v not in skip and v not in OBS_VARS:   # observations already reported above
            warnings.append(f"{v} is all missing (NaN)")
    for v in groups["all_zeros"]:
        if v not in skip:
            warnings.append(f"{v} is all zeros")
    for v in groups["constant"]:
        # constant uncertainty is normal; with one timestep everything is "constant"
        if v not in skip and not v.endswith("unc") and ds.sizes.get(TIME_DIM, 0) > 1:
            warnings.append(f"{v} is constant (non-zero)")

    # Gaps matter for forcings (CARDAMOM needs every step). Observation gaps are
    # normal and already visible in the lanes, so they aren't repeated here.
    n_times = ds.sizes.get(TIME_DIM, 0)
    for v in forcings:
        if v in groups["has_data"] and n_times:
            n_missing, _ = find_gaps(ds[v])
            if n_missing:
                warnings.append(f"Forcing {v} has {n_missing} missing steps")
    return warnings


def check_ranges(ds, reqs):
    """Range and unit checks. Returns (warnings, flagged) where flagged is the
    set of variables to highlight in red in the summary table.
      1. DALEC [min,max] from the requirements file (hard limits)
      2. Temperature that looks like Kelvin
      3. SANITY_RANGES: plausible values (catches wrong units)
      4. Units in the file vs the units column of the requirements file (if present)"""
    warnings, flagged = [], set()
    for var in ds.data_vars:
        vals = values_of(ds[var])
        vals = vals[~np.isnan(vals)]
        if vals.size == 0:
            continue

        if reqs and var in reqs:
            lo, hi = reqs[var]["min"], reqs[var]["max"]
            n_out = int(np.sum((vals < lo) | (vals > hi)))
            if n_out:
                warnings.append(f"{var}: {n_out}/{vals.size} values outside DALEC range "
                                f"[{fmt_bound(lo)}, {fmt_bound(hi)}] (min {fmt_stat(vals.min())}, "
                                f"max {fmt_stat(vals.max())})")
                flagged.add(var)

        if var in TEMP_VARS and np.mean(vals) > 100:
            warnings.append(f"{var} looks like Kelvin (mean {np.mean(vals):.0f}); expected °C")
            flagged.add(var)
        elif var in SANITY_RANGES and var not in flagged:
            lo, hi = SANITY_RANGES[var]
            n_out = int(np.sum((vals < lo) | (vals > hi)))
            if n_out:
                warnings.append(f"{var}: {n_out} values outside plausible range [{lo}, {hi}]: check units")
                flagged.add(var)

        if reqs and var in reqs and reqs[var]["units"]:
            expected = clean_units(reqs[var]["units"])
            actual = clean_units(ds[var].attrs.get("units", ""))
            if actual != expected:
                warnings.append(f"{var} units '{actual or 'none'}', expected '{expected}'")
                flagged.add(var)
    return warnings, flagged


def check_attributes(ds, obs_lanes, forcing_lanes):
    """Attribute problems: zero thresholds, settings on forcings (ignored by
    CARDAMOM), and lat/lon variables that disagree with LAT/LON."""
    warnings = []
    for v in obs_lanes:
        thr = ds[v].attrs.get("min_threshold")
        if thr is not None and float(thr) == 0:
            warnings.append(f"{v} min_threshold = 0 (risk of divide-by-zero)")
    for v in forcing_lanes:
        set_attrs = [a for a in list(UNC_ATTRS) + list(OBS_OPT_ATTRS) if a in ds[v].attrs]
        if set_attrs:
            warnings.append(f"Forcing {v} has {', '.join(set_attrs)}: ignored by CARDAMOM")
    for alt, main in (("latitude", "LAT"), ("longitude", "LON")):
        if alt in ds and main in ds and ds[alt].size == 1:
            a, m = float(ds[alt].values), float(ds[main].values)
            if abs(a - m) > 1e-6:
                warnings.append(f"'{alt}' = {a:g} but {main} = {m:g} (CARDAMOM reads {main})")
    return warnings


def obs_attr_cells(ds, var, is_obs):
    """The uncertainty-table cells for one lane, as {column: (text, colour)}.
    Observations: missing uncertainty is red, min_threshold = 0 is red.
    Forcings: normally blank; anything set is amber (CARDAMOM ignores it)."""
    attrs = ds[var].attrs
    base = "#222222" if is_obs else C_IGNORED
    unc = [f"{short}{fmt_stat(attrs[a])}" for a, short in UNC_ATTRS.items() if a in attrs]
    if f"{var}unc" in ds:
        unc.append(f"{var}unc")
    cells = {"unc": (" ".join(unc), base) if unc else (("none", C_ALERT) if is_obs else ("", base))}
    for attr, col in OBS_OPT_ATTRS.items():
        if attr in attrs:
            val = float(attrs[attr])
            colour = C_ALERT if (attr == "min_threshold" and val == 0) else base
            cells[col] = (fmt_stat(val), colour)
        else:
            cells[col] = ("", base)
    return cells


def monthly_means(ds, var):
    """Mean for each calendar month (Jan..Dec), or None if there are no dates."""
    if "time" not in ds.coords or not is_dates(ds["time"].values):
        return None
    da = ds[var].where(ds[var] != -9999)
    m = da.groupby("time.month").mean(skipna=True)
    out = np.full(12, np.nan)
    out[m["month"].values - 1] = m.values
    return out


# =============================================================================
# PLOTTING: building blocks
# =============================================================================

def plot_text_panel(ax, title, lines, highlight=False, ncols=1, wrap=45, fontsize=8):
    """Titled box of text. highlight=True draws it in red; ncols splits long lists."""
    ax.set_xticks([]); ax.set_yticks([])
    color = C_ALERT if highlight else "#222222"
    ax.set_facecolor("#fff2f2" if highlight else "#f7f7f7")
    for spine in ax.spines.values():
        spine.set_edgecolor(C_ALERT if highlight else "#cccccc")
        spine.set_linewidth(1.0)
    lines = wrap_lines(lines, width=wrap)
    per_col = max(1, math.ceil(len(lines) / ncols))
    for c in range(ncols):
        chunk = lines[c * per_col:(c + 1) * per_col]
        ax.text(0.015 + c / ncols, 0.95, "\n".join(chunk), transform=ax.transAxes,
                fontsize=fontsize, va="top", fontfamily="monospace", color=color, linespacing=1.35)
    ax.set_title(title, fontsize=fontsize + 2, fontweight="bold", color=color, loc="left", pad=3)


def add_location_map(fig, spec, lat, lon):
    """Small world map with the site marked. Uses cartopy coastlines if cartopy
    is installed AND its coastline data is available; otherwise a plain
    lat/lon grid. Never fails because of the map."""
    try:
        import cartopy.crs as ccrs
        import cartopy.io.shapereader as shpreader
        shpreader.natural_earth(resolution="110m", category="physical", name="coastline")  # raises if unavailable
        ax = fig.add_subplot(spec, projection=ccrs.PlateCarree())
        ax.set_global()
        ax.coastlines(linewidth=0.4, color="#888888")
    except Exception:
        ax = fig.add_subplot(spec)
        ax.set_xlim(-180, 180); ax.set_ylim(-90, 90)
        # Grid lines every 90° lon / 45° lat; no tick labels (the title gives the coordinates)
        ax.set_xticks(range(-180, 181, 90)); ax.set_yticks(range(-90, 91, 45))
        ax.tick_params(labelbottom=False, labelleft=False, length=0)
        ax.grid(True, linestyle=":", color="#bbbbbb", lw=0.6)
        ax.axhline(0, color="#999999", lw=0.6)
        ax.set_aspect("equal")
    if np.isfinite(lat) and np.isfinite(lon):
        ax.plot(lon, lat, "o", color=C_ALERT, ms=6, mec="white", mew=0.8, zorder=5)
        ns, ew = ("N" if lat >= 0 else "S"), ("E" if lon >= 0 else "W")
        ax.set_title(f"Location: {abs(lat):.2f}{ns}, {abs(lon):.2f}{ew}",
                     fontsize=9.5, fontweight="bold", loc="left", pad=3)
    else:
        ax.set_title("Location: LAT/LON missing", fontsize=9.5, fontweight="bold",
                     loc="left", pad=3, color=C_ALERT)
    return ax


def draw_lane(ax, ds, var, color):
    """One lane: green background where data exists, red where missing,
    and the data itself drawn on top as dots joined by a faint line."""
    times = get_times(ds)
    vals = values_of(ds[var])
    ax.set_facecolor(C_PRESENT)
    ax.set_yticks([])
    for spine in ax.spines.values():
        spine.set_edgecolor("#cfcfcf"); spine.set_linewidth(0.5)

    valid = vals[~np.isnan(vals)]
    if valid.size == 0:
        ax.set_facecolor(C_MISSING)
        ax.text(0.01, 0.5, "all missing", transform=ax.transAxes, fontsize=6.5,
                va="center", color=C_ALERT, style="italic")
        return

    _, gaps = find_gaps(ds[var])
    shade_gaps(ax, times, gaps, color=C_MISSING, alpha=1.0)

    ax.plot(times, vals, "-", lw=0.5, color=color, alpha=0.45)
    ax.plot(times, vals, ".", ms=marker_size(len(vals)) + 0.6, color=color)

    lo, hi = valid.min(), valid.max()
    pad = (hi - lo) * 0.12 if hi > lo else (abs(hi) * 0.1 or 1.0)
    ax.set_ylim(lo - pad, hi + pad)
    if np.all(valid == 0):
        ax.text(0.01, 0.5, "all zeros", transform=ax.transAxes, fontsize=6.5,
                va="center", color=C_ALERT, style="italic")
    elif hi == lo:
        ax.text(0.01, 0.5, f"constant {fmt_stat(hi)}", transform=ax.transAxes,
                fontsize=6.5, va="center", color=C_ALERT, style="italic")


def plot_timeseries(ax, ds, var, color="#1f77b4"):
    """Full-size time series: dots, a thin line and light-red gap shading."""
    times = get_times(ds)
    vals = values_of(ds[var])
    ax.plot(times, vals, marker=".", markersize=marker_size(len(vals)), linewidth=0.6, color=color)
    _, gaps = find_gaps(ds[var])
    shade_gaps(ax, times, gaps)
    ax.set_title(var, fontsize=10, fontweight="bold", pad=3, color=color)
    ax.set_ylabel(clean_units(ds[var].attrs.get("units", "")), fontsize=8.5)
    ax.tick_params(axis="both", labelsize=8)
    set_time_axis(ax, times)


def plot_temperature(ax, ds):
    """T2M_MAX and T2M_MIN on one panel."""
    times = get_times(ds)
    tmin, tmax = values_of(ds["T2M_MIN"]), values_of(ds["T2M_MAX"])
    ms = marker_size(len(tmax))
    ax.plot(times, tmax, marker=".", markersize=ms, linewidth=0.5, color="#d95f02", label="MAX")
    ax.plot(times, tmin, marker=".", markersize=ms, linewidth=0.5, color="#2b83ba", label="MIN")
    _, gaps = find_gaps(ds["T2M_MIN"])
    shade_gaps(ax, times, gaps)
    ax.set_title("Temperature", fontsize=10, fontweight="bold", pad=3, color=C_FORCING, loc="left")
    ax.set_ylabel(clean_units(ds["T2M_MAX"].attrs.get("units", "°C")), fontsize=8.5)
    ax.legend(loc="lower right", bbox_to_anchor=(1.0, 1.0), ncol=2, fontsize=7.5, frameon=False)
    ax.tick_params(axis="both", labelsize=8)
    set_time_axis(ax, times)


def plot_seasonal_cycle(ax, ds, variables, title=None, color=None):
    """Mean for each calendar month (Jan-Dec) for one or more variables."""
    if isinstance(variables, str):
        variables = [variables]
    colors = [color] if color else ["#d95f02", "#2b83ba", "#2ca02c", "#7570b3"]
    for i, var in enumerate(variables):
        means = monthly_means(ds, var)
        if means is None:
            ax.text(0.5, 0.5, "No dates in file", transform=ax.transAxes, ha="center", fontsize=8)
            return
        ax.plot(range(1, 13), means, marker="o", markersize=3.5, linewidth=1.2,
                color=colors[i % len(colors)], label=var)
    ax.set_xticks(range(1, 13))
    ax.set_xticklabels(list("JFMAMJJASOND"), fontsize=7)
    if len(variables) > 1:
        ax.legend(fontsize=6.5)
    ax.set_title(title or f"Typical year: {variables[0]}", fontsize=8.5, fontweight="bold", pad=2)
    ax.set_ylabel(clean_units(ds[variables[0]].attrs.get("units", "")), fontsize=7.5)
    ax.tick_params(axis="both", labelsize=7)
    ax.grid(True, linestyle=":", alpha=0.5)


# =============================================================================
# FIGURE 1: EXECUTIVE SUMMARY
# =============================================================================

# Window size in inches. At matplotlib's default 100 dpi this is 1450 x 830
# points, which fits a 13-14" MacBook screen with the menu bar and the
# window toolbar. Fonts are in points, so they stay readable at this size.
# Avoid going full screen: the canvas grows but the text doesn't.
SCREEN_W, SCREEN_H = 14.5, 8.3
FS = 9            # base font size (points) for the summary page

# Column positions (figure fraction) for the table to the right of the lanes.
# Change these numbers to move or resize columns.
LANE_LEFT, LANE_RIGHT = 0.103, 0.47
COLS = {"pct": 0.477, "units": 0.507, "mean": 0.575, "min": 0.617, "max": 0.659,
        "cycle": (0.70, 0.055),                       # (left, width) of the mini cycle
        "unc": 0.765, "type": 0.878, "filt": 0.905, "norm": 0.932, "thr": 0.959}
COL_TITLES = {"pct": "data", "units": "units", "mean": "mean", "min": "min", "max": "max",
              "cycle": "typical yr", "unc": "uncertainty", "type": "type", "filt": "filt",
              "norm": "norm", "thr": "thr"}


def build_summary_figure(ds, info):
    """The one-page executive summary, sized to fit a laptop screen.
    Layout (top to bottom): title, header row (overview | climate | warnings | map),
    column titles, forcing lanes, observation lanes, footer.
    Fixed parts are in inches; the lanes share whatever height is left.
    With a lot of variables the lanes hit a minimum height and the page grows."""
    forcings, obs = info["forcings"], info["obs"]
    n_lanes = max(1, len(forcings) + len(obs))

    top_pad, header_h, cols_h, group_h, bottom_pad = 0.35, 1.75, 0.42, 0.24, 0.55
    fixed = top_pad + header_h + cols_h + 2 * group_h + bottom_pad
    lane_h = max(0.17, (SCREEN_H - fixed) / n_lanes)          # inches per lane
    H = fixed + n_lanes * lane_h
    body_h = n_lanes * lane_h + 2 * group_h
    fig = plt.figure(figsize=(SCREEN_W, H))
    y = lambda inches_from_top: 1 - inches_from_top / H       # inches -> figure fraction
    lane_fs = FS if lane_h >= 0.2 else FS - 1

    fig.text(0.01, y(0.17), f"CARDAMOM input summary: {info['base']}", fontsize=FS + 5,
             fontweight="bold", va="center")
    fig.text(0.99, y(0.17), VERSION, fontsize=FS - 2, color=C_MUTED, ha="right", va="center")

    # ---- Header row --------------------------------------------------------
    hdr = fig.add_gridspec(1, 4, width_ratios=[1.5, 0.75, 1.9, 0.75], wspace=0.08,
                           left=0.01, right=0.99, top=y(top_pad + 0.22), bottom=y(top_pad + header_h))
    plot_text_panel(fig.add_subplot(hdr[0, 0]), "File overview", info["overview"], wrap=60, fontsize=FS - 1)
    plot_text_panel(fig.add_subplot(hdr[0, 1]), "Climate snapshot", info["climate"], wrap=30, fontsize=FS - 1)

    warn = info["warnings"] or ["No problems found"]
    warn_lines = warn + ["", "Not used by CARDAMOM: " + (", ".join(info["additional"]) or "none")]
    # One column if it fits, otherwise two narrower columns; anything beyond
    # that is cut, with a pointer to the terminal output where the full list is.
    max_lines = 10
    ncols, wrap = (1, 74) if len(wrap_lines(warn_lines, 74)) <= max_lines else (2, 36)
    wrapped = wrap_lines(warn_lines, wrap)
    if len(wrapped) > max_lines * ncols:
        wrapped = wrapped[: max_lines * ncols - 1] + ["... more: see terminal output"]
    plot_text_panel(fig.add_subplot(hdr[0, 2]), f"Warnings ({len(info['warnings'])})", wrapped,
                    highlight=bool(info["warnings"]), ncols=ncols, wrap=200,
                    fontsize=FS - 1 if ncols == 1 else FS - 1.5)
    add_location_map(fig, hdr[0, 3], info["lat"], info["lon"])

    # ---- Lanes ---------------------------------------------------------------
    body_top = top_pad + header_h + cols_h
    rows = ["__F__"] + forcings + ["__O__"] + obs
    ratios = [group_h if r.startswith("__") else lane_h for r in rows]
    gs = fig.add_gridspec(len(rows), 1, height_ratios=ratios, hspace=0.15,
                          left=LANE_LEFT, right=LANE_RIGHT,
                          top=y(body_top), bottom=y(body_top + body_h))

    # Column titles
    ytitle = y(body_top - 0.06)
    fig.text(LANE_LEFT, ytitle, "coverage + data  (green = data, red = missing)",
             fontsize=FS, fontweight="bold", va="bottom")
    for key, title in COL_TITLES.items():
        x = COLS[key][0] if isinstance(COLS[key], tuple) else COLS[key]
        fig.text(x, ytitle, title, fontsize=FS, fontweight="bold", va="bottom")
    fig.text(COLS["unc"], ytitle + 0.17 / H, "observation settings (CARDAMOM attributes)",
             fontsize=FS - 1.5, color=C_MUTED, va="bottom")

    times = get_times(ds)
    n_times = max(1, len(times))
    first_ax, last_ax = None, None
    for r, var in enumerate(rows):
        if var.startswith("__"):
            label, color = ("Forcings", C_FORCING) if var == "__F__" else ("Observations", C_OBS)
            # Place the group title in the middle of its spacer row
            pos = gs[r, 0].get_position(fig)
            fig.text(0.01, (pos.y0 + pos.y1) / 2, label, fontsize=FS + 1, fontweight="bold",
                     color=color, va="center")
            continue

        is_obs = var in OBS_VARS
        color = C_OBS if is_obs else C_FORCING
        ax = fig.add_subplot(gs[r, 0], sharex=first_ax)
        first_ax = first_ax or ax
        last_ax = ax
        draw_lane(ax, ds, var, color)
        ax.tick_params(axis="x", labelbottom=False, length=0)
        ax.grid(True, axis="x", which="major", color="white", lw=0.8)

        # Everything in this row is aligned to the lane's vertical centre
        pos = ax.get_position()
        yc = (pos.y0 + pos.y1) / 2
        txt = lambda x, s, c="#222222", **kw: fig.text(x, yc, s, fontsize=lane_fs, color=c, va="center", **kw)
        txt(LANE_LEFT - 0.004, var, color, ha="right", fontweight="bold")

        vals = values_of(ds[var])
        valid = vals[~np.isnan(vals)]
        pct = 100 * valid.size / n_times
        txt(COLS["pct"], f"{pct:.0f}%", C_ALERT if (pct < 100 and not is_obs) else "#222222")
        units = clean_units(ds[var].attrs.get("units", ""))
        flag = var in info["flagged"]
        txt(COLS["units"], units or "none", C_ALERT if (flag or not units) else "#222222")
        stat_c = C_ALERT if flag else "#222222"
        if valid.size:
            txt(COLS["mean"], fmt_stat(valid.mean()), stat_c)
            txt(COLS["min"], fmt_stat(valid.min()), stat_c)
            txt(COLS["max"], fmt_stat(valid.max()), stat_c)

        # Mini typical-year panel
        means = monthly_means(ds, var)
        cx, cw = COLS["cycle"]
        cax = fig.add_axes([cx, pos.y0, cw, pos.height])
        cax.set_xticks([]); cax.set_yticks([])
        for s in cax.spines.values():
            s.set_edgecolor("#dddddd"); s.set_linewidth(0.5)
        if means is not None and np.any(np.isfinite(means)):
            cax.plot(range(1, 13), means, "-", lw=1.2, color=color)
            cax.plot(range(1, 13), means, ".", ms=3, color=color)
            cax.set_xlim(0.5, 12.5)

        # Observation settings table
        for col, (text, colour) in obs_attr_cells(ds, var, is_obs).items():
            txt(COLS[col], text, colour)

    # Year labels under the last lane only (all lanes share the x-axis)
    if last_ax is not None:
        last_ax.tick_params(axis="x", labelbottom=True, labelsize=FS - 0.5, length=2)
        if is_dates(times):
            span_years = (times[-1] - times[0]) / np.timedelta64(365, "D") if len(times) > 1 else 1
            base = 1 if span_years <= 8 else 2 if span_years <= 16 else 5
            last_ax.xaxis.set_major_locator(mdates.YearLocator(base))
            last_ax.xaxis.set_major_formatter(mdates.DateFormatter("%Y"))
            last_ax.xaxis.set_minor_locator(mdates.YearLocator(1))
            for a in fig.axes:
                if a.get_shared_x_axes().joined(a, last_ax):
                    a.grid(True, axis="x", which="minor", color="white", lw=0.4)

    # ---- Footer --------------------------------------------------------------
    fig.text(0.01, y(H - 0.1),
             "Red text = problem (see Warnings).  Amber = attribute on a forcing (ignored by CARDAMOM).  "
             "unc: plain = single_unc; mo/yr/dec/mean/str = single_monthly/annual/decadal/mean/structural_unc.  "
             f"Full detail: {info['base']}_log.pdf",
             fontsize=FS - 1.5, color=C_MUTED, va="bottom")
    return fig


# =============================================================================
# FIGURE 2: ALL TIME SERIES ON ONE PAGE
# =============================================================================

def build_timeseries_figure(ds, info, cols=5):
    """Every lane variable as a small full time-series plot, on one screen-sized page."""
    panels = list(info["forcings"]) + list(info["obs"])
    has_temp = "T2M_MIN" in panels and "T2M_MAX" in panels
    if has_temp:
        panels = ["__TEMP__"] + [p for p in panels if p not in ("T2M_MIN", "T2M_MAX")]
    rows = max(1, math.ceil(len(panels) / cols))
    fig = plt.figure(figsize=(SCREEN_W, max(SCREEN_H, 1.55 * rows + 0.6)))
    fig.suptitle(f"All time series: {info['base']}", fontsize=FS + 5, fontweight="bold", x=0.01, ha="left")
    gs = fig.add_gridspec(rows, cols, hspace=0.55, wspace=0.36, top=0.92, bottom=0.04, left=0.045, right=0.99)
    for i, var in enumerate(panels):
        ax = fig.add_subplot(gs[i // cols, i % cols])
        if var == "__TEMP__":
            plot_temperature(ax, ds)
        else:
            plot_timeseries(ax, ds, var, color=C_OBS if var in OBS_VARS else C_FORCING)
    return fig


# =============================================================================
# LOG PDF: one page per variable + constraints + unused variables
# =============================================================================

def build_variable_page(ds, var, note=""):
    """Full page for one variable: time series, typical year, attributes, stats, gaps."""
    color = C_OBS if var in OBS_VARS else C_FORCING
    fig = plt.figure(figsize=(11.7, 8.3))   # A4 landscape
    fig.suptitle(f"{var}{note}", fontsize=14, fontweight="bold", x=0.02, ha="left", color=color)
    gs = fig.add_gridspec(2, 2, height_ratios=[1.3, 1], width_ratios=[1, 1.2],
                          hspace=0.3, wspace=0.15, top=0.92, bottom=0.05, left=0.07, right=0.98)
    plot_timeseries(fig.add_subplot(gs[0, :]), ds, var, color=color)
    plot_seasonal_cycle(fig.add_subplot(gs[1, 0]), ds, var, color=color)

    vals = values_of(ds[var])
    valid = vals[~np.isnan(vals)]
    n_missing, gaps = find_gaps(ds[var])
    lines = [f"Dims: {ds[var].dims}   shape {ds[var].shape}"]
    if valid.size:
        lines.append(f"mean {fmt_stat(valid.mean())}  min {fmt_stat(valid.min())}  "
                     f"max {fmt_stat(valid.max())}  std {fmt_stat(valid.std())}")
    lines.append(f"Present: {valid.size}/{vals.size}   missing: {n_missing}")
    lines.append("")
    lines.append("Attributes:")
    lines += [f"  {k}: {v}" for k, v in ds[var].attrs.items()] or ["  (none)"]
    lines.append("")
    lines.append(f"Missing runs ({len(gaps)}):")
    fmt_t = lambda t: np.datetime_as_string(t, unit="M") if is_dates(np.asarray([t])) else str(t)
    for a, b in gaps[:15]:
        lines.append(f"  {fmt_t(a)}" + (f" to {fmt_t(b)}" if b != a else ""))
    if len(gaps) > 15:
        lines.append(f"  ... and {len(gaps) - 15} more")
    plot_text_panel(fig.add_subplot(gs[1, 1]), "Details", lines, wrap=70, fontsize=7)
    return fig


def build_text_page(title, lines):
    fig = plt.figure(figsize=(11.7, 8.3))
    ax = fig.add_axes([0.03, 0.03, 0.94, 0.9])
    plot_text_panel(ax, title, lines, ncols=2 if len(lines) > 45 else 1, wrap=80, fontsize=7)
    return fig


def write_log_pdf(ds, info, summary_fig, ts_fig, path):
    """Exhaustive log: summary, all time series, then one page per variable."""
    with PdfPages(path) as pdf:
        pdf.savefig(summary_fig)
        pdf.savefig(ts_fig)
        for var in info["forcings"] + info["obs"]:
            fig = build_variable_page(ds, var)
            pdf.savefig(fig)
            plt.close(fig)

        cons = [v for v in sorted(ds.data_vars) if v.startswith(CONSTRAINT_PREFIXES)]
        lines = []
        for v in cons:
            lines.append(f"{v} = {fmt_stat(ds[v].values) if ds[v].size == 1 else ds[v].shape}")
            lines += [f"    {k}: {val}" for k, val in ds[v].attrs.items()]
        fig = build_text_page("Single-value constraints (PEQ_*, Mean_*)", lines or ["none"])
        pdf.savefig(fig); plt.close(fig)

        for var in info["additional"]:
            if is_time_series(ds, var):
                fig = build_variable_page(ds, var, note="  (not used by CARDAMOM)")
            else:
                fig = build_text_page(f"{var} (not used by CARDAMOM)",
                                      [f"value: {ds[var].values}"] +
                                      [f"{k}: {shorten_middle(str(v), 150)}" for k, v in ds[var].attrs.items()])
            pdf.savefig(fig); plt.close(fig)
    print(f"Log PDF saved to: {path}")


# =============================================================================
# MAIN
# =============================================================================

def analyse(path):
    """Load a file and run every check. Returns (ds, info) for the figures."""
    ds, notes = load_file(path)
    ds, step_line, time_warnings = check_time_axis(ds)
    groups = classify_variables(ds)
    model_id = int(ds["ID"].values) if "ID" in ds else "Unknown"

    req_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), "DALEC_MODEL_FIELD_REQUIREMENTS.txt")
    reqs = parse_requirements(req_file)
    required = {f for f, r in reqs.items() if model_requires(model_id, r["req_by"])} if reqs else None

    additional = find_additional_vars(ds, set(reqs) if reqs else set())
    additional_names = {n for n, _ in additional}
    forcings, obs = split_lanes(ds, reqs, additional_names)
    range_warnings, flagged = check_ranges(ds, reqs)

    # Order: things that stop CARDAMOM first, then nonsense-producing problems, then notes
    warnings = (build_warnings(ds, groups, required, additional, forcings)
                + range_warnings + time_warnings
                + check_attributes(ds, obs, forcings) + notes)

    base = os.path.basename(path)
    for ext in (".nc", ".cbf"):
        if base.endswith(ext):
            base = base[: -len(ext)]

    info = {
        "base": base, "forcings": forcings, "obs": obs, "flagged": flagged,
        "warnings": warnings, "additional": sorted(additional_names),
        "overview": file_overview(ds, step_line), "climate": climate_snapshot(ds),
        "lat": float(ds["LAT"].values) if "LAT" in ds else np.nan,
        "lon": float(ds["LON"].values) if "LON" in ds else np.nan,
    }
    return ds, info


def print_summary(info):
    print("=" * 70)
    print("FILE OVERVIEW:\n" + "\n".join(info["overview"]))
    print("-" * 70)
    print("CLIMATE SNAPSHOT:\n" + "\n".join(info["climate"]))
    print("-" * 70)
    print(f"WARNINGS ({len(info['warnings'])}):")
    print("\n".join(f"  - {w}" for w in info["warnings"]) or "  none")
    print("-" * 70)
    print("NOT USED BY CARDAMOM: " + (", ".join(info["additional"]) or "none"))
    print("=" * 70)


def expand_paths(paths):
    """Accept files and folders; a folder means every *.cbf.nc inside it."""
    out = []
    for p in paths:
        out += sorted(glob.glob(os.path.join(p, "*.cbf.nc"))) if os.path.isdir(p) else [p]
    return out


def main():
    parser = argparse.ArgumentParser(description="Visual sanity check of CARDAMOM .cbf.nc input files.")
    parser.add_argument("paths", nargs="+", help=".cbf.nc file(s), or a folder of them")
    parser.add_argument("--save", action="store_true",
                        help="save the executive summary as {file}_summary.pdf instead of opening windows")
    parser.add_argument("--no-show", action="store_true", help="don't open any windows")
    parser.add_argument("--no-log", action="store_true", help="don't write the log PDF")
    parser.add_argument("--output", default=".", help="folder for saved files (default: current folder)")
    args = parser.parse_args()

    paths = expand_paths(args.paths)
    if not paths:
        print("No .cbf.nc files found.", file=sys.stderr)
        sys.exit(1)
    if len(paths) > 1 and not args.save:
        print(f"{len(paths)} files: windows are off in batch mode; use --save to keep the summaries.")
    show = not (args.save or args.no_show) and len(paths) == 1
    os.makedirs(args.output, exist_ok=True)

    for path in paths:
        print(f"\n### {path}")
        ds, info = analyse(path)
        print_summary(info)

        # The window created LAST opens on top, so build the time-series page
        # first and the executive summary second: the summary is what you see.
        ts_fig = build_timeseries_figure(ds, info)
        summary_fig = build_summary_figure(ds, info)
        if show:
            # Name the windows instead of "Figure 1" / "Figure 2"
            ts_fig.canvas.manager.set_window_title(f"2. All time series: {info['base']}")
            summary_fig.canvas.manager.set_window_title(f"1. Summary: {info['base']}")
        if args.save:
            out = os.path.join(args.output, f"{info['base']}_summary.pdf")
            summary_fig.savefig(out)
            print(f"Summary saved to: {out}")
        if not args.no_log:
            write_log_pdf(ds, info, summary_fig, ts_fig, os.path.join(args.output, f"{info['base']}_log.pdf"))
        if not show:
            plt.close("all")
        ds.close()

    if show:
        plt.show()


if __name__ == "__main__":
    main()
