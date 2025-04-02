#ifndef UILISTUSER_H
#define UILISTUSER_H

#include <QMainWindow>
#include <uiabstractwindow.h>
#include <usermodel.h>

namespace Ui {
class UIListUser;
}

class UIListUser : public QMainWindow, public UIAbstractWindow
{
    Q_OBJECT

public:
    explicit UIListUser(QWidget *parent = nullptr);
    UIListUser(QObject* controller);
    void setTableViewModel(UserModel* userModel);
    void updateTitle(QString libelle);
    void top();
    ~UIListUser();

private:
    Ui::UIListUser *ui;
};

#endif // UILISTUSER_H
