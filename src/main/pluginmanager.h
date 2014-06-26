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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

#include "plugin.h"
#include "algorithmplugin.h"

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = 0);
    ~PluginManager();

    static inline PluginManager* getInstance()
    {
        return instance;
    }

    inline AlgorithmPlugin* loadAlgorithmPlugin(QString path, QString version = "")
    {
        AlgorithmPlugin* plugin = (AlgorithmPlugin*) getPlugin(path);

        if(!plugin)
        {
            plugin = new AlgorithmPlugin(path, version);

            if(plugin)
            {
                loadPlugin(plugin);
            }
        }

        return plugin;
    }

    inline Plugin* loadPlugin(QString path, QString version = "")
    {
        Plugin* plugin = getPlugin(path);

        if(!plugin)
        {
            plugin = new Plugin(path, version);

            if(plugin)
            {
                loadPlugin(plugin);
            }
        }

        return plugin;
    }

    inline void loadPlugin(Plugin* plugin)
    {
        if(!plugins.contains(plugin))
        {
            plugins.append(plugin);

            if(!plugin->getLibrary()->isLoaded())
            {
                if(!plugin->init())
                {
                    qCritical("An error happened while initializing the algorithm plugin");
                }
            }
        }
    }

    inline void removePlugin(Plugin* plugin)
    {
        plugins.removeOne(plugin);
    }

    inline bool contains(QString path)
    {
        return getPlugin(path) ? true : false;
    }

    inline Plugin* getPlugin(QString path)
    {
        foreach(Plugin* plugin, plugins)
        {
            if(plugin->getPath() == path)
            {
                return plugin;
            }
        }
        return 0;
    }

signals:

public slots:

private:
    static PluginManager* instance;
    QList<Plugin*> plugins;
};

#endif // PLUGINMANAGER_H
