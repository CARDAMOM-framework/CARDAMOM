%Step 1. Load example CBF (driver structure), CBR (output strucutre) from example CARDAMOM .cbf and .cbr output files
%Timesteps span 2010-2015


%Load two locations
r = 20; c = 25;
r = 24; c = 25;


%PXI
PXI=CARDAMOM_PROJECT('GCRUN_APR20_TDLE');
PXIv1=PARRFUN_CARDAMOM_CBF2JPLHPC(PXI,'v1',[]);



[CBF,CBR]=CARDAMOM_PROJECT_EXTRACT_GRIDCELL(PXIv1,r,c);


CCF=cardamomfun_cost_function(CBF,CBR);


%
edit cardamomfun_cost_function.m


[CBR,CBF]=CARDAMOM_RUN_MODEL('CARDAMOM/DATA/EXAMPLE_IO/GCRUN_JUL19_TDLE_1942.cbf','CARDAMOM/DATA/EXAMPLE_IO/GCRUN_JUL19_TDLEv1_182_*.cbr');

%


[CBR,CBF]=CARDAMOM_RUN_MODEL('CARDAMOM/DATA/EXAMPLE_IO/GCRUN_JUL19_TDLE_1942.cbf','CARDAMOM/DATA/EXAMPLE_IO/GCRUN_JUL19_TDLEv1_182_*.cbr');


%Try Amazon
PXI=CARDAMOM_PROJECT('GCRUN_APR20_TDLE');
PXIv1=PARRFUN_CARDAMOM_CBF2JPLHPC(PXI,'v1',[]);

r = 25; c = 25;




[CBF,CBR]=CARDAMOM_PROJECT_EXTRACT_GRIDCELL(PXIv1,24,24);








PARS=CBR.PARS;%These are parameters to call
%Step 2. Derive "climatology" here

    for n=1:12;M_climatological(n,[2:5,7:9])=mean(CBF.MET(n:12:end,[2:5,7:9]),1);end
    
 %Step 3. Make new CBF drivers (CBF0) substitute in climatology for TEMP,
 %SRAD, CO2, VPD, PREC and ALL.
 %Can summarize ?NBE/?Clim = ?NBE/?RAD + ?NBE/?CO2 + ?NBE/?PREC + ... + INTERACTIONS
 %Same for ?GPP/?Clim, ?HR/?RAD
 
 CBF0=CBF;
 CBF0.mmet=mean(CBF.MET); %Hard-coding mean values as model parameters use fixed met references...

 
 %***************************
 %***************************
 %******************************Example 1.  Test sensitivity of Hrespiration to precipitation ?HR/?P during 1:12****
 %anomaly, 
 
 %Copying nominal CBF0 structure
 CBF0_dhr_dP = CBF0;
 CBF0_dhr_dT = CBF0;
 
 %Substituting climatological P and T for first 12 months
 CBF0_dhr_dP.MET(1:12,9)=M_climatological(1:12,9);
 CBF0_dhr_dT.MET(1:12,[2,3])=M_climatological(1:12,[2,3]);
 
 %Re-running model with new drivers
 CBR0_dhr_dP=CARDAMOM_RUN_MODEL(CBF0_dhr_dP,CBR.PARS); 
 CBR0_dhr_dT=CARDAMOM_RUN_MODEL(CBF0_dhr_dT,CBR.PARS); 
 
 %?HR/?T, ?HR/?P
 %This is the sensitivity of 
deltaHRP = mean(CBR.RHE(:,1:12) - CBR0_dhr_dP.RHE(:,1:12) ,2);%Change in respiration 
deltaP = mean(CBF.MET(1:12,9)-  CBF0_dhr_dP.MET(1:12,9));%Change in precip
dHRdP = deltaHRP./deltaP; %gC/m2/mm

deltaHRT = mean(CBR.RHE(:,1:12) - CBR0_dhr_dT.RHE(:,1:12) ,2);%Change in respiration 
deltaT = mean(mean(CBF.MET(1:12,[2,3])-  CBF0_dhr_dT.MET(1:12,[2,3])));%Change in temp
dHRdT = deltaHRT./deltaT; %gC/m2/d/C

figure(1);clf;
subplot(2,3,1);hist(dHRdP);xlabel('?HR/?P [gC/m2/mm]');
subplot(2,3,2);hist(dHRdT);xlabel('?HR/?T [gC/m2/d/C]');
subplot(2,3,3);plot(dHRdP,dHRdT,'.','MarkerSize',3);xlabel('?HR/?P [gC/m2/mm]');ylabel('?HR/?T [gC/m2/d/C]');

%*******Example 2. Test sensitivity of GPP to VPD, ?GPP/?VPD with moving 6-month window*******
    %Step 1. Loop through each 12-month period at 1-month increments

    
    
    
    
    
    
    Ws=[3,6,12];

for w=1:numel(Ws)
    W=Ws(w);%Only works for even numbers for now
    %
for n=176:180-W+1%n=1:size(CBF.MET,1)-W+1;
    
    %Step 2. Insert climatology for VPD for those months/years
    %For faster runs: you can extract states at given times, and update
    %PARS initial condition terms. Alternatively just re-run the whole
    %thing, it takes N times longer (N = number of years)
    idx=[1:W]+n-1;
    
        
    CBFclim{w}=CBF0;
    CBFclim{w}.MET(idx,[2:5,7:9])=M_climatological(mod(idx-1,12)+1,[2:5,7:9]);
    
    for f=1:4
    CBFsens{w}(f)=CBFclim;
    end

    
%Actual met drivers
    CBFsens{w}(1).MET(idx,8)=CBF.MET(idx,8);
    CBFsens{w}(2).MET(idx,9)=CBF.MET(idx,9);
    CBFsens{w}(3).MET(idx,4)=CBF.MET(idx,4);
    CBFsens{w}(4).MET(idx,[2,3])=CBF.MET(idx,[2,3]);
    
    
    
