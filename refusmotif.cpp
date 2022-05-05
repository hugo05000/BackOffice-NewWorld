#include "refusmotif.h"
#include "ui_refusmotif.h"
#include <QSqlQuery>

/**
 * @brief RefusMotif::RefusMotif
 * Initialise la boîte de dialogue
 */

RefusMotif::RefusMotif(int leNumero, QString laTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefusMotif)
{
    ui->setupUi(this);
    numero=leNumero;
    table=laTable;
}

/**
 * @brief Destructeur de la classe
 * Destructeur de la classe RefusMotif
 */

RefusMotif::~RefusMotif()
{
    delete ui;
}

/**
 * @brief RefusMotif::on_pushButton_valider_clicked()
 * Accepte le refus
 */

void RefusMotif::on_pushButton_valider_clicked()
{
    QSqlQuery refusRequest("INSERT INTO refus"+table+" "
                                                     "VALUES("+getMaxTable()+",'"+ui->textEdit_motif->toPlainText().replace("'","\'").replace(";","")+"',NOW(),"+QString::number(numero)+")");
    accept();
}

/**
 * @brief RefusMotif::on_pushButton_annuler_clicked()
 * Accepte le refus en fermant la boîte de dialogue
 */

void RefusMotif::on_pushButton_annuler_clicked()
{
    close();
}

/**
 * @brief RefusMotif::getMaxTable()
 * Recherche le plus grand id de la table refus +1
 * @return Renvoie l'id le plus grand +1
 */

QString RefusMotif::getMaxTable()
{
    QSqlQuery maxNumRefusTableRequest("SELECT IFNULL((SELECT MAX(num"+table+")+1 FROM refus"+table+"),0)");
    maxNumRefusTableRequest.first();

    QString maxNumRefusTable = maxNumRefusTableRequest.value(0).toString();

    return maxNumRefusTable;
}

