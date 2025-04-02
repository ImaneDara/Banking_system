#ifndef UILISTACCOUNT_H
#define UILISTACCOUNT_H

#include <QMainWindow>
#include <uiabstractwindow.h>
#include <accountmodel.h>

namespace Ui {
class UIListAccount;
}

class UIListAccount : public QMainWindow, public UIAbstractWindow
{
    Q_OBJECT

public:
    explicit UIListAccount(QWidget *parent = nullptr);
    UIListAccount(QObject* controller);
    void setTableViewModel(AccountModel* accountModel);
    void updateTitle(QString libelle);
    void updateAccountTitle(QString nomClient);
    void top();
    ~UIListAccount();

private:
    Ui::UIListAccount *ui;
};

#endif // UILISTACCOUNT_H
