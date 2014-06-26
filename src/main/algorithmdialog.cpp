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

#include "algorithmdialog.h"
#include "ui_algorithmdialog.h"

#include <QColorDialog>
#include <QDebug>
#include <QLibrary>
#include <QFileDialog>

AlgorithmDialog::AlgorithmDialog(QString title, QString acceptButtonText, Fraktal* fraktal, QWidget* parent, bool mustAllowed) :
        QDialog(parent),
        ui(new Ui::AlgorithmDialog), currentFraktal(fraktal), writingAllowed(false)
{
    ui->setupUi(this);

    this->setWindowTitle(title);
    ui->pushButton_2->setText(acceptButtonText);

    connect(ui->radioButton, SIGNAL(toggled(bool)), this, SLOT(slChangeMode(bool)));
    connect(ui->checkBox_2, SIGNAL(toggled(bool)), this, SLOT(slChangePlugin(bool)));
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(slChangeColor()));
    connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(slFileDialog()));
    connect(ui->lineEdit_2, SIGNAL(editingFinished()), this, SLOT(slCheckLibrary()));
    connect(ui->checkBox_4, SIGNAL(toggled(bool)), this, SLOT(slChangeDoubleInput(bool)));
    connect(this, SIGNAL(accepted()), this, SLOT(slFinish()));

    ui->checkBox->setEnabled(!mustAllowed);

    if(fraktal)
    {
        ui->lineEdit->setText(fraktal->getName());
        if(fraktal->getIsLSystem())
        {
            ui->radioButton->setChecked(true);
            ui->doubleSpinBox->setValue(fraktal->getDefaultLength());
            ui->spinBox->setValue(fraktal->getDefaultDelta());
            ui->textEdit_2->setText(fraktal->getAlgorithm());

            QString tmp;
            foreach(QChar c, fraktal->getForwardFunctions())
            {
                tmp += c;
                tmp += ',';
            }
            ui->textEdit_3->setText(tmp);

            ui->frame->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(fraktal->getPenColor().red()).arg(fraktal->getPenColor().green()).arg(fraktal->getPenColor().blue()));
            currentColor = fraktal->getPenColor();
            slChangeMode(true);
        }
        else
        {
            ui->textEdit->setText(fraktal->getAlgorithm());
            ui->checkBox_3->setChecked(fraktal->getIsJuliaSet());
            ui->checkBox_4->setChecked(fraktal->getIsDoubleInput());

            if(fraktal->getIsDoubleInput())
            {
                ui->checkBox_5->setChecked(fraktal->getIsInvertedDoubleInput());
            }
        }

        if(fraktal->getIsPlugin())
        {
            ui->checkBox_2->setChecked(true);
            ui->lineEdit_2->setText(fraktal->getPluginPath());
        }
    }
    else
    {
        slChangePlugin(false);
        slChangeDoubleInput(false);
    }
}

AlgorithmDialog::~AlgorithmDialog()
{
    delete ui;
}

void AlgorithmDialog::slChangeMode(bool b)
{
    if(b)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void AlgorithmDialog::slFinish()
{
    if(!currentFraktal)
    {
        currentFraktal = new Fraktal;
    }

    if(currentFraktal)
    {
        //qDebug() << currentFraktal;
        currentFraktal->setName(ui->lineEdit->text());

        currentFraktal->setIsLSystem(ui->radioButton->isChecked());

        if(currentFraktal->getIsLSystem())
        {
            currentFraktal->setDefaultLength(ui->doubleSpinBox->value());
            currentFraktal->setDefaultDelta(ui->spinBox->value());
            currentFraktal->setAlgorithm(ui->textEdit_2->toPlainText());
            currentFraktal->setPenColor(currentColor);

            QList<QChar> forwardFunctions;
            QStringList list = ui->textEdit_3->toPlainText().split(',');
            foreach(QString s, list)
            {
                if(s.count() == 1)
                {
                    forwardFunctions.append(s[0]);
                }
            }
            currentFraktal->setForwardFunctions(forwardFunctions);
        }
        else
        {
            currentFraktal->setAlgorithm(ui->textEdit->toPlainText());
            currentFraktal->setIsJuliaSet(ui->checkBox_3->isChecked());
            currentFraktal->setIsDoubleInput(ui->checkBox_4->isChecked());

            if(currentFraktal->getIsDoubleInput())
            {
                currentFraktal->setIsInvertedDoubleInput(ui->checkBox_5->isChecked());
            }
        }

        currentFraktal->setIsPlugin(ui->checkBox_2->isChecked());

        if(currentFraktal->getIsPlugin())
        {
            currentFraktal->setPluginPath(ui->lineEdit_2->text());
        }

        writingAllowed = ui->checkBox->isChecked();
    }
}

void AlgorithmDialog::slChangeColor()
{
    currentColor = QColorDialog::getColor(currentColor, this, tr("Choose a pen color"));
    ui->frame->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(currentColor.red()).arg(currentColor.green()).arg(currentColor.blue()));
}

void AlgorithmDialog::slChangePlugin(bool b)
{
    if(b)
    {
        ui->label_6->setVisible(true);
        ui->label_8->setVisible(true);
        ui->line_3->setVisible(true);
        ui->line_4->setVisible(true);
        ui->lineEdit_2->setVisible(true);
        ui->toolButton_2->setVisible(true);

        slCheckLibrary();
    }
    else
    {
        ui->label_6->setVisible(false);
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
        ui->line_3->setVisible(false);
        ui->line_4->setVisible(false);
        ui->lineEdit_2->setVisible(false);
        ui->toolButton_2->setVisible(false);
    }
}

void AlgorithmDialog::slFileDialog()
{
    QFileInfo fileInfo(ui->lineEdit_2->text());
    QString lib = QFileDialog::getOpenFileName(this, tr("Choose a plugin"), fileInfo.absolutePath(), tr("Libraries (*.dll *.so)"));

    if(!lib.isEmpty())
    {
        ui->lineEdit_2->setText(lib);
    }
}

void AlgorithmDialog::slCheckLibrary()
{
    if(QLibrary::isLibrary(ui->lineEdit_2->text()))
    {
        ui->label_7->setVisible(true);
    }
    else
    {
        ui->label_7->setVisible(false);
    }
}

void AlgorithmDialog::slChangeDoubleInput(bool b)
{
    if(b)
    {
        ui->line_7->setVisible(true);
        ui->checkBox_5->setVisible(true);
    }
    else
    {
        ui->line_7->setVisible(false);
        ui->checkBox_5->setVisible(false);
    }
}
