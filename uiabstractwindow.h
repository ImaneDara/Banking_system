#ifndef UIABSTRACTWINDOW_H
#define UIABSTRACTWINDOW_H

#include <QString>

class UIAbstractWindow
{
public:
    UIAbstractWindow();

    void critical(QString title, QString content);
    void information(QString title, QString content);
    void warning(QString title, QString content);
    bool question(QString title, QString content);
    virtual void updateTitle (QString libelle) = 0;
};

#endif // UIABSTRACTWINDOW_H
