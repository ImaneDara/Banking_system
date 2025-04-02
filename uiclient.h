#ifndef UICLIENT_H
#define UICLIENT_H

#include "accountmodel.h"
#include <uiabstractwindow.h>
#include <QMainWindow>

namespace Ui {
class UIClient;
}

class UIClient : public QMainWindow, public UIAbstractWindow
{
    Q_OBJECT

public:
    explicit UIClient(QWidget *parent = nullptr);
    UIClient(QObject* controller);
    void setTableViewModel(AccountModel* accountModel);
    void updateTitle (QString libelle);
    void showTransactionForm(QString labelCurrentTransaction, QString typeTransaction);
    void hideTransactionForm();

    QString getNumeroBeneficiaire();
    QString getMontant ();
    QString getTypeTransaction() {  return typeTransaction; }

    ~UIClient();

private:
    QString typeTransaction;
    Ui::UIClient *ui;
};

#endif // UICLIENT_H
