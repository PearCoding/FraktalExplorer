#include "plugininterface.h"

#include <math.h>

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

    double a2 = (pa * pa) - (pb * pb) + ax2; //a1² - b1² + ax
    double b2 = (2 * pa * pb) + ay2;//2 * a1 * b1 + ay

    if ((a2 * a2) + (b2 * b2) > lim)
    {
        return -1;
    }

    ++it;
    pa = *a = (a2 * a2) - (b2 * b2) + ax2;
    pb = *b = (2 * a2 * b2) + ay2;

    if ((pa * pa) + (pb * pb) > lim)
    {
        return -1;
    }

    return 0;
}
