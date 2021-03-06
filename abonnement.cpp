#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmersuppression.h"
#include <QSqlQuery>

/**
 * @brief MainWindow::affichageAbonnement()
 * Affiche l'ensemble des éléments dans l'onglet "Abonnement"
 */

void MainWindow::affichageAbonnement()
{
    ui->pushButton_supprimerAbonnement->setDisabled(1);

    QStringList tableLabels;
    tableLabels << "Numéro" << "Nom" << "Prix" << "Durée (mois)";
    ui->tableWidget_abonnement->clear();
    ui->tableWidget_abonnement->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_abonnement->setRowCount(0);
    ui->tableWidget_abonnement->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery affichageAbonnementRequest("SELECT * FROM TypeAbonnement");

    affichageAbonnementRequest.exec();

    int cptAbonnement=0;
    while(affichageAbonnementRequest.next())
    {
        ui->tableWidget_abonnement->insertRow(cptAbonnement);
        ui->tableWidget_abonnement->setItem(cptAbonnement,0,new QTableWidgetItem(affichageAbonnementRequest.value("numeroTypeAbonnement").toString()));
        ui->tableWidget_abonnement->setItem(cptAbonnement,1,new QTableWidgetItem(affichageAbonnementRequest.value("libelleTypeAbonnement").toString()));
        ui->tableWidget_abonnement->setItem(cptAbonnement,2,new QTableWidgetItem(affichageAbonnementRequest.value("prixTypeAbonnement").toString()));
        ui->tableWidget_abonnement->setItem(cptAbonnement,3,new QTableWidgetItem(affichageAbonnementRequest.value("dureeTypeAbonnement").toString()));
        cptAbonnement++;
    }

    QStringList tableLabels2;
    tableLabels2 << "Producteur" << "Date début" << "Date fin" << "Abonnement";
    ui->tableWidget_historiqueAbonnement->clear();
    ui->tableWidget_historiqueAbonnement->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_historiqueAbonnement->setRowCount(0);
    ui->tableWidget_historiqueAbonnement->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery historiqueAbonnementRequest("SELECT * FROM abonnement");

    historiqueAbonnementRequest.exec();

    int cptHistorique=0;
    while(historiqueAbonnementRequest.next())
    {
        QSqlQuery nomProducteurAbonnementRequest("SELECT nomProducteur,prenomProducteur FROM producteur WHERE numeroAbonnement="+historiqueAbonnementRequest.value("numeroAbonnement").toString());

        nomProducteurAbonnementRequest.first();

        ui->tableWidget_historiqueAbonnement->insertRow(cptHistorique);
        ui->tableWidget_historiqueAbonnement->setItem(cptHistorique,0,new QTableWidgetItem(nomProducteurAbonnementRequest.value("nomProducteur").toString()
                                                                                           + " " + nomProducteurAbonnementRequest.value("prenomProducteur").toString()));
        ui->tableWidget_historiqueAbonnement->setItem(cptHistorique,1,new QTableWidgetItem(historiqueAbonnementRequest.value("dateDebutAbonnement").toString()));
        ui->tableWidget_historiqueAbonnement->setItem(cptHistorique,2,new QTableWidgetItem(historiqueAbonnementRequest.value("dateFinAbonnement").toString()));

        QSqlQuery libelleAbonnementRequest("SELECT libelleTypeAbonnement FROM TypeAbonnement WHERE numeroTypeAbonnement="+historiqueAbonnementRequest.value("numeroTypeAbonnement").toString());

        libelleAbonnementRequest.first();

        ui->tableWidget_historiqueAbonnement->setItem(cptHistorique,3,new QTableWidgetItem(libelleAbonnementRequest.value("libelleTypeAbonnement").toString()));
        cptHistorique++;
    }
}

/**
 * @brief MainWindow::on_tableWidget_abonnement_cellClicked()
 * Récupère les éléments de la ligne sélectionnée dans le tableau des abonnements pour compléter les zones de saisies et menus déroulants correspondants.
 */

