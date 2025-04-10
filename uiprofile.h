#ifndef UIPROFILE_H
#define UIPROFILE_H

#include <QMainWindow>

namespace Ui {
class uiprofile;
}

class uiprofile : public QMainWindow
{
    Q_OBJECT

public:
    explicit uiprofile(QWidget *parent = nullptr);
    ~uiprofile();

    void setClientData(const QMap<QString, QString>& clientData);

private slots:
    void on_pushButtonFermer_clicked();

private:
    Ui::uiprofile *ui;
};

#endif // UIPROFILE_H
