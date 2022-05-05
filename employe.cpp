#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmersuppression.h"
#include <QSqlQuery>
#include <QFileDialog>
#include <QCryptographicHash>

/**
 * @brief MainWindow::affichageEmploye()
 * Affiche l'ensemble des éléments dans l'onglet "Employés"
 */

void MainWindow::affichageEmploye()
{
    ui->pushButton_supprimerEmploye->setDisabled(1);

    ui->lineEdit_mdpEmploye->setPlaceholderText("Selement pour ajouter un employé.");
    if(typeEmploye!=1)
    {
        ui->comboBox_typeEmploye->setVisible(0);
        ui->label_comboTypeEmploye->setVisible(0);
    }

    if(typeEmploye==3)
    {
        ui->tab_onglets->removeTab(0);
        ui->tab_onglets->removeTab(7);
    }

    //On initialise le tableau
    QStringList tableLabels;
    tableLabels << "Numéro" << "Login" << "Nom" << "Prénom" << "Code postal" << "Adresse" << "Mail" << "Téléphone" << "Type employé";
    ui->tableWidget_employe->clear();
    ui->tableWidget_employe->setHorizontalHeaderLabels(tableLabels);
    ui->tableWidget_employe->setRowCount(0);
    ui->tableWidget_employe->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery lesEmployesRequestequest;

    switch (typeEmploye)
    {
    //SuperAdministrateur
    case 1:
        //Recherche des administrateurs et des modérateurs
        lesEmployesRequestequest.exec("SELECT * FROM Employe WHERE numeroTypeEmploye=2 OR numeroTypeEmploye=3");
        break;
    //Administrateur
    case 2:
        //Recherche des modérateurs
        lesEmployesRequestequest.exec("SELECT * FROM Employe WHERE numeroTypeEmploye=3");
        break;
    default:
        break;
    }

    int cptEmploye=0;
    while(lesEmployesRequestequest.next())
    {
        ui->tableWidget_employe->insertRow(cptEmploye);
        ui->tableWidget_employe->setItem(cptEmploye,0,new QTableWidgetItem(lesEmployesRequestequest.value("numEmploye").toString()));
        ui->tableWidget_employe->setItem(cptEmploye,1,new QTableWidgetItem(lesEmployesRequestequest.value("loginEmploye").toString()));
        ui->tableWidget_employe->setItem(cptEmploye,2,new QTableWidgetItem(lesEmployesRequestequest.value("nomEmploye").toString()));
        ui->tableWidget_employe->setItem(cptEmploye,3,new QTableWidgetItem(lesEmployesRequestequest.value("prenomEmploye").toString()));
        ui->tableWidget_employe->setItem(cptEmploye,4,new QTableWidgetItem(lesEmployesRequestequest.value("cp").toString()));
        ui->tableWidget_employe->setItem(cptEmploye,5,new QTableWidgetItem(lesEmployesRequestequest.value("adresse").toString()));
        ui->tableWidget_employe->setItem(cptEmploye,6,new QTableWidgetItem(lesEmployesRequestequest.value("mail").toString()));
        ui->tableWidget_employe->setItem(cptEmploye,7,new QTableWidgetItem(lesEmployesRequestequest.value("telephone").toString()));

        QSqlQuery nomTypeEmployeRequest("SELECT libelleTypeEmploye FROM TypeEmploye WHERE numeroTypeEmploye="+lesEmployesRequestequest.value("numeroTypeEmploye").toString());
        nomTypeEmployeRequest.first();

        ui->tableWidget_employe->setItem(cptEmploye,8,new QTableWidgetItem(nomTypeEmployeRequest.value(0).toString()));
        cptEmploye++;
    }

    if(typeEmploye==1)
    {
        ui->comboBox_typeEmploye->clear();
        QSqlQuery typeEmployeRequest("SELECT * FROM TypeEmploye");

        while(typeEmployeRequest.next())
        {
            ui->comboBox_typeEmploye->addItem(typeEmployeRequest.value("libelletypeEmploye").toString(),
                                          typeEmployeRequest.value("numeroTypeEmploye").toInt());
        }
    }
}

