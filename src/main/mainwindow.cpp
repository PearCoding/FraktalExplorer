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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "config.h"
#include "algorithmdialog.h"
#include "databankdialog.h"
#include "xmlparser.h"
#include "helpdialog.h"

#include <QSettings>
#include <QFileDialog>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuDocks->addAction(ui->dockWidget->toggleViewAction());
    ui->menuToolbars->addAction(ui->toolBar->toggleViewAction());
    ui->menuToolbars->addAction(ui->toolBar_2->toggleViewAction());
    createLanguageMenu();

    ui->label_9->setVisible(false);
    ui->label_10->setVisible(false);
    ui->doubleSpinBox->setVisible(false);
    ui->spinBox->setVisible(false);
    ui->line_5->setVisible(false);

    readSettings();

    if(!XMLParser::validate(&mMainXMLFile))
    {
        qFatal("Couldn't init Fraktal Explorer");
    }

    QList<Fraktal*>* list = XMLParser::getFraktals(&mMainXMLFile);

    fraktals = *list;
    if(list)
    {
        /*qDebug() << "Fraktals:";
        foreach(Fraktal* fraktal, *list)
        {
            qDebug() << fraktal->name << fraktal->algorithm;
        }*/

        delete list;
    }

    if(fraktals.isEmpty())
    {
        QMessageBox::warning(this, tr("No available fraktals"), tr("No fraktal is available.\nCreate a new one."));
        ui->actionDetails->setEnabled(false);
    }
    else
    {
        currentFraktal = fraktals.first();
        ui->widget->setFraktal(currentFraktal);
    }

    rebuildComboBox();

    ui->widget->setAlgebraParserPath(maximaPath);

    connect(ui->actionFullscreen, SIGNAL(toggled(bool)), this, SLOT(slFullscreen(bool)));
    connect(ui->actionSave_Picture, SIGNAL(triggered()), this, SLOT(slSavePicture()));

    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionAbout_Fraktal_Explorer, SIGNAL(triggered()), this, SLOT(slAboutFraktalExplorer()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(slHelp()));

    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(slComboBox(int)));

    connect(ui->actionNew_algorithm, SIGNAL(triggered()), this, SLOT(slNewAlgorithm()));
    connect(ui->actionDetails, SIGNAL(triggered()), this, SLOT(slShowDetails()));
    connect(ui->actionDatabank, SIGNAL(triggered()), this, SLOT(slDatabank()));
    connect(ui->actionSave_databank, SIGNAL(triggered()), this, SLOT(slSaveDatabank()));

    connect(ui->widget, SIGNAL(renderBegin()), this, SLOT(slRenderBegin()));
    connect(ui->widget, SIGNAL(renderFinished()), this, SLOT(slRenderFinished()));

    connect(actionSystem, SIGNAL(toggled(bool)), this, SLOT(slChangeLanguageSystem(bool)));
    connect(actionEnglish, SIGNAL(toggled(bool)), this, SLOT(slChangeLanguageEnglish(bool)));
    connect(actionGerman, SIGNAL(toggled(bool)), this, SLOT(slChangeLanguageGerman(bool)));
    connect(actionTurkish, SIGNAL(toggled(bool)), this, SLOT(slChangeLanguageTurkish(bool)));

    changeLanguage(usedLocale.language());
}

MainWindow::~MainWindow()
{
    qDeleteAll(fraktals);

    delete languageGroup;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    emit closed();

    event->accept();
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LocaleChange)
    {
        changeLanguage(QLocale::system().language());
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("General");
    usedLocale = settings.value("language", QLocale::system()).toLocale();

    //qDebug() << usedLocale.name();
    switch(usedLocale.language())
    {
    case QLocale::English:
        actionEnglish->setChecked(true);
        break;
    case QLocale::German:
        actionGerman->setChecked(true);
        break;
    case QLocale::Turkish:
        actionTurkish->setChecked(true);
        break;
    default:
        actionSystem->setChecked(true);
        break;
    }

    maximaPath = settings.value("maxima", "").toString();//TODO If false disable it
    settings.endGroup();

    settings.beginGroup("MainWindow");
    this->restoreGeometry(settings.value("geometry").toByteArray());
    this->restoreState(settings.value("state").toByteArray());
    settings.endGroup();

    if(this->isFullScreen())
    {
        ui->actionFullscreen->setChecked(true);
    }

    settings.beginGroup("XML");
	mMainXMLFile.setFileName(settings.value("file", QString(FE_APP_INSTALL_PATH "/settings.xml")).toString());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("General");
    settings.setValue("language", usedLocale);
    settings.endGroup();

    settings.beginGroup("MainWindow");
    settings.setValue("geometry", this->saveGeometry());
    settings.setValue("state", this->saveState());
    settings.endGroup();
}

