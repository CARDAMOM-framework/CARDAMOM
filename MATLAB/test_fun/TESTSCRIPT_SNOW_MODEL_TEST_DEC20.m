function TEMPSCRIPT_SNOW_MODEL_TEST_DEC20

keyboard

%SNOW_ENERGY = wmass * ( (1.0 - fliq) * cice * temp & + fliq * cliq * (temp - tsupercool_liq) )
%Snow energy per mm SWE (which means energy/mm/m2)
PARS.cice = 2.093e3;% ! Ice specific heat (Ci) [ J/kg/K]13:34
PARS.cliq = 4.186e3; % ! Liquid water specific heat (Cl) [ J/kg/K]
%     tsupercool_liq = t3ple - (uiicet3 + alli ) * cliqi13:54
%     temp = uint * cliqi + tsupercool_liq13:53

PARS.tp = 273.16; %triple-point temp.
PARS.latent_heat_fusion = 334000; % J/kg
PARS.IE_at_0C_solid=PARS.cice*PARS.tp;
PARS.IE_at_0C_liquid=PARS.cice*PARS.tp + PARS.latent_heat_fusion;
PARS.tsupercool_liq =  PARS.tp  - (PARS.IE_at_0C_liquid)/PARS.cliq; %[K];


%Snow precip (assuming liquid precip = 0)
PRECsnow = [1,1,1,1,0,0,0,0,0,1,1,1]; %mm/day (liquid water equivalent)
deltat=365.25/12;
%Assume zero soil-snow flux

%SW
%./CARDAMOM_JPL/PYTHON/ERA5_API/ERA5_API_DOWNLOADER.py -o  ./DATA/ERA5/wyoming_snow_test.nc -v surface_solar_radiation_downwards  --year 2017  --month 01 02 03 04 05 06 07 08 09 10 11 12   --area 43 -111 44 -110    --monthly_average    --grid_size  1 1


%MJ/m2/d

SWdown = [    7.3381   11.5162   16.8397   21.9049   25.4404   28.4061   28.5217   24.6995   19.1908   12.8110    8.0631    5.7136];

 %Lowngwave (MJ/m2/d)
 
LWdown = [254, 266, 274, 286, 295, 335, 355, 360, 340, 326, 282, 254]*3600*24/1e6*1.02;

Lwup = [271, 283, 292, 305, 314, 357, 378, 383, 362, 347, 300, 271]*3600*24/1e6;

%ET can be calculated offline
ET = SWdown * 0;%mm/d

albedo=0.95;

SWup=SWdown*albedo;

Rnet = SWdown + LWdown - SWup - Lwup;

%"S" is state variable
%Snow SWE in Jan 1st  = 50 mm
SNOW(1) = 50;
%Snow temp initial condition (K)
SNOW_TEMP(1) = 268;


%E_tsw = M_tsw *c_i*T_tsw T_tsw = E_tsw/(c_i*M_tsw)13:34

%snow2energy= @(snowtemp) (1.0 - fliq) * cice * temp & + fliq * cliq * (temp - tsupercool_liq) 
%l = (\frac{Q_{tsw}-Q_{tsw0}}{Q_{tsw1}-Q_{tsw0}})13:40
FUNC.snow_fliq=@(snow_ie, PARS) min(max((snow_ie - PARS.IE_at_0C_solid )/ (PARS.IE_at_0C_liquid - PARS.IE_at_0C_solid),0),1);
%FUNC.snow_temp@(

    
    
    
snow2energy= @(snowtemp,PARS,fliq) (1.0 - fliq) * PARS.cice * snowtemp & + fliq * PARS.cliq * (snowtemp - PARS.tsupercool_liq) ;

%SNOW_IE(1) = ;



%dSWE/dt = PRECsnow - Sublimation - Melt
for m=1:12
    
    %MELT (m) = 
    
    SNOW(m+1) =SNOW(m) + PRECsnow(m)*deltat  - ET(m) - MELT(m);
    
end


end


%This can be the CARDAMOM_wide temp calculation
%Adapt for dry soil heat capacity
%Look at Longo overleaf or ED2 fortran code for dry soil stuff.
function temp_all=snow_temp(snow_ie,PARS)


for n=1:numel(snow_ie)
if snow_ie(n)>PARS.IE_at_0C_liquid


    temp=(snow_ie(n))./PARS.cliq + PARS.tsupercool_liq;
    
elseif snow_ie(n)<PARS.IE_at_0C_solid
    
    
        temp=(snow_ie(n))./PARS.cice;

else
    temp=PARS.tp;
end

temp_all(n)=temp;

end

end

