#ifndef BIGRANDOMPRIME_H
#define BIGRANDOMPRIME_H

#include <QObject>
#include <QBitArray>
#include <QDebug>

namespace BRP
{
const quint16 primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499};
const quint8 tableSize = 95;
}


namespace DecToBin
{
quint64 determinateDecSizeToBin(const quint64 &dec);
QBitArray decToBitArray(quint64 dec);
quint64 bitArrayToDec(const QBitArray &a);
QBitArray operator+(const QBitArray &a1, const QBitArray &a2);
QBitArray operator-(const QBitArray &a1, const QBitArray &a2);
QBitArray operator*(const QBitArray &a1, const QBitArray &a2);
QBitArray operator/(const QBitArray &a1, const QBitArray &a2);
QBitArray operator%(const QBitArray &a1, const QBitArray &a2);
bool operator>(const QBitArray &a1, const QBitArray &a2);
bool operator>=(const QBitArray &a1, const QBitArray &a2);
bool operator<(const QBitArray &a1, const QBitArray &a2);
bool operator<=(const QBitArray &a1, const QBitArray &a2);
bool operator==(const QBitArray &a1, const QBitArray &a2);
bool operator!=(const QBitArray &a1, const QBitArray &a2);
QBitArray operator^(const QBitArray &a1, QBitArray a2);
}

class BigRandom
{
private:
    QBitArray m_bitnumber;
    void randomize() const;
public:
    BigRandom(const quint32 length);
    QBitArray getQBitArray() { return m_bitnumber; }
};

class BigRandomPrime
{
private:
    QBitArray m_bitnumber;
    bool tableTest(const quint16 [], const quint8 tableSize) const;
    bool MRtest(const QBitArray &prime, const quint32 rounds) const;
    void randomize() const;

public:
    explicit BigRandomPrime(const quint32 length);
    BigRandomPrime(const QBitArray &bitarray) : m_bitnumber(bitarray) { }
    void show(const quint8 shift = 0) const;
    friend BigRandomPrime operator+(const BigRandomPrime &a1, const BigRandomPrime &a2);
    friend BigRandomPrime operator-(const BigRandomPrime &a1, const BigRandomPrime &a2);
    friend BigRandomPrime operator*(const BigRandomPrime &a1, const BigRandomPrime &a2);
    friend BigRandomPrime operator/(const BigRandomPrime &a1, const BigRandomPrime &a2);
    friend BigRandomPrime operator%(const BigRandomPrime &a1, const BigRandomPrime &a2);
    friend bool operator==(const BigRandomPrime &a1, const BigRandomPrime &a2);
    friend bool operator!=(const BigRandomPrime &a1, const BigRandomPrime &a2);
    quint64 ToDec() const;
    ~BigRandomPrime() { }

    bool test() const;

signals:

public slots:
    
};

#endif // BIGRANDOMPRIME_H
