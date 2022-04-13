#include "connexion.h"
#include "ui_connexion.h"
#include <QSqlQuery>
#include <QDebug>


Connexion::Connexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connexion)
{
    ui->setupUi(this);
    ui->label_motDePasseIncorrect->setVisible(0);
}

Connexion::~Connexion()
{
    delete ui;
}

void Connexion::on_pushButton_connexion_clicked()
{
    QSqlQuery verification("SELECT COUNT(loginEmploye) "
                           "FROM Employe "
                           "WHERE loginEmploye='"+ui -> lineEdit_login -> text()+"' AND mdp=SHA2('"+ui -> lineEdit_mdp -> text()+"',256)");

    verification.first();
    int resultat = verification.value(0).toInt();

    if(resultat>0){
        accept();
    } else{
        cptEchec++;
        if(cptEchec==3)
        {
            close();
        }
        ui->label_motDePasseIncorrect->setVisible(1);
    }
}

QString Connexion::getIdUtilisateur()
{
    QSqlQuery idUtilisateurRequest("SELECT numEmploye FROM Employe WHERE loginEmploye='"+ui->lineEdit_login->text()+"'");
    idUtilisateurRequest.first();

    QString idEmploye = idUtilisateurRequest.value("numEmploye").toString();

    return idEmploye;
}
