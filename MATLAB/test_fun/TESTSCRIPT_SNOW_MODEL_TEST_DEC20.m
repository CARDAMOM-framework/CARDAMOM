function TESTSCRIPT_SNOW_MODEL_TEST_DEC20

keyboard

%SNOW_ENERGY = wmass * ( (1.0 - fliq) * cice * temp & + fliq * cliq * (temp - tsupercool_liq) )
%Snow energy per mm SWE (which means energy/mm/m2)
PARS.cice = 2.093e3;% ! Ice specific heat (Ci) [ J/kg/K]13:34
PARS.cliq = 4.186e3; % ! Liquid water specific heat (Cl) [ J/kg/K]
%     tsupercool_liq = t3ple - (uiicet3 + alli ) * cliqi13:54
%     temp = uint * cliqi + tsupercool_liq13:53

PARS.tp = 273.16; %triple-point temp.
PARS.latent_heat_fusion = 334000; % J/kg
PARS.latent_heat_vap=2.5e6;%J/kg
PARS.latent_heat_sub=PARS.latent_heat_fusion + PARS.latent_heat_vap;%J/kg
PARS.IE_at_0C_solid=PARS.cice*PARS.tp;
PARS.IE_at_0C_liquid=PARS.cice*PARS.tp + PARS.latent_heat_fusion;
PARS.tsupercool_liq =  PARS.tp  - (PARS.IE_at_0C_liquid)/PARS.cliq; %[K];


%Snow precip (assuming liquid precip = 0)
PRECsnow = [1,1,1,1,0,0,0,0,0,1,1,1]; %mm/day (liquid water equivalent)
AIRtemp= [-5,-5,-5,-3,0,4,10,10,4,-2,-3,-5]; %mm/day (liquid water equivalent)
deltat=365.25/12;
%Assume zero soil-snow flux

%SW MJ/m2/d
%./CARDAMOM_JPL/PYTHON/ERA5_API/ERA5_API_DOWNLOADER.py -o  ./DATA/ERA5/wyoming_snow_test.nc -v surface_solar_radiation_downwards  --year 2017  --month 01 02 03 04 05 06 07 08 09 10 11 12   --area 43 -111 44 -110    --monthly_average    --grid_size  1 1
SWdown = [7.3381   11.5162   16.8397   21.9049   25.4404   28.4061   28.5217   24.6995   19.1908   12.8110    8.0631    5.7136];
%Lowngwave (MJ/m2/d)
LWdown = [254, 266, 274, 286, 295, 335, 355, 360, 340, 326, 282, 254]*3600*24/1e6*1.02;
% LWdown =[22.3845   23.4420   24.1471   25.2046   25.9978   29.5229   31.2854   31.7261   29.9635   28.7297   24.8521   22.3845]
LWup = [271, 283, 292, 305, 314, 357, 378, 383, 362, 347, 300, 271]*3600*24/1e6;
% LWup =[23.4144   24.4512   25.2288   26.3520   27.1296   30.8448   32.6592   33.0912   31.2768   29.9808   25.9200   23.4144]

% -------------------------
% downloaded from ERA5
%./CARDAMOM_JPL/PYTHON/ERA5_API/ERA5_API_DOWNLOADER.py -o ./RESEARCH/DATA/ERA5_API/wyoming_radiation_test.nc -v surface_net_solar_radiation surface_net_thermal_radiation surface_solar_radiation_downwards surface_thermal_radiation_downwards --year 2017 --month 01 02 03 04 05 06 07 08 09 10 11 12   --area 43.5 -110.5 43.5 -110.5 --monthly_average --grid_size 1 1

% unit covert from J/m2/day to MJ/m2/day  (/1e6)
 SWdown = [7.5455    9.1782   15.4878   20.6763   27.3366   27.1438   27.7512   24.4484   17.6172   13.7335    7.1347    5.8443];
 LWdown = [ 18.2143   21.2924   21.8513   21.1625   21.6022   23.9659   25.6225   24.7010   24.0173   20.4801   21.5173   19.8812];
 %Net upward  = LWup - LWdown
 %LWup = LWdown + Net upward  
 LWup = ([3.9281    3.0944    3.9953    4.7450    6.2141    5.6763    7.1693    7.3669    5.4266    4.9323    3.3277    3.2545] +  LWdown);

