#include "uilistaccount.h"
#include "ui_uilistaccount.h"

UIListAccount::UIListAccount(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIListAccount)
{
    ui->setupUi(this);
}

UIListAccount::UIListAccount(QObject* controller) :
    ui(new Ui::UIListAccount)
{
    qDebug("Contruction/Initialisation de la fenêtre UIListAccount ...");
    ui->setupUi(this);

    connect(ui->pushButtonNouveau, SIGNAL(clicked()), controller, SLOT(onNouveau_UIListAccount()));
    connect(ui->pushButtonModifier, SIGNAL(clicked()), controller, SLOT(onModifier_UIListAccount()));	
    connect(ui->pushButtonOuvrir, SIGNAL(clicked()), controller, SLOT(onOuvrir_UIListAccount()));
	connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIListAccount()));
}

void UIListAccount::setTableViewModel(AccountModel* accountModel)
{
    ui->tableViewAccounts->setModel(dynamic_cast<QAbstractItemModel*> (accountModel));
    ui->tableViewAccounts->setSelectionModel(accountModel->getSelectionModel());

    ui->tableViewAccounts->show();
}

void UIListAccount::top()
{
    ui->tableViewAccounts->setCurrentIndex(ui->tableViewAccounts->model()->index(0, 0));
}

void UIListAccount::updateTitle(QString libelle)
{
    this->setWindowTitle("Gestion des comptes client - Gestionnaire : " + libelle);
}

void UIListAccount::updateAccountTitle(QString nomClient)
{
    ui->labelTitle->setText("Les comptes du client : " + nomClient);
}

UIListAccount::~UIListAccount()
{
    delete ui;
}
