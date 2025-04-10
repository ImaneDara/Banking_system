#include "accountmodel.h"
#include <QDebug>          // For qCritical(), qDebug()
#include <QSqlError>       // For QSqlError (used by lastError())
#include <QSqlQuery>

AccountModel::AccountModel()
{
    selectionModel = new QItemSelectionModel(this);
    dbManager = DBManager::getInstance();
}

void AccountModel::create(Account account)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("INSERT INTO t_accounts "
                  "(clientId, number, type, balance, statut) VALUES "
                  "(:idClient, :number, :type, :balance, 'ACTIF')");

    query.bindValue(":idClient", account.getIdClient());
    query.bindValue(":number", account.getNumber());
    query.bindValue(":type", account.getType());
    query.bindValue(":balance", account.getBalance());

    query.exec();
    dbManager->close();

    qDebug("Account added successfully !");
    readAll(account.getIdClient()); // recupère les nouvelles données de la base ...
}

void AccountModel::update(Account account)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("UPDATE t_accounts SET "
                  "number=:number, "
                  "type=:type, balance=:balance, "
                  "statut=:statut "  // Added statut
                  "WHERE id=:id");

    query.bindValue(":number", account.getNumber());
    query.bindValue(":balance", account.getBalance());
    query.bindValue(":type", account.getType());
    query.bindValue(":id", account.getId());
    query.bindValue(":statut", account.getStatut());

    query.exec();

    dbManager->close();

    readAll(account.getIdClient()); // recupère les nouvelles données de la base ...
}

Account AccountModel::read(int id)
{
    Account account; // constructeur par défaut !!!

    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT * FROM t_accounts WHERE id=:id");
    query.bindValue(":id", id);

    query.exec();

    if (query.next())
    {
        qDebug () << "account found!";

        account.setId(query.record().field("id").value().toInt());
        account.setIdClient(query.record().field("clientId").value().toInt());
        account.setNumber(query.record().field("number").value().toString());
        account.setType(query.record().field("type").value().toString());
        account.setBalance(query.record().field("balance").value().toDouble());
        account.setStatut(query.record().field("statut").value().toString());
    }
    else
    {
        qDebug () << "account not found!";
    }

    dbManager->close();

    return account;
}

QList<Account> AccountModel::list()
{
    Account account;
    QList<Account> accounts;

    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT * FROM t_accounts WHERE 1");
    query.exec();

    while (query.next())
    {
        account.setId(query.record().field("id").value().toInt());
        account.setIdClient(query.record().field("clientId").value().toInt());
        account.setNumber(query.record().field("number").value().toString());
        account.setType(query.record().field("type").value().toString());
        account.setBalance(query.record().field("balance").value().toDouble());
        account.setStatut(query.record().field("statut").value().toString());

        accounts.push_back(account);
    }

    dbManager->close();

    return accounts;
}

void AccountModel::readAll(int clientId)
{
    dbManager->open();
    QSqlDatabase database = dbManager->database();

    QSqlQuery query(database);
    query.prepare("SELECT id, clientId, number, type, balance, statut FROM t_accounts WHERE clientId = :clientId");
    query.bindValue(":clientId", clientId);

    this->setQuery(std::move(query)); // Use std::move to pass the query
    setHeaderTitle();
    dbManager->close();
}

void AccountModel::readAll()
{
    dbManager->open();

    QSqlDatabase database = dbManager->database();

    this->setQuery("SELECT id, clientId, number, type, balance, statut FROM t_accounts", database);
    setHeaderTitle();

    dbManager->close();
}

void AccountModel::readBy(int clientId)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());
    query.prepare("SELECT id, clientId, number, type, balance, statut FROM t_accounts WHERE clientId=:clientId");
    query.bindValue(":clientId", clientId);
    query.exec();
    this->setQuery(query);
    setHeaderTitle();
    dbManager->close();
}

Account AccountModel::readByAccountNumber(const QString &accountNumber)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());
    query.prepare("SELECT * FROM t_accounts WHERE number = :number");
    query.bindValue(":number", accountNumber);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return Account();
    }

    if (!query.next()) {
        qDebug() << "No account found with number:" << accountNumber;
        return Account();
    }

    Account account;
    account.setId(query.value("id").toInt());
    account.setNumber(query.value("number").toString());
    account.setBalance(query.value("balance").toDouble());
    account.setStatut(query.value("statut").toString());
    // Set other account properties as needed

    return account;
}

void AccountModel::setHeaderTitle()
{
    this->setHeaderData(0, Qt::Horizontal, tr("Account Id"));
    this->setHeaderData(1, Qt::Horizontal, tr("Client Id"));
    this->setHeaderData(2, Qt::Horizontal, tr("Account number"));
    this->setHeaderData(3, Qt::Horizontal, tr("Account type"));
    this->setHeaderData(4, Qt::Horizontal, tr("Balance"));
    this->setHeaderData(5, Qt::Horizontal, tr("Statut"));
}

