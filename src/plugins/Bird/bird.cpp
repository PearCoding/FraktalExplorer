#include "plugininterface.h"

#include <math.h>

#define PI 3.1415926535897932384626433832795
#define EULER 2.718281828459045

extern "C"
{
	FRAKTALEXPLORER_INITPLUGIN;
	FRAKTALEXPLORER_ALGORITHM(it, lim, ax, ay, a, b, isJuliaSet);
}

FRAKTALEXPLORER_INITPLUGIN
{
    return 0;
}

FRAKTALEXPLORER_ALGORITHM(it, lim, ax, ay, a, b, isJuliaSet)
{
    ++(*it);
    double ax2;
    double ay2;

    if(isJuliaSet)
    {
        static double sax = *ax;
        ax2 = sax;
        static double say = *ay;
        ay2 = say;
    }
    else
    {
        ax2 = *ax;
        ay2 = *ay;
    }

    double pa = *a;
    double pb = *b;

    double a2 = pow(EULER, -(pa*pa) - (pb*pb)) + ax2;
    double b2 = (PI*pa*pb) + ay2;

    if ((a2 * a2) + (b2 * b2) > lim)
    {
        return -1;
    }

    ++it;

    pa = *a = pow(EULER, -(a2*a2) - (b2*b2)) + ax2;
    pb = *b = (PI*a2*b2) + ay2;

    if ((pa * pa) + (pb * pb) > lim)
    {
        return -1;
    }

    return 0;
}
