#ifndef UIACCOUNT_H
#define UIACCOUNT_H

#include <QMainWindow>
#include <uiabstractwindow.h>

namespace Ui {
class UIAccount;
}

class UIAccount : public QMainWindow, public UIAbstractWindow
{
    Q_OBJECT

public:
    explicit UIAccount(QWidget *parent = nullptr);
    /*
     * Ce second constructeur permet de connecter le contrôleur à la
     * fenêtre */
    UIAccount(QObject* controller);

    QString getIdClient();
    QString getAccountNumber();
    QString getType();
    QString getBalance();
    QString getAccountId();
    QString getStatut();

    void reinit();

    void populate (QString identifiant, QString idClient, QString accountNumber, QString type, QString balance, QString statut);
    void updateTitle (QString libelle);

    void deactivateCreate();
    void deactivateUpdate();

    ~UIAccount();

private:
    Ui::UIAccount *ui;
};

#endif // UIACCOUNT_H
