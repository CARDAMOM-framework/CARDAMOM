

%STep 1. Load CBF (this has proven to be a "worst-case-scenario")
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CBF_FILE_BLOOM2020_LE_1943.cbf');


%Step 2. Set up and run DEMCMC
MCO.niterations=1000000;
MCO.printrate=10000;
MCO.samplerate=MCO.niterations/2000*100;
MCO.mcmcid=3;
CBF1=CBF;
CBF1.EDC=1;
for n=1:2;
tic;CBR1(n)=CARDAMOM_RUN_MDF(CBF1,MCO);toc
end
save('CBRdumptest1.mat','CBR1');

figure(1);clf;hold on
plot(mean(CBR1(1).NBE),'r')
plot(mean(CBR1(2).NBE),'b')

figure(2);clf
PARSALL(:,:,1)=CBR1(1).PARS;
PARSALL(:,:,2)=CBR1(2).PARS;
PARSALL(:,[12,15],:)=mod(PARSALL(:,[12,15],:),365.25)+365.25;

MD=CARDAMOM_MODEL_LIBRARY(CBF.ID);
NPARSall=MD.par2nor(PARSALL);

gr=mcmc_psrf(log(NPARSall));
for n=1:33;
    subplot(6,6,n);hold on
    plotpdf(NPARSall(:,n,1),'b');
    plotpdf(NPARSall(:,n,2),'r');
    text(0.97,0.9,sprintf('%2.2f',gr(n)),'Units','normalized','HorizontalAlignment','right','Color','r');
end