#include <assert.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>
#include <stdio.h>

#include "linspace.h"

// Computes the 3-points right derivative of Y at the Nth point with X-step=h
double DRight3(double *Y, int N, double h)
{
    return (1./2.*Y[N-2] - 2.*Y[N-1] + 3./2.*Y[N])/h;
}

// Computes the 3-points left derivative of Y at the Nth point with X-step=h
double DLeft3(double *Y, int N, double h)
{
    return (-3./2.*Y[N] + 2.*Y[N+1] - 1./2.*Y[N+2])/h;
}

// Computes the 3-points right logarithmic derivative of Y at the Nth point with X-step=h
double DLogRight3(double *Y, int N, double h)
{
    return DRight3(Y,N,h)/Y[N];
}

// Computes the 3-points left logarithmic derivative of Y at the Nth point with X-step=h
double DLogLeft3(double *Y, int N, double h)
{
    return DLeft3(Y,N,h)/Y[N];
}

void TestD3(void)
{
    const int N = 1000;
    double *X = malloc(N*sizeof(double));
    double *Y = malloc(N*sizeof(double));

    Linspace(X,0.,M_PI_2,N);
    for (int i=0; i<N; i++) Y[i]=sin(X[i]);

    assert( fabs(DRight3(Y,N-1,X[N-1]-X[N-2])) < 1.e-6 );
    assert( gsl_fcmp(DLeft3(Y,0,X[1]-X[0]),1.,1.e-6) == 0 );
}

