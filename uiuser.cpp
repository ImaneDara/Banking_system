#include "uiuser.h"
#include "ui_uiuser.h"

UIUser::UIUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIUser)
{
    ui->setupUi(this);
}

UIUser::UIUser(QObject* controller) :
    ui(new Ui::UIUser)
{
    qDebug("Contruction/Initialisation de la fenêtre UIUser ...");
    ui->setupUi(this);

    connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIUser()));
    connect(ui->pushButtonCreate, SIGNAL(clicked()), controller, SLOT(onCreate_UIUser()));
    connect(ui->pushButtonUpdate, SIGNAL(clicked()), controller, SLOT(onUpdate_UIUser()));
    connect(ui->pushButtonFind, SIGNAL(clicked()), controller, SLOT(onFind_UIUser()));
    connect(ui->pushButtonList, SIGNAL(clicked()), controller, SLOT(onList_UIUser()));
    connect(ui->comboBoxRole, SIGNAL(currentTextChanged(QString)), controller, SLOT(onComboBoxRoleChanged_UIUser()));
}

QString UIUser::getNom()
{
    return ui->lineEditNom->text();
}

QString UIUser::getPassword()
{
    return ui->lineEditPassword->text();
}

QString UIUser::getLogin()
{
    return ui->lineEditLogin->text();
}

QString UIUser::getStatut()
{
    return ui->comboBoxStatut->currentText();
}

QString UIUser::getRole()
{
    return ui->comboBoxRole->currentText();
}

QString UIUser::getCountry()
{
    return ui->comboBoxCountry->currentText();
}

QString UIUser::getBirthdate()
{
    return ui->dateEditBirthdate->text();
}

QString UIUser::getEmail()
{
    return ui->lineEditEmail->text();
}

void UIUser::reinit()
{
    populate("-1", "", "Senegal", "01/01/2000", "", "", "", "ADMINISTRATEUR", "");
}

void UIUser::deactivateUpdate()
{
    ui->pushButtonUpdate->setEnabled(false);
    ui->pushButtonCreate->setEnabled(true);
    ui->lineEditIdentifiant->setEnabled(false);
}

void UIUser::activateUpdate()
{
    ui->pushButtonUpdate->setEnabled(true);
    ui->pushButtonCreate->setEnabled(false);
}

void UIUser::populate (QString identifiant, QString nom, QString country, QString birthdate, QString login, QString password
                         , QString email, QString role, QString statut)
{
    ui->lineEditIdentifiant->setText(identifiant);
    ui->lineEditNom->setText(nom);    
    ui->comboBoxCountry->setCurrentText(country);
    ui->dateEditBirthdate->setDate(QDate::fromString(birthdate, "dd/MM/yyyy"));
    ui->lineEditLogin->setText(login);
    ui->lineEditPassword->setText(password);
    ui->lineEditEmail->setText(email);
    ui->comboBoxRole->setCurrentText(role);
    ui->comboBoxStatut->setCurrentText(statut);
}

void UIUser::hideStatut ()
{
    ui->comboBoxStatut->hide();
    ui->labelSatut->hide();
}

void UIUser::showStatut ()
{
    ui->comboBoxStatut->show();
    ui->labelSatut->show();
}

void UIUser::show()
{
    QMainWindow::show();

    if (getRole().compare("ADMINISTRATEUR") == 0)
    {
        hideStatut();
    }
    else
    {
        showStatut();
    }
}

void UIUser::setRoleEditable(bool editable)
{
    ui->comboBoxRole->setEnabled(editable);
}

void UIUser::updateTitle(QString libelle)
{
    this->setWindowTitle(windowTitle() + libelle);
}

UIUser::~UIUser()
{
    delete ui;
}
