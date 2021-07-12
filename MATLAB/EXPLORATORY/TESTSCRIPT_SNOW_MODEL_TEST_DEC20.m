function TESTSCRIPT_SNOW_MODEL_TEST_DEC20

disp('Go straight to ERA5 data, and start swapping "correct forcings" into MET datasets');
disp('This will either (a) solve our problems, or (b) make it clear what next best step forward is');
disp('Once all ERA5 datasets are in place, first check = is internal energy (J) ~ enegry fluxes (J/month), if so, then finer timestep is 100% warranted');

%keyboard



%Options going forward
%(1) finer timestep for addressing numerical instability... (i) represent
%daily would mean daily fluxes<<energy storage, which is desirable, (ii)
%diurnal, which is key for removing LW representation errors, (iii) need to
%do both.
%
%Get all snow metrics available from ERA5: snow evaporation, + anything else
%


PARS=parameter_constants;



%Snow precip (assuming liquid precip = 0)
%METDATAPRECsnow = [1,1,1,1,0,0,0,0,0,1,1,1]; %mm/day (liquid water equivalent)
METDATAPRECsnow = [3.50 6.50 2.80 3.70 0.90 0.30 0. 0. 1.40 1.40 4.60 2.50]; % unit mm/day liquid water equivalent, ERA5
% total precipitation = [3.50 6.70 3.30 4.20 1.40 2.0 0.90 0.90 3.70  1.80  4.90 2.50]% unit mm/day liquid water equivalent, ERA5
deltat=365.25/12;
%Assume zero soil-snow flux

%SW MJ/m2/d
%./CARDAMOM_JPL/PYTHON/ERA5_API/ERA5_API_DOWNLOADER.py -o  ./DATA/ERA5/wyoming_snow_test.nc -v surface_solar_radiation_downwards  --year 2017  --month 01 02 03 04 05 06 07 08 09 10 11 12   --area 43 -111 44 -110    --monthly_average    --grid_size  1 1
METDATASWdown = [7.3381   11.5162   16.8397   21.9049   25.4404   28.4061   28.5217   24.6995   19.1908   12.8110    8.0631    5.7136];
%Lowngwave (MJ/m2/d)
METDATALWdown = [254, 266, 274, 286, 295, 335, 355, 360, 340, 326, 282, 254]*3600*24/1e6*1.02;
% LWdown =[22.3845   23.4420   24.1471   25.2046   25.9978   29.5229   31.2854   31.7261   29.9635   28.7297   24.8521   22.3845]
METDATALWup = [271, 283, 292, 305, 314, 357, 378, 383, 362, 347, 300, 271]*3600*24/1e6;
% LWup =[23.4144   24.4512   25.2288   26.3520   27.1296   30.8448   32.6592   33.0912   31.2768   29.9808   25.9200   23.4144]

% -------------------------
% downloaded from ERA5
%./CARDAMOM_JPL/PYTHON/ERA5_API/ERA5_API_DOWNLOADER.py -o ./RESEARCH/DATA/ERA5_API/wyoming_radiation_test.nc -v surface_net_solar_radiation surface_net_thermal_radiation surface_solar_radiation_downwards surface_thermal_radiation_downwards --year 2017 --month 01 02 03 04 05 06 07 08 09 10 11 12   --area 43.5 -110.5 43.5 -110.5 --monthly_average --grid_size 1 1

% unit covert from J/m2/day to MJ/m2/day  (/1e6)
 METDATASWdown = [7.5455    9.1782   15.4878   20.6763   27.3366   27.1438   27.7512   24.4484   17.6172   13.7335    7.1347    5.8443];
 METDATALWdown = [ 18.2143   21.2924   21.8513   21.1625   21.6022   23.9659   25.6225   24.7010   24.0173   20.4801   21.5173   19.8812];
 %netLWup  = LWup - LWdown
 %LWup = LWdown + netLWup
 %netLWup = [3.9281    3.0944    3.9953    4.7450    6.2141    5.6763    7.1693    7.3669    5.4266    4.9323    3.3277    3.2545];
 METDATALWnet=[3.9281    3.0944    3.9953    4.7450    6.2141    5.6763    7.1693    7.3669    5.4266    4.9323    3.3277    3.2545] ;
 METDATALWup = ( METDATALWnet +  METDATALWdown);

