#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("French guy CH");
    QCoreApplication::setOrganizationDomain("frenchguy.ch");
    QCoreApplication::setApplicationName("PiCasterQt");

    QApplication a(argc, argv);
    MainWindow w;
    w.init();
    w.show();

    return a.exec();
}
