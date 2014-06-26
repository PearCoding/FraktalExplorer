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

#include "plugin.h"

#include <QDebug>

Plugin::Plugin(QString path, QString version) :
        library(path, version), initFunction(0)
{
}

Plugin::~Plugin()
{
    library.unload();
}

bool Plugin::init()
{
    library.load();
    if(!library.isLoaded())
    {
        qCritical() << "Couldn't load library" << library.fileName() << ":" << library.errorString();
        return false;
    }

    initFunction = (FRAKTALEXPLORER_INITPLUGIN_FUNCTION) library.resolve(FRAKTALEXPLORER_INITPLUGIN_NAME);
    if(!initFunction)
    {
        qCritical() << "Couldn't load the symbol" << FRAKTALEXPLORER_INITPLUGIN_NAME << "from" << library.fileName() << ":" << library.errorString();
        return false;
    }

    int ret = initFunction();
    if(!ret)
    {
        qDebug() << "Successful init plugin";
        return true;
    }
    else
    {
        qCritical() << "Couldn't init plugin, return value" << ret;
        return false;
    }
}
