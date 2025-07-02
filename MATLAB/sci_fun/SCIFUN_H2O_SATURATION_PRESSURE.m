function VPSAT=SCIFUN_H2O_SATURATION_PRESSURE(T)
%function P=H2O_SATURATION_PRESSURE(T)
%input = Temperature (degrees C)
%output = Water vapour saturation pressure(kPa)
%Method:
%NOAA website http://www.srh.noaa.gov/epz/?n=wxcalc_vaporpressure
%formulas in http://www.srh.noaa.gov/images/epz/wxcalc/vaporPressure.pdf

%
%Alternative
%ref:  PAPERS/manuals_userguides_etc/Humidity_Conversion_Formulas_B210973EN-F.pdf 
%OR %http://www.vaisala.com/Vaisala%20Documents/Application%20notes/Humidity_Conversion_Formulas_B210973EN-F.pdf


%in kPA (Temperature must be in C)
%The vapor pressure answers will be in units of millibars (mb) or hectoPascals (hPa)
%converting to kilopascals(divide by 10);

VPSAT=6.11*10.^(7.5*T./(237.3+T))./10 ;


%NOTE: for VPD use this relationship
%xs=0.6*VPSAT/(101-VPSAT)
%http://www.engineeringtoolbox.com/humidity-ratio-air-d_686.html



end


function VPSAT=SCIFUN_H2O_SATURATION_PRESSURE(T)


VPSAT=6.11*10.^(7.5*T./(237.3+T))./10 ;

end