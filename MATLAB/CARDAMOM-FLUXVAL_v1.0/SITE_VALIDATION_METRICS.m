%Section 1: main function
%Calculate the the mertrics for evaluate the model ouputs
function SITE_VALIDATION_METRICS % VALIDATE_CARDAMOM_MODEL


%These are for test runs
%Runs in Yang et al. used 10^7, these take about one or two hours to run
MCO.niterations=10000;%Runs in Yang et al. used 10^7


%
fluxval_path=[getenv('CARDAMOM_PATH'),'/MATLAB/CARDAMOM-FLUXVAL_v1.0/'];
data=readtable([fluxval_path,'/location_fluxnet.csv']);

%Looping through FLUXNET 2015 sites
for n=1:size(data,1)
    disp(['n=' num2str(n)]);
    CBF=CARDAMOM_READ_BINARY_FILEFORMAT([fluxval_path,'cbf_files/flux_site_' num2str(n) '.cbf']);
    %This is the function for running CARDAMOM on local machine. 
    %Consider parallelizing with CPU cluster for running all sites  
    CBR=CARDAMOM_RUN_MDF(CBF,MCO);
    vdata=table2array(readtable([fluxval_path,'validation_data/validation_' data.Flux_name{n} '.csv']));% first col is the number of monthsfrom 2000/01/01; second col is date; the 3rd to 5th col is GPP, NEE and ET
    site_name=[data.Flux_name{n}(1:2) '_' data.Flux_name{n}(4:end)]; 
    [M1,M2]= VALIDATE_OUTPUTS_AGAINST_SITE_DATA(CBR,vdata,site_name);
    Mseason.(site_name)=M1.(site_name);
    Mannual.(site_name)=M2.(site_name);
    clear M1 M2
    % ploting the CARDAMOM outputs (GPP, NEE and ET) along with the
    % training and validation data from the flux data
    CARDAMOM_output_plot(CBR,CBF,vdata,path)
end
%This will be saved in your current directory,
%ensure that this is outside github repo
save('site_mertrics.mat','Mseason','Mannual'); 
end

%Section 2: validation statistics
function [M1,M2] = VALIDATE_OUTPUTS_AGAINST_SITE_DATA(CBR,vdata,site_name)
% generate seasonal RMSE, r, bias and MEF
x(:,:,1)=CBR.GPP'; x(:,:,2)=CBR.NEE'; x(:,:,3)=CBR.ET';
x1=reshape(x,[size(x,1)*size(x,2),3]);
month=1:192; id=ismember(month,vdata(:,1));
y=NaN(192,3); y(id,:)=vdata(:,3:5);
y(y==-9999)=NaN;
y1=repmat(y,size(x,2),1);

for i=1:size(x,3)
    M1.(site_name)(i)=get_mertics(y1(:,i),x1(:,i));
end
clear x1 y1
%generate the annual RMSE, r, bias and MEF
tmp1=reshape(x,[12,16,size(x,2),size(x,3)]);
x2=reshape(squeeze(nanmean(tmp1,2)),[size(tmp1,1)*size(tmp1,3),size(tmp1,4)]);
clear tmp1
tmp2=reshape(y,[12,16,size(y,2)]);
y2=repmat(squeeze(nanmean(tmp2,2)),size(x,2),1);
clear tmp2
for i=1:size(x,3)
    M2.(site_name)(i)=get_mertics(y2(:,i),x2(:,i));
end
clear x y x2 y2
% get r value, RMSE, bias and MEF
    function M=get_mertics(y0,x0)
        x0(y0==-9999) = NaN;
        y0(y0==-9999) = NaN;
        x0(isnan(y0))=[];
        y0(isnan(y0))=[];
        ym=nanmean(y0);
        m=1-nansum((x0(:)-y0(:)).^2)./nansum((x0(:)-ym).^2);
        ba=nansum(x0(:)-y0(:))./length(y0(~isnan(y0)));
        M.ba=ba; % bias
        M.m=m; % MEF
        M.R=corr(x0,y0);% R
        mdl=fitlm(x0,y0);
        M.RMSE=mdl.RMSE;% RMSE
    end
end
