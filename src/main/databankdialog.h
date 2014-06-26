#ifndef DATABANKDIALOG_H
#define DATABANKDIALOG_H

#include <QDialog>
#include <QList>

#include "fraktale.h"

namespace Ui {
    class DatabankDialog;
}

class DatabankDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabankDialog(QList<Fraktal*>* fraktals, QWidget *parent = 0);
    ~DatabankDialog();

private:
    Ui::DatabankDialog *ui;
    QList<Fraktal*>* fraktals;

    void populateList();

private slots:
    void slDelete();
    void slNew();
    void slSelection(int index);
};

#endif // DATABANKDIALOG_H
