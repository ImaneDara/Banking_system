#include "uiaccount.h"
#include "ui_uiaccount.h"

UIAccount::UIAccount(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIAccount)
{
    ui->setupUi(this);
}

UIAccount::UIAccount(QObject* controller) :
    ui(new Ui::UIAccount)
{
    qDebug("Contruction/Initialisation de la fenêtre UIAccount ...");
    ui->setupUi(this);

    connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIAccount()));
    connect(ui->pushButtonCreate, SIGNAL(clicked()), controller, SLOT(onCreate_UIAccount()));
    connect(ui->pushButtonUpdate, SIGNAL(clicked()), controller, SLOT(onUpdate_UIAccount()));
}

QString UIAccount::getAccountNumber()
{
    return ui->lineEditAccountNumber->text();
}

QString UIAccount::getType()
{
    return ui->comboBoxType->currentText();
}

QString UIAccount::getBalance()
{
    return ui->lineEditBalance->text();
}

QString UIAccount::getIdClient()
{
    return ui->lineEditIdClient->text();
}

QString UIAccount::getAccountId()
{
    return ui->lineEditAccountId->text();
}

void UIAccount::reinit()
{
    populate("-1", "-1", "", "COURANT", "0");
}

void UIAccount::populate (QString accountId, QString idClient, QString accountNumber, QString type, QString balance)
{
    ui->lineEditAccountId->setText(accountId);
	ui->lineEditIdClient->setText(idClient);
    ui->lineEditAccountNumber->setText(accountNumber);    
    ui->comboBoxType->setCurrentText(type);    
    ui->lineEditBalance->setText(balance);
}

void UIAccount::updateTitle(QString libelle)
{
    this->setWindowTitle("Client : " + libelle);
}


void UIAccount::deactivateCreate()
{
    ui->pushButtonCreate->setEnabled(false);
    ui->pushButtonUpdate->setEnabled(true);

    ui->lineEditBalance->setEnabled(false);
}

void UIAccount::deactivateUpdate()
{
    ui->pushButtonCreate->setEnabled(true);
    ui->pushButtonUpdate->setEnabled(false);

    ui->lineEditBalance->setEnabled(true);
}

UIAccount::~UIAccount()
{
    delete ui;
}
