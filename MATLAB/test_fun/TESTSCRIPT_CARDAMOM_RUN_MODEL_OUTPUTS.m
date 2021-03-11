cbffile='/Users/abloom/CARDAMOM_DISK/CBF_FILES/GCRUN_JUN20_TDLE/GCRUN_JUN20_TDLE_1942.cbf';
cbrfile='/Users/abloom/CARDAMOM_DISK/PROJECTS/GCRUN_JUN20_TDLE/cbr_files/GCRUN_JUN20_TDLE_1942_1.cbr';

%Run CARDAMOM with existing driver files (.cbf) and results from previous
%analyses (.cbr)
CBR=CARDAMOM_RUN_MODEL(cbffile,cbrfile);

%Make some plots
figure(1);clf
subplot(3,1,1);
plotmultilines(CBR.NBE)
subplot(3,1,2);
plotmultilines(CBR.GPP)
subplot(3,1,3);
plotmultilines(CBR.LAI)

%Load driver file (in case we want to make changes)
CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffile);


%"Step 3." write some outputs to file

GPP=CBR.GPP;
save('mycardamomoutputs.txt','GPP','-ascii')