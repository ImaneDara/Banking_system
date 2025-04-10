#ifndef TRANSATIONMODEL_H
#define TRANSATIONMODEL_H
#include "dbmanager.h"
#include "transaction.h"
#include <QtDebug>
#include <QMetaType>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QModelIndex>

class TransactionModel : public QSqlQueryModel
{
private:
    DBManager* dbManager;
    // Permet d'avoir à tout moment les informations
    // sur l'élélement sélectionné sur la vue.
    QItemSelectionModel* selectionModel;
    int m_accountId = -1;
    QList<QSqlRecord> cachedTransactions; // Add this line to store transactions
    int currentAccountId = -1;

public:
    TransactionModel();
    ~TransactionModel() {}
    QItemSelectionModel* getSelectionModel() { return selectionModel; }
    void create(Transaction transaction);
    void refresh();
    void setAccountId(int id);
    QList<Transaction> list();

    // Rafraîchit la collection et la tableView ...
    void readAll();
    void readAll(int accountId);
    void setHeaderTitle();
    void readBy(int clientId);
    void filtrerTransactions(const QString& type, const QString& dateFilter, bool sortByBalanceAsc, int userId);
    void createNotificationForTransaction(Transaction transaction, int transactionId);

};

#endif // TRANSATIONMODEL_H
