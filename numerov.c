#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>

#include <stdio.h>

#include "numerov.h"
#include "potential.h"
#include "linspace.h"

#define MACRO(n) ( 1. -gsl_pow_2(h)/12.*W(X[n]) ) 

void NumerovRight(double *X, double *Y, double (*W)(double), int N, ...)
{
    assert(N>2);
    assert(X && Y && W);

    va_list ap;
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

void TestNumerovRight(void)
{
    const int N = 1000;
    double *X = malloc(N*sizeof(double));
    Linspace(X,0.,1,N);

    double *Y = malloc(N*sizeof(double));
    Y[0] = X[0];
    Y[1] = X[1];

    V0 = -1.;
    R0 = 2.;
    NumerovRight(X,Y,Well,N);

    assert( gsl_fcmp(Y[N-1],sin(X[N-1]),1.e-6) == 0 );

    free(X);
    free(Y);
}

