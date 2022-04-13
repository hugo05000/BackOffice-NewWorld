#include "confirmersuppression.h"
#include "ui_confirmersuppression.h"

ConfirmerSuppression::ConfirmerSuppression(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmerSuppression)
{
    ui->setupUi(this);
}

ConfirmerSuppression::~ConfirmerSuppression()
{
    delete ui;
}

void ConfirmerSuppression::on_pushButton_supprimer_clicked()
{
    accept();
}

void ConfirmerSuppression::on_pushButton_Annuler_clicked()
{
    close();
}
