%NBE 

%Defintions 
% NBE = Rh + Rauto + fires - GPP

%Step 1. Reading some outputs

%Driver file
cbffile='/Users/abloom/CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942.cbf';
%FIle with optimized parameters
cbrfile='/Users/abloom/CARDAMOM/DATA/GCRUN_JUN20_TDLE_1942_1.cbr';

%Step 2. Step Run CARDAMOM with existing driver files (.cbf) and results from previous
%analyses (.cbr) "Forward mode"
%Load the driver structure into matlab
CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffile);
%Rerun with drivers and parameterw
CBR=CARDAMOM_RUN_MODEL(CBF,cbrfile);



%Step 3. Perturb nutrient levels (via canopy efficiency parameter).
%"CBR.PARS"
%rows = parameter samples generated from cardamom (1000 samples)
%columns = parameters (33 parameters)


%Step 3.1 Feed model ID to "CARDAMOM_MODEL_LIBRARY" to get parameter
%details
MD=CARDAMOM_MODEL_LIBRARY(CBF.ID);


%Step 3.2, extract relevant parameters
LCMA=CBR.PARS(:,17);
LL=CBR.PARS(:,5);
Ceff = CBR.PARS(:,11);

%step 3.3 
%perturb parameters for sensitivity test
PARS=CBR.PARS;
PARS1=PARS;

Ceff1=Ceff*1.0001;%Increase CE by 1%

PARS1(:,11)=Ceff1;
%This is the sensitivity test outcome
CBR1=CARDAMOM_RUN_MODEL(CBF,PARS1);


%Step 4. Plot difference (e.g. values needed to estimate dNBE/dCeff)

%Do for GPP, Respirations, fires, NBE
plotpdf(mean(CBR1.NBE-CBR.NBE,2)*1000)



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