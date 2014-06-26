#include "algorithmplugin.h"

#include <QDebug>

AlgorithmPlugin::AlgorithmPlugin(QString path, QString version) :
        Plugin(path, version), algorithmFunction(0)
{
}

bool AlgorithmPlugin::init()
{
    if(Plugin::init())
    {
        algorithmFunction = (FRAKTALEXPLORER_ALGORITHM_FUNCTION) getLibrary()->resolve(FRAKTALEXPLORER_ALGORITHM_NAME);

        if(algorithmFunction)
        {
            qDebug() << "Successful load symbol" << FRAKTALEXPLORER_ALGORITHM_NAME;
            return true;
        }
        else
        {
            qCritical() << "Couldn't load symbol" << FRAKTALEXPLORER_ALGORITHM_NAME;
            return false;
        }
    }
    else
    {
        return false;
    }

}
