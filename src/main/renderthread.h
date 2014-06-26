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

#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QSize>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>

#include "fraktale.h"
#include "algebra_parser.h"

class RenderThread : public QThread
{
    Q_OBJECT
public:
    typedef bool (*FraktalAlgorithm)(int &iteration, const int limit, double &ax, double &ay, double &a, double &b);

    explicit RenderThread(QObject *parent = 0);
    ~RenderThread();

    inline void setMaxIterations(int i)
    {
        mutex.lock();
        maxIterations = i;
        mutex.unlock();
    }

    inline void setLimit(int i)
    {
        mutex.lock();
        limit = i;
        mutex.unlock();
    }

    inline void setAntialiasing(bool b)
    {
        mutex.lock();
        antialiasing = b;
        mutex.unlock();
    }

    inline void setAlgebraParserPath(QString path)
    {
        mutex.lock();
        alParser.setPath(path);

        if(alParser.getState() == Algebra_Parser::Running)
        {
            alParser.restart();
        }

        mutex.unlock();
    }

    void render(Fraktal* fraktal, double centerX, double centerY, double scaleFactor, QSize resultSize);
signals:
    void renderedImage(const QImage &image, double scaleFactor);
    void renderBegin();
    void renderFinished();

protected:
    void run();

private:
    uint rgbFromWaveLength(double wave);

    QMutex mutex;
    QWaitCondition condition;
    double centerX;
    double centerY;
    double scaleFactor;
    QSize resultSize;
    bool restart;
    bool abort;
    bool antialiasing;

    int maxIterations;
    int limit;

    Algebra_Parser alParser;

    enum { ColormapSize = 512 };
    uint colormap[ColormapSize];

    Fraktal* currentFraktal;

    void calculate(Fraktal* currentFraktal, QSize resultSize, double scaleFactor, double centerX, double centerY);
};

#endif // RENDERTHREAD_H
