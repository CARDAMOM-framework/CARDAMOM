#pragma once
int WRITE_DEMCMC_RESULTS(double *PARS,PARAMETER_INFO PI,MCMC_OPTIONS MCO){

int n;



FILE *fileout=fopen(MCO.outfile,"ab");
for (n=0;n<PI.npars*MCO.nchains;n++){
        fwrite(&PARS[n],1,sizeof(double),fileout);}
    
/*writing likelyhood*/
/*NOTE: As of July 11th 2014, probability no longer written to file*/
/*Probability is a "re-derivable" quantity, therefore if needed, it can
either (a) be re-derived using the MODEL_LIKELIHOOD function, or (b) 
written to a separate file.*/
/*fwrite(&PROB,1,sizeof(double),fileout);*/

fclose(fileout);


return 0;


}
