
%Notes: https://www.unidata.ucar.edu/software/netcdf/docs/netcdf_data_set_components.html

%- Scalar variables don't need dimension definitions
function TEST_CARDAMOM_CONVERT_CBF_TO_NC_FORMAT(CBF,fname)

if nargin==0
% Step 1. Read CBF file
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');
end

if isstr(CBF);CBF=CARDAMOM_READ_BINARY_FILEFORMAT(CBF);end


% %Guarantees CBF.RAW available (soon to be decomissioned).
CARDAMOM_WRITE_BINARY_FILEFORMAT(CBF,'testnetcdf.cbf')
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('testnetcdf.cbf');
% 

%Dimensions
%nopars
%nodays
%Number of timesteps
nodays=size(CBF.MET,1);
%Number of met fields
nomet=size(CBF.MET,2);


%Step 2. write as netcdf file

if nargin<2
fname='CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE_beta_v2.cbf.nc';delete(fname)
end


    if isempty(dir(fname))==0;warning(sprintf('%s\nFile exists',fname));end





%*******Write CBF structure fields to .nc file**********

%Setting/testing convention here: pass global "attributes" as variables,
%unless completely self-explanatory...

%IMPORTANT NOTE ON NAMES: Do NOT use space charicters in the names of
%variables or attributes. netCDF does not have well defined behavior around
%space chars, and they should be avoided at all costs. 
%For instance, when defining a variable and string attribute, this is ok: 
    %nccreate(fname,'My_Variable_Name');
    %ncwriteatt(fname,'My_Variable_Name','My_Variable_Attribute_Name', 'This text is the value of the attribute My_Variable_Attribute_Name of variable My_Variable_Name')
%These are NOT ok:
    %nccreate(fname,'My Variable Name');
    %ncwriteatt(fname,'My Variable Name','My Variable Attribute Name', 'This text is the value of the attribute My Variable Attribute Name of variable My Variable Name')
    %ncwriteatt(fname,'My_Variable_Name','My Variable Attribute Name', 'This text is the value of the attribute My Variable Attribute Name of variable My_Variable_Name')
    %ncwriteatt(fname,'My Variable Name','My_Variable_Attribute_Name', 'This text is the value of the attribute My_Variable_Attribute_Name of variable My Variable Name')


%Global attributes
nccreate(fname,'ID'); ncwrite(fname,'ID',CBF.ID)

nccreate(fname,'LAT'); ncwrite(fname,'LAT',CBF.LAT)

nccreate(fname,'EDC'); ncwrite(fname,'EDC',CBF.EDC)

% nccreate(fname,'EDCDIAG'); ncwrite(fname,'EDCDIAG',0)

nccreate(fname,'MCMCID');
ncwrite(fname,'MCMCID',119);
ncwriteatt(fname,'MCMCID','nITERATIONS',10000);
ncwriteatt(fname,'MCMCID','nSAMPLES',2000);
ncwriteatt(fname,'MCMCID','nPRINT',1000);
ncwriteatt(fname,'MCMCID','seed_number',0.0);


% nccreate(fname,'lat','FillValue',NaN); ncwrite(fname,'lat',CBF.LAT)
% ncwriteatt(fname,'lat','long_name','Latitude of Grid Cell Centers')
% ncwriteatt(fname,'lat','standard_name','latitude')
% ncwriteatt(fname,'lat','units','degrees_north')
% ncwriteatt(fname,'lat','axis','Y')
% ncwriteatt(fname,'lat','valid_min','-90.0')
% ncwriteatt(fname,'lat','valid_max','90.0')
% ncwriteatt(fname,'lat','bounds','lat_bnds')
% 
% 
% nccreate(fname,'lon','FillValue',NaN);
% if isfield(CBF,'LON')==1
%     ncwrite(fname,'lon',CBF.LON)
% else
%     ncwrite(fname,'lon',NaN)
% end
% ncwriteatt(fname,'lon','long_name','Longitude of Grid Cell Centers')
% ncwriteatt(fname,'lon','standard_name','longitude')
% ncwriteatt(fname,'lon','units','degrees_east')
% ncwriteatt(fname,'lon','axis','Y')
% ncwriteatt(fname,'lon','modulo','360.0')
% ncwriteatt(fname,'lon','topology','circular')
% ncwriteatt(fname,'lon','valid_min','-180.0')
% ncwriteatt(fname,'lon','valid_max','180.0')
% ncwriteatt(fname,'lon','bounds','lon_bnds')

