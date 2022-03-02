#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>

void MainWindow::affichageProduits()
{
    //On initialise le tableau
    QStringList tableLabels;
    tableLabels << "Numéro" << "Nom" << "Informations" << "Image" << "Quantité" << "Variété";
    ui->tableWidget_produits->clear();
    ui->tableWidget_produits->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_produits->setRowCount(0);
    ui->tableWidget_produits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery lesProduitsrequest("SELECT * FROM produit");

    lesProduitsrequest.exec();

    int cptProduit=0;
    while(lesProduitsrequest.next())
    {
        ui->tableWidget_produits->insertRow(cptProduit);
        ui->tableWidget_produits->setItem(cptProduit,0,new QTableWidgetItem(lesProduitsrequest.value(0).toString()));
        ui->tableWidget_produits->setItem(cptProduit,1,new QTableWidgetItem(lesProduitsrequest.value(1).toString()));
        ui->tableWidget_produits->setItem(cptProduit,2,new QTableWidgetItem(lesProduitsrequest.value(2).toString()));
        ui->tableWidget_produits->setItem(cptProduit,3,new QTableWidgetItem(lesProduitsrequest.value(3).toString()));
        ui->tableWidget_produits->setItem(cptProduit,4,new QTableWidgetItem(lesProduitsrequest.value(4).toString()));
        ui->tableWidget_produits->setItem(cptProduit,5,new QTableWidgetItem(lesProduitsrequest.value(5).toString()));
        ui->tableWidget_produits->setColumnHidden(1,true);
        ui->tableWidget_produits->setColumnHidden(5,true);
        cptProduit++;
    }

    ui->comboBox_variete->clear();
    QSqlQuery lesVarietesRequest("SELECT * FROM variete");

    int cptvariete = 0;
    while(lesVarietesRequest.next())
    {
        ui->comboBox_variete->addItem(lesVarietesRequest.value("nomVariete").toString(),
                                      lesVarietesRequest.value("numeroVariete").toInt());

        cptvariete++;
    }
}

void MainWindow::on_tableWidget_produits_cellClicked(int row, int column)
{
    ui->lineEdit_nomProduit->setText(ui->tableWidget_produits->item(row,1)->text());
    ui->lineEdit_informations->setText(ui->tableWidget_produits->item(row,2)->text());
    ui->lineEdit_image->setText(ui->tableWidget_produits->item(row,3)->text());
    ui->lineEdit_quantitee->setText(ui->tableWidget_produits->item(row,4)->text());
    ui->comboBox_variete->setCurrentIndex(ui->comboBox_variete->findData(ui->tableWidget_produits->item(row,5)->text().toInt()));
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    QSqlQuery ajouterProduitRequest("INSERT INTO produit VALUES("
                                    +getMaxProduit()+",'"
                                    +ui->lineEdit_nomProduit->text()+"','"
                                    +ui->lineEdit_informations->text()+"','"
                                    +ui->lineEdit_image->text()+"',"
                                    +ui->lineEdit_quantitee->text()+","
                                    +ui->comboBox_variete->currentData().toString()+")");

    if(ajouterProduitRequest.numRowsAffected() > 0){
        affichageProduits();
        ui->statusBar->showMessage("Le produit a bien été ajouté");
    }else{
        ui->statusBar->showMessage("Erreur lors de l'ajout du produit. Veuillez vérifier les informations saisies");
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{

}

QString MainWindow::getMaxProduit()
{
    QSqlQuery maxNumProduitRequest("SELECT IFNULL((SELECT MAX(numeroProduit)+1 FROM produit),0)");
    maxNumProduitRequest.first();

    QString maxNumProduit = maxNumProduitRequest.value(0).toString();

    return maxNumProduit;
}
