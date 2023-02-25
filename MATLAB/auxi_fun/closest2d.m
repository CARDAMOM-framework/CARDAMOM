function [pti,ri,ci]=closest2d(xp,yp,x,y,irregular)

%FUNCTION [P,R,C]=CLOSEST2D(XP,YP,X,Y,IRREGULAR)
%
%IN: 
%Xpoints, Ypoints: coordinate(s) of point(s) to be matched to nearest gridcell(s) 
%
%Xgrid, Ygrid: X-Y grid coordinates
%
%irregular: Set to 0 for regular X-Y grid, set to 1 otherwise (default = 1)
%
%OUT:
%
%[P,R,C]= Points, rows and columns of closest gridcells to xp and yp
%
%
%if the grid is regular then a fast method has been devised
%to perform fast method set 'irregular=0'
%assumes flat (cartesian) geometry
%
%Last Modified by A.A.Bloom 01/06/2010


ri=xp*0;
ci=xp*0;
defval('irregular',1)

switch irregular 
case 0
%ASSUMING CONSTANT INTERVALS
minx=x(1,1);
maxx=x(end,end);
miny=y(1,1);
maxy=y(end,end);

%In case of edge deformations (eg gcf grid) we sample middle of grid
inty=(y(2,1)-y(1,1));
intx=(x(1,2)-x(1,1));

ox=round(minx/intx)*intx-minx;
oy=round(miny/inty)*inty-miny;

ci=(round((ox+xp)/(intx))*intx-ox-minx)/intx+1;
ri=(round((oy+yp)/(inty))*inty-oy-miny)/inty+1;
%number of rows
nrows=size(x,1);

%reducing any outliers (with rows and columns less or more than limits)
%down to limits

ri(ri<1)=1;
ci(ci<1)=1;
ri(ri>size(x,1))=size(x,1);
ci(ci>size(x,2))=size(x,2);




pti=(ci-1)*nrows+ri;






case 1
   p=xp*0;
   for n=1:length(p)
      d=(x-xp(n)).^2+(y-yp(n)).^2;%no need for square root
      [r,c]=find(d==min(d(1:end)));
      %rounding up
      ri(n)=r(end);
      ci(n)=c(end);
      if mod(n,10000)==0; disp([num2str(n/length(p)*100),'%']);end
   end
   nrows=size(x,1);
   pti=(ci-1)*nrows+ri;
   

   
case 2
   p=xp*0;
   for n=1:length(p)
      d=(x-xp(n)).^2+(y-yp(n)).^2;%no need for square root
      p(n)=find(d==min(d(1:end)));
      %rounding up

   end
   pti=p;
   ri=[];
   ci=[];
   
   
    case 3 % for vectors only
         p=xp*0;
         i=1:length(x);
   for n=1:length(p)
      d=(x-xp(n)).^2+(y-yp(n)).^2;%no need for square root
      pti=i(d==min(d));
      %rounding up
      if mod(n,10000)==0; disp([num2str(n/length(p)*100),'%']);end
   end
   ri=[];
   ci=[];
      
   

end

%Introducing "round" to avoid numerical error (errors are 1e-9)
pti=round(pti);
ri=round(ri);
ci=round(ci);

end

