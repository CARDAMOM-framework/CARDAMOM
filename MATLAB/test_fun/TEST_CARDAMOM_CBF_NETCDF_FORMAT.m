%Notes: https://www.unidata.ucar.edu/software/netcdf/docs/netcdf_data_set_components.html

%- Scalar variables don't need dimension definitions
function fname=TEST_CARDAMOM_CBF_NETCDF_FORMAT(CBF)

if nargin==0
% Step 1. Read CBF file
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');
end
%Dimensions
%nopars
%nodays
%Number of timesteps
nodays=size(CBF.MET,1);
%Number of met fields
nomet=size(CBF.MET,2);


%Step 2. write as netcdf file


fname='CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE_beta.nc.cbf';delete(fname)





%*******Write CBF structure fields to .nc file**********

%Setting/testing convention here: pass global "attributes" as variables,
%unless completely self-explanatory...

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


%Other obs
%Mean biomass
nccreate(fname,'Mean Biomass'); 
ncwrite(fname,'Mean Biomass',CBF.OTHER_OBS.MBiomass.mean)
ncwriteatt(fname,'Mean Biomass','Uncertainty',CBF.OTHER_OBS.MBiomass.unc)
ncwriteatt(fname,'Mean Biomass','Uncertainty Type','factor')

%Mean fire
nccreate(fname,'Mean Fire'); 
ncwrite(fname,'Mean Fire',CBF.OTHER_OBS.MFire.mean)
ncwriteatt(fname,'Mean Fire','Uncertainty',CBF.OTHER_OBS.MFire.unc)
ncwriteatt(fname,'Mean Fire','Uncertainty Type','absolute');

%Mean LAI
nccreate(fname,'Mean LAI'); 
ncwrite(fname,'Mean LAI',CBF.OTHER_OBS.MLAI.mean)
ncwriteatt(fname,'Mean LAI','Uncertainty',CBF.OTHER_OBS.MLAI.unc)
ncwriteatt(fname,'Mean LAI','Uncertainty Type','absolute');


%Mean GPP
nccreate(fname,'Mean GPP'); 
ncwrite(fname,'Mean GPP',CBF.OTHER_OBS.MGPP.mean)
ncwriteatt(fname,'Mean GPP','Uncertainty',CBF.OTHER_OBS.MGPP.unc)
ncwriteatt(fname,'Mean GPP','Uncertainty Type','absolute');



%Time-varing obs

%********GPP*****************

nccreate(fname,'GPP','Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'GPP',CBF.OBS.GPP);
%Variable attributes
ncwriteatt(fname,'GPP','Description','Gross Primary Productivity')
ncwriteatt(fname,'GPP','info',CBF.OBSUNC.GPP.info)
ncwriteatt(fname,'GPP','gppabs_info',CBF.OBSUNC.GPP.gppabs_info)
%Same as standard CBF variables
ncwriteatt(fname,'GPP','Uncertainty',CBF.OBSUNC.GPP.unc)
ncwriteatt(fname,'GPP','Annual Uncertainty',CBF.OBSUNC.GPP.annual_unc)
ncwriteatt(fname,'GPP','gppabs',CBF.OBSUNC.GPP.gppabs)
ncwriteatt(fname,'GPP','obs_unc_threshold',CBF.OBSUNC.GPP.obs_unc_threshold)


%********LAI*****************

nccreate(fname,'LAI','Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'LAI',CBF.OBS.LAI);
ncwriteatt(fname,'LAI','LAI data requirement',CBF.OBSinfo.LAI)
%Variable attributes
ncwriteatt(fname,'LAI','Description','Leaf Area Index')



%********NBE*****************

nccreate(fname,'NBE');%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'NBE',CBF.OBS.NBE);
%Variable attributes
ncwriteatt(fname,'NBE','Description','Net Biospheric Exchange')
ncwriteatt(fname,'NBE','info',CBF.OBSUNC.NBE.info)
%Same as standard CBF variables
ncwriteatt(fname,'NBE','Seasonal Uncertainty',CBF.OBSUNC.NBE.seasonal_unc)
ncwriteatt(fname,'NBE','Annual Uncertainty',CBF.OBSUNC.NBE.annual_unc)



%********ABGB*****************

nccreate(fname,'ABGB');%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'ABGB',CBF.OBS.ABGB);
%Variable attributes
ncwriteatt(fname,'ABGB','Description','Above- and below-ground biomass')
ncwriteatt(fname,'ABGB','info',CBF.OBSUNC.ABGB.info)
%Same as standard CBF variables
ncwriteatt(fname,'ABGB','Uncertainty',CBF.OBSUNC.ABGB.unc);



