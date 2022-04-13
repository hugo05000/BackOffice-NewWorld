#ifndef CONFIRMERSUPPRESSION_H
#define CONFIRMERSUPPRESSION_H

#include <QDialog>

namespace Ui {
class ConfirmerSuppression;
}

class ConfirmerSuppression : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmerSuppression(QWidget *parent = nullptr);
    ~ConfirmerSuppression();

private slots:
    void on_pushButton_supprimer_clicked();

    void on_pushButton_Annuler_clicked();

private:
    Ui::ConfirmerSuppression *ui;
};

#endif // CONFIRMERSUPPRESSION_H
