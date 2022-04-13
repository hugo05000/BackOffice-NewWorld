#include "suppressionproduit.h"
#include "ui_suppressionproduit.h"

SuppressionProduit::SuppressionProduit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SuppressionProduit)
{
    ui->setupUi(this);
}

SuppressionProduit::~SuppressionProduit()
{
    delete ui;
}

void SuppressionProduit::on_pushButton_supprimer_clicked()
{
    accept();
}


void SuppressionProduit::on_pushButton_Annuler_clicked()
{
    close();
}

