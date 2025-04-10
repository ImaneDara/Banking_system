#include "controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller ctrl;
    ctrl.execute();

    return a.exec();
}
