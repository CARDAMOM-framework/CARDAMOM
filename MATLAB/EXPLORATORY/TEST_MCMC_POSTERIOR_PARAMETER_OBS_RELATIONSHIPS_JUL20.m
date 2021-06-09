%Step 1. Define model "C = A*B"
DATA.obs=10;
MODEL=@(pars) pars(:,1).*pars(:,2).^2;

%Step 2. Define p(x)p(O|x)
MLF=@(DATA,pars) -0.5*(MODEL(pars)-DATA.obs).^2 - 0.5*sum((pars - 2).^2);

%Step 3. Ranges for a and b
PARS.min=[0.1,0.1];
PARS.max=[10,10];
PARS.init=[1,1];

%Step 4. run mcmc
MCO.nout=100000;
MCO.printrate=1000;
mcmc_output=MHMCMC(DATA,MLF,PARS,MCO);

Aposterior=mcmc_output.PARSOUT(end/2+1:end,1);
Bposterior=mcmc_output.PARSOUT(end/2+1:end,2);
Cposterior=MODEL(mcmc_output.PARSOUT(end/2+1:end,:));

%Step 5. dc/da and dc/db
dcda=(MODEL([Aposterior+0.00001,Bposterior]) - Cposterior)./(0.00001);
dcdb=(MODEL([Aposterior,Bposterior+0.00001]) - Cposterior)./(0.00001);

%Step 6, make plots
figure(1);clf

subplot(3,3,1);hist(Aposterior);xlabel('A');
subplot(3,3,2);hist(Bposterior);xlabel('B');
subplot(3,3,3);hist(Cposterior);xlabel('C');

subplot(3,3,4);plot(Aposterior,Cposterior,'.');xlabel('A');ylabel('C');
subplot(3,3,5);plot(Bposterior,Cposterior,'.');xlabel('B');ylabel('C');
subplot(3,3,6);plot(Aposterior,Bposterior,'.');xlabel('A');ylabel('B');

subplot(3,3,7);hist(dcda);xlabel('dC/dA');
subplot(3,3,8);hist(dcdb);xlabel('dC/dB');
subplot(3,3,9);plot(dcda,dcdb,'.');xlabel('dC/dA');ylabel('dC/dB');
