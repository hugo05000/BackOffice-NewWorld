#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlQuery>
#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QString numUtilisateur, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Numéro de l'utilisateur
    numUser = numUtilisateur;

    //Recherche du type de l'employé
    QSqlQuery typeEmployeRequest("SELECT numeroTypeEmploye FROM Employe WHERE numEmploye="+numUser);
    typeEmployeRequest.first();

    typeEmploye = typeEmployeRequest.value(0).toInt();

    //Initialisation des affichages
    affichageProduits();
    affichageVarietes();
    affichageRayon();
    affichageProducteur();
    affichageAbonnement();
    affichageInformations();
    affichageEmploye();
    afficheTableaudebord();
}

MainWindow::~MainWindow()
{
    delete ui;
}
