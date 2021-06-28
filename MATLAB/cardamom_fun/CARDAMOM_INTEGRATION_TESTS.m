

%This code runs a sequence of CARDAMOM benchmark tests with expected or
%known outcomes

cbffilename='CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf';
cbrfilename='CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbr';

%Step 1. Run forward run
CBR=CARDAMOM_RUN_MODEL(cbffilename,cbrfilename);

disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('CARDAMOM_RUN_MODEL successfully executed')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')




CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffilename);

disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('CARDAMOM_READ_BINARY_FILEFORMAT successfully executed')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')


cbrtest='cardamom_integration_test.cbr';
delete('cardamom_integration_test.cbr');
delete('cardamom_integration_test.cbrSTART');
CBF.EDC=0;
CBRtest=CARDAMOM_RUN_MDF(CBF,[],cbrtest);


disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('CARDAMOM_RUN_MDF with MHMCMC successfully executed')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')

%ADEMCMC

%Step 2. Set up and run DEMCMC
%Still in prototype phase
MCO.niterations=10000;
MCO.printrate=100;
MCO.samplerate=MCO.niterations/2000*100;
MCO.mcmcid=3;
CBF.EDC=0;
CBR=CARDAMOM_RUN_MDF(CBF,MCO);toc



disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('CARDAMOM_RUN_MDF with ADEMCMC (with EDC=0 successfully executed')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')



%COst function tests
%Step 1. Use existing solution
%cbffilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942.cbf';
%cbrfilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942_1.cbr';

%Step 1. Run forward run
%CBR=CARDAMOM_RUN_MODEL(cbffilename,cbrfilename);cbffilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942.cbf';
%cbrfilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942_1.cbr';

%Step 1. Generate some output (since 813 not available yet).
%CBF=CARDAMOM_CBF_TEMPLATE(1000);
cbffilename='CARDAMOM/DATA/MODEL_ID_1000_EXAMPLE.cbf';
cbrfilename='CARDAMOM/DATA/MODEL_ID_1000_EXAMPLE.cbr';
%CARDAMOM_WRITE_BINARY_FILEFORMAT(CBF,cbffilename);
%CBR=CARDAMOM_RUN_MDF(CBF,[],cbrfilename);

CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffilename);
CBR=CARDAMOM_RUN_MODEL(CBF,cbrfilename);

CBFcf=cardamomfun_clear_cbf_obs(CBF);
cfpars=CBR.PARS(end,:);
CBRcf=CARDAMOM_RUN_MODEL(CBFcf,cfpars);


%Step 2. COst function with zero obs
disp(sprintf('Cost function with zero obs %2.2f (expected 0)',CBRcf.PROB));

%*************LAI for 809*****************
%LAI, no change
CBFcftest=CBFcf;
CBFcftest.OBS.LAI=CBF.MET(:,1)*-9999;
CBFcftest.OBS.LAI(2) = (CBRcf.POOLS(1,2,2) +  CBRcf.POOLS(1,1,2))/2/CBRcf.PARS(1,17);
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with LAI(2) obs %2.2f (expected 0)',CBRcf.PROB));

%LAI 1-sigma change
CBFcftest.OBS.LAI(2) = (CBRcf.POOLS(1,2,2) +  CBRcf.POOLS(1,1,2))/2/CBRcf.PARS(1,17)*2;
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with LAI(2) obs %2.2f (expected -0.5)',CBRcf.PROB));

CBFcftest.OBS.LAI(2) = (CBRcf.POOLS(1,2,2) +  CBRcf.POOLS(1,1,2))/2/CBRcf.PARS(1,17)/2;
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with LAI(2) obs %2.2f (expected -0.5)',CBRcf.PROB));

%*************GPP for 809*****************

CBFcftest=CBFcf;
CBFcftest.OBS.GPP=CBF.MET(:,1)*-9999;
CBFcftest.OBS.GPP(2) = CBRcf.GPP(2);
CBFcftest.OBSUNC.GPP.gppabs=0;
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with GPP(2) obs %2.2f (expected 0)',CBRcf.PROB));

CBFcftest.OBS.GPP(2) = CBRcf.GPP(2)*1.23;
CBFcftest.OBSUNC.GPP.gppabs=1;
CBFcftest.OBSUNC.GPP.unc=1.23;
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with GPP(2) obs %2.2f (expected -0.5)',CBRcf.PROB));






