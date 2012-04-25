#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator myTranslator;
    myTranslator.load("rsa_" + QLocale::system().name());
    app.installTranslator(&myTranslator);
    MainWindow w;
    w.show();
    return app.exec();
}
