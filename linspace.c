#include <assert.h>
#include <stdlib.h>

#include "linspace.h"

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

void TestLinspace(void)
{
    double *X = malloc(3*sizeof(double));

    Linspace(X, 1., 2., 3);

    assert(X[0] == 1.);
    assert(X[1] == 1.5);
    assert(X[2] == 2.);
}

