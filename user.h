#ifndef USER_H
#define USER_H
#include <QString>

enum Role {CLIENT, ADMINISTRATEUR, GESTIONNAIRE, INCONNU};

class User
{
private:
    int id;
    QString nom;
    QString login;
    QString password;
    QString country;
    QString birthdate;
    QString email;
    QString role;
    QString statut;

public:

    int getId() {return id; };
    QString getNom() {return nom; };
    QString getPassword() {return password; };
    QString getLogin() {return login; };
    QString getEmail() {return email; };
    QString getRole() {return role; };
    Role getEnumRole();
    QString getStatut() {return statut; };
    QString getCountry() {return country; };
    QString getBirthdate() {return birthdate; };

    void setId(int id) {this->id = id;}
    void setNom (QString nom) {this->nom = nom;}
    void setLogin (QString login) {this->login = login;}
    void setPassword (QString password) {this->password = password;}
    void setRole (QString role) {this->role = role;}
    void setStatut (QString statut) {this->statut = statut;}
    void setEmail (QString email) {this->email = email;}
    void setCountry (QString country) {this->country = country;}
    void setBirthdate (QString birthdate) {this->birthdate = birthdate;}

    User();
    User(QString nom, QString login, QString password, QString country, QString birthdate, QString email, QString role, QString statut);
    User(int id, QString nom, QString login, QString password, QString country, QString birthdate, QString email, QString role, QString statut);
};

#endif // USER_H

