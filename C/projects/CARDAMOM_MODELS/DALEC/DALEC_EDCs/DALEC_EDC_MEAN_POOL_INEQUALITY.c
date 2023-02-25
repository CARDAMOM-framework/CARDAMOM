
/*EDC no 6*/
/*0.2*Cf < Cr < 5*Cf*/
/*Cfoliar : Croot = 5:1 or 1:5*/
/*if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[6-1]==1)) & (MPOOLS[S.C_fol]>MPOOLS[S.C_roo]*5 | MPOOLS[S.C_fol]*5<MPOOLS[S.C_roo])){EDC=ipow(0,EDCD->SWITCH[6-1]);EDCD->PASSFAIL[6-1]=0; }*/



EDCD->pEDC=EDCD->pEDC-0.5*pow(log(MPOOLS[S.C_fol]/MPOOLS[S.C_roo])/log(2),2);