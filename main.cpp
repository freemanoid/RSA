#include <QtGui/QApplication>
#include "mainwindow.h"
#include <key.h>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    a.exec();
    using namespace std;
    Key a(64);
//    QVector<mpz_class> *lol = a.crypt("12342134213561239874648721364870921342183974");
    qDebug() << a.decrypt(a.crypt("123214321421341234123412341234213"));
//    return a.exec();
    return 0;
}
