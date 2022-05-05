#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmersuppression.h"
#include "refusmotif.h"
#include <QSqlQuery>
#include <QFileDialog>

/**
 * @brief MainWindow::affichageProduits()
 * Affiche l'ensemble des éléments dans l'onglet "Produits"
 */

void MainWindow::affichageProduits()
{
    ui->pushButton_supprimer->setDisabled(1);

    //On initialise le tableau
    QStringList tableLabels;
    tableLabels << "Numéro" << "Nom" << "Informations" << "Image" << "Quantité" << "Variété" << "Image";
    ui->tableWidget_produits->clear();
    ui->tableWidget_produits->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_produits->setRowCount(0);
    ui->tableWidget_produits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery lesProduitsrequest("SELECT * FROM produit WHERE activationProduit=1");

    lesProduitsrequest.exec();

    int cptProduit=0;
    while(lesProduitsrequest.next())
    {
        QLabel* sonImage = new QLabel();
        sonImage->setPixmap(QPixmap(lesProduitsrequest.value("imageProduit").toString()));
        sonImage->setScaledContents(true);
        sonImage->setStyleSheet("max-height:150px;");

        ui->tableWidget_produits->insertRow(cptProduit);
        ui->tableWidget_produits->setItem(cptProduit,0,new QTableWidgetItem(lesProduitsrequest.value("numeroProduit").toString()));
        ui->tableWidget_produits->setItem(cptProduit,1,new QTableWidgetItem(lesProduitsrequest.value("nomProduit").toString()));
        ui->tableWidget_produits->setItem(cptProduit,2,new QTableWidgetItem(lesProduitsrequest.value("informationProduit").toString()));
        ui->tableWidget_produits->setCellWidget(cptProduit,3,sonImage);
        ui->tableWidget_produits->setItem(cptProduit,4,new QTableWidgetItem(lesProduitsrequest.value("quantiteProduit").toString()));
        ui->tableWidget_produits->setItem(cptProduit,5,new QTableWidgetItem(lesProduitsrequest.value("numeroVariete").toString()));
        ui->tableWidget_produits->setItem(cptProduit,6,new QTableWidgetItem(lesProduitsrequest.value("imageProduit").toString()));
        cptProduit++;
    }

    ui->tableWidget_produits->setColumnHidden(0,true);
    ui->tableWidget_produits->setColumnHidden(5,true);
    ui->tableWidget_produits->setColumnHidden(6,true);
    ui->tableWidget_produits->resizeColumnsToContents();
    ui->tableWidget_produits->resizeRowsToContents();

    ui->comboBox_variete->clear();

    QSqlQuery lesVarietesRequest("SELECT * FROM variete");

    while(lesVarietesRequest.next())
    {
        ui->comboBox_variete->addItem(lesVarietesRequest.value("nomVariete").toString(),
                                      lesVarietesRequest.value("numeroVariete").toInt());
    }

    ui->tableWidget_validationProduit->clear();
    ui->tableWidget_validationProduit->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_validationProduit->setRowCount(0);
    ui->tableWidget_validationProduit->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery produitAttenteRequest("SELECT * FROM produit WHERE activationProduit=0");

    produitAttenteRequest.exec();

    int cptActivation=0;
    while(produitAttenteRequest.next())
    {
        //On vérifie si le produit n'a été refusé
        QSqlQuery refusProduitRequest("SELECT numRefusProduit FROM refusProduit WHERE numRefusProduit="+produitAttenteRequest.value("numeroProduit").toString());
        refusProduitRequest.first();

        //On affiche seulement le produit s'il n'est pas refusé
        if(refusProduitRequest.size() == 0)
        {
            QLabel* sonImage = new QLabel();
            sonImage->setPixmap(QPixmap(produitAttenteRequest.value("imageProduit").toString()));
            sonImage->setScaledContents(true);
            sonImage->setStyleSheet("max-height:150px;");

            ui->tableWidget_validationProduit->insertRow(cptActivation);
            ui->tableWidget_validationProduit->setItem(cptActivation,0,new QTableWidgetItem(produitAttenteRequest.value("numeroProduit").toString()));
            ui->tableWidget_validationProduit->setItem(cptActivation,1,new QTableWidgetItem(produitAttenteRequest.value("nomProduit").toString()));
            ui->tableWidget_validationProduit->setItem(cptActivation,2,new QTableWidgetItem(produitAttenteRequest.value("informationProduit").toString()));
            ui->tableWidget_validationProduit->setCellWidget(cptActivation,3,sonImage);
            ui->tableWidget_validationProduit->setItem(cptActivation,4,new QTableWidgetItem(produitAttenteRequest.value("quantiteProduit").toString()));
            ui->tableWidget_validationProduit->setItem(cptActivation,5,new QTableWidgetItem(produitAttenteRequest.value("numeroVariete").toString()));
            ui->tableWidget_validationProduit->setColumnHidden(0,true);
            ui->tableWidget_validationProduit->setColumnHidden(5,true);
            cptActivation++;
        }
    }
}

/**
 * @brief MainWindow::on_tableWidget_produits_cellClicked()
 * Récupère les éléments de la ligne sélectionnée dans le tableau des produits pour compléter les zones de saisies et menus déroulants correspondants.
 */

