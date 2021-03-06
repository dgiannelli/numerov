#include <assert.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>

#include "functions.h"

double R0 = 1.65;
double V0 = -60.575;

double Zero(double r) {return 0.;}

double Well(double r)
{
    if (r < R0) return V0;
    else return 0.;
}

double Gauss(double r) {return V0*exp(-gsl_pow_2(r/R0));}

void Linspace(double *X, double from, double to, int N)
{
    assert(from<to);
    assert(N>1);

    const double step = (to-from)/(N-1);

    for(int i=0; i<N; i++)
    {
        X[i] = from + step*i;
    }
}

// Computes the 3-points right derivative of Y at the Nth piont (Xstep must be constant)
double DRight3(double *Y, int N, double h)
{
    return (1./2.*Y[N-2] - 2.*Y[N-1] + 3./2.*Y[N])/h;
}

// Computes the 3-points left derivative of Y at the Nth point (Xstep must be constant)
double DLeft3(double *Y, int N, double h)
{
    return (-3./2.*Y[N] + 2.*Y[N+1] - 1./2.*Y[N+2])/h;
}

// Computes the 3-points right logarithmic derivative of Y at the Nth point (Xstep must be constant)
double DLogRight3(double *Y, int N, double h)
{
    return DRight3(Y,N,h)/Y[N];
}

// Computes the 3-points left logarithmic derivative of Y(X) at the Nth point (Xstep must be constant)
double DLogLeft3(double *Y, int N, double h)
{
    return DLeft3(Y,N,h)/Y[N];
}

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

int newtonIters = 0;
// Root finding of function F: x0 and x1 are the first and second guesses
double Newton(double (*F)(double), double x1, double x2)
{
    newtonIters = 0;
    double y1, y2;
    y1 = F(x1);
    y2 = F(x2);
    while ( (y2-y1 != 0) && (fabs(x1-x2) > 1.e-10) )
    {
        const double x3 = x2 - y2*(x2-x1)/(y2-y1);
        x1 = x2;
        x2 = x3;
        y1 = y2;
        y2 = F(x3);
        newtonIters++;
    }
    return x2;
}

// ***** Function tests *****

void TestLinspace(void)
{
    double *X = malloc(3*sizeof(double));

    Linspace(X, 1., 2., 3);

    assert(X[0] == 1.);
    assert(X[1] == 1.5);
    assert(X[2] == 2.);
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

void TestNewton(void)
{
    double F(double x) {return gsl_pow_2(x) - 2.;}
    assert( gsl_fcmp(Newton(F,-3.,-2.),-sqrt(2), 1.e-7) == 0 );
}