%********ET*****************

nccreate(fname,'ET');%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'ET',CBF.OBS.ET);
%Variable attributes
ncwriteatt(fname,'ET','Description','Evapotranspiration')
ncwriteatt(fname,'ET','info',CBF.OBSUNC.ET.info)
%Same as standard CBF variables
ncwriteatt(fname,'ET','Uncertainty',CBF.OBSUNC.ET.unc)
ncwriteatt(fname,'ET','Annual Uncertainty',CBF.OBSUNC.ET.annual_unc)
ncwriteatt(fname,'ET','Uncertainty Threshold',CBF.OBSUNC.ET.obs_unc_threshold);


%********EWT*****************

nccreate(fname,'EWT');%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'EWT',CBF.OBS.EWT);
%Variable attributes
ncwriteatt(fname,'EWT','Description','Equivalent Water Thickness')
ncwriteatt(fname,'EWT','info',CBF.OBSUNC.EWT.info)
%Same as standard CBF variables
ncwriteatt(fname,'EWT','Uncertainty',CBF.OBSUNC.EWT.unc)
ncwriteatt(fname,'EWT','Annual Uncertainty',CBF.OBSUNC.EWT.annual_unc)


%********SOM*****************

nccreate(fname,'SOM');%,'Dimensions',{'time',nodays});
%Writing variable 
ncwrite_if_exists(fname,'SOM',CBF.OBS.SOM);
%Variable attributes
ncwriteatt(fname,'SOM','Description','Soil organic matter');
ncwriteatt(fname,'SOM','info',CBF.OBSUNC.SOM.info);
%Same as standard CBF variables
ncwriteatt(fname,'SOM','Uncertainty',CBF.OBSUNC.SOM.unc);


%********CH4*****************


%global attributes
nccreate(fname,'CH4');
%Writing variable 
%----- ncwrite(fname,'CH4',CBF.OBS.CH4); %Variable data must not be empty.
ncwrite_if_exists(fname,'CH4',CBF.OBS.CH4)
%Description and comments
ncwriteatt(fname,'CH4','Description','Net Methane Emission')
ncwriteatt(fname,'CH4','info',CBF.OBSUNC.CH4.info)
%Variable attributes
ncwriteatt(fname,'CH4','Uncertainty',CBF.OBSUNC.CH4.unc)
ncwriteatt(fname,'CH4','Annual Uncertainty',CBF.OBSUNC.CH4.annual_unc)
ncwriteatt(fname,'CH4','obs_unc_threshold',CBF.OBSUNC.CH4.obs_unc_threshold)

%**********LAI******************





% nccreate(fname,'MCMC_ID'); ncwrite(fname,'MCMC_ID',int(119)) %Undefined function 'int' for input arguments of type 'double'.
%nccreate(fname,'nomet'); ncwrite(fname,'nomet',CBF.nomet)

%*********write CBF met***********
nccreate(fname,'MET','Dimensions',{'nodays', nodays,'nomet',nomet}); 

ncwrite(fname,'MET',CBF.MET);


nccreate(fname,'EDCDIAG'); 
ncwrite(fname,'EDCDIAG',CBF.EDCDIAG)



% not sure to keep MET one variable or divide into multiple ones?
% nccreate(fname,'MET','Dimensions',{}); 
% ncwrite(fname,'MET',CBF.MET)

%Caveat = for some reason, matlab wants variable created before making
%global attributes




%Global attributes
ncwriteatt(fname,'/','uncertainty factors',CBF.OBSinfo.uncertainty_factors)


end

function ncwrite_if_exists(filename,fieldname,F)


if isempty(F)==0
    ncwrite(filename,fieldname,F);
end

end








