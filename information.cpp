#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmersuppression.h"
#include <QSqlQuery>
#include <QCryptographicHash>

void MainWindow::affichageInformations()
{
    QSqlQuery informationsRequest("SELECT * FROM Employe WHERE numEmploye="+numUser);
    informationsRequest.first();
    QSqlQuery typeEmployeRequest("SELECT * FROM typeEmploye WHERE numeroTypeEmploye="+informationsRequest.value("numeroTypeEmploye").toString());
    typeEmployeRequest.first();

    ui->label_login->setText(informationsRequest.value("numEmploye").toString());
    ui->label_nom->setText(informationsRequest.value("nomEmploye").toString());
    ui->label_prenom->setText(informationsRequest.value("prenomEmploye").toString());
    ui->label_typeEmploye->setText(typeEmployeRequest.value("libelletypeEmploye").toString());
    ui->lineEdit_cp->setText(informationsRequest.value("cp").toString());
    ui->lineEdit_adresse->setText(informationsRequest.value("adresse").toString());
    ui->lineEdit_mail->setText(informationsRequest.value("mail").toString());
    ui->lineEdit_telephone->setText(informationsRequest.value("telephone").toString());
    ui->lineEdit_changemenMdp->setPlaceholderText("Au moins 12 caractères, 1 Majuscule, 1 minuscule, 1 caractère spécial.");
    ui->lineEdit_confirmationMdp->setVisible(0);
    ui->label_confirmationMdp->setVisible(0);
}

void MainWindow::on_pushButton_modifierInformations_clicked()
{
    if(ui->lineEdit_confirmationMdp->text().isEmpty()) {
        QSqlQuery modifierInformatinsRequest("UPDATE Employe SET "
                                         "cp='"+ui->lineEdit_cp->text().replace("'","\'").replace(";","")+
                                         "',adresse='"+ui->lineEdit_adresse->text().replace("'","\'").replace(";","")+
                                         "',mail='" +ui->lineEdit_mail->text().replace("'","\'").replace(";","")+
                                         "',telephone='"+ui->lineEdit_telephone->text().replace("'","\'").replace(";","")+
                                         "' WHERE numEmploye="+numUser);

        if(modifierInformatinsRequest.numRowsAffected() > 0) {
            affichageInformations();
            ui->statusBar->showMessage("Informations modifiées avec succès !");
        } else{
            ui->statusBar->showMessage("Erreur lors de la modification des ifnormations. Veuillez vérifier les informations saisies");
        }
    } else {
        if(ui->lineEdit_changemenMdp->text() == ui->lineEdit_confirmationMdp->text()) {
            if(ui->lineEdit_changemenMdp->text().count() > 11) {
                QString hash = QString(QCryptographicHash::hash((ui->lineEdit_changemenMdp->text().replace("'","\'").replace(";","")).toUtf8(),QCryptographicHash::Sha256).toHex());
                QSqlQuery modifierInformatinsMdpRequest("UPDATE Employe SET "
                                                 "cp='"+ui->lineEdit_cp->text().replace("'","\'").replace(";","")+
                                                 "',adresse='"+ui->lineEdit_adresse->text().replace("'","\'").replace(";","")+
                                                 "',mail='" +ui->lineEdit_mail->text().replace("'","\'").replace(";","")+
                                                 "',telephone='"+ui->lineEdit_telephone->text().replace("'","\'").replace(";","")+
                                                 "',mdp='"+hash+
                                                 "' WHERE numEmploye="+numUser);
                if(modifierInformatinsMdpRequest.numRowsAffected() > 0) {
                    affichageInformations();
                    ui->statusBar->showMessage("Informations modifiées avec succès !");
                } else{
                    ui->statusBar->showMessage("Erreur lors de la modification des ifnormations. Veuillez vérifier les informations saisies");
                }
            } else {
                ui->statusBar->showMessage("Erreur lors de la modification des informations. Le nouveau mot de passe est trop court (12 caractères minimum).");
            }
        }
        else {
            ui->statusBar->showMessage("Erreur lors de la modification des ifnormations. Les mots de passe ne correspondent pas.");
        }
    }
}

void MainWindow::on_lineEdit_changemenMdp_textEdited(const QString &arg1)
{
    ui->lineEdit_confirmationMdp->setVisible(1);
    ui->label_confirmationMdp->setVisible(1);
}
