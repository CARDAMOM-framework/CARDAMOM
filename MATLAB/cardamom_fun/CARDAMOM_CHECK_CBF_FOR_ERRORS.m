function CARDAMOM_CHECK_CBF_FOR_ERRORS(CBF)

if ischar(CBF)==0
    cbffile='DUMPFILES/test_for_errors_temp_file.cbf.nc';
    CARDAMOM_WRITE_NC_CBF_FILE(CBF,cbffile);
    
else
    cbffile=CBF;
end


    
    commandstr=sprintf('python3 CARDAMOM/PYTHON/cardamom_utilities/misc_utility/error_check_python.py --cbf_file %s',cbffile);
    
    disp('Copy this command to terminal window')
    disp(commandstr)
    
    
    
    
end
