#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlQuery>
#include <QVector>
#include <QDebug>

/**
 * @brief Constructeur de la classe
 * Lance l'ensemble des affichages et récupère le type de l'utilisateur
 * @param numUtilisateur : numéro de l'utilisateur connecté
 */

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

/**
 * @brief Destructeur de la classe
 * Destructeur de la classe MainWindow
 */

MainWindow::~MainWindow()
{
    delete ui;
}