% in W/m2 /(3600*24)
% SWdown = [87.3325  106.2289  179.2574  239.3091  316.3962  314.1638  321.1939  282.9676  203.9033  158.9526   82.5774   67.6428];
% LWdown = [210.8138  246.4401  252.9088  244.9368  250.0251  277.3837  296.5569  285.8908  277.9777  237.0380  249.0425  230.1061];
% LWup = [45.4647   35.8150   46.2419   54.9189   71.9221   65.6975   82.9775   85.2654   62.8074   57.0869   38.5149   37.6683];
% ratio of netSW/SWdown = [0.6538    0.6533    0.6629    0.6566    0.6778  0.6924    0.8487    0.8718    0.8088    0.6811    0.6597    0.6552]

% snow variables
% snowcover=[99.9512   99.9512   99.9512   99.9512   99.9512   99.8947   11.3079     0   37.8159   94.7090   97.9974   99.9512]
% SWE=[0.2658    0.4133    0.5333    0.6217    0.6410    0.3495    0.0078     0    0.0105    0.0255    0.0953    0.1821]
% snowmelt=[0         0         0         0    0.0032    0.0173    0.0027         0    0.0008    0.0013         0         0]
METDATAsnowsurft=[258.9934  265.2638  268.6025  268.8325  271.9822  273.0088  273.0925  273.1602  271.8199  267.3160  266.4712  262.0739];
METDATAskintemp=[259.4507  265.7600  269.7007  269.9660  274.7928  279.0397  286.2732  284.8947  278.5555  268.4642  266.9638  262.3272];
METDATAAIRtemp=[260.1428  265.7048  269.4146  269.4298  274.1185  279.5416  286.9063  285.1542  278.7792  270.3499  267.0802 262.7811];%;Shuang: put ERA5 air temp here.
%-------------------------


%Era5 fieldsERA5=ERA5_DATA
ERA5=ERA5_DATA;

%Monthly flux (J/m2/month)
ERA5_H_FLUX=ERA5.H;


% ET can be calculated offline
METDATAET = METDATASWdown * 0.01;%mm/day

%CONST.ant for now
%Changes in reality
%Shuang has snow albedo timeseries
CONST.albedo=0.85;

CONST.emissivity = 0.97;

%Aerodynamic resistance
fric_vel=0.1;
CONST.Hconductance= fric_vel/50;%m/s

CONST.air_density = 1;%kg/m3 %Function of ERA5 pressure/specific humidity/temperature
CONST.Cp=1004; %J/kg/K %specific heat of air at const pressure

%UNITS CHECK: Hconductance * ( delta_TEMP)*CONST.air_density *CONST.Cp 
%UNITS = J/m2/s



CONST.sigmaSB=5.67e-8;

DIAG.SWup=METDATASWdown*ERA5.albedo;

DIAG.Rnet = METDATASWdown + METDATALWdown - DIAG.SWup - METDATALWup;

%"S" is state variable
%Snow SWE in Jan 1st  = 50 mm
%Units mm of liquid water
%1mm/m2 = 1kg H2O
%Meaning mm/m2 and kg can be used interchangeably.
SNOW_H2O(1) = 100;
%Snow temp initial condition (K)
SNOW_TEMP(1) = 250;





%E_tsw = M_tsw *c_i*T_tsw T_tsw = E_tsw/(c_i*M_tsw)13:34

