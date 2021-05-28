function CARDAMOM_CHECK_CBF_FOR_ERRORS(CBF)


disp('Met checks')
okcheck=met_checks(CBF);
if okcheck==0; warning('Met check failed, see warnings/errors above!');end
%next checks here

end

function okcheck=met_checks(CBF)

okcheck=1;

if CBF.ID==0;warning('CBF ID = 0, file will make CARDAMOM crash');end


sprintf('Checking minimum temperature is within range...');
if (min(CBF.MET(:,2))<-273.15)
	warning('Minimum temperature not within physical range');
	okcheck=0;
else
	sprintf('OK!\n');
end

if (min(CBF.MET(:,3))<-273.15)
	warning('Maximum temperature not within physical range')
end 

if (min(CBF.MET(:,4))<0)
	warning('solar radiation not within physical range')
end

if (min(CBF.MET(:,5))<0)
	warning('CO2 concentration not within physical range')
end

if (min(CBF.MET(:,6))<0)
	warning('Day of year not within physical range')
end

if (min(CBF.MET(:,7))<0)
	warning('burned area value not within physical range')
end

if (min(CBF.MET(:,8))<0)
	warning('VPD value not within physical range')
end

if (min(CBF.MET(:,9))<0)
	warning('Precip value not within physical range')
end

end
