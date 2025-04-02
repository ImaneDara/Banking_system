#ifndef UILISTTRANSACTION_H
#define UILISTTRANSACTION_H

#include <QMainWindow>
#include <uiabstractwindow.h>
#include <transactionmodel.h>

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
    ~UIListTransaction();
private slots:
    void onFiltrerClicked();
private:
    Ui::UIListTransaction *ui;
};

#endif // UILISTTRANSACTION_H
