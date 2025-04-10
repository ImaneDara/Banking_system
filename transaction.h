#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>

class Transaction
{
private:
    int id;
    int idClient;
    QString type;
    int idCompteTire;
    int idCompteBeneficiaire;
    QString numeroCompteTire;
    QString numeroCompteBeneficiaire;
    double montant;
    QString date;
    QString statut;

public:
    Transaction (int id,
        int idClient,
        QString type,
        int idCompteTire,
        int idCompteBeneficiaire,
        QString numeroCompteTire,
        QString numeroCompteBeneficiaire,
        double montant,
        QString date,
        QString statut);

    Transaction (QString type,
        int idClient,
        int idCompteTire,
        int idCompteBeneficiaire,
        QString numeroCompteTire,
        QString numeroCompteBeneficiaire,
        double montant,
        QString date,
        QString statut);

    Transaction () {}

    int getId () { return id; }
    int getIdClient () { return idClient; }
    QString getType () { return type; }
    int getIdCompteTire () { return idCompteTire; }
    int getIdCompteBeneficiaire () { return idCompteBeneficiaire; }
    QString getNumeroCompteTire () { return numeroCompteTire; }
    QString getNumeroCompteBeneficiaire () { return numeroCompteBeneficiaire; }
    double getMontant () { return montant; }
    QString getDate () { return date; }
    QString getStatut() { return statut; }

    void setId (int id) { this->id = id; }
    void setIdClient (int id) { this->idClient = id; }
    void setType (QString type) { this->type = type; }
    void setIdCompteBeneficiaire (int idCompteBeneficiaire) { this->idCompteBeneficiaire = idCompteBeneficiaire; }
    void setIdCompteTire (int idCompteTire) { this->idCompteTire = idCompteTire; }
    void setNumeroCompteTire (QString numeroCompteTire) { this->numeroCompteTire = numeroCompteTire; }
    void setNumeroCompteBeneficiaire (QString numeroCompteBeneficiaire) { this->numeroCompteBeneficiaire = numeroCompteBeneficiaire; }
    void setMontant (double montant) { this->montant = montant; }
    void setDate (QString date) { this->date = date; }
    void setStatut (QString statut) { this->statut = statut; }
};

#endif // TRANSACTION_H
