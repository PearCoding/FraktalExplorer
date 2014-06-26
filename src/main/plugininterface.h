/*
 * This file is part of the FraktalExplorer project.
 * FraktalExplorer is a dynamic fractal explorer with an internal L-System parser.
 * Copyright (C) 2010-2014 Ömercan Yazici <pearcoding@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not,
 * write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110, USA
 *
 * More information about FraktalExplorer here:
 * http://pearcoding.eu/projects/fraktalexplorer
 */

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
