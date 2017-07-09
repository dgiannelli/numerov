#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <gsl/gsl_math.h>

#include "functions.h"

#define K 41.47 // hbar^2/m MeV*fm^2

int N1; // Number of intervals in the left part
int N2; // Number of intervals in the right part
double H; // Step of the discretization (in fm)

double E; // Energy

double *X1; // Points in the left interval
double *X2; // Points in the right interval
double *Y1; // Left wave function
double *Y2; // Right wave function

//Function for Numerov algorithm
double W(double r)
{
    return (Gauss(r)-E)/K;
}

// Function whose root need to be found:
double F(double e)
{
    E = e; // Sets the new energy value

    NumerovRight(X1,Y1,W,N1+1); // Finds solution from left to right

    Y2[N2] = 1.e-4;
    Y2[N2-1] = Y2[N2]*exp(-E/K);
    NumerovLeft(X2,Y2,W,N2+1); // Finds solution from right to left

    const double DL = DLogRight3(Y1,N1,H); // Computes the right 3points log derivative
    const double DR = DLogLeft3(Y2,0,H); // Computes the left 3points log derivative
    return DL - DR;
}

// argv -> R1 R2 N mode
// R1 = Matching point (in fm)
// R2 = Tail point (in fm)
// N = Number of discretized intervals
// mode = 0 -> saves the binding energy and the normalyzed wave function
// mode = 1 -> appends the binding energy vs # of intervals
// mode = 2 -> appends the binding energy vs tail point
// mode = 3 -> appends the binding energy vs matching point
int main(int argc, char *argv[])
{
    assert(argc==5);
    double R1 = atof(argv[1]);
    double R2 = atof(argv[2]);
    int N = atoi(argv[3]);
    int mode = atoi(argv[4]);

    N1 = ((int) round(N*R1/R2));
    N2 = (N-N1);
    H = (R2/N); 

    //External variables: depth and width of the Gaussian potential
    V0 = -60.575; //Mev
    R0 = 1.65; //fm

    X1 = malloc((N1+1)*sizeof(double));
    X2 = malloc((N2+1)*sizeof(double));
    Y1 = malloc((N1+1)*sizeof(double));
    Y2 = malloc((N2+1)*sizeof(double));

    Linspace(X1,0.,N1*H,N1+1); // Left interval discretization
    Linspace(X2,N1*H,R2,N2+1); // Right interval discretization
    Y1[0] = 0.;
    Y1[1] = H;

    Newton(F,-2.,-2.1); // Finds the root of F(e) with Newton tangent method

    printf("The algorithm converged after %d iterations, with final energy: E = %.9f\n", 
            newtonIters, E);

    // Compute and save the normalized wave function
    if (mode==0)
    {
        // Set the same overall constant
        const double A = Y2[0]/Y1[N1];
        for (int i=0; i<=N1; i++) Y1[i] *= A;

        // Compute the square integral
        double sumSq = 0.5*gsl_pow_2(Y2[N2]);
        for (int i=1; i<=N1; i++)
        {
            sumSq += gsl_pow_2(Y1[i]);
        }
        for (int i=1; i<N2; i++)
        {
            sumSq += gsl_pow_2(Y2[i]);
        }
        sumSq *= H;
        sumSq += gsl_pow_2(Y2[N2])*0.5*sqrt(-K/E); //Analytic integral of the tail
        const double norm = sqrt(sumSq);

        FILE *outWave; assert( outWave=fopen("boundWave.dat", "w") );
        for (int i=0; i<=N1; i++)
        {
            fprintf(outWave, "%.5e\n", Y1[i]/norm);
        }
        for (int i=1; i<=N2; i++)
        {
            fprintf(outWave, "%.5e\n", Y2[i]/norm);
        }
        assert( !fclose(outWave) );
    }

    free(X1);
    free(X2);
    free(Y1);
    free(Y2);

    // Appends # of intervals and binding energy
    if (mode==1)
    {
        FILE *outInter; assert( outInter=fopen("boundInter.dat", "a") );
        fprintf(outInter, "%05d\t%.10e\n", N, -E);
        assert( !fclose(outInter) );
    }

    // Appends tail point and binding energy
    if (mode==2)
    {
        FILE *outTail; assert( outTail=fopen("boundTail.dat", "a") );
        fprintf(outTail, "%.2f\t%.10e\n", R2, -E);
        assert( !fclose(outTail) );
    }

    // Appends matching point and binding energy
    if (mode==3)
    {
        FILE *outMatch; assert( outMatch=fopen("boundMatch.dat", "a") );
        fprintf(outMatch, "%.2f\t%.10e\n", R1, -E);
        assert( !fclose(outMatch) );
    }

    return 0;
}

