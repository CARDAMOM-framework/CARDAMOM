#pragma once
#include "../../../math_fun/std.c"
#include "../../../math_fun/mean.c"
#include "../../../math_fun/max.c"
#include "DALEC_806_MFCF.c"
#include "DALEC_807_MFCF.c"
#include "DALEC_LIKELIHOOD_GRACE_EWT.c"
#include "DALEC_LIKELIHOOD_ET.c"
#include "DALEC_LIKELIHOOD_GPP.c"
#include "DALEC_LIKELIHOOD_LAI.c"
#include "DALEC_LIKELIHOOD_NEE.c"
#include "DALEC_LIKELIHOOD_CH4.c"
/*Any likelihood functions used in multiple MLF functions are kept here!*/



int EDCSETUP(DATA DATA, struct EDCDIAGNOSTIC ** EDCD){

/*TO DO HERE: set EDCD.EDCSWITCHES to DATA.OTHERPRIORS fields 30 to 50*/
/*DON'T FORGET TO DO THE SAME IN CDEA files*/
/*see if it is possible to initiate both using the same function*/
/*E.g. INITIATE_EDC_SWITCHES*/

static struct EDCDIAGNOSTIC EDCDmem;
EDCDmem.DIAG=DATA.EDCDIAG;
int n;
/*EDC switches are stored in DATA->parpriors (positions 31-50);*/
/*switch all on if EDCDIAG<2 , otherwise, switch on/off according to EDCSWITCH*/
/*EDCSETUP function will be included in DALEC_ALL_LIKELIHOOD.c*/
for (n=0;n<100;n++){EDCDmem.SWITCH[n]=1;}
if (EDCDmem.DIAG==2){for (n=0;n<20;n++){EDCDmem.SWITCH[n]=DATA.otherpriors[n+30];}}

/*EQF is stored in the "DATA.otherpriorunc" fields associated with EDCs 7-12*/
/*default value is 2*/
EDCDmem.EQF=DATA.otherpriorunc[36]; if (EDCDmem.EQF==-9999){EDCDmem.EQF=2;}

printf("EDCD->EQF = %2.2f\n",EDCDmem.EQF);

/*Default structure has all EDC=1, and 100 EDCs*/
EDCDmem.nedc=100;
for (n=0;n<EDCDmem.nedc;n++){EDCDmem.PASSFAIL[n]=1;}


/*Double pointer = contents of EDCD reassigned to static struct*/
*EDCD=&EDCDmem;


return 0;
}


double LIKELIHOOD_P(DATA DATA,double *PARS)
{
/*remember - LOG likelihood*/
double p=0,p_lma,pn;
int n;

/*looping through all priors for P*/
/*where no prior distribution is used, insert 9999*/
for (n=0;n<50;n++){if (DATA.parpriors[n]>-9999 & DATA.parpriorunc[n]!=-9999){
if (DATA.parpriorunc[n]>0){
/*log-normal if uncertainty value is positive*/
p=p-0.5*pow(log(PARS[n]/DATA.parpriors[n])/log(DATA.parpriorunc[n]),2);}
else {
/*log-normal if uncertainty value is positive*/
p=p-0.5*pow((PARS[n]-DATA.parpriors[n])/(DATA.parpriorunc[n]),2);}

}}

/*for any other priors, explicitly define functions based on values in DATA.otherpriors*/

/*total biomass (pools 1:4) defined here - using first space of pappriors for total biomass*/


double LIKELIHOOD(DATA D)
{
int n,dn,m,f;
double P=0, Ps;
double tot_exp;
double CPG;
double mfire=0;
double msif=0;
double mgpp=0;
double mnpdf=0;
double mlai=0;
//int npdfi[7]={9,10,11,23,24,25,26};


/*Mean biomass at time = 0 */
if (DATA.otherpriors[0]>-9999){
//p=p-0.5*pow(log((PARS[17]+PARS[18]+PARS[19]+PARS[20])/DATA.otherpriors[0])/log(DATA.otherpriorunc[0]),2);}
p=p-0.5*pow(log((DATA.M_BIOMASS_t0[0])/DATA.otherpriors[0])/log(DATA.otherpriorunc[0]),2);}

return p;}





P=P+DALEC_LIKELIHOOD_GPP(D);
P=P+DALEC_LIKELIHOOD_LAI(D);
P=P+DALEC_LIKELIHOOD_ET(D);
P=P+DALEC_LIKELIHOOD_NEE(D);
if (D.ID==1010){
	P=P+DALEC_LIKELIHOOD_CH4(D);
	}

	if (D.ID==1011){
	P=P+DALEC_LIKELIHOOD_CH4(D);
	}
/*shuang: 101010 was created for climate sensitivity test Nov2020*/

double mam=0,am=0;

P = P + DALEC_LIKELIHOOD_ABGB(D);
P = P + DALEC_LIKELIHOOD_SOIL(D);
P = P + DALEC_LIKELIHOOD_GRACE_EWT(D);


/*Note: only use with model ID = 806*/

if (D.ID==806){P = P + DALEC_806_MFCF(D);}
if (D.ID==807){P = P + DALEC_807_MFCF(D);}
if (D.ID==808){P = P + DALEC_807_MFCF(D);}



/*Constrain CMS disturbance fluxes*/
//if (D.otherpriors[6]>-9999){mnpdf=0;
/*Step 1. Sum of biomass -> litter emissions (including fire)*/
//for (n=0;n<D.nodays;n++){
//for (f=0;f<7;f++){mnpdf+=D.M_FLUXES[n*D.nofluxes+npdfi[f]];}}
/*Normalize npdf constraint to daily mean flux*/
//mnpdf=mnpdf/((double)D.nodays);
/*Step 2. Constrain against npdf*/
/*Only constrain if model flux < observed flux (otherwise mortality can explain internal cardamom fluxes)*/
//if (mnpdf<D.otherpriors[6]) {
//P=P-0.5*pow(log(mnpdf/D.otherpriors[6])/log(D.otherpriorunc[6]),2);
//}}






if (isnan(P)){P=log(0);}
return P;}

