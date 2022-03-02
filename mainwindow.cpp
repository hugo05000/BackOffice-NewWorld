#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "produits.cpp"
#include <QSqlQuery>
#include <QVector>
#include <QDebug>

MainWindow::MainWindow(int numUtilisateur, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    numUser = numUtilisateur;
    affichageProduits();

}

MainWindow::~MainWindow()
{
    delete ui;
}

