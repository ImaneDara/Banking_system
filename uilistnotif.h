#ifndef UILISTNOTIF_H
#define UILISTNOTIF_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <notificationmodel.h>

namespace Ui {
class UIListNotif;
}

class UIListNotif : public QMainWindow
{
    Q_OBJECT

public:
    explicit UIListNotif(QWidget *parent = nullptr);
    UIListNotif(QObject* controller);
    ~UIListNotif();

    void setNotificationModel(NotificationModel* model);
    void refreshNotifications(int userId);

    void updateWindowTitle(const QString& userInfo);
    void updateNotifications(int userId);
    void showUnreadCount(int count);

private slots:
    void onNotificationClicked(const QModelIndex &index);

private:
    Ui::UIListNotif *ui;
    NotificationModel* notificationModel;
    QStandardItemModel* listViewModel;

    void setupListView();
    void addNotificationToView(Notification notif);
};

#endif // UILISTNOTIF_H

