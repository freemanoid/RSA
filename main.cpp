#include <QtGui/QApplication>
#include "mainwindow.h"
#include <bigrandomprime.h>

using namespace DecToBin;

int main()
{
    //QApplication a();
    //MainWindow w;
    //w.show();
    BigRandomPrime *number1 = new BigRandomPrime(15);
    while(!number1->test())
    {
        number1->~BigRandomPrime();
        //sleep(1);
        number1 = new BigRandomPrime(15);
    }
    qDebug() << number1->ToDec();
//    (number1 % number2).show();
//    qDebug() << number1.ToDec() << " % " << number2.ToDec() << " = " << number1.ToDec() % number2.ToDec() << "(" << (number1 % number2).ToDec() << ")";
    //return a.exec();
    return 0;
}
