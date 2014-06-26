#include "renderthread.h"

#include <QPainter>
#include <QDebug>

#include "lsystem_parser.h"
#include "algorithmplugin.h"
#include "pluginmanager.h"

#include <math.h>

RenderThread::RenderThread(QObject *parent) :
        QThread(parent), maxIterations(64), limit(4), alParser("", this), currentFraktal(0)
{
    restart = false;
    abort = false;

    for (int i = 0; i < ColormapSize; ++i)
    {
        colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
    }
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void RenderThread::render(Fraktal* fraktal, double centerX, double centerY, double scaleFactor, QSize resultSize)
{
    QMutexLocker locker(&mutex);

    currentFraktal = fraktal;
    this->centerX = centerX;
    this->centerY = centerY;
    this->scaleFactor = scaleFactor;
    this->resultSize = resultSize;

    if (!isRunning())
    {
        start(QThread::HighestPriority);
    }
    else
    {
        restart = true;
        condition.wakeOne();
    }
}

void RenderThread::run()
{
    forever
    {
        mutex.lock();
        QSize resultSize = this->resultSize;
        double scaleFactor = this->scaleFactor;
        double centerX = this->centerX;
        double centerY = this->centerY;
        Fraktal* currentFraktal = this->currentFraktal;
        mutex.unlock();


        calculate(currentFraktal, resultSize, scaleFactor, centerX, centerY);

        mutex.lock();
        if (!restart)
        {
            condition.wait(&mutex);
        }
        restart = false;
        mutex.unlock();
    }
}

uint RenderThread::rgbFromWaveLength(double wave)
{
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    if (wave >= 380.0 && wave <= 440.0)
    {
        r = -1.0 * (wave - 440.0) / (440.0 - 380.0);
        b = 1.0;
    }
    else if (wave >= 440.0 && wave <= 490.0)
    {
        g = (wave - 440.0) / (490.0 - 440.0);
        b = 1.0;
    }
    else if (wave >= 490.0 && wave <= 510.0)
    {
        g = 1.0;
        b = -1.0 * (wave - 510.0) / (510.0 - 490.0);
    }
    else if (wave >= 510.0 && wave <= 580.0)
    {
        r = (wave - 510.0) / (580.0 - 510.0);
        g = 1.0;
    }
    else if (wave >= 580.0 && wave <= 645.0)
    {
        r = 1.0;
        g = -1.0 * (wave - 645.0) / (645.0 - 580.0);
    }
    else if (wave >= 645.0 && wave <= 780.0)
    {
        r = 1.0;
    }

    double s = 1.0;
    if (wave > 700.0)
    {
        s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
    }
    else if (wave <  420.0)
    {
        s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);
    }

    r = pow(r * s, 0.8);
    g = pow(g * s, 0.8);
    b = pow(b * s, 0.8);

    return qRgb(int(r * 255), int(g * 255), int(b * 255));
}

