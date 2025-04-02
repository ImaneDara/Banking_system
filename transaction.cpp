#include "transaction.h"

Transaction::Transaction (int _id,
    int _idClient,
    QString _type,
    int _idCompteTire,
    int _idCompteBeneficiaire,
    QString _numeroCompteTire,
    QString _numeroCompteBeneficiaire,
    double _montant,
    QString _date,
    QString _statut) :
        id (_id),
        idClient (_idClient),
        type(_type),
        idCompteTire (_idCompteTire),
        idCompteBeneficiaire (_idCompteBeneficiaire),
        numeroCompteTire (_numeroCompteTire),
        numeroCompteBeneficiaire (_numeroCompteBeneficiaire),
        montant (_montant),
        date (_date),
        statut (_statut)
{}

Transaction::Transaction (QString _type,
    int _idClient,
    int _idCompteTire,
    int _idCompteBeneficiaire,
    QString _numeroCompteTire,
    QString _numeroCompteBeneficiaire,
    double _montant,
    QString _date,
    QString _statut) :
        idClient(_idClient),
        type(_type),
        idCompteTire (_idCompteTire),
        idCompteBeneficiaire (_idCompteBeneficiaire),
        numeroCompteTire (_numeroCompteTire),
        numeroCompteBeneficiaire (_numeroCompteBeneficiaire),
        montant (_montant),
        date (_date),
        statut (_statut)
{}
