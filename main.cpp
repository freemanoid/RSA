#include <QtGui/QApplication>
#include "mainwindow.h"
#include <bigprime.h>
#include <key.h>
#include <openssl/rsa.h>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    a.exec();
    using namespace std;
    Key a(64);
    mpz_class rsa = a.crypt(1488);
    cout << "rsa(" << rsa << ") = " << a.decrypt(rsa) << endl << flush;
//    cout << "mutuallyprime(425) = " << BigPrime(mpz_class(425)).mutuallyPrime() << endl << flush;
//    cout << "secret(319, 3) = " << Key::secret(280, 3) << endl << flush;
//    return a.exec();
    return 0;
}
