#include "uilistnotif.h"
#include "ui_uilistnotif.h"

UIListNotif::UIListNotif(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIListNotif),
    notificationModel(nullptr),
    listViewModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    //ui->listViewNotifs->setModel(listViewModel);

    setupListView();
    connect(ui->listViewNotifs, &QListView::clicked,
            this, &UIListNotif::onNotificationClicked);
}

UIListNotif::UIListNotif(QObject* controller) :
    ui(new Ui::UIListNotif),
    notificationModel(nullptr),
    listViewModel(new QStandardItemModel(this))
{
    qDebug("Contruction/Initialisation de la fenêtre UIListClient ...");
    ui->setupUi(this);
    // ui->listViewNotifs->setModel(listViewModel);

    setupListView();

    connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIListNotif()));
    connect(ui->pushButtonOpen, SIGNAL(clicked()), controller, SLOT(onOuvrir_UIListNotif()));
}

UIListNotif::~UIListNotif()
{
    delete ui;
}

void UIListNotif::refreshNotifications(int userId)
{
    if (!notificationModel) return;

    listViewModel->clear();
    QList<Notification> notifications = notificationModel->listByUser(userId);

    for (const Notification& notif : notifications) {
        addNotificationToView(notif);
    }

    // Mettre à jour le compteur de non-lues
    updateWindowTitle(QString("Notifications (%1 non lues)")
                          .arg(notificationModel->getUnreadCount(userId)));
}

void UIListNotif::updateWindowTitle(const QString& info)
{
    ui->labelTitle->setText(info);
}

void UIListNotif::addNotificationToView(Notification notif)
{
    QString notificationText = QString("%1 - %2\n%3")
    .arg(notif.getDate().left(10))  // Date formatée
        .arg(notif.getTitle())          // Titre
        .arg(notif.getMessage());       // Message

    QStandardItem* item = new QStandardItem(notificationText);
    item->setData(notif.getId(), Qt::UserRole);

    if (!notif.getIsRead()) {
        QFont boldFont;
        boldFont.setBold(true);
        item->setFont(boldFont);
    }

    listViewModel->appendRow(item);
}

void UIListNotif::setNotificationModel(NotificationModel* model)
{
    notificationModel = model;
}

void UIListNotif::setupListView()
{
    listViewModel->clear();
    listViewModel->setColumnCount(1); // Pour affichage simple
    ui->listViewNotifs->setModel(listViewModel);
    ui->listViewNotifs->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void UIListNotif::updateNotifications(int userId)
{
    if (!notificationModel) return;

    listViewModel->clear();
    QList<Notification> notifications = notificationModel->listByUser(userId);

    for (Notification notif : notifications) {
        QStandardItem* item = new QStandardItem();
        item->setText(QString("%1 - %2").arg(notif.getDate(), notif.getTitle()));
        item->setData(notif.getId(), Qt::UserRole);
        item->setData(!notif.getIsRead(), Qt::FontRole);

        if (!notif.getIsRead()) {
            QFont font;
            font.setBold(true);
            item->setData(font, Qt::FontRole);
        }

        listViewModel->appendRow(item);
    }
}

void UIListNotif::showUnreadCount(int count)
{
    QString title = "Notifications";
    if (count > 0) {
        title += QString(" (%1 non lues)").arg(count);
    }
    ui->labelTitle->setText(title);
}

void UIListNotif::onNotificationClicked(const QModelIndex &index)
{
    if (!notificationModel || !index.isValid()) return;

    int notifId = index.data(Qt::UserRole).toInt();
    notificationModel->markAsRead(notifId);

    // Mettre à jour l'affichage
    QStandardItem* item = listViewModel->itemFromIndex(index);
    QFont font;
    font.setBold(false);
    item->setData(font, Qt::FontRole);
}
