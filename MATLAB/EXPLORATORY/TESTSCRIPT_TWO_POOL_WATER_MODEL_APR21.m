

function EM_TESTSCRIPT_TWO_POOL_WATER_MODEL_JUL20
%But now configured to take inputs and outputs
%Continue, make sure ET and GPP units are monthly.
%
%Major changes
%Will resolve monthly (with corresponding depths)
%Will

%***NEXT STEPS***
%Bring in rivershed datasets
%Evaporation term


%Step 2. Define full data structure here
DATA=DATA_STRUCTURE;

%Step 3. Define full set of optimizable parameters here
[PARS_OPT, PARS_FIXED]=PARS_STRUCTURE;

DATA.PARS_OPT=PARS_OPT; %Contains all info for optimizable parameters
DATA.PARS_FIXED=PARS_FIXED;%Contains all fixed parameters


%Step 4. Define all soil H2O model functions here
DATA.FUNC=FUNCTION_STRUCTURE;


%Run MCMC setup
test_mcmc_setup(DATA)

keyboard
%Run model


end

function test_mcmc_setup(DATA)

%MCMCPARS, MC
MCMC_PARS.init=struct2mat(DATA.PARS_OPT,'init')';
MCMC_PARS.min=struct2mat(DATA.PARS_OPT,'min')';
MCMC_PARS.max=struct2mat(DATA.PARS_OPT,'max')';
MCMC_PARS.step=0.0001;

MCMC_OPTIONS.nout=100000;
MCMC_OPTIONS.printrate=5000;
MCMC_OPTIONS.samplerate=20;
MCMC_OPTIONS.nadapt=2;
MCMC_OPTIONS.minimumstep=0.001;
MCMC_OPTIONS.mcmcid=119;

%o
MCMC_LIKELIHOOD_FUNC = @(DATA,pars) MLFlocal(DATA,pars);

%Parameter names: not necessary
parameter_names=struct2mat(DATA.PARS_OPT,'name')';     %Not necessary for MCMC to run

%

rerun=1;

if rerun==1;
mcmcout=MHMCMC(DATA,MCMC_LIKELIHOOD_FUNC,MCMC_PARS,MCMC_OPTIONS);

save dumpfile_mcmc_test2h2opools_jan20_2.mat mcmcout
else
    load dumpfile_mcmc_test2h2opools_jan20_2.mat
end

%******************Plotting results**************
for s=1:13;
    subplot(4,4,s);hist(mcmcout.PARSOUT(:,s));
    xlabel(parameter_names{s});
end


%Run EWTs and show residuals
n_out = MCMC_OPTIONS.nout / MCMC_OPTIONS.samplerate ;
for p=1:n_out
    [~,STATES]=MLFlocal(DATA,mcmcout.PARSOUT(p,:));
    EWT(p,:)=normvec(sum(STATES.EWT,2),3)';
    F(p,:)=STATES.ALLFm(:,1);
    I(p,:)=STATES.INFR;
    D(p,:,1)=STATES.Q(:,1);
    D(p,:,2)=STATES.Q(:,2);
    PSI(p,:,1)=STATES.PSI(:,1);
    PSI(p,:,2)=STATES.PSI(:,2);
       MOI(p,:,1)=STATES.SM(:,1);
    MOI(p,:,2)=STATES.SM(:,2);
end

figure(2);clf
subplot(3,1,1:2);
[mh,ch,fh]=plotmultilines(EWT(:,2:end));delete(ch)
hold on
ph=plot(normvec(DATA.OBS.EWT(1:end),3),'k');
xlabel('Months')
ylabel('EWT [mm]')
legend([fh(1),ph],'Two pool model','GRACE');
grid
subplot(3,1,3);
bar(DATA.MET.PREC(1:end));ylabel('Prec [mm]');



