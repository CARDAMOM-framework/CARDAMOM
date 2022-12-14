%%%%%%%Load existing run %%%%%%
cbr='Desktop/fluxval_US-NR1_1100_ABGB_LAI.cbr';
cbf='Desktop/fluxval_US-NR1_1100_ABGB_LAI.cbf.nc';
CBRfull=CARDAMOM_RUN_MODEL(cbf,cbr);
pars=CBRfull.PARS(450,:);
CBR=CARDAMOM_RUN_MODEL(cbf,pars);
CBF=CARDAMOM_READ_NC_CBF_FILE(cbf);


%New parameters for everything
%******needleleaf, LCMA ~ 100gC/m2, LAI ~ 4
parsnew=pars;
parsnew(MD.PARAMETER_IDs.i_wood)=5000;
parsnew(MD.PARAMETER_IDs.i_foliar)=400;
parsnew(MD.PARAMETER_IDs.LCMA)=100;
parsnew(MD.PARAMETER_IDs.t_wood)=3e-5;%This is too low
parsnew(MD.PARAMETER_IDs.Vcmax25)=85;
parsnew(:,MD.PARAMETER_IDs.lambda_max)=4;
parsnew(:,MD.PARAMETER_IDs.lambda_max)=4;
