#include "uilistclient.h"
#include "ui_uilistclient.h"

UIListClient::UIListClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIListClient)
{
    ui->setupUi(this);
}

UIListClient::UIListClient(QObject* controller) :
    ui(new Ui::UIListClient)
{
    qDebug("Contruction/Initialisation de la fenêtre UIListClient ...");
    ui->setupUi(this);

    connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIListClient()));
    connect(ui->pushButtonOuvrir, SIGNAL(clicked()), controller, SLOT(onOuvrir_UIListClient()));
    connect(ui->pushButton, SIGNAL(clicked()), controller, SLOT(onNotif_UIClient()));
    connect(ui->pushButtonMessage, SIGNAL(clicked()), controller, SLOT(onMessage_UIListClient()));
}

void UIListClient::setTableViewModel(UserModel* clientModel)
{
    ui->tableViewClients->setModel(dynamic_cast<QAbstractItemModel*> (clientModel));
    ui->tableViewClients->setSelectionModel(clientModel->getSelectionModel());

    ui->tableViewClients->show();
}

void UIListClient::top()
{
    ui->tableViewClients->setCurrentIndex(ui->tableViewClients->model()->index(0, 0));
}

void UIListClient::updateTitle(QString libelle)
{
    this->setWindowTitle("Gestion des comptes - Client : " + libelle);
}

UIListClient::~UIListClient()
{
    delete ui;
}
