%*****Step 1. Read example files*******
%Step 1.1 Load files
PARS0=load('CARDAMOM/DATA/ENTROPY_REDUCTION_EXAMPLE_APR20/example_pars_baseline.csv');%"Prior" from now on
PARS=load('CARDAMOM/DATA/ENTROPY_REDUCTION_EXAMPLE_APR20/example_pars_cmsflux.csv');%"Posterior" from now on

%Step 1.2 Log-transform & mod(*,365.25) for date variables,
PARS0(:,[12,15])=mod(PARS0(:,[12,15]),365.25);
PARS(:,[12,15])=mod(PARS(:,[12,15]),365.25);
PARS0=log(PARS0);
PARS=log(PARS);

%******Step 2. Covariance matrices******

S0= cov(PARS0);%Prior covariance
S= cov(PARS);%Posterior covariance

%*****Step 3. AKs and entropy reduction****
%Averaging kernel
AK=1 - S*inv(S0);

%Determinants

%Entropy reduction = 1/2 log2(|S0|) - 1/2 log2 (|S|), from fisher 2003,
%"Estimation of Entropy Reduction and Degrees of Freedom for Signal for Large Variational Analysis Systems"
Ered = 0.5*(log2(det(S0)) - log2(det(S)));