%FIX: need this as dimension
%nccreate(fname,'nodays'); ncwrite(fname,'nodays',nodays)


% %Adjust number as needed. Not urgent for field to dynamically vary
% nccreate(fname,'PARPRIORS','Dimensions',{'nopars',50},'FillValue',NaN); 
% ncwrite(fname,'PARPRIORS',CBF.PARPRIORS)
% nccreate(fname,'PARPRIORUNC','Dimensions',{'nopars'},'FillValue',NaN);
% ncwrite(fname,'PARPRIORUNC',CBF.PARPRIORUNC)
% 
% %Adjust number as needed. Not urgent for field to dynamically vary
% nccreate(fname,'OTHERPRIORS','Dimensions',{'nopars',50},'FillValue',NaN); 
% ncwrite(fname,'OTHERPRIORS',CBF.RAW.OTHERPRIORS)
% nccreate(fname,'OTHERPRIORSUNC','Dimensions',{'nopars'},'FillValue',NaN);
% ncwrite(fname,'OTHERPRIORSUNC',CBF.RAW.OTHERPRIORSUNC)



%Other obs
%Mean biomass
nccreate(fname,'Mean_Biomass','FillValue',-9999);
ncwrite(fname,'Mean_Biomass',CBF.OTHER_OBS.MBiomass.mean)
ncwriteatt(fname,'Mean_Biomass','unc',CBF.OTHER_OBS.MBiomass.unc)
ncwriteatt_if_exists_else_fill(fname,'Mean_Biomass','ignore','opt_unc_type',CBF.OTHER_OBS.MBiomass,-9999)
ncwriteatt_if_exists_else_fill(fname,'Mean_Biomass','ignore','min_threshold',CBF.OTHER_OBS.MBiomass,-9999)

%Mean fire
nccreate(fname,'Mean_Fire','FillValue',-9999); 
ncwrite(fname,'Mean_Fire',CBF.OTHER_OBS.MFire.mean)
ncwriteatt(fname,'Mean_Fire','unc',CBF.OTHER_OBS.MFire.unc)
ncwriteatt_if_exists_else_fill(fname,'Mean_Fire','ignore','opt_unc_type',CBF.OTHER_OBS.MBiomass,-9999)
ncwriteatt_if_exists_else_fill(fname,'Mean_Fire','ignore','min_threshold',CBF.OTHER_OBS.MBiomass,-9999)

%Mean LAI
nccreate(fname,'Mean_LAI','FillValue',-9999); 
ncwrite(fname,'Mean_LAI',CBF.OTHER_OBS.MLAI.mean)
ncwriteatt(fname,'Mean_LAI','unc',CBF.OTHER_OBS.MLAI.unc)
ncwriteatt_if_exists_else_fill(fname,'Mean_LAI','ignore','opt_unc_type',CBF.OTHER_OBS.MLAI,-9999)
ncwriteatt_if_exists_else_fill(fname,'Mean_LAI','ignore','min_threshold',CBF.OTHER_OBS.MLAI,-9999)

%Mean GPP
nccreate(fname,'Mean_GPP','FillValue',-9999); 
ncwrite(fname,'Mean_GPP',CBF.OTHER_OBS.MGPP.mean)
ncwriteatt(fname,'Mean_GPP','unc',CBF.OTHER_OBS.MGPP.unc)
ncwriteatt_if_exists_else_fill(fname,'Mean_GPP','ignore','opt_unc_type',CBF.OTHER_OBS.MGPP,-9999)
ncwriteatt_if_exists_else_fill(fname,'Mean_GPP','ignore','min_threshold',CBF.OTHER_OBS.MGPP,-9999)


%Time-varing obs

%********GPP*****************

nccreate(fname,'GPP','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'GPPunc','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'GPP',CBF.OBS.GPP);
%Variable attributes
ncwriteatt(fname,'GPP','opt_unc_type',-9999)
ncwriteatt(fname,'GPP','opt_normalization',-9999)
ncwriteatt(fname,'GPP','opt_filter',-9999)
ncwriteatt(fname,'GPP','single_monthly_unc',-9999)
ncwriteatt(fname,'GPP','single_mean_unc',-9999)
ncwriteatt(fname,'GPP','structural_unc',-9999)
ncwriteatt_if_exists_else_fill(fname,'GPP','unc','single_unc',CBF.OBSUNC.GPP,-9999)
if isfield(CBF.OBSUNC.GPP,'gppabs')
    if CBF.OBSUNC.GPP.gppabs == 0
        ncwriteatt(fname,'GPP','opt_normalization',2)
    elseif CBF.OBSUNC.GPP.gppabs == 1
        ncwriteatt(fname,'GPP','opt_normalization',0)
    end
