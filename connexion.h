#ifndef CONNEXION_H
#define CONNEXION_H

#include <QDialog>

/**
 * @brief Classe Connexion
 * Classe fille de la classe QDialog
 */

namespace Ui {
class Connexion;
}

class Connexion : public QDialog
{
    Q_OBJECT

public:
    explicit Connexion(QWidget *parent = nullptr);
    ~Connexion();
    QString getIdUtilisateur();

private slots:
    void on_pushButton_connexion_clicked();

private:
    Ui::Connexion *ui;
    int cptEchec=0;

};

#endif // CONNEXION_H
