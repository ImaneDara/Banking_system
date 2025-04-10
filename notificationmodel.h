#ifndef NOTIFICATIONMODEL_H
#define NOTIFICATIONMODEL_H

#include "dbmanager.h"
#include <QtDebug>
#include <QMetaType>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include "notification.h"
#include "transaction.h"

class NotificationModel : public QSqlQueryModel
{
private:
    DBManager* dbManager;
    // Permet d'avoir à tout moment les informations
    // sur l'élélement sélectionné sur la vue.
    QItemSelectionModel* selectionModel;
    QSqlQueryModel* sqlModel;

public:
    QItemSelectionModel* getSelectionModel() { return selectionModel; }
    QList<Notification> listByUser(int userId);

    void create(Notification notification);
    void markAsRead(int notificationId);
    int getUnreadCount(int userId);

    void readAll();
    void readByUser(int userId);
    void readBy(QString notificationType);

    void setHeaders();


    NotificationModel();
};

#endif // NOTIFICATIONMODEL_H