else
    ncwriteatt(fname,'GPP','opt_normalization',-9999)
end
ncwriteatt_if_exists_else_fill(fname,'GPP','obs_unc_threshold','min_threshold',CBF.OBSUNC.GPP,-9999); % double check


%Endless list, up to users to decide what goes here (e.g. source data?)
%Same as standard CBF variables
% ncwriteatt(fname,'GPP','Uncertainty',CBF.OBSUNC.GPP.unc)
% ncwriteatt(fname,'GPP','Annual_Uncertainty',CBF.OBSUNC.GPP.annual_unc)

%********LAI*****************

nccreate(fname,'LAI','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'LAIunc','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'LAI',CBF.OBS.LAI);
% ncwriteatt(fname,'LAI','LAI_data_requirement',CBF.OBSinfo.LAI)
%Variable attribute
% ncwriteatt(fname,'LAI','Description','Leaf Area Index')
ncwriteatt(fname,'LAI','opt_unc_type',-9999)
ncwriteatt(fname,'LAI','opt_normalization',-9999)
ncwriteatt(fname,'LAI','opt_filter',-9999)
ncwriteatt(fname,'LAI','single_monthly_unc',-9999)
ncwriteatt(fname,'LAI','single_mean_unc',-9999)
ncwriteatt(fname,'LAI','structural_unc',-9999)
if isfield(CBF.OBSUNC,'LAI')
    ncwriteatt(fname,'LAI','single_unc',CBF.OBSUNC.LAI.unc)
    if CBF.OBSUNC.LAI.annual_unc == -9999
        ncwriteatt_if_exists_else_fill(fname,'LAI','seasonal_unc','single_unc',CBF.OBSUNC.LAI,-9999)
        ncwriteatt_if_exists_else_fill(fname,'LAI','annual_unc','single_annual_unc',CBF.OBSUNC.LAI,-9999)
    else
        ncwriteatt_if_exists_else_fill(fname,'LAI','seasonal_unc','single_monthly_unc',CBF.OBSUNC.LAI,-9999)
        ncwriteatt_if_exists_else_fill(fname,'LAI','annual_unc','single_annual_unc',CBF.OBSUNC.LAI,-9999)
    end
    ncwriteatt_if_exists_else_fill(fname,'LAI','obs_unc_threshold','min_threshold',CBF.OBSUNC.LAI,-9999); % double check
else
    ncwriteatt(fname,'LAI','single_unc',-9999)
    ncwriteatt(fname,'LAI','single_monthly_unc',-9999)
    ncwriteatt(fname,'LAI','single_annual_unc',-9999)
    ncwriteatt(fname,'LAI','min_threshold',-9999)
end


%********NBE*****************

nccreate(fname,'NBE','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'NBEunc','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'NBE',CBF.OBS.NBE);
%Variable attributes
ncwriteatt(fname,'NBE','Description','Net Biospheric Exchange')
ncwriteatt(fname,'NBE','info',CBF.OBSUNC.NBE.info)
%Same as standard CBF variables
ncwriteatt(fname,'NBE','opt_unc_type',-9999)
ncwriteatt(fname,'NBE','opt_normalization',-9999)
ncwriteatt(fname,'NBE','opt_filter',-9999)
ncwriteatt(fname,'NBE','single_monthly_unc',-9999)
ncwriteatt(fname,'NBE','single_mean_unc',-9999)
% ncwriteatt(fname,'NBE','single_unc',CBF.OBSUNC.NBE.unc)
ncwriteatt_if_exists_else_fill(fname,'NBE','unc','single_unc',CBF.OBSUNC.NBE,-9999)
ncwriteatt(fname,'NBE','structural_unc',-9999)
if CBF.OBSUNC.NBE.annual_unc == -9999
    ncwriteatt_if_exists_else_fill(fname,'NBE','seasonal_unc','single_unc',CBF.OBSUNC.NBE,-9999)
    ncwriteatt_if_exists_else_fill(fname,'NBE','annual_unc','single_annual_unc',CBF.OBSUNC.NBE,-9999)
else
    ncwriteatt_if_exists_else_fill(fname,'NBE','seasonal_unc','single_monthly_unc',CBF.OBSUNC.NBE,-9999)
    ncwriteatt_if_exists_else_fill(fname,'NBE','annual_unc','single_annual_unc',CBF.OBSUNC.NBE,-9999)
