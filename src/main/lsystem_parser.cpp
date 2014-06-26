#include "lsystem_parser.h"

#include <QDebug>
#include <QRegExp>
#include <QPainter>

#include <math.h>

LSystem_Parser::LSystem_Parser(QString string, QPainter* painter, double length, int delta, QPointF position) :
        string(string), length(length), delta(delta), currentDelta(0), startPosition(position), position(0,0), painter(painter)
{
    //qDebug() << "Before" << string;
    string.remove(QRegExp("[ \n\t\r]*"));
    //qDebug() << "After" << string;
    path.moveTo(position);
}

void LSystem_Parser::turnLeft()
{
    //qDebug() << "TurnLeft";
    currentDelta += delta;

    if(currentDelta > 360)
    {
        currentDelta -= 360;
    }
}

void LSystem_Parser::turnRight()
{
    //qDebug() << "TurnRight";
    currentDelta -= delta;

    if(currentDelta < -360)
    {
        currentDelta += 360;
    }
}

void LSystem_Parser::forward()
{
    //qDebug() << "Forward";
    double x = position.x() + cos(currentDelta / 180.0 * 3.141592654) * length;
    double y = position.y() + sin(currentDelta / 180.0 * 3.141592654) * length;
    //qDebug() << "dX:" << x << "dY:" << y;
    path.lineTo(x + startPosition.x(), y + startPosition.y());

    position.setX(x);
    position.setY(y);
}

void LSystem_Parser::backward()
{
    //qDebug() << "Backward";
    double x = position.x() - cos(currentDelta / 180.0 * 3.141592654) * length;
    double y = position.y() - sin(currentDelta / 180.0 * 3.141592654) * length;
    //qDebug() << "dX:" << x << "dY:" << y;
    path.lineTo(x + startPosition.x(), y + startPosition.y());

    position.setX(x);
    position.setY(y);
}

void LSystem_Parser::onlyMove()
{
    //qDebug() << "Only move";

    double x = position.x() + cos(currentDelta / 180.0 * 3.141592654) * length;
    double y = position.y() + sin(currentDelta / 180.0 * 3.141592654) * length;
    //qDebug() << "dX:" << x << "dY:" << y;
    path.moveTo(x + startPosition.x(), y + startPosition.y());

    position.setX(x);
    position.setY(y);
}

void LSystem_Parser::turn180()
{
    currentDelta += 180;

    if(currentDelta > 360)
    {
        currentDelta -= 360;
    }
}

QString LSystem_Parser::getFunction(QChar name)
{
    return rules.value(name);
}

void LSystem_Parser::parse(QList<QChar>& forwardFunctions, int maxIteration, int maxDeep)
{
    checkFunctions();

    for(int it = 0; it < maxIteration; it++)
    {
        //qDebug() << "Iteration" << it << "Result:" << string;
        iterateString();
    }

    //qDebug() << "Iteration result:" << string;

    executeString(maxDeep, forwardFunctions);
    if(painter)
    {
        painter->drawPath(path);
    }
}

void LSystem_Parser::executeString(int maxDeep, QList<QChar>& forwardFunctions)
{
    int deep = 0;
    for(QString::Iterator c = string.begin(); c < string.end(); c++)
    {
        if(deep >= maxDeep)
        {
            break;
        }

        switch(c->toLatin1())
        {
        case '+':
            turnLeft();
            break;
        case '-':
            turnRight();
            break;
        case 'F':
            forward();
            break;
        case 'B':
            backward();
            break;
        case '.':
            onlyMove();
            break;
        case '|':
            turn180();
            break;
        default:
            if(getFunction(*c).isEmpty())
            {
                qDebug() << "Unknown function";
            }
            break;
        }
        deep++;

        if(c->toLatin1() != 'F' || c->toLatin1() != 'B' || c->toLatin1() != '.')
        {
            if(forwardFunctions.contains(*c))
            {
                forward();
            }
        }
        //qDebug() << "Deep" << deep;
    }
}

void LSystem_Parser::checkFunctions()
{
    //qDebug() << "Check Functions:";
    for(QString::Iterator c = string.begin(); c < string.end()-1; c++)
    {
        if(*(c+1) == '=')
        {
            QChar name = *c;
            c+=2;
            QString rule;
            for(; c < string.end(); c++)
            {
                if(*c == ';')
                {
                    break;
                }

                rule += *c;
            }

            addRule(name, rule);
            //qDebug() << "New Function" << name << "with rule" << rule;
        }
    }

    string.remove(QRegExp(".=.*;"));
    //qDebug() << "Resulted string" << string;
    //qDebug() << "-------------------------------------";
}

void LSystem_Parser::iterateString()
{
    QString tmp;
    for(QString::Iterator c = string.begin(); c < string.end(); c++)
    {
        QString result = getFunction(*c);
        if(result.isEmpty())
        {
            tmp += *c;
        }
        else
        {
            tmp += result;
        }
    }

    string = tmp;
}