%snow2energy= @(snowtemp) (1.0 - fliq) * cice * temp & + fliq * cliq * (temp - tsupercool_liq) 
%IE=(1.0 - fliq) * cice * temp & + fliq * cliq * (temp - tsupercool_liq) ;
%l = (\frac{Q_{tsw}-Q_{tsw0}}{Q_{tsw1}-Q_{tsw0}})13:40

FUNC.snow_fliq=@(snow_ie_pum, PARS) min(max((snow_ie_pum - PARS.IE_at_0C_solid )/ (PARS.IE_at_0C_liquid - PARS.IE_at_0C_solid),0),1);
%OVERLEAF FOR SNOW TEMP FUNCTION;
%Re-arrange from above equations
%Ensure snow_ie is per unit mass
FUNC.snow_temp=@(snow_ie,PARS) min(snow_ie/PARS.cice,PARS.tp);

    
 
%Arg 1. temp
%ARg 2. par struct 
%Arg 3. fliq
FUNC.snow2energy= @(snowtemp,PARS,fliq) (1.0 - fliq) * PARS.cice * snowtemp + fliq * PARS.cliq * (snowtemp - PARS.tsupercool_liq) ;

% **********************************************************************
% calculate aero conductance for sensible heat transfer (as a function of wind speed)
% Inputs:
 PARS.AERO.z=10;% Velocity reference height in meters
 PARS.AERO.h=0.03; % Characteristic roughness height in meters
 PARS.AERO.kappa=0.4; % von Karman's constant
 v10=[0.4817    0.9765    0.8134    0.5430    0.1999    0.5219    0.3391    0.2098    0.3396    0.4556    1.0371    0.4066];% Horizontal wind velocity at reference height (z) in m/s
 u10=[0.0191    0.7561    0.9194    0.7794    0.2525    0.5823    0.2504    0.3517    0.1578    0.4720    0.3309    0.3905];
 METDATA.ws10=sqrt(v10.^2+u10.^2);
 
%  ws2=v10*4.87/ln(67.8*10-5.42);% roughly ws2=0.75*ws10
 PARS.AERO.d=0.7*PARS.AERO.h; % Approximation for zero-plane displacement height
 PARS.AERO.z_0=0.1*PARS.AERO.h; % Approximation for momentum roughness height
% Outputs: Aerodynamic resistance in s/m
 FUNC.aeroconductance=@(PARS,ws10) (PARS.AERO.kappa^2.*ws10)/log((PARS.AERO.z-PARS.AERO.d)./PARS.AERO.z_0).^2;

% **********************************************************************

%Snow internal energy per unit snow (mm) * SNOW MASS
SNOW_IE(1) = double(FUNC.snow2energy(SNOW_TEMP(1), PARS, 0) * SNOW_H2O(1));


SNOW_LF(1)=FUNC.snow_fliq(SNOW_IE(1)/SNOW_H2O(1) , PARS);


disp('NEXT STEP: CHECK SNOW TEMP ROUTINE')
disp('internal energy per area vs internal energy per kg check')
%keyboard

%dSWE/dt = PRECsnow - Sublimation - Melt


for m=1:12
    
    %Step 1. Add snow H2O contribution to snow pack
    %Note: rain goes straight to surface runoff, 
    %ASSUMPTION: liquid precip no interaction with snow energy balance.
    
    %One month timestep (adding snow in mm/month)
    %50mm
    %Assume all snow goes to snowpack
    SNOW_H2O(m+1) = SNOW_H2O(m) + METDATAPRECsnow (m)*deltat;
    
    %Step 2. NEXT STEP, calculate PRECsnow_IE
    %Re-analysis data includes snow:rain fraction
    %For now: assume snow is <= 0C
    %Energy per unit mass X mass
    %(J/m2/day)
    
    %Per unit snow area
    METDATALWup(m)=CONST.emissivity*min(METDATAskintemp(m),PARS.tp).^4*CONST.sigmaSB *24*3600*1e-6;%MJ/m2/d
