#include <QApplication>

#include "ui/mainwindow.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavresample/avresample.h>
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("French guy CH");
    QCoreApplication::setOrganizationDomain("frenchguy.ch");
    QCoreApplication::setApplicationName("PiCasterQt");

    av_register_all();

    QApplication a(argc, argv);
    MainWindow w;
    w.init();
    w.show();

    return a.exec();
}