end
ncwriteatt_if_exists_else_fill(fname,'NBE','obs_unc_threshold','min_threshold',CBF.OBSUNC.NBE,-9999); % double check


%********ABGB*****************

nccreate(fname,'ABGB','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'ABGBunc','Dimensions',{'time',nodays},'FillValue',-9999);
% ncwriteatt(fname,'ABGB','Description','Above- and below-ground biomass')
% ncwriteatt(fname,'ABGB','info',CBF.OBSUNC.ABGB.info)
%Writing variable 
ncwrite_if_exists(fname,'ABGB',CBF.OBS.ABGB);
ncwriteatt(fname,'ABGB','opt_unc_type',-9999)
ncwriteatt(fname,'ABGB','opt_normalization',-9999)
ncwriteatt(fname,'ABGB','opt_filter',-9999)
ncwriteatt(fname,'ABGB','single_monthly_unc',-9999)
ncwriteatt(fname,'ABGB','single_mean_unc',-9999)
ncwriteatt_if_exists_else_fill(fname,'ABGB','unc','single_unc',CBF.OBSUNC.ABGB,-9999)
ncwriteatt(fname,'ABGB','structural_unc',-9999)
if isfield(CBF.OBSUNC.ABGB,'annual_unc')
    if CBF.OBSUNC.ABGB.annual_unc == -9999
        ncwriteatt_if_exists_else_fill(fname,'ABGB','seasonal_unc','single_unc',CBF.OBSUNC.ABGB,-9999)
        ncwriteatt_if_exists_else_fill(fname,'ABGB','annual_unc','single_annual_unc',CBF.OBSUNC.ABGB,-9999)
    else
        ncwriteatt_if_exists_else_fill(fname,'ABGB','seasonal_unc','single_monthly_unc',CBF.OBSUNC.ABGB,-9999)
        ncwriteatt_if_exists_else_fill(fname,'ABGB','annual_unc','single_annual_unc',CBF.OBSUNC.ABGB,-9999)
    end
end
ncwriteatt_if_exists_else_fill(fname,'ABGB','obs_unc_threshold','min_threshold',CBF.OBSUNC.ABGB,-9999); % double check


%********ET*****************

nccreate(fname,'ET','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'ETunc','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'ET',CBF.OBS.ET);
%Variable attributes
% ncwriteatt(fname,'ET','Description','Evapotranspiration')
% ncwriteatt(fname,'ET','info',CBF.OBSUNC.ET.info)
%Same as standard CBF variables
ncwriteatt(fname,'ET','opt_unc_type',-9999)
ncwriteatt(fname,'ET','opt_normalization',-9999)
ncwriteatt(fname,'ET','opt_filter',-9999)
ncwriteatt(fname,'ET','single_monthly_unc',-9999)
ncwriteatt(fname,'ET','single_mean_unc',-9999)
ncwriteatt_if_exists_else_fill(fname,'ET','unc','single_unc',CBF.OBSUNC.ET,-9999)
ncwriteatt(fname,'ET','structural_unc',-9999)
if CBF.OBSUNC.ET.annual_unc == -9999
    ncwriteatt_if_exists_else_fill(fname,'ET','seasonal_unc','single_unc',CBF.OBSUNC.ET,-9999)
    ncwriteatt_if_exists_else_fill(fname,'ET','annual_unc','single_annual_unc',CBF.OBSUNC.ET,-9999)
else
    ncwriteatt_if_exists_else_fill(fname,'ET','seasonal_unc','single_monthly_unc',CBF.OBSUNC.ET,-9999)
    ncwriteatt_if_exists_else_fill(fname,'ET','annual_unc','single_annual_unc',CBF.OBSUNC.ET,-9999)
end
ncwriteatt_if_exists_else_fill(fname,'ET','obs_unc_threshold','min_threshold',CBF.OBSUNC.ET,-9999); % double check

%********EWT*****************

nccreate(fname,'EWT','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'EWTunc','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'EWT',CBF.OBS.EWT);
%Variable attributes
% ncwriteatt(fname,'EWT','Description','Equivalent Water Thickness')
% ncwriteatt(fname,'EWT','info',CBF.OBSUNC.EWT.info)
%Same as standard CBF variables
ncwriteatt(fname,'EWT','opt_unc_type',-9999)
ncwriteatt(fname,'EWT','opt_normalization',-9999)
ncwriteatt(fname,'EWT','opt_filter',-9999)
ncwriteatt(fname,'EWT','single_monthly_unc',-9999)
ncwriteatt(fname,'EWT','single_mean_unc',-9999)
ncwriteatt(fname,'EWT','single_unc',CBF.OBSUNC.EWT.unc)
ncwriteatt(fname,'EWT','structural_unc',-9999)
if CBF.OBSUNC.EWT.annual_unc == -9999
    ncwriteatt_if_exists_else_fill(fname,'EWT','seasonal_unc','single_unc',CBF.OBSUNC.EWT,-9999)
    ncwriteatt_if_exists_else_fill(fname,'EWT','annual_unc','single_annual_unc',CBF.OBSUNC.EWT,-9999)
else
    ncwriteatt_if_exists_else_fill(fname,'EWT','seasonal_unc','single_monthly_unc',CBF.OBSUNC.EWT,-9999)
    ncwriteatt_if_exists_else_fill(fname,'EWT','annual_unc','single_annual_unc',CBF.OBSUNC.EWT,-9999)
end
ncwriteatt_if_exists_else_fill(fname,'EWT','obs_unc_threshold','min_threshold',CBF.OBSUNC.EWT,-9999); % double check

%********SOM*****************

nccreate(fname,'SOM','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'SOMunc','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'SOM',CBF.OBS.SOM);
%Variable attributes
% ncwriteatt(fname,'SOM','Description','Soil organic matter');
% ncwriteatt(fname,'SOM','info',CBF.OBSUNC.SOM.info);
%Same as standard CBF variables
% ncwriteatt(fname,'SOM','Uncertainty',CBF.OBSUNC.SOM.unc);
ncwriteatt(fname,'SOM','opt_unc_type',-9999)
ncwriteatt(fname,'SOM','opt_normalization',-9999)
ncwriteatt(fname,'SOM','opt_filter',-9999)
ncwriteatt(fname,'SOM','single_monthly_unc',-9999)
ncwriteatt(fname,'SOM','single_mean_unc',-9999)
ncwriteatt_if_exists_else_fill(fname,'SOM','unc','single_unc',CBF.OBSUNC.SOM,-9999)
ncwriteatt(fname,'SOM','structural_unc',-9999)
if isfield(CBF.OBSUNC.SOM,'annual_unc')
    if CBF.OBSUNC.SOM.annual_unc == -9999
        ncwriteatt_if_exists_else_fill(fname,'SOM','seasonal_unc','single_unc',CBF.OBSUNC.SOM,-9999)
        ncwriteatt_if_exists_else_fill(fname,'SOM','annual_unc','single_annual_unc',CBF.OBSUNC.SOM,-9999)
    else
        ncwriteatt_if_exists_else_fill(fname,'SOM','seasonal_unc','single_monthly_unc',CBF.OBSUNC.SOM,-9999)
        ncwriteatt_if_exists_else_fill(fname,'SOM','annual_unc','single_annual_unc',CBF.OBSUNC.SOM,-9999)
    end
end
ncwriteatt_if_exists_else_fill(fname,'SOM','obs_unc_threshold','min_threshold',CBF.OBSUNC.SOM,-9999); % double check


%********CH4*****************


%global attributes
nccreate(fname,'CH4','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'CH4unc','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
%----- ncwrite(fname,'CH4',CBF.OBS.CH4); %Variable data must not be empty.
ncwrite_if_exists(fname,'CH4',CBF.OBS.CH4)
%Description and comments
% ncwriteatt(fname,'CH4','Description','Net Methane Emission')
% ncwriteatt(fname,'CH4','info',CBF.OBSUNC.CH4.info)
%Variable attributes
ncwriteatt(fname,'CH4','opt_unc_type',-9999)
ncwriteatt(fname,'CH4','opt_normalization',-9999)
ncwriteatt(fname,'CH4','opt_filter',-9999)
ncwriteatt(fname,'CH4','single_monthly_unc',-9999)
ncwriteatt(fname,'CH4','single_mean_unc',-9999)
ncwriteatt_if_exists_else_fill(fname,'CH4','unc','single_unc',CBF.OBSUNC.CH4,-9999)
ncwriteatt(fname,'CH4','structural_unc',-9999)
if CBF.OBSUNC.CH4.annual_unc == -9999
    ncwriteatt_if_exists_else_fill(fname,'CH4','seasonal_unc','single_unc',CBF.OBSUNC.CH4,-9999)
    ncwriteatt_if_exists_else_fill(fname,'CH4','annual_unc','single_annual_unc',CBF.OBSUNC.CH4,-9999)
else
    ncwriteatt_if_exists_else_fill(fname,'CH4','seasonal_unc','single_monthly_unc',CBF.OBSUNC.CH4,-9999)
    ncwriteatt_if_exists_else_fill(fname,'CH4','annual_unc','single_annual_unc',CBF.OBSUNC.CH4,-9999)
end
ncwriteatt_if_exists_else_fill(fname,'CH4','obs_unc_threshold','min_threshold',CBF.OBSUNC.CH4,-9999); % double check


%********SCF*****************


% %global attributes
% nccreate(fname,'SCF','Dimensions',{'time',nodays},'FillValue',-9999);
% %Writing variable 
% %----- ncwrite(fname,'CH4',CBF.OBS.CH4); %Variable data must not be empty.
% %check if field exists in the input CBF
% if isfield(CBF.OBS,'SCF')==1
%     ncwrite_if_exists(fname,'SCF',CBF.OBS.SCF)
%     %Description and comments
%     ncwriteatt(fname,'SCF','Description','Snow Cover Fraction')
%     ncwriteatt(fname,'SCF','info',CBF.OBSUNC.SCF.info)
%     %Variable attributes
%     ncwriteatt(fname,'SCF','Uncertainty',CBF.OBSUNC.SCF.unc)
%     ncwriteatt(fname,'SCF','Uncertainty_Threshold',CBF.OBSUNC.SCF.obs_unc_threshold)
% end






% nccreate(fname,'MCMC_ID'); ncwrite(fname,'MCMC_ID',int(119)) %Undefined function 'int' for input arguments of type 'double'.
%nccreate(fname,'nomet'); ncwrite(fname,'nomet',CBF.nomet)

%*********write CBF met***********
%nccreate(fname,'MET','Dimensions',{'time', nodays,'nomet',nomet}); 

% nccreate(fname,'TIME_INDEX','Dimensions',{'time', nodays},'FillValue',-9999);
nccreate(fname,'time','Dimensions',{'time', nodays},'FillValue',-9999);
nccreate(fname,'T2M_MIN','Dimensions',{'time', nodays},'FillValue',-9999); 
nccreate(fname,'T2M_MAX','Dimensions',{'time', nodays},'FillValue',-9999);
nccreate(fname,'SSRD','Dimensions',{'time', nodays},'FillValue',-9999); 
nccreate(fname,'CO2','Dimensions',{'time', nodays},'FillValue',-9999); 
nccreate(fname,'DOY','Dimensions',{'time', nodays},'FillValue',-9999);
nccreate(fname,'BURNED_AREA','Dimensions',{'time', nodays},'FillValue',-9999);
nccreate(fname,'VPD','Dimensions',{'time',nodays},'FillValue',-9999);
nccreate(fname,'TOTAL_PREC','Dimensions',{'time', nodays},'FillValue',-9999);
nccreate(fname,'SNOW_FALL','Dimensions',{'time', nodays},'FillValue',-9999);






%ncwrite(fname,'MET',CBF.MET);
METVNAME={'time','T2M_MIN','T2M_MAX','SSRD','CO2','DOY','BURNED_AREA','VPD','TOTAL_PREC','SNOW_FALL'};
METINFO={'Time since Jan 01/01/2000 00:00','Mean daily min. temperature','Mean daily max. temperature','Global radiation','Atmospheric CO2','Day of year','Burned area','VPD','Total precipitation','Snowfall'};
METUNITS={'Days','deg C','deg C','MJ/m2/d','ppm','Days','m2/m2','hPA','mm/day','mm/day'};

for n=1:nomet
ncwrite(fname,METVNAME{n},CBF.MET(:,n));
ncwriteatt(fname,METVNAME{n},'reference_mean',mean(CBF.MET(:,n)));
ncwriteatt(fname,METVNAME{n},'info',METINFO{n});
ncwriteatt(fname,METVNAME{n},'units',METUNITS{n});
end




%Global attributes
ncwriteatt(fname,'/','uncertainty_factors',CBF.OBSinfo.uncertainty_factors)


end

function ncwrite_if_exists(filename,fieldname,F)


if isempty(F)==0
    ncwrite(filename,fieldname,F);
end

end

function ncwriteatt_if_exists_else_fill(filename,variable,oldfieldname,newfieldname,F,FillValue)


if isfield(F,oldfieldname)==1
    ncwriteatt(filename,variable,newfieldname,F.(oldfieldname));
else
    ncwriteatt(filename,variable,newfieldname,FillValue);
end

end








