#ifndef UILISTTRANSACTION_H
#define UILISTTRANSACTION_H

#include <QMainWindow>
#include <uiabstractwindow.h>
#include <transactionmodel.h>
#include "service.h"

namespace Ui {
class UIListTransaction;
}

class UIListTransaction : public QMainWindow, public UIAbstractWindow
{
    Q_OBJECT

public:
    explicit UIListTransaction(QWidget *parent = nullptr);
    UIListTransaction(QObject* controller);
    void setTableViewModel(TransactionModel* transactionModel);
    void updateTitle(QString suite);
    void updateTitle(QString suite, QString TransactionTitle);
    void top();
    void setAccountNumber(const QString& accountNumber);
    void disableActionButtons();
    void enableActionButtons();
    QString getUserAccountNumber();
    QString getSelectedFilter();
    QString getTransactionType();
    QString getDateStr();
    TransactionModel* getTransactionModel() const;
    bool validateSelectedTransaction();
    bool rejectSelectedTransaction();
    QModelIndex getSelectedTransactionIndex() const;
    Transaction* getSelectedTransaction() const;
    ~UIListTransaction();

private:
    Ui::UIListTransaction *ui;
    Ui::UIListTransaction *uiListTransactionUi;
    DBManager* dbManager;
    Transaction userAccountNumber;
    QString currentAccountNumber;
    QString currentUserRole;
};

#endif // UILISTTRANSACTION_H
