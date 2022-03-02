#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int numUtilisateur, QWidget *parent = nullptr);
    ~MainWindow();
    QString getMaxProduit();
    void affichageProduits();

private slots:
    void on_tableWidget_produits_cellClicked(int row, int column);

    void on_pushButton_ajouter_clicked();

    void on_pushButton_modifier_clicked();

private:
    Ui::MainWindow *ui;
    int numUser;

};

#endif // MAINWINDOW_H
