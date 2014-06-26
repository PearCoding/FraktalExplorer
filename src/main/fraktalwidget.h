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

#ifndef FRAKTALWIDGET_H
#define FRAKTALWIDGET_H

#include <QPixmap>
#include <QWidget>

#include "renderthread.h"

class FraktalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FraktalWidget(QWidget *parent = 0);
    ~FraktalWidget();

    inline QPixmap* getPixmap()
    {
        return &pixmap;
    }

    inline void setAlgebraParserPath(QString path)
    {
        thread.setAlgebraParserPath(path);
    }

    void render();

signals:
    void centerXChanged(double x);
    void centerYChanged(double y);
    void scaleChanged(double s);

    void renderBegin();
    void renderFinished();

public slots:
    void setCenterX(double x);
    void setCenterY(double y);
    void setScale(double s);
    void setZoomFaktor(double f);
    void setFraktal(Fraktal* fraktal);
    void setAntialiasing(bool b);
    void setMaxIterations(int i);
    void setLimit(int i);
    void stopRendering();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void updatePixmap(const QImage &image, double scaleFactor);

private:
    void zoom(double zoomFactor);
    void scroll(int deltaX, int deltaY);

    RenderThread thread;
    QPixmap pixmap;
    QPoint pixmapOffset;
    QPoint lastDragPos;
    double centerX;
    double centerY;
    double pixmapScale;
    double curScale;
    double zoomFaktor;
    Fraktal* currentFraktal;

    bool antialiasing;
};
#endif // FRAKTALWIDGET_H
