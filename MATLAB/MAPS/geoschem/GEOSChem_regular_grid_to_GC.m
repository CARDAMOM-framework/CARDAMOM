function FOUT=GEOSChem_regular_grid_to_GC(FIN,gcres)
%NOTE: replace NaNs with zeros unless NaNs need to be ignored in GC grid averaging
%INPUTS:
%FIN: 1x1 degree dataset (180 x 360 x N).
%gcres: either '2x2.5' or '4x5'
%Supports 
%Includes calls to 
%loadworldmesh
%GEOSChem_xygrids
%2x2.5 is now obsolete
defval('gcres','4x5')


if total(isfinite(FIN)==0)==0  & total(FIN<0)==0; sf=1;else sf=0;end

if strcmp(gcres,'4x5');FOUT=regrid_4x5(FIN,sf);
elseif strcmp(gcres,'2x2.5');FOUT=regrid_2x25(FIN,sf);end

if total(FIN<0)>0; disp('Note: not rescaling field since input is not positive definite');end


end


function FOUT=regrid_4x5(FIN,sf)

silent=1;
if silent==0
disp('Assuming input is 1x1 , 0.5x0.5 or 0.25x0.25');
disp('(Update code here otherwise)');
end
GC=GEOSChem_xygrids('4x5');
Ag=GC.area;

FOUT=repmat(Ag,[1,1,size(FIN,3),size(FIN,4)]);
%assuming input is regular global cartesian grid. n x (180x360)

res=180/size(FIN,1);
[x,y,A]=loadworldmesh(res);
     
for nn=1:size(FIN,4);
    for n=1:size(FIN,3);
             fch4=FIN(:,:,n,nn);
                 %currently first pixel edge is at 180W, move to 177.5E.
if res==1;
    %move left-edge from 180W to 179.5E (1/2 rotation eastwards).
    fch4=fch4/2+fch4(:,[end,1:end-1])/2;
    %now rotate by two eastwards to place left-edge at 177.5
    fch4=fch4(:,[end-1:end,1:end-2]);   
%end

elseif res==0.5
        %now rotate by five eastwards to move from 180W left-edge at 177.5E
    fch4=fch4(:,[end-4:end,1:end-5]);   
elseif res==0.25
        %now rotate by ten eastwards to move from 180W left-edge at 177.5E
    fch4=fch4(:,[end-9:end,1:end-10]);   
elseif res==0.1
            %now rotate by twenty five eastwards to move from 180W left-edge at 177.5E
    fch4=fch4(:,[end-24:end,1:end-25]);   
end

%griding at GC 4x5 resolution
    fch4_gc=GC.x*0;
    count=GC.x*0;
    
    nc=5/res;
    nr=4/res;
for c=1:nc; 
    for r=1:nr;
        %ignore first two rows (belong to -90) and last two rows (belong to
        %+90)
        fch4_gc(2:end-1,:)=fch4_gc(2:end-1,:)+nan2zero( fch4(r+nr/2:nr:end-nr/2,c:nc:end).*A(r+nr/2:nr:end-nr/2,c:nc:end));
        count(2:end-1,:)=count(2:end-1,:)+A(r+nr/2:nr:end-nr/2,c:nc:end).*isfinite( fch4(r+nr/2:nr:end-nr/2,c:nc:end));
    end
end

        %Normalizing by "count"
        fch4_gc(2:end-1,:)=fch4_gc(2:end-1,:)./count(2:end-1,:);
        
        %scale factor (should be close to 1)
        %Only apply if non nans for conservation of quantity
        %Also: only apply to positive-definite values
        if sf==1
        SF=nantotal(FIN(:,:,n,nn).*A)/nantotal(fch4_gc.*Ag);
        else
            SF=1;
        end

        FOUT(:,:,n,nn)=fch4_gc*SF;
    end
end



end



function FOUT=regrid_2x25(FIN,sf)

silent=1;
if silent==0
disp('Assuming input is  0.5x0.5 or 0.25x0.25');
disp('(Update code here otherwise)');
end
GC=GEOSChem_xygrids('2x2.5');
Ag=GC.area;

FOUT=repmat(Ag,[1,1,size(FIN,3),size(FIN,4)]);
%assuming input is regular global cartesian grid. n x (180x360)

res=180/size(FIN,1);
[x,y,A]=loadworldmesh(res);
     
for nn=1:size(FIN,4);
    for n=1:size(FIN,3);
             fch4=FIN(:,:,n,nn);
                 %currently first pixel edge is at 180W, move to 178.7500E
if res==0.5
    %move left-edge from 180W to 179.75E (1/2 rotation eastwards).
    fch4=fch4/2+fch4(:,[end,1:end-1])/2;
    %now rotate by two eastwards to place left-edge at 178.7500E
    fch4=fch4(:,[end-1:end,1:end-2]);   
    %end
elseif res==0.25
    %rotate by five eastwards to move from 180W left-edge at 178.7500E        
    fch4=fch4(:,[end-4:end,1:end-5]);   
end

%griding at GC 2.25 resolution
    fch4_gc=GC.x*0;
    count=GC.x*0;
    
    
    %CONTINUE FROM HERE**********
    nc=2.5/res;
    nr=2/res;
for c=1:nc; 
    for r=1:nr;
        %ignore first two rows (belong to -90) and last two rows (belong to
        %+90)
        fch4_gc(2:end-1,:)=fch4_gc(2:end-1,:)+nan2zero( fch4(r+nr/2:nr:end-nr/2,c:nc:end).*A(r+nr/2:nr:end-nr/2,c:nc:end));
        count(2:end-1,:)=count(2:end-1,:)+A(r+nr/2:nr:end-nr/2,c:nc:end).*isfinite( fch4(r+nr/2:nr:end-nr/2,c:nc:end));
    end
end

        %Normalizing by "count"
        fch4_gc(2:end-1,:)=fch4_gc(2:end-1,:)./count(2:end-1,:);
        
        %scale factor (should be close to 1)
        %Only apply if non nans for conservation of quantity
        if sf==1
        SF=nantotal(FIN(:,:,n,nn).*A)/nantotal(fch4_gc.*Ag);
        else
            SF=1;
        end

        FOUT(:,:,n,nn)=fch4_gc*SF;
    end
end



end