%    METDATALWup(m)=CONST.emissivity*SNOW_TEMP(m).^4*CONST.sigmaSB *24*3600*1e-6;%MJ/m2/d
           LWUP(m) = METDATALWup(m)*1e6*deltat;


    %Radiative fluxes (J/m2/month)
        %Per unit snow area
        %Energy Loss = negative values

        SWNET(m)=METDATASWdown(m)*(1-CONST.albedo)*1e6*deltat;
        LWDOWN(m)=METDATALWdown(m)*CONST.emissivity*1e6*deltat;
        LWnet(m) = LWDOWN(m) -  LWUP(m);
        
    RAD_FLUXES(m)= SWNET(m) +LWnet(m);
    
    %Turbulent fluxes (ground to air flux = +ve)
    %J/m2/s
    %Convention = +ve flux upwards
    %npote : airtemp in celcius
    %H is in J/m2/month
        %Per unit snow area

        H(m) =CONST.Hconductance * (min(METDATAskintemp(m),PARS.tp) - (METDATAAIRtemp(m)))*CONST.air_density *CONST.Cp *3600*24*deltat;
    
        %ET = mm/day
        %Evapotranspiration flux
        %J/m2/month
            %Per unit snow area

        LE(m) = METDATAET(m)*PARS.latent_heat_sub*CONST.air_density*deltat;
        
        %Turbulent fluxes
        %Convention = +ve flux upward
            %Per unit snow area

        TURB_FLUXES(m) = H(m) + LE(m);
      %  TURB_FLUXES(m) = ERA5.H(m) + ERA5.LE(m);
    

       
       %ET units = mm/day
       %Water loss internal energy flux
           %Per unit snow area
       ET_MASS_ENERGY_FLUX(m) = METDATAET(m)* FUNC.snow2energy(SNOW_TEMP(m) ,PARS ,0)*deltat;
       
%Snow integrated flux (not per unit area)       
        SNOW_MASS_ENERGY_FLUX(m)= FUNC.snow2energy(min(METDATAAIRtemp(m)+PARS.tp, PARS.tp),PARS,0) *METDATAPRECsnow(m)*deltat ;
       
        %Surface met mass energy exchange  (excluding melt)
        %Snow input
        %ET 
        
                %Shunag has a few option option
        
        SNOW_COVER_FRAC(m)=double(SNOW_H2O(m)>30);
        SNOW_COVER_FRAC(m)=min(SNOW_H2O(m)/100,1);
        
        
%Assume we get 100% of new snow
%BUT ET scales with snow cover

        MASS_EXCHANGE_ENERGY_FLUXES(m) = SNOW_MASS_ENERGY_FLUX(m) - ET_MASS_ENERGY_FLUX(m)*SNOW_COVER_FRAC(m);
         

        
        
    %Track energy in Jules
    %CONTINUE FROM HERE (WHY IS IE NEGATIVE)?
    %SNOW_IE and MASS_EXCHANGE_ENERGY_FLUXES 
         SNOW_IE(m+1) = max(SNOW_IE(m) + MASS_EXCHANGE_ENERGY_FLUXES(m)+ (RAD_FLUXES(m) - TURB_FLUXES(m))*SNOW_COVER_FRAC(m),0);
         

         
         %Calculate temperature of snow after enegry exchange.
         if SNOW_H2O(m)>0;
             %if m==3;keyboard;end
         SNOW_TEMP(m+1) =  FUNC.snow_temp(SNOW_IE(m+1)/SNOW_H2O(m), PARS) ;
         %Snow LF correct for any temperature
         %FUNC.snow_fliq=@(snow_ie, PARS) min(max((snow_ie - PARS.IE_at_0C_solid )/ (PARS.IE_at_0C_liquid - PARS.IE_at_0C_solid),0),1);
      % If temp is above zero, snow LF calculation = 1
      % If temp is at zero, snow LF calculation = 0-1
      % If temp is below zero, snow LF calculation = 0
      %CONTINUE FROM HERE: check liquid fraction and internal energy
      
         SNOW_LF(m+1) =  FUNC.snow_fliq(SNOW_IE(m+1)/SNOW_H2O(m), PARS) ;
         

