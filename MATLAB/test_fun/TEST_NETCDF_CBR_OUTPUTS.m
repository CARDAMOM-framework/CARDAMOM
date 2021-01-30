%Step 1. RUn model with pre-existing parameters

CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/EXAMPLE_IO/GCRUN_JUL19_TDLE_1942.cbf');
CBR=CARDAMOM_RUN_MODEL(CBF,'CARDAMOM/DATA/EXAMPLE_IO/GCRUN_JUL19_TDLEv1_182_1.cbr');

%Step 2. load netcdf dataset

ncfname='tempcardata.nc';

FLUXES=permute(ncread(ncfname,'M_FLUXES'),[3,2,1]);
POOLS=permute(ncread(ncfname,'M_POOLS'),[3,2,1]);
PROB=ncread(ncfname,'M_P');
EDCD=double(permute(ncread(ncfname,'M_EDCD'),[2,1]));


total(PROB - CBR.PROB); %within numerical error
total(EDCD- CBR.EDCDIAG);
total(POOLS(:,2:end,:)- CBR.POOLS);