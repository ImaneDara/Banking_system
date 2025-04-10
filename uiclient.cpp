#include "uiclient.h"
#include "ui_uiclient.h"
#include <QLabel>
#include <QFormLayout>

UIClient::UIClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIClient)
{
    ui->setupUi(this);

    // Initialize profile widget
    profileWidget = new QWidget(this);
    profileLayout = new QFormLayout(profileWidget);

    // Create labels and value fields for profile
    QStringList fieldLabels = {"Nom", "Login", "Password", "Country", "Birthdate", "Email", "Role", "Statut"};
    for (int i = 0; i < fieldLabels.size(); ++i) {
        profileLabels[i] = new QLabel(fieldLabels[i] + " :");
        profileValues[i] = new QLabel();
        profileValues[i]->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        profileLayout->addRow(profileLabels[i], profileValues[i]);
    }

    profileWidget->setLayout(profileLayout);
    profileWidget->hide();

    // Add profile widget to your main layout
    ui->verticalLayout->insertWidget(0, profileWidget); // Adjust position as needed
}

UIClient::UIClient(QObject* controller) :
    ui(new Ui::UIClient), profileWindow(nullptr)
{
    qDebug("Contruction/Initialisation de la fenêtre UIListUser ...");
    ui->setupUi(this);

    // Initialize profile widget (same as above)
    profileWidget = new QWidget(this);
    profileLayout = new QFormLayout(profileWidget);

    QStringList fieldLabels = {"Nom", "Login", "Password", "Country", "Birthdate", "Email", "Role", "Statut"};
    for (int i = 0; i < fieldLabels.size(); ++i) {
        profileLabels[i] = new QLabel(fieldLabels[i] + " :");
        profileValues[i] = new QLabel();
        profileValues[i]->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        profileLayout->addRow(profileLabels[i], profileValues[i]);
    }

    profileWidget->setLayout(profileLayout);
    profileWidget->hide();
    ui->verticalLayout->insertWidget(0, profileWidget);

    // Existing connections
    connect(ui->pushButtonOuvrir, SIGNAL(clicked()), controller, SLOT(onOuvrir_UIClient()));
    connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIClient()));
    connect(ui->pushButtonVirement, SIGNAL(clicked()), controller, SLOT(onVirement_UIClient()));
    connect(ui->pushButtonRetrait, SIGNAL(clicked()), controller, SLOT(onRetrait_UIClient()));
    connect(ui->pushButtonVersement, SIGNAL(clicked()), controller, SLOT(onVersement_UIClient()));
    connect(ui->pushButtonHistorique, SIGNAL(clicked()), controller, SLOT(onHistorique_UIClient()));

    // Add the new profile button connection
    connect(ui->pushButtonProfile, SIGNAL(clicked()), controller, SLOT(onProfile_UIClient()));
    connect(ui->pushButtonNotification, SIGNAL(clicked()), controller, SLOT(onNotif_UIClient()));

    // Existing OK/Cancel buttons
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

void UIClient::showProfileForm(const QMap<QString, QString>& clientData)
{
    qDebug() << "Showing profile form";

    if (!profileWindow) {
        profileWindow = new uiprofile(this);
    }
    hideTransactionForm();
    profileWindow->setClientData(clientData);
    profileWindow->show();
    profileWindow->raise();

    this->update();
}

void UIClient::hideProfileForm()
{
    profileWidget->hide();
}

void UIClient::setClientInfo(const QMap<QString, QString>& clientData)
{
    qDebug() << "Setting client info in UI";

    profileValues[0]->setText(clientData.value("nom", "N/A"));
    profileValues[1]->setText(clientData.value("login", "N/A"));
    profileValues[2]->setText(clientData.value("password", "N/A"));
    profileValues[3]->setText(clientData.value("country", "N/A"));
    profileValues[4]->setText(clientData.value("birthdate", "N/A"));
    profileValues[5]->setText(clientData.value("email", "N/A"));
    profileValues[6]->setText(clientData.value("role", "N/A"));
    profileValues[7]->setText(clientData.value("statut", "N/A"));
}

UIClient::~UIClient()
{
    delete ui;
    // Clean up profile widgets
    for (int i = 0; i < 6; ++i) {
        delete profileLabels[i];
        delete profileValues[i];
    }
    delete profileLayout;
    delete profileWidget;
}

