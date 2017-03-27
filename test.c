#include <assert.h>
#include <stdio.h>

#include "functions.h"

int main()
{
    TestLinspace();
    TestD3();
    TestNumerov();

    puts("Everything OK!");
    return 0;
}
