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

#ifndef LSYSTEM_PARSER_H
#define LSYSTEM_PARSER_H

#include <QPoint>
#include <QString>
#include <QMap>
#include <QPainterPath>

class LSystem_Parser
{
public:
    LSystem_Parser(QString string, QPainter* painter, double length = 0.1, int delta = 30, QPointF position = QPointF());

    inline void setLength(double d)
    {
        length = d;
    }

    inline double getLength() const
    {
        return length;
    }

    inline void setDelta(int i)
    {
        delta = i;
    }

    inline int getDelta() const
    {
        return delta;
    }

    inline void addRule(QChar name, QString rule)
    {
        rules.insert(name, rule);
    }

    void parse(QList<QChar>& forwardFunctions, int maxIteration, int maxDeep = 20);

private:
    struct PosContext
    {
        int delta;
        QPointF position;
    };

    QString string;
    double length;
    int delta;
    int currentDelta;
    QPointF startPosition;
    QPointF position;
    QPainter* painter;
    QPainterPath path;
    QMap<QChar, QString> rules;
    QList<PosContext> stack;

    void turnLeft();
    void turnRight();
    void forward();
    void backward();
    void onlyMove();
    void turn180();

    QString getFunction(QChar name);
    void checkFunctions();
    void iterateString();
    void executeString(int maxDeep, QList<QChar>& forwardFunctions);
};

#endif // LSYSTEM_PARSER_H
