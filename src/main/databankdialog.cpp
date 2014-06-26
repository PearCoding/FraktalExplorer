#include "databankdialog.h"
#include "ui_databankdialog.h"

#include <QMessageBox>

#include "algorithmdialog.h"

DatabankDialog::DatabankDialog(QList<Fraktal*>* fraktals, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabankDialog), fraktals(fraktals)
{
    ui->setupUi(this);

    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slDelete()));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(slSelection(int)));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(slNew()));

    populateList();
}

DatabankDialog::~DatabankDialog()
{
    delete ui;
}

void DatabankDialog::populateList()
{
    if(fraktals)
    {
        if(fraktals->isEmpty())
        {
            QMessageBox::information(this, tr("No algorithm in Databank"), tr("In the Databank is no algorithm, create one at first"));

            ui->listWidget->setEnabled(false);
            ui->groupBox->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
        }

        ui->listWidget->clear();

        foreach(Fraktal* fraktal, *fraktals)
        {
            ui->listWidget->addItem(fraktal->getName());
        }

        ui->listWidget->setCurrentRow(0);
    }
}

void DatabankDialog::slDelete()
{
    if(QMessageBox::question(this, tr("Really delete?"), tr("Want you really delete the algorithm?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        fraktals->removeAt(ui->listWidget->row(ui->listWidget->selectedItems().first()));

        populateList();
    }
}

void DatabankDialog::slSelection(int index)
{
    if(fraktals)
    {
        Fraktal* fraktal = fraktals->at(index);
        ui->label_2->setText(fraktal->getName());
        ui->checkBox->setChecked(fraktal->getIsLSystem());
        ui->textEdit->setText(fraktal->getAlgorithm());
        if(fraktal->getIsLSystem())
        {
            ui->line_2->setVisible(true);
            ui->label_4->setVisible(true);
            ui->label_7->setVisible(true);
            ui->label_5->setVisible(true);
            ui->label_8->setVisible(true);
            ui->label_6->setVisible(true);
            ui->frame->setVisible(true);

            ui->checkBox_3->setVisible(false);
            ui->checkBox_4->setVisible(false);
            ui->checkBox_5->setVisible(false);
            ui->line_5->setVisible(false);

            ui->label_7->setNum(fraktals->at(index)->getDefaultLength());
            ui->label_8->setNum(fraktals->at(index)->getDefaultDelta());
            ui->frame->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(fraktals->at(index)->getPenColor().red()).arg(fraktals->at(index)->getPenColor().green()).arg(fraktals->at(index)->getPenColor().blue()));
        }
        else
        {
            ui->line_2->setVisible(false);
            ui->label_4->setVisible(false);
            ui->label_7->setVisible(false);
            ui->label_5->setVisible(false);
            ui->label_8->setVisible(false);
            ui->label_6->setVisible(false);
            ui->frame->setVisible(false);

            ui->checkBox_3->setVisible(true);
            ui->checkBox_3->setChecked(fraktal->getIsJuliaSet());
            ui->checkBox_4->setVisible(true);
            ui->checkBox_4->setChecked(fraktal->getIsDoubleInput());

            if(fraktal->getIsDoubleInput())
            {
                ui->checkBox_5->setVisible(true);
                ui->checkBox_5->setChecked(fraktal->getIsInvertedDoubleInput());
                ui->line_5->setVisible(true);
            }
        }

        if(fraktal->getIsPlugin())
        {
            ui->line_3->setVisible(true);
            ui->label_9->setVisible(true);
            ui->label_10->setVisible(true);

            ui->label_10->setText(fraktal->getPluginPath());
        }
        else
        {
            ui->line_3->setVisible(false);
            ui->label_9->setVisible(false);
            ui->label_10->setVisible(false);
        }
    }
}

void DatabankDialog::slNew()
{
    AlgorithmDialog dialog(tr("New algorithm"), tr("Create"), 0, this, true);
    dialog.show();
    dialog.exec();

    if(dialog.getCurrentFraktal())
    {
        fraktals->append(dialog.getCurrentFraktal());

        populateList();
    }
}
