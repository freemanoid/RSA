#ifndef KEY_H
#define KEY_H

#include <bigprime.h>
#include <QObject>
#include <gmpxx.h>
#include <cstdlib>
#include <QDebug>

class Key : public QObject
{
    Q_OBJECT
private:
    mpz_class m_pubkey;
    mpz_class m_privkey;
    mpz_class m_blocklength;
    mpz_class secret(const mpz_class& _a, const mpz_class& _b);
    void matrix_mult(mpz_class a[2][2], const mpz_class &q);

public:
    explicit Key(const quint32 &length = 0, QObject *parent = 0);
    QVector<mpz_class> *crypt(const QString &infString);
    QString decrypt(const QVector<mpz_class> *inf);
    mpz_class getMaxBlockLength() const { return m_blocklength - 1; }
    static QString toQString(const mpz_class &val, int base = 10);
    static QString toQString(const QVector<mpz_class> &valarray, int base = 10);

signals:
    
public slots:
    
};

#endif // KEY_H
