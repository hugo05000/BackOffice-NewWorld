#include "mainwindow.h"
#include "connexion.h"
#include <QApplication>
#include <QSqlDatabase>

/**
 * @brief main
 * Connexion à la base de données
 * @return Ce que renvoie l'application
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Patatestore");
    db.setUserName("hugo");
    db.setPassword("elini01");
    db.open();

    Connexion seConnecter;
    if(seConnecter.exec()==QDialog::Accepted)
    {
        MainWindow w(seConnecter.getIdUtilisateur());
        w.show();

        return a.exec();
    }
    else {
        return 20;
    }
}
