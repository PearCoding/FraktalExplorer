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

#include <QApplication>
#include "mainwindow.h"

#include <QSplashScreen>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>

#include <stdio.h>
#include <stdlib.h>

#include "config.h"

int main(int argc, char *argv[])
{
#ifdef Q_WS_X11
    bool useGUI = getenv("DISPLAY") != 0;
    if (!useGUI)
    {
        fprintf(stderr, "Fatal: You need a graphical window manager to work with this program.\n");
        return 1;
    }
#endif

    QApplication a(argc, argv);

    QString locale = QLocale::system().name();

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator translator;
	translator.load("FraktalExplorer_" + locale, QString(FE_APP_INSTALL_PATH));//TODO
    a.installTranslator(&translator);

    //qDebug("FraktalExplorer_%s", locale.toLatin1().constData());

    a.setApplicationName("Fractal Explorer");
    a.setApplicationVersion("0.1");
    a.setOrganizationName("PearCoding");

    QFile styleSheet(":/style/style.qss");

    if(styleSheet.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(styleSheet.readAll());
    }
    else
    {
        fprintf(stderr, "Error: Couldn't find stylesheet.\n");
    }

    QPixmap pixmap(":/SplashScreen/images/SplashScreen.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QSplashScreen::tr("Loading..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    a.processEvents();

    MainWindow w;
    w.show();
    splash.finish(&w);

    QApplication::connect(&w, SIGNAL(closed()), qApp, SLOT(quit()));

    return a.exec();
}
