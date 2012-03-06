#include <QtGui/QApplication>
#include "mainwindow.h"
#include <bigrandomsimple.h>

using namespace DecToBin;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    BigRandomSimple number1(8);
    number1.show(0);
    sleep(1);
    BigRandomSimple number2(8);
    number2.show(0);
    (number1 - number2).show(0);
    return a.exec();
}
