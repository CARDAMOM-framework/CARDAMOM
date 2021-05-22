

%This code runs a sequence of CARDAMOM benchmark tests with expected or
%known outcomes

cbffilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942.cbf';
cbrfilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942_1.cbr';

%Step 1. Run forward run
CBR=CARDAMOM_RUN_MODEL(cbffilename,cbrfilename);
CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffilename);

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

cbrtest='cardamom_integration_test.cbr';
delete('cardamom_integration_test.cbrSTART');
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
disp(sprintf('Cost function with zero obs %2.2f',CBRcf.PROB));









