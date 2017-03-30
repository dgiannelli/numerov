#include <assert.h>
#include <stdio.h>

#include "functions.h"

int main()
{
    TestLinspace();
    TestD3();
    TestNumerov();
    TestNewton();
    assert(newtonIters<100);

    puts("Everything OK!");
    return 0;
}
