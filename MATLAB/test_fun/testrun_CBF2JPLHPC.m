
% %Loading generic CBF file
% CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');

%experiment CBF and name
% expname='BOREAL3';
% CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/BOREAL_EXAMPLE_155E_66S.cbf');
expname='TEMPERATE3';
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/TEMPERATE_EXAMPLE_90W_50N.cbf');
expname='TROPICS3';
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/WET_TROPICS_EXAMPLE_70W_2S.cbf');
% expname='DRYTROPICS1';
% CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/DRY_TROPICS_EXAMPLE_25E_18S.cbf');

%Changing ID
CBF.ID=1021;
CBF.EDC=0;
CBF.OBS.GPP=[];
CBF.OTHER_OBS.MLAI.mean=-9999;
CBF.OTHER_OBS.MLAI.unc=-9999;


%Worth doing following any modification to the given model ID, just in case, 
MD=CARDAMOM_MODEL_LIBRARY(1021,[],1);

%For PARRFUN_CARDAMOM_CBF2JPLHPC.m
OPT.username='jnorton';
OPT.niterations=1e7;
OPT.timefactor=4;

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