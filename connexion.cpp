#include "connexion.h"
#include "ui_connexion.h"
#include <QSqlQuery>


Connexion::Connexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connexion)
{
    ui->setupUi(this);
}

Connexion::~Connexion()
{
    delete ui;
}

void Connexion::verifIdentifiant() {
    QSqlQuery verification("SELECT COUNT(loginEmploye) "
                           "FROM Employe "
                           "WHERE loginEmploye='"+ui -> lineEdit_login -> text()+"' AND mdp=PASSWORD('"+ui -> lineEdit_mdp -> text()+"')");

    verification.first();
    QString resultat = verification.value(0).toString();


}
