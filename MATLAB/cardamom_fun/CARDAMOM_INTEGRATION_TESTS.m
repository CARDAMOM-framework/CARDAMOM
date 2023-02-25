

%This code runs a sequence of CARDAMOM benchmark tests with expected or
%known outcomes
%cbffilename_old='CARDAMOM/DATA/MODEL_ID_1000_EXAMPLE.cbf';
% %delete(cbffilename);TEST_CARDAMOM_CBF_NETCDF_FORMAT(cbffilename_old,cbffilename);
% 
% nccbffilename1000='CARDAMOM/DATA/MODEL_ID_1000_EXAMPLE.cbf.nc';
% 
% 
% CBF1000=CARDAMOM_READ_NC_CBF_FILE(nccbffilename1000);
%  CBF1100=CBF1000(1);
%  CBF1100.ID.values=1100;
%  
%  nccbffilename1100='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLE.cbf.nc';
% 
%  CARDAMOM_WRITE_NC_CBF_FILE(CBF1100,nccbffilename1100);

%  cbrfilename1100='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLE.cbr';
% 
% CBR1100=CARDAMOM_RUN_MDF(nccbffilename1100,[],cbrfilename1100);
%CBR1100=CARDAMOM_RUN_MDF(nccbffilename1100,[],cbrfilename1100);

%Step 1. Run forward run
%This benchmark is obsolete, only applied to ID=1000
% %CBR_old=CARDAMOM_RUN_MODEL(cbffilename_old,cbrfilename);
% CBR=CARDAMOM_RUN_MODEL(nccbffilename1100,cbrfilename1100);
% 
% %Compare CBR against benchmarks
% load('CARDAMOM/DATA/CBR_PRE_NCDF_BENCHMARKS','CBRcbf','CBRnccbf')
% 
% disp('Numerical differences: expected to be ~0')
% disp(minmax(CBRnccbf.FLUXES-CBR.FLUXES))
% disp(minmax(CBRcbf.FLUXES-CBR.FLUXES))
% disp(minmax(CBRnccbf.POOLS-CBR.POOLS))
% disp(minmax(CBRcbf.POOLS-CBR.POOLS))




disp('DEMO files are created using CARDAMOM_v3_MAKE_EXAMPLE_NC_FILE_SEP21.m')


%%%%%*********Test 1 ************ 
nccbffilename1100='CARDAMOM/DATA/CARDAMOM_DEMO_DRIVERS_prototype.cbf.nc';
CBF=CARDAMOM_READ_NC_CBF_FILE(nccbffilename1100);
disp('Successfully read file using "CARDAMOM_READ_NC_CBF_FILE" ...')
%************ set all fields to NAN*****

nccbffilename1005='CARDAMOM/DATA/CARDAMOM_DEMO_DRIVERS_prototype_1005.cbf.nc';
nccbffilename821='CARDAMOM/DATA/CARDAMOM_DEMO_DRIVERS_prototype_821.cbf.nc';

 CBF1005=CBF;CBF1005.ID.values=1005;
 CBF821=CBF;CBF821.ID.values=821;
%  CBF.MCMCID.values=119;
%  CBF.MCMCID.nITERATIONS=1e6;

CARDAMOM_WRITE_NC_CBF_FILE(CBF821,nccbffilename821);
CARDAMOM_WRITE_NC_CBF_FILE(CBF1005,nccbffilename1005);

CBR1005=CARDAMOM_RUN_MDF(nccbffilename1005);
CBR821=CARDAMOM_RUN_MDF(nccbffilename821);

disp('valgrind ./CARDAMOM/C/projects/CARDAMOM_MDF/CARDAMOM_MDF_debug.exe CARDAMOM/DATA/CARDAMOM_DEMO_DRIVERS_prototype_1005.cbf.nc testout.cbr')




        
        
        %%%%%*********Test 2 ************ 
nccbffilename1100='CARDAMOM/DATA/CARDAMOM_DEMO_DRIVERS_prototype.cbf.nc';
CBF=CARDAMOM_READ_NC_CBF_FILE(nccbffilename1100);
disp('Successfully read file using "CARDAMOM_READ_NC_CBF_FILE" ...')
%************ set all fields to NAN*****

CBF.MCMCID.nITERATIONS=2e5;
CBF.MCMCID.nSAMPLES=2;
CBF.ID.values=1005;

% CBF.MCMCID.nITERATIONS=2e3;
% CBF.MCMCID.values=3;

        CBR=CARDAMOM_RUN_MDF(CBF);
        
        %save CBRtest2 CBR
        
        
        load CBRtest2 CBR
        %Rerun
        %CBR2=CARDAMOM_RUN_MODEL(CBF,CBR.PARS);
        
        cardamom_vvuq_nccbf_summary(CBF)
        
        plot(CBR.GPP); hold on; plot(CBF.GPP.values)
        plot(CBR.NBE); hold on; plot(CBF.NBE.values)
        plot(CBR.LAI); hold on; plot(CBF.LAI.values)

%Test model-data fusion for each timeseries.



        
        


%*********Try writing out
nccbftestfile='DUMPFILES/MODEL_ID_1100_TEST_ONLY.cbf.nc';
CARDAMOM_WRITE_NC_CBF_FILE(CBF,nccbftestfile);
disp('Successfully wrote file using "CARDAMOM_WRITE_NC_CBF_FILE" ...')


%Get single non-inf solution:





