#ifndef SERVICE_H
#define SERVICE_H
#include <QString>
#include <QDate>
#include "usermodel.h"
#include "accountmodel.h"
#include "transactionmodel.h"

class Service
{

public:
    Role authentifier (QString login, QString password);
    bool authentifier(QString login, QString password, User& user);

    void ajouterUnUser(QString nom, QString login, QString password, QString country, QString birthdate, QString email, QString role, QString statut);
    void modifierUnUser(QString nom, QString login, QString password, QString country, QString birthdate, QString email, QString role, QString statut);
    void ajouterUnCompte(QMap<QString, QString> input,QString &message);
    void modifierUnCompte(QMap<QString, QString> input);
    void supprimerUnUser();
    bool rechercherUnUser(int id);
    void listerLesUsers();
    void listerLesClients();
    void listerLesTransactions();
    void listerLesTransactionsDuCompte();
    void listerLesComptes(int clientId);
    void listerLesTransactions(int clientId);
    // -
    void executeTransaction (QMap<QString, QString> input, bool &status, QString &message);

    Service(UserModel* userModel);
    Service(UserModel* userModel, AccountModel* accountModel);
    Service(UserModel* userModel, AccountModel* accountModel, TransactionModel* transactionModel);

private:
    UserModel* userModel;
    AccountModel* accountModel;
    TransactionModel* transactionModel;

    bool effectuerUnRetrait(int idClient, double montant);
    void effectuerUnVersement(int idClient, double montant);
    void effectuerUnVirement (int idClient, QString numeroCompteBeneficiaire, double montant);
};

#endif // SERVICE_H
