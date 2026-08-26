
#pragma once
#include "../../../COST_FUNCTION/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF.c"
//#include "../../../COST_FUNCTION/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF_beta.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_119.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/DEMCMC.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/ADEMCMC.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/DEMCMCZS_WARMUP.c"

int FIND_EDC_INITIAL_VALUES(DATA CARDADATA,PARAMETER_INFO *PI, MCMC_OPTIONS *MCOPT_CARDAMOM){

    printf("*********made it to here FIND_EDC_INITIAL_VALUES********\n");

/*This search finds parameter vectors that pass the EDC likelihood before the
 *main CARDAMOM MCMC starts. The search uses CARDADATA.EMLF; the main sampler
 *continues to use CARDADATA.MLF after this function returns.*/

MCMC_OPTIONS MCOPT;
MCMC_OUTPUT MCOUT;


int PEDCC,nn;

/*For multi-chain EDC searches, require more than this many chains to pass
 *EDCs before ending the search. Because the check is PEDCC>nstartchains,
 *the default value of 10 requires at least 11 passing chains.*/
int nstartchains=10;
int multichain_edc_search=(MCOPT_CARDAMOM->mcmcid==3 || MCOPT_CARDAMOM->mcmcid==4 || MCOPT_CARDAMOM->mcmcid==5 || MCOPT_CARDAMOM->mcmcid==6 || MCOPT_CARDAMOM->mcmcid==7 || MCOPT_CARDAMOM->mcmcid==8 || MCOPT_CARDAMOM->mcmcid==9 || MCOPT_CARDAMOM->mcmcid==10);
        
        
MCOPT.APPEND=0;
MCOPT.nADAPT=10;
MCOPT.fADAPT=0.5;
MCOPT.nOUT=1000;
MCOPT.nPRINT=100;
MCOPT.nWRITE=0;
MCOPT.nSTART=0;
MCOPT.randparini=1;
MCOPT.returnpars=1;
MCOPT.fixedpars=1;
MCOPT.mcmcid=119;/*Using metropolis-hastings to find initial parameters*/
MCOPT.nchains=1;
MCOPT.minstepsize=1e-2;


/*Modes 3, 4, 5, 6, 7, 8, 9, and 10 must enter the exact same EDC search: 400-chain
 *ADEMCMC search options, identical pass threshold, identical sampler call,
 *and the same 400-chain PI->parini allocation path. Mode-specific handoff
 *only happens after the shared search completes.*/
if (multichain_edc_search){
MCOPT.mcmcid=3;
default_int_value(&CARDADATA.ncdf_data.MCMCID.nSAMPLES_EDC_SEARCH ,200000);
MCOPT.nOUT=CARDADATA.ncdf_data.MCMCID.nSAMPLES_EDC_SEARCH ;
MCOPT.nPRINT=2000;
MCOPT.minstepsize=1e-5;
MCOPT.nchains=400;
MCOPT.fixedpars=0;
MCOPT.fADAPT=0;
/*Rebuild PI->parini to the search size for every multi-chain mode. This keeps
 *modes 3/4/5/6/7/8/9/10 identical before the first EDC-search random draw.*/
free(PI->parini);
PI->parini=calloc(MCOPT.nchains*PI->npars,sizeof(double));}



int OK=INITIALIZE_MCMC_OUTPUT(*PI,&MCOUT,MCOPT);
printf("C/projects/CARDAMOM_MDF/MCMC_SETUP/PROJECT_FUN/FIND_EDC_INITIAL_VALUES.c: MCOUT structure initialized\n");



int n,nnn;

printf("PI->npars (INSIDE FIND_EDC_INITIAL_VALUES.c)= %d\n",PI->npars);




for (n=0;n<PI->npars*MCOPT.nchains;n++){PI->parini[n]=DEFAULT_DOUBLE_VAL;}
for (n=0;n<PI->npars;n++){
PI->stepsize[n]=0.02;
PI->parfix[n]=0;}


double PEDC=log(0);
int count=0;
while (PEDC!=0){
	printf("EDC Attempt no %d\n",count);oksofar("---");

	for (n=0;n<PI->npars;n++){PI->stepsize[n]=0.0005;}

	oksofar("Running short MCMC to find x_{EDC} = 1");
    
	if (MCOPT.mcmcid==119){MHMCMC_119(CARDADATA.EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};
        if (MCOPT.mcmcid==2){DEMCMC(CARDADATA.EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};
        if (multichain_edc_search){ADEMCMC(CARDADATA.EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};

	oksofar("Short MCMC complete");
	for (n=0;n<PI->npars*MCOPT.nchains;n++){PI->parini[n]=MCOUT.best_pars[n];}

	PEDCC=0;
	for (nn=0;nn<MCOPT.nchains;nn++){
	PEDC=CARDADATA.EMLF(CARDADATA, PI->parini + nn*PI->npars);double P;
	P=CARDADATA.MLF(CARDADATA, PI->parini + nn*PI->npars);
	printf("PEDC for chain %i = %2.1f (%2.1f)\n",nn,PEDC,P);
	if (PEDC==0){PEDCC=PEDCC+1;}}

	
	printf("*******\n");
	printf("*******\n");
	printf("%i out of %i chains pass EDCs\n",PEDCC,MCOPT.nchains);
    printf("EDC stats\n");
    for (nnn=0;nnn<CARDADATA.noedcs;nnn++){
        double prcnt=100*(double)CARDADATA.EDC_PASS_COUNTER[nnn]/(double)CARDADATA.EDC_INSTANCE_COUNTER[nnn];
printf("EDC no %i; attempts = %i; passes = %i (%2.2f%%);\n",nnn,CARDADATA.EDC_INSTANCE_COUNTER[nnn],CARDADATA.EDC_PASS_COUNTER[nnn],prcnt );
    }
	printf("*******\n");
	printf("*******\n");

	
	count=count+1;
	
	if (MCOPT.mcmcid==2 && PEDCC>=MCOPT.nchains){PEDC=0;}
	if (multichain_edc_search){if (PEDCC>nstartchains){PEDC=0;}else{PEDC=-1;}}
	if (MCOPT.mcmcid==2 || multichain_edc_search){MCOPT.randparini=0;}
	
	/*For single-chain MH search, periodically restart from priors if no
	 *EDC-passing point has been found.*/
	if (MCOPT.mcmcid==119 && PEDC!=0 && count%3==0){for (n=0;n<PI->npars;n++){PI->parini[n]=CARDADATA.parpriors[n];}}

}

/*mode 5: pick the best MCOPT_CARDAMOM->nchains (production chain count) chains
 *out of the 400 searched, ranked by real likelihood among those passing EDCs,
 *then shrink PI->parini back down to that size - the production DEMCMCZS run
 *only ever sees this final, small, EDC-satisfying set of starting points.*/
if (MCOPT_CARDAMOM->mcmcid==5){
int nprod=MCOPT_CARDAMOM->nchains;
double *chainP=calloc(MCOPT.nchains,sizeof(double));
int nn2;
for (nn2=0;nn2<MCOPT.nchains;nn2++){
double edc_p=CARDADATA.EMLF(CARDADATA, PI->parini + nn2*PI->npars);
if (edc_p==0){chainP[nn2]=CARDADATA.MLF(CARDADATA, PI->parini + nn2*PI->npars);}
else{chainP[nn2]=log(0);}
}

double *parini_small=calloc(nprod*PI->npars,sizeof(double));
int k,best_idx;
for (k=0;k<nprod;k++){
best_idx=0;
double best_val=chainP[0];
for (nn2=1;nn2<MCOPT.nchains;nn2++){
if (chainP[nn2]>best_val){best_val=chainP[nn2];best_idx=nn2;}}
for (n=0;n<PI->npars;n++){parini_small[k*PI->npars+n]=PI->parini[best_idx*PI->npars+n];}
chainP[best_idx]=log(0);
}

free(chainP);
free(PI->parini);
PI->parini=parini_small;
MCOPT.nchains=nprod;
}else{
/*For modes other than mode 5, keep the full EDC-search ensemble. Mode 6 needs
 *all 400 chains for the warmup/merge block below; modes 3, 4, 7, 8, 9, and 10 hand the
 *same 400 chains directly to their production samplers.*/
for (n=0;n<PI->npars*MCOPT.nchains;n++){

	PI->parini[n]=MCOUT.best_pars[n];
}
}


/*mode 6: hybrid warmup step. PI->parini now holds all 400 EDC-search-endpoint
 *chains (filled by the generic "else" branch above, same as modes 3/4).
 *Rank them by real likelihood among EDC-passing chains, take the best 10,
 *optionally run those 10 through an intermediate DEMCMCZS phase, then write
 *the resulting evolved states back into their
 *original slots - leaving the other 390 EDC-search endpoints untouched.
 *MCOPT.nchains stays at 400: the full mixed ensemble (390 EDC endpoints +
 *10 DEMCMCZS-evolved) is what gets handed to AFDEMCMC for production.*/
if (MCOPT_CARDAMOM->mcmcid==6){
int nwarm=10;
int WARMUP_ITERS=0; /*0 disables the intermediate DEMCMCZS warmup.*/

double *chainP=calloc(MCOPT.nchains,sizeof(double));
int nn2;
for (nn2=0;nn2<MCOPT.nchains;nn2++){
double edc_p=CARDADATA.EMLF(CARDADATA, PI->parini + nn2*PI->npars);
if (edc_p==0){chainP[nn2]=CARDADATA.MLF(CARDADATA, PI->parini + nn2*PI->npars);}
else{chainP[nn2]=log(0);}
}

int *warm_idx=calloc(nwarm,sizeof(int));
int k,nn3;
for (k=0;k<nwarm;k++){
int bi=0; double best_val=chainP[0];
for (nn3=1;nn3<MCOPT.nchains;nn3++){
if (chainP[nn3]>best_val){best_val=chainP[nn3];bi=nn3;}}
warm_idx[k]=bi;
chainP[bi]=log(0);
}
free(chainP);

double *parini_warm=calloc(nwarm*PI->npars,sizeof(double));
for (k=0;k<nwarm;k++){
for (n=0;n<PI->npars;n++){parini_warm[k*PI->npars+n]=PI->parini[warm_idx[k]*PI->npars+n];}}

oksofar("mode 6: running intermediate DEMCMCZS warmup phase on the best 10 EDC-search chains");
RUN_DEMCMCZS_WARMUP(CARDADATA.MLF,CARDADATA,*PI,nwarm,WARMUP_ITERS,parini_warm);
oksofar("mode 6: DEMCMCZS warmup phase complete, merging back into the 400-chain ensemble");

for (k=0;k<nwarm;k++){
for (n=0;n<PI->npars;n++){PI->parini[warm_idx[k]*PI->npars+n]=parini_warm[k*PI->npars+n];}}

free(parini_warm);
free(warm_idx);
}


/*clearing MCOUT fields*/
free(MCOUT.best_pars);

return 0;}
