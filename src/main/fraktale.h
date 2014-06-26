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

#ifndef FRAKTALE_H
#define FRAKTALE_H

#include <QString>
#include <QColor>
#include <QMutex>
#include <QMutexLocker>

#define FRAKTALEXPLORER_GETMETHODE(type, name, var) \
inline type get##name() \
{ \
  QMutexLocker locker(&mutex); \
  return var; \
}

#define FRAKTALEXPLORER_SETMETHODE(type, name, var) \
inline void set##name(type v) \
{ \
  mutex.lock(); \
  this->var = v; \
  mutex.unlock(); \
}

#define FRAKTALEXPLORER_DEFINEVARIABLE(type, name) \
public: \
    FRAKTALEXPLORER_GETMETHODE(type, name, name) \
    FRAKTALEXPLORER_SETMETHODE(type, name, name) \
private: \
    type name;

class Fraktal
{
public:
    Fraktal() : IsJuliaSet(false), IsDoubleInput(false), IsInvertedDoubleInput(false), IsPlugin(false), IsLSystem(false), DefaultDelta(0), DefaultLength(0), PenColor(Qt::white), Algorithm(""), Name("Unknown"), PluginPath("") {}

    ~Fraktal()
    {
    }

    inline void setForwardFunctions(QList<QChar>& f)
    {
        mutex.lock();
        this->ForwardFunctions = f;
        mutex.unlock();
    }

    inline QList<QChar>& getForwardFunctions()
    {
        QMutexLocker locker(&mutex);

        return this->ForwardFunctions;
    }

private:
    QMutex mutex;

    FRAKTALEXPLORER_DEFINEVARIABLE(bool, IsJuliaSet)
    FRAKTALEXPLORER_DEFINEVARIABLE(bool, IsDoubleInput)
    FRAKTALEXPLORER_DEFINEVARIABLE(bool, IsInvertedDoubleInput)
    FRAKTALEXPLORER_DEFINEVARIABLE(bool, IsPlugin)
    FRAKTALEXPLORER_DEFINEVARIABLE(bool, IsLSystem)
    FRAKTALEXPLORER_DEFINEVARIABLE(int, DefaultDelta)
    FRAKTALEXPLORER_DEFINEVARIABLE(double, DefaultLength)
    FRAKTALEXPLORER_DEFINEVARIABLE(QColor, PenColor)
    FRAKTALEXPLORER_DEFINEVARIABLE(QString, Algorithm)
    FRAKTALEXPLORER_DEFINEVARIABLE(QString, Name)
    FRAKTALEXPLORER_DEFINEVARIABLE(QString, PluginPath)
    QList<QChar> ForwardFunctions;
};


#endif // FRAKTALE_H
