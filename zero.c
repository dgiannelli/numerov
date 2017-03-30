#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>

#include "functions.h"

#define C 41.47 // hbar^2/m MeV*fm^2

#define R 40. //fm -> Tail point 
#define N 1000 // Number of points in the left part
#define H (R/(N-1)) //fm -> step

double *X, *Y;

//Function for Numerov algorithm
double W(double r)
{
    return Gauss(r)/C;
}

//Scattering length
#define A (X[N-1]-Y[N-1]*H/(Y[N-1]-Y[N-2]))

// Function that needs the root to be found:
// the depth V such that the scattering length is the right one for nn
double F(double V)
{
    V0 = V;
    NumerovRight(X,Y,W,N);
    return -23.74 - A;
}

int main()
{
    //External variable: width of the Gaussian potential
    R0 = 1.65; //fm

    X = malloc(N*sizeof(double));
    Y = malloc(N*sizeof(double));
    Linspace(X,0.,R,N);
    Y[0] = 0.;
    Y[1] = H;

    Newton(F,-37.,-36.);

    puts("*****");
    puts("To obtain a scattering length of -23.74,");
    puts("with a potential width of 1.65");
    printf("The depth needs to be: %.7f\n", V0);
    puts("*****");

    //for (int i=0; i<N; i++)
    //{
    //    printf("%.16f\n", Y[i]);
    //}

    free(X);
    free(Y);

    return 0;
}

