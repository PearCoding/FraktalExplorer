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
