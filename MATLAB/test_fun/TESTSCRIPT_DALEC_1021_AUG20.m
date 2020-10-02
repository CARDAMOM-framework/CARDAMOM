
%Loading generic CBF file
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');

%Changing ID
CBF.ID=1021;
CBF.EDC=0;
CBF.OBS.GPP=[];
CBF.OTHER_OBS.MLAI.mean=-9999;
CBF.OTHER_OBS.MLAI.unc=-9999;


%Worth doing during DALEC_1020 modificaitons, just in case, 
MD=CARDAMOM_MODEL_LIBRARY(1021,[],1);

%Trying out MDF
MCO.niterations=1e6;%1 million iterations OK 
CBR=CARDAMOM_RUN_MDF(CBF,MCO);

%For seg fault: "CARDAMOM_VALGRIND.m" (i) recompiles in debug mode, and (ii) returns extra seg fault info, and provides
%(somewhat) less cryptic error messages.
%
%CARDAMOM_VALGRIND(CBF);

%
figure(1);clf
subplot(2,1,1);
plotmultilines(CBR.LAI); hold on
ph=plot(CBF.OBS.LAI(2:end),'r','LineWidth',2);
legend(ph,'MODIS LAI');
ylabel('LAI [m2/m2]')
xlabel('Months');
subplot(2,2,3);hist(CBR.PARS(:,43));title(MD.parname{43});ylabel('Frequency')

