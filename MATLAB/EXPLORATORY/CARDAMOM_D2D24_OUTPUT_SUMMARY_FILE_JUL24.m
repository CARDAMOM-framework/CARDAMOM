cbffile='Desktop/site58N_10E_ID1100HF_DFNBSTv3.cbf[47].nc';
cbrfile='Desktop/site58N_10E_ID1100HF_DFNBSTv3.cbf[99].cbr';
%Branch: e5d9f66 %new ST EDC’

%Step 1. Run model and extract full dataset
CBF=CARDAMOM_READ_NC_CBF_FILE(cbffile);
CBR=CARDAMOM_RUN_MODEL(cbffile,cbrfile);
MD=CARDAMOM_MODEL_LIBRARY(1100,[],1);

%Step 2. Exrtract useful drivers
%- RADIATION
%- PRECIP
%- 


% Step 2. Create named variables for export
F=MD.FLUX_IDs;
P=MD.POOL_IDs;

                    % fx_lab2lit: 23
                    % fx_fol2lit: 24
                    % fx_roo2lit: 25
                    % fx_woo2cwd: 26


                    
carbon_field_names={'gpp','rau','rhe','fir','ddh','mor','lib','dom'};
CARBON.gpp=CBR.FLUXES(:,:,F.gpp);
CARBON.rau = CBR.FLUXES(:,:,F.resp_auto);
CARBON.rhe = sum(CBR.FLUXES(:,:,[F.rh_co2,F.rh_ch4]),3);
CARBON.fir = sum(CBR.FLUXES(:,:,F.f_total),3);
CARBON.ddh=CBF.DISTURBANCE_FLUX.values;
CARBON.mor=sum(CBR.FLUXES(:,:,[F.fx_lab2lit,F.fx_fol2lit,F.fx_roo2lit,F.fx_woo2cwd]),3);
CARBON.lib=sum(CBR.POOLS(:,1:end-1,[P.C_fol,P.C_lab,P.C_roo,P.C_woo]  )+CBR.POOLS(:,2:end,[P.C_fol,P.C_lab,P.C_roo,P.C_woo] ),3)/2;
CARBON.dom=sum(CBR.POOLS(:,1:end-1,[P.C_cwd,P.C_lit,P.C_som]  )+CBR.POOLS(:,2:end,[P.C_cwd,P.C_lit,P.C_som] ),3)/2;


filename='CARDAMOM_D2D24_OUTPUT_SUMMARY_FILE_JUL24.nc';
delete(filename);
N=size(CBR.FLUXES,2);

nccreate(filename,'time','Dimensions',{'time' N});
nccreate(filename,'stats','Dimensions',{'stats' 3});
ncwrite(filename,'time',CBF.time.values);
ncwriteatt(filename,'time','units', 'days since 2001-01-01');
ncwriteatt(filename,'stats','details', '25th percentile, 50th percentile, 75th percentile');


for n=1:numel(carbon_field_names)
    %Step 1. derive stats
    ncfieldname=carbon_field_names{n};
    NCF=prctile(CARBON.(carbon_field_names{n}),[25,50,75]);
    nccreate(filename, ncfieldname,"Dimensions",{'stats' 3 'time' N});
    ncwrite(filename,ncfieldname,NCF)
end




%DefDegunc=CBF.DISTURBANCE_FLUX.values*0.1;






%Step 2.2; check all looks OK
figure(1);clf
V=GPP;U=GPPunc;
subplot(3,3,1);plot(V);hold on;plot(V+U,'--');plot(V-U,'--');title('GPP')

V=Rauto;U=Rautounc;
subplot(3,3,2);plot(V);hold on;plot(V+U,'--');plot(V-U,'--');title('Autotrophic Resp')

V=Rhet;U=Rhetunc;
subplot(3,3,3);plot(V);hold on;plot(V+U,'--');plot(V-U,'--');title('Heterotrophic Resp')

V=Fire;U=Fireunc;
subplot(3,3,4);plot(V);hold on;plot(V+U,'--');plot(V-U,'--');title('Fire')



V=DefDeg;U=DefDegunc;
subplot(3,3,5);plot(V);hold on;plot(V+U,'--');plot(V-U,'--');title('Def & Deg')

V=MORTALITY;U=MORTALITYunc;
subplot(3,3,6);plot(V);hold on;plot(V+U,'--');plot(V-U,'--');title('Mortality')

V=ABGB;U=ABGBunc;
subplot(3,3,7);plot(V);hold on;plot(V+U,'--');plot(V-U,'--');title('Live Biomass')

V=DOM;U=DOMunc;
subplot(3,3,8);plot(V);hold on;plot(V+U,'--');plot(V-U,'--');title('Dead Organic Matter')





% Step 3. Write to netcdf file




