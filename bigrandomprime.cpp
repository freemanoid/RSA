#include "bigrandomprime.h"
#include <QTime>
#include <QDebug>
#include <iostream>

BigRandomPrime::BigRandomPrime(const quint16 length) : m_bitnumber(length)
{
    qsrand(time(NULL));
    m_bitnumber.setBit(0, 1);
    for(quint16 itr = 1; itr < m_bitnumber.size(); ++itr)
        m_bitnumber.setBit(itr, bool(qrand() % 2));
}

void BigRandomPrime::show(const quint8 shift) const
{
    for(quint8 itr = 0; itr < shift; ++itr)
        std::cout << 0 << std::flush;
    for(quint16 itr = 0; itr < m_bitnumber.size(); ++itr)
        std::cout << m_bitnumber.testBit(itr) << std::flush;
    std::cout << std::endl << std::flush;
}

BigRandomPrime BigRandomPrime::operator +(const BigRandomPrime &a1)
{
    QBitArray result = DecToBin::operator +(this->m_bitnumber, a1.m_bitnumber);
    return BigRandomPrime(result);
}

BigRandomPrime BigRandomPrime::operator -(const BigRandomPrime &a1)
{
    QBitArray result = DecToBin::operator -(this->m_bitnumber, a1.m_bitnumber);
    return BigRandomPrime(result);
}

BigRandomPrime BigRandomPrime::operator *(const BigRandomPrime &a1)
{
    QBitArray result = DecToBin::operator *(this->m_bitnumber, a1.m_bitnumber);
    return BigRandomPrime(result);
}

quint64 BigRandomPrime::ToDec() const
{
    return DecToBin::bitArrayToDec(m_bitnumber);
}

bool BigRandomPrime::tableTest(const quint16 table[], const quint8 tableSize) const
{


}

quint64 DecToBin::bitArrayToDec(const QBitArray &a)
{
    Q_ASSERT_X(a.size() < 64, "quint64", "quint64 cannot have more than 63 bits length");
    quint64 result = 0;
    quint64 poweroftwo = 1;
    for(qint8 itr = a.size() - 1; itr >= 0; --itr, poweroftwo *= 2)
        if(a.testBit(itr))
            result += poweroftwo;
    return result;
}

QBitArray DecToBin::operator+(const QBitArray &a1, const QBitArray &a2)
{
    QBitArray result(qMax(a1.size(), a2.size()) + 1);
    const QBitArray *min = a1.size() < a2.size() ? &a1 : &a2;
    const QBitArray *max = a1.size() > a2.size() ? &a1 : &a2;
    quint16 itr = 1;
    bool rest = 0;
    bool nextrest = 0;
    for(; itr <= min->size(); ++itr)
    {
        result.setBit(result.size() - itr, max->testBit(max->size() - itr) ^ min->testBit(min->size() - itr));
        nextrest = max->testBit(max->size() - itr) && min->testBit(min->size() - itr);
        nextrest = (result.testBit(result.size() - itr) && rest) || nextrest;
        result.setBit(result.size() - itr, result.testBit(result.size() - itr) ^ rest);
        rest = nextrest;
    }
    nextrest = 0;
    for(;itr <= max->size(); ++itr)
    {
        result.setBit(result.size() - itr, max->testBit(max->size() - itr) ^ rest);
        rest = max->testBit(max->size() - itr) && rest;
    }
    if(rest)
    {
        result.setBit(0, 1);
        return result;
    }
    QBitArray resultWithLessSize(result.size() - 1);
    for(quint16 itr = 1; itr <= resultWithLessSize.size(); ++itr)
        resultWithLessSize.setBit(resultWithLessSize.size() - itr, result.testBit(result.size() - itr));
    return resultWithLessSize;
}

QBitArray DecToBin::operator-(const QBitArray &a1, const QBitArray &a2)
{
    QBitArray result(qMax(a1.size(), a2.size()));
    const QBitArray *min = a1 < a2 ? &a1 : &a2;
    const QBitArray *max = a1 > a2 ? &a1 : &a2;
    if(max == min)
        return QBitArray(0);
    quint16 itr = 1;
    bool rest = 0;
    bool nextrest = 0;
    for(; itr <= min->size(); ++itr)
    {
        result.setBit(result.size() - itr, max->testBit(max->size() - itr) ^ min->testBit(min->size() - itr));
        nextrest = !max->testBit(max->size() - itr) && min->testBit(min->size() - itr);
        nextrest = (!result.testBit(result.size() - itr) && rest) || nextrest;
        result.setBit(result.size() - itr, result.testBit(result.size() - itr) ^ rest);
        rest = nextrest;
    }
    nextrest = 0;
    for(;itr <= max->size(); ++itr)
    {
        result.setBit(result.size() - itr, max->testBit(max->size() - itr) ^ rest);
        rest = !max->testBit(max->size() - itr) && rest;
    }
    if(rest)
    {
        result.setBit(0, 0);
        return result;
    }
    quint16 lessResultSize = result.size();
    for(itr = 0; itr < result.size() && !result.testBit(itr); ++itr)
        --lessResultSize;
    QBitArray resultWithLessSize(lessResultSize);
    for(itr = 1; itr <= lessResultSize; ++itr)
        resultWithLessSize.setBit(resultWithLessSize.size() - itr, result.testBit(result.size() - itr));
    return resultWithLessSize;
}

QBitArray DecToBin::operator*(const QBitArray &a1, const QBitArray &a2)
{
    QBitArray result(0);
    if(a1.size() == 0 || a2.size() == 0)
        return result; //empty array
    result.resize(a1.size() + a2.size());
    quint16 a1size = a1.size();
    for(quint16 itr = 1; itr <= a2.size(); ++itr)
    {
        if(a2.testBit(a2.size() - itr))
            result = result + a1;
        const_cast<QBitArray &>(a1).resize(a1.size() + 1); //in this way we make left shift
    }
    const_cast<QBitArray &>(a1).resize(a1size);
    return result;
}

bool DecToBin::operator>(const QBitArray &a1, const QBitArray &a2) //this realization basics on concept that each QBitArray do not contain any zeros (falses) in the begining (0, 1, 2 etc indexes)
{
    if(a1.size() > a2.size())
        return true;
    if(a1.size() < a2.size())
        return false;
    for(quint16 itr = 0; itr < a1.size(); ++itr) //now we have QBitArrays with similar size and just need to check the bits from highest to lowest
        if(a2.testBit(itr) < a1.testBit(itr))
            return true;
    return false;
}

bool DecToBin::operator<(const QBitArray &a1, const QBitArray &a2) //this realization basics on concept that each QBitArray do not contain any zeros (falses) in the begining (0, 1, 2 etc indexes)
{
    if(a1.size() < a2.size())
        return true;
    if(a1.size() > a2.size())
        return false;
    for(quint16 itr = 0; itr < a1.size(); ++itr) //now we have QBitArrays with similar size and just need to check the bits from highest to lowest
        if(a1.testBit(itr) > a2.testBit(itr))
            return false;
    return true;
}

quint64 DecToBin::determinateDecSizeToBin(const quint64 &dec)
{
    if(dec == 0)
        return 1;
    quint64 result = 0;
    quint64 powerOfTwo = 1;
    while(dec >= powerOfTwo)
    {
        powerOfTwo *= 2;
        ++result;
    }
    return result;
}

QBitArray *DecToBin::decToBitArray(quint64 dec)
{
    QBitArray *result = new QBitArray(determinateDecSizeToBin(dec));
    quint16 index = 0;
    do
    {
        result->setBit(index, dec % 2);
        ++index;
    }
    while(!(dec /= 2));
    return result;
}
