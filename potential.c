#include <gsl/gsl_math.h>

double R0 = 1.65;
double V0 = -60.575;

double Zero(double r) {return 0.;}

double Well(double r)
{
    if (r < R0) return V0;
    else return 0.;
}

double Gauss(double r) {return V0*exp(-gsl_pow_2(r/R0));}

