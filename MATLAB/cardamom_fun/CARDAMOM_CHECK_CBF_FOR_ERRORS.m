function CARDAMOM_CHECK_CBF_FOR_ERRORS(CBF)


disp('Met checks')
met_error_check =[]
met_error_check.warning_message = '';
met_error_check.no_of_warnings 	= 0;
met_error_check=met_checks(CBF,met_error_check);
%okcheck=met_checks(CBF);
%if okcheck==0; warning('Met check failed, see warnings/errors above!');end

if met_error_check.no_of_warnings > 0 
	disp(sprintf('Met check failed with total %d warning(s)',met_error_check.no_of_warnings)) 
	for warning_counter = 1:met_error_check.no_of_warnings
		warning(char(met_error_check.warnings(warning_counter)))
	end %end displaying warnings for met variables 
else 
	disp('no warnings in met variables')
end 

%next checks here
end

function met_error_check=met_checks(CBF,met_error_check)
%okcheck=1;

disp('Checking CBF id is non zero...')
if CBF.ID==0;
%warning('CBF ID = 0, file will make CARDAMOM crash');
	met_error_check.warning_message = [met_error_check.warning_message,newline,'CBF ID = 0, file will make CARDAMOM crash'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
end

disp('Checking minimum temperature is within range...')
if (min(CBF.MET(:,2))<-273.15)
	met_error_check.warning_message = [met_error_check.warning_message,newline,'Minimum temperature not within physical range'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
	%warning('Minimum temperature not within physical range');
	%okcheck=0;
%else
%	sprintf('OK!\n');
end

disp('Checking maximum temperature is within range...')
if (min(CBF.MET(:,3))<-273.15)
	%warning('Maximum temperature not within physical range')
	met_error_check.warning_message = [met_error_check.warning_message,newline,'Maximum temperature not within physical range'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
end 

disp('Checking solar radiation is within range...');
if (min(CBF.MET(:,4))<0)
	%warning('solar radiation not within physical range')
	met_error_check.warning_message = [met_error_check.warning_message,newline,'solar radiation not within physical range'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
end

disp('Checking CO2 concentration is within range...');
if (min(CBF.MET(:,5))<0)
	%warning('CO2 concentration not within physical range')
	met_error_check.warning_message = [met_error_check.warning_message,newline,'CO2 concentration not within physical range'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
end

disp('Checking Day of year is within range...');
if (min(CBF.MET(:,6))<0)
	%warning('Day of year not within physical range')
	met_error_check.warning_message = [met_error_check.warning_message,newline,'Day of year not within physical range'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
end

disp('Checking burned area is within range...');
if (min(CBF.MET(:,7))<0)
	%warning('burned area value not within physical range')
	met_error_check.warning_message = [met_error_check.warning_message,newline,'burned area value not within physical range'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
end

disp('Checking VPD is within range...');
if (min(CBF.MET(:,8))<0)
	%warning('VPD value not within physical range')
	met_error_check.warning_message = [met_error_check.warning_message,newline,'VPD value not within physical range'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
end

disp('Checking Precip is within range...');
if (min(CBF.MET(:,9))<0)
	%warning('Precip value not within physical range')
	met_error_check.warning_message = [met_error_check.warning_message,newline,'Precip value not within physical range'];
	met_error_check.no_of_warnings = met_error_check.no_of_warnings + 1;
end

temp_str = splitlines(met_error_check.warning_message);
if (met_error_check.no_of_warnings > 0 & size(temp_str,1) == (met_error_check.no_of_warnings+1))
	met_error_check.warnings = temp_str(2:end,:);
else 
	disp('Error occurred during met var testing')
	exit(1);
end 

end
