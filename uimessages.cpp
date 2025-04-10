#include "uimessages.h"
#include "ui_uimessages.h"
#include <QSqlQuery>
#include <QMessageBox>

Uimessages::Uimessages(int idUtilisateur, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Uimessages),
    idUtilisateur(idUtilisateur),
    modeleRecus(new QSqlQueryModel(this)),
    modeleEnvoyes(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    chargerDestinataires();
    chargerMessagesRecus();
    chargerMessagesEnvoyes();

    connect(ui->pushButtonEnvoyer, &QPushButton::clicked, this, &Uimessages::envoyerMessage);
}

Uimessages::~Uimessages()
{
    delete ui;
}

void Uimessages::chargerDestinataires()
{
    QSqlQuery query("SELECT id, nom FROM t_users WHERE id != " + QString::number(idUtilisateur));
    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();
        ui->comboBoxDestinataire->addItem(nom, id);
    }
}

void Uimessages::chargerMessagesRecus()
{
    QSqlQuery query;
    query.prepare("SELECT u.nom AS Expediteur, m.sujet, m.contenu, m.date_envoi "
                  "FROM messages m JOIN t_users u ON m.expediteur_id = u.id "
                  "WHERE m.destinataire_id = :id ORDER BY m.date_envoi DESC");
    query.bindValue(":id", idUtilisateur);
    query.exec();

    modeleRecus->setQuery(query);
    ui->tableViewRecus->setModel(modeleRecus);
}

void Uimessages::chargerMessagesEnvoyes()
{
    QSqlQuery query;
    query.prepare("SELECT u.nom AS Destinataire, m.sujet, m.contenu, m.date_envoi "
                  "FROM messages m JOIN t_users u ON m.destinataire_id = u.id "
                  "WHERE m.expediteur_id = :id ORDER BY m.date_envoi DESC");
    query.bindValue(":id", idUtilisateur);
    query.exec();

    modeleEnvoyes->setQuery(query);
    ui->tableViewEnvoyes->setModel(modeleEnvoyes);
}

void Uimessages::envoyerMessage()
{
    int destinataireId = ui->comboBoxDestinataire->currentData().toInt();
    QString sujet = ui->lineEditSujet->text();
    QString contenu = ui->textEditContenu->toPlainText();

    if (sujet.isEmpty() || contenu.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Sujet et contenu requis.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO messages (expediteur_id, destinataire_id, sujet, contenu, date_envoi) "
                  "VALUES (:exp, :dest, :sujet, :contenu, datetime('now'))");
    query.bindValue(":exp", idUtilisateur);
    query.bindValue(":dest", destinataireId);
    query.bindValue(":sujet", sujet);
    query.bindValue(":contenu", contenu);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Message envoyé.");
        ui->lineEditSujet->clear();
        ui->textEditContenu->clear();
        chargerMessagesEnvoyes();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'envoi.");
    }
}
