#include "account.h"

Account::Account(int _id, int _idClient, QString _number, QString _type) :
    Account(_id, _idClient, _number, _type, 0.00, "actif") {}

Account::Account(int _idClient, QString _number, QString _type) :
    Account(_idClient, _number, _type, 0.00) {}

Account::Account(int _idClient, QString _number, QString _type, double _balance) :
    idClient(_idClient), number(_number), type(_type), balance(_balance), statut("actif") {}

Account::Account(int id, int idClient, QString number, QString type, double balance) :
    Account(id, idClient, number, type, balance, "actif") {}

Account::Account(int id, int idClient, QString number, QString type, double balance, QString statut) :
    id(id), idClient(idClient), number(number), type(type), balance(balance), statut(statut) {}

Account::Account(int idClient, QString number, QString type, double balance, QString statut) :
    idClient(idClient), number(number), type(type), balance(balance), statut(statut) {}

Account::Account() : statut("actif") {}