figure(3);clf
subplot(3,3,1);plotmultilines(PSI(:,:,1));ylim([-5,0]);title('PAW \Psi')
subplot(3,3,2);plotmultilines(PSI(:,:,2));ylim([-5,0]);title('PUW \Psi')
subplot(3,3,3);plotmultilines(F);title('PUW->PAW flux');ylabel('mm');
subplot(3,3,4);plotmultilines(D(:,:,1));title('Q PAW');ylabel('mm');
subplot(3,3,5);plotmultilines(D(:,:,2));title('Q PUW');ylabel('mm');
subplot(3,3,6);plotmultilines(I);title('Infiltration');ylabel('mm');
subplot(3,3,7);plotmultilines(MOI(:,:,1));title('SM PAW');ylabel('m3/m3');
subplot(3,3,8);plotmultilines(MOI(:,:,2));title('SM PUW');ylabel('m3/m3');
%map mcmc parameters onto "PARS" structure

keyboard
%


end

function [P,STATES]=MLFlocal(DATA,pars)


%project on structure
%CONTINUE FROM HERE: look at struct2mat
%PARS=mat2struct(pars,DATA.MCMCPARStest);

%Run model
STATES=soil_water_model(DATA,pars);

%Likelihood function
%EWT units are in mm
%Uncertainty = 50mm
unc0=50;%units mm, %E.g. GRACE vector timeseries
unc =unc0;

% log L(x|O) ~ (M - O)^2/sigma^2
%

% % Eliminate bad data
% id_nan = find( isnan(STATES.EWT(2:end,1)));
% id_nan2 = find( isnan(STATES.EWT(2:end,2)));
%
% n_elem = id_nan(1)-1;