% in W/m2 /(3600*24)
% SWdown = [87.3325  106.2289  179.2574  239.3091  316.3962  314.1638  321.1939  282.9676  203.9033  158.9526   82.5774   67.6428];
% LWdown = [210.8138  246.4401  252.9088  244.9368  250.0251  277.3837  296.5569  285.8908  277.9777  237.0380  249.0425  230.1061];
% LWup = [45.4647   35.8150   46.2419   54.9189   71.9221   65.6975   82.9775   85.2654   62.8074   57.0869   38.5149   37.6683];
% ratio of netSW/SWdown = [0.6538    0.6533    0.6629    0.6566    0.6778  0.6924    0.8487    0.8718    0.8088    0.6811    0.6597    0.6552]

% snow variables
% snowcover=[99.9512   99.9512   99.9512   99.9512   99.9512   99.8947   11.3079     0   37.8159   94.7090   97.9974   99.9512]
% SWE=[0.2658    0.4133    0.5333    0.6217    0.6410    0.3495    0.0078     0    0.0105    0.0255    0.0953    0.1821]
% snowmelt=[0         0         0         0    0.0032    0.0173    0.0027         0    0.0008    0.0013         0         0]
 snowsurft=[258.9934  265.2638  268.6025  268.8325  271.9822  273.0088  273.0925  273.1602  271.8199  267.3160  266.4712  262.0739];
skintemp=[259.4507  265.7600  269.7007  269.9660  274.7928  279.0397  286.2732  284.8947  278.5555  268.4642  266.9638  262.3272];
%-------------------------

% ET can be calculated offline
ET = SWdown * 0.02;%mm/d

%CONST.ant for now
%Changes in reality
%Shuang has snow albedo timeseries
CONST.albedo=0.95;

CONST.emissivity = 0.97;

%Aerodynamic resistance
fric_vel=0.1;
CONST.Hconductance= fric_vel/5;%m/s

CONST.air_density = 1;%kg/m3 %Function of ERA5 pressure/specific humidity/temperature
CONST.Cp=1004; %J/kg/K %specific heat of air at const pressure

%UNITS CHECK: Hconductance * ( delta_TEMP)*CONST.air_density *CONST.Cp 
%UNITS = J/m2/s



CONST.sigmaSB=5.67e-8;

SWup=SWdown*CONST.albedo;

Rnet = SWdown + LWdown - SWup - LWup;

%"S" is state variable
%Snow SWE in Jan 1st  = 50 mm
%Units mm of liquid water
%1mm/m2 = 1kg H2O
%Meaning mm/m2 and kg can be used interchangeably.
SNOW_H2O(1) = 50;
%Snow temp initial condition (K)
SNOW_TEMP(1) = 268;






%E_tsw = M_tsw *c_i*T_tsw T_tsw = E_tsw/(c_i*M_tsw)13:34

