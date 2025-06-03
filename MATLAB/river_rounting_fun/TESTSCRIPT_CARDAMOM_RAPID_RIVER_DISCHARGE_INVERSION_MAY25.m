function TESTSCRIPT_CARDAMOM_RAPID_RIVER_DISCHARGE_INVERSION_MAY25



keyboard



end



function [TEMP,LF]=energy_to_temp_converter(U)

%#define DGCM_PI 3.1415927 //pi 

DGCM_T3 = 273.16; % //Triple point temp, K
DGCM_TK0C=273.15;% //Kelvin at 0C
%// Specific heat of ice 
DGCM_SPECIFIC_HEAT_ICE=2093;%.// 2093 J kg?1 K
 DGCM_SPECIFIC_HEAT_WATER= 4186;%.// 4186 J kg?1 K
%//Latent heat of vaporization
%#define DGCM_LATENT_HEAT_VAPORIZATION 2.501e6; // J kg-1 
DGCM_LATENT_HEAT_FUSION_3=3.34e5;
 DGCM_T_LIQUID_H2O_ZERO_ENERGY=56.79;%//Zero-energy temperature of super-cooled liquid water 56.79 K
%#define DGCM_SEC_DAY (24*60*60)//Seconds per day
    
    %Input = U per unit kg (or mm)

UI3 = DGCM_SPECIFIC_HEAT_ICE* DGCM_T3 ; 
UL3 = UI3 + DGCM_LATENT_HEAT_FUSION_3; 

for n=1:numel(U)

if U(n)<UI3
    TEMP(n)= (U(n)/UI3)*DGCM_T3 ;LF(n) = 0;
elseif (U(n)>UL3)
       TEMP(n) = (U(n)+DGCM_SPECIFIC_HEAT_WATER*DGCM_T_LIQUID_H2O_ZERO_ENERGY)/(DGCM_SPECIFIC_HEAT_WATER) ;
      LF(n) = 1;
else 
    TEMP(n) = DGCM_TK0C;  
     LF(n) =(U(n) - UI3) /(DGCM_LATENT_HEAT_FUSION_3);  
end
    
end
end


function U=temp_to_energy_converter(TEMP,LF)
  %//Internal energy per mm (or kg) H2O (J/kg or J/mm)

%#define DGCM_PI 3.1415927 //pi 
%#define DGCM_T3 273.16 //Triple point temp, K
%#define DGCM_TK0C 273.15 //Kelvin at 0C
%// Specific heat of ice 
DGCM_SPECIFIC_HEAT_ICE=2093;%.// 2093 J kg?1 K
 DGCM_SPECIFIC_HEAT_WATER= 4186;%.// 4186 J kg?1 K
%//Latent heat of vaporization
%#define DGCM_LATENT_HEAT_VAPORIZATION 2.501e6; // J kg-1 
%#define DGCM_LATENT_HEAT_FUSION_3 3.34e5
 DGCM_T_LIQUID_H2O_ZERO_ENERGY=56.79;%//Zero-energy temperature of super-cooled liquid water 56.79 K
%#define DGCM_SEC_DAY (24*60*60)//Seconds per day
    
    
    U=(1 - LF).* DGCM_SPECIFIC_HEAT_ICE.* TEMP + LF .* DGCM_SPECIFIC_HEAT_WATER.* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);
    


end


function augment_h2o_problem_with_temp_energy

%INCOMPLETE: DO WITH DEGREES C FOR NOW

%Need to set up a "Truth" here as well


%uncertainty 
x_prior_flow=[10,20,30,40,50]';
x_prior_flow_unc=[5,10,15,20,25]';

%Degrees C
x_prior_temp=[2,4,6,8,10]';
%Temp to energy uncertainty
Uunc=3;%1 deg C
x_prior_temp_unc=[1,1,1,1,1]'*Uunc;

x_prior_e=x_prior_temp.*x_prior_flow;
x_prior=[x_prior_flow;x_prior_e];


x_prior_energy_unc=x_prior_flow.*x_prior_temp_unc;


