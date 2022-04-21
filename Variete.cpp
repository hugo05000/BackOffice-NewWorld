#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmersuppression.h"
#include <refusmotif.h>
#include <QSqlQuery>

void MainWindow::affichageVarietes()
{
    ui->pushButton_supprimerVariete->setDisabled(1);

    QStringList tableLabels;
    tableLabels << "Numéro" << "Nom" << "Rayon";
    ui->tableWidget_variete->clear();
    ui->tableWidget_variete->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_variete->setRowCount(0);
    ui->tableWidget_variete->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery affichageVarieteRequest("SELECT * FROM Variete WHERE activationVariete=1");

    affichageVarieteRequest.exec();

    int cptVariete=0;
    while(affichageVarieteRequest.next())
    {
        ui->tableWidget_variete->insertRow(cptVariete);
        ui->tableWidget_variete->setItem(cptVariete,0,new QTableWidgetItem(affichageVarieteRequest.value("numeroVariete").toString()));
        ui->tableWidget_variete->setItem(cptVariete,1,new QTableWidgetItem(affichageVarieteRequest.value("nomVariete").toString()));
        ui->tableWidget_variete->setItem(cptVariete,2,new QTableWidgetItem(affichageVarieteRequest.value("numeroRayon").toString()));
        cptVariete++;
    }

    ui->tableWidget_variete->setColumnHidden(0,true);
    ui->tableWidget_variete->setColumnHidden(2,true);

    ui->comboBox_rayon->clear();

    QSqlQuery lesRayonsRequest("SELECT * FROM Rayon");

    while(lesRayonsRequest.next())
    {
        ui->comboBox_rayon->addItem(lesRayonsRequest.value("nomRayon").toString(),
                                      lesRayonsRequest.value("numeroRayon").toInt());
    }

    ui->tableWidget_validationVariete->clear();
    ui->tableWidget_validationVariete->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_validationVariete->setRowCount(0);
    ui->tableWidget_validationVariete->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery varieteAttenteRequest("SELECT * FROM Variete WHERE activationVariete=0");

    varieteAttenteRequest.exec();

    int cptActivation=0;
    while(varieteAttenteRequest.next())
    {
        //On vérifie si la variété n'a été refusée
        QSqlQuery refusVarietetRequest("SELECT numRefusVariete FROM refusVariete WHERE numRefusVariete="+varieteAttenteRequest.value("numeroProduit").toString());
        refusVarietetRequest.first();

        //On affiche seulement la variété si elle n'est pas refusée
        if(refusVarietetRequest.size() == 0)
        {
            QSqlQuery nomRayonRequest("SELECT nomRayon FROM rayon WHERE numeroRayon="+varieteAttenteRequest.value("numeroRayon").toString());
            nomRayonRequest.first();

            ui->tableWidget_validationVariete->insertRow(cptActivation);
            ui->tableWidget_validationVariete->setItem(cptActivation,0,new QTableWidgetItem(varieteAttenteRequest.value("numeroVariete").toString()));
            ui->tableWidget_validationVariete->setItem(cptActivation,1,new QTableWidgetItem(varieteAttenteRequest.value("nomVariete").toString()));
            ui->tableWidget_validationVariete->setItem(cptActivation,2,new QTableWidgetItem(nomRayonRequest.value("nomRayon").toString()));
            ui->tableWidget_validationVariete->setColumnHidden(0,true);
            cptActivation++;
        }
    }
}

void MainWindow::on_tableWidget_variete_cellClicked(int row, int column)
{
    ui->pushButton_supprimerVariete->setEnabled(1);

    ui->lineEdit_variete->setText(ui->tableWidget_variete->item(row,1)->text());
    ui->comboBox_rayon->setCurrentIndex(ui->comboBox_rayon->findData(ui->tableWidget_variete->item(row,2)->text().toInt()));
}

void MainWindow::on_pushButton_ajouterVariete_clicked()
{
    QSqlQuery ajouterVarieteRequest("INSERT INTO Variete VALUES("
                                    +getMaxVariete()+",'"
                                    +ui->lineEdit_variete->text().replace("'","\'").replace(";","")+"',"
                                    +ui->comboBox_variete->currentData().toString()+",1,NOW())");

    if(ajouterVarieteRequest.numRowsAffected() > 0){
        affichageVarietes();
        affichageProduits();
        ui->statusBar->showMessage("La variété a bien été ajouté");
    } else{
        ui->statusBar->showMessage("Erreur lors de l'ajout de la variété. Veuillez vérifier les informations saisies");
    }
}


void MainWindow::on_pushButton_modifierVariete_clicked()
{
    QSqlQuery modifierVarieteRequest("UPDATE Variete SET "
                                     "nomVariete='"+ui->lineEdit_variete->text().replace("'","\'").replace(";","")+
                                     "',numeroRayon=" +ui->comboBox_rayon->currentData().toString()+
                                     " WHERE numeroVariete="+ui->tableWidget_variete->item(ui->tableWidget_variete->currentRow(),0)->text());

    if(modifierVarieteRequest.numRowsAffected() > 0){
        affichageVarietes();
        affichageProduits();
        ui->statusBar->showMessage("La variété a bien été modifié");
    } else{
        ui->statusBar->showMessage("Erreur lors de la modification de la variété. Veuillez vérifier les informations saisies");
    }
}


void MainWindow::on_pushButton_supprimerVariete_clicked()
{
    ConfirmerSuppression laSuppression;

    if (laSuppression.exec()==QDialog::Accepted)
    {
        QSqlQuery supprimerProduitrequest("DELETE FROM produit "
                                          "WHERE numeroVariete="+ui->tableWidget_variete->item(ui->tableWidget_variete->currentRow(),0)->text().replace("'","\'").replace(";",""));
        QSqlQuery supprimerVarieteRequest("DELETE FROM Variete "
                                          "WHERE numeroVariete="+ui->tableWidget_variete->item(ui->tableWidget_variete->currentRow(),0)->text().replace("'","\'").replace(";",""));

        if(supprimerVarieteRequest.numRowsAffected() > 0) {
            affichageVarietes();
            affichageProduits();
            ui->statusBar->showMessage("La variété a bien été supprimé");
        } else{
            ui->statusBar->showMessage("Erreur lors de la suppression de la variété.");
        }
    }
}


void MainWindow::on_pushButton_accepterVariete_clicked()
{
    QSqlQuery activationVarieteRequest("UPDATE Variete SET activationVariete=1,dateActivationVariete=NOW() "
                                       "WHERE numeroVariete="+ui->tableWidget_validationVariete->item(ui->tableWidget_validationVariete->currentRow(),0)->text().replace("'","\'").replace(";",""));
    if(activationVarieteRequest.numRowsAffected() > 0) {
        affichageVarietes();
        affichageProduits();
        afficheTableaudebord();
        ui->statusBar->showMessage("La variété a bien été validé");
    } else{
        ui->statusBar->showMessage("Un problème est survenu lors de la validation de la variété");
    }
}


void MainWindow::on_pushButton_declinerVariete_clicked()
{
    RefusMotif motifRefus(ui->tableWidget_validationVariete->item(ui->tableWidget_validationVariete->currentRow(),0)->text().toInt(),"Variete");
    motifRefus.exec();
}


QString MainWindow::getMaxVariete()
{
    QSqlQuery maxNumVarietetRequest("SELECT IFNULL((SELECT MAX(numeroVariete)+1 FROM Variete),0)");
    maxNumVarietetRequest.first();

    QString maxNumVariete = maxNumVarietetRequest.value(0).toString();

    return maxNumVariete;
}
