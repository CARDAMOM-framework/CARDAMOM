#pragma once
#include "./LAI_KNORR_funcs.c"
/* Leaf Phenology Model of Knorr et al. (2010, doi: 10.1029/2009JG001119)*/
double* LAI_KNORR(double const *met_list, double const *var_list)
{
  /*double gc=0,pp=0,qq=0,ci=0,e0=0,mult=0,dayl=0,cps=0,dec=0;*/
  //double gc,pp,qq,ci,e0,mult,dayl,cps,dec,GPP;
/*Notes: DALEC code passes the following *consts and *pars pointers:
 double constants[10]={pars[10],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};
pars terms:
pars[0]=LAI[n]
pars[1]=(DATA.MET[m+1] + DATA.MET[m+2]/2 ; mean air temperature
pars[2]=

constants terms:
consts[0] = T_Phi = 12.5        # parameter: temperature at leaf onset (degrees C)
consts[1] = T_r   = 2.0         # parameter: spatial range (1 sigma) of T_Phi (degrees C)
consts[2] = t_c   = 10.0        # parameter: day length at leaf shedding (hours)
consts[3] = t_r   = 0.5         # parameter: spatial range (1 sigma) of t_c (hours)
consts[4] = xi  = 0.5           # parameter: initial linear leaf growth (1/days)
consts[5] = k_L   = 0.1         # parameter: inverse of leaf longevity from start of senscence (1/days)
consts[6] = lambda_max = 6.0.   # parameter: maximum potential leaf area index (m2/m2)


constant gpppars terms
gpppars[3]=1;
gpppars[6]=DATA.LAT;
gpppars[8]=-2.0;
gpppars[9]=1.0;
gpppars[10]=pi;
gpppars[0]=LAI[n];
gpppars[1]=DATA.MET[m+2];
gpppars[2]=DATA.MET[m+1];
gpppars[4]=DATA.MET[m+4];
gpppars[5]=DATA.MET[m+5];
gpppars[7]=DATA.MET[m+3];
*/


 /* Initialization: only run this on the first time step! */
  //T_init=T_phi+3*T_r
  /* cumulative normal distribution function (derived from the c-function erfc) */
  //T_deviation=(T-T_phi)/T_r
  //f_T=0.5*erfc(-T_deviation*sqrt(0.5))

  // // Exponentially declining memory of temperature
  // T      = exp(- 1 / m.τ_m)*aux.T + m.T_air(t) * (1 - exp(- 1 / m.τ_m))
  // // Updating f (fraction of plants in active growth state) using a cumulative normal distribution function, one for temperature and one for sunlight duration
  // f_T    = cdf(Normal(), (aux.T - m.T_phi)/m.T_r)
  // f_td   = cdf(Normal(), (m.t_d(t) - m.t_c)/m.t_r)
  // f      = aux.f_T * aux.f_td
  // r      = m.ξ * aux.f + (1 - aux.f)*m.k_L
  // // Compute water-limited maximum LAI
  // aux.Λ_W    = m.PASM(t) * state.Λ / m.τ_W / MaxExponentialSmooth(m.E(t), 1e-3, 2e-2)
  // aux.Λ_tilde_max = MinQuadraticSmooth(m.Λ_max, aux.Λ_W, 0.99)
  // // Exponentially declining memory of water limitation
  // aux.Λ_max_memory  = exp(- 1 / m.τ_s)*aux.Λ_max_memory + aux.Λ_tilde_max * (1 - exp(- 1 / m.τ_s))
  // aux.Λ_lim  = m.ξ * aux.Λ_max_memory * aux.f / aux.r
  
  // senescencephaseflag = aux.Λ_lim < state.Λ
  
  // aux.Λ_sources = state.Λ/m.τ_L + m.ξ * (aux.Λ_max_memory - state.Λ) * aux.f
  // aux.Λ_sinks = state.Λ * m.k_L * (1.0 - aux.f) + state.Λ/m.τ_L


  //cps=(double)e0*pars[7]*gc*(pars[4]-ci)/(e0*pars[7]+gc*(pars[4]-ci));
  //GPP=cps*(consts[1]*dayl+consts[4]);

  /*initialize intermediate variables*/
  double n;
  double meantemp;
  double T_init, T_phi, T_r;
  double T, T_memory, T_deviation, f_T, tau_m;
  double plgr, k_L;
  double f, r;
  double pasm, E;
  double lambda, lambda_W, lambda_tilde_max, lambda_max;
  double tau_W;

  meantemp=(double)met_list[0];
  n=(double)var_list[0];
  lambda=(double)var_list[2];
  T_init=(double)0.0;
  T_phi=(double)var_list[3];
  T_r=(double)var_list[4];
  T_memory=(double)var_list[5];
  tau_m=(double)var_list[6];
  plgr=(double)var_list[7];
  k_L=(double)var_list[8];
  pasm=(double)50.0;
  E=(double)10.0;
  tau_W=(double)10.0;
  lambda_max=(double)6.0;

  /* Initialization: only run this on the first time step! */
  // if (n==0){printf("> in LAI_KNORR: T_memory = %2.1f\n",T_memory);}
  // if (n==0){T_memory=T_phi+3*T_r;}   /* set the temperature memory to be high so that we start in growth phase */
  // Exponentially declining memory of temperature
  T      = exp(- 1 / tau_m)*T_memory + meantemp * (1 - exp(- 1 / tau_m));
  T_deviation=(T-T_phi)/T_r;
  /* cumulative normal distribution function (derived from the intrinsic c-function erfc) */
  f_T    = 0.5*erfc(-T_deviation*sqrt(0.5));

  f      = f_T;   /* also multiply by the day-length fraction here when thats ready */
  r      = plgr * f + (1 - f)*k_L;

  /* compute water-limited maximum LAI */
  lambda_W    = pasm * lambda / tau_W / MaxExponentialSmooth(E, 1e-3, 2e-2);
  lambda_tilde_max = MinQuadraticSmooth(lambda_max, lambda_W, 0.99);
  if (n==0){printf("> in LAI_KNORR: lambda = %2.2f\n",lambda);}
  if (n==0){printf("> in LAI_KNORR: lambda_W = %2.2f\n",lambda_W);}
  if (n==0){printf("> in LAI_KNORR: lambda_tilde_max = %2.2f\n",lambda_tilde_max);}

  static double return_arr[4];
  return_arr[0] = 5.0;
  return_arr[1] = 10.0;
  return_arr[2] = T;
  return_arr[3] = r;
  return return_arr;
}