void MainWindow::slFullscreen(bool b)
{
    if(b)
    {
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
    }
}

void MainWindow::slRenderBegin()
{
    ui->widget->setCursor(Qt::BusyCursor);
    ui->statusBar->showMessage(tr("Rendering..."));
    ui->actionStop->setEnabled(true);
    renderTime.restart();
}

void MainWindow::slRenderFinished()
{
    ui->widget->setCursor(Qt::ArrowCursor);
    ui->actionStop->setEnabled(false);
    ui->statusBar->showMessage(QString("Finished (Render time: %1 ms)...").arg(renderTime.elapsed()), 10000);
}

void MainWindow::slSavePicture()
{
    if(!ui->widget->getPixmap()->isNull())
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Picture to..."), QDir::homePath(), "PNG (*.png)");

        if(ui->widget->getPixmap()->save(fileName, "PNG"))
        {
            ui->statusBar->showMessage(tr("Picture saved to %1 successful").arg(fileName), 10000);
        }
        else
        {
            ui->statusBar->showMessage(tr("Failed to save picture %1").arg(fileName), 10000);
        }
    }
    else
    {
        ui->statusBar->showMessage(tr("Picture is null!"), 10000);
    }
}

void MainWindow::slComboBox(int index)
{
    if(ui->comboBox->count()-1 == index)
    {
        slNewAlgorithm();
    }
    else
    {
        if(fraktals.count() < index)
        {
            QMessageBox::critical(this, tr("Error"), tr("Unable to choose fraktal, index is too high"));
            ui->statusBar->showMessage(tr("Unable to choose fraktal, index is too high"));
            return;
        }

        currentFraktal = fraktals.at(index);
        ui->widget->setFraktal(currentFraktal);

        if(currentFraktal->getIsLSystem())
        {
            ui->label_9->setVisible(true);
            ui->label_10->setVisible(true);
            ui->doubleSpinBox->setVisible(true);
            ui->spinBox->setVisible(true);
            ui->line_5->setVisible(true);

            if(ui->spinBox_5->value() > 5)
            {
                if(QMessageBox::warning(this, tr("Iteration may be too hight"), tr("Your choosed iteration value is very high, it could be very slow."), QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Cancel)
                {
                    return;
                }
            }

            if(ui->spinBox_6->value() < 30)
            {
                QMessageBox::warning(this, tr("Limit may be too low"), tr("Your choosed limit value is very low, you could see nothing."), QMessageBox::Ok);
            }
        }
        else
        {
            ui->label_9->setVisible(false);
            ui->label_10->setVisible(false);
            ui->doubleSpinBox->setVisible(false);
            ui->spinBox->setVisible(false);
            ui->line_5->setVisible(false);
        }
    }
}

void MainWindow::slNewAlgorithm()
{
    AlgorithmDialog dialog(tr("New algorithm"), tr("Create"), 0, this);
    dialog.show();
    dialog.exec();

    if(dialog.getCurrentFraktal())
    {
        currentFraktal = dialog.getCurrentFraktal();
        //qDebug() << "New created algorithm" << currentFraktal->name;
        fraktals.append(currentFraktal);
        ui->widget->setFraktal(currentFraktal);
        ui->widget->render();

        rebuildComboBox();

        if(dialog.isWritingAllowed())
        {
            writeXML();
        }
    }
}

void MainWindow::slShowDetails()
{
    if(currentFraktal)
    {
        AlgorithmDialog dialog(QString("Details %1").arg(currentFraktal->getName()), tr("Change"), currentFraktal, this);
        dialog.show();
        dialog.exec();

        ui->widget->render();

        rebuildComboBox();

        if(dialog.isWritingAllowed())
        {
            writeXML();
        }
    }
}

void MainWindow::slAboutFraktalExplorer()
{
    QMessageBox::about(this, tr("About Fraktal Explorer"), tr("<b>About Fraktal Explorer</b><p>Fraktal Explorer is a viewer and analyser for fractals, which can be created with normal algebra or the L-System.</p><p>Author: &Ouml;mercan Yazici<br/>Copyright (C): PearCoding, &Ouml;mercan Yazici 2010-2014</p>"));
}

void MainWindow::rebuildComboBox()
{
    ui->comboBox->clear();

    foreach(Fraktal* fraktal, fraktals)
    {
        ui->comboBox->addItem(fraktal->getName());
    }

    ui->comboBox->insertSeparator(ui->comboBox->count());
    ui->comboBox->addItem(tr("new algorithm..."));

    if(currentFraktal)
    {
        ui->comboBox->setCurrentIndex(fraktals.indexOf(currentFraktal));
    }

    if(currentFraktal->getIsLSystem())
    {
        ui->label_9->setVisible(true);
        ui->label_10->setVisible(true);
        ui->doubleSpinBox->setVisible(true);
        ui->spinBox->setVisible(true);
        ui->line_5->setVisible(true);
    }
    else
    {
        ui->label_9->setVisible(false);
        ui->label_10->setVisible(false);
        ui->doubleSpinBox->setVisible(false);
        ui->spinBox->setVisible(false);
        ui->line_5->setVisible(false);
    }
}

void MainWindow::slDatabank()
{
    DatabankDialog dialog(&fraktals);

    dialog.show();
    dialog.exec();

    rebuildComboBox();
    writeXML();
}

void MainWindow::writeXML()
{
    if(!mMainXMLFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Couldn't open file"));
        ui->statusBar->showMessage(tr("Couldn't open file"));
    }

    if(!XMLParser::writeFraktals(&mMainXMLFile, fraktals))
    {
        QMessageBox::critical(this, tr("Error"), tr("Unable write fraktal to global databank"));
        ui->statusBar->showMessage(tr("Unable write fraktal to global databank"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Fraktal is appened to global databank successful"));
    }
    mMainXMLFile.close();
}

void MainWindow::changeLanguage(QLocale::Language language)
{
    QLocale loc(language);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + loc.name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);

    QTranslator translator;
	translator.load("FraktalExplorer_" + loc.name(), QString(FE_APP_INSTALL_PATH));//TODO
    qApp->installTranslator(&translator);

    //qDebug() << "Retranslate to" << "FraktalExplorer_" + loc.name();
    usedLocale = loc;
    QLocale::setDefault(usedLocale);
    ui->retranslateUi(this);
}

void MainWindow::slChangeLanguageSystem(bool b)
{
    if(b)
    {
        changeLanguage(QLocale::system().language());
    }
}

void MainWindow::slChangeLanguageEnglish(bool b)
{
    if(b)
    {
        changeLanguage(QLocale::English);
    }
}

void MainWindow::slChangeLanguageGerman(bool b)
{
    if(b)
    {
        changeLanguage(QLocale::German);
    }
}

void MainWindow::slChangeLanguageTurkish(bool b)
{
    if(b)
    {
        changeLanguage(QLocale::Turkish);
    }
}

void MainWindow::createLanguageMenu()
{
    actionSystem = new QAction(tr("System"), this);
    actionSystem->setCheckable(true);

    actionSeperatorLanguage = new QAction(this);
    actionSeperatorLanguage->setSeparator(true);

    actionEnglish = new QAction(tr("English"), this);
    actionEnglish->setCheckable(true);

    actionGerman = new QAction(tr("German"), this);
    actionGerman->setCheckable(true);

    actionTurkish = new QAction(tr("Turkish"), this);
    actionTurkish->setCheckable(true);

    languageGroup = new QActionGroup(ui->menuLanguage);
    languageGroup->addAction(actionSystem);
    languageGroup->addAction(actionSeperatorLanguage);
    languageGroup->addAction(actionEnglish);
    languageGroup->addAction(actionGerman);
    languageGroup->addAction(actionTurkish);
    actionSystem->setChecked(true);

    ui->menuLanguage->addAction(actionSystem);
    ui->menuLanguage->addAction(actionSeperatorLanguage);
    ui->menuLanguage->addAction(actionEnglish);
    ui->menuLanguage->addAction(actionGerman);
    ui->menuLanguage->addAction(actionTurkish);
}

void MainWindow::slHelp()
{
    HelpDialog dialog;
    dialog.show();
    dialog.exec();
}
