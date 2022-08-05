import warnings
from netCDF4 import Dataset
import argparse
import sys
import numpy

#checks if any observation variable attribute opt_unc_type ==1 and (single_unc_value <1 or any value <=0)
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
        # check opt_filter             
        if hasattr(current_var, 'opt_filter'): 
            opt_filter_value = getattr(current_var, 'opt_filter')
            if hasattr(current_var, 'single_mean_unc'):
                single_mean_unc=getattr(current_var, 'single_mean_unc')
            # check if opt_filter ==1 and single_mean_unc is -9999 or NaN
                if opt_filter_value==1 and single_mean_unc==-9999:
                    print("Error: Variable " + current_name + " has invalid combination of opt_filter=" + str(opt_filter_value) + " and single_mean_unc="+ str(single_mean_unc))
                    obs_attribute_errors = obs_attribute_errors + 1
                if opt_filter_value==1 and numpy.isnan(single_mean_unc)==True:
                    print("Error: Variable " + current_name + " has invalid combination of opt_filter=" + str(opt_filter_value) + " and single_mean_unc="+ str(single_mean_unc))
                    obs_attribute_errors = obs_attribute_errors + 1
            # check if opt_filter ==2 and single_annual_unc is -9999 or NaN
            if hasattr(current_var, 'single_annual_unc'):    
                single_annual_unc=getattr(current_var, 'single_annual_unc')
                if opt_filter_value==2 and single_annual_unc==-9999:
                    print("Error: Variable " + current_name + " has invalid combination of opt_filter=" + str(opt_filter_value) + " and single_annual_unc="+ str(single_annual_unc))
                    obs_attribute_errors = obs_attribute_errors + 1
                if opt_filter_value==2 and numpy.isnan(single_annual_unc)==True:
                    print("Error: Variable " + current_name + " has invalid combination of opt_filter="+ str(opt_filter_value) + " and single_annual_unc="+ str(single_annual_unc))
                    obs_attribute_errors = obs_attribute_errors + 1
            # check if opt_filter ==2 and single_monthly_unc is -9999 or NaN
            if hasattr(current_var, 'single_monthly_unc'):    
                single_monthly_unc=getattr(current_var, 'single_monthly_unc') 
                if opt_filter_value==2 and single_monthly_unc==-9999:
                    print("Error: Variable " + current_name + " has invalid combination of opt_filter="+ str(opt_filter_value) + " and single_monthly_unc=" +str(single_monthly_unc))
                    obs_attribute_errors = obs_attribute_errors + 1
                if opt_filter_value==2 and numpy.isnan(single_monthly_unc)==True:
                    print("Error: Variable " + current_name + " has invalid combination of opt_filter=" + str(opt_filter_value) + " and single_monthly_unc=" + str(single_monthly_unc))
                    obs_attribute_errors = obs_attribute_errors + 1
    return obs_attribute_errors

# check all variables and their attribute are type double    
def vars_attributes_datatype_checks(CBF):
    vars_datatype_errors = 0
    toexclude = ['info', 'units']    
    for name, variable in CBF.variables.items():   
        current_var = variable
        current_name = name
        temp_data = CBF[name]
        if (not temp_data.datatype=='double'):
            vars_datatype_errors = vars_datatype_errors + 1
            print("Error: Variable " + current_name + " is not of type double")
            vars_datatype_errors = vars_datatype_errors + 1
            
        for attrname in variable.ncattrs():
            if attrname not in toexclude:
                temp_attr_data = getattr(variable, attrname)
                if (not isinstance(temp_attr_data,float)):
                    print("Error: Variable " + current_name + " has attribute " + attrname + " that is not of type double")  
                    vars_datatype_errors = vars_datatype_errors + 1
        #check info and units to be string type 
            else:
                temp_attr_data = getattr(variable, attrname)
                if (not isinstance(temp_attr_data,str)):
                    print("Error: Variable " + current_name + " has attribute " + attrname + " that is not of type string")  
                    vars_datatype_errors = vars_datatype_errors + 1
    return vars_datatype_errors
    
#checks if any obsevation var is <=0 and not -9999 
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
        warnings.warn('No variable "{}" in CBF file\n'.format(var))
        return 0
        
#checks if any obsevation mean var is <=0 and not -9999
def check_mean_variable(CBF,var):
    if var in CBF.variables:
        print("Checking CBF OTHER OBS {} is positive or valid missing value".format(var))
        temp = CBF[var][:]
        if (temp <=0):
            if (temp != -9999):
                warnings.warn('CBF OTHER OBS {} is negative, file will make CARDAMOM crash'.format(var))
                return 1
            else: 
                print('OK\n')
                return 0
        else: 
            print('OK\n')
            return 0
    else:
        warnings.warn('No variable "{}" in CBF file\n'.format(var))
        return 0

