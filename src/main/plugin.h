#ifndef PLUGIN_H
#define PLUGIN_H

#include <QString>
#include <QLibrary>

#include "plugininterface.h"

class Plugin
{
    friend class PluginManager;
public:
    Plugin(QString path="", QString version="");
    virtual ~Plugin();

    inline void setPath(QString& path)
    {
        library.setFileName(path);
    }

    inline QString getPath() const
    {
        return library.fileName();
    }

    inline void setVersion(QString& version)
    {
        library.setFileNameAndVersion(library.fileName(), version);
    }

    virtual bool init();

protected:
    inline QLibrary* getLibrary()
    {
        return &library;
    }

private:
    QLibrary library;
    FRAKTALEXPLORER_INITPLUGIN_FUNCTION initFunction;
};

#endif // PLUGIN_H
