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
