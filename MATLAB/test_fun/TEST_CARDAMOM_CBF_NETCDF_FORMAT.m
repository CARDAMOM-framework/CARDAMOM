% Step 1. Read CBF file
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');


%Step 2. write as netcdf file


fname='CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE_beta.nc.cbf';delete(fname)

%*******Write CBF structure fields to .nc file**********

%Setting/testing convention here: pass global "attributes" as variables,
%unless completely self-explanatory...

%Global attributes
nccreate(fname,'ID'); ncwrite(fname,'ID',CBF.ID)
nccreate(fname,'LAT'); ncwrite(fname,'LAT',CBF.LAT)
nccreate(fname,'EDC'); ncwrite(fname,'EDC',CBF.EDC)
nccreate(fname,'MCMC_ID'); ncwrite(fname,'MCMC_ID',int(119))
nccreate(fname,'nodays'); ncwrite(fname,'nodays',CBF.nodays)
nccreate(fname,'nomet'); ncwrite(fname,'nomet',CBF.nomet)
nccreate(fname,'EDCDIAG'); ncwrite(fname,'EDCDIAG',CBF.EDCDIAG)
nccreate(fname,'rc_random_search'); ncwrite(fname,'rc_random_search',double(CBF.rc_random_search))

%Caveat = for some reason, matlab wants variable created before making
%global attributes
nccreate(fname,'GPP','Dimensions',{'time',numel(CBF.OBS.GPP)});
%Writing variable 
ncwrite(fname,'GPP',CBF.OBS.GPP);
%Variable attributes
ncwriteatt(fname,'GPP','Description','Gross Primary Productivity')
ncwriteatt(fname,'GPP','Uncertainty',CBF.OBSUNC.GPP.unc)
ncwriteatt(fname,'GPP','Annual Uncertainty',CBF.OBSUNC.GPP.annual_unc)
%...
%...
%...






