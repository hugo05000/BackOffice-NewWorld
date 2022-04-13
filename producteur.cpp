#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmersuppression.h"
#include <QSqlQuery>

void MainWindow::affichageProducteur()
{
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

void MainWindow::on_tableWidget_producteur_cellClicked(int row, int column)
{
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

void MainWindow::on_pushButton_modifierProducteur_clicked()
{
    QSqlQuery modifierProducteurRequest("UPDATE producteur SET "
                                     "nomProducteur='"+ui->lineEdit_nomProducteur->text()+
                                     "',prenomProducteur='"+ui->lineEdit_prenomProducteur->text()+
                                     "',loginProducteur='" +ui->lineEdit_loginProducteur->text()+
                                     "',adresseProducteur='" +ui->lineEdit_adresseProducteur->text()+
                                     "',villeProducteur='" +ui->lineEdit_villeProducteur->text()+
                                     "',cpProducteur='" +ui->lineEdit_cpProducteur->text()+
                                     "',numeroAbonnement=" +ui->comboBox_abonnement->currentData().toString()+
                                     " WHERE numeroProducteur="+ui->tableWidget_producteur->item(ui->tableWidget_producteur->currentRow(),0)->text());

    if(modifierProducteurRequest.numRowsAffected() > 0){
        affichageProducteur();
        ui->statusBar->showMessage("Le producteur a bien été modifié");
    } else{
        ui->statusBar->showMessage("Erreur lors de la modification du producteur. Veuillez vérifier les informations saisies");
    }
}


void MainWindow::on_pushButton_supprimerProducteur_clicked()
{
    ConfirmerSuppression laSuppression;

    if (laSuppression.exec()==QDialog::Accepted)
    {
        QSqlQuery supprimerProducteurtRequest("DELETE FROM producteur WHERE numeroProducteur="+ui->tableWidget_producteur->item(ui->tableWidget_producteur->currentRow(),0)->text());

        if(supprimerProducteurtRequest.numRowsAffected() > 0) {
            affichageProducteur();
            ui->statusBar->showMessage("Le producteur a bien été supprimé");
        } else{
            ui->statusBar->showMessage("Erreur lors de la suppression du producteur.");
        }
    }
}


void MainWindow::on_pushButton_accepterProducteur_clicked()
{
    QSqlQuery activationProducteurRequest("UPDATE producteur SET activationProducteur=1 WHERE numeroProducteur="+ui->tableWidget_validationProducteur->item(ui->tableWidget_validationProducteur->currentRow(),0)->text());
    if(activationProducteurRequest.numRowsAffected() > 0) {
        affichageProducteur();
        ui->statusBar->showMessage("Le producteur a bien été validé");
    } else{
        ui->statusBar->showMessage("Un problème est survenu lors de la validation du producteur");
    }
}


void MainWindow::on_pushButton_declinerProducteur_clicked()
{
    QSqlQuery declinerProducteurRequest("DELETE FROM Producteur WHERE numeroProducteur="+ui->tableWidget_validationProducteur->item(ui->tableWidget_validationProducteur->currentRow(),0)->text());

    if(declinerProducteurRequest.numRowsAffected() > 0) {
        affichageProducteur();
        ui->statusBar->showMessage("Le producteur a bien été décliné");
    } else{
        ui->statusBar->showMessage("Erreur lors du déclin de la demande de validation du producteur.");
    }
}


QString MainWindow::getMaxProducteur()
{
    QSqlQuery maxNumProducteurRequest("SELECT IFNULL((SELECT MAX(numeroProducteur)+1 FROM Producteur),0)");
    maxNumProducteurRequest.first();

    QString maxNumProducteur = maxNumProducteurRequest.value(0).toString();

    return maxNumProducteur;
}
