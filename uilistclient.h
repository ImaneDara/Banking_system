#ifndef UILISTCLIENT_H
#define UILISTCLIENT_H

#include <QMainWindow>
#include <uiabstractwindow.h>
#include <usermodel.h>

namespace Ui {
class UIListClient;
}

class UIListClient : public QMainWindow, public UIAbstractWindow
{
    Q_OBJECT

public:
    explicit UIListClient(QWidget *parent = nullptr);
    UIListClient(QObject* controller);
    void setTableViewModel(UserModel* clientModel);
    void updateTitle(QString libelle);
    void top();
    ~UIListClient();

private:
    Ui::UIListClient *ui;
};

#endif // UILISTCLIENT_H
