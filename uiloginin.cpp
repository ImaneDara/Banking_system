#include "uiloginin.h"
#include "ui_uiloginin.h"

UILoginIn::UILoginIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UILoginIn)
{
    ui->setupUi(this);
}


UILoginIn::UILoginIn(QObject *controller) :
    ui(new Ui::UILoginIn)
{
    qDebug("Contruction/Initialisation de la fenêtre UILoginIn ...");
    ui->setupUi(this);

    connect(ui->pushButtonSubmit, SIGNAL(clicked()), controller, SLOT(onSubmit_UILoginIn()));
    connect(ui->pushButtonExit, SIGNAL(clicked()), controller, SLOT(onExit_UILoginIn()));
}

QString UILoginIn::getLogin()
{
    return ui->lineEditLogin->text();
}

QString UILoginIn::getPassword()
{
    return ui->lineEditPassword->text();
}

void UILoginIn::reinit()
{
    ui->lineEditLogin->setText("");
    ui->lineEditPassword->setText("");
}

void UILoginIn::updateTitle(QString libelle)
{

}

UILoginIn::~UILoginIn()
{
    delete ui;
}
