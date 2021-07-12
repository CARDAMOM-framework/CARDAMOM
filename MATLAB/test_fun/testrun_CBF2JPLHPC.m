
% %Loading generic CBF file
% CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');

%experiment CBF and name
% expname='BOREAL6';
% CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/BOREAL_EXAMPLE_155E_66S.cbf');
expname='TEMPERATE6';
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/TEMPERATE_EXAMPLE_90W_50N.cbf');
expname='TROPICS6';
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/WET_TROPICS_EXAMPLE_70W_2S.cbf');
expname='DRYTROPICS6';
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/DRY_TROPICS_EXAMPLE_25E_18S.cbf');
expname='DRYTROPICS1021b';
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/DRY_TROPICS_EXAMPLE_25E_18S.cbf');


%Changing ID
CBF.ID=1020;
CBF.EDC=0;
CBF.OBS.GPP=[];
CBF.OTHER_OBS.MLAI.mean=-9999;
CBF.OTHER_OBS.MLAI.unc=-9999;


%Worth doing following any modification to the given model ID, just in case, 
MD=CARDAMOM_MODEL_LIBRARY(1020,[],1);

%For PARRFUN_CARDAMOM_CBF2JPLHPC.m
OPT.username='jnorton';
OPT.niterations=1e7;
OPT.timefactor=5;

CBR=PARRFUN_CARDAMOM_CBF2JPLHPC(CBF,expname,OPT);

%% Plotting some key results

isubsample0=1;
isubsample1=1000;
LAI0median=median(CBR{1,1}.LAI(isubsample0:isubsample1,:),1);
isubsample0=1001;
isubsample1=2000;
LAI1median=median(CBR{1,1}.LAI(isubsample0:isubsample1,:),1);
isubsample0=2001;
isubsample1=3000;
LAI2median=median(CBR{1,1}.LAI(isubsample0:isubsample1,:),1);
isubsample0=3001;
isubsample1=4000;
LAI3median=median(CBR{1,1}.LAI(isubsample0:isubsample1,:),1);
LAIobs=CBF.OBS.LAI;

figure(1);clf
subplot(1,1,1);
ph=plot(LAI0median); hold on
plot(LAI1median);
plot(LAI2median);
plot(LAI3median);
plot(LAIobs,'k','LineWidth',1.5);
legend('MODEL 1','MODEL 2','MODEL 3','MODEL 4','MODIS LAI');
ylabel('LAI [m2/m2]')
xlabel('Months');
ylim([0 max(CBF.OBS.LAI)*1.5]);


%% Writing CBR results to csv

% PARS
cHeader = MD.parname; %dummy header
commaHeader = [cHeader;repmat({','},1,numel(cHeader))]; %insert commaas
commaHeader = commaHeader(:)';
textHeader = cell2mat(commaHeader); %cHeader in text with commas

%write header to file
fid = fopen(sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_PARS.csv', expname),'w'); 
fprintf(fid,'%s\n',textHeader)
fclose(fid)
%write data to end of file
dlmwrite(sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_PARS.csv', expname),CBR{1,1}.PARS,'-append');

% PARS min and max arrays
writematrix(MD.parmin,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_PARMIN.csv', expname));
writematrix(MD.parmax,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_PARMAX.csv', expname));

% CBR PROB
writematrix(CBR{1,1}.PROB,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_PROB.csv', expname));

% chainid
writematrix(CBR{1,1}.chainid,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_chainid.csv', expname));

% CBR LAI (modeled)
writematrix(CBR{1,1}.LAI,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_LAI.csv', expname));
% CBF LAI (observed)
writematrix(CBF.OBS.LAI,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_LAIOBS.csv', expname));

% CBR FLUXES
writematrix(CBR{1,1}.FLUXES,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_FLUXES.csv', expname));
% CBR POOLS
writematrix(CBR{1,1}.POOLS,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_POOLS.csv', expname));

% CBF MET
writematrix(CBF.MET,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_MET.csv', expname));


%% Test the degree of water limitation on LAI
% first, we do a control run with optimized parameters
partest1=CBR{1,1}.PARS(:,:);
cbrtest1=CARDAMOM_RUN_MODEL(CBF,partest1);
% second, we do a test run with no water limitation, by setting tau_W to near zero
partest2=CBR{1,1}.PARS(:,:);
partest2(:,44)=1e-6;
cbrtest2=CARDAMOM_RUN_MODEL(CBF,partest2);


% CBR FLUXES
writematrix(cbrtest2.FLUXES,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_FLUXES_sensitivitytauw.csv', expname));
% CBR POOLS
writematrix(cbrtest2.POOLS,sprintf('/Users/jnorton/Projects/CARDAMOM/Phenology/results/%s_POOLS_sensitivitytauw.csv', expname));

