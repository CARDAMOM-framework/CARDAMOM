function TEMPSCRIPT_BILIR_CLIMATE_CO2_STATE_SENSITIVITY_JUL24


%git checkout e5d9f66
%*************CO2 yearly detrends
cbf='Desktop/site58N_10E_ID1100HF_DFNBSTv3.cbf[47].nc';
cbr='Desktop/site58N_10E_ID1100HF_DFNBSTv3.cbf[99].cbr';

CBF=CARDAMOM_READ_NC_CBF_FILE(cbf);
CBR=CARDAMOM_RUN_MODEL(cbf,cbr);

MD=CARDAMOM_MODEL_LIBRARY(1100,[],1);

%Extract pool sensitivities
P=MD.POOL_IDs;
poolidx=[P.C_lab,P.C_fol,P.C_roo, P.C_woo, P.C_cwd,P.C_lit,P.C_som];
%Create annually detrended climate for each year
for y=1:21
    CBFybclim(y)=CBF;
    CBFybclim(y)=derive_and_remove_year_climate_trends(CBF,y+2000);
end


for y=1:21
    %STep 1. Rerun
    CBRybclim=CARDAMOM_RUN_MODEL(CBFybclim(y),cbr);

    %Total C by end of year.
    eoyidx=y*12+1;
    TOTAL_C_DIFF_CLIM(:,y)=sum(CBR.POOLS(:,eoyidx,poolidx),3)- sum(CBRybclim.POOLS(:,eoyidx,poolidx),3);
        
end



CO2trends=linfit(CBF.CO2.values);
for y=1:21;
    CBFybco2(y)=CBF;
    CBFybco2(y).CO2.values([1:12]+(y-1)*12)=CBFybco2(y).CO2.values([1:12]+(y-1)*12) - CO2trends*[1:12]';
end



for y=1:21
    %STep 1. Rerun
    CBRybco2=CARDAMOM_RUN_MODEL(CBFybco2(y),cbr);

    %Total C by end of year.
    eoyidx=y*12+1;
    TOTAL_C_DIFF_CO2(:,y)=sum(CBR.POOLS(:,eoyidx,poolidx),3)- sum(CBRybco2.POOLS(:,eoyidx,poolidx),3);
        
end


%Also doing CO2 



%Also comparing full detrend for reference

figure(1);clf

subplot(2,2,1); 
plotmultilines(TOTAL_C_DIFF_CO2); ylim([1,3]);
ylabel('gC/m2/(\Delta CO2 /yr])');
title('State-dependent sensitivity of land C sink to CO2')


subplot(2,2,2); 
plotmultilines(TOTAL_C_DIFF_CLIM); ylim([-4,2])
ylabel('gC/m2/(\Delta Clim/yr])');
title('State-dependent sensitivity of land C sink to change in climate')

% subplot(2,2,3); 
% plotmultilines(cumsum(TOTAL_C_DIFF_CO2,2)); ylim([-50,50])
% 
% 
% 
% subplot(2,2,4); 
% plotmultilines(cumsum(TOTAL_C_DIFF_CLIM,2)); ylim([-25,25])





keyboard





end


function CBFout=derive_and_remove_year_climate_trends(CBFin,yr)

%load DUMPFILES/DUMP_AMAZON_IMERG_ANALYSIS_SEP23.mat CBF CBFnoco2 CBR CBRnoco2 

%Step 1. Summarize trends in datasets, remove months 

vnames={'T2M_MIN','T2M_MAX','SKT','VPD','TOTAL_PREC','SSRD','SNOWFALL'};


CBFout=CBFin;
    for v=1:numel(vnames)
        VALUES=CBFin.(vnames{v}).values';
        VALUES_DT=VALUES;
        for m=1:12
            %Step 1.1 extract data
            DMONTH=VALUES(m:12:end);
            %Step 1.2, fit line
            x=DMONTH*0;x(1:end)=1:numel(DMONTH);
            [grd,~]=linfit(x,DMONTH);
            %Ensuring none go below zero
            VALUES_DT(m+(yr-2001)*12)=DMONTH(yr-2000)-1*grd;
        end

        if total(strcmp(vnames{v},{'VPD','TOTAL_PREC','SSRD','SNOWFALL'}));VALUES_DT=max(VALUES_DT,0);end

              CBFout.(vnames{v}).values(:)=VALUES_DT;
    end







end
