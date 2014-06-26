#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <Qt>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef Q_OS_WIN
# define FRAKTALEXPLORER_EXPORT __declspec(dllexport)
#else
# define FRAKTALEXPLORER_EXPORT
#endif

/**
 * A return value of 0 say that everything is okey, -1, a failure happened
 */
#define FRAKTALEXPLORER_INITPLUGIN int FRAKTALEXPLORER_EXPORT fraktalexplorer_initPlugin()
#define FRAKTALEXPLORER_INITPLUGIN_NAME "fraktalexplorer_initPlugin"
typedef int (*FRAKTALEXPLORER_INITPLUGIN_FUNCTION)();

/**
 * The same above
 */
#define FRAKTALEXPLORER_ALGORITHM(it, lim, ax, ay, a, b, isJuliaSet) int FRAKTALEXPLORER_EXPORT fraktalexplorer_algorithm(int* it, const int lim, double* ax, double* ay, double* a, double* b, int isJuliaSet)
#define FRAKTALEXPLORER_ALGORITHM_NAME "fraktalexplorer_algorithm"
typedef int (*FRAKTALEXPLORER_ALGORITHM_FUNCTION)(int*, const int, double*, double*, double*, double*, int);

#ifdef __cplusplus
}
#endif

#endif // PLUGININTERFACE_H
