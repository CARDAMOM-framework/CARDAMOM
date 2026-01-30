function GLWDdata=DATASCRIPT_READ_GLWD_WETLANDS_DATASET(res)

% 1. GLWD is in adf format
% 2. To convert to ascii format, we used
% - gdal_translate -of AAIGrid w001001_cp.adf output_dataset.txt
% 3. Then loaded into matlab (took 20 min, as output....txt is 3GB)
% 4. saved as GLWD.mat
% 5. deleted output_dataset.txt
defval('res',1);

filepath='/Users/abloom/DATA/GLWD_WETLANDS/GLWD-level3/glwd_3/';
filename=[filepath,'GLWD.mat'];
disp('Takes a while :)');
load(filename);
%a
GLWD=flipud(GLWD);
GLWD(GLWD==255)=0;

GLWDdata.glwd=GLWD;

%grid to 1x1 resolution
%12 wetland types
[x,y]=loadworldmesh(res);
fracwet=repmat(x*0,[1,1,13]);

for m=1:13
    for r=1:120*res
        for c=1:120*res
            fracwet(:,:,m)=fracwet(:,:,m)+double(GLWD(r:120*res:end,c:120*res:end)==m-1)/((120*res)^2);
        end
    end    
end

GLWDdata.data=fracwet;

%Based on /Users/abloom/DATA/GLWD_WETLANDS/GLWD-level3/GLWD_Data_Documentation.pdf 
%
% 1 No wetland
% 2 Lake
% 3 Reservoir
% 4 River
% 5 Freshwater Marsh, Floodplain
% 6 Swamp Forest, Flooded Forest
% 7 Coastal Wetland (incl. Mangrove, Estuary, Delta, Lagoon)
% 8 Pan, Brackish/Saline Wetland
% 9 Bog, Fen, Mire (Peatland)
% 10 Intermittent Wetland/Lake
% 11 50-100% Wetland
% 12 25-50% Wetland
% 13 Wetland Compex (0-25% Wetland)

FWtype{1} = 'No wetland';
FWtype{2} = 'Lake';
FWtype{3} = 'Reservoir';
FWtype{4} = 'River';
FWtype{5} = 'Freshwater Marsh, Floodplain';
FWtype{6} = 'Swamp Forest, Flooded Forest';
FWtype{7} = 'Coastal Wetland (incl. Mangrove, Estuary, Delta, Lagoon)';
FWtype{8} = 'Pan, Brackish/Saline Wetland';
FWtype{9} = 'Bog, Fen, Mire (Peatland)';
FWtype{10} = 'Intermittent Wetland/Lake';
FWtype{11} = '50-100% Wetland';
FWtype{12} = '25-50% Wetland';
FWtype{13} = 'Wetland Compex (0-25% Wetland)';

GLWDdata.type=FWtype;


%lons and lats coordinates 
GLWDdata.lon=-180+1/240:1/120:180;
GLWDdata.lat=-90+1/240:1/120:90;

%read

