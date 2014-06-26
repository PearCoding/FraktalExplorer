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
