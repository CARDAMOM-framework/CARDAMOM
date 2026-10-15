#pragma once
int EDC1_1111(double const *pars, DATA DATA, struct EDCDIAGNOSTIC *EDCD)
{

struct DALEC_1111_PARAMETERS P=DALEC_1111_PARAMETERS;

double meantemp = (DATA.ncdf_data.T2M_MAX.reference_mean + DATA.ncdf_data.T2M_MIN.reference_mean)/2;
double meanrad  = DATA.ncdf_data.SSRD.reference_mean;

EDCD->nedc=100;
int n; for (n=0;n<EDCD->nedc;n++){EDCD->PASSFAIL[n]=1;}
int EDC=1;
int DIAG=EDCD->DIAG;

/* Derived allocation fractions */
double const fauto=pars[P.f_auto];
double const ffol=(1-fauto)*pars[P.f_foliar];
double const flab=(1-fauto-ffol)*pars[P.f_lab];
double const froot=(1-fauto-ffol-flab)*pars[P.f_root];
double const fwood=1-fauto-ffol-flab-froot;
double const fsom=fwood+(froot+flab+ffol)*pars[P.tr_lit2soil]/(pars[P.tr_lit2soil]+pars[P.t_lit]);

double torfol=1/(pars[P.t_foliar]*365.25);

/* EDC 1: TOR_LIT faster than TOR_SOM */
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[1-1]==1)) & (pars[P.t_soil]>pars[P.t_lit])){EDC=0;EDCD->PASSFAIL[1-1]=0;}

/* EDC 2: Litter2SOM > SOM to Atm rate */
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[2-1]==1)) & (pars[P.tr_lit2soil]<pars[P.t_soil])){EDC=0;EDCD->PASSFAIL[2-1]=0;}

/* EDC 3: TOR_FOL faster than TOR_WOOD */
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[3-1]==1)) & (pars[P.t_wood]>torfol)){EDC=0;EDCD->PASSFAIL[3-1]=0;}

/* EDC 4: Root turnover > SOM turnover at meantemp */
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[4-1]==1)) & (pars[P.t_root]<pars[P.t_soil]*pow(pars[P.Q10rhco2],meantemp/10))){EDC=0;EDCD->PASSFAIL[4-1]=0;}

/* EDC 5: Allocation to canopy comparable to fine roots */
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[5-1]==1)) & ((ffol+flab)>5*froot | (ffol+flab)*5<froot)){EDC=0;EDCD->PASSFAIL[5-1]=0;}

/* EDC 16: Foliage CF > wood CF & Soil CF */
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[16-1]==1)) & (pars[P.cf_foliar]<pars[P.cf_ligneous] | pars[P.cf_foliar]<pars[P.cf_DOM])){EDC=0;EDCD->PASSFAIL[16-1]=0;}

/* EDC PyC-1: labile PyC turnover faster than refractory */
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[20-1]==1)) & (pars[P.k_PyC_L]<pars[P.k_PyC_R])){EDC=0;EDCD->PASSFAIL[20-1]=0;}

EDCD->pEDC=log((double)EDC);
return EDC;
}
