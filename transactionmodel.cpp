#include "transactionmodel.h"

TransactionModel::TransactionModel()
{    
    selectionModel = new QItemSelectionModel(this);
    dbManager = DBManager::getInstance();
}

void TransactionModel::create(Transaction transaction)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("INSERT INTO t_transactions "
                  "(idClient, type, idCompteTire, idCompteBeneficiaire, numeroCompteTire, numeroCompteBeneficiaire, montant, date, statut) VALUES "
                  "(:idClient, :type, :idCompteTire, :idCompteBeneficiaire, :numeroCompteTire, :numeroCompteBeneficiaire, :montant, :date, :statut)");

    query.bindValue(":type", transaction.getType());
    query.bindValue(":idClient", transaction.getIdClient());
    query.bindValue(":idCompteTire", transaction.getIdCompteTire());
    query.bindValue(":idCompteBeneficiaire", transaction.getIdCompteBeneficiaire());
    query.bindValue(":numeroCompteTire", transaction.getNumeroCompteTire());
    query.bindValue(":numeroCompteBeneficiaire", transaction.getNumeroCompteBeneficiaire());
    query.bindValue(":montant", transaction.getMontant());
	query.bindValue(":date", transaction.getDate());
	query.bindValue(":statut", transaction.getStatut());    

    query.exec();
    dbManager->close();

    qDebug("Transaction added successfully !");
    readAll(); // recupère les nouvelles données de la base ...
}

QList<Transaction> TransactionModel::list()
{
    Transaction transaction;
    QList<Transaction> transactions;

    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT * FROM t_transactions WHERE 1");
    query.exec();

    while (query.next())
    {
        transaction.setId(query.record().field("id").value().toInt());
        transaction.setId(query.record().field("idClient").value().toInt());
		transaction.setType(query.record().field("type").value().toString());
        transaction.setIdCompteTire(query.record().field("idCompteTire").value().toInt());
		transaction.setIdCompteBeneficiaire(query.record().field("idCompteBeneficiaire").value().toInt());
        transaction.setNumeroCompteTire(query.record().field("numeroCompteTire").value().toString());
		transaction.setNumeroCompteBeneficiaire(query.record().field("numeroCompteBeneficiaire").value().toString());		
        transaction.setMontant(query.record().field("montant").value().toDouble());
        transaction.setDate(query.record().field("date").value().toString());
        transaction.setStatut(query.record().field("statut").value().toString());
        
        transactions.push_back(transaction);
    }

    dbManager->close();

    return transactions;
}

void TransactionModel::readAll()
{
    dbManager->open();

    QSqlDatabase database = dbManager->database();

    this->setQuery("SELECT id, type, numeroCompteTire,"
					" numeroCompteBeneficiaire, montant, date, statut"
					" FROM t_transactions", database);
    setHeaderTitle();

    dbManager->close();
}

void TransactionModel::readAll(int accountId)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT id, type, numeroCompteTire,"
                  " numeroCompteBeneficiaire, montant, date, statut FROM t_transactions WHERE idCompteTire=:accountId OR idCompteBeneficiaire=:accountId");
    query.bindValue(":accountId", accountId);
    query.exec();

    this->setQuery(query);
    setHeaderTitle();

    dbManager->close();
}

void TransactionModel::readBy(int clientId)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT id, type, numeroCompteTire,"
                  " numeroCompteBeneficiaire, montant, date, statut FROM t_transactions WHERE idClient=:idClient");
    query.bindValue(":idClient", clientId);
    query.exec();

    this->setQuery(query);
    setHeaderTitle();

    dbManager->close();
}
void TransactionModel::filtrerTransactions(const QString& type)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    if (type == "Tous") {
        query.prepare("SELECT id, type, numeroCompteTire, numeroCompteBeneficiaire, montant, date, statut FROM t_transactions");
    } else {
        query.prepare("SELECT id, type, numeroCompteTire, numeroCompteBeneficiaire, montant, date, statut FROM t_transactions WHERE type = :type");
        query.bindValue(":type", type);
    }

    query.exec();
    this->setQuery(query);
    setHeaderTitle();

    dbManager->close();
}

void TransactionModel::setHeaderTitle()
{
    this->setHeaderData(0, Qt::Horizontal, tr("Id Transaction"));
    this->setHeaderData(1, Qt::Horizontal, tr("Type"));
    this->setHeaderData(2, Qt::Horizontal, tr("Compte Tire"));
    this->setHeaderData(3, Qt::Horizontal, tr("Compte Beneficiaire"));
    this->setHeaderData(4, Qt::Horizontal, tr("Montant"));
    this->setHeaderData(5, Qt::Horizontal, tr("Date"));
    this->setHeaderData(6, Qt::Horizontal, tr("Statut"));
}
