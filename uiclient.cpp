#include "uiclient.h"
#include "ui_uiclient.h"

UIClient::UIClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIClient)
{
    ui->setupUi(this);
}

UIClient::UIClient(QObject* controller) :
    ui(new Ui::UIClient)
{
    qDebug("Contruction/Initialisation de la fenêtre UIListUser ...");
    ui->setupUi(this);

    connect(ui->pushButtonOuvrir, SIGNAL(clicked()), controller, SLOT(onOuvrir_UIClient()));
    connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIClient()));
    // -
    connect(ui->pushButtonVirement, SIGNAL(clicked()), controller, SLOT(onVirement_UIClient()));
    connect(ui->pushButtonRetrait, SIGNAL(clicked()), controller, SLOT(onRetrait_UIClient()));
    connect(ui->pushButtonVersement, SIGNAL(clicked()), controller, SLOT(onVersement_UIClient()));
    connect(ui->pushButtonHistorique, SIGNAL(clicked()), controller, SLOT(onHistorique_UIClient()));
    //-
    connect(ui->pushButtonOK, SIGNAL(clicked()), controller, SLOT(onOK_UIClient()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), controller, SLOT(onCancel_UIClient()));

}

void UIClient::setTableViewModel(AccountModel* accountModel)
{
    ui->tableViewAccounts->setModel(dynamic_cast<QAbstractItemModel*> (accountModel));
    ui->tableViewAccounts->setSelectionModel(accountModel->getSelectionModel());

    ui->tableViewAccounts->show();
}

void UIClient::updateTitle(QString libelle)
{
    this->setWindowTitle("Gestion des comptes bancaires - Client : " + libelle);
}


void UIClient::showTransactionForm(QString labelCurrentTransaction, QString typeTransaction)
{
    ui->lineEditMontant->show();
    ui->lineEditMontant->setText("");
    // -
    ui->labelCurrentTransaction->show();
    ui->labelCurrentTransaction->setText(labelCurrentTransaction);
    ui->labelMontant->show();

    if (typeTransaction.compare("VIREMENT") == 0)
    {
        ui->labelNumeroBeneficiaire->show();
        // -
        ui->lineEditNumeroBeneficiaire->setText("");
        ui->lineEditNumeroBeneficiaire->show();
    }

    ui->pushButtonOK->show();
    ui->pushButtonCancel->show();

    this->typeTransaction = typeTransaction;
}

void UIClient::hideTransactionForm()
{
    ui->lineEditMontant->hide();
    ui->labelCurrentTransaction->hide();
    ui->labelMontant->hide();
    ui->labelNumeroBeneficiaire->hide();
    ui->lineEditNumeroBeneficiaire->hide();

    ui->pushButtonOK->hide();
    ui->pushButtonCancel->hide();
}

QString UIClient::getNumeroBeneficiaire()
{
    return ui->lineEditNumeroBeneficiaire->text();
}

QString UIClient::getMontant ()
{
    return ui->lineEditMontant->text();
}

UIClient::~UIClient()
{
    delete ui;
}
