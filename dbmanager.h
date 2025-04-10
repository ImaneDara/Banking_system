#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QtDebug>

class DBManager
{
private:
    QSqlDatabase db;
    DBManager();

    static DBManager* instance;

public:

    bool open();
    void close();

    static DBManager* getInstance();
    QSqlDatabase database() { return db; }

    static void release();

    ~DBManager();
};


#endif // DBMANAGER_H
