#include "pluginmanager.h"

#include <QDebug>

PluginManager* PluginManager::instance = 0;

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
    if(instance)
    {
        qWarning() << "There is already a pluginmanager";
    }
    else
    {
        instance = this;
    }
}

PluginManager::~PluginManager()
{
    if(instance == this)
    {
        instance = 0;
    }

    qDeleteAll(plugins);
}
