#include <QtGui/QApplication>
#include "mainwindow.h"
#include <bigrandomprime.h>

using namespace DecToBin;

int main()
{
    //QApplication a();
    //MainWindow w;
    //w.show();
    BigRandomPrime number1(8);
    number1.show(0);
    sleep(1);
    BigRandomPrime number2(8);
    number2.show(0);
    (number1 * number2).show();
    qDebug() << number1.ToDec() << " * " << number2.ToDec() << " = " << number1.ToDec() * number2.ToDec() << "(" << (number1 * number2).ToDec() << ")";
    return a.exec();
    //return a.exec();
    return 0;
}
