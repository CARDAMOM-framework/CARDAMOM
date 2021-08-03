

%This code runs a sequence of CARDAMOM benchmark tests with expected or
%known outcomes
%cbffilename_old='CARDAMOM/DATA/MODEL_ID_1000_EXAMPLE.cbf';
%delete(cbffilename);TEST_CARDAMOM_CBF_NETCDF_FORMAT(cbffilename_old,cbffilename);

nccbffilename='CARDAMOM/DATA/MODEL_ID_1000_EXAMPLE.nc.cbf';
ncdisp(nccbffilename)
nccbftestfile='CARDAMOM/DATA/MODEL_ID_1000_TEST_ONLY.nc.cbf';
nccbftestfile2='CARDAMOM/DATA/MODEL_ID_1000_TEST_ONLY_2.nc.cbf';


%testing matlab read-write functions

CBF=CARDAMOM_READ_NC_CBF_FILE(nccbffilename);
CARDAMOM_WRITE_NC_CBF_FILE(CBF,nccbftestfile2);
CBF(2)=CARDAMOM_READ_NC_CBF_FILE(nccbftestfile2);

f=fields(CBF);cbfioerror=0;
for n=1:numel(f)
    cbffielddif=nansum(CBF(2).(f{n}).values- CBF(1).(f{n}).values);
    if cbffielddif~=0;disp(sprintf('CBF.%s: Warning, non-zero dif',f{n}));cbfioerror=1;end
end
if cbfioerror==0;
    
    
    
    
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
    disp('CARDAMOM_*_NC_CBF_FILE() commands check complete')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
disp('**********')
end


%Converting to netcdf file
%Make a copy
copyfile(nccbffilename,nccbftestfile);


cbrfilename='CARDAMOM/DATA/MODEL_ID_1000_EXAMPLE.cbr';







%CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffilename);




%Step 1. Run forward run
%CBR_old=CARDAMOM_RUN_MODEL(cbffilename_old,cbrfilename);
CBR=CARDAMOM_RUN_MODEL(nccbffilename,cbrfilename);

%Compare CBR against benchmarks
load('CARDAMOM/DATA/CBR_PRE_NCDF_BENCHMARKS','CBRcbf','CBRnccbf')

disp('Numerical differences: expected to be ~0')
disp(minmax(CBRnccbf.FLUXES-CBR.FLUXES))
disp(minmax(CBRcbf.FLUXES-CBR.FLUXES))
disp(minmax(CBRnccbf.POOLS-CBR.POOLS))
disp(minmax(CBRcbf.POOLS-CBR.POOLS))



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




mdf_tests=0;
if mdf_tests==1;
cbrtest='cardamom_integration_test.cbr';
delete('cardamom_integration_test.cbr');
delete('cardamom_integration_test.cbrSTART');



CBRtest=CARDAMOM_RUN_MDF(nccbftestfile,[],cbrtest);


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
CBR=CARDAMOM_RUN_MDF(nccbftestfile,MCO);



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


end
%COst function tests
%Step 1. Use existing solution
%cbffilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942.cbf';
%cbrfilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942_1.cbr';

%Step 1. Run forward run
%CBR=CARDAMOM_RUN_MODEL(cbffilename,cbrfilename);cbffilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942.cbf';
%cbrfilename='CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942_1.cbr';

%Step 1. Generate some output (since 813 not available yet).
%CBF=CARDAMOM_CBF_TEMPLATE(1000);

%CARDAMOM_WRITE_BINARY_FILEFORMAT(CBF,cbffilename);
%CBR=CARDAMOM_RUN_MDF(CBF,[],cbrfilename);


cost_function_tests=0;

if cost_function_tests==1;


CBF=CARDAMOM_READ_BINARY_FILEFORMAT(nccbffilename);
CBR=CARDAMOM_RUN_MODEL(CBF,cbrfilename);

CBFcf=cardamomfun_clear_cbf_obs(CBF);clear CBF;
cfpars=CBR.PARS(end,:);
CBRcf=CARDAMOM_RUN_MODEL(CBFcf,cfpars);clear CBR;


