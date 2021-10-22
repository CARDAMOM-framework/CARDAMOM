
%Notes: https://www.unidata.ucar.edu/software/netcdf/docs/netcdf_data_set_components.html

%- Scalar variables don't need dimension definitions
function TEST_CARDAMOM_CBF_NETCDF_FORMAT(CBF,fname)

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

%FIX: need this as dimension
%nccreate(fname,'nodays'); ncwrite(fname,'nodays',nodays)


%Adjust number as needed. Not urgent for field to dynamically vary
nccreate(fname,'PARPRIORS','Dimensions',{'nopars',50}); 
ncwrite(fname,'PARPRIORS',CBF.PARPRIORS)
nccreate(fname,'PARPRIORUNC','Dimensions',{'nopars'});
ncwrite(fname,'PARPRIORUNC',CBF.PARPRIORUNC)


%Adjust number as needed. Not urgent for field to dynamically vary
nccreate(fname,'OTHERPRIORS','Dimensions',{'nopars',50}); 
ncwrite(fname,'OTHERPRIORS',CBF.RAW.OTHERPRIORS)
nccreate(fname,'OTHERPRIORSUNC','Dimensions',{'nopars'});
ncwrite(fname,'OTHERPRIORSUNC',CBF.RAW.OTHERPRIORSUNC)




%Other obs
%Mean biomass
nccreate(fname,'Mean_Biomass'); 
ncwrite(fname,'Mean_Biomass',CBF.OTHER_OBS.MBiomass.mean)
ncwriteatt(fname,'Mean_Biomass','Uncertainty',CBF.OTHER_OBS.MBiomass.unc)
ncwriteatt(fname,'Mean_Biomass','Uncertainty_Type','factor')

%Mean fire
nccreate(fname,'Mean_Fire'); 
ncwrite(fname,'Mean_Fire',CBF.OTHER_OBS.MFire.mean)
ncwriteatt(fname,'Mean_Fire','Uncertainty',CBF.OTHER_OBS.MFire.unc)
ncwriteatt(fname,'Mean_Fire','Uncertainty_Type','absolute');

%Mean LAI
nccreate(fname,'Mean_LAI'); 
ncwrite(fname,'Mean_LAI',CBF.OTHER_OBS.MLAI.mean)
ncwriteatt(fname,'Mean_LAI','Uncertainty',CBF.OTHER_OBS.MLAI.unc)
ncwriteatt(fname,'Mean_LAI','Uncertainty_Type','absolute');


%Mean GPP
nccreate(fname,'Mean_GPP'); 
ncwrite(fname,'Mean_GPP',CBF.OTHER_OBS.MGPP.mean)
ncwriteatt(fname,'Mean_GPP','Uncertainty',CBF.OTHER_OBS.MGPP.unc)
ncwriteatt(fname,'Mean_GPP','Uncertainty_Type','absolute');



%Time-varing obs

%********GPP*****************

nccreate(fname,'GPP','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'GPP',CBF.OBS.GPP);
%Variable attributes
ncwriteatt(fname,'GPP','Description','Gross Primary Productivity')
ncwriteatt(fname,'GPP','info',CBF.OBSUNC.GPP.info)
ncwriteatt(fname,'GPP','gppabs_info',CBF.OBSUNC.GPP.gppabs_info)
%Endless list, up to users to decide what goes here (e.g. source data?)
%Same as standard CBF variables
ncwriteatt(fname,'GPP','Uncertainty',CBF.OBSUNC.GPP.unc)
ncwriteatt(fname,'GPP','Annual_Uncertainty',CBF.OBSUNC.GPP.annual_unc)
ncwriteatt(fname,'GPP','gppabs',CBF.OBSUNC.GPP.gppabs)
ncwriteatt(fname,'GPP','Uncertainty_Threshold',CBF.OBSUNC.GPP.obs_unc_threshold)


%********LAI*****************

nccreate(fname,'LAI','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'LAI',CBF.OBS.LAI);
ncwriteatt(fname,'LAI','LAI_data_requirement',CBF.OBSinfo.LAI)
%Variable attributes
ncwriteatt(fname,'LAI','Description','Leaf Area Index')



%********NBE*****************

nccreate(fname,'NBE','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
ncwrite_if_exists(fname,'NBE',CBF.OBS.NBE);
%Variable attributes
ncwriteatt(fname,'NBE','Description','Net Biospheric Exchange')
ncwriteatt(fname,'NBE','info',CBF.OBSUNC.NBE.info)
%Same as standard CBF variables
ncwriteatt(fname,'NBE','Uncertainty',CBF.OBSUNC.NBE.seasonal_unc)
ncwriteatt(fname,'NBE','Annual_Uncertainty',CBF.OBSUNC.NBE.annual_unc)



%********ABGB*****************

nccreate(fname,'ABGB','Dimensions',{'time',nodays},'FillValue',-9999);%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'ABGB',CBF.OBS.ABGB);
%Variable attributes
ncwriteatt(fname,'ABGB','Description','Above- and below-ground biomass')
ncwriteatt(fname,'ABGB','info',CBF.OBSUNC.ABGB.info)
%Same as standard CBF variables
ncwriteatt(fname,'ABGB','Uncertainty',CBF.OBSUNC.ABGB.unc);



%********ET*****************

nccreate(fname,'ET','Dimensions',{'time',nodays},'FillValue',-9999);%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'ET',CBF.OBS.ET);
%Variable attributes
ncwriteatt(fname,'ET','Description','Evapotranspiration')
ncwriteatt(fname,'ET','info',CBF.OBSUNC.ET.info)
%Same as standard CBF variables
ncwriteatt(fname,'ET','Uncertainty',CBF.OBSUNC.ET.unc)
ncwriteatt(fname,'ET','Annual_Uncertainty',CBF.OBSUNC.ET.annual_unc)
ncwriteatt(fname,'ET','Uncertainty_Threshold',CBF.OBSUNC.ET.obs_unc_threshold);


%********EWT*****************

nccreate(fname,'EWT','Dimensions',{'time',nodays},'FillValue',-9999);%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'EWT',CBF.OBS.EWT);
%Variable attributes
ncwriteatt(fname,'EWT','Description','Equivalent Water Thickness')
ncwriteatt(fname,'EWT','info',CBF.OBSUNC.EWT.info)
%Same as standard CBF variables
ncwriteatt(fname,'EWT','Uncertainty',CBF.OBSUNC.EWT.unc)
ncwriteatt(fname,'EWT','Annual_Uncertainty',CBF.OBSUNC.EWT.annual_unc)


