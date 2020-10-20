
%Loading generic CBF file
% CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('/Users/jnorton/Models/CARDAMOM_2.1.6c/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');

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


% Check carbon balance (ignoring fire losses by setting burned area to zero)
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
subplot(1,1,1);
plotmultilines(CBR.LAI); hold on
ph=plot(CBF.OBS.LAI(2:end),'k','LineWidth',2);
legend(ph,'MODIS LAI');
ylabel('LAI [m2/m2]')
xlabel('Months');
%subplot(2,2,3);hist(CBR.PARS(:,43));title(MD.parname{43});ylabel('Frequency')


fancy_labels = {'Initial LAI', '$T_{\phi}$', '$T_r$', '$T_{\phi}$ averaging period', ... 
    '$\xi$', '$k_L$', '$\hat{\Lambda}$', '$\tau_W$', '$t_c$', '$t_r$', '$tau_{foliar}$'};
fancy_labels_units = {'(m2 m$^{-2}$)', '($^{\circ}$C)', '($^{\circ}$C)', '(months)', ...
    '(days$^{-1}$)', '(days$^{-1}$)', 'm$^2$ m$^{-2}$)', '(days)', '(hrs day$^{-1}$)', '(hrs day$^{-1}$)', '(days)'};

%Make figure
npars_knorr=11;
figure(3);clf
for s=1:npars_knorr;
    subplot(3,4,s);
    %hist(CBR.PARS(:,s+36));title(MD.parname{s+36});ylabel('Frequency')
    hist(CBR.PARS(:,s+36));title([fancy_labels{s}, ' ', fancy_labels_units{s}], 'Interpreter', 'latex');ylabel('Frequency')
end


% Test the degree of water limitation on LAI
% first, we do a control run with optimized parameters
partest1=CBR.PARS(:,:);
cbrtest1=CARDAMOM_RUN_MODEL(CBF,partest1);
% second, we do a test run with no water limitation, by setting tau_W to near zero
partest2=CBR.PARS(:,:);
partest2(:,44)=1e-6;
cbrtest2=CARDAMOM_RUN_MODEL(CBF,partest2);

figure(4);clf
subplot(2,1,1);
plot(median(cbrtest1.LAI,1),'b','LineWidth',1.5,'DisplayName','median LAI: water limitation');hold on;
plot(median(cbrtest2.LAI,1),'r','LineWidth',1.5,'DisplayName','median LAI: no water limitation');hold off;
legend;
ylabel({'LAI';'(m2 m$^{-2}$)'}, 'Interpreter', 'latex');
xlabel('Months');
subplot(2,1,2);
diff=cbrtest2.LAI - cbrtest1.LAI;
p2=plot(median(diff,1),'k','LineWidth',1.5,'DisplayName','median difference');hold on;
p3=plot(mean(diff,1),'--k','LineWidth',1.5,'DisplayName','mean difference'); hold off;
legend([p2, p3]);
ylabel({'Reduction in LAI';'from water limitation'; '(m$^2$ m$^{-2}$)'}, 'Interpreter', 'latex');
xlabel('Months');


% Test the degree of temperature and day length limitation on LAI
% for this we use the variables f_T and f_d in the Knorr model that
% determine the fraction of plants within the grid cell above the
% temperature and day length thresholds, respectively
figure(4);clf
subplot(2,1,1);
f_T=CBR.FLUXES(:,:,43);
plotmultilines(f_T); hold on
plot(median(f_T,1),'LineWidth',1.5,'DisplayName','f_T');hold on;
ylim([0 1.05]);
ylabel({'Fraction of plants above';'temperature threshold'});
xlabel('Months');
subplot(2,1,2);
f_d=CBR.FLUXES(:,:,44);
plotmultilines(f_d); hold on
plot(median(f_d,1),'LineWidth',1.5,'DisplayName','f_d');hold on;
ylim([0 1.05]);
ylabel({'Fraction of plants above';'day length threshold'});
xlabel('Months');

