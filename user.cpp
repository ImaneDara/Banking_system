#include "user.h"

User::User()
{
    this->setId(-1);
    this->setRole("NOT_DEFINED");
}

User::User(QString _nom, QString _login, QString _password, QString _country, QString _birthdate, QString _email, QString _role, QString _statut) :
    nom(_nom), login(_login), password(_password), country(_country), birthdate(_birthdate), email(_email), role(_role), statut(_statut)
{}
User::User(int _id, QString _nom, QString _login, QString _password, QString _country, QString _birthdate, QString _email, QString _role, QString _statut) :
    id (_id), nom(_nom), login(_login), password(_password), country(_country), birthdate(_birthdate), email(_email), role(_role), statut(_statut)
{}

Role User::getEnumRole()
{
    if (role.compare("GESTIONNAIRE") == 0)
        return GESTIONNAIRE;
    else if (role.compare("CLIENT") == 0)
            return CLIENT;
    else if (role.compare("ADMINISTRATEUR") == 0)
            return ADMINISTRATEUR;

        return INCONNU;
};
