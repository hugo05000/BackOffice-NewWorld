#include "mainwindow.h"
#include "connexion.h"
#include <QApplication>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Patatestore");
    db.setUserName("userHugo");
    db.setPassword("elini01");
    db.open();

    Connexion seConnecter;
    if(seConnecter.exec()==QDialog::Accepted)
    {
        MainWindow w;
        w.show();

        return a.exec();
    }
    else {
        return 20;
    }
}