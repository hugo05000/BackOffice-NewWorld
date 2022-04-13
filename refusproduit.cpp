#include "refusproduit.h"
#include "ui_refusproduit.h"
#include <QSqlQuery>

Refusproduit::Refusproduit(int numeroProduit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Refusproduit)
{
    ui->setupUi(this);

    numProduit =numeroProduit;
}


Refusproduit::~Refusproduit()
{
    delete ui;
}


void Refusproduit::on_pushButton_valider_clicked()
{
    QSqlQuery refusRequest("INSERT INTO refusProduit VALUES("+getMaxNumRefusProduit()+",'"+ui->textEdit_motifRefus->toPlainText()+"',NOW(),"+QString::number(numProduit)+")");
    accept();
}


void Refusproduit::on_pushButton_annuler_clicked()
{
    close();
}

QString Refusproduit::getMaxNumRefusProduit()
{
    QSqlQuery maxNumRefusProduitRequest("SELECT IFNULL((SELECT MAX(numRefusProduit)+1 FROM refusProduit),0)");
    maxNumRefusProduitRequest.first();

    QString maxNumRefusProduit = maxNumRefusProduitRequest.value(0).toString();

    return maxNumRefusProduit;
}
