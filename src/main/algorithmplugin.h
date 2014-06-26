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
