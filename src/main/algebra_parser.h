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

#ifndef ALGEBRA_PARSER_H
#define ALGEBRA_PARSER_H

#include <QObject>
#include <QProcess>

class Algebra_Parser : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        Running,
        Stopped,
        Error
    };

    explicit Algebra_Parser(QString path = "", QObject *parent = 0);
    ~Algebra_Parser();

    void start();
    void stop();

    inline void restart()
    {
        stop();
        start();
    }

    inline void setPath(QString path)
    {
        this->path = path;
    }

    inline QString getPath() const
    {
        return path;
    }

    inline State getState() const
    {
        return processState;
    }

    QString executeExpression(QString expr);

    void flush();

signals:

protected:
    QString postOutputOperation(QString output);

private slots:
    void computeError(QProcess::ProcessError error);
    void finished(int exitCode);
    void started();
    void processEmitError();
    void processEmitOutput();//Debug

private:
    QString path;
    QProcess process;
    State processState;
};

#endif // ALGEBRA_PARSER_H