deltaclim{w}(1,n) = mean(mean(CBF.MET(idx,8)-  CBFclim.MET(idx,8)));%Change in temp
deltaclim{w}(2,n) = mean(mean(CBF.MET(idx,9)-  CBFclim.MET(idx,9)));%Change in temp
deltaclim{w}(3,n) = mean(mean(CBF.MET(idx,4)-  CBFclim.MET(idx,4)));%Change in temp
deltaclim{w}(4,n) = mean(mean(CBF.MET(idx,[2,3])-  CBFclim.MET(idx,[2,3])));%Change in temp

    
    
    %Step 3. Re-run (doing it for subset of parameters because it takes
    %long!).
    
    CBRclim=CARDAMOM_RUN_MODEL(CBFclim,CBR.PARS(1:10:end,:)); 
    
     for f=1:4;
         CBRsens{w}(f)=CARDAMOM_RUN_MODEL(CBFsens(f),CBR.PARS(1:10:end,:)); 
        deltaGPP{w}(:,n,f) = mean(CBRsens{w}(f).GPP(:,idx) - CBRclim.GPP(:,idx) ,2);%Change in respiration 
        deltaRHE{w}(:,n,f) = mean(CBRsens{w}(f).RHE(:,idx) - CBRclim.RHE(:,idx) ,2);%Change in respiration 
        deltaGPPdc{w}(:,n,f) = deltaGPP{w}(f,n) ./deltaclim{w}(f,n);
        deltaRHEdc{w}(:,n,f) = deltaRHE{w}(f,n) ./deltaclim{w}(f,n);
     end
     
     

     
     

    
end
    end






N=180;

figure(2);clf;

cmp=multicolormap([1,1,1;0.3,0.3,1]);

subplot(5,2,1);[~,ch]=plotmultilines(CBR.GPP);ylabel('GPP [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid
subplot(5,2,2);[~,ch]=plotmultilines(CBR.GPP);ylabel('GPP [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid

subplot(5,2,3);plot(CBF.MET(:,8),'b','LineWidth',2); ylabel('VPD [hPA]');xlim([0.5,N+0.5]);grid
subplot(5,2,5);plot(CBF.MET(:,9),'b','LineWidth',2);   ylabel('PREC [mm]');xlim([0.5,N+0.5]);grid
subplot(5,2,7);plot(CBF.MET(:,4),'b','LineWidth',2); ylabel('SRAD [MJ/m2/d]');xlim([0.5,N+0.5]);grid
subplot(5,2,9);plot(mean(CBF.MET(:,[2,3]),2),'b','LineWidth',2);   ylabel('Temp [C]');xlim([0.5,N+0.5]);grid

subplot(5,2,4);[~,ch]=plotmultilines(W/2+1:N-W/2+1,deltaGPP(:,:,1),[],cmp);ylabel('GPP [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid;ylim([-0.2,0.2]);title('VPD response')
subplot(5,2,6);[~,ch]=plotmultilines(W/2+1:N-W/2+1,deltaGPP(:,:,2),[],cmp);ylabel('GPP  [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid;ylim([-0.2,0.2]);title('PREC response')
subplot(5,2,8);[~,ch]=plotmultilines(W/2+1:N-W/2+1,deltaGPP(:,:,3),[],cmp);ylabel('GPP [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid;ylim([-0.2,0.2]);title('SRAD response')
subplot(5,2,10);[~,ch]=plotmultilines(W/2+1:N-W/2+1,deltaGPP(:,:,4),[],cmp);ylabel('GPP [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid;ylim([-0.2,0.2]);title('TEMP response')




figure(3);clf;

cmp=multicolormap([1,1,1;0.3,0.3,1]);

subplot(5,2,1);[~,ch]=plotmultilines(CBR.RHE);ylabel('RHE [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid
subplot(5,2,2);[~,ch]=plotmultilines(CBR.RHE);ylabel('RHE [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid

subplot(5,2,3);plot(CBF.MET(:,8),'b','LineWidth',2); ylabel('VPD [hPA]');xlim([0.5,N+0.5]);grid
subplot(5,2,5);plot(CBF.MET(:,9),'b','LineWidth',2);   ylabel('PREC [mm]');xlim([0.5,N+0.5]);grid
subplot(5,2,7);plot(CBF.MET(:,4),'b','LineWidth',2); ylabel('SRAD [MJ/m2/d]');xlim([0.5,N+0.5]);grid
subplot(5,2,9);plot(mean(CBF.MET(:,[2,3]),2),'b','LineWidth',2);   ylabel('Temp [C]');xlim([0.5,N+0.5]);grid

subplot(5,2,4);[~,ch]=plotmultilines(W/2+1:72-W/2+1,deltaRHE(:,:,1),[],cmp);ylabel('RHE [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid;ylim([-0.05,0.05]);title('VPD response')
subplot(5,2,6);[~,ch]=plotmultilines(W/2+1:72-W/2+1,deltaRHE(:,:,2),[],cmp);ylabel('RHE  [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid;ylim([-0.05,0.05]);title('PREC response')
subplot(5,2,8);[~,ch]=plotmultilines(W/2+1:72-W/2+1,deltaRHE(:,:,3),[],cmp);ylabel('RHE [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid;ylim([-0.05,0.05]);title('SRAD response')
subplot(5,2,10);[~,ch]=plotmultilines(W/2+1:72-W/2+1,deltaRHE(:,:,4),[],cmp);ylabel('RHE [gC/m2/d]');delete(ch);xlim([0.5,N+0.5]);grid;ylim([-0.05,0.05]);title('TEMP response')









%Step 3. 

