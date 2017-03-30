#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>

#include "functions.h"

#define C 41.47 // hbar^2/m MeV*fm^2

#define R1 3. //fm -> Matching point
#define R2 40. //fm -> Tail point 
#define N1 10000 // Number of points in the left part
#define N2 10000 // Number of points in the right part
#define H1 (R1/(N1-1)) //fm -> step of the left part
#define H2 ((R2-R1)/(N2-1)) //fm -> step of the right part

double E;

double W(double r)
{
    return (Gauss(r)-E)/C;
}

int main()
{
    //External variables: depth and width of the Gaussian potential
    V0 = -60.575; //Mev
    R0 = 1.65; //fm

    double *X1 = malloc(N1*sizeof(double));
    double *X2 = malloc(N2*sizeof(double));
    double *Y1 = malloc(N1*sizeof(double));
    double *Y2 = malloc(N2*sizeof(double));
    Linspace(X1,0.,R1,N1);
    Linspace(X2,R1,R2,N2);

    double E1 = -2.; //MeV -> First energy guess
    double E2 = -2.1;//MeV -> Second energy guess

    E = E1;

    Y1[0] = 0.;
    Y1[1] = H1;
    NumerovRight(X1,Y1,W,N1);

    Y2[N2-1] = 1.e-4;
    Y2[N2-2] = Y2[N2-1]*exp(-E/C);
    NumerovLeft(X2,Y2,W,N2);

    const double DL1 = DLogRight3(Y1,N1-1,H1);
    const double DR1 = DLogLeft3(Y2,0,H2);
    double Delta1 = DL1 - DR1;

    E = E2;
    NumerovRight(X1,Y1,W,N1);
    Y2[N2-1] = 1.e-4;
    Y2[N2-2] = Y2[N2-1]*exp(-E/C);
    NumerovLeft(X2,Y2,W,N2);

    double DL2 = DLogRight3(Y1,N1-1,H1);
    double DR2 = DLogLeft3(Y2,0,H2);
    double Delta2 = DL2 - DR2;

    int iter = 0;
    while(fabs(Delta2-Delta1)>1.e-8)
    {
        const double E3 = E2 - Delta2*(E2-E1)/(Delta2-Delta1);
        E1 = E2;
        Delta1 = Delta2;
        E2 = E3;

        E = E2;
        NumerovRight(X1,Y1,W,N1);
        Y2[N2-1] = 1.e-4;
        Y2[N2-2] = Y2[N2-1]*exp(-E/C);
        NumerovLeft(X2,Y2,W,N2);

        DL2 = DLogRight3(Y1,N1-1,H1);
        DR2 = DLogLeft3(Y2,0,H2);
        Delta2 = DL2 - DR2;
        iter++;
    }
    printf("The algorithm converged after %d iterations, with final energy:\nE = %.7f\n", 
            iter, E);

    free(X1);
    free(X2);
    free(Y1);
    free(Y2);

    //Computation of the normalized wave function
    double *X = malloc((N1+N2)*sizeof(double));
    double *Y = malloc((N1+N2)*sizeof(double));
    Linspace(X,0.,R2,N1+N2);
    Y[0] = 0.;
    Y[1] = R2/(N1+N2-1);
    NumerovRight(X,Y,W,N1+N2);

    double sumSq = 0.;
    for (int i=0; i<N1+N2; i++)
    {
        sumSq += gsl_pow_2(Y[i])*R2/(N2+N1-1);
    }
    sumSq += gsl_pow_2(Y[N1+N2-1])*sqrt(-0.5*C/E); //Analytic integral of the tail

    const double norm = sqrt(sumSq);
    for (int i=0; i<N1+N2; i++)
    {
        Y[i] /= norm;
    }

    for (int i=0; i<N1+N2; i++)
    {
        printf("%.16f\n", Y[i]);
    }

    free(X);
    free(Y);

    return 0;
}

