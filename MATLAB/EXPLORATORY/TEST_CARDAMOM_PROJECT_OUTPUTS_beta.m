
%Example PXI project
PXI=CARDAMOM_PROJECT('GCRUN_APR20_TDLE');

%Example PXI output
PXIv1=PARRFUN_CARDAMOM_CBF2JPLHPC(PXI,'v1',[]);

%Example field request
FIELDS(1).name='ET_ANNUAL_DETREND';
FIELDS(1).func=@(CBR,CBF) detrend(monthly2annual(CBR.ET)')';
FIELDS(1).overwrite=0;

FIELDS(2).name='ET_MEAN_SEASONAL_CYCLE';
FIELDS(2).func=@(CBR,CBF) pauls_sens_test(CBR.ET);
FIELDS(2).overwrite=0;
% 
% FIELDS(2).name='GPP_ANNUAL_DETREND';
% FIELDS(2).func=@(CBR,CBF) detrend(monthly2annual(CBR.GPP)')';
% FIELDS(2).overwrite=0;
% 
% FIELDS(3).name='NBE_ANNUAL_DETREND3';
% FIELDS(3).func=@(CBR,CBF) detrend(monthly2annual(CBR.NBE)',3)';
% FIELDS(3).overwrite=0;

%Example request for fields
CARFIELDS=CARDAMOM_PROJECT_OUTPUTS_beta(PXIv1,FIELDS);











%
%
6





