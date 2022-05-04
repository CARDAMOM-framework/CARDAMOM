

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********
typedef struct {
    int big_par_index;
    int small_par_index;}DALEC_EDC_TRAJECTORY_STRUCT;


//General inequality function
double DALEC_EDC_TRAJECTORY(DATA * DATA, void * EDCstruct){
    
 
 double PEDC = 0;
    
 
 
 
/*deriving mean pools here!*/
double MPOOLS;
nopools=DATA->


for (n=0;n<noprogpools;n++){MPOOLS[n]=mean_pool(POOLS,n,N_timesteps+1,nopools);};



/*deriving mean January pools*/
/*Assuming COMPLETE years*/
double *MPOOLSjan;
/*pool interval*/


int dint=(int)floor(N_timesteps/(TIME_INDEX[N_timesteps-1]-TIME_INDEX[0])*365.25);
/*declaring mean pool array*/
MPOOLSjan=calloc(nopools,sizeof(double));if (MPOOLSjan==0){printf("WARNING NULL POINTER");}
/*deriving mean jan pools*/
/*based on all jan pools except initial conditions*/
for (n=0;n<noprogpools;n++){
for (m=0;m<(N_timesteps/dint+1);m++){
MPOOLSjan[n]=MPOOLSjan[n]+POOLS[nopools*(m*dint)+n]/(N_timesteps/dint+1);}}
/*printing just to make sure*/
/*for (n=0;n<nopools;n++){printf("Pool = %d, janmnean=%2.2f\n",n,MPOOLSjan[n]);}*/


/***********************EDCs start here****************************/


/*EDC no 6*/
/*0.2*Cf < Cr < 5*Cf*/
/*Cfoliar : Croot = 5:1 or 1:5*/
/*if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[6-1]==1)) & (MPOOLS[S.C_fol]>MPOOLS[S.C_roo]*5 | MPOOLS[S.C_fol]*5<MPOOLS[S.C_roo])){EDC=ipow(0,EDCD->SWITCH[6-1]);EDCD->PASSFAIL[6-1]=0; }*/
EDCD->pEDC=EDCD->pEDC-0.5*pow(log(MPOOLS[S.C_fol]/MPOOLS[S.C_roo])/log(2),2);

/*equilibrium factor (in comparison to C_initial)*/
//double EQF=EDCD->EQF;
double EQF=DATA.ncdf_data.EDC_EQF;

/*Total fluxes*/
double *FT;
FT=calloc(nofluxes,sizeof(double));
int f=0;
for (f=0;f<nofluxes;f++){FT[f]=0;for (n=0;n<N_timesteps;n++){FT[f]+=FLUXES[n*nofluxes+f];}}
/*Total prec*/
double TOTAL_PREC=0;
double TOTAL_SNOW=0;
for (n=0;n<N_timesteps;n++){TOTAL_PREC+=PREC[n];TOTAL_SNOW+=SNOWFALL[n];}


double Fin[10];
double Fout[10];
double Pstart;
double Pend;
/*temporary print switch*/
int psw=0;
/*exponential decay tolerance*/
double etol=0.1;





/*Inputs and outputs for each pool*/
/*labile*/
Fin[S.C_lab]=FT[F.lab_prod];
Fout[S.C_lab]=FT[F.lab_release]+FT[F.f_lab]+FT[F.fx_lab2lit];
/*foliar*/
Fin[S.C_fol]=FT[F.lab_release];
Fout[S.C_fol]=FT[F.fol2lit]+FT[F.f_fol]+FT[F.fx_fol2lit];
/*root*/
Fin[S.C_roo]=FT[F.root_prod];
Fout[S.C_roo]=FT[F.root2lit]+FT[F.f_roo]+FT[F.fx_roo2lit];
/*wood*/
Fin[S.C_woo]=FT[F.wood_prod];
Fout[S.C_woo]=FT[F.wood2cwd]+FT[F.f_woo]+FT[F.fx_woo2cwd];
/*CWD*/
Fin[S.C_cwd]=FT[F.wood2cwd]+FT[F.fx_woo2cwd];
Fout[S.C_cwd]=FT[F.ae_rh_cwd]+FT[F.an_rh_cwd]+FT[F.cwd2som]+FT[F.f_cwd]+FT[F.fx_cwd2som];
/*litter*/
Fin[S.C_lit]=FT[F.fol2lit]+FT[F.root2lit]+FT[F.fx_lab2lit]+FT[F.fx_fol2lit]+FT[F.fx_roo2lit];
Fout[S.C_lit]=FT[F.ae_rh_lit]+FT[F.an_rh_lit]+FT[F.lit2som]+FT[F.f_lit]+FT[F.fx_lit2som];
/*som*/
Fin[S.C_som]=FT[F.cwd2som]+FT[F.lit2som]+FT[F.fx_cwd2som]+FT[F.fx_lit2som];
Fout[S.C_som]=FT[F.ae_rh_som]+FT[F.an_rh_som]+FT[F.f_som];
/*PAH2O*/
Fin[S.H2O_PAW]=TOTAL_PREC-FT[F.q_surf]-TOTAL_SNOW+FT[F.melt];
Fout[S.H2O_PAW]=FT[F.et]+FT[F.q_paw]+FT[F.paw2puw];
/*PUH2O*/
Fin[S.H2O_PUW]=FT[F.paw2puw];
Fout[S.H2O_PUW]=FT[F.q_puw];
/*SWE*/
Fin[S.H2O_SWE]=TOTAL_SNOW;
Fout[S.H2O_SWE]=FT[F.melt];



/*Inlcuding H2O pool*/
/*EDCs 7-13 - inputs, outputs and exponential tolerance*/

/*mean input/output ratio and start ratio*/
double Rm, Rs;



for (n=0;n<noprogpools;n++){
/*start and end pools*/
Pstart=POOLS[n];
Pend=POOLS[nopools*N_timesteps+n];
/*mean input/output*/
Rm=Fin[n]/Fout[n];
/*Theoretical starting input/output*/
Rs=Rm*MPOOLSjan[n]/Pstart;

/*if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[7-1+n]==1))
& ((fabs(log(Rs))>log(EQF)) || (fabs(Rs-Rm)>etol)))
{EDC=ipow(0,EDCD->SWITCH[7-1+n]);EDCD->PASSFAIL[7-1+n]=0;}*/
if (Fin[n] > 0){
EDCD->pEDC=EDCD->pEDC-0.5*pow(log(Rs)/log(EQF),2) - 0.5 *pow((Rs-Rm)/etol,2);

/*storing EDCPROB: i.e. the log probability of each EDC based on a gaussian representation*/
/*of each constraint*/
EDCD->EDCPROB[7-1+n]=-0.5*pow(log(Rs)/log(EQF),2);/*-0.5*pow((Rs-Rm)/etol,2);*/
}

 
 
 
 
 
 
 
    
    //Reading by casting void pointer to "DALEC_EDC_PARS_INEQUALITY_STRUCT" format
   DALEC_EDC_PARS_INEQUALITY_STRUCT  E = *(DALEC_EDC_PARS_INEQUALITY_STRUCT * ) EDCstruct;
    //Checking inequality
    if (DATA->M_PARS[E.big_par_index]<DATA->M_PARS[E.small_par_index]){PEDC = -INFINITY;} ;
    
    return PEDC;
}



