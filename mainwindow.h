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
    explicit MainWindow(QString numUtilisateur, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tableWidget_produits_cellClicked(int row, int column);

    void on_pushButton_ajouter_clicked();

    void on_pushButton_modifier_clicked();

    void on_pushButton_supprimer_clicked();

    void on_pushButton_ajouterVariete_clicked();

    void on_pushButton_modifierVariete_clicked();

    void on_pushButton_supprimerVariete_clicked();

    void on_tableWidget_variete_cellClicked(int row, int column);

    void on_pushButton_supprimerRayon_clicked();

    void on_pushButton_modifierRayon_clicked();

    void on_pushButton_ajouterRayon_clicked();

    void on_tableWidget_rayon_cellClicked(int row, int column);

    void on_tableWidget_producteur_cellClicked(int row, int column);

    void on_pushButton_modifierProducteur_clicked();

    void on_pushButton_supprimerProducteur_clicked();

    void on_pushButton_supprimerAbonnement_clicked();

    void on_pushButton_ajouterAbonnement_clicked();

    void on_tableWidget_abonnement_cellClicked(int row, int column);

    void on_pushButton_modifierAbonnement_clicked();

    void on_pushButton_accepterProducteur_clicked();

    void on_pushButton_modifierInformations_clicked();

    void on_lineEdit_changemenMdp_textEdited(const QString &arg1);

    void on_pushButton_image_clicked();

    void on_tableWidget_employe_cellClicked(int row, int column);

    void on_pushButton_supprimerEmploye_clicked();

    void on_pushButton_modifierEmploye_clicked();

    void on_pushButton_ajouterEmploye_clicked();

    void on_pushButton_declinerProducteur_clicked();

    void on_pushButton_accepterProduit_clicked();

    void on_pushButton_declinerProduit_clicked();

    void on_pushButton_accepterVariete_clicked();

    void on_pushButton_declinerVariete_clicked();

    void on_comboBox_rayonTDB_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString numUser;
    int typeEmploye;
    QString getMaxProduit();
    QString getMaxVariete();
    QString getMaxRayon();
    QString getMaxAbonnement();
    QString getMaxEmploye();
    void affichageProduits();
    void affichageVarietes();
    void affichageRayon();
    void affichageProducteur();
    void affichageAbonnement();
    void affichageInformations();
    void affichageEmploye();
    void afficheTableaudebord();
    void nouveauxInscrits();
    void nouvellesVarietes();
    void ldpMisEnVente();
    void varietesParRayons();

};

#endif // MAINWINDOW_H