void RenderThread::calculate(Fraktal* currentFraktal, QSize resultSize, double scaleFactor, double centerX, double centerY)
{
    if(!currentFraktal)
    {
        return;
    }

    emit renderBegin();

    int halfWidth = resultSize.width() / 2;
    int halfHeight = resultSize.height() / 2;
    QImage image(resultSize, QImage::Format_RGB32);

    mutex.lock();
    const int Limit = limit;
    mutex.unlock();

    if(!currentFraktal->getIsLSystem() && !currentFraktal->getIsPlugin())
    {
        if(alParser.getState() != Algebra_Parser::Running)
        {
            alParser.start();

        }
        alParser.executeExpression("display2d:false$\n");
        alParser.flush();
        alParser.executeExpression("fpprintprec:6$\n");
        alParser.flush();
        alParser.executeExpression("fpprec:6$\n");
        alParser.flush();

        QString realPart = alParser.executeExpression(QString("realpart(%1);\n").arg(currentFraktal->getAlgorithm()));
        alParser.flush();
        qDebug() << "Realpart" << realPart;
        alParser.executeExpression(QString("fr(a,b,c):=%1;\n").arg(realPart));
        alParser.flush();

        QString imagPart = alParser.executeExpression(QString("imagpart(%1);\n").arg(currentFraktal->getAlgorithm()));
        alParser.flush();
        qDebug() << "Imagpart" << imagPart;
        alParser.executeExpression(QString("fi(a,b,d):=%1;\n").arg(imagPart));
        alParser.flush();
        /*FraktalAlgorithm algorithm = Algorithm[currentFraktal];

        if(algorithm)
        {*/
        for (int y = -halfHeight; y < halfHeight; ++y)
        {
            if (restart)
            {
                break;
            }

            if (abort)
            {
                return;
            }

            uint *scanLine = reinterpret_cast<uint *>(image.scanLine(y + halfHeight));
            double ay = centerY + (y * scaleFactor);

            for (int x = -halfWidth; x < halfWidth; ++x)
            {
                double ax = centerX + (x * scaleFactor);
                double a = ax;
                double b = ay;
                int numIterations = 0;

                do
                {
                    bool ok;
                    ++numIterations;
                    double a2 = alParser.executeExpression(QString("fr(%1,%2,%3);\n").arg(a).arg(b).arg(ax)).toDouble(&ok);

                    if(!ok)
                    {
                        qCritical("Couldn't get result from type double");
                        qDebug() << "Last Result:" << alParser.executeExpression(QString("fr(%1,%2,%3);\n").arg(a).arg(b).arg(ax));
                        emit renderFinished();
                        return;
                    }

                    double b2 = alParser.executeExpression(QString("fi(%1,%2,%3);\n").arg(a).arg(b).arg(ay)).toDouble(&ok);

                    if(!ok)
                    {
                        qCritical("Couldn't get result from type double");
                        qDebug() << "Last Result:" << alParser.executeExpression(QString("fi(%1,%2,%3);\n").arg(a).arg(b).arg(ay));
                        emit renderFinished();
                        return;
                    }

                    if ((a2 * a2) + (b2 * b2) > limit)
                    {
                        break;
                    }

                    ++numIterations;
                    a = alParser.executeExpression(QString("fr(%1,%2,%3);\n").arg(a2).arg(b2).arg(ax)).toDouble(&ok);

                    if(!ok)
                    {
                        qCritical("Couldn't get result from type double");
                        qDebug() << "Last Result:" << alParser.executeExpression(QString("fr(%1,%2,%3);\n").arg(a2).arg(b2).arg(ax));
                        emit renderFinished();
                        return;
                    }

                    b = alParser.executeExpression(QString("fi(%1,%2,%3);\n").arg(a2).arg(b2).arg(ay)).toDouble(&ok);

                    if(!ok)
                    {
                        qCritical("Couldn't get result from type double");
                        qDebug() << "Last Result:" << alParser.executeExpression(QString("fi(%1,%2,%3);\n").arg(a2).arg(b2).arg(ay));
                        emit renderFinished();
                        return;
                    }

                    if ((a * a) + (b * b) > limit)
                    {
                        break;
                    }
                } while (numIterations < maxIterations);

                if (numIterations < maxIterations)
                {
                    *scanLine++ = colormap[numIterations % ColormapSize];
                }
                else
                {
                    *scanLine++ = qRgb(0, 0, 0);
                }
            }
        }

        if (!restart)
        {
            emit renderedImage(image, scaleFactor);
        }
    }
    else if(currentFraktal->getIsLSystem() && !currentFraktal->getIsPlugin())
    {
        if(scaleFactor)
        {
            QPainter painter(&image);
            if(antialiasing)
            {
                painter.setRenderHint(QPainter::Antialiasing);
            }

            painter.setPen(currentFraktal->getPenColor());

            //qDebug() << "###################################################################################";
            LSystem_Parser parser(currentFraktal->getAlgorithm(), &painter, currentFraktal->getDefaultLength()/scaleFactor, currentFraktal->getDefaultDelta(), QPointF(centerX+halfWidth, centerY+halfHeight));
            parser.parse(currentFraktal->getForwardFunctions(), maxIterations, Limit);

            emit renderedImage(image, scaleFactor);
        }
    }
    else if(!currentFraktal->getIsLSystem() && currentFraktal->getIsPlugin())
    {
        AlgorithmPlugin *plugin = PluginManager::getInstance()->loadAlgorithmPlugin(currentFraktal->getPluginPath());
        if(plugin)
        {
            bool isJuliaSet = currentFraktal->getIsJuliaSet();
            bool isDoubleOutput = currentFraktal->getIsDoubleInput();
            bool isInvertedDoubleOutput = currentFraktal->getIsInvertedDoubleInput();

            for (int y = -halfHeight; y < halfHeight; ++y)
            {
                if (restart)
                {
                    break;
                }

                if (abort)
                {
                    return;
                }

                uint *scanLine = reinterpret_cast<uint *>(image.scanLine(y + halfHeight));
                double ay = centerY + (y * scaleFactor);
                for (int x = -halfWidth; x < halfWidth; ++x)
                {
                    double ax = centerX + (x * scaleFactor);

                    double a = ax;
                    double b = ay;
                    int numIterations = 0;

                    do
                    {
                        if(isDoubleOutput)
                        {
                            if(!isInvertedDoubleOutput)
                            {
                                if(plugin->algorithm(&numIterations, Limit, &a, &b, &a, &b, isJuliaSet))
                                {
                                    break;
                                }
                            }
                            else
                            {
                                if(plugin->algorithm(&numIterations, Limit, &b, &a, &a, &b, isJuliaSet))
                                {
                                    break;
                                }
                            }
                        }
                        else
                        {
                            if(plugin->algorithm(&numIterations, Limit, &ax, &ay, &a, &b, isJuliaSet))
                            {
                                break;
                            }
                        }
                    } while (numIterations < maxIterations);

                    if (numIterations < maxIterations)
                    {
                        *scanLine++ = colormap[numIterations % ColormapSize];
                    }
                    else
                    {
                        *scanLine++ = qRgb(0, 0, 0);
                    }
                }
            }

            if (!restart)
            {
                emit renderedImage(image, scaleFactor);
            }
        }
    }
    else
    {
        qWarning() << "Couldn't render a L-System plugin, at the moment";
    }

    emit renderFinished();
}
/*
// Algorithm...

#define PI 3.1415926535897932384626433832795

bool drawTriangleHead(int &iterations, const int limit, double &ax, double &ay, double &a, double &b)//z^4 + c
{
    ++iterations;

    double a2 = (a * a * a * a) - 6 * (b * b) * (a * a) + (b * b * b * b) + ax;
    double b2 = 4 * b * (a * a * a) - 4 * (b * b * b) * a + ay;

    if((a2*a2) + (b2*b2) > limit)
    {
        return false;
    }

    ++iterations;

    a = (a2 * a2 * a2 * a2) - 6 * (b2 * b2) * (a2 * a2) + (b2 * b2 * b2 * b2) + ax;
    b = 4 * b2 * (a2 * a2 * a2) - 4 * (b2 * b2 * b2) * a2 + ay;

    if((a*a) + (b*b) > limit)
    {
        return false;
    }

    return true;
}

bool drawQuadHead(int &iterations, const int limit, double &ax, double &ay, double &a, double &b)//z^5 + c
{
    ++iterations;

    double a2 = (a * a * a * a * a) - 10 * (b * b) * (a * a * a) + 5 * (b * b * b * b) * a + ax;
    double b2 = 5 * b * (a * a * a * a) - 10 * (b * b * b) * (a * a) + ay + (b * b * b * b * b);

    if((a2*a2) + (b2*b2) > limit)
    {
        return false;
    }

    ++iterations;

    a = (a2 * a2 * a2 * a2 * a2) - 10 * (b2 * b2) * (a2 * a2 * a2) + 5 * (b2 * b2 * b2 * b2) * a2 + ax;
    b = 5 * b2 * (a2 * a2 * a2 * a2) - 10 * (b2 * b2 * b2) * (a2 * a2) + ay + (b2 * b2 * b2 * b2 * b2);

    if((a*a) + (b*b) > limit)
    {
        return false;
    }

    return true;
}

bool drawPiHead(int &iterations, const int limit, double &ax, double &ay, double &a, double &b)//z^PI + c
{
    ++iterations;

    double a2 = cos(PI*atan2(b,a))*pow((a*a+b*b),(PI/2)) + ax;
    double b2 = sin(PI*atan2(b,a))*pow((a*a+b*b),(PI/2)) + ay;

    if((a2*a2) + (b2*b2) > limit)
    {
        return false;
    }

    ++iterations;

    a = cos(PI*atan2(b2,a2))*pow((a2*a2+b2*b2),(PI/2)) + ax;
    b = sin(PI*atan2(b2,a2))*pow((a2*a2+b2*b2),(PI/2)) + ay;

    if((a*a) + (b*b) > limit)
    {
        return false;
    }

    return true;
}

bool drawSpectrumWaves(int &iterations, const int limit, double &ax, double &ay, double &a, double &b)
{//TODO
    ++iterations;

    double a2 = sqrt(pow((ax - a),2) + pow((ay - b), 2)) + ax;
    double b2 = (a * sin(2*b)) + ay;

    if((a2*a2) + (b2*b2) > limit)
    {
        return false;
    }

    ++iterations;

    a = sqrt(pow((ax - a2),2) + pow((ay - b2), 2)) + ax;
    b = (a2 * sin(2*b2)) + ay;

    if((a*a) + (b*b) > limit)
    {
        return false;
    }

    return true;
}*/
