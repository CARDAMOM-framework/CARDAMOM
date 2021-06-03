function CARDAMOM_CHECK_CBF_FOR_ERRORS(CBF)

disp('Met checks')
k=met_checks(CBF);
if k>0
	%warning('Met check failed, see warnings/errors above!');
        disp(sprintf('Met check failed with total %d warning(s)',k))
end

%next checks here
end

function k=met_checks(CBF)
%okcheck=1;
k=0;

disp('Checking CBF id is non zero...')
if CBF.ID==0;
	warning('CBF ID = 0, file will make CARDAMOM crash');
	k = k + 1;
end

disp('Checking minimum temperature is within range...')
if (min(CBF.MET(:,2))<-273.15)
	warning('Minimum temperature not within physical range');
	k = k + 1;
%else
%	sprintf('OK!\n');
end

disp('Checking maximum temperature is within range...')
if (min(CBF.MET(:,3))<-273.15)
	warning('Maximum temperature not within physical range')
	k = k + 1;
end 

disp('Checking solar radiation is within range...');
if (min(CBF.MET(:,4))<0)
	warning('solar radiation not within physical range')
	k = k + 1;
end

disp('Checking CO2 concentration is within range...');
if (min(CBF.MET(:,5))<0)
	warning('CO2 concentration not within physical range')
	k = k + 1;
end

disp('Checking Day of year is within range...');
if (min(CBF.MET(:,6))<0)
	warning('Day of year not within physical range')
	k = k + 1;
end

disp('Checking burned area is within range...');
if (min(CBF.MET(:,7))<0)
	warning('burned area value not within physical range')
	k = k + 1;
end

disp('Checking VPD is within range...');
if (min(CBF.MET(:,8))<0)
	warning('VPD value not within physical range')
	k = k + 1;
end

disp('Checking Precip is within range...');
if (min(CBF.MET(:,9))<0)
	warning('Precip value not within physical range')
	k = k + 1;
end

end
