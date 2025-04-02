#include "service.h"
#include "user.h"
#include <QtDebug>
#include <QMessageBox>

Service::Service(UserModel* _userModel) :
    userModel(_userModel){}

Service::Service(UserModel* _userModel, AccountModel* _accountModel) :
    userModel(_userModel), accountModel(_accountModel){}

Service::Service(UserModel* _userModel, AccountModel* _accountModel, TransactionModel* _transactionModel) :
    userModel(_userModel), accountModel(_accountModel), transactionModel (_transactionModel) {}

Role Service::authentifier(QString login, QString password)
{
    qDebug () << "Service::authentifier " << login << "-" << password << "";
    User user = userModel->readBy(login, password);
    return user.getEnumRole();
}

bool Service::authentifier(QString login, QString password, User& user)
{
    qDebug () << "Service::authentifier " << login << "-" << password << "";
    user = userModel->readBy(login, password);
    QString userRole = user.getRole();
    return userRole.compare("NOT_DEFINED") == 0 ? false : true;
}

void Service::ajouterUnUser(QString nom, QString login, QString password, QString country, QString birthdate, QString email, QString role, QString statut)
{
    User user(nom, login, password, country, birthdate, email, role, statut);
    userModel->create(user);
}

void Service::modifierUnUser(QString nom, QString login, QString password, QString country, QString birthdate, QString email, QString role, QString statut)
{
    QModelIndex selectedIndex = userModel->getSelectionModel()->currentIndex();
    int selectedLine = selectedIndex.row();

    QSqlRecord selectedRecord = userModel->record(selectedLine);
    QSqlField fieldId = selectedRecord.field(0);

    int selectedId = fieldId.value().toInt();

    User user(selectedId, nom, login, password, country, birthdate, email, role, statut);
    userModel->update(user);
}

bool Service::rechercherUnUser(int id)
{
    User user = userModel->read(id);
    if (user.getId() == -1)
    {
        return false;
    }
    else
    {
        userModel->readBy(id); // Afficher sur le tableView la User trouvée !!!
        return true;
    }
}

void Service::supprimerUnUser()
{
    userModel->remove();
}

void Service::listerLesUsers()
{
    userModel->getSelectionModel()->reset();
    userModel->readAll();
}

void Service::listerLesClients()
{
    userModel->getSelectionModel()->reset();
    userModel->readAllClients();
}

void Service::listerLesComptes(int clientId)
{
    accountModel->getSelectionModel()->reset();    
    accountModel->readBy(clientId);
}

void Service::listerLesTransactions()
{
    transactionModel->getSelectionModel()->reset();
    transactionModel->readAll();
}

void Service::listerLesTransactions(int clientId)
{
    transactionModel->getSelectionModel()->reset();
    transactionModel->readBy(clientId);
}


void Service::listerLesTransactionsDuCompte()
{
    QModelIndex selectedIndex = accountModel->getSelectionModel()->currentIndex();
    int selectedLine = selectedIndex.row();
    QSqlRecord selectedRecord = accountModel->record(selectedLine);
    QSqlField fieldId = selectedRecord.field(0);
    int accountId = fieldId.value().toInt();

    transactionModel->getSelectionModel()->reset();
    transactionModel->readAll(accountId);
}

bool Service::effectuerUnRetrait(int idClient, double montant)
{
    QModelIndex selectedIndex = accountModel->getSelectionModel()->currentIndex();
    int selectedLine = selectedIndex.row();
    QSqlRecord selectedRecord = accountModel->record(selectedLine);
    QSqlField fieldId = selectedRecord.field(0);
    int accountId = fieldId.value().toInt();

    Account account = accountModel->read(accountId);
    if (account.getBalance() < montant)
    {
        return false;
    }

    QDate today = QDate::currentDate();
    QTime now = QTime::currentTime();

    account.setBalance(account.getBalance() - montant);
    accountModel->update(account);
    accountModel->readBy(idClient);

    Transaction transaction ("Retrait", idClient, accountId, -1, account.getNumber(), "NULL", montant, today.toString("yyyy-MM-ddT") + now.toString("HH:mm:ss.zzz"), "Completed");
    transactionModel->create(transaction);

    return true;
}

