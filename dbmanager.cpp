#include "dbmanager.h"

// initialisation de l'instance a null
DBManager* DBManager::instance = nullptr;

DBManager::DBManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbFullPath = "C:\\Bank\\banking_system.db";
    db.setDatabaseName(dbFullPath);

    qDebug("Connection opened successfully with database.");
    qDebug("Singleton DBManager initialized.");

    qDebug() << db.databaseName();
}

bool DBManager::open()
{
    if (db.isOpen()) return true;

    if (db.open()) return true;
    else
    {
        qDebug("Failed to open connection.");
        return false;
    }
}

void DBManager::close()
{
    if (db.isOpen()) db.close();
}

DBManager* DBManager::getInstance()
{
    if (instance == nullptr) instance = new DBManager();

    return instance;
}

void DBManager::release()
{    
    if (instance != nullptr)
    {
        delete instance;
        qDebug("Suppression du Singleton DBManager effectuéé.");
    }
}

DBManager::~DBManager() {}