def obs_checks(CBF):
    obs_flag = 0
    for var in ["GPP","LAI","ABGB","ET","SOM","CH4"]:
        obs_flag += check_variable(CBF,var)
    for var in ["Mean_Biomass","Mean_Fire","Mean_LAI","Mean_GPP"]:
        obs_flag += check_mean_variable(CBF,var)
    return obs_flag

def valid_value_check(temp_data):
    #checks for nan and inf 
    flag_valid_data = 0
    ar_nan = numpy.argwhere(numpy.isnan(temp_data))
    ar_inf = numpy.argwhere(numpy.isinf(temp_data))
    if (len(ar_nan)==0 and len(ar_inf)==0):
        flag_valid_data = 1
    return flag_valid_data

def valid_negativevalue_check(temp_data):
    #checks for nan and inf 
    flag_valid_negative_data = 0
    temp_data[temp_data == -9999] = 9999
    ar_negative = any(numpy.array(temp_data) < 0)
    if (ar_negative==False):
        flag_valid_negative_data = 1
    return flag_valid_negative_data

#checks if any driver var is NaN or Inf/-Inf 
def check_driver_variable(CBF,var):
    if var in CBF.variables:
        print("Checking CBF Driver {} is positive or valid missing value".format(var))
        temp = CBF[var][:]
        flag_non_na_inf = valid_value_check(temp)
        if flag_non_na_inf ==0:
            warnings.warn("CBF Driver {} contains Nan or Inf values, file will make CARDAMOM crash".format(var))
            return 1
        else:
            flag_non_negative = valid_negativevalue_check(temp)
            if flag_non_negative ==0:
                warnings.warn("Driver {} not within physical range, file will make CARDAMOM crash".format(var))
                return 1
            else:
                print('OK\n')
                return 0
    else:
        warnings.warn('No variable "{}" in CBF file\n'.format(var))
        return 0  
        
def check_temperature_vars(CBF,var):
    if var in CBF.variables:
        print("Checking CBF Driver {} is valid value".format(var))
        temp = CBF[var][:]
        flag_non_na_inf = valid_value_check(temp)
        if flag_non_na_inf ==0:
            warnings.warn("CBF Driver {} contains Nan or Inf values, file will make CARDAMOM crash".format(var))
            return 1
        else:
            #check for negative kelvin temp
            temp[temp == -9999] = 9999        
            ar_negative = any((numpy.array(temp)+273.15) < 0) #convert to kelvin and check anything < 0 kelvin
            if (ar_negative==False):
                print("OK\n")
                return 0 #no addtional warning hence return zero
            else:
                warnings.warn("Driver {} not within physical range, file will make CARDAMOM crash".format(var))
                return 1
    else:
        warnings.warn('No variable "{}" in CBF file\n'.format(var))
        return 0  

def driver_checks(CBF):
    driver_flag = 0
    for var in ["BURNED_AREA","CO2","DOY","TOTAL_PREC","SNOWFALL","SSRD","VPD"]:
        driver_flag += check_driver_variable(CBF,var)
    #end for loop        
    #check separately for "T2M_MIN","T2M_MAX" as they contain negative celsius values 
    driver_flag += check_temperature_vars(CBF,"T2M_MIN")
    driver_flag += check_temperature_vars(CBF,"T2M_MAX")
    
    return driver_flag
    
def ID_checks(CBF,var):
    k=0
    print("Checking CBF id is non zero...")
    if var in CBF.variables:
        if CBF[var]==0:
            warnings.warn("CBF ID = 0, file will make CARDAMOM crash")
            k = k + 1
    else:
        warnings.warn('No variable "{}" in CBF file\n'.format(var))
    return k
# end CBF ID 

def time_var_checks(CBF,var):
    k=0 
    print("Checking time field is in increasing order...")
    if var in CBF.variables:
        flag_time = valid_value_check(CBF[var])
        if flag_time ==1:
            diff_timestep = CBF[var][1] - CBF[var][0]
            if (diff_timestep<0):
                warnings.warn("time value not in increasing order")
                k = k + 1
        else:
            warnings.warn("time variable contains NaN or Inf values")
            k = k + 1
    else:
        warnings.warn('No variable "{}" in CBF file\n'.format(var))
    return k 

def check_EDC_range(CBF,var):
    if var in CBF.variables:
        EDC = CBF[var]
        if EDC not in range(0,2):
            print("Error: EDC value is neither 0 nor 1")
    else:
        warnings.warn('No variable "{}" in CBF file\n'.format(var))

