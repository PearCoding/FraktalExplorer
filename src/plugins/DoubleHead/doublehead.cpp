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

    double a2 = (pa * pa * pa) - (3*(pb * pb)*pa) + ax2; //a1³ - 3 * b1² * a1 + ax
    double b2 = (3 * pa * pa * pb) + ay2 - (pb * pb * pb);//3 * a1 * b1² + ay - b1³

    if ((a2 * a2) + (b2 * b2) > lim)
    {
        return -1;
    }

    ++it;
    pa = *a = (a2 * a2 * a2) - (3*(b2 * b2)*a2) + ax2;
    pb = *b = (3 * a2 * a2 * b2) + ay2 - (b2 * b2 * b2);

    if ((pa * pa) + (pb * pb) > lim)
    {
        return -1;
    }

    return 0;
}
