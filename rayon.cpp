#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmersuppression.h"
#include <QSqlQuery>

void MainWindow::affichageRayon()
{
    QStringList tableLabels;
    tableLabels << "Numéro" << "Nom";
    ui->tableWidget_rayon->clear();
    ui->tableWidget_rayon->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_rayon->setRowCount(0);
    ui->tableWidget_rayon->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery affichageVarieteRequest("SELECT * FROM Rayon");

    affichageVarieteRequest.exec();

    int cptVariete=0;
    while(affichageVarieteRequest.next())
    {
        ui->tableWidget_rayon->insertRow(cptVariete);
        ui->tableWidget_rayon->setItem(cptVariete,0,new QTableWidgetItem(affichageVarieteRequest.value("numeroRayon").toString()));
        ui->tableWidget_rayon->setItem(cptVariete,1,new QTableWidgetItem(affichageVarieteRequest.value("nomRayon").toString()));
        cptVariete++;
    }

    ui->tableWidget_rayon->setColumnHidden(0,true);
}

void MainWindow::on_tableWidget_rayon_cellClicked(int row, int column)
{
    ui->lineEdit_rayon->setText(ui->tableWidget_rayon->item(row,1)->text());
}

void MainWindow::on_pushButton_ajouterRayon_clicked()
{
    QSqlQuery ajouterRayonRequest("INSERT INTO Rayon VALUES("
                                    +getMaxRayon()+",'"
                                    +ui->lineEdit_rayon->text()+"')");

    if(ajouterRayonRequest.numRowsAffected() > 0){
        affichageRayon();
        affichageVarietes();
        afficheTableaudebord();
        ui->statusBar->showMessage("Le rayon a bien été ajouté");
    } else{
        ui->statusBar->showMessage("Erreur lors de l'ajout du rayon. Veuillez vérifier les informations saisies");
    }
}

void MainWindow::on_pushButton_modifierRayon_clicked()
{
    QSqlQuery modifierRayonRequest("UPDATE Rayon SET "
                                     "nomRayon='"+ui->lineEdit_rayon->text()+
                                     "' WHERE numeroRayon="+ui->tableWidget_rayon->item(ui->tableWidget_rayon->currentRow(),0)->text());

    if(modifierRayonRequest.numRowsAffected() > 0){
        affichageRayon();
        affichageVarietes();
        ui->statusBar->showMessage("Le rayon a bien été modifié");
    } else{
        ui->statusBar->showMessage("Erreur lors de la modification du rayon. Veuillez vérifier les informations saisies");
    }
}

void MainWindow::on_pushButton_supprimerRayon_clicked()
{
    ConfirmerSuppression laSuppression;

    if (laSuppression.exec()==QDialog::Accepted)
    {
        QSqlQuery supprimerRayonRequest("DELETE FROM Rayon WHERE numeroRayon="+ui->tableWidget_rayon->item(ui->tableWidget_rayon->currentRow(),0)->text());

        if(supprimerRayonRequest.numRowsAffected() > 0) {
            affichageRayon();
            affichageVarietes();
            ui->statusBar->showMessage("Le rayon a bien été supprimé");
        } else{
            ui->statusBar->showMessage("Erreur lors de la suppression du rayon.");
        }
    }
}

QString MainWindow::getMaxRayon()
{
    QSqlQuery maxNumRayonRequest("SELECT IFNULL((SELECT MAX(numeroRayon)+1 FROM Rayon),0)");
    maxNumRayonRequest.first();

    QString maxNumRayon = maxNumRayonRequest.value(0).toString();

    return maxNumRayon;
}




