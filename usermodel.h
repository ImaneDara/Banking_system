#ifndef USERMODEL_H
#define USERMODEL_H
#include "dbmanager.h"
#include "user.h"
#include <QtDebug>
#include <QMetaType>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QModelIndex>

class UserModel : public QSqlQueryModel
{
private:
    DBManager* dbManager;
    // Permet d'avoir à tout moment les informations
    // sur l'élélement sélectionné sur la vue.
    QItemSelectionModel* selectionModel;

public:
    UserModel();
    ~UserModel() {}
    QItemSelectionModel* getSelectionModel() { return selectionModel; }
    void create(User person);
    void update(User person);
    User read(int id);
    void remove (int id);
    QList<User> list();

    // Supprime l'element seletionne dans le model
    // L'element est obtenu a partir de l'objet
    // QItemSelectionModel -> QModelIndex
    void remove();

    // Rafraîchit la collection et la tableView ...
    void readAll();
    void readAllClients();
    void readBy(int id);
    void readBy(QString str);
    User readBy(QString login, QString password);
    void setHeaderTitle();
    void setClientHeaderTitle();
};

#endif // USERMODEL_H
