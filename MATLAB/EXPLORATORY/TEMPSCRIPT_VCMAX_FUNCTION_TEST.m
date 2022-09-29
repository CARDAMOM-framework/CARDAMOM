


DGCM_TK0C=273.15;
Tupp=300;
Tdown = 250;
vcmax25=100;
q_10=2;

T_C=-50:50;

%C code command
%Vcmax = vcmax25*pow(q_10,0.1*(T_C-25.))/((1 + exp(0.3*(T_C-Tupp-273.15)))*(1 +exp(0.3*(Tdown-T_C-273.15))));


Vcmax = vcmax25.*q_10.^(0.1.*(T_C-25.))./((1 + exp(0.3.*(T_C-(Tupp-DGCM_TK0C)))).*(1 +exp(0.3.*((Tdown-DGCM_TK0C)-T_C))));



