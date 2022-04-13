#ifndef REFUSPRODUIT_H
#define REFUSPRODUIT_H

#include <QDialog>

namespace Ui {
class Refusproduit;
}

class Refusproduit : public QDialog
{
    Q_OBJECT

public:
    explicit Refusproduit(int numProduit, QWidget *parent = nullptr);
    ~Refusproduit();

private slots:
    void on_pushButton_valider_clicked();

    void on_pushButton_annuler_clicked();

private:
    Ui::Refusproduit *ui;
    QString getMaxNumRefusProduit();
    int numero;
    QString table;
};

#endif // REFUSPRODUIT_H
