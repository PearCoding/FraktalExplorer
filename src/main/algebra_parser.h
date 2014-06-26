#ifndef ALGEBRA_PARSER_H
#define ALGEBRA_PARSER_H

#include <QObject>
#include <QProcess>

class Algebra_Parser : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        Running,
        Stopped,
        Error
    };

    explicit Algebra_Parser(QString path = "", QObject *parent = 0);
    ~Algebra_Parser();

    void start();
    void stop();

    inline void restart()
    {
        stop();
        start();
    }

    inline void setPath(QString path)
    {
        this->path = path;
    }

    inline QString getPath() const
    {
        return path;
    }

    inline State getState() const
    {
        return processState;
    }

    QString executeExpression(QString expr);

    void flush();

signals:

protected:
    QString postOutputOperation(QString output);

private slots:
    void computeError(QProcess::ProcessError error);
    void finished(int exitCode);
    void started();
    void processEmitError();
    void processEmitOutput();//Debug

private:
    QString path;
    QProcess process;
    State processState;
};

#endif // ALGEBRA_PARSER_H
