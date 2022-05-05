#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmersuppression.h"
#include <QSqlQuery>

/**
 * @brief MainWindow::affichageProducteur()
 * Affiche l'ensemble des éléments dans l'onglet "Producteurs".
 */

void MainWindow::affichageProducteur()
{
    ui->pushButton_supprimerProducteur->setDisabled(1);

    //On initialise le tableau
    QStringList tableLabels;
    tableLabels << "Numéro" << "Nom" << "Prénom" << "Login" << "Adresse" << "Ville" << "CP" << "Type abonnement";
    ui->tableWidget_producteur->clear();
    ui->tableWidget_producteur->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_producteur->setRowCount(0);
    ui->tableWidget_producteur->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery lesProducteursRequest("SELECT * FROM producteur WHERE activationProducteur=1");

    lesProducteursRequest.exec();

    int cptProducteur=0;
    while(lesProducteursRequest.next())
    {
        ui->tableWidget_producteur->insertRow(cptProducteur);
        ui->tableWidget_producteur->setItem(cptProducteur,0,new QTableWidgetItem(lesProducteursRequest.value("numeroProducteur").toString()));
        ui->tableWidget_producteur->setItem(cptProducteur,1,new QTableWidgetItem(lesProducteursRequest.value("nomProducteur").toString()));
        ui->tableWidget_producteur->setItem(cptProducteur,2,new QTableWidgetItem(lesProducteursRequest.value("prenomProducteur").toString()));
        ui->tableWidget_producteur->setItem(cptProducteur,3,new QTableWidgetItem(lesProducteursRequest.value("loginProducteur").toString()));
        ui->tableWidget_producteur->setItem(cptProducteur,4,new QTableWidgetItem(lesProducteursRequest.value("adresseProducteur").toString()));
        ui->tableWidget_producteur->setItem(cptProducteur,5,new QTableWidgetItem(lesProducteursRequest.value("villeProducteur").toString()));
        ui->tableWidget_producteur->setItem(cptProducteur,6,new QTableWidgetItem(lesProducteursRequest.value("cpProducteur").toString()));
        ui->tableWidget_producteur->setItem(cptProducteur,7,new QTableWidgetItem(lesProducteursRequest.value("numeroAbonnement").toString()));
        cptProducteur++;
    }

    ui->tableWidget_producteur->setColumnHidden(0,true);

    ui->comboBox_abonnement->clear();
    QSqlQuery lesAbonnementsRequest("SELECT * FROM TypeAbonnement");

    while(lesAbonnementsRequest.next())
    {
        ui->comboBox_abonnement->addItem(lesAbonnementsRequest.value("libelleTypeAbonnement").toString(),
                                      lesAbonnementsRequest.value("numeroTypeAbonnement").toInt());
    }

    ui->tableWidget_validationProducteur->clear();
    ui->tableWidget_validationProducteur->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_validationProducteur->setRowCount(0);
    ui->tableWidget_validationProducteur->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery producteurAttenteRequest("SELECT * FROM producteur WHERE activationProducteur=0");

    producteurAttenteRequest.exec();

    int cptActivation=0;
    while(producteurAttenteRequest.next())
    {
        ui->tableWidget_validationProducteur->insertRow(cptActivation);
        ui->tableWidget_validationProducteur->setItem(cptActivation,0,new QTableWidgetItem(producteurAttenteRequest.value("numeroProducteur").toString()));
        ui->tableWidget_validationProducteur->setItem(cptActivation,1,new QTableWidgetItem(producteurAttenteRequest.value("nomProducteur").toString()));
        ui->tableWidget_validationProducteur->setItem(cptActivation,2,new QTableWidgetItem(producteurAttenteRequest.value("prenomProducteur").toString()));
        ui->tableWidget_validationProducteur->setItem(cptActivation,3,new QTableWidgetItem(producteurAttenteRequest.value("loginProducteur").toString()));
        ui->tableWidget_validationProducteur->setItem(cptActivation,4,new QTableWidgetItem(producteurAttenteRequest.value("adresseProducteur").toString()));
        ui->tableWidget_validationProducteur->setItem(cptActivation,5,new QTableWidgetItem(producteurAttenteRequest.value("villeProducteur").toString()));
        ui->tableWidget_validationProducteur->setItem(cptActivation,6,new QTableWidgetItem(producteurAttenteRequest.value("cpProducteur").toString()));
        ui->tableWidget_validationProducteur->setItem(cptActivation,7,new QTableWidgetItem(producteurAttenteRequest.value("numeroAbonnement").toString()));
        ui->tableWidget_validationProducteur->setColumnHidden(0,true);
        cptActivation++;
    }
}

/**
 * @brief MainWindow::on_tableWidget_producteur_cellClicked()
 * Récupère les éléments de la ligne sélectionné dans le tableau des producteurs pour compléter les zones de saisies et menus déroulants correspondants.
 */

void MainWindow::on_tableWidget_producteur_cellClicked(int row, int column)
{
    ui->pushButton_supprimerProducteur->setEnabled(1);

    QSqlQuery rechercheAbonnementRequest("SELECT TypeAbonnement.numeroTypeAbonnement "
                                         "FROM TypeAbonnement "
                                         "INNER JOIN Abonnement ON Abonnement.numeroTypeAbonnement = TypeAbonnement.numeroTypeAbonnement "
                                         "WHERE numeroAbonnement="+ui->tableWidget_producteur->item(row,5)->text());

    rechercheAbonnementRequest.first();

    ui->lineEdit_nomProducteur->setText(ui->tableWidget_producteur->item(row,1)->text());
    ui->lineEdit_prenomProducteur->setText(ui->tableWidget_producteur->item(row,2)->text());
    ui->lineEdit_loginProducteur->setText(ui->tableWidget_producteur->item(row,3)->text());
    ui->lineEdit_adresseProducteur->setText(ui->tableWidget_producteur->item(row,4)->text());
    ui->lineEdit_villeProducteur->setText(ui->tableWidget_producteur->item(row,5)->text());
    ui->lineEdit_cpProducteur->setText(ui->tableWidget_producteur->item(row,6)->text());
    ui->comboBox_abonnement->setCurrentIndex(rechercheAbonnementRequest.value(0).toInt());
}

