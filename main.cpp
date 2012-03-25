#include <QtGui/QApplication>
#include "mainwindow.h"
#include <bigprime.h>

int main()
{
    //QApplication a();
    //MainWindow w;
    //w.show();
    BigPrime *a = new BigPrime(1024);
//    a->startTimer();
    a->show();
    //return a.exec();
    return 0;
}
