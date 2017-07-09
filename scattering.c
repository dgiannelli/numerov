#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <gsl/gsl_math.h>

#include "functions.h"

#define K 41.47 // hbar^2/m MeV*fm^2

#define R 50. // Tail point (in fm)
#define N 4000 // Number intervals
#define H (R/N) // Step of the discretization (in fm)

double E;

double *X, *Y;

// Function for Numerov algorithm
double W(double r)
{
    return (Gauss(r) - E)/K;
}

// return k*cot(delta) with Cramer method on the tail:
// Y[N-1] = A sin(k*X[N-1]) + B cos(k*X[N-1])
// Y[N]   = A sin(K*X[N])   + B cos(K*X[N])
// k*cot(delta) = k*A/B
double F(void)
{
    const double k = sqrt(E/K);
    
    const double cot = (Y[N-1]*cos(k*X[N])-Y[N]*cos(k*X[N-1])) /
                       (Y[N]*sin(k*X[N-1])-Y[N-1]*sin(k*X[N]));

    return k*cot;
}

int main()
{
    //External variables: depth and width of the Gaussian potential
    V0 = -60.575; //Mev
    R0 = 1.65; //fm

    X = malloc((N+1)*sizeof(double));
    Y = malloc((N+1)*sizeof(double));
    Linspace(X,0.,R,N+1);
    Y[0] = 0.;
    Y[1] = H;
    
    FILE *outScat; assert(outScat=fopen("scattering.dat","w"));
    for (double kSq = 0.05; kSq<=0.5; kSq += 0.05) 
    {
        E = kSq*K;
        NumerovRight(X,Y,W,N+1);
        fprintf(outScat, "%.3e\t%.7e\n", kSq, F());
    }
    assert( !fclose(outScat) );

    free(X);
    free(Y);

    return 0;
}

