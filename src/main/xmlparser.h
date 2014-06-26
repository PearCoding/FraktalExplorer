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
