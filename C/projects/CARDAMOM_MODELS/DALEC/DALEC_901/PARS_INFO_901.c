#pragma once
/*PARAMETER_INFO (typedef struct) must have at least 3 fields
 *  * npars,
 *   * parmax
 *    * parmin*/
/*where is it defined?*/
/*For DALEC_FIRES: as all GPP allocation fractions are inter-dependent (sum = 1)*/
/*MCMC sampling of GPP allocation priors approximated as 0.01-0.5 NPP for*/
/*photosynthetic pools and 0.01-1 of remaining NPP for root and wood pool*/

int PARS_INFO_901(DATA *CARDADATA)
{

// Decomposition rate
CARDADATA->parmin[0]=0.00001;
CARDADATA->parmax[0]=0.01;

// Fraction of GPP respired
CARDADATA->parmin[1]=0.2;
CARDADATA->parmax[1]=0.8;

// Fraction of (1-fgpp) to foliage
CARDADATA->parmin[2]=0.01;
CARDADATA->parmax[2]=0.5;

// Fraction of (1-fgpp) to roots
CARDADATA->parmin[3]=0.01;
CARDADATA->parmax[3]=1.;

// TOR foliar
CARDADATA->parmin[4]=0.000001;
CARDADATA->parmax[4]=0.1; // changed 0.1 to 0.99 on 03/11/14

// TOR wood
CARDADATA->parmin[5]=0.000025;
CARDADATA->parmax[5]=0.001;

// TOR roots

CARDADATA->parmin[6]=0.0001;
CARDADATA->parmax[6]=0.01;

// TOR litter
CARDADATA->parmin[7]=0.0001;
CARDADATA->parmax[7]=0.01;

// TOR SOM
CARDADATA->parmin[8]=0.0000001;
CARDADATA->parmax[8]=0.001;

// Temp factor* = Q10 = 1.2-1.6
CARDADATA->parmin[9]=0.018;
CARDADATA->parmax[9]=0.08;

// Canopy Efficiency
CARDADATA->parmin[10]=5;
CARDADATA->parmax[10]=50;

// TORlabile
CARDADATA->parmin[11]=0.000001;
CARDADATA->parmax[11]=0.1; // changed 0.1 to 0.99 on 03/11/14

// Fraction to Clab
CARDADATA->parmin[12]=0.01;
CARDADATA->parmax[12]=0.5;

// Tmn,min for GSI
CARDADATA->parmin[13]=225.;
CARDADATA->parmax[13]=330.;

// Tmn,max for GSI
CARDADATA->parmin[14]=225.;
CARDADATA->parmax[14]=330.;

// Changed to min photoperiod, in sec
CARDADATA->parmin[15]=3600.; // 1h
CARDADATA->parmax[15]=82800.;// 23h

// LMA - Kattge et al. 2011
CARDADATA->parmin[16]=5;
CARDADATA->parmax[16]=200;

// INITIAL VALUES DECLARED HERE

// Clab
CARDADATA->parmin[17]=1.0;
CARDADATA->parmax[17]=2000.0;

// Cfol
CARDADATA->parmin[18]=1.0;
CARDADATA->parmax[18]=2000.0;

// Croots
CARDADATA->parmin[19]=1.0;
CARDADATA->parmax[19]=2000.0;

// Cwood
CARDADATA->parmin[20]=1.0;
CARDADATA->parmax[20]=100000.0;

// Clit
CARDADATA->parmin[21]=1.0;
CARDADATA->parmax[21]=2000.0;

// Csom
CARDADATA->parmin[22]=1.0;
CARDADATA->parmax[22]=200000.0;

// Max photoperiod for GSI
CARDADATA->parmin[23]=3600.; //6h
CARDADATA->parmax[23]=82800.; //18h

/*Foliar biomass CF*/
CARDADATA->parmin[24]=0.01;
CARDADATA->parmax[24]=1;

/*"Ligneous" biomass CF".*/
CARDADATA->parmin[25]=0.01;
CARDADATA->parmax[25]=1;

/*DOM CF".*/
CARDADATA->parmin[26]=0.01;
CARDADATA->parmax[26]=1;

return 0;

}
