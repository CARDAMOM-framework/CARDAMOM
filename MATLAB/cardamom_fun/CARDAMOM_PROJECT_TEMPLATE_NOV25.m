function CARDAMOM_PROJECT_TEMPLATE_NOV25

%Step 1. Read configuration structure and derive any auxilliary variables
%needed
CONFIG=READ_CONFIG_STRUCTURE;
AUXVAR=DERIVE_AUXILLIARY_VARIABLES(CONFIG);
%Step 2. Define domain driver files
%In this step





end

function AUXIFIELDS=DERIVE_AUXILLIARY_VARIABLES(CONFIG);



AUXFIELDS.longitude=
AUXFIELDS.latitude=




end

function CONFIG=READ_CONFIG_STRUCTURE;
%Each field here matches the 

CONFIG.time_res_opt='monthly_solar';



CONFIG.longitude_minmax=[-130,-80];
CONFIG.longitude_res=0.5;
CONFIG.latitude_minmax=[-3










end


