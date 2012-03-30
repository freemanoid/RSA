#include <QtGui/QApplication>
#include "mainwindow.h"
#include <bigprime.h>
#include <key.h>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    a.exec();
    using namespace std;
    Key a(64);
    mpz_class rsa = a.crypt(314213432);
    cout << "rsa(" << rsa << ") = " << a.decrypt(rsa) << endl << flush;
//    return a.exec();
    return 0;
}
