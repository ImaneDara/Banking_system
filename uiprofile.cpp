#include "uiprofile.h"
#include "ui_uiprofile.h"
#include <QDebug>

uiprofile::uiprofile(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::uiprofile)
{
    ui->setupUi(this);

    connect(ui->pushButtonFermer, &QPushButton::clicked, this, &uiprofile::on_pushButtonFermer_clicked);

    this->setWindowTitle("Profile client");
}

void uiprofile::setClientData(const QMap<QString, QString>& clientData)
{
    ui->lineEditName->setText(clientData.value("nom", "N/A"));
    ui->lineEditName->setEnabled(false);
    ui->lineEditLogin->setText(clientData.value("login", "N/A"));
    ui->lineEditLogin->setEnabled(false);
    ui->lineEditPassword->setText(clientData.value("password", "N/A"));
    ui->lineEditPassword->setEnabled(false);
    ui->lineEditCountry->setText(clientData.value("country", "N/A"));
    ui->lineEditCountry->setEnabled(false);
    ui->lineEditBirthdate->setText(clientData.value("birthdate", "N/A"));
    ui->lineEditBirthdate->setEnabled(false);
    ui->lineEditEmail->setText(clientData.value("email", "N/A"));
    ui->lineEditEmail->setEnabled(false);
    ui->lineEditRole->setText(clientData.value("role", "N/A"));
    ui->lineEditRole->setEnabled(false);
    ui->lineEditStatut->setText(clientData.value("statut", "N/A"));
    ui->lineEditStatut->setEnabled(false);
}

void uiprofile::on_pushButtonFermer_clicked()
{
    qDebug() << "Fermer button clicked";
    this->hide();
    // this->close();
}

uiprofile::~uiprofile()
{
    delete ui;
}
