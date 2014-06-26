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

#ifndef ALGORITHMDIALOG_H
#define ALGORITHMDIALOG_H

#include <QDialog>

#include "fraktale.h"

namespace Ui {
    class AlgorithmDialog;
}

class AlgorithmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlgorithmDialog(QString title, QString acceptButtonText, Fraktal* fraktal = 0, QWidget* parent = 0, bool mustAllowed = false);
    ~AlgorithmDialog();

    inline Fraktal* getCurrentFraktal()
    {
        return currentFraktal;
    }

    inline bool isWritingAllowed()
    {
        return writingAllowed;
    }

private:
    Ui::AlgorithmDialog *ui;

    Fraktal* currentFraktal;
    QColor currentColor;
    bool writingAllowed;

private slots:
    void slChangeMode(bool b);
    void slChangeColor();
    void slFinish();

    void slChangePlugin(bool b);
    void slFileDialog();
    void slCheckLibrary();

    void slChangeDoubleInput(bool b);
};

#endif // NEWALGORITHMDIALOG_H