%Make covariance matrix here:
C_x_prior=diag([x_prior_flow_unc;x_prior_energy_unc]');



%Translating to "energy"-weighed temps
%Bring in values here

Asingle=forward_model;

A=Asingle;

A(end+1:end*2,end+1:end*2)=A;
%Characterize uncertainty

observables=A*x_prior;
normalizeobs=ones(size(observables));
normalizeobs(end/2+1:end)=observables(1:end/2);
obs_with_energy=observables./normalizeobs;

% [   60.0000
%   150.0000
%     4.6667
%     7.3333


%Uncertaingty of flow and temperature 
Obs_temp_unc=[10,10];
y_obs_flow=[50; 160];
y_obs_energy=[5;7].*y_obs_flow;
y_obs_energy_unc=y_obs_flow'.*Obs_temp_unc;
y_obs_unc_diag=[5,5,y_obs_energy_unc];

y_obs=[y_obs_flow;y_obs_energy];


%Make covariances happen only 
%Here we have to calculae thye covariance as Obs_temp_unc*flow 
CRM=eye(4);
C_y_obs=diag(y_obs_unc_diag)*CRM*diag(y_obs_unc_diag);

C_y_obs(1,3)=y_obs_flow(1).*Obs_temp_unc(1);
C_y_obs(3,1)=C_y_obs(1,3);
C_y_obs(2,4)=y_obs_flow(2).*Obs_temp_unc(2);
C_y_obs(4,2)=C_y_obs(2,4);

%Next step: Now we can calculate Hessian
C_x_posterior=inv(A'*inv(C_y_obs)*A + inv(C_x_prior));


%Next step: Calculate posterior flow and temperature
x_posterior=x_prior + C_x_posterior*A'*(C_y_obs^-1)* (y_obs - A*x_prior);

%Now calculate temperature in x_posrerior

x_posterior_temp=x_posterior(end/2+1:end)./x_posterior(1:end/2);
cxpd=diag(C_x_posterior);
x_posterior_temp_unc=cxpd(end/2+1:end)./x_posterior(1:end/2);


end




function test_cov_offset

%Find the covariance that works with 
x=randn(1,1000);
x=x/std(x);
y=randn(1,1000);
y=y/std(y);

xo=0:1:100;
yo=0:1:100;
C=[];
V=[];
for xi=1:numel(xo)
for yi=1:numel(yo)

z=(x+xo(xi)).*(y+yo(yi));
cm=cov(x,z);
C(xi,yi)=cm(2,1);
V(xi,yi)=cm(2,2);

end
end




end

function A=forward_model
%Step 1. Set up problem, use example
%Networking matrix
N = [0,0,0,0,0;...
       0,0,0,0,0;...
       1,1,0,0,0;...
       0,0,0,0,0;...
       0,0,1,1,0];


S = [0,0,1,0,0;...
       0,0,0,0,1];

%Forward operator, such that 
% x: runoff
% y: discharge obs
% y = Ax
A= S*(eye(5)-N)^-1;


end


function OE_SETUP


A=forward_model;

%prior x and covariance
x_prior=[10,20,30,40,50]';
C_x_prior=diag([5,10,15,20,25]');

%Prior estimare of discharge obs (y_prior)
y_prior = A*x_prior;
%Discharge obs (y_obs)
y_obs=[50; 160];
C_y_obs=[10,0;0,10];

%Optimal estimation step 1. Posterior covariance matrix
C_x_posterior= (A'*C_y_obs^-1*A + C_x_prior^-1)^-1;


%Optimal estimation step 2. Posteriro covariance runoff
x_posterior = x_prior + C_x_posterior*A'*(C_y_obs^-1)* (y_obs - A*x_prior);

%Making some plots
figure(1);clf
subplot(2,3,1:2)
ph1=plotfun_points_and_unc([1:5]-0.1,x_prior,diag(C_x_prior));set(ph1,'Color','b');hold on
ph2=plotfun_points_and_unc([1:5]+0.1,x_posterior,diag(C_x_posterior));set(ph2,'Color','r')
set(gca,'xtick',1:5);xlabel('River reach'); ylabel('River Reach runoff [mm/day or whatnot]');
legend([ph1(1),ph2(1),ph1(2),ph2(2)],'Prior','Posterior','Prior Unc','Posterior Unc');
subplot(2,3,3);hold on
ph1=plot([1:2]-0.1,y_prior,'bo','LineWidth',2);
ph2=plotfun_points_and_unc([1:2]+0.1,y_obs,diag(C_y_obs));set(ph2,'Color','k');
legend([ph1,ph2(1),ph2(2)],'Prior','Observation','Observation Unc');
set(gca,'xtick',1:2);xlabel('River gauge'); ylabel('River Gauge streamflow [mm/day eq. or whatnot]');
xlim([0.5,2.5]);










10
20
30
40
50
60






Q=[1,2,3,4,5];


%Step 2.

%Read Cedric's example
% m3_riv
% rivid
% time
% time_bnds
% lon
% lat
% crs
fname_m3_riv='DATA/RAPID_CEDRIC_TEST_CASE/m3_riv_Test_20000101_20000102.nc4';
fname_Qinit='DATA/RAPID_CEDRIC_TEST_CASE/Qinit_Test_20000101_20000102.nc4';

m3_riv=ncread(fname_m3_riv,'m3_riv');
rivid=ncread(fname_m3_riv,'rivid');
Qout=ncread(fname_Qinit,'Qout');



end