%Removing time-varying LAI
CBF.LAI.values=NaN;
%Adding mean LAI constraint
CBF.Mean_LAI.values=3;
CBF.Mean_LAI.unc=1.5;
CBF.Mean_LAI.opt_unc_type=1;


%first test is retrieving parameters. Skip only for partial testing
retrievepars=1;
if retrievepars==1
    
%     MCO.niterations=1;
%     MCO.samplerate=1;
     cbrfilename1100='DUMPFILES/MODEL_ID_1100_EXAMPLE.cbr.nc';
    %CBR=CARDAMOM_RUN_MDF(CBF1100,[],cbrfilename1100);
        CBR=CARDAMOM_RUN_MDF(CBF);
end


%1005 test

nccbffilename1100='CARDAMOM/DATA/CARDAMOM_DEMO_DRIVERS_prototype.cbf.nc';
CBF=CARDAMOM_READ_NC_CBF_FILE(nccbffilename1100);
CBF.EWT.values=CBF.EWT.values*NaN;
CBF.ET.values=CBF.ET.values*NaN;
CBF.GPP.values=CBF.GPP.values*NaN;
CBF.NBE.values=CBF.NBE.values*NaN;
CBF.LAI.values=CBF.LAI.values*NaN;
CBF.ABGB.values=CBF.ABGB.values*NaN;
CBF.Mean_Biomass.values=CBF.Mean_Biomass.values*NaN;
CBF.Mean_Fire.values=CBF.Mean_Fire.values*NaN;
CBF.Mean_LAI.values=CBF.Mean_LAI.values*NaN;
CBF.Mean_GPP.values=CBF.Mean_GPP.values*NaN;
CBF.EDC.values=0;
CBF1005=CBF;
CBF1005.ID.values=1005;
CBF1005.MCMCID.nITERATIONS=1e6;
CBR1005=CARDAMOM_RUN_MDF(CBF1005);
        
        









 cbrfilename1100='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLE.cbr';
 cbrfilename1100ref='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLEref.cbr';
 cbrfilename1100refmat='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLEref.cbr.mat';

 ncdisp(nccbffilename1100)
nccbftestfile='CARDAMOM/DATA/MODEL_ID_1100_TEST_ONLY.cbf.nc';


%testing matlab read-write functions

CBF=CARDAMOM_READ_NC_CBF_FILE(nccbffilename1100);

CBF.EWT.values=CBF.EWT.values*NaN;
CBF.ABGB.values=CBF.ABGB.values*NaN;
CBF.NBE.values=CBF.NBE.values*NaN;
CBF.LAI.values=CBF.LAI.values*NaN;
CBF.Mean_Biomass.values=CBF.Mean_Biomass.values*NaN;
CBF.Mean_Fire.values=CBF.Mean_Fire.values*NaN;
CBF.Mean_LAI.values=CBF.Mean_LAI.values*NaN;
CBF.Mean_GPP.values=CBF.Mean_GPP.values*NaN;
CBF.PARPRIORS.values=CBF.PARPRIORS.values*NaN;
CBF.OTHERPRIORS.values=CBF.OTHERPRIORS.values*NaN;
CBF.PARPRIORUNC.values=CBF.PARPRIORUNC.values*NaN;
CBF.OTHERPRIORSUNC.values=CBF.OTHERPRIORSUNC.values*NaN;
CBF.EDC.values=0;



CARDAMOM_WRITE_NC_CBF_FILE(CBF,nccbftestfile);
%CARDAMOM_RUN_MDF(nccbftestfile)
CBF1100test=CARDAMOM_READ_NC_CBF_FILE(nccbftestfile);
 




updatecbrref=1;
 if updatecbrref==1
CARDAMOM_RUN_MDF(CBF1100test,[],cbrfilename1100ref);
 %save(cbrfilename1100refmat, 'CBRref');
 end


updateCBROUTref=1;
 if updateCBROUTref==1
 copyfile( cbrfilename1100, cbrfilename1100ref);
  CBRref=CARDAMOM_RUN_MODEL(nccbftestfile,cbrfilename1100ref);
 save(cbrfilename1100refmat, 'CBRref');
 end


f=fields(CBF);cbfioerror=0;
for n=1:numel(f)
    cbffielddif=nansum(CBF1100test.(f{n}).values- CBF.(f{n}).values);
    if cbffielddif~=0;disp(sprintf('CBF.%s: Warning, non-zero dif',f{n}));cbfioerror=1;end
end
if cbfioerror==0; disp('CARDAMOM READ/WRITE NETCDF functions check out...');
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
%copyfile(nccbffilename1100,nccbftestfile);


%cbrfilename1100='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLE.cbr';







%CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffilename);



 CBR=CARDAMOM_RUN_MODEL(nccbftestfile,cbrfilename1100);
load(cbrfilename1100refmat, 'CBRref');

% 
% %Compare CBR against benchmarks 
 disp('Numerical differences: expected to be ~0')
 disp(minmax(CBR.FLUXES-CBRref.FLUXES))
 disp(minmax(CBR.POOLS-CBRref.POOLS))
 disp(minmax(CBR.PARS-CBRref.PARS))
 disp(minmax(CBR.PROB-CBRref.PROB))


 

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


CBF=CARDAMOM_READ_BINARY_FILEFORMAT(nccbffilename1100);
CBR=CARDAMOM_RUN_MODEL(CBF,cbrfilename1000);

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