%********SOM*****************

nccreate(fname,'SOM','Dimensions',{'time',nodays},'FillValue',-9999);%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'SOM',CBF.OBS.SOM);
%Variable attributes
ncwriteatt(fname,'SOM','Description','Soil organic matter');
ncwriteatt(fname,'SOM','info',CBF.OBSUNC.SOM.info);
%Same as standard CBF variables
ncwriteatt(fname,'SOM','Uncertainty',CBF.OBSUNC.SOM.unc);


%********CH4*****************


%global attributes
nccreate(fname,'CH4','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
%----- ncwrite(fname,'CH4',CBF.OBS.CH4); %Variable data must not be empty.
ncwrite_if_exists(fname,'CH4',CBF.OBS.CH4)
%Description and comments
ncwriteatt(fname,'CH4','Description','Net Methane Emission')
ncwriteatt(fname,'CH4','info',CBF.OBSUNC.CH4.info)
%Variable attributes
ncwriteatt(fname,'CH4','Uncertainty',CBF.OBSUNC.CH4.unc)
ncwriteatt(fname,'CH4','Annual_Uncertainty',CBF.OBSUNC.CH4.annual_unc)
ncwriteatt(fname,'CH4','Uncertainty_Threshold',CBF.OBSUNC.CH4.obs_unc_threshold)

%********SCF*****************


%global attributes
nccreate(fname,'SCF','Dimensions',{'time',nodays},'FillValue',-9999);
%Writing variable 
%----- ncwrite(fname,'CH4',CBF.OBS.CH4); %Variable data must not be empty.
ncwrite_if_exists(fname,'SCF',CBF.OBS.SCF)
%Description and comments
ncwriteatt(fname,'SCF','Description','Snow Cover Fraction')
ncwriteatt(fname,'SCF','info',CBF.OBSUNC.SCF.info)
%Variable attributes
ncwriteatt(fname,'SCF','Uncertainty',CBF.OBSUNC.SCF.unc)
ncwriteatt(fname,'SCF','Uncertainty_Threshold',CBF.OBSUNC.SCF.obs_unc_threshold)





% nccreate(fname,'MCMC_ID'); ncwrite(fname,'MCMC_ID',int(119)) %Undefined function 'int' for input arguments of type 'double'.
%nccreate(fname,'nomet'); ncwrite(fname,'nomet',CBF.nomet)

%*********write CBF met***********
%nccreate(fname,'MET','Dimensions',{'time', nodays,'nomet',nomet}); 

nccreate(fname,'TIME_INDEX','Dimensions',{'time', nodays},'FillValue',-9999);
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
METVNAME={'TIME_INDEX','T2M_MIN','T2M_MAX','SSRD','CO2','DOY','BURNED_AREA','VPD','TOTAL_PREC','SNOW_FALL'};
METINFO={'Time index','Mean daily min. temperature','Mean daily max. temperature','Global radiation','Atmospheric CO2','Day of year','Burned area','VPD','Total precipitation','Snowfall'};
METUNITS={'Days','deg C','deg C','MJ/m2/d','ppm','Days','m2/m2','hPA','mm/day','mm/day'};

for n=1:nomet
ncwrite(fname,METVNAME{n},CBF.MET(:,n));
ncwriteatt(fname,METVNAME{n},'reference_mean',mean(CBF.MET(:,n)));
ncwriteatt(fname,METVNAME{n},'info',METINFO{n});
ncwriteatt(fname,METVNAME{n},'units',METUNITS{n});
end



nccreate(fname,'EDCDIAG'); 
ncwrite(fname,'EDCDIAG',CBF.EDCDIAG)



% not sure to keep MET one variable or divide into multiple ones?
% nccreate(fname,'MET','Dimensions',{}); 
% ncwrite(fname,'MET',CBF.MET)

%Caveat = for some reason, matlab wants variable created before making
%global attributes




%Global attributes
ncwriteatt(fname,'/','uncertainty_factors',CBF.OBSinfo.uncertainty_factors)


end

function ncwrite_if_exists(filename,fieldname,F)


if isempty(F)==0
    ncwrite(filename,fieldname,F);
end

end








