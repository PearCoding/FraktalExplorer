#include "plugininterface.h"

#include <math.h>

extern "C"
{
	FRAKTALEXPLORER_INITPLUGIN;
	FRAKTALEXPLORER_ALGORITHM(it, lim, ax, ay, a, b, isJuliaSet);
}

#define EULER 2.718281828459045

FRAKTALEXPLORER_INITPLUGIN
{
    return 0;
}

FRAKTALEXPLORER_ALGORITHM(it, lim, ax, ay, a, b, isJuliaSet)//EULER^z + c
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

    double e = pow(EULER,pa);
    double a2 = cos(pb) * e + ax2;
    double b2 = sin(pb) * e + ay2;

    if ((a2 * a2) + (b2 * b2) > lim)
    {
        return -1;
    }

    ++it;

    e = pow(EULER,a2);
    pa = *a = cos(b2) * e + ax2;
    pb = *b = sin(b2) * e + ay2;

    if ((pa * pa) + (pb * pb) > lim)
    {
        return -1;
    }

    return 0;
}
