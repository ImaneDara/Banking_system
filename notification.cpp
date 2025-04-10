#include "notification.h"

Notification::Notification()
{

}

Notification::Notification(int _id, int _idUser, QString _title, QString _message, QString _date, bool _isRead, NotificationType _type) :
    id(_id), idUser(_idUser), title(_title), message(_message), date(_date),isRead(_isRead), type(_type)
{}

Notification::Notification(int _idUser, QString _title, QString _message, QString _date, NotificationType _type) :
    idUser(_idUser), title(_title), message(_message), date(_date), type(_type)
{}

Notification::Notification(int _idUser, QString _title, QString _message, QString _date, bool _isRead, NotificationType _type) :
    idUser(_idUser), title(_title), message(_message), date(_date), isRead(_isRead), type(_type)
{}

//Notification::Notification() : id(0), idUser(0), isRead(false), type(NotificationType::TRANSACTION_SUCCESS) {}

Notification::Notification(int idUser, NotificationType type, QString date, const QString& additionalInfo) :
    id(0), idUser(idUser), date(date), isRead(false), type(type)
{
    title = NotificationHelper::getTitle(type);
    message = NotificationHelper::getMessage(type, additionalInfo);
}
//Notification::Notification(int id, int idUser, QString title, QString message, QString date, bool isRead, QString type) :
//   Notification (idUser, title, message, date, isRead, type)
//{
//    this->id = id;
//}
