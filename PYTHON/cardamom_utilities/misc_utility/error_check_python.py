import warnings
from netCDF4 import Dataset
import argparse
import sys
import numpy

def obs_attributes_checks(CBF):
    obs_attribute_errors = 0 
    for name, variable in CBF.variables.items():   
        current_var = variable
        current_name = name 
        if hasattr(current_var, 'opt_unc_type'): 
            opt_unc_type_value = getattr(current_var, 'opt_unc_type')
            if hasattr(current_var, 'single_unc'):
                single_unc_value=getattr(current_var, 'single_unc')
                if opt_unc_type_value==1 and single_unc_value<1:
                    print("Error: Variable " + current_name + " has invalid combination of opt_unc_type==1 and single_unc<1")
                    obs_attribute_errors = obs_attribute_errors + 1
            if opt_unc_type_value==1:
                data=CBF.variables[current_name][:]
                if numpy.any(data)<= 0:
                    print("Error: Variable " + current_name + " has invalid combination of opt_unc_type==1 and values<=0")
                    obs_attribute_errors = obs_attribute_errors + 1
    return obs_attribute_errors

def check_variable(CBF,var):
    if var in CBF.variables:
        print("Checking CBF OBS {} is positive or valid missing value".format(var))
        temp = CBF[var][:]
        subset_temp = temp[temp != -9999]
        result_temp = any(subset_temp <=0)
        if (result_temp is True):
            warnings.warn('CBF OBS {} is negative, file will make CARDAMOM crash'.format(var))
            return 1
        else: 
            print('OK\n')
            return 0
    else:
        print('No variable "{}" in CBF file\n'.format(var))
        return 0

def check_mean_variable(CBF,var):
    if var in CBF.variables:
        print("Checking CBF OTHER OBS {} is positive or valid missing value".format(var))
        temp = CBF[var][:]
        result_temp =  (temp != -9999 and temp <=0)
        if (result_temp is True):
            warnings.warn('CBF OTHER OBS {} is negative, file will make CARDAMOM crash'.format(var))
            return 1
        else: 
            print('OK\n')
            return 0
    else:
        print('No variable "{}" in CBF file\n'.format(var))
        return 0

def obs_checks(CBF):
    obs_flag = 0
    for var in ["GPP","LAI","ABGB","ET","SOM","CH4"]:
        obs_flag += check_variable(CBF,var)
    for var in ["Mean_Biomass","Mean_Fire","Mean_LAI","Mean_GPP"]:
        obs_flag += check_mean_variable(CBF,var)
    return obs_flag

def met_checks(CBF):
    k=0

    print("Checking CBF id is non zero...")
    if CBF['ID']==0:
        warnings.warn("CBF ID = 0, file will make CARDAMOM crash")
        k = k + 1
    # end CBF ID 

    print("Checking minimum temperature is within range...")
    if (min(CBF['T2M_MIN'])<-273.15):
        warnings.warn("Minimum temperature not within physical range")
        k = k + 1
    #end CBF Min temp

    print("Checking maximum temperature is within range...")
    if (min(CBF['T2M_MAX'])<-273.15):
        warnings.warn("Maximum temperature not within physical range")
        k = k + 1
    #end Max temp  

    print("Checking solar radiation is within range...")
    if (min(CBF['SSRD'])<0):
        warnings.warn("solar radiation not within physical range")
        k = k + 1
    #end solar radiation

    print("Checking CO2 concentration is within range...")
    if (min(CBF['CO2'])<0):
        warnings.warn("CO2 concentration not within physical range")
        k = k + 1
    #end CO2

    print("Checking Day of year is within range...")
    if (min(CBF['DOY'])<0):
        warnings.warn("Day of year not within physical range")
        k = k + 1
    #end Day of Year

    print("Checking burned area is within range...")
    if (min(CBF['BURNED_AREA'])<0):
        warnings.warn("burned area value not within physical range")
        k = k + 1
    #end burned area 

    print("Checking VPD is within range...")
    if (min(CBF['VPD'])<0):
        warnings.warn("VPD value not within physical range")
        k = k + 1
    #end VPD

    print("Checking Precip is within range...")
    if (min(CBF['TOTAL_PREC'])<0):
        warnings.warn("Precip value not within physical range")
        k = k + 1
    #end Precip

    return k

def main():

    parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter, description = "This tool checks netcdf cbf files")
    parser.add_argument("--cbf_file", "-c", help="Path to the netcdf cbf output file to test",default=None)

    # read arguments from the command line
    args = parser.parse_args()

    print(args.cbf_file)
    dataset=Dataset(args.cbf_file, "r")
    dataset.set_auto_mask(False)
    
    print("Met checks")
    k = met_checks(dataset)
    if k>0:
        print("Met check failed with total " + str(k) + " warning(s)")
    #end met checks

    print("OBS checks")    
    obs_flag=obs_checks(dataset)
    if obs_flag>0:
        print("OBS check failed with total " + str(obs_flag) + "warning(s)") 

    obs_attribute_flag = obs_attributes_checks(dataset)
    if obs_attribute_flag>0:
        print("OBS attribute check failed with total " + str(obs_attribute_flag) + "error(s)") 
    
#next checks here
# end main 

if __name__ == "__main__":
    main()

