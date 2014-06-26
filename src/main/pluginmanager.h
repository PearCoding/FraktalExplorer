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
