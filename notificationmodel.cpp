#include "notificationmodel.h"

NotificationModel::NotificationModel()
{
    selectionModel = new QItemSelectionModel(this);
    dbManager = DBManager::getInstance();
}


void NotificationModel::create(Notification notification)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("INSERT INTO t_notifications "
                  "(idUser, title, message, date, isRead, type) VALUES "
                  "(:idUser, :title, :message, :date, :isRead, :type)");

    query.bindValue(":idUser", notification.getIdUser());
    query.bindValue(":title", notification.getTitle());
    query.bindValue(":message", notification.getMessage());
    query.bindValue(":date", notification.getDate());
    query.bindValue(":isRead", notification.getIsRead());
    query.bindValue(":type", NotificationHelper::typeToString(notification.getType()));
    query.bindValue(":title", notification.getTitle());  // Maintenant utilisé
    query.bindValue(":message", notification.getMessage());
    query.exec();
    dbManager->close();

    qDebug("Notification sent successfully !");
    readAll(); // recupère les nouvelles données de la base ...

}

void NotificationModel::readAll()
{
    dbManager->open();

    QSqlDatabase database = dbManager->database();

    this->setQuery("SELECT id, title, message,"
                   " date, isRead, type"
                   " FROM t_notifications", database);
    setHeaders();

    dbManager->close();
}

void NotificationModel::readByUser(int userId)
{
    dbManager->open();

    QSqlQuery query(dbManager->database());
    query.prepare("SELECT id, title, message, date, isRead FROM t_notifications "
                  "WHERE idUser = :userId ORDER BY date DESC");
    query.bindValue(":userId", userId);

    if (query.exec()) {
        this->setQuery(query);
        setHeaders();
    }

    dbManager->close();
}

void NotificationModel::readBy(QString notificationType)
{
    dbManager->open();

    QSqlDatabase database = dbManager->database();

    QSqlQuery query(dbManager->database());

    this->setQuery("SELECT id, title, message, date, isRead, type "
                   "FROM t_notifications "
                   "WHERE type = :type "
                   "ORDER BY date DESC");
    query.bindValue(":type", notificationType);
    query.exec();

    this->setQuery(query);
    setHeaders();
    dbManager->close();
}

void NotificationModel::markAsRead(int notificationId)
{
    dbManager->open();
    QSqlQuery query(dbManager->database());

    query.prepare("UPDATE t_notifications SET isRead = 1 WHERE id = :id");
    query.bindValue(":id", notificationId);

    query.exec();
    dbManager->close();
    qDebug("Notification lue !");

}

QList<Notification> NotificationModel::listByUser(int userId)
{
    QList<Notification> notifications;
    dbManager->open();

    QSqlQuery query(dbManager->database());
    QString sql = "SELECT * FROM t_notifications WHERE idUser = :userId";

    //if (unreadOnly) {
    //    sql += " AND isRead = 0";
    //}

    sql += " ORDER BY date DESC";

    query.prepare(sql);
    query.bindValue(":userId", userId);

    if (query.exec()) {
        while (query.next()) {
            Notification notification(
                query.value("id").toInt(),
                query.value("idUser").toInt(),
                query.value("title").toString(),
                query.value("message").toString(),
                query.value("date").toString(),
                query.value("isRead").toBool(),
                static_cast<NotificationType>(query.value("type").toInt())
                );
            notifications.append(notification);
        }
    }

    dbManager->close();
    return notifications;
}

int NotificationModel::getUnreadCount(int userId)
{
    int count = 0;
    dbManager->open();

    QSqlQuery query(dbManager->database());
    query.prepare("SELECT COUNT(*) FROM t_notifications WHERE idUser = :userId AND isRead = 0");
    query.bindValue(":userId", userId);

    if (query.exec() && query.next()) {
        count = query.value(0).toInt();
    }

    dbManager->close();
    return count;
}


void NotificationModel::setHeaders()
{
    this->setHeaderData(0, Qt::Horizontal, tr("ID"));
    this->setHeaderData(1, Qt::Horizontal, tr("Titre"));
    this->setHeaderData(2, Qt::Horizontal, tr("Message"));
    this->setHeaderData(3, Qt::Horizontal, tr("Date"));
    this->setHeaderData(4, Qt::Horizontal, tr("Lu"));
}
