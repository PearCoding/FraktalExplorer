#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QIODevice>
#include <QList>
#include <QDomDocument>

#include "fraktale.h"

#define PS_XML_HEADER "FraktalExplorerXML"
#define PS_XML_VERSION "0.1"

#define PS_XML_FRAKTAL "Fraktal"

#define PS_XML_FRAKTAL_NAME "name"
#define PS_XML_FRAKTAL_ISLSYSTEM "isLSystem"
#define PS_XML_FRAKTAL_DEFAULTLENGTH "defaultLength"
#define PS_XML_FRAKTAL_DEFAULTDELTA "defaultDelta"
#define PS_XML_FRAKTAL_PENCOLOR "penColor"
#define PS_XML_FRAKTAL_ISPLUGIN "isPlugin"
#define PS_XML_FRAKTAL_PLUGINPATH "pluginPath"
#define PS_XML_FRAKTAL_ISJULIASET "isJuliaSet"
#define PS_XML_FRAKTAL_ISDOUBLEINPUT "isDoubleInput"
#define PS_XML_FRAKTAL_ISINVERTEDDOUBLEINPUT "isInvertedDoubleInput"
#define PS_XML_FRAKTAL_FORWARDFUNCTIONS "forwardFunctions"

class XMLParser
{
public:
    XMLParser();

    static bool validate(QIODevice* device);
    static QList<Fraktal*>* getFraktals(QIODevice* device);//Achtung New == QList, Delete nicht vergessen
    static bool writeFraktals(QIODevice* device, QList<Fraktal*>& fraktals);

private:
    static bool newDocument(QIODevice* device, QDomDocument* document, bool opened);
    static bool createDocument(QIODevice* device, QDomDocument* document, bool opened);
    static bool attachFile(QIODevice* device, QDomDocument* document);
};

#endif // XMLPARSER_H
