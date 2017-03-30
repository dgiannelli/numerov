#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>

#include "functions.h"

#define C 41.47 // hbar^2/m MeV*fm^2

#define R 40. //fm -> Tail point 
#define N 10000 // Number of points in the left part
#define H (R/(N-1)) //fm -> step

#define SL(n) (X[n]-Y[n]*H/(Y[n]-Y[n-1])) // Scattering length formula

double W(double r)
{
    return Gauss(r)/C;
}

int main()
{
    //External variables: depth and width of the Gaussian potential
    V0 = -37.7; //Mev
    R0 = 1.65; //fm

    double *X = malloc(N*sizeof(double));
    double *Y = malloc(N*sizeof(double));
    Linspace(X,0.,R,N);

    Y[0] = 0.;
    Y[1] = H;
    NumerovRight(X,Y,W,N);
    puts("*****");
    printf("For V0 = %g and R0 = %g,\n", V0, R0);
    puts("the scattering length is:");
    printf("     a = %.16f\n", SL(N-1));
    puts("*****\n");
    //printf("The algorithm converged after %d iterations, with final energy:\nE = %.7f\n", iter, E);

    //for (int i=0; i<N; i++)
    //{
    //    printf("%.16f\n", Y[i]);
    //}

    free(X);
    free(Y);

    return 0;
}

