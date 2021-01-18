
%Loading generic CBF file
% CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');

%Changing ID
CBF.ID=1000;
CBF.EDC=0;
CBF.OBS.GPP=[];
CBF.OTHER_OBS.MLAI.mean=-9999;
CBF.OTHER_OBS.MLAI.unc=-9999;


%Worth doing during DALEC_1020 modificaitons, just in case, 
MD=CARDAMOM_MODEL_LIBRARY(1000,[],1);

%Trying out MDF
MCO.niterations=5e6;%1 million iterations OK 
CBR=CARDAMOM_RUN_MDF(CBF,MCO);

%For seg fault: "CARDAMOM_VALGRIND.m" (i) recompiles in debug mode, and (ii) returns extra seg fault info, and provides
%(somewhat) less cryptic error messages.
%
%CARDAMOM_VALGRIND(CBF);


%Make figure
figure(1);clf
subplot(1,1,1);
plotmultilines(CBR.LAI); hold on
ph=plot(CBF.OBS.LAI(2:end),'k','LineWidth',2);
legend(ph,'MODIS LAI');
ylabel('LAI [m2/m2]')
xlabel('Months');
%subplot(2,2,3);hist(CBR.PARS(:,43));title(MD.parname{43});ylabel('Frequency')


% Interannual variability in LAI
nsamples=size(CBR.LAI,1);
nyears=size(CBR.LAI,2)/12;
lai_iav_model=zeros(nsamples,nyears);
lai_iav_obs=zeros(nyears,1);
for t=1:6;
    t0=1+(t-1)*12;
    t1=t*12;
    lai_iav_model(:,t)=mean(CBR.LAI(:,t0:t1),2);
    lai_iav_obs(t)=mean(CBF.OBS.LAI(t0:t1),1);
end
figure(2);clf
subplot(1,1,1);
plotmultilines(lai_iav_model); hold on
p1=plot(lai_iav_obs,'k','LineWidth',2);
p2=plot(median(lai_iav_model,1),'LineWidth',2);
legend([p1 p2], {'MODIS LAI', 'Model median'});
ylabel('LAI IAV [m2/m2]')
xlabel('Years');