%snow2energy= @(snowtemp) (1.0 - fliq) * cice * temp & + fliq * cliq * (temp - tsupercool_liq) 
%l = (\frac{Q_{tsw}-Q_{tsw0}}{Q_{tsw1}-Q_{tsw0}})13:40
FUNC.snow_fliq=@(snow_ie, PARS) min(max((snow_ie - PARS.IE_at_0C_solid )/ (PARS.IE_at_0C_liquid - PARS.IE_at_0C_solid),0),1);
%FUNC.snow_temp@(

    
 
FUNC.snow2energy= @(snowtemp,PARS,fliq) (1.0 - fliq) * PARS.cice * snowtemp + fliq * PARS.cliq * (snowtemp - PARS.tsupercool_liq) ;



%Snow internal energy per unit snow (mm) * SNOW MASS
SNOW_IE(1) = FUNC.snow2energy(SNOW_TEMP(1), PARS, 0) * SNOW_H2O(1);


SNOW_LF(1)=FUNC.snow_fliq(SNOW_IE(1) , PARS);




%dSWE/dt = PRECsnow - Sublimation - Melt
for m=1:12
    
    %Step 1. Add snow H2O contribution to snow pack
    %Note: rain goes straight to surface runoff, 
    %ASSUMPTION: liquid precip no interaction with snow energy balance.
    
    %One month timestep (adding snow in mm/month)
    SNOW_H2O(m+1) = SNOW_H2O(m) +PRECsnow (m)*365.25/12;
    
    %Step 2. NEXT STEP, calculate PRECsnow_IE
    %Re-analysis data includes snow:rain fraction
    %For now: assume snow is <= 0C
    %Energy per unit mass X mass
    
    LWup(m)=CONST.emissivity*skintemp(m).^4*CONST.sigmaSB*24*3600*1e-6;%MJ/d/m2
    
    %Radiative fluxes (J/m2/m)
    RAD_FLUXES(m)= ( SWdown(m)*(1-CONST.albedo) +LWdown(m)*CONST.emissivity -LWup(m))*1e6*365.25/12;
    
    %Turbulent fluxes (ground to air flux = +ve)
    %J/m2/s
        H(m) =CONST.Hconductance * (AIRtemp(m)+PARS.tp - skintemp(m))*CONST.air_density *CONST.Cp *3600*24*365.25/12;
    
        %LE = ET * 
        %***CONTINUE FROM HERE NEXT TIME: any missing terms (?)***
        LE(m) = ET*PARS.latent_heat_sub ;
        
    %Track energy in Jules
         SNOW_IE(m+1) = SNOW_IE(m) + FUNC.snow2energy(min(AIRtemp(m)+PARS.tp, PARS.tp),PARS,0) *PRECsnow(m) + RAD_FLUXES(m);
         


         %Step 3. Cas
         
             MELT(m) = (SNOW_TEMP(1)  -  PARS.tp) *PARS.cice * (SNOW_H2O(m)*(1-SNOW_LF(m));

    %
         
         
    %    FUNC.snow2energy(SNOW_TEMP(m),PARS,

    FUNC.snow_fliq
    
    %Snow melt at each timestep
    %TO DO NEXT:
    %Calculate melt as a function of change in liquid fraction
    %Calculate liquid fraction as a function of change in energy
    %"Melt" only occurs if SNOW_LF(t+1) - SNOW_LF(t) > 0;
    %"Re-Freezing" occurs if SNOW_LF(t+1) - SNOW_LF(t) < 0;
    %Option 1 = prognostically track SNOW_LF, but ALSO decide how much (if
    %not all) leaves snowpack
    %Option 2 = SNOW_LF is always zero (so snow is drained/dry at
    %begining of each iteration), so no refreezing.
    
    
    
      
    %Snow H2O balance
    SNOW_H2O(m+1) =SNOW_H2O(m) + PRECsnow(m)*deltat  - ET(m) - MELT(m);
    %Snow internal energy
    %SNOW_IE(m+1) =SNOW_IE(m) + 
    
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





function cardamomized_snow


load  MATLAB/test_fun/ERAforcing_wyo_2008_2017.mat  
load CARDAMOM/MATLAB/test_fun/ERAsupp_wyo_2008_2017.mat

% 
% SWE=ERAsuppwyo.swe_m_;
% MELT=ERAsnowwyo.smlt_m_;
% SKIN=ERAsnowwyo.skt_K_;
% SNOW=ERAsnowwyo.snfall_m_;






%Snow data
ERA5.swe=ERAsuppwyo.swe_m_*1000;
ERA5.snow=ERAsnowwyo.snfall_m_*1000;
ERA5.snow_melt=ERAsnowwyo.smlt_m_*1000;
ERA5.skintemp=ERAsnowwyo.skt_K_;
ERA5.srad=ERAsnowwyo.SWdown_MJ_m2_d_;


t1=275;
t2=0.02;
    MELT_frac_exp=min(1,max((ERA5.skintemp-t1)*t2,0));
    
    
        MELT_frac_exp=min(1,max(((250:290)-t1)*t2,0));
        
        MELT_frac_exp_conc=min(1,max(((ERA5.skintemp)-t1)*t2,0));

    %Step 1. plot
    figure(1);clf;hold on;
    subplot(2,1,1);hold on
    plot(ERA5.skintemp,ERA5.snow_melt./ERA5.swe,'o')
    plot(250:290,MELT_frac_exp,'r')
    
    ylabel('Snow melt / SWE [fraction]')
        subplot(2,1,2);
        plot(ERA5.snow_melt./ERA5.swe,'ko--'); hold on

            plot( MELT_frac_exp_conc,'b')



SWEmodel(1)=300; %Parameter 1. Initial condition
t1=270; %min threshold for melt
t2=0.1;% slope 
deltat=365.25/12;
for m=1:numel(ERA5.swe);
            MELT_frac(m)=min(max((ERA5.skintemp(m)-t1)*t2,0),1);

    SWEmodel(m+1)=max(SWEmodel(m)+ERA5.snow(m)*deltat - MELT_frac(m)*SWEmodel(m),0);
    MELTmodel(m)=MELT_frac(m)*SWEmodel(m);
end

figure(2);clf;hold on
plot(ERA5.swe,'k','Linewidth',2)


figure(3);clf;hold on
plot(SWEmodel,'b','Linewidth',2)
plot(ERA5.swe,'k--o','Linewidth',2);
ylabel('SWE [mm]')
xlabel('Months since Jan 2008');
legend('Reduced Complexity', 'ERA5')






figure(4);clf;hold on
plot(SWEmodel,'b','Linewidth',2)
plot(ERA5.swe,'k--o','Linewidth',2)








end

function cardamomized_snow_old


load  MATLAB/test_fun/ERAforcing_wyo_2008_2017.mat  

load CARDAMOM/MATLAB/test_fun/ERAsupp_wyo_2008_2017.mat


SWE=ERAsuppwyo.swe_m_;
MELT=ERAsnowwyo.smlt_m_;
SKIN=ERAsnowwyo.skt_K_;



%Snow flux
METDATAPRECsnow = [3.50 6.50 2.80 3.70 0.90 0.30 0. 0. 1.40 1.40 4.60 2.50]; % unit mm/day liquid water equivalent, ERA5

%LST
METDATAskintemp=[259.4507  265.7600  269.7007  269.9660  274.7928  279.0397  286.2732  284.8947  278.5555  268.4642  266.9638  262.3272];




%Snow data
ERA5data=load('MATLAB/test_fun/ERAsnow_wyo_2017.mat');
ERA5.swe=ERA5data.ERAsnowwyo.swe_m_*1000;
ERA5.snow=ERA5data.ERAsnowwyo.snfall_m_*1000;
ERA5.snow_melt=ERA5data.ERAsnowwyo.smlt_m_*1000;
ERA5.skintemp=ERA5data.ERAsnowwyo.skt_K_;


t1=275;
t2=0.02;
    MELT_frac_exp=min(1,max((ERA5.skintemp-t1)*t2,0));
    
    
        MELT_frac_exp=min(1,max(((250:290)-t1)*t2,0));

    %Step 1. plot
    figure(1);clf;hold on;
    
    plot(ERA5.skintemp,ERA5.snow_melt./ERA5.swe,'o')
    plot(250:290,MELT_frac_exp,'r')
    ylabel('Snow melt / SWE [fraction]')


SWE(1)=300; %Parameter 1. Initial condition
t1=270; %min threshold for melt
t2=0.1;% slope 
deltat=365.25/12;
for m=1:12;
            MELT_frac(m)=min(max((ERA5.skintemp(m)-t1)*t2,0),1);

    SWE(m+1)=max(SWE(m)+ERA5.snow(m)*deltat - MELT_frac(m)*SWE(m),0);


end

figure(2);clf;hold on
plot(ERA5.swe,'k','Linewidth',2)


figure(3);clf;hold on
plot(SWE,'b','Linewidth',2)
plot(ERA5.swe,'k','Linewidth',2)










end

