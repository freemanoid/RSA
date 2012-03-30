#ifndef BIGPRIME_H
#define BIGPRIME_H

#include <QObject>
#include <QDebug>
#include <iostream>
#include <gmpxx.h>
#include <key.h>

namespace bp
{
const quint16 primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499};
const quint8 tableSize = 95;
}

class BigPrime : public QObject
{
    Q_OBJECT

private:
    mpz_class m_number;
    quint32 m_length;
    static gmp_randclass *randomator();
    bool tableTest() const;
    bool MRtest() const;
    bool primalityTest() const;
//    BigPrime(mpz_class number, QObject *parent = 0);

public:
    BigPrime(mpz_class number, QObject *parent = 0);//
    explicit BigPrime(quint32 length, const BigPrime &enemy = 0,QObject *parent = 0);
    BigPrime(QObject *parent = 0) : QObject(parent), m_number(0), m_length(0) { }
    BigPrime(const BigPrime &a);
    void show() const;
    ~BigPrime() { }
    friend BigPrime operator*(const BigPrime &a1, const BigPrime &a2);
    friend BigPrime operator-(const BigPrime &a1, const quint64 &a2);
    void operator-=(const quint64 &a);
    quint32 getLength() const { return m_length; }
    mpz_class value() const { return m_number; }
    mpz_class mutuallyPrime() const;

signals:
    
public slots:
    
};

#endif // BIGPRIME_H