CBFall=CBFcf;
%Add all fields
CBFall.PARPRIORS(1:numel(cfpars))=cfpars;
CBFall.PARPRIORUNC(1:numel(cfpars))=2;
CBFall.OBS.ABGB=zeros(72,1)-9999;
CBFall.OBS.ABGB(1)=sum(squeeze(CBRcf.POOLS(1,1,1:4))'/2+CBRcf.PARS(1,18:21)/2);
CBFall.OBS.ABGB(2:CBFall.nodays,1)=sum(CBRcf.POOLS(1,1:end-1,1:4)/2+CBRcf.POOLS(1,2:end,1:4)/2,3)';
 CBFall.OBS.SOM(1)=sum(squeeze(CBRcf.POOLS(1,1,5:6))'/2+CBRcf.PARS(1,22:23)/2);
 CBFall.OBS.SOM(2:CBFall.nodays,1)=sum(CBRcf.POOLS(1,1:end-1,5:6)/2+CBRcf.POOLS(1,2:end,5:6)/2,3)';


CBFall.OBS.EWT=zeros(72,1)-9999;CBFall.OBS.EWT(1)=CBRcf.EWT(1,1);
CBFall.OBS.ET=CBRcf.ET(1,:)';
CBFall.OBS.LAI(1)=sum(squeeze(CBRcf.POOLS(1,1,2))'/2+CBRcf.PARS(1,19)/2)/CBRcf.PARS(1,17);
CBFall.OBS.LAI(2:CBFall.nodays,1)=sum(CBRcf.POOLS(1,1:end-1,2)/2+CBRcf.POOLS(1,2:end,2)/2,3)'/CBRcf.PARS(1,17);

 CBFall.OBS.NBE=zeros(72,1)-9999;
 CBFall.OBS.NBE(1)=CBRcf.NBE(1);
 CBFall.OBS.GPP=CBRcf.GPP(1,:)';

 
CBRall=CARDAMOM_RUN_MODEL(CBFall,cfpars);
CBRall.PROB



k=1;


disp(sprintf('Cost function with all obs %2.2f (expected 0)',CBRcf.PROB));
ALL_TESTS(k,:)=[ 0, CBRcf.PROB];TEST_NAMES{k}='All obs';k=k+1;

CBFall=CBFcf;
CBRall=CARDAMOM_RUN_MODEL(CBFall,cfpars);

%Step 2. COst function with zero obs
disp(sprintf('Cost function with zero obs %2.2f (expected 0)',CBRcf.PROB));
ALL_TESTS(k,:)=[ 0, CBRcf.PROB];TEST_NAMES{k}='No obs';k=k+1;

%*************LAI for 809*****************
%LAI, no change
CBFcftest=CBFcf;
CBFcftest.OBS.LAI=CBFcf.MET(:,1)*-9999;
CBFcftest.OBS.LAI(2) = (CBRcf.POOLS(1,2,2) +  CBRcf.POOLS(1,1,2))/2/CBRcf.PARS(1,17);
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with LAI(2) obs %2.2f (expected 0)',CBRcf.PROB));
ALL_TESTS(k,:)=[ 0, CBRcf.PROB];TEST_NAMES{k}='LAI, 1 obs, no offset';k=k+1;


%LAI 1-sigma change
CBFcftest.OBS.LAI(2) = (CBRcf.POOLS(1,2,2) +  CBRcf.POOLS(1,1,2))/2/CBRcf.PARS(1,17)*2;
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with LAI(2) obs %2.2f (expected -0.5)',CBRcf.PROB));

ALL_TESTS(k,:)=[ -0.5, CBRcf.PROB];TEST_NAMES{k}='LAI, 1 obs, + 1-delta offset';k=k+1;


CBFcftest.OBS.LAI(2) = (CBRcf.POOLS(1,2,2) +  CBRcf.POOLS(1,1,2))/2/CBRcf.PARS(1,17)/2;
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with LAI(2) obs %2.2f (expected -0.5)',CBRcf.PROB));
ALL_TESTS(k,:)=[ -0.5, CBRcf.PROB];TEST_NAMES{k}='LAI, 1 obs, - 1-delta offset';k=k+1;




%*************GPP for 809*****************

CBFcftest=CBFcf;
CBFcftest.OBS.GPP=CBFcf.MET(:,1)*-9999;
CBFcftest.OBS.GPP(2) = CBRcf.GPP(2);
CBFcftest.OBSUNC.GPP.gppabs=0;
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with GPP(2) obs %2.2f (expected 0)',CBRcf.PROB));
ALL_TESTS(k,:)=[ 0, CBRcf.PROB];TEST_NAMES{k}='GPP, 1 obs, 0-delta offset';k=k+1;


CBFcftest.OBSUNC.GPP.unc=1.23;
CBFcftest.OBS.GPP(2) = CBRcf.GPP(2)*CBFcftest.OBSUNC.GPP.unc;
CBFcftest.OBSUNC.GPP.gppabs=1;
CBRcf=CARDAMOM_RUN_MODEL(CBFcftest,cfpars);
disp(sprintf('Cost function with GPP(2) obs %2.2f (expected -0.5)',CBRcf.PROB));
ALL_TESTS(k,:)=[ -0.5, CBRcf.PROB];TEST_NAMES{k}='GPP, 1 obs, 1-delta offset';k=k+1;


end



