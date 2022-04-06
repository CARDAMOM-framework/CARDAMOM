#pragma once

typedef struct MLF{
    int nolikelihoods;
    double (*mlf)(DATA, double *);
}MLF;


