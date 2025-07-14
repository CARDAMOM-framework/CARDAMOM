import numpy as np

def h2o_saturation_pressure(temperature_celsius):
    """
%function P=H2O_SATURATION_PRESSURE(T)
%input = Temperature (degrees C)
%output = Water vapour saturation pressure(kPa)
%Method:
%NOAA website http://www.srh.noaa.gov/epz/?n=wxcalc_vaporpressure
%formulas in http://www.srh.noaa.gov/images/epz/wxcalc/vaporPressure.pdf

%
%Alternative
%ref:  PAPERS/manuals_userguides_etc/Humidity_Conversion_Formulas_B210973EN-F.pdf 
%OR %http://www.vaisala.com/Vaisala%20Documents/Application%20notes/Humidity_Conversion_Formulas_B210973EN-F.pdf


%in kPA (Temperature must be in C)
%The vapor pressure answers will be in units of millibars (mb) or hectoPascals (hPa)
%converting to kilopascals(divide by 10);
"""
    T = np.asarray(temperature_celsius)
    vpsat_kpa = (6.11 * 10**(7.5 * T / (237.3 + T))) / 10
    return vpsat_kpa

# --- Main Execution Block (without def main) ---

if __name__ == "__main__":
    # This demonstration code is now placed directly inside the 'if' block.
    # It will still only run when the script is executed directly.
    print("--- Running Example Calculations ---")
    
    # 1. Calculate for a single temperature
    temp_single = 20
    vpsat_single = h2o_saturation_pressure(temp_single)
    print(f"\nFor a temperature of {temp_single}°C:")
    print(f"Water Vapor Saturation Pressure is {vpsat_single:.4f} kPa")

    # 2. Calculate for a list of temperatures
    temp_list = [-10, 0, 10, 20, 30]
    vpsat_list = h2o_saturation_pressure(temp_list)
    print(f"\nFor temperatures of {temp_list}°C:")
    print(f"Pressures are {np.round(vpsat_list, 4)} kPa")