/**
 * @brief MainWindow::on_tableWidget_employe_cellClicked()
 * Récupère les éléments de la ligne sélectionnée dans le tableau des employés pour compléter les zones de saisies et menus déroulants correspondants.
 */

void MainWindow::on_tableWidget_employe_cellClicked(int row, int column)
{
    ui->pushButton_supprimerEmploye->setEnabled(1);

    ui->lineEdit_loginEmploye->setText(ui->tableWidget_employe->item(row,1)->text());
    ui->lineEdit_nomEmploye->setText(ui->tableWidget_employe->item(row,2)->text());
    ui->lineEdit_prenomEmploye->setText(ui->tableWidget_employe->item(row,3)->text());
    ui->lineEdit_cpEmploye->setText(ui->tableWidget_employe->item(row,4)->text());
    ui->lineEdit_adresseEmploye->setText(ui->tableWidget_employe->item(row,5)->text());
    ui->lineEdit_mailEmploye->setText(ui->tableWidget_employe->item(row,6)->text());
    ui->lineEdit_telephoneEmploye->setText(ui->tableWidget_employe->item(row,7)->text());

    if(typeEmploye==1)
    {
        QSqlQuery rechercheTypeEmployeRequest("SELECT TypeEmploye.numeroTypeEmploye "
                                             "FROM TypeEmploye "
                                             "INNER JOIN Employe ON Employe.numeroTypeEmploye = TypeEmploye.numeroTypeEmploye "
                                             "WHERE numEmploye="+ui->tableWidget_employe->item(row,0)->text());

        rechercheTypeEmployeRequest.first();

        ui->comboBox_typeEmploye->setCurrentIndex(ui->comboBox_typeEmploye->findData(rechercheTypeEmployeRequest.value(0).toInt()));
    }
}

/**
 * @brief MainWindow::on_pushButton_ajouterEmploye_clicked()
 * Insère l'employé en base de données
 */

void MainWindow::on_pushButton_ajouterEmploye_clicked()
{
    //On vérifie qu'aucun employé a déjà le même login
    QSqlQuery verifLoginRequest("SELECT * FROM Employe WHERE loginEmploye='"+ui->lineEdit_loginEmploye->text()+"'");

    if(verifLoginRequest.first()==0) {
        QString hash = QString(QCryptographicHash::hash((ui->lineEdit_mdpEmploye->text()).toUtf8(),QCryptographicHash::Sha256).toHex());

        QSqlQuery ajouterEmployeRequest;

        if(typeEmploye==1) {
            ajouterEmployeRequest.exec("INSERT INTO Employe VALUES("
                                            +getMaxEmploye()+",'"
                                            +ui->lineEdit_loginEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +ui->lineEdit_nomEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +ui->lineEdit_prenomEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +ui->lineEdit_cpEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +ui->lineEdit_adresseEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +ui->lineEdit_mailEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +ui->lineEdit_telephoneEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +hash+"',"
                                            +ui->comboBox_typeEmploye->currentData().toString()+")");
        } else if(typeEmploye==2) {
            ajouterEmployeRequest.exec("INSERT INTO Employe VALUES("
                                            +getMaxEmploye()+",'"
                                            +ui->lineEdit_loginEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +ui->lineEdit_nomEmploye->text().replace("'","\'").replace(";","")+"','"
                                            +ui->lineEdit_prenomEmploye->text().replace("'","\'").replace(";","")+"',"
                                            +ui->lineEdit_cpEmploye->text().replace("'","\'").replace(";","")+","
                                            +ui->lineEdit_adresseEmploye->text().replace("'","\'").replace(";","")+","
                                            +ui->lineEdit_mailEmploye->text().replace("'","\'").replace(";","")+","
                                            +ui->lineEdit_telephoneEmploye->text().replace("'","\'").replace(";","")+","
                                            +hash+","
                                            +"3)");
        }

        if(ajouterEmployeRequest.numRowsAffected() > 0){
            affichageEmploye();
            ui->statusBar->showMessage("L'Employé a bien été ajouté");
        } else{
            ui->statusBar->showMessage("Erreur lors de l'ajout de l'employé. Veuillez vérifier les informations saisies");
        }
    } else{
        ui->statusBar->showMessage("Erreur lors de l'ajout de l'employé. Un employé avec le même login existe déjà.");
    }
}

