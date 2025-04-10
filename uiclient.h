#ifndef UICLIENT_H
#define UICLIENT_H

#include "accountmodel.h"
#include "uiprofile.h"
#include <uiabstractwindow.h>
#include <QFormLayout>
#include <QLabel>
#include <QMainWindow>
#include "uiprofile.h"

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
    void showProfileForm(const QMap<QString, QString>& clientData);
    void showProfileForm();
    void hideProfileForm();
    void setClientInfo(const QMap<QString, QString>& clientData);

    QString getNumeroBeneficiaire();
    QString getMontant ();
    QString getTypeTransaction() {  return typeTransaction; }

    ~UIClient();

private:
    QString typeTransaction;
    Ui::UIClient *ui;

    uiprofile* profileWindow;

    QWidget* profileWidget;
    QFormLayout* profileLayout;
    QLabel* profileLabels[8]; // Adjust size based on how many fields you have
    QLabel* profileValues[8];
};

#endif // UICLIENT_H
