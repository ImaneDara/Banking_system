#include "accountmodel.h"

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
                  "(clientId, number, type, balance) VALUES "
                  "(:idClient, :number, :type, :balance)");

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
                  "type=:type, balance=:balance "
                  "WHERE id=:id");

    query.bindValue(":number", account.getNumber());
    query.bindValue(":balance", account.getBalance());
    query.bindValue(":type", account.getType());
    query.bindValue(":id", account.getId());

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

        accounts.push_back(account);
    }

    dbManager->close();

    return accounts;
}

void AccountModel::readAll(int clientId)
{
    dbManager->open();

    QSqlDatabase database = dbManager->database();

    this->setQuery("SELECT id, clientId, number, type, balance"
                   " FROM t_accounts WHERE clientId=:clientId", database);
    this->query().bindValue(":clientId", clientId);
    setHeaderTitle();

    dbManager->close();
}

void AccountModel::readAll()
{
    dbManager->open();

    QSqlDatabase database = dbManager->database();

    this->setQuery("SELECT id, clientId, number, type, balance"
                   " FROM t_accounts", database);
    setHeaderTitle();

    dbManager->close();
}

void AccountModel::readBy(int clientId)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());
    query.prepare("SELECT id, clientId, number, type, balance "
                  "FROM t_accounts WHERE clientId=:clientId");
    query.bindValue(":clientId", clientId);
    query.exec();
    this->setQuery(query);
    setHeaderTitle();
    dbManager->close();
}

void AccountModel::setHeaderTitle()
{
    this->setHeaderData(0, Qt::Horizontal, tr("Account Id"));
    this->setHeaderData(1, Qt::Horizontal, tr("Client Id"));
    this->setHeaderData(2, Qt::Horizontal, tr("Account number"));
    this->setHeaderData(3, Qt::Horizontal, tr("Account type"));
    this->setHeaderData(4, Qt::Horizontal, tr("Balance"));
}
