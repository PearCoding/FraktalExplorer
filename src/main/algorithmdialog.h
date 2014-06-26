#ifndef ALGORITHMDIALOG_H
#define ALGORITHMDIALOG_H

#include <QDialog>

#include "fraktale.h"

namespace Ui {
    class AlgorithmDialog;
}

class AlgorithmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlgorithmDialog(QString title, QString acceptButtonText, Fraktal* fraktal = 0, QWidget* parent = 0, bool mustAllowed = false);
    ~AlgorithmDialog();

    inline Fraktal* getCurrentFraktal()
    {
        return currentFraktal;
    }

    inline bool isWritingAllowed()
    {
        return writingAllowed;
    }

private:
    Ui::AlgorithmDialog *ui;

    Fraktal* currentFraktal;
    QColor currentColor;
    bool writingAllowed;

private slots:
    void slChangeMode(bool b);
    void slChangeColor();
    void slFinish();

    void slChangePlugin(bool b);
    void slFileDialog();
    void slCheckLibrary();

    void slChangeDoubleInput(bool b);
};

#endif // NEWALGORITHMDIALOG_H
