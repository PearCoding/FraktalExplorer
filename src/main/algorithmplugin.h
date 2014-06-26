#ifndef ALGORITHMPLUGIN_H
#define ALGORITHMPLUGIN_H

#include <QString>

#include "plugin.h"

class AlgorithmPlugin : public Plugin
{
public:
    AlgorithmPlugin(QString path = "", QString version = "");

    bool init();

    inline int algorithm(int* it, const int lim, double* ax, double* ay, double* a, double* b, int isJuliaSet)
    {
        if(algorithmFunction)
        {
            return algorithmFunction(it, lim, ax, ay, a, b, isJuliaSet);
        }
        else
        {
            return -1;
        }
    }

private:
    FRAKTALEXPLORER_ALGORITHM_FUNCTION algorithmFunction;
};

#endif // ALGORITHMPLUGIN_H
