

%Function provided by AA Bloom
GPPraw=quickload('DATASCRIPT_READ_FLUXCOM_DATASETS_OCT17');

%Average ML layers
MGPP=mean(GPPraw.data,4);

%Save structure for eventiual CARDAMOM-MAPS export
GPP.data=MGPP;
GPP.units=GPPraw.units;
GPP.date=GPPraw.date;
GPP.year=GPPraw.year;
GPP.month=GPPraw.month;

save DUMPFILES/CARDAMOM_MAPS_DATASCRIPT_FLUXCOM_GPP.mat GPP;
