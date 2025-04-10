#ifndef UILOGININ_H
#define UILOGININ_H

#include <uiabstractwindow.h>
#include <QMainWindow>

namespace Ui {
class UILoginIn;
}

class UILoginIn : public QMainWindow, public UIAbstractWindow
{
    Q_OBJECT

public:
    explicit UILoginIn(QWidget *parent = nullptr);

    // Ce second constructeur permet de connecter le controleur à la fenêtre
    UILoginIn(QObject* controller);
    void updateTitle(QString libelle);

    QString getLogin();
    QString getPassword();

    void reinit();

    ~UILoginIn();

private:
    Ui::UILoginIn *ui;
};

#endif // UILOGININ_H