/**
 * @brief MainWindow::on_pushButton_modifierProducteur_clicked()
 * Modifie le producteur en base de données.
 */

void MainWindow::on_pushButton_modifierProducteur_clicked()
{
    QSqlQuery modifierProducteurRequest("UPDATE producteur SET "
                                     "nomProducteur='"+ui->lineEdit_nomProducteur->text().replace("'","\'").replace(";","")+
                                     "',prenomProducteur='"+ui->lineEdit_prenomProducteur->text().replace("'","\'").replace(";","")+
                                     "',loginProducteur='" +ui->lineEdit_loginProducteur->text().replace("'","\'").replace(";","")+
                                     "',adresseProducteur='" +ui->lineEdit_adresseProducteur->text().replace("'","\'").replace(";","")+
                                     "',villeProducteur='" +ui->lineEdit_villeProducteur->text().replace("'","\'").replace(";","")+
                                     "',cpProducteur='" +ui->lineEdit_cpProducteur->text().replace("'","\'").replace(";","")+
                                     "',numeroAbonnement=" +ui->comboBox_abonnement->currentData().toString()+
                                     " WHERE numeroProducteur="+ui->tableWidget_producteur->item(ui->tableWidget_producteur->currentRow(),0)->text().replace("'","\'").replace(";",""));

    if(modifierProducteurRequest.numRowsAffected() > 0){
        affichageProducteur();
        ui->statusBar->showMessage("Le producteur a bien été modifié");
    } else{
        ui->statusBar->showMessage("Erreur lors de la modification du producteur. Veuillez vérifier les informations saisies");
    }
}

/**
 * @brief MainWindow::on_pushButton_supprimerProducteur_clicked()
 * Supprime le producteur en base de données.
 */

void MainWindow::on_pushButton_supprimerProducteur_clicked()
{
    ConfirmerSuppression laSuppression;

    if (laSuppression.exec()==QDialog::Accepted)
    {
        QSqlQuery supprimerestUnRequest("DELETE FROM estUn WHERE numeroProducteur="+ui->tableWidget_producteur->item(ui->tableWidget_producteur->currentRow(),0)->text().replace("'","\'").replace(";",""));
        QSqlQuery supprimerCommandeRequest("DELETE FROM Commande WHERE numeroProducteur="+ui->tableWidget_producteur->item(ui->tableWidget_producteur->currentRow(),0)->text().replace("'","\'").replace(";",""));
        QSqlQuery supprimerLotDeCommandeRequest("DELETE FROM lotDeCommande WHERE numeroProducteur="+ui->tableWidget_producteur->item(ui->tableWidget_producteur->currentRow(),0)->text().replace("'","\'").replace(";",""));
        QSqlQuery supprimerProduireRequest("DELETE FROM Produire WHERE numeroProducteur="+ui->tableWidget_producteur->item(ui->tableWidget_producteur->currentRow(),0)->text().replace("'","\'").replace(";",""));
        QSqlQuery supprimerProducteurtRequest("DELETE FROM producteur WHERE numeroProducteur="+ui->tableWidget_producteur->item(ui->tableWidget_producteur->currentRow(),0)->text().replace("'","\'").replace(";",""));

        if(supprimerProducteurtRequest.numRowsAffected() > 0) {
            affichageProducteur();
            ui->statusBar->showMessage("Le producteur a bien été supprimé");
        } else{
            ui->statusBar->showMessage("Erreur lors de la suppression du producteur.");
        }
    }
}

/**
 * @brief MainWindow::on_pushButton_accepterProducteur_clicked()
 * Active le producteur en base de données.
 */

void MainWindow::on_pushButton_accepterProducteur_clicked()
{
    QSqlQuery activationProducteurRequest("UPDATE producteur SET activationProducteur=1 "
                                          "WHERE numeroProducteur="+ui->tableWidget_validationProducteur->item(ui->tableWidget_validationProducteur->currentRow(),0)->text().replace("'","\'").replace(";",""));
    if(activationProducteurRequest.numRowsAffected() > 0) {
        affichageProducteur();
        ui->statusBar->showMessage("Le producteur a bien été validé");
    } else{
        ui->statusBar->showMessage("Un problème est survenu lors de la validation du producteur");
    }
}

/**
 * @brief MainWindow::on_pushButton_supprimerProducteur_clicked()
 * Refuse la demande du producteur en base de données.
 */

void MainWindow::on_pushButton_declinerProducteur_clicked()
{
    QSqlQuery declinerProducteurRequest("DELETE FROM Producteur "
                                        "WHERE numeroProducteur="+ui->tableWidget_validationProducteur->item(ui->tableWidget_validationProducteur->currentRow(),0)->text().replace("'","\'").replace(";",""));

    if(declinerProducteurRequest.numRowsAffected() > 0) {
        affichageProducteur();
        ui->statusBar->showMessage("Le producteur a bien été décliné");
    } else{
        ui->statusBar->showMessage("Erreur lors du déclin de la demande de validation du producteur.");
    }
}
