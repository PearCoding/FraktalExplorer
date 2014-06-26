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

#include "fraktalwidget.h"

#include <QtGui>
#include <math.h>

const double DefaultCenterX = -0.637011f;
const double DefaultCenterY = -0.0395159f;
const double DefaultScale = 0.00403897f;

const int ScrollStep = 20;

FraktalWidget::FraktalWidget(QWidget *parent) :
        QWidget(parent), zoomFaktor(0.8f), currentFraktal(0), antialiasing(false)
{
    centerX = DefaultCenterX;
    centerY = DefaultCenterY;
    pixmapScale = DefaultScale;
    curScale = DefaultScale;

    emit centerXChanged(centerX);
    emit centerYChanged(centerY);
    emit scaleChanged(curScale);

    qRegisterMetaType<QImage>("QImage");
    connect(&thread, SIGNAL(renderedImage(QImage,double)), this, SLOT(updatePixmap(QImage,double)));

    connect(&thread, SIGNAL(renderBegin()), this, SIGNAL(renderBegin()));
    connect(&thread, SIGNAL(renderFinished()), this, SIGNAL(renderFinished()));

#ifndef QT_NO_CURSOR
    setCursor(Qt::CrossCursor);
#endif
}

FraktalWidget::~FraktalWidget()
{
    thread.terminate();
}

void FraktalWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    if(antialiasing)
    {
        painter.setRenderHint(QPainter::Antialiasing);
    }

    painter.fillRect(rect(), Qt::black);

    if (pixmap.isNull())
    {
        return;
    }

    if (curScale == pixmapScale)
    {
        painter.drawPixmap(pixmapOffset, pixmap);
    }
    else
    {
        double scaleFactor = pixmapScale / curScale;
        int newWidth = int(pixmap.width() * scaleFactor);
        int newHeight = int(pixmap.height() * scaleFactor);
        int newX = pixmapOffset.x() + (pixmap.width() - newWidth) / 2;
        int newY = pixmapOffset.y() + (pixmap.height() - newHeight) / 2;
        painter.save();
        painter.translate(newX, newY);
        painter.scale(scaleFactor, scaleFactor);
        QRectF exposed = painter.matrix().inverted().mapRect(rect()).adjusted(-1, -1, 1, 1);
        painter.drawPixmap(exposed, pixmap, exposed);
        painter.restore();
    }

    if(this->hasFocus())
    {
        QPen tmp = painter.pen();

        QPen pen(Qt::green, 5);
        painter.setPen(pen);

        painter.drawRect(rect());
        painter.setPen(tmp);
    }
}
void FraktalWidget::resizeEvent(QResizeEvent * /* event */)
{
    render();
}

void FraktalWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
    case Qt::Key_PageUp:
        zoom(zoomFaktor);
        break;
    case Qt::Key_Minus:
    case Qt::Key_PageDown:
        zoom(1/zoomFaktor);
        break;
    case Qt::Key_Left:
        scroll(-ScrollStep, 0);
        break;
    case Qt::Key_Right:
        scroll(ScrollStep, 0);
        break;
    case Qt::Key_Down:
        scroll(0, ScrollStep);
        break;
    case Qt::Key_Up:
        scroll(0, -ScrollStep);
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void FraktalWidget::wheelEvent(QWheelEvent *event)
{
    /*int deltaX = -(width() - pixmap.width()) / 2 + event->pos().x();
    int deltaY = -(height() - pixmap.height()) / 2 + event->pos().y();
    scroll(deltaX, deltaY);*/

    int numDegrees = event->delta() / 8;
    double numSteps = numDegrees / 15.0f;
    zoom(pow(zoomFaktor, numSteps));
}

void FraktalWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastDragPos = event->pos();
    }

    this->setFocus(Qt::MouseFocusReason);
}

void FraktalWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        pixmapOffset += event->pos() - lastDragPos;
        lastDragPos = event->pos();
        update();
    }
}

void FraktalWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pixmapOffset += event->pos() - lastDragPos;
        lastDragPos = QPoint();

        int deltaX = (width() - pixmap.width()) / 2 - pixmapOffset.x();
        int deltaY = (height() - pixmap.height()) / 2 - pixmapOffset.y();
        scroll(deltaX, deltaY);
    }
}

void FraktalWidget::updatePixmap(const QImage &image, double scaleFactor)
{
    if (!lastDragPos.isNull())
        return;

    pixmap = QPixmap::fromImage(image);
    pixmapOffset = QPoint();
    lastDragPos = QPoint();
    pixmapScale = scaleFactor;
    update();
}

void FraktalWidget::zoom(double zoomFactor)
{
    curScale *= zoomFactor;
    emit scaleChanged(curScale);
    render();
}

void FraktalWidget::scroll(int deltaX, int deltaY)
{
    centerX += deltaX * curScale;
    centerY += deltaY * curScale;
    emit centerXChanged(centerX);
    emit centerYChanged(centerY);

    render();
}


void FraktalWidget::setCenterX(double x)
{
    centerX = x;
    render();
}

void FraktalWidget::setCenterY(double y)
{
    centerY = y;
    render();
}

void FraktalWidget::setScale(double s)
{
    curScale = s;
    render();
}

void FraktalWidget::setZoomFaktor(double f)
{
    zoomFaktor = f;
}

void FraktalWidget::setFraktal(Fraktal* fraktal)
{
    currentFraktal = fraktal;
    render();
}

void FraktalWidget::render()
{
    update();
    thread.render(currentFraktal, centerX, centerY, curScale, size());
}

void FraktalWidget::setAntialiasing(bool b)
{
    antialiasing = b;
    thread.setAntialiasing(b);
    render();
}

void FraktalWidget::setMaxIterations(int i)
{
    thread.setMaxIterations(i);
    render();
}

void FraktalWidget::setLimit(int i)
{
    thread.setLimit(i);
    render();
}

void FraktalWidget::stopRendering()
{
    thread.terminate();
    emit renderFinished();
}