%         me%lt (m) = max(0, (snow_ie - snow_h2o * snow_ie_0C_all_ice));

                  %melt (m) = @(snow_ie,PARS) max(0, (snow_ie - PARS.IE_at_0C_solid));

         %units = SNOW_IE (J)
         %units =
         %
                 %MELT_ENERGY(m) =  max(0, (SNOW_IE(m+1) - SNOW_H2O(m) *PARS.IE_at_0C_solid));
                 
                 %MELT_IE = SNOW_IE - (1 - SNOW_LF) * SNOW_H2O * PARS.IE_0C_solid
                 
                 
                 %All units = J/m2/month
                 %Quantity = total energy removed through timestep
                 %(timestep is already implicit)
                    %MELT_ENERGY(m) = max(0, SNOW_IE(m+1) - (1 - SNOW_LF(m+1)) * SNOW_H2O(m) * PARS.IE_at_0C_solid);

                    %SNOW liq. frac x snow mass
                    
                  MELT_MASS(m) =    SNOW_LF(m+1) * SNOW_H2O(m)  ;
                  
                  MELT_ENERGY(m) =   MELT_MASS(m)* PARS.IE_at_0C_solid;


                 
                %MELT(m) = (SNOW_TEMP(m)  -  PARS.tp) *PARS.cice * (SNOW_H2O(m)*(1-SNOW_LF(m));

         else
              SNOW_TEMP(m+1) = NaN;
              SNOW_LF(m+1) = NaN;
                     MELT_ENERGY(m) = 0;
                     MELT_MASS(m) = 0;

         end
         %Step 3. Cas
         

         
                 %Melt 
        %Mass flux
        %MELT_MASS_FLUX(m) = 
        %Energy flux
    %
         
         
    %    FUNC.snow2energy(SNOW_TEMP(m),PARS,

    %FUNC.snow_fliq
    
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
    %ET units are in mm/day
    SNOW_H2O(m+1) =SNOW_H2O(m) + METDATAPRECsnow(m)*deltat  - METDATAET(m)*deltat - MELT_MASS(m);
    
                      SNOW_IE(m+1) = max(SNOW_IE(m+1) - MELT_ENERGY(m),0);

    %Snow internal energy
    %SNOW_IE(m+1) =SNOW_IE(m) + 
    
end


figure(1);clf;
for s=1:9;subplot(5,3,s);set(gca,'FontSize',10);end
subplot(5,3,1);plot(ET_MASS_ENERGY_FLUX);title('ET MASS ENERGY FLUX')
subplot(5,3,2);plot(MELT_ENERGY);title('MELT ENERGY');
subplot(5,3,3);plot(MASS_EXCHANGE_ENERGY_FLUXES);title(sprintf('MASS EXCHANGE\n ENERGY FLUXES'))
subplot(5,3,4);plot(RAD_FLUXES);title('RAD_FLUXES(m)')
subplot(5,3,5);plot(TURB_FLUXES);title('TURB_FLUXES')
subplot(5,3,6);plot(SNOW_COVER_FRAC);title('SNOW_COVER_FRAC')
subplot(5,3,7);plot(SWNET);title('SWnet');
subplot(5,3,8);plot(LWDOWN);title('LWdown');
subplot(5,3,9);plot(LWUP);title('LWup');
subplot(5,3,10);plot(LWnet);title('LWnet');
subplot(5,3,11);plot(SNOW_IE);title('IE');
subplot(5,3,12);plot(SNOW_TEMP);title('TEMP');
subplot(5,3,13);plot(H);title('H');
subplot(5,3,14);plot(LE);title('LE');
subplot(5,3,15);plot(SNOW_H2O);title('SNOW H2O');



for s=1:9;subplot(5,3,s);set(gca,'FontSize',10);end

+ (RAD_FLUXES(m) - TURB_FLUXES(m))*SNOW_COVER_FRAC(m);




keyboard


end


function ERA5=ERA5_DATA

load('MATLAB/test_fun/ERAsupp_wyo_2017.mat')
load('MATLAB/test_fun/ERAsnow_wyo_2017.mat')



%eva_baresoil(m),
%eva_topcano(m),
%eva_vegetran(m),
%forecast_albedo(0-1),
%potential_eva(m),stl1(K),stl2(K),stl3(K),stl4(K),swvl1(m3/m3),swvl2(m3/m3),swvl3(m3/m3),swvl4(m3/m3),LE(J/m2),H(J/m2)
  %ERAsuppwyo.
  
  ERA5.albedo = ERAsuppwyo.forecast_albedo_0_1_;
  ERA5.albedo = ERAsnowwyo.albsn_0_1_;
  
  deltat=365.25/12;


%ERA5 cponvention: net downward heat flux = +ve, i.e. _ve values = warmer
%atmosphere
  ERA5.H = -ERAsuppwyo.H_J_m2_*deltat;
  ERA5.LE = -ERAsuppwyo.LE_J_m2_*deltat;
  ERA5.snow_temp=ERAsnowwyo.surtsn_K_;
  ERA5.snow_evap=ERAsnowwyo.snevap_m_;
  %ERA5. = ERAsuppwyo.

end


function PARS=parameter_constants




%SNOW_ENERGY = wmass * ( (1.0 - fliq) * cice * temp & + fliq * cliq * (temp - tsupercool_liq) )
%Snow energy per mm SWE (which means energy/mm/m2)
PARS.cice = 2.093e3;% ! Ice specific heat (Ci) [ J/kg/K]13:34
PARS.cliq = 4.186e3; % ! Liquid water specific heat (Cl) [ J/kg/K]
%     tsupercool_liq = t3ple - (uiicet3 + alli ) * cliqi13:54
%     temp = uint * cliqi + tsupercool_liq13:53

PARS.tp = 273.16; %triple-point temp.
PARS.latent_heat_fusion = 334000; % J/kg
PARS.latent_heat_vap=2.5e6;%J/kg
%Note: latent heat of vaporization is (more accurately) 
%True for any temperature (simplification: not a function of temperature);
%E..g. look at ED2 or Williams et al., (2012) for linear temp dependence
%Climate and vegetation controls on the surface water balance: Synthesis of evapotranspiration measured across a global network of flux towers
PARS.latent_heat_sub=PARS.latent_heat_fusion + PARS.latent_heat_vap;%J/kg
PARS.IE_at_0C_solid=PARS.cice*PARS.tp;
PARS.IE_at_0C_liquid=PARS.cice*PARS.tp + PARS.latent_heat_fusion;
PARS.tsupercool_liq =  PARS.tp  - (PARS.IE_at_0C_liquid)/PARS.cliq; %[K];

end


%This can be the CARDAMOM_wide temp calculation
%Adapt for dry soil heat capacity
%Look at Longo overleaf or ED2 fortran code for dry soil stuff.


%Alternative form here
%FUNC.snow_temp=@(snow_ie,PARS) min(snow_ie/PARS.cice,PARS.tp);


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


SWE(1)=300; %Parameter 1. Initial condition [0.00001, 10000]
t1=270; %min threshold for melt Prior = [240 300]
t2=0.1;% slope (units = melt fraction per degree K). [0.0001 1]
deltat=365.25/12;
for m=1:12;
            MELT_frac(m)=min(max((ERA5.skintemp(m)-t1)*t2,0),1);

    SWE(m+1)=max(SWE(m)+ERA5.snow(m)*deltat - MELT_frac(m)*(SWE(m)),0);


end

figure(2);clf;hold on
plot(ERA5.swe,'k','Linewidth',2)


figure(3);clf;hold on
plot(SWE,'b','Linewidth',2)
plot(ERA5.swe,'k','Linewidth',2)










end

