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

#include "algebra_parser.h"

#include <QDebug>
#include <QMetaType>

Algebra_Parser::Algebra_Parser(QString path, QObject *parent) :
    QObject(parent), path(path), process(this), processState(Stopped)
{
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");

    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(computeError(QProcess::ProcessError)));
    connect(&process, SIGNAL(finished(int)), this, SLOT(finished(int)));
    connect(&process, SIGNAL(started()), this, SLOT(started()));
    //connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(processEmitError()));
    //connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(processEmitOutput()));
}

Algebra_Parser::~Algebra_Parser()
{
    stop();
}

void Algebra_Parser::start()
{
    if(path.isEmpty())
    {
        processState = Error;
        qCritical() << "[Algebra_Parser]: Path is empty";
        return;
    }

    if(processState == Running)
    {
        return;
    }

    process.start("\"" + path + "\"");
    process.waitForStarted();

    if(!process.isReadable() || !process.isWritable())
    {
        stop();
        processState = Error;
        qCritical() << "[Algebra_Parser]: The created process is not read or writeable";
    }
}

void Algebra_Parser::stop()
{
    process.terminate();
    process.kill();
}

void Algebra_Parser::computeError(QProcess::ProcessError)
{
    processState = Error;
    qCritical() << "[Algebra_Parser]:" << process.errorString();
    //qDebug() << "System Enviroment:" << QProcess::systemEnvironment();
    qDebug() << "Path:" << path;
}

void Algebra_Parser::finished(int exitCode)
{
    processState = Stopped;
    qDebug() << "[Algebra_Parser]: Process exit with" << exitCode;
}

void Algebra_Parser::started()
{
    processState = Running;
}

void Algebra_Parser::processEmitError()
{
    QString error = process.readAllStandardError();
    if(!error.isEmpty())
    {
        qCritical() << "[Algebra_Parser] INTERN:" << error;
    }
}

void Algebra_Parser::processEmitOutput()
{
    QString output = process.readAllStandardOutput();
    if(!output.isEmpty())
    {
        qDebug() << "[Algebra_Parser] INTERN:" << output;
    }
}

QString Algebra_Parser::executeExpression(QString expr)
{
    if(processState != Running)
    {
        return "Not running";
    }

    process.write(expr.toLatin1());

    if(!process.waitForBytesWritten())
    {
        return "Couldn't write data";
    }

    if(!process.waitForReadyRead())
    {
        return "Couldn't read data";
    }

    QString output = postOutputOperation(process.readAllStandardOutput());
    //qDebug() << "After" << output;
    return output;
}

QString Algebra_Parser::postOutputOperation(QString output)
{
    //qDebug() << "Before" << output;
    output.remove(QRegExp("\\(%o\\d+\\)"));
    output.remove(QRegExp("\\(%i\\d+\\)"));
    output.remove(QRegExp("[ \\n\\r\\t]*"));
    return output;
}

void Algebra_Parser::flush()
{
    process.readAll();
}
