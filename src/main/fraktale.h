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
