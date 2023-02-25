#pragma once
int EDC1_1025(double const *pars, DATA DATA, struct EDCDIAGNOSTIC *EDCD)
{

/*EDCD=EDCD2;*/
/*List of EDCs in this file*/
/*1.  TOR_LIT faster than TOR_SOM
2. Litter2SOM greater than SOM to Atm. rate
3. TOR_FOL faster than TOR_WOOD
4. Root turnover greater than SOM turnover at meantemp
5. Allocation to canopy is comparable to allocation to fine roots
6-15. Taken up by EDC2
16. Foliage CF> wood CF & Foliage CF > Soil CF 
*/

struct DALEC_1025_PARAMETERS P=DALEC_1025_PARAMETERS;

double meantemp = (DATA.ncdf_data.T2M_MAX.reference_mean + DATA.ncdf_data.T2M_MIN.reference_mean)/2;
double meanrad = DATA.ncdf_data.SSRD.reference_mean;


/*This function was created on 7 Jan 2014*/
/*Rules here are specified as in Bloom et al., 2014 paper*/
/*EDC1 contains all checks that do not require a full DALEC_CDEA (DALEC2) model run*/


/*ALL EDCs set as 1 initially*/
EDCD->nedc=100;
int n; for (n=0;n<EDCD->nedc;n++){EDCD->PASSFAIL[n]=1;}
/*declaring variables and constants for subsequent EDCs*/
int EDC=1;
int DIAG=EDCD->DIAG;
/*obsolete edcc constant was kept here*/
/*deriving true allocation fractions*/
double const fauto=pars[P.f_auto];
// double const ffol=(1-fauto)*pars[P.f_foliar];  // Removed this as there is no longer a direct flux from NPP -> Cfol, it all goes via Clab
double const flab=(1-fauto)*pars[P.f_lab];
double const froot=(1-fauto-flab)*pars[P.f_root];
double const fwood=1-fauto-flab-froot;
/*fraction of GPP som under equilibrium conditions*/
double const fsom=fwood+(froot+flab)*pars[P.tr_lit2soil]/(pars[P.tr_lit2soil]+pars[P.t_lit]);

/*yearly leaf loss fraction*/
// double torfol=1/(pars[P.t_foliar]*365.25);



/*EDC CHECK NO 1*/
/*TOR_LIT faster than TOR_SOM*/
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[1-1]==1)) & (pars[P.t_soil]>pars[P.t_lit])){EDC=0;EDCD->PASSFAIL[1-1]=0;}

/*EDC CHECK NO 2*/
/*Litter2SOM greater than SOM to Atm. rate*/
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[2-1]==1)) & (pars[P.tr_lit2soil]<pars[P.t_soil])){EDC=0;EDCD->PASSFAIL[2-1]=0;}

/*EDC CHECK NO 3*/
/*TOR_FOL faster than TOR_WOOD */
// if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[3-1]==1)) & (pars[P.t_wood]>torfol)){EDC=0;EDCD->PASSFAIL[3-1]=0;}

/*EDC CHECK NO 4*/
/*Root turnover greater than SOM turnover at meantemp*/
/*same as this*/
/*\text{EDC 4: }(1-\pavii)^{365} > \Pi_{i=1}^{365} (1-\paix \tratei)*/
// if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[4-1]==1)) & (pars[P.t_root]<pars[P.t_soil]*exp(pars[P.temp_factor]*meantemp))){EDC=0;EDCD->PASSFAIL[4-1]=0;}

/*EDC no 5 is addressed in EDC2_FIREBUCKET.c*/

/*EDC CHECK NO 6*/
/*Bday Fday difference>45 days */
/*if (((EDC==1 & DIAG==0) || DIAG==1) & (fabs(pars[P.Fday] - pars[P.Bday])<45 | fabs(pars[P.Fday]-pars[P.Bday])>320.25)){EDC=0;EDCD->PASSFAIL[6-1]=0;}*/
/*now obsolete!*/

/*EDC CHECK NO 5*/
/*Allocation to canopy is comparable to allocation to fine roots*/
// if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[5-1]==1)) & ((flab)>5*froot | (flab)*5<froot)){EDC=0;EDCD->PASSFAIL[5-1]=0;}

/*EDC No 16* Foliage CF> wood CF & SOM CF*/
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[16-1]==1)) & (pars[P.cf_foliar]<pars[P.cf_ligneous] | pars[P.cf_foliar]<pars[P.cf_DOM])){EDC=0;EDCD->PASSFAIL[16-1]=0;}

/*EDC No 17: RD_puw > RD_paw*/
// if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[17-1]==1)) & (pars[P.PAW_Qmax]<pars[P.P])){EDC=0;EDCD->PASSFAIL[17-1]=0;}



/*Add any generalisations derivable from EDC2 (post-run checks) here*/
/*Note: these must be tested to ensure that DALEC2 run is NOT needed */


EDCD->pEDC=log((double)EDC);



return EDC;


}






