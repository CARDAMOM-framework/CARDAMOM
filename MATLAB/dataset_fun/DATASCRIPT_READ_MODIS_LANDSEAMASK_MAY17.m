function LSM=DATASCRIPT_READ_MODIS_LANDSEAMASK_MAY17(res)
%Based on DATASCRIPT_GRID_MODIS_LAI_APR17



[x,y]=loadworldmesh(res);
%cutoff = Lon of bottom-left corner of grid
            lonbl=x(1,1)-(x(1,2)-x(1,1))/2;

if isstr(res)==0;resstr=num2str(res);else; resstr=res;end
%making array of h and v and grid identifier number

%sequence
%1 - load first file#
%2 - bin data into pre-defined grid and count
%3 - Voila

 
%making list of files here

modisdatapath='DATA/MODIS/MOD44W/';
%listing all dates and looping through those


    LSMfilename=sprintf('DATA/MODIS/MOD44W/PROCESSED/MOD44W_RES=%s.mat',resstr);
    %here cycling through the whole grid
    LM=x*0;
    SM=x*0;
    
%     LAIERRlayer=x*0;
%     LAISTDlayer=repmat(x*0,[1,1,3]);
    
    
    countlayer=x*0;
    
    if isempty(dir(LSMfilename));
    
    for h=0:36
        for v=0:18
            filename=sprintf('%sMOD44*h%02iv%02i*hdf',modisdatapath,h,v);
            fullfile=dir(filename);
            isthere=length(fullfile);
            fullfilename=sprintf('%s%s',modisdatapath,fullfile.name);
            if isthere==1
            disp(sprintf('%s \t v= %i \t h= %i',fullfilename,v,h))
            %loading data =FPAR would be loaded here too (if needed)

            [LSM_1km,LSM_1km_QA]=datascript_read_modis_land_water_file(fullfilename);
            %loading coordinates
            %Transform to cartesian internally
            [xmod,ymod]=modis2cartesian(h,v,1,0.25); 
            %mod-correct xmod w.r.t. bottom-left corner
            xmod=mod(xmod-lonbl,360)+lonbl;

            %0 = land; 1 = water
            ptsL=find(LSM_1km==0);
            ptsS=find(LSM_1km==1);
            
            %now need to determine closest of each point. 
            pL=closest2d(xmod(ptsL),ymod(ptsL),x,y,0);
            pS=closest2d(xmod(ptsS),ymod(ptsS),x,y,0);
            
            upL=unique(pL);
            upS=unique(pS);
            
            for m=1:length(upL);LM(upL(m))=LM(upL(m))+sum(pL==upL(m));end
            for m=1:length(upS);SM(upS(m))=SM(upS(m))+sum(pS==upS(m));end
            
            
            end
        end
    end
   
    
LSM=LM./(LM+SM);
    
    %loop through each point where count > 3 

    save(LSMfilename,'LSM');
    else
        
        load(LSMfilename,'LSM');
end
    



end

function [LSM_1km,LSM_1km_QA]=datascript_read_modis_land_water_file(filename)

defval('filename', 'DATA/MODIS/MOD44W/MOD44W.A2000055.h00v08.005.2009212172814.hdf')

%infocrap=hdfinfo(filename);
 

%hi.Vgroup.Vgroup(1)

%'Data Fields'

%1200 x 1200 values


% 
% 
% 255 = _Fillvalue, assigned when:
%     * the MODAGAGG suf. reflectance for channel VIS, NIR was assigned its _Fillvalue, or
%     * land cover pixel itself was assigned _Fillvalus 255 or 254.
% 254 = land cover assigned as perennial salt or inland fresh water.
% 253 = land cover assigned as barren, sparse vegetation (rock, tundra, desert.)
% 252 = land cover assigned as perennial snow, ice.
% 251 = land cover assigned as "permanent" wetlands/inundated marshlands.
% 250 = land cover assigned as urban/built-up.
% 249 = land cover assigned as "unclassified" or not able to determine.
% 248 = no standard deviation available, pixel produced using backup method.


%to display all the instructions just do this:
%infocrap.Attributes.Value;

LSM_1km = hdfread(filename, 'MOD44W_250m_GRID', 'Fields', 'water_mask');
LSM_1km_QA = hdfread(filename, 'MOD44W_250m_GRID', 'Fields', 'water_mask_QA');





end






