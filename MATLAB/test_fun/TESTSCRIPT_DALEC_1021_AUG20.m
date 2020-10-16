
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

%Short test
MCO.niterations=1e4;%1 million iterations OK 
CBRtest=CARDAMOM_RUN_MDF(CBF,MCO);

%Trying out MDF
MCO.niterations=1e6;%1 million iterations OK 
CBR=CARDAMOM_RUN_MDF(CBF,MCO);

%For seg fault: "CARDAMOM_VALGRIND.m" (i) recompiles in debug mode, and (ii) returns extra seg fault info, and provides
%(somewhat) less cryptic error messages.
%
%CARDAMOM_VALGRIND(CBF);



% Check carbon balance
partest=CBRtest.PARS(end,:);
CBFtest=CBF;
CBFtest.MET(:,7)=0;

cbrtest=CARDAMOM_RUN_MODEL(CBFtest,partest);


deltat=CBF.MET(2,1)-CBF.MET(1,1);
NBE=cbrtest.NBE;
%NBE is - dC/dt if "C" is total carbon
NBEdpools=-diff(sum(cbrtest.POOLS(:,:,1:6),3))/deltat;

figure(2);clf
%subplot(3,3,1);plot(NBEdpools,NBE(2:end),'o-')
subplot(3,1,1);hold on;plot(NBE(2:end),'b-');plot(NBEdpools,'r--')
subplot(3,1,2);hold on;plot(cbrtest.POOLS(:,:,2))
subplot(3,1,3);hold on;plot(cbrtest.LAI)



for s=1:6;
    subplot(3,3,s+3);
    plot(cbrtest.POOLS(:,:,s))
end




%Make figure
figure(1);clf
subplot(2,1,1);
plotmultilines(CBR.LAI); hold on
ph=plot(CBF.OBS.LAI(2:end),'r','LineWidth',2);
legend(ph,'MODIS LAI');
ylabel('LAI [m2/m2]')
xlabel('Months');
subplot(2,2,3);hist(CBR.PARS(:,43));title(MD.parname{43});ylabel('Frequency')