% Likelihood function
%P = -0.5*sum((normvec(sum(STATES.EWT(2:end,:),2),3)-normvec(DATA.OBS.EWT(1:24),3)').^2/unc^2);
% P = -0.5*sum((normvec(sum(STATES.EWT(2:end,:),2),3)-normvec(DATA.OBS.EWT(1:end),3)').^2/unc^2);
% P = -0.5*sum((normvec(sum(STATES.EWT(2:n_elem+1,:),2),3)-normvec(DATA.OBS.EWT(1:n_elem),3)').^2/unc^2);
% P = -0.5*sum((normvec(sum(STATES.EWT(2:25,:),2),3)-normvec(DATA.OBS.EWT(1:24),3)').^2/unc^2);

% % % P_lik = -0.5*sum((normvec(sum(STATES.EWT(2:121,:),2),3)-normvec(DATA.OBS.EWT(1:120),3)').^2/unc^2);
% % % % % % % P_lik = -0.5*sum((normvec(sum(STATES.EWT(2:61,:),2),3)-normvec(DATA.OBS.EWT(1:60),3)').^2/unc^2);
% % % 
% % % % Add Prior
% % % PARS.EPSF=pars(DATA.PARS_OPT.EPSF.id);
% % % Pscalefac = PARS.EPSF(2);
% % % Escalefac = PARS.EPSF(1);
% % % Pprior = -0.5*( (Pscalefac - 1)^2/0.2^2 + (Escalefac - 1)^2/0.2^2 );
% % % 
% % % % Add prior to posterior
% % % % [ P_lik Pprior P ]
% % % P = P_lik + Pprior;
% P = P_lik; %
 P = 0;

% % Likelihood function - summary metrics
% std_OBS = std(normvec(DATA.OBS.EWT(1:120),3)');
% std_MODEL = std(normvec(sum(STATES.EWT(2:121,:),2),3));
% unc=5;%units mm, %E.g. GRACE vector timeseries
%
% P = -0.5*sum( std_MODEL - std_OBS ).^2 / unc^2 ;



if isfinite(P)==0 | isreal(P)==0; P=-inf;end
% if sum(sum(STATES.SM<0))>0 | sum(sum(STATES.SM>1))>0; P=-inf;end

end

function FUNC=FUNCTION_STRUCTURE

%Include all commonly used functions here


FUNC.psi2metric=@(psi) psi*1e6/(9.8*1e3);%MPa to EWT
FUNC.metric2psi=@(ewt) ewt*(9.8*1e3)/1e6;%MPa to EWT
FUNC.moi2psi=@(sm,psiporosity,b) psiporosity.*((1./sm).^b);%m3/m3 to MPa
FUNC.psi2moi=@(psi,psiporosity,b) ((psi./psiporosity).^(-1/b));%m3/m3 to MPa


%m3/m3 to m/s
FUNC.moi2con=@(sm,k0,b)  k0*(sm).^(2*b+3);

%density * conductivity
% smflux=@(PSI, ) -1e3

%Convert mm to soil moisture
FUNC.ewt2moi=@(ewt,z,p) ewt/1e3/(p*z);
FUNC.moi2ewt=@(moi,z,p) moi*(p*z)*1e3;


end

function DATA=DATA_STRUCTURE


%This file contains area definitions for watershed region
%ARD=quickload('DATASCRIPT_READ_AMAZON_DISCHARGE_DATA_SEP17');
%ASK AABloom for above dataset: contains masks and original discharge
%datasets


%Loads Amazon basin discharge datasets
%ASK AABloom for  masks and original discharge datasets
load CARDAMOM/DATA/AMAZON_DISCHARGE_DATASETS_AND_BASINS.mat H2OB


%     DATA.MET.PREC=repmat(monthly2seasonal(H2OB(5,2).PREC(1,:)),[1,10]);
%     DATA.MET.ET=repmat(monthly2seasonal(H2OB(5,2).ET(1,:)),[1,10]);
%     DATA.OBS.EWT=repmat(monthly2seasonal(H2OB(5,2).EWT(1,:)),[1,10]);
%     DATA.OBS.Q=repmat(monthly2seasonal(H2OB(5,2).RUNOFF(1,:) ),[1,10]);


%     DATA.MET.PREC=H2OB(5,2).PREC(1,:);
%     DATA.MET.ET=H2OB(5,2).ET(1,:);
%     DATA.OBS.EWT=H2OB(5,2).EWT(1,:);
%     DATA.OBS.Q=H2OB(5,2).RUNOFF(1,:);

% Which test to do?
test_number = 1;

    % This test uses the first of all data products
    if test_number == 1

        % Which products to use?
        id_PREC = 1;
        id_ET = 1;
        id_EWT = 1;
        id_RUNOFF = 1;

        % Which time steps to consider?
        timesteps = 1:120;

        % Load the data structure with desired forcings
        DATA.MET.PREC=H2OB(5,2).PREC( id_PREC , timesteps );
        DATA.MET.ET=H2OB(5,2).ET( id_ET , timesteps );
        DATA.OBS.EWT=H2OB(5,2).EWT( id_EWT , timesteps );
        DATA.OBS.Q=H2OB(5,2).RUNOFF( id_RUNOFF , timesteps );
    end

    % This test uses the mean of all data products
    if test_number == 2

        % Which products to use?
        id_PREC = 1:3;
        id_ET = 1:9;
        id_EWT = 1:3;
        id_RUNOFF = 1;

        % Which time steps to consider?
        timesteps = 1:120;

        % Load the data structure with desired forcings
        DATA.MET.PREC= mean( H2OB(5,2).PREC( id_PREC , timesteps ));
        DATA.MET.ET= mean( H2OB(5,2).ET( id_ET , timesteps ));
        DATA.OBS.EWT= mean( H2OB(5,2).EWT( id_EWT , timesteps ));
        DATA.OBS.Q= mean(H2OB(5,2).RUNOFF( id_RUNOFF , timesteps ));
    end

    % This test uses the first of all data products for Basin 11
    if test_number == 3

        % Which products to use?
        id_PREC = 1;
        id_ET = 1;
        id_EWT = 1;
        id_RUNOFF = 1;

        % Which time steps to consider?
        timesteps = 1:120;

        % Load the data structure with desired forcings
        DATA.MET.PREC=H2OB(11,2).PREC( id_PREC , timesteps );
        DATA.MET.ET=H2OB(11,2).ET( id_ET , timesteps );
        DATA.OBS.EWT=H2OB(11,2).EWT( id_EWT , timesteps );
        DATA.OBS.Q=H2OB(11,2).RUNOFF( id_RUNOFF , timesteps );
    end

    % This test uses the first of all data products for Basin 14
    if test_number == 4

        % Which products to use?
        id_PREC = 1;
        id_ET = 1;
        id_EWT = 1;
        id_RUNOFF = 1;

        % Which time steps to consider?
        timesteps = 1:120;

        % Load the data structure with desired forcings
        DATA.MET.PREC=H2OB(14,2).PREC( id_PREC , timesteps );
        DATA.MET.ET=H2OB(14,2).ET( id_ET , timesteps );
        DATA.OBS.EWT=H2OB(14,2).EWT( id_EWT , timesteps );
        DATA.OBS.Q=H2OB(14,2).RUNOFF( id_RUNOFF , timesteps );
    end

    % This test uses the first few years of the data for Val/Cal for Basin 5
    if test_number == 5

        % Which products to use?
        id_PREC = 1;
        id_ET = 1;
        id_EWT = 1;
        id_RUNOFF = 1;

        % Which time steps to consider?
        timesteps = 1:60;

        % Load the data structure with desired forcings
        DATA.MET.PREC=H2OB(5,2).PREC( id_PREC , timesteps );
        DATA.MET.ET=H2OB(5,2).ET( id_ET , timesteps );
        DATA.OBS.EWT=H2OB(5,2).EWT( id_EWT , timesteps );
        DATA.OBS.Q=H2OB(5,2).RUNOFF( id_RUNOFF , timesteps );
    end

end

%Defines both optimizable and fixed parameters
function [PARS_OPT, PARS_FIXED]=PARS_STRUCTURE;


%d=struct2mat(PARS,'names') or any other similar approach to pull out
%parameters
%still need to use "id" to project to model parameters

%Loading functions
FUNC=FUNCTION_STRUCTURE;

% %PARS
% PARS.min=[0.2,0.2,-0.1,1,1,1.5,1e-7,100,0.1,0.1];
% PARS.max=[0.5,0.5,-0.01,100,100,10,1e-5,2000,0.5,0.5];

%PARS.iWUE=10;
PARS_OPT.porosity.init=[0.4,0.3];
PARS_OPT.porosity.min=[0.2,0.2];
PARS_OPT.porosity.max=[0.8,0.8];
PARS_OPT.porosity.name={'Porosity L1','Porosity L2'};

PARS_OPT.psifield.init=-0.05;%MPA (field capacity)
PARS_OPT.psifield.min=-0.11;%MPA (field capacity)
PARS_OPT.psifield.max=-0.01;%MPA (field capacity)
PARS_OPT.psifield.name={'Psi field'};

%Resolved
PARS_OPT.layer_thickness.init=[20,20];
PARS_OPT.layer_thickness.min=[1,1];
PARS_OPT.layer_thickness.max=[100,100];
PARS_OPT.layer_thickness.name={'L1 depth','L2 depth'};

%PARS.et_fractional_loss=[1,0];
PARS_OPT.psi_b.init=5;
PARS_OPT.psi_b.min=1.5;
PARS_OPT.psi_b.max=10;
PARS_OPT.psi_b.name={'VG parameter b'};

%Conductivity
PARS_OPT.k0.init=6.5*1e-6;%m/s
PARS_OPT.k0.min=1e-7;%m/s
PARS_OPT.k0.max=1e-5;%m/s
PARS_OPT.k0.name={'Conductivity'};

%Maximum infiultration
%units = mm/
PARS_OPT.Imax.init=1000;%mm/month
PARS_OPT.Imax.min=100;%mm/month
PARS_OPT.Imax.max=2000;%mm/month
PARS_OPT.Imax.name={'Max infiltration'};

%Initial soil moisture conditions
PARS_OPT.SM0.init=[0.25,0.25];%
PARS_OPT.SM0.min=[0.1,0.1];%
PARS_OPT.SM0.max=[0.5,0.5];%
PARS_OPT.SM0.name={'Moisture@t=0, L1','Moisture@t=0, L2'};

% Precip and ET scalar parameters
PARS_OPT.EPSF.init=[1,1];%
PARS_OPT.EPSF.min=[0.5,0.5];%
PARS_OPT.EPSF.max=[1.5,1.5];%
PARS_OPT.EPSF.name={'E scale fac','P scale fac'} ;

% Drainage sm excess turnover parameter: removes fixed fraction of SM excess above field capacity
PARS_OPT.Qexcess.init=0.5;%
PARS_OPT.Qexcess.min=0.01;%
PARS_OPT.Qexcess.max=1;%
PARS_OPT.Qexcess.name={'Q Excess in SM'} ;

% PARS.psiporosity=FUNC.metric2psi(-0.117);
% PARS.psiwilt=-3;%MPa (wilting point)
[PARS_OPT]=add_ids_to_each_parameter_field(PARS_OPT,0);


PARS_FIXED.psiporosity=FUNC.metric2psi(-0.117);
PARS_FIXED.psiwilt=-3;%MPa (wilting point)


end

function [S,k]=add_ids_to_each_parameter_field(S,k)


fname='init';
if nargin<2;k=0;end

fn=fieldnames(S);
for f=1:numel(fn);
        if isstruct(S.(fn{f}));
          [S.(fn{f}),k]=add_ids_to_each_parameter_field(S.(fn{f}),k);
        else
            if strcmp(fn{f},fname);
                S.id=k+1:k+numel(S.(fn{f}));
                                k=k+numel(S.(fn{f}));
            end

        end

end


end

%Convention:
%   - anything "known" is in DATA
%   - anything "unknown" and/or optimizable is in PARS
function STATES=soil_water_model(DATA,pars);

%Unpacking functions
FUNC=DATA.FUNC;
%HARDCODED PARAMETERS

%Fixed parameters
PARS=DATA.PARS_FIXED;


%Appending with optimizable parameters
%Step 2.1. Parameters
%Layer 1 & 2 thicknwss
%"Unpack parameters"
PARS.layer_thickness=pars(DATA.PARS_OPT.layer_thickness.id);%in m
PARS.porosity=pars(DATA.PARS_OPT.porosity.id);
PARS.psifield=pars(DATA.PARS_OPT.psifield.id);
PARS.psi_b=pars(DATA.PARS_OPT.psi_b.id);
PARS.k0=pars(DATA.PARS_OPT.k0.id);
PARS.Imax=pars(DATA.PARS_OPT.Imax.id);
%STATES: cotains
PARS.SM0=pars(DATA.PARS_OPT.SM0.id);
PARS.EPSF=pars(DATA.PARS_OPT.EPSF.id);
PARS.Qexcess=pars(DATA.PARS_OPT.Qexcess.id);

%iWUE

%Step 2,2. Layer 1 and 2 porosity


%Step 2.3. Layer 1 and 2 ET fractional loss (@>field capacity)

%Step 2.4; SM->PSI conversion
%Psi = Psi0 * (1/SM)^b



%conductivity
%m/s


%STep 2.5 layer-to-layer transfer

%STATES: cotains
STATES.SM=PARS.SM0;


%Step 3. Initial conditions
%SM  (in m3/m3 of pore space)
%SM(:,1) = top layer
%SM(:,2) = h



%

for t=1:numel(DATA.MET.PREC) % 24

    %Infiltration
   [I,S]= infiltration(DATA,PARS,STATES,t);
    STATES.INFR(t,1)=I;
    STATES.SRUN(t,1)=S;


    %evapotranspiration
    STATES.ET(t,:)=evapotranspiration(DATA, PARS,STATES,t);

    %Put precip in top layer
    %SM = soil moisture (m3/m3)
    STATES.SM(t+1,:)=STATES.SM(t,:);
    %Adding
    STATES.SM(t+1,1)=STATES.SM(t,1)+FUNC.ewt2moi(STATES.INFR(t),PARS.layer_thickness(1),PARS.porosity(1));



     STATES.PSI(t,:)=FUNC.moi2psi(STATES.SM(t+1,:),PARS.psiporosity,PARS.psi_b);

        %***drainage***

    STATES.DR(t,:)=drainage(DATA,PARS,STATES,t);

    STATES.SM(t+1,:)= STATES.SM(t+1,:)-STATES.DR(t,:);



       %Conductivity
       %Decreases fast, ok to cap bottom values
    STATES.K(t,:)=FUNC.moi2con(STATES.SM(t+1,:),PARS.k0,PARS.psi_b);
     %Calculate fluxes (deep 2 shallow)
     %PSI = soil water potential
    STATES.PSI(t,:)=FUNC.moi2psi(STATES.SM(t+1,:),PARS.psiporosity,PARS.psi_b);
    %Currently in metres,
    %Positiv e = Bottom to top
    %Tp tyest , try PSI(1) = PSI(2)
    F= -1e3*sqrt(prod(STATES.K(t,:)))*( FUNC.psi2metric(STATES.PSI(t,1)-STATES.PSI(t,2))/mean(PARS.layer_thickness)  +1);


    %m/s to mm/month COnvert to monthly
    %Also convert from metres to moisture?
    Fm=F*3600*24*365.25/12;
    %CHECK UNITS,
     STATES.ALLFm(t,:)=[Fm,-Fm]*1000;
    %Soil moisture flux
    %timestep question?



    %ET = ETmax *
    ti=mod(t-1,12)+1;
    %Cal
    %ETmax=DATA.MET.GPP(ti).*DATA.MET.VPD(ti)/iWUE;
    ETmax=DATA.MET.ET(ti);
    %  STATES.ET(t,:)=ETmax.*PARS.et_fractional_loss.*(1- (PARS.psifield-min(max(STATES.PSI(t,:),PARS.psiwilt),PARS.psifield))./(PARS.psifield-PARS.psiwilt));
    %  STATES.ET(t,2)=0;


    for n=1:2
        STATES.SM(t+1,n)=STATES.SM(t+1,n)+FUNC.ewt2moi( - STATES.ET(t,n)+STATES.ALLFm(t,n),PARS.layer_thickness(n),PARS.porosity(n));
    end

    %


end


    %*****OUTPUTS******
    STATES.UNITS.SM='m3/m3';
    STATES.UNITS.INFR='mm/month';


    %Derive EWT
    for t=1:size(STATES.SM,1)
        for l=1:2
            STATES.EWT(t,l)=FUNC.moi2ewt(STATES.SM(t,l),PARS.layer_thickness(l),PARS.porosity(l));
            if t<size(STATES.SM,1)
               STATES.Q(t,l)=FUNC.moi2ewt(STATES.DR(t,l),PARS.layer_thickness(l),PARS.porosity(l));
            end
        end
    end




end

function D= drainage(DATA,PARS,STATES,t);

    % What is Drainage_sm_excess_turnover parameter: removes fixed fraction of SM excess above field capacity
    frac_Qexcess =  PARS.Qexcess;

    % What is Drainage
    SMdelta = max(STATES.SM(t+1,:) - DATA.FUNC.psi2moi(PARS.psifield,PARS.psiporosity,PARS.psi_b),0);
    D= frac_Qexcess * (1- (PARS.psiporosity-min(max(STATES.PSI(t,:),PARS.psifield),PARS.psiporosity))./(PARS.psiporosity-PARS.psifield)).*(SMdelta);

end

%Modules
function [I,S]=infiltration(DATA,PARS,STATES,t);

%Unpacking required drivers and parameters
P=DATA.MET.PREC(t);
P_scalar = PARS.EPSF(2); % Precip scalar parameter
P = P*P_scalar;

%dI/dP ~ 1 for low P
%dI/dP ~ 0 for high P

%
%dI/dP = - Imaxexp(-P/Imax)
I = PARS.Imax*(1-exp(-P/PARS.Imax));
%dI/dP = exp(-P/Imax)) = exp(-0/Imax)) = 1;
S=P-I;



end

function ET=evapotranspiration(DATA, PARS,STATES,t);

%ET for now
%Next step is to define as E and T
ET=[DATA.MET.ET(t),0];
ET_scalar = PARS.EPSF(1); % ET scalar parameter
ET = ET*ET_scalar;

end

function plot_smod_outputs(SMOD)

figure(1);clf
plot(SMOD.SM.data(:,1),'b'); hold on
plot(SMOD.SM.data(:,2),'r')

end
