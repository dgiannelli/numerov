#include <assert.h>

#include "linspace.h"
#include "numerov.h"

int main()
{
    TestLinspace();
    TestNumerovRight();
    TestNumerovLeft();

    return 0;
}
