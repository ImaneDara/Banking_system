#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include "dbmanager.h"
#include <QtDebug>
#include <QMetaType>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include "account.h"

class AccountModel : public QSqlQueryModel
{
private:
    DBManager* dbManager;
    // Permet d'avoir à tout moment les informations
    // sur l'élélement sélectionné sur la vue.
    QItemSelectionModel* selectionModel;

public:
    QItemSelectionModel* getSelectionModel() { return selectionModel; }

    void create(Account account);
    void update(Account account);
    Account read(int id);

    QList<Account> list();

    // Rafraîchit la collection et la tableView ...
    void readAll(int clientId);
    void readAll();

    void readBy(int clientId);
    void setHeaderTitle();

    AccountModel();
};

#endif // ACCOUNTMODEL_H
