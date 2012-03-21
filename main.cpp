#include <QtGui/QApplication>
#include "mainwindow.h"
#include <bigrandomprime.h>

using namespace DecToBin;

int main()
{
    //QApplication a();
    //MainWindow w;
    //w.show();
//    qsrand(time(NULL));
    BigRandomPrime *number1 = new BigRandomPrime(7);
    while(!number1->test())
    {
        number1->~BigRandomPrime();
        number1 = new BigRandomPrime(7);
    }
    qDebug() << number1->ToDec();
    number1->show();
//    (number1 % number2).show();
//    qDebug() << number1.ToDec() << " % " << number2.ToDec() << " = " << number1.ToDec() % number2.ToDec() << "(" << (number1 % number2).ToDec() << ")";
    //return a.exec();
    return 0;
}
