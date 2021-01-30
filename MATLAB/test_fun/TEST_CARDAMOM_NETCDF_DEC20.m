%Step 1.  Run with existing output;


CBF=CARDAMOM_READ_BINARY_FILEFORMAT('testnc.cbf');
CBRnew=CARDAMOM_RUN_MODEL('testnc.nc.cbf','testnc.cbr');

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


