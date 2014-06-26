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

#include "xmlparser.h"

#include <QMessageBox>
#include <QDomProcessingInstruction>
#include <QDebug>

XMLParser::XMLParser()
{
}

bool XMLParser::newDocument(QIODevice *device, QDomDocument *document, bool opened)
{
    return true;
}

bool XMLParser::createDocument(QIODevice *device, QDomDocument *document, bool opened)
{
    QString errorString;
    int errorLine;
    int errorColumn;

    if(!document->setContent(device, true, &errorString, &errorLine, &errorColumn))
    {
        if(opened)
        {
            device->close();
        }

        QMessageBox::warning(0, QString("XML Parser"), QString("Parse error at line %1, column %2:\n%3")
                                          .arg(errorLine)
                                          .arg(errorColumn)
                                          .arg(errorString));
        return false;
    }

    return true;
}

bool XMLParser::attachFile(QIODevice *device, QDomDocument *document)
{
    bool opened = false;
    if(!device->isOpen())
    {
        if(!device->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(0, QString("XML Parser"), QString("Couldn't open file."));
            return false;
        }

        opened = true;
    }

    if(!createDocument(device, document, opened))
    {
        return false;
    }

    if(opened)
    {
        //device->seek(0);
        //qDebug("\n%s", device->readAll().constData());

        device->close();
    }

    return true;
}

bool XMLParser::validate(QIODevice *device)
{
    QDomDocument document;

    if(!attachFile(device, &document))
    {
        return false;
    }

    QDomElement root = document.documentElement();
    if (root.tagName() != PS_XML_HEADER)
    {
        QMessageBox::warning(0, QString("XML Parser"), QString("The file is not an " PS_XML_HEADER " file."));
        return false;
    }
    else if (root.hasAttribute("version") && root.attribute("version") != PS_XML_VERSION)
    {
        QMessageBox::warning(0, QString("XML Parser"), QString("The file is not an " PS_XML_HEADER " version " PS_XML_VERSION " file."));
        return false;
    }

    return true;
}

QList<Fraktal*>* XMLParser::getFraktals(QIODevice *device)
{
    QList<Fraktal*>* list = new QList<Fraktal*>;

    if(!list)
    {
        return 0;
    }

    QDomDocument document;
    if(!attachFile(device, &document))
    {
        return 0;
    }

    QDomElement root = document.documentElement();
    QDomNodeList nodeList = root.childNodes();

    for(int i = 0; i < nodeList.count(); i++)
    {
        QDomNode node = nodeList.at(i);

        if(node.nodeName() == PS_XML_FRAKTAL)
        {
            QDomNode fraktal = nodeList.at(i);

            Fraktal* fr = new Fraktal;

            if(!fr)
            {
                return false;
            }

            QDomNamedNodeMap map = fraktal.attributes();

            QDomNode attribute = map.namedItem(PS_XML_FRAKTAL_NAME);
            fr->setName(attribute.nodeValue());

            attribute = map.namedItem(PS_XML_FRAKTAL_ISLSYSTEM);
            fr->setIsLSystem(attribute.nodeValue().toInt());

            if(fr->getIsLSystem())
            {
                attribute = map.namedItem(PS_XML_FRAKTAL_DEFAULTLENGTH);
                fr->setDefaultLength(attribute.nodeValue().toDouble());

                attribute = map.namedItem(PS_XML_FRAKTAL_DEFAULTDELTA);
                fr->setDefaultDelta(attribute.nodeValue().toInt());

                attribute = map.namedItem(PS_XML_FRAKTAL_PENCOLOR);
                fr->setPenColor(attribute.nodeValue());

                attribute = map.namedItem(PS_XML_FRAKTAL_FORWARDFUNCTIONS);
                QList<QChar> forwardFunctions;
                QStringList list = attribute.nodeValue().split(',');
                foreach(QString s, list)
                {
                    if(s.count() == 1)
                    {
                        forwardFunctions.append(s[0]);
                    }
                }
                fr->setForwardFunctions(forwardFunctions);
            }
            else
            {
                attribute = map.namedItem(PS_XML_FRAKTAL_ISJULIASET);
                fr->setIsJuliaSet(attribute.nodeValue().toInt());
                attribute = map.namedItem(PS_XML_FRAKTAL_ISDOUBLEINPUT);
                fr->setIsDoubleInput(attribute.nodeValue().toInt());

                if(fr->getIsDoubleInput())
                {
                    attribute = map.namedItem(PS_XML_FRAKTAL_ISINVERTEDDOUBLEINPUT);
                    fr->setIsInvertedDoubleInput(attribute.nodeValue().toInt());
                }
            }

            attribute = map.namedItem(PS_XML_FRAKTAL_ISPLUGIN);
            fr->setIsPlugin(attribute.nodeValue().toInt());

            if(fr->getIsPlugin())
            {
                attribute = map.namedItem(PS_XML_FRAKTAL_PLUGINPATH);
                fr->setPluginPath(attribute.nodeValue());
            }

            QDomNodeList fraktalChildList = fraktal.childNodes();

            for(int a = 0; a < fraktalChildList.count(); a++)
            {
                if(fraktalChildList.at(a).isText())
                {
                    QDomNode fraktalAlgorithm = fraktalChildList.at(a);
                    fr->setAlgorithm(fraktalAlgorithm.nodeValue());
                }
            }
            //qDebug() << "Name" << fr->getName() << "Notes" << fr->getNotes().count();

            list->append(fr);
        }
    }

    return list;
}

