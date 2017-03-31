#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>

#include "functions.h"

#define C 41.47 // hbar^2/m MeV*fm^2

#define R 40. //fm -> Tail point 
#define N 10000 // Number of points in the left part
#define H (R/(N-1)) //fm -> step

double E;

double *X, *Y;

// Function for Numerov algorithm
double W(double r)
{
    return (Gauss(r) - E)/C;
}

// k*cot(delta) with Cramer method on the tail:
// Y[N-2] = A sin(k*X[N-2]) + B cos(k*X[N-2])
// Y[N-1] = A sin(K*X[N-1]) + B cos(K*X[N-1])
// k*cot(delta) = k*A/B
double Shift(void)
{
    const double k = sqrt(E/C);
    const double a = sin(k*X[N-2]);
    const double b = cos(k*X[N-2]);
    const double c = sin(k*X[N-1]);
    const double d = cos(k*X[N-1]);

    const double det = a*d - b*c;
    const double A = (Y[N-2]*d - b*Y[N-1])/det;
    const double B = (a*Y[N-1] - Y[N-2]*c)/det;

    return k*A/B;
}

int main()
{
    //External variables: depth and width of the Gaussian potential
    V0 = -60.575; //Mev
    R0 = 1.65; //fm

    X = malloc(N*sizeof(double));
    Y = malloc(N*sizeof(double));
    Linspace(X,0.,R,N);
    Y[0] = 0.;
    Y[1] = H;

    for (E=0.2; E<10.; E+=0.2) 
    {
        NumerovRight(X,Y,W,N);
        printf("%.3f\t%.5f\n", E/C , Shift());
    }

    //puts("*****");
    //puts("To obtain a scattering length of -23.74,");
    //puts("with a potential width of 1.65");
    //printf("The depth needs to be: %.7f\n", V0);
    //puts("*****");

    //for (int i=0; i<N; i++)
    //{
    //    printf("%.16f\n", Y[i]);
    //}

    free(X);
    free(Y);

    return 0;
}

