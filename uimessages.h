#ifndef UIMESSAGES_H
#define UIMESSAGES_H

#include <QMainWindow>
#include <QSqlQueryModel>

namespace Ui {
class Uimessages;
}

class Uimessages : public QMainWindow
{
    Q_OBJECT

public:
    explicit Uimessages(int idUtilisateur, QWidget *parent = nullptr);
    ~Uimessages();

private slots:
    void envoyerMessage();
    void chargerDestinataires();
    void chargerMessagesRecus();
    void chargerMessagesEnvoyes();

private:
    Ui::Uimessages *ui;
    int idUtilisateur;
    QSqlQueryModel *modeleRecus;
    QSqlQueryModel *modeleEnvoyes;
};

#endif // UIMESSAGES_H
