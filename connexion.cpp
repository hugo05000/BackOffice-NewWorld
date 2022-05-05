#include "connexion.h"
#include "ui_connexion.h"
#include <QSqlQuery>
#include <QDebug>

/**
 * @brief Connexion::Connexion
 * Initialise la boîte de dialogue.
 */

Connexion::Connexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connexion)
{
    ui->setupUi(this);
    ui->label_motDePasseIncorrect->setVisible(0);
}

/**
 * @brief Destructeur de la classe
 * Destructeur de la classe Connexion.
 */

Connexion::~Connexion()
{
    delete ui;
}

/**
 * @brief Connexion::on_pushButton_connexion_clicked()
 * Valide ou non la connexion à l'application en recherchant l'utilisateur dans la base de données.
 */

void Connexion::on_pushButton_connexion_clicked()
{
    QSqlQuery verification("SELECT COUNT(loginEmploye) "
                           "FROM Employe "
                           "WHERE loginEmploye='"+ui -> lineEdit_login -> text().replace("'","\'").replace(";","")+"' AND mdp=SHA2('"+ui -> lineEdit_mdp -> text().replace("'","\'").replace(";","")+"',256)");

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

/**
 * @brief Connexion::getIdUtilisateur()
 * Récupère l'id de l'utilisateur.
 */

QString Connexion::getIdUtilisateur()
{
    QSqlQuery idUtilisateurRequest("SELECT numEmploye FROM Employe WHERE loginEmploye='"+ui->lineEdit_login->text().replace("'","\'").replace(";","")+"'");
    idUtilisateurRequest.first();

    QString idEmploye = idUtilisateurRequest.value("numEmploye").toString();

    return idEmploye;
}
