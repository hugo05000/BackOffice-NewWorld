#include "refusmotif.h"
#include "ui_refusmotif.h"
#include <QSqlQuery>

RefusMotif::RefusMotif(int leNumero, QString laTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefusMotif)
{
    ui->setupUi(this);
    numero=leNumero;
    table=laTable;
}

RefusMotif::~RefusMotif()
{
    delete ui;
}

void RefusMotif::on_pushButton_valider_clicked()
{
    QSqlQuery refusRequest("INSERT INTO refus"+table+" "
                                                     "VALUES("+getMaxTable()+",'"+ui->textEdit_motif->toPlainText().replace("'","\'").replace(";","")+"',NOW(),"+QString::number(numero)+")");
    accept();
}


void RefusMotif::on_pushButton_annuler_clicked()
{
    close();
}

QString RefusMotif::getMaxTable()
{
    QSqlQuery maxNumRefusTableRequest("SELECT IFNULL((SELECT MAX(num"+table+")+1 FROM refus"+table+"),0)");
    maxNumRefusTableRequest.first();

    QString maxNumRefusTable = maxNumRefusTableRequest.value(0).toString();

    return maxNumRefusTable;
}

