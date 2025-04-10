#include "usermodel.h"

UserModel::UserModel()
{    
    selectionModel = new QItemSelectionModel(this);
    dbManager = DBManager::getInstance();
}

void UserModel::create(User user)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("INSERT INTO t_users "
                  "(nom, login, password, country, birthdate, email, role, statut) VALUES "
                  "(:nom, :login, :password, :country, :birthdate, :email, :role, :statut)");

    query.bindValue(":nom", user.getNom());
    query.bindValue(":login", user.getLogin());
    query.bindValue(":password", user.getPassword());
    query.bindValue(":country", user.getCountry());
    query.bindValue(":birthdate", user.getBirthdate());
    query.bindValue(":email", user.getEmail());
    query.bindValue(":role", user.getRole());
    query.bindValue(":statut", user.getStatut().compare("") == 0 ? "NULL" : user.getStatut());

    query.exec();
    dbManager->close();

    qDebug("User added successfully !");
    readAll(); // recupère les nouvelles données de la base ...
}

void UserModel::update(User user)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("UPDATE t_users SET "
                  "nom=:nom, "
                  "login=:login, "
                  "password=:password, "
                  "country=:country, "
                  "birthdate=:birthdate, "
                  "email=:email, "
                  "role=:role, "
                  "statut=:statut WHERE id=:id");

    query.bindValue(":nom", user.getNom());
    query.bindValue(":login", user.getLogin());
    query.bindValue(":password", user.getPassword());
    query.bindValue(":country", user.getCountry());
    query.bindValue(":birthdate", user.getBirthdate());
    query.bindValue(":email", user.getEmail());
    query.bindValue(":role", user.getRole());
    query.bindValue(":statut", user.getStatut().compare("") == 0 ? "NULL" : user.getStatut());
    query.bindValue(":id", user.getId());

    query.exec();

    dbManager->close();

    readAll(); // recupère les nouvelles données de la base ...
}

User UserModel::read(int id)
{
    User user; // constructeur par défaut !!!

    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT * FROM t_users WHERE id=:id");
    query.bindValue(":id", id);

    query.exec();

    if (query.next())
    {
        qDebug () << "user found!";

        user.setId(query.record().field("id").value().toInt());
        user.setNom(query.record().field("nom").value().toString());
        user.setLogin(query.record().field("login").value().toString());
        user.setPassword(query.record().field("password").value().toString());
        user.setCountry(query.record().field("country").value().toString());
        user.setBirthdate(query.record().field("birthdate").value().toString());
        user.setEmail(query.record().field("email").value().toString());
        user.setRole(query.record().field("role").value().toString());
        user.setStatut(query.record().field("statut").value().toString());
    }
    else
    {
        qDebug () << "user not found!";
    }

    dbManager->close();

    return user;
}

void UserModel::remove(int id)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("DELETE FROM t_users WHERE id=:id");
    query.bindValue(":id", id);

    query.exec();

    dbManager->close();

    readAll(); // recupère les nouvelles données de la base ...
}

QList<User> UserModel::list()
{
    User user;
    QList<User> users;

    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT * FROM t_users WHERE 1");
    query.exec();

    while (query.next())
    {
        user.setId(query.record().field("id").value().toInt());
        user.setNom(query.record().field("nom").value().toString());
        user.setLogin(query.record().field("login").value().toString());
        user.setLogin(query.record().field("password").value().toString());
        user.setCountry(query.record().field("country").value().toString());
        user.setBirthdate(query.record().field("birthdate").value().toString());
        user.setEmail(query.record().field("email").value().toString());
        user.setRole(query.record().field("role").value().toString());
        user.setStatut(query.record().field("statut").value().toString());

        users.push_back(user);
    }

    dbManager->close();

    return users;
}

void UserModel::remove()
{
    dbManager->open();

    QModelIndex currentIndex = selectionModel->currentIndex();
    if (currentIndex.isValid())
    {
        // valide si un élément est sélectionné ...
        int ligne = currentIndex.row();

        QSqlRecord record = this->record(ligne);
        int id = record.field(0).value().toInt();

        remove (id);
    }
    else
    {
        dbManager->close();
    }

    readAll();
}

void UserModel::readAll()
{
    dbManager->open();

    QSqlDatabase database = dbManager->database();

    this->setQuery("SELECT id, nom, country, birthdate, "
                   "login, password, email, role, statut FROM t_users", database);
    setHeaderTitle();

    dbManager->close();
}

void UserModel::readAllClients()
{
    dbManager->open();

    QSqlDatabase database = dbManager->database();

    this->setQuery("SELECT id, nom, country, birthdate, "
                   "email, statut FROM t_users where role=\"CLIENT\"", database);
    setClientHeaderTitle();

    dbManager->close();
}

void UserModel::readBy(int id)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT id, nom, country, "
                  "birthdate, login, password, email, role, statut FROM t_users WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();

    this->setQuery(query);
    setHeaderTitle();

    dbManager->close();
}

User UserModel::readBy(QString login, QString password)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT * FROM t_users WHERE login=:login AND password=:password");

    qDebug () << "UserModel::readBy::login : " << login;
    qDebug () << "UserModel::readBy::password : " << password;

    query.bindValue(":login", QVariant(login));
    query.bindValue(":password", QVariant(password));
    query.exec();

    qDebug () << "UserModel::readBy::lastQuery : " << query.lastQuery();

    User user;

    if (query.next())
    {
        qDebug () << "UserModel::readBy:: user found!";

        user.setId(query.record().field("id").value().toInt());
        user.setNom(query.record().field("nom").value().toString());
        user.setLogin(query.record().field("login").value().toString());
        user.setPassword(query.record().field("password").value().toString());
        user.setCountry(query.record().field("country").value().toString());
        user.setBirthdate(query.record().field("birthdate").value().toString());
        user.setEmail(query.record().field("email").value().toString());
        user.setRole(query.record().field("role").value().toString());
        user.setStatut(query.record().field("statut").value().toString());
    }
    else
    {
        qDebug () << "UserModel::readBy:: user not found!";
    }

    dbManager->close();

    return user;
}

void UserModel::readBy(QString str)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("SELECT id, nom, login, password, country, "
                  "birthdate, email, role, statut FROM t_users WHERE nom LIKE %:str% OR email LIKE %:str% OR role LIKE %:str%");
    query.bindValue(":str", str);
    query.exec();

    this->setQuery(query);
    setHeaderTitle();

    dbManager->close();
}

void UserModel::setHeaderTitle()
{
    this->setHeaderData(0, Qt::Horizontal, tr("Identifiant"));
    this->setHeaderData(1, Qt::Horizontal, tr("Nom"));    
    this->setHeaderData(2, Qt::Horizontal, tr("Country"));
    this->setHeaderData(3, Qt::Horizontal, tr("Birthdate"));
    this->setHeaderData(4, Qt::Horizontal, tr("Login"));
    this->setHeaderData(5, Qt::Horizontal, tr("password"));
    this->setHeaderData(6, Qt::Horizontal, tr("Email"));
    this->setHeaderData(7, Qt::Horizontal, tr("Role"));
    this->setHeaderData(8, Qt::Horizontal, tr("Statut"));
}

void UserModel::setClientHeaderTitle()
{
    this->setHeaderData(0, Qt::Horizontal, tr("Identifiant"));
    this->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    this->setHeaderData(2, Qt::Horizontal, tr("Country"));
    this->setHeaderData(3, Qt::Horizontal, tr("Birthdate"));
    this->setHeaderData(6, Qt::Horizontal, tr("Email"));
    this->setHeaderData(8, Qt::Horizontal, tr("Statut"));
}
