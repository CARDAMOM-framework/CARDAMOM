%Step 1.  Run with existing output;


CBF=CARDAMOM_READ_BINARY_FILEFORMAT('testnc.cbf');

%Step 2. Write as netcdf file
fname_old='testnc.nc.cbf';
fname='testnc2.nc.cbf';
delete(fname);
CARDAMOM_CBF2NETCDF(CBF,fname)


CBRnew=CARDAMOM_RUN_MODEL(fname,'testnc.cbr');

ni=ncinfo('testnc.nc.cbf');
ncdisp('testnc.nc.cbf')


%Step 2. Load "pre-saved outputs"
%save testncCBR_save.mat CBR
load testncCBR_save.mat CBR

%Step 3. COmpare the two
total(CBRnew.POOLS-CBR.POOLS)
total(CBRnew.FLUXES-CBR.FLUXES)
total(CBRnew.PARS-CBR.PARS)
mean(CBRnew.PROB-CBR.PROB)