void MainWindow::on_tableWidget_produits_cellClicked(int row, int column)
{
    ui->pushButton_supprimer->setEnabled(1);

    ui->lineEdit_nomProduit->setText(ui->tableWidget_produits->item(row,1)->text());
    ui->lineEdit_informations->setText(ui->tableWidget_produits->item(row,2)->text());
    ui->lineEdit_quantitee->setText(ui->tableWidget_produits->item(row,4)->text());
    ui->lineEdit_image->setText(ui->tableWidget_produits->item(row,6)->text());
    ui->comboBox_variete->setCurrentIndex(ui->comboBox_variete->findData(ui->tableWidget_produits->item(row,5)->text().toInt()));
}

/**
 * @brief MainWindow::on_pushButton_ajouter_clicked()
 * Insère le produit en base de données.
 */

void MainWindow::on_pushButton_ajouter_clicked()
{
    QSqlQuery ajouterProduitRequest("INSERT INTO produit VALUES("
                                    +getMaxProduit()+",'"
                                    +ui->lineEdit_nomProduit->text().replace("'","\'").replace(";","")+"','"
                                    +ui->lineEdit_informations->text().replace("'","\'").replace(";","")+"','"
                                    +ui->lineEdit_image->text().replace("'","\'").replace(";","")+"',"
                                    +ui->lineEdit_quantitee->text().replace("'","\'").replace(";","")+","
                                    +ui->comboBox_variete->currentData().toString()+")");

    if(ajouterProduitRequest.numRowsAffected() > 0){
        affichageProduits();
        ui->statusBar->showMessage("Le produit a bien été ajouté");
    } else{
        ui->statusBar->showMessage("Erreur lors de l'ajout du produit. Veuillez vérifier les informations saisies");
    }
}

/**
 * @brief MainWindow::on_pushButton_modifier_clicked()
 * Modifie le produit en base de données.
 */

void MainWindow::on_pushButton_modifier_clicked()
{
    QSqlQuery modifierProduitRequest("UPDATE produit SET "
                                     "nomProduit='"+ui->lineEdit_nomProduit->text().replace("'","\'").replace(";","")+
                                     "',informationProduit='"+ui->lineEdit_informations->text().replace("'","\'").replace(";","")+
                                     "',imageProduit='" +ui->lineEdit_image->text().replace("'","\'").replace(";","")+
                                     "',quantiteProduit="+ui->lineEdit_quantitee->text()+
                                     ",numeroVariete=" +ui->comboBox_variete->currentData().toString().replace("'","\'").replace(";","")+
                                     " WHERE numeroProduit="+ui->tableWidget_produits->item(ui->tableWidget_produits->currentRow(),0)->text().replace("'","\'").replace(";",""));

    if(modifierProduitRequest.numRowsAffected() > 0){
        affichageProduits();
        ui->statusBar->showMessage("Le produit a bien été modifié");
    } else{
        ui->statusBar->showMessage("Erreur lors de la modification du produit. Veuillez vérifier les informations saisies");
    }
}

/**
 * @brief MainWindow::on_pushButton_supprimer_clicked()
 * supprime le produit en base de données.
 */


void MainWindow::on_pushButton_supprimer_clicked()
{
    ConfirmerSuppression laSuppression;

    if (laSuppression.exec()==QDialog::Accepted)
    {
        QSqlQuery supprimerProduitRequest("DELETE FROM produit "
                                          "WHERE numeroProduit="+ui->tableWidget_produits->item(ui->tableWidget_produits->currentRow(),0)->text().replace("'","\'").replace(";",""));

        if(supprimerProduitRequest.numRowsAffected() > 0) {
            affichageProduits();
            ui->statusBar->showMessage("Le produit a bien été supprimé");
        } else{
            ui->statusBar->showMessage("Erreur lors de la suppression du produit.");
        }
    }
}

/**
 * @brief MainWindow::on_pushButton_supprimer_clicked()
 * supprime le produit en base de données.
 */

void MainWindow::on_pushButton_image_clicked()
{
    QString image = QFileDialog::getOpenFileName(this,tr("Image"), "/home", tr("Images (*.png *.jpeg *.jpg"));

    ui->lineEdit_image->setText(image);
}

/**
 * @brief MainWindow::on_pushButton_accepterProduit_clicked()
 * Accepte le produit en base de données.
 */

void MainWindow::on_pushButton_accepterProduit_clicked()
{
    QSqlQuery activationProduitRequest("UPDATE produit SET activationProduit=1 "
                                       "WHERE numeroProduit="+ui->tableWidget_validationProduit->item(ui->tableWidget_validationProduit->currentRow(),0)->text());
    if(activationProduitRequest.numRowsAffected() > 0) {
        affichageProduits();
        ui->statusBar->showMessage("Le produit a bien été validé");
    } else{
        ui->statusBar->showMessage("Un problème est survenu lors de la validation du produit");
    }
}

/**
 * @brief MainWindow::on_pushButton_declinerProduit_clicked()
 * Refuse le produit en base de données.
 */

void MainWindow::on_pushButton_declinerProduit_clicked()
{
    RefusMotif motifRefus(ui->tableWidget_validationProduit->item(ui->tableWidget_validationProduit->currentRow(),0)->text().toInt(),"Produit");
    motifRefus.exec();
}

/**
 * @brief MainWindow::getMaxProduit()
 * Recherche le plus grand id de la table produit +1
 * @return Renvoie l'id le plus grand +1
 */

QString MainWindow::getMaxProduit()
{
    QSqlQuery maxNumProduitRequest("SELECT IFNULL((SELECT MAX(numeroProduit)+1 FROM produit),0)");
    maxNumProduitRequest.first();

    QString maxNumProduit = maxNumProduitRequest.value(0).toString();

    return maxNumProduit;
}