def singleval_vars_attribute_check(CBF):
    vars_unc_errors = 0
    toexclude = ['ID','EDC','LAT','MCMCID']
    for name, variable in CBF.variables.items():   
        current_var = variable
        current_name = name
        if name not in toexclude:        
            temp_data = CBF[name][:]
            if temp_data.size==1 and temp_data!= -9999 and valid_value_check(temp_data)==1: 
                if hasattr(current_var, 'unc'): 
                    unc_value = getattr(current_var, 'unc')
                    if unc_value==-9999:
                        print("Error: Variable " + current_name + " has invalid combination of length==1 and unc equal to -9999")
                        vars_unc_errors = vars_unc_errors + 1
                    elif numpy.isnan(unc_value)==True or numpy.isinf(unc_value)==True:       
                        print("Error: Variable " + current_name + " has invalid combination of length==1 and unc equal to Inf or NaN")
                        vars_unc_errors = vars_unc_errors + 1                            
                else:
                    print("Error: Variable " + current_name + " has invalid combination of length==1 and missing attribute unc")
                    vars_unc_errors = vars_unc_errors + 1                    
    return vars_unc_errors

def check_fillvalue(CBF):
    vars_fillVal_errors=0
    for name, variable in CBF.variables.items():   
        current_var = variable
        current_name = name
        temp_data = CBF[name]#[:]
        if hasattr(current_var, '_FillValue'):
            current_FillVal=temp_data._FillValue    
            if current_FillVal!=-9999:
                vars_fillVal_errors=vars_fillVal_errors+1
                print("Error: Variable " + current_name + " has invalid fill value and not equal to -9999")
    return vars_fillVal_errors

def main():

    parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter, description = "This tool checks netcdf cbf files")
    parser.add_argument("--cbf_file", "-c", help="Path to the netcdf cbf output file to test",default=None)

    # read arguments from the command line
    args = parser.parse_args()
    error_message_summary='';
    total_errors=0;
    
    print("\n*******CARDAMOM cbf.nc file error check********")
    print(args.cbf_file)
    dataset=Dataset(args.cbf_file, "r")
    dataset.set_auto_mask(False)
    
    print("ID checks")
    k = ID_checks(dataset,"ID")
    if k>0:
        print("ID check failed with total " + str(k) + " warning(s)")
        error_message_summary = error_message_summary + "\n ID check failed"
        total_errors=total_errors+k
        
    print("time checks")
    k = time_var_checks(dataset,"time")
    if k>0:
        print("time check failed with total " + str(k) + " warning(s)")
        error_message_summary = error_message_summary + "\n time check failed with total " + str(k) + " warning(s)"
        total_errors=total_errors+k
        
    print("OBS checks")    
    obs_flag=obs_checks(dataset)
    if obs_flag>0:
        print("OBS check failed with total " + str(obs_flag) + " warning(s)") 
        error_message_summary = error_message_summary + "\n OBS check failed with total " + str(obs_flag) + " warning(s)"
        total_errors=total_errors+obs_flag

    obs_attribute_flag = obs_attributes_checks(dataset)
    if obs_attribute_flag>0:
        print("OBS attribute check failed with total " + str(obs_attribute_flag) + " error(s)") 
        error_message_summary = error_message_summary + "\n OBS attribute check failed with total " + str(obs_attribute_flag) + " error(s)"
        total_errors=total_errors+obs_attribute_flag
        
    print("Driver checks")
    driver_flag = driver_checks(dataset)
    if driver_flag>0:
        print("Driver check failed with total " + str(driver_flag) + " warning(s)") 
        error_message_summary = error_message_summary + "\n Driver check failed with total " + str(driver_flag) + " warning(s)"
        total_errors=total_errors+driver_flag
    #datatype_flag = vars_attributes_datatype_checks(dataset)
    # commented for now 
    #if datatype_flag>0:
    #    print("Datatype check failed with total " + str(datatype_flag) + " error(s)") 
    
    print("\nunc attribute check for single length variables")
    unc_flag= singleval_vars_attribute_check(dataset)
    if unc_flag>0:
        print("unc attribute check failed with total " + str(unc_flag) + " error(s)") 
        error_message_summary = error_message_summary + "\n unc attribute check failed with total " + str(unc_flag) + " error(s)"
        total_errors=total_errors+unc_flag
    else:
        print("OK")
    
    print("\nfill value check")
    fill_value_flag= check_fillvalue(dataset)
    if fill_value_flag>0:
        print("fill value check failed with total " + str(fill_value_flag) + " error(s)") 
        error_message_summary = error_message_summary + "\n fill value check failed with total " + str(fill_value_flag) + " error(s)"
        total_errors=total_errors+fill_value_flag
    else:
        print("OK")
    
    if len(error_message_summary)>0:
        print("\nError Summary: Total " + str(total_errors) + " Errors/Warnings Found. See below:" + error_message_summary)
        
    dataset.close()
    print("\n***Other possible error sources***")
    print("Tip: check that all met fields required for model ID are provided in cbf.nc file")
#next checks here
# end main 

if __name__ == "__main__":
    main()

