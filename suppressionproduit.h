#ifndef SUPPRESSIONPRODUIT_H
#define SUPPRESSIONPRODUIT_H

#include <QDialog>

namespace Ui {
class SuppressionProduit;
}

class SuppressionProduit : public QDialog
{
    Q_OBJECT

public:
    explicit SuppressionProduit(QWidget *parent = nullptr);
    ~SuppressionProduit();

private slots:
    void on_pushButton_supprimer_clicked();

    void on_pushButton_Annuler_clicked();

private:
    Ui::SuppressionProduit *ui;
};

#endif // SUPPRESSIONPRODUIT_H
