



nccbffilename1100='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLE.nc.cbf';
 cbrfilename1100='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLE.cbr';
 cbrfilename1100ref='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLEref.cbr';
 cbrfilename1100refmat='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLEref.cbr.mat';

 ncdisp(nccbffilename1100)
nccbftestfiledev='CARDAMOM/DATA/MODEL_ID_1100_TEST_ONLY_DEV.nc.cbf';
nccbftestfiledev_noobs='CARDAMOM/DATA/MODEL_ID_1100_TEST_ONLY_DEV_NOOBS.nc.cbf';


%testing matlab read-write functions

CBF1100dev0=CARDAMOM_READ_NC_CBF_FILE(nccbffilename1100);

CBF1100dev0.EWT.values=CBF1100dev0.EWT.values*NaN;
CBF1100dev0.ET.values=CBF1100dev0.ET.values*NaN;
CBF1100dev0.Mean_Biomass.values=CBF1100dev0.Mean_Biomass.values*NaN;
CBF1100dev0.Mean_Fire.values=CBF1100dev0.Mean_Fire.values*NaN;
CBF1100dev0.Mean_LAI.values=CBF1100dev0.Mean_LAI.values*NaN;
CBF1100dev0.Mean_GPP.values=CBF1100dev0.Mean_GPP.values*NaN;
CBF1100dev0.PARPRIORS.values=CBF1100dev0.PARPRIORS.values*NaN;
CBF1100dev0.OTHERPRIORS.values=CBF1100dev0.OTHERPRIORS.values*NaN;
CBF1100dev0.PARPRIORUNC.values=CBF1100dev0.PARPRIORUNC.values*NaN;
CBF1100dev0.OTHERPRIORSUNC.values=CBF1100dev0.OTHERPRIORSUNC.values*NaN;
CBF1100dev0.EDC.values=0;








CARDAMOM_WRITE_NC_CBF_FILE(CBF1100dev0,nccbftestfiledev);
CBF1100dev0=CARDAMOM_READ_NC_CBF_FILE(nccbftestfiledev);
 updateref=0;
 if updateref==1
 copyfile( cbrfilename1100, cbrfilename1100ref);
  CBRref=CARDAMOM_RUN_MODEL(nccbftestfiledev,cbrfilename1100ref);
 save(cbrfilename1100refmat, 'CBRref');
 end
load(cbrfilename1100refmat, 'CBRref');


f=fields(CBF1100dev0);cbfioerror=0;
for n=1:numel(f)
    cbffielddif=nansum(CBF1100dev0.(f{n}).values- CBF1100dev0.(f{n}).values);
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




%Add ET
% 
% OBS_STRUCT OBS;
% OBS.unc = ncdf_read_double_var(ncid, strcat(OBSNAME,"unc") , &(OBS.length));
% OBS.values = ncdf_read_double_var(ncid, OBSNAME , &(OBS.length));
% OBS.opt_log_transform=ncdf_read_int_attr(ncid, OBSNAME,"opt_log_transform");
% OBS.opt_normalization=ncdf_read_int_attr(ncid, OBSNAME,"opt_normalization");
% OBS.opt_mean_only=ncdf_read_int_attr(ncid, OBSNAME,"opt_mean_only");
% OBS.opt_structural_error=ncdf_read_int_attr(ncid, OBSNAME,"opt_structural_error");
% OBS.min_threshold_value=ncdf_read_double_attr(ncid, OBSNAME,"min_threshold_value");
% OBS.single_monthly_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_monthly_unc");
% OBS.single_annual_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_annual_unc");
% OBS.structural_unc=ncdf_read_double_attr(ncid, OBSNAME,"structural_unc");
% return OBS;
% };


CBF1100dev_noobs=CBF1100dev0;
CBF1100dev=CBF1100dev0;
CBF1100dev.ET.values=CBRref.FLUXES(1,:,29);
CBF1100dev.ETunc.values=CBRref.FLUXES(1,:,29)*0.1;
CBF1100dev.ET.Attributes(3).Name='opt_unc_type';
CBF1100dev.ET.Attributes(3).Value=0;
CBF1100dev.ET.Attributes(4).Name='opt_normalization';
CBF1100dev.ET.Attributes(4).Value=0;
CBF1100dev.ET.Attributes(5).Name='opt_mean_only';
CBF1100dev.ET.Attributes(5).Value=0;
CBF1100dev.ET.Attributes(6).Name='opt_structural_error';
CBF1100dev.ET.Attributes(6).Value=0;
CBF1100dev.ET.Attributes(7).Name='opt_filter';
CBF1100dev.ET.Attributes(7).Value=2;



% typedef struct OBS_STRUCT{
% size_t length;
% double * values;
% size_t unc_length;
% double * unc;
% int opt_unc_type;//log-transform data 
% int opt_normalization;//(0 = none, 1 = remove mean, 2 = divide by mean)
% int opt_filter;//(0 = no filter, 1 = mean only, 2==annual mean & monthly anomaly, 3 = seasonal cycle & inter-annual anomalies). 
% int opt_structural_error;
% int opt_mean_only;
% double min_threshold_value;
% double single_monthly_unc;
% double single_annual_unc;
% double single_mean_unc;
% double single_unc;
% double structural_unc;
% //expand as needed
% int valid_obs_length;//number of non-empty obs
% int * valid_obs_indices;//indices of non-empty obs
% }OBS_STRUCT;
% 



% 
% CARDAMOM_WRITE_NC_CBF_FILE(CBF1100dev,nccbftestfiledev);
% CARDAMOM_WRITE_NC_CBF_FILE(CBF1100dev_noobs,nccbftestfiledev_noobs);

 CBR=CARDAMOM_RUN_MODEL(CBF1100dev,cbrfilename1100);
 CBR_noobs=CARDAMOM_RUN_MODEL(CBF1100dev_noobs,cbrfilename1100);


 
 
 
 
 
%CHeck ET 
pts=[1,2,4,5];
total(-0.5*(CBF1100dev.ET.values(pts)-ET1(pts)').^2./CBF1100dev.ETunc.values(pts).^2)


%Converting to netcdf file
%Make a copy
%copyfile(nccbffilename1100,nccbftestfile);


%cbrfilename1100='CARDAMOM/DATA/MODEL_ID_1100_EXAMPLE.cbr';







%CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffilename);




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



CBRtest=CARDAMOM_RUN_MDF(nccbftestfiledev,[],cbrtest);


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
CBR=CARDAMOM_RUN_MDF(nccbftestfiledev,MCO);



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


CBF1100dev=CARDAMOM_READ_BINARY_FILEFORMAT(nccbffilename1100);
CBR=CARDAMOM_RUN_MODEL(CBF1100dev,cbrfilename1000);

CBFcf=cardamomfun_clear_cbf_obs(CBF1100dev);clear CBF;
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



