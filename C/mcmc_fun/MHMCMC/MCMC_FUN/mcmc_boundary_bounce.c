
#pragma once
/*Currently just matlab code, work on coverting to C*/

double mcmc_boundary_bounce(double vec0, double vstep){


/*TOT = dir + full_reflections x m+ residual
%
%Step 1. direct 
%Should amount to v (if vs is negative) or 1-v (if vs is positive)
*/

double vec=vec0+vstep, v;
/*Only bouncing if vec is outside bounds*/
if (vec>1 | vec<0){

/*sign of (non-zero) vector step*/
double svs=((double)(vstep>0))*2-1;

/*1-vec0 if svs=1*/
/*vec0 if svs=-1*/
/*ad = Direct distance to wall*/
double ad=(1+svs)/2 - svs*vec0;

/*%Step 2.*/
/*d = remaining distance to be covered*/
double d= fabs(vstep) -ad;
/*m = number of bounces after 1st bounce*/
double m = floor(d);
/*reflection distance*/
double ar = d - m;

/*Step 3. Determine whether step is 1-ar or ar, based on number of bounces
%
%Outcome 1: if m = 0,2,4,..., then step = 1-ar if vs = +ve, or ar if vs
%=-ve
%Outcome 2: if m = 1,3,5,..., then step = ar if vs = +ve or 1-ar if vs =
%-ve
*/
vec=(pow(-1,m)*svs+1)/2 - pow(-1,m)*svs*ar;
}

/*return either first version, or "bounced" version*/
return vec;

}