bool XMLParser::writeFraktals(QIODevice *device, QList<Fraktal*>& fraktals)
{
    QDomDocument document;

    QDomProcessingInstruction header = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"" );
    document.appendChild(header);

    QDomElement root = document.createElement(PS_XML_HEADER);
    root.setAttribute("version", PS_XML_VERSION);
    document.appendChild(root);

    foreach(Fraktal* fraktal, fraktals)
    {
        QDomElement fr = document.createElement(PS_XML_FRAKTAL);
        fr.setAttribute(PS_XML_FRAKTAL_NAME, fraktal->getName());
        fr.setAttribute(PS_XML_FRAKTAL_ISLSYSTEM, fraktal->getIsLSystem());

        if(fraktal->getIsLSystem())
        {
            fr.setAttribute(PS_XML_FRAKTAL_DEFAULTLENGTH, fraktal->getDefaultLength());
            fr.setAttribute(PS_XML_FRAKTAL_DEFAULTDELTA, fraktal->getDefaultDelta());
            fr.setAttribute(PS_XML_FRAKTAL_PENCOLOR, fraktal->getPenColor().name());

            QString tmp;
            foreach(QChar c, fraktal->getForwardFunctions())
            {
                tmp += c;
                tmp += ',';
            }
            fr.setAttribute(PS_XML_FRAKTAL_FORWARDFUNCTIONS, tmp);
        }
        else
        {
            fr.setAttribute(PS_XML_FRAKTAL_ISJULIASET, fraktal->getIsJuliaSet());
            fr.setAttribute(PS_XML_FRAKTAL_ISDOUBLEINPUT, fraktal->getIsDoubleInput());

            if(fraktal->getIsDoubleInput())
            {
                fr.setAttribute(PS_XML_FRAKTAL_ISINVERTEDDOUBLEINPUT, fraktal->getIsInvertedDoubleInput());
            }
        }

        fr.setAttribute(PS_XML_FRAKTAL_ISPLUGIN, fraktal->getIsPlugin());

        if(fraktal->getIsPlugin())
        {
            fr.setAttribute(PS_XML_FRAKTAL_PLUGINPATH, fraktal->getPluginPath());
        }

        QDomText algorithm = document.createTextNode(fraktal->getAlgorithm());
        fr.appendChild(algorithm);
        root.appendChild(fr);
    }

    if(device->isOpen() && device->isWritable())
    {
        device->write(document.toString().toUtf8());
        return true;
    }
    else
    {
        return false;
    }
}
