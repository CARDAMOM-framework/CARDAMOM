

%STep 1. Load CBF
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
save('CBRdumptest1.mat',CBR1);


