#include "uilistuser.h"
#include "ui_uilistuser.h"

UIListUser::UIListUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIListUser)
{
    ui->setupUi(this);
}

UIListUser::UIListUser(QObject* controller) :
    ui(new Ui::UIListUser)
{
    qDebug("Contruction/Initialisation de la fenêtre UIListUser ...");
    ui->setupUi(this);

    connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIListUser()));
    connect(ui->pushButtonDelete, SIGNAL(clicked()), controller, SLOT(onDelete_UIListUser()));
    connect(ui->pushButtonUpdate, SIGNAL(clicked()), controller, SLOT(onUpdate_UIListUser()));
    connect(ui->pushButtonNotification, SIGNAL(clicked()), controller, SLOT(onNotif_UIClient()));
}

void UIListUser::setTableViewModel(UserModel* userModel)
{
    ui->tableViewUsers->setModel(dynamic_cast<QAbstractItemModel*> (userModel));
    ui->tableViewUsers->setSelectionModel(userModel->getSelectionModel());

    ui->tableViewUsers->show();
}

void UIListUser::top()
{
    ui->tableViewUsers->setCurrentIndex(ui->tableViewUsers->model()->index(0, 0));
}

void UIListUser::updateTitle(QString libelle)
{
    this->setWindowTitle(windowTitle() + libelle);
}

UIListUser::~UIListUser()
{
    delete ui;
}
