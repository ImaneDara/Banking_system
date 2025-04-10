#ifndef UIUSER_H
#define UIUSER_H

#include <QMainWindow>
#include <uiabstractwindow.h>

namespace Ui {
class UIUser;
}

class UIUser : public QMainWindow, public UIAbstractWindow
{
    Q_OBJECT

public:
    explicit UIUser(QWidget *parent = nullptr);
    /*
     * Ce second constructeur permet de connecter le contrÃ´leur Ã  la
     * fenÃªtre */
    UIUser(QObject* controller);

    QString getNom();
    QString getPassword();
    QString getCountry();
    QString getBirthdate();
    QString getEmail();
    QString getStatut();
    QString getRole();
    QString getLogin();

    void reinit();
    void deactivateUpdate();
    void activateUpdate();

    void populate (QString identifiant, QString nom, QString country, QString birthdate, QString login, QString password
                   , QString role, QString email, QString statut);
    void hideStatut ();
    void showStatut ();
    void show();
    void setRoleEditable(bool editable);
    void updateTitle (QString libelle);

    ~UIUser();

private:
    Ui::UIUser *ui;
};

#endif // UIUSER_H