void MainWindow::on_tableWidget_abonnement_cellClicked(int row, int column)
{
    ui->pushButton_supprimerAbonnement->setEnabled(1);

    ui->lineEdit_nomAbonnement->setText(ui->tableWidget_abonnement->item(row,1)->text());
    ui->lineEdit_prixAbonnement->setText(ui->tableWidget_abonnement->item(row,2)->text());
    ui->lineEdit_dureeAbonnement->setText(ui->tableWidget_abonnement->item(row,3)->text());
}

/**
 * @brief MainWindow::on_pushButton_ajouterAbonnement_clicked()
 * Insère l'abonnement en base de données
 */

void MainWindow::on_pushButton_ajouterAbonnement_clicked()
{
    QSqlQuery ajouterAbonnementRequest("INSERT INTO TypeAbonnement VALUES("
                                    +getMaxAbonnement()+",'"
                                    +ui->lineEdit_nomAbonnement->text().replace("'","\'").replace(";","")+"',"
                                    +ui->lineEdit_prixAbonnement->text().replace("'","\'").replace(";","")+","
                                    +ui->lineEdit_dureeAbonnement->text().replace("'","\'").replace(";","")+")");

    if(ajouterAbonnementRequest.numRowsAffected() > 0){
        affichageAbonnement();
        ui->statusBar->showMessage("L'abonnement a bien été ajouté");
    } else{
        ui->statusBar->showMessage("Erreur lors de l'ajout de l'abonnement. Veuillez vérifier les informations saisies");
    }
}

/**
 * @brief MainWindow::on_pushButton_modifierAbonnement_clicked()
 * Modifie l'abonnement en base de données
 */

void MainWindow::on_pushButton_modifierAbonnement_clicked()
{
    QSqlQuery modifierAbonnementRequest("UPDATE TypeAbonnement SET "
                                     "libelleTypeAbonnement='"+ui->lineEdit_nomAbonnement->text().replace("'","\'").replace(";","")+
                                     "',prixTypeAbonnement="+ui->lineEdit_prixAbonnement->text().replace("'","\'").replace(";","")+
                                     ",dureeTypeAbonnement=" +ui->lineEdit_dureeAbonnement->text().replace("'","\'").replace(";","")+
                                     " WHERE numeroTypeAbonnement="+ui->tableWidget_abonnement->item(ui->tableWidget_abonnement->currentRow(),0)->text());

    if(modifierAbonnementRequest.numRowsAffected() > 0){
        affichageAbonnement();
        affichageProducteur();
        ui->statusBar->showMessage("L'abonnement a bien été modifié");
    } else{
        ui->statusBar->showMessage("Erreur lors de la modification de l'abonnement. Veuillez vérifier les informations saisies");
    }
}

/**
 * @brief MainWindow::on_pushButton_supprimerAbonnement_clicked()
 * Supprime l'abonnement en base de données.
 */

void MainWindow::on_pushButton_supprimerAbonnement_clicked()
{
    ConfirmerSuppression laSuppression;

    if (laSuppression.exec()==QDialog::Accepted)
    {
        QSqlQuery supprimerAbonnementRequest("DELETE FROM TypeAbonnement "
                                             "WHERE numeroTypeAbonnement="+ui->tableWidget_abonnement->item(ui->tableWidget_abonnement->currentRow(),0)->text().replace("'","\'").replace(";",""));

        if(supprimerAbonnementRequest.numRowsAffected() > 0) {
            affichageAbonnement();
            affichageProducteur();
            ui->statusBar->showMessage("L'abonnement a bien été supprimé");
        } else{
            ui->statusBar->showMessage("Erreur lors de la suppression de l'abonnement. Veuillez vérifier que l'ensemble des éléments de la table \"Abonnement\" ne sont plus associé à cet abonnement.");
        }
    }
}

/**
 * @brief MainWindow::getMaxVariete()
 * Recherche le plus grand id de la table abonnement +1
 * @return Renvoie l'id le plus grand +1
 */

QString MainWindow::getMaxAbonnement()
{
    QSqlQuery maxNumAbonnementRequest("SELECT IFNULL((SELECT MAX(numeroTypeAbonnement)+1 FROM TypeAbonnement),0)");
    maxNumAbonnementRequest.first();

    QString maxNumAbonnement = maxNumAbonnementRequest.value(0).toString();

    return maxNumAbonnement;
}
