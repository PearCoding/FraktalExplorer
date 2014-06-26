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