/**
 * @brief MainWindow::on_pushButton_modifierEmploye_clicked()
 * Modifie l'employé en base de données
 */

void MainWindow::on_pushButton_modifierEmploye_clicked()
{
    QSqlQuery modifierEmployeRequest;

    if(typeEmploye==1) {
        modifierEmployeRequest.exec("UPDATE Employe SET "
                                         "loginEmploye='"+ui->lineEdit_loginEmploye->text().replace("'","\'").replace(";","")+
                                         "',nomEmploye='"+ui->lineEdit_nomEmploye->text().replace("'","\'").replace(";","")+
                                         "',prenomEmploye='" +ui->lineEdit_prenomEmploye->text().replace("'","\'").replace(";","")+
                                         "',cp='"+ui->lineEdit_cpEmploye->text().replace("'","\'").replace(";","")+
                                         "',adresse='"+ui->lineEdit_adresseEmploye->text().replace("'","\'").replace(";","")+
                                         "',mail='"+ui->lineEdit_mailEmploye->text().replace("'","\'").replace(";","")+
                                         "',telephone='"+ui->lineEdit_telephoneEmploye->text().replace("'","\'").replace(";","")+
                                         "',numeroTypeEmploye="+ui->comboBox_typeEmploye->currentData().toString()+
                                         " WHERE numEmploye="+ui->tableWidget_employe->item(ui->tableWidget_employe->currentRow(),0)->text());
    } else if(typeEmploye==2) {
        modifierEmployeRequest.exec("UPDATE Employe SET "
                                    "loginEmploye='"+ui->lineEdit_loginEmploye->text().replace("'","\'").replace(";","")+
                                    "',nomEmploye='"+ui->lineEdit_nomEmploye->text().replace("'","\'").replace(";","")+
                                    "',prenomEmploye='" +ui->lineEdit_prenomEmploye->text().replace("'","\'").replace(";","")+
                                    "',cp='"+ui->lineEdit_cpEmploye->text().replace("'","\'").replace(";","")+
                                    "',adresse='"+ui->lineEdit_adresseEmploye->text().replace("'","\'").replace(";","")+
                                    "',mail='"+ui->lineEdit_mailEmploye->text().replace("'","\'").replace(";","")+
                                    "',telephone='"+ui->lineEdit_telephoneEmploye->text().replace("'","\'").replace(";","")+
                                    " WHERE numEmploye="+ui->tableWidget_employe->item(ui->tableWidget_employe->currentRow(),0)->text().replace("'","\'").replace(";",""));
    }


    if(modifierEmployeRequest.numRowsAffected() > 0){
        affichageEmploye();
        ui->statusBar->showMessage("L'employé a bien été modifié");
    } else{
        ui->statusBar->showMessage("Erreur lors de la modification de l'employé. Veuillez vérifier les informations saisies");
    }
}

/**
 * @brief MainWindow::on_pushButton_supprimerEmploye_clicked()
 * Supprime l'employé en base de données
 */

void MainWindow::on_pushButton_supprimerEmploye_clicked()
{
    ConfirmerSuppression laSuppression;

    if (laSuppression.exec()==QDialog::Accepted)
    {
        QSqlQuery supprimerEmployeRequest("DELETE FROM Employe "
                                          "WHERE numEmploye="+ui->tableWidget_employe->item(ui->tableWidget_employe->currentRow(),0)->text().replace("'","\'").replace(";",""));

        if(supprimerEmployeRequest.numRowsAffected() > 0) {
            affichageEmploye();
            ui->statusBar->showMessage("L'employé a bien été supprimé.");
        } else{
            ui->statusBar->showMessage("Erreur lors de la suppression de l'employé.");
        }
    }
}

/**
 * @brief MainWindow::getMaxEmploye()
 * Recherche le plus grand id de la table employe +1
 * @return Renvoie l'id le plus grand +1
 */

QString MainWindow::getMaxEmploye()
{
    QSqlQuery maxNumEmployeRequest("SELECT IFNULL((SELECT MAX(numEmploye)+1 FROM Employe),0)");
    maxNumEmployeRequest.first();

    QString maxNumEmploye = maxNumEmployeRequest.value(0).toString();

    return maxNumEmploye;
}
