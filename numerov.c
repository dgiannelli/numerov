#include <assert.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>

#include "numerov.h"
#include "potential.h"
#include "linspace.h"

#define MACRO(n) ( 1. -gsl_pow_2(h)/12.*W(X[n]) ) 

// Numerov algorithm: given Z0 and Z1 finds Z2
void NumerovRight(double *X, double *Y, double (*W)(double), int N)
{
    assert(N>2);
    assert(X && Y && W);

    const double h = X[1] - X[0];

    double Z0 = MACRO(0) * Y[0];
    double Z1 = MACRO(1) * Y[1];
    double Z2;
    for(int i=0; i<N-2; i++)
    {
        Z2 = -Z0 - 10.*Z1 + 12.*Y[i+1];

        Z0 = Z1;
        Z1 = Z2;

        Y[i+2] = Z2 / MACRO(i+2);
    }
}

// Numerov algorithm: given Z2 and Z1 finds Z0
void NumerovLeft(double *X, double *Y, double (*W)(double), int N)
{
    assert(N>2);
    assert(X && Y && W);

    const double h = X[1] - X[0];

    double Z2 = MACRO(N-1) * Y[N-1];
    double Z1 = MACRO(N-2) * Y[N-2];
    double Z0;
    for(int i=N-1; i>1; i--)
    {
        Z0 = -Z2 - 10.*Z1 + 12.*Y[i-1];

        Z2 = Z1;
        Z1 = Z0;

        Y[i-2] = Z0 / MACRO(i-2);
    }
}

void TestNumerov(void)
{
    const int N = 1000;
    double *X = malloc(N*sizeof(double));
    double *Y = malloc(N*sizeof(double));
    Linspace(X,1.,2.,N);

    //Tests the right step
    Y[0] = sin(X[0]);
    Y[1] = sin(X[1]);

    V0 = -1.;
    R0 = 3.;
    NumerovRight(X,Y,Well,N);

    assert( gsl_fcmp(Y[N-1],sin(X[N-1]),1.e-8) == 0 );

    //Tests the left step
    Y[N-1] = exp(X[N-1]);
    Y[N-2] = exp(X[N-2]);

    V0 = 1.;
    R0 = 3.;
    NumerovLeft(X,Y,Well,N);

    assert( gsl_fcmp(Y[0],exp(X[0]),1.e-8) == 0 );

    free(X);
    free(Y);
}

