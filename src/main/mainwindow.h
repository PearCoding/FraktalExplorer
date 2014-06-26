#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QFile>
#include <QActionGroup>
#include <QLocale>
#include <QTime>

#include "fraktale.h"
#include "pluginmanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* event);
    void changeEvent(QEvent *event);

private:
    Ui::MainWindow *ui;

    Fraktal* currentFraktal;
    QList<Fraktal*> fraktals;
    QFile mMainXMLFile;

    QTime renderTime;

    QString maximaPath;

    PluginManager pluginManager;

    QLocale usedLocale;
    QActionGroup* languageGroup;
    QAction* actionSystem;
    QAction* actionSeperatorLanguage;
    QAction* actionEnglish;
    QAction* actionGerman;
    QAction* actionTurkish;

    void readSettings();
    void writeSettings();
    void rebuildComboBox();
    void writeXML();

    void createLanguageMenu();
    void changeLanguage(QLocale::Language language);

private slots:
    void slFullscreen(bool b);

    void slSavePicture();

    void slAboutFraktalExplorer();

    void slComboBox(int index);

    void slNewAlgorithm();
    void slShowDetails();
    void slDatabank();
    inline void slSaveDatabank()
    {
        writeXML();
    }

    void slHelp();

    void slRenderBegin();
    void slRenderFinished();

    void slChangeLanguageSystem(bool b);
    void slChangeLanguageEnglish(bool b);
    void slChangeLanguageGerman(bool b);
    void slChangeLanguageTurkish(bool b);
};

#endif // MAINWINDOW_H
