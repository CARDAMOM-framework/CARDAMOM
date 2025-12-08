function [CARDAMOM_TIMESTEP, CARDAMOM_DOY]=cardamomfun_populate_time_indices(startyear, endyear,time_res_opt)

%All this can be called from function CARDAMOM_PROJECT_TEMPLATE_NOV25 or
%anywhere else
%Curently only supporting 
%COnvention: timestep 0 = Jan 01 2001 00:00:00
if nargin==2; time_res_opt='monthly_solar';end
if strcmp(time_res_opt,'monthly_solar')

    CARDAMOM_TIMESTEP=365.25/24 + (startyear-2001)*365.25:365.25/12:(endyear+1-2001)*365.25;
    CARDAMOM_DOY=mod(CARDAMOM_TIMESTEP,365.25);


end




end