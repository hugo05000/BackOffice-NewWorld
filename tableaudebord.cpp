#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QtCharts>

/**
 * @brief MainWindow::afficheTableaudebord()
 * Affiche l'ensemble des éléments dans l'onglet "Tableau de bord".
 */

void MainWindow::afficheTableaudebord()
{
    nouveauxInscrits();
    nouvellesVarietes();
    ldpMisEnVente();

    QSqlQuery comboboxVarieteRequest("SELECT * FROM Rayon");

    while(comboboxVarieteRequest.next())
    {
     ui->comboBox_rayonTDB->addItem(comboboxVarieteRequest.value("nomRayon").toString(),
                                    comboboxVarieteRequest.value("numeroRayon").toInt());
    }
}

/**
 * @brief MainWindow::nouveauxInscrits()
 * Affiche les nouveaux inscrits dans le graphique correspondant.
 */

void MainWindow::nouveauxInscrits()
{
    QSqlQuery nbrNouveauInscritRequest("SELECT DATE_FORMAT(dateAjoutProducteur,'%M') AS mois,COUNT(numeroProducteur) "
                                       "FROM producteur "
                                       "WHERE YEAR(dateAjoutProducteur)=YEAR(NOW()) "
                                       "GROUP BY mois");
    nbrNouveauInscritRequest.exec();

    QBarSet *set0 = new QBarSet("Nouveaux inscrits");

    QBarSeries *series = new QBarSeries();
    QStringList categories;

    series->append(set0);

    while(nbrNouveauInscritRequest.next())
    {
        categories << nbrNouveauInscritRequest.value(0).toString();
        *set0 << nbrNouveauInscritRequest.value(1).toInt();
    }

     QChart *chart = new QChart();
     chart->addSeries(series);
     chart->setTitle("Nouveaux inscrits / mois");
     chart->setAnimationOptions(QChart::SeriesAnimations);


     QBarCategoryAxis *axisX = new QBarCategoryAxis();
     axisX->append(categories);
     chart->addAxis(axisX, Qt::AlignBottom);
     series->attachAxis(axisX);

     QValueAxis *axisY = new QValueAxis();
     axisY->setRange(0,10);
     chart->addAxis(axisY, Qt::AlignLeft);
     series->attachAxis(axisY);

     chart->legend()->setVisible(true);
     chart->legend()->setAlignment(Qt::AlignBottom);

     QChartView *chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);
     chartView->resize(420, 300);

     ui->scrollArea_nouveauxInscrits->setWidget(chartView);
}

/**
 * @brief MainWindow::nouvellesVarietes()
 * Affiche les nouvelles variétés dans le graphique correspondant.
 */

void MainWindow::nouvellesVarietes()
{
    QSqlQuery nouvellesVarietesRequest("SELECT DATE_FORMAT(dateActivationVariete,'%M') AS mois,COUNT(numeroVariete) "
                                       "FROM variete "
                                       "WHERE YEAR(dateActivationVariete)=YEAR(NOW()) "
                                       "GROUP BY mois");
    nouvellesVarietesRequest.exec();

    QBarSet *set0 = new QBarSet("Nouvelles Variétés");

    QBarSeries *series = new QBarSeries();
    QStringList categories;

    series->append(set0);

    while(nouvellesVarietesRequest.next())
    {
        categories << nouvellesVarietesRequest.value(0).toString();
        *set0 << nouvellesVarietesRequest.value(1).toInt();
    }

     QChart *chart = new QChart();
     chart->addSeries(series);
     chart->setTitle("Nouvelles variétés / mois");
     chart->setAnimationOptions(QChart::SeriesAnimations);


     QBarCategoryAxis *axisX = new QBarCategoryAxis();
     axisX->append(categories);
     chart->addAxis(axisX, Qt::AlignBottom);
     series->attachAxis(axisX);

     QValueAxis *axisY = new QValueAxis();
     axisY->setRange(0,10);
     chart->addAxis(axisY, Qt::AlignLeft);
     series->attachAxis(axisY);

     chart->legend()->setVisible(true);
     chart->legend()->setAlignment(Qt::AlignBottom);

     QChartView *chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);
     chartView->resize(420, 300);

     ui->scrollArea_nouvellesVarietes->setWidget(chartView);
}

/**
 * @brief MainWindow::ldpMisEnVente()
 * Affiche les nouveaux lots de production dans le graphique correspondant.
 */

void MainWindow::ldpMisEnVente()
{
    QSqlQuery lotDeProductionrequest("SELECT DATE_FORMAT(dateDebutSemaine,'%M') AS mois,COUNT(prixUnitaire) "
                                     "FROM Produire "
                                     "WHERE YEAR(dateDebutSemaine)=YEAR(NOW()) "
                                     "GROUP BY mois");
    lotDeProductionrequest.exec();

    QBarSet *set0 = new QBarSet("Lots de productions mis en vente");

    QBarSeries *series = new QBarSeries();
    QStringList categories;

    series->append(set0);

    while(lotDeProductionrequest.next())
    {
        categories << lotDeProductionrequest.value(0).toString();
        *set0 << lotDeProductionrequest.value(1).toInt();
    }

     QChart *chart = new QChart();
     chart->addSeries(series);
     chart->setTitle("Lots de productions mis en vente / mois");
     chart->setAnimationOptions(QChart::SeriesAnimations);


     QBarCategoryAxis *axisX = new QBarCategoryAxis();
     axisX->append(categories);
     chart->addAxis(axisX, Qt::AlignBottom);
     series->attachAxis(axisX);

     QValueAxis *axisY = new QValueAxis();
     axisY->setRange(0,10);
     chart->addAxis(axisY, Qt::AlignLeft);
     series->attachAxis(axisY);

     chart->legend()->setVisible(true);
     chart->legend()->setAlignment(Qt::AlignBottom);

     QChartView *chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);
     chartView->resize(420, 300);

     ui->scrollArea_ldp->setWidget(chartView);
}

/**
 * @brief MainWindow::on_comboBox_rayonTDB_currentTextChanged()
 * Affiche le nombre de variétés dans le rayon lors du changement d'index du menu déroulant correspondant.
 */

void MainWindow::on_comboBox_rayonTDB_currentTextChanged(const QString &arg1)
{
    QSqlQuery varieteRayonRequest("SELECT COUNT(numeroVariete) FROM Variete WHERE numeroRayon="+ui->comboBox_rayonTDB->currentData().toString());
    varieteRayonRequest.first();

    ui->lcdNumber_varietesParRayons->display(varieteRayonRequest.value(0).toInt());
}
