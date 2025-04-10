#include "uiabstractwindow.h"
#include <QMessageBox>

UIAbstractWindow::UIAbstractWindow()
{}

void UIAbstractWindow::information(QString title, QString content)
{
    QMessageBox::information(nullptr, title, content);
}

void UIAbstractWindow::warning(QString title, QString content)
{
    QMessageBox::warning(nullptr, title, content);
}

bool UIAbstractWindow::question(QString title, QString content)
{
    return QMessageBox::Yes == QMessageBox::question(nullptr, title, content);
}

void UIAbstractWindow::critical(QString title, QString content)
{
    QMessageBox::critical(nullptr, title, content);
}

void updateTitle ()
{

}
