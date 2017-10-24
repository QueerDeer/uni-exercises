#include <stdio.h>
#include <string.h>

double computExp(double base, int power) 
{
    int i = 0;
    double exp = 1;

    for (i; i<power; i++)
    {
        exp*=base;
    }

    return exp;
}

char* getCat()
{
    static char catBody[] = "ฅ^..^ฅ";
    return catBody;
}
