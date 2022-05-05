#ifndef REFUSMOTIF_H
#define REFUSMOTIF_H

#include <QDialog>

/**
 * @brief Classe RefusMotif
 * Classe fille de la classe QDialog
 */

namespace Ui {
class RefusMotif;
}

class RefusMotif : public QDialog
{
    Q_OBJECT

public:
    explicit RefusMotif(int numero, QString table, QWidget *parent = nullptr);
    ~RefusMotif();

private slots:
    void on_pushButton_valider_clicked();

    void on_pushButton_annuler_clicked();

private:
    Ui::RefusMotif *ui;
    int numero;
    QString table;
    QString getMaxTable();
};

#endif // REFUSMOTIF_H
