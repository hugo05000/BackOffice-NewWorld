#include "confirmersuppression.h"
#include "ui_confirmersuppression.h"

/**
 * @brief ConfirmerSuppression::ConfirmerSuppression
 * Initialise la boîte de dialogue
 */

ConfirmerSuppression::ConfirmerSuppression(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmerSuppression)
{
    ui->setupUi(this);
}

/**
 * @brief Destructeur de la classe
 * Destructeur de la classe ConfirmerSuppression
 */

ConfirmerSuppression::~ConfirmerSuppression()
{
    delete ui;
}

/**
 * @brief ConfirmerSuppression::on_pushButton_supprimer_clicked()
 * Accepte la suppression
 */

void ConfirmerSuppression::on_pushButton_supprimer_clicked()
{
    accept();
}

/**
 * @brief ConfirmerSuppression::on_pushButton_Annuler_clicked()
 * Annule la suppression en fermant la boîte de dialogue
 */

void ConfirmerSuppression::on_pushButton_Annuler_clicked()
{
    close();
}