void Service::effectuerUnVersement(int idClient, double montant)
{
    QModelIndex selectedIndex = accountModel->getSelectionModel()->currentIndex();
    int selectedLine = selectedIndex.row();
    QSqlRecord selectedRecord = accountModel->record(selectedLine);
    QSqlField fieldId = selectedRecord.field(0);
    int accountId = fieldId.value().toInt();

    Account account = accountModel->read(accountId);
    QDate today = QDate::currentDate();
    QTime now = QTime::currentTime();

    account.setBalance(account.getBalance() + montant);
    accountModel->update(account);
    accountModel->readBy(idClient);

    Transaction transaction ("Versement", idClient, -1, accountId, "NULL", account.getNumber(), montant, today.toString("yyyy-MM-ddT") + now.toString("HH:mm:ss.zzz"), "Completed");
    transactionModel->create(transaction);
}

void Service::effectuerUnVirement (int idClient, QString numeroCompteBeneficiaire, double montant)
{
    QModelIndex selectedIndex = accountModel->getSelectionModel()->currentIndex();
    int selectedLine = selectedIndex.row();
    QSqlRecord selectedRecord = accountModel->record(selectedLine);
    QSqlField fieldId = selectedRecord.field(0);
    int accountId = fieldId.value().toInt();

    Account account = accountModel->read(accountId);
    QDate today = QDate::currentDate();
    QTime now = QTime::currentTime();

    Transaction transaction ("Virement", idClient, accountId, -1, account.getNumber(), numeroCompteBeneficiaire, montant, today.toString("yyyy-MM-ddT") + now.toString("HH:mm:ss.zzz"), "In progress");
    transactionModel->create(transaction);
}

void Service::executeTransaction(QMap<QString, QString> input, bool &status, QString &message)
{
    QString typeTransaction = input.value("typeTransaction");
    QString connectedUserId = input.value("connectedUserId");
    QString montant = input.value("montant");
    QString numeroBeneficiaire = input.value("numeroBeneficiaire");

    if (typeTransaction.compare("VERSEMENT") == 0)
    {
        effectuerUnVersement(connectedUserId.toInt(), montant.toDouble());
        message = "Versement effectué avec succès.";
    }
    else if (typeTransaction.compare("RETRAIT") == 0)
    {
        status = effectuerUnRetrait(connectedUserId.toInt(), montant.toDouble());
        if (status) message = "Retrait effectué avvec succès.";
        else message = "**ERR** Erreur lors de la transaction, veuillez vous rapprocher de votre gestionnaire.";
    }
    else if (typeTransaction.compare("VIREMENT") == 0)
    {
        qDebug () << typeTransaction << "- Service::executeTransaction";
        effectuerUnVirement(connectedUserId.toInt(), numeroBeneficiaire, montant.toDouble());
        message = "Le virement a été enrégistré et est en cours de traitement ... vous recevrez une notification.";
    }
    else
    {
        qDebug () << "typeTransaction " << typeTransaction << " - aucune transaction elligible - Service::executeTransaction";
    }
}

void Service::ajouterUnCompte(QMap<QString, QString> input, QString &message)
{
    QString idClient = input.value("idClient");
    QString number = input.value("number");
    QString type = input.value("type");
    double balance = input.value("balance").toDouble(); // Conversion correcte

    // Vérifier si la balance est négative
    if (balance < 0) {
        message = "Erreur : La balance initiale doit être positive.";
        return; // Stoppe l'exécution
    }

    Account account(idClient.toInt(), number, type, balance);
    accountModel->create(account);

    listerLesComptes(idClient.toInt());

    // Message de succès
    message = "Compte ajouté avec succès.";
}



void Service::modifierUnCompte(QMap<QString, QString> input)
{
    QString accountId = input.value("accountId");
    QString idClient = input.value("idClient");
    QString number = input.value("number");
    QString balance = input.value("balance");
    QString type = input.value("type");

    Account account(accountId.toInt(), idClient.toInt(), number, type, balance.toInt());
    accountModel->update(account);

    listerLesComptes(idClient.toInt());
}
