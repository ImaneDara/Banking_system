#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QString>

class Account
{
private:
    int id;
    int idClient;
    QString number;
    QString type;
    double balance;

public:
    int getId () {return id;}
    int getIdClient () {return idClient;}
    QString getNumber () { return number;}
    QString getType () { return type;}
    double getBalance () { return balance;}

    void setId (int id) {this->id = id > 0 ? id : this->id;}
    void setIdClient (int idClient) {this->idClient = idClient > 0 ? idClient : this->idClient;}
    void setNumber (QString number) {this->number = number;}
    void setType (QString type) {this->type = type;}
    void setBalance (double balance) {this->balance = balance >= 0 ? balance : this->balance;}

    Account();
    Account(int idClient, QString number, QString type);
    Account(int idClient, QString number, QString type, double balance);
    Account(int id, int idClient, QString number, QString type, double balance);
    Account(int id, int idClient, QString number, QString type);
};

#endif // ACCOUNT_H
