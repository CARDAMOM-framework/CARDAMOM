

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




