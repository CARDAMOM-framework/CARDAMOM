function CARDAMOM_CHECK_CBF_FOR_ERRORS(CBF)

fprintf('Met checks')
k=met_checks(CBF);
if k>0
	%warning('Met check failed, see warnings/errors above!');
        fprintf(sprintf('Met check failed with total %d warning(s) \n',k))
end

fprintf('OBS checks')
obs_flag=obs_checks(CBF);
if obs_flag>0
        fprintf(sprintf('OBS check failed with total %d warning(s) \n',obs_flag))
end

%next checks here
end

function obs_flag=obs_checks(CBF)
obs_flag=0;
fprintf('Checking CBF OBS GPP is positive or valid missing value\n')
if (sum(CBF.OBS.GPP<=0 & CBF.OBS.GPP~=-9999)>0)
        warning('CBF OBS GPP is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end

fprintf('Checking CBF OBS LAI is positive or valid missing value\n')
if (sum(CBF.OBS.LAI<=0 & CBF.OBS.LAI~=-9999)>0)
        warning('CBF OBS LAI is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end


fprintf('Checking CBF OBS ABGB is positive or valid missing value\n')
if (sum(CBF.OBS.ABGB<=0 & CBF.OBS.ABGB~=-9999)>0)
        warning('CBF OBS ABGB is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end

fprintf('Checking CBF OBS ET is positive or valid missing value\n')
if (sum(CBF.OBS.ET<=0 & CBF.OBS.ET~=-9999)>0)
        warning('CBF OBS ET is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end

fprintf('Checking CBF OBS SOM is positive or valid missing value\n')
if (sum(CBF.OBS.SOM<=0 & CBF.OBS.SOM~=-9999)>0)
        warning('CBF OBS SOM is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end


fprintf('Checking CBF OBS CH4 is positive or valid missing value\n')
if (sum(CBF.OBS.CH4<=0 & CBF.OBS.CH4~=-9999)>0)
        warning('CBF OBS CH4 is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end


fprintf('Checking CBF OTHER OBS MBiomass is positive or valid missing value\n')
if ((CBF.OTHER_OBS.MBiomass.mean<=0 & CBF.OTHER_OBS.MBiomass.mean~=-9999)>0)
        warning('CBF OTHER_OBS MBiomass is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end


fprintf('Checking CBF OTHER OBS MFire is positive or valid missing value\n')
if ((CBF.OTHER_OBS.MFire.mean<=0 & CBF.OTHER_OBS.MFire.mean~=-9999)>0)
        warning('CBF OTHER_OBS MFire is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end


fprintf('Checking CBF OTHER OBS MLAI is positive or valid missing value\n')
if ((CBF.OTHER_OBS.MLAI.mean<=0 & CBF.OTHER_OBS.MLAI.mean~=-9999)>0)
        warning('CBF OTHER_OBS MLAI is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end


fprintf('Checking CBF OTHER OBS MGPP is positive or valid missing value\n')
if ((CBF.OTHER_OBS.MGPP.mean<=0 & CBF.OTHER_OBS.MGPP.mean~=-9999)>0)
        warning('CBF OTHER_OBS MGPP is negative, file will make CARDAMOM crash');
        obs_flag = obs_flag + 1; else; fprintf('OK\n');
end


end 

function k=met_checks(CBF)
%OK\ncheck=1;
k=0;

fprintf('Checking CBF id is non zero...')
if CBF.ID==0;
	warning('CBF ID = 0, file will make CARDAMOM crash');
	k = k + 1; else; fprintf('OK\n');
end

fprintf('Checking minimum temperature is within range...')
if (min(CBF.MET(:,2))<-273.15)
	warning('Minimum temperature not within physical range');
	k = k + 1; else; fprintf('OK\n');
%else
%	sprintf('OK\n!\n');
end

fprintf('Checking maximum temperature is within range...')
if (min(CBF.MET(:,3))<-273.15)
	warning('Maximum temperature not within physical range')
	k = k + 1; else; fprintf('OK\n');
end 

fprintf('Checking solar radiation is within range...');
if (min(CBF.MET(:,4))<0)
	warning('solar radiation not within physical range')
	k = k + 1; else; fprintf('OK\n');
end

fprintf('Checking CO2 concentration is within range...');
if (min(CBF.MET(:,5))<0)
	warning('CO2 concentration not within physical range')
	k = k + 1; else; fprintf('OK\n');
end

fprintf('Checking Day of year is within range...');
if (min(CBF.MET(:,6))<0)
	warning('Day of year not within physical range')
	k = k + 1; else; fprintf('OK\n');
end

fprintf('Checking burned area is within range...');
if (min(CBF.MET(:,7))<0)
	warning('burned area value not within physical range')
	k = k + 1; else; fprintf('OK\n');
end

fprintf('Checking VPD is within range...');
if (min(CBF.MET(:,8))<0)
	warning('VPD value not within physical range')
	k = k + 1; else; fprintf('OK\n');
end

fprintf('Checking Precip is within range...');
if (min(CBF.MET(:,9))<0)
	warning('Precip value not within physical range')
	k = k + 1; else; fprintf('OK\n');
end

end
