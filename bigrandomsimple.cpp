#include "bigrandomsimple.h"
#include <QTime>
#include <QDebug>
#include <iostream>

BigRandomSimple::BigRandomSimple(const quint16 length) : m_bitnumber(length)
{
    qsrand(time(NULL));
    m_bitnumber.setBit(0, true);
    for(quint16 itr = 1; itr < m_bitnumber.size(); ++itr)
        m_bitnumber.setBit(itr, bool(qrand() % 2));
}

void BigRandomSimple::show(quint8 shift = 0) const
{
    for(quint8 itr = 0; itr < shift; ++itr)
        std::cout << 0 << std::flush;
    for(quint16 itr = 0; itr < m_bitnumber.size(); ++itr)
        std::cout << m_bitnumber.testBit(itr) << std::flush;
    std::cout << std::endl << std::flush;
}

BigRandomSimple BigRandomSimple::operator +(const BigRandomSimple &a1)
{
    QBitArray result = DecToBin::operator +(this->m_bitnumber, a1.m_bitnumber);
    return BigRandomSimple(result);
}

BigRandomSimple BigRandomSimple::operator -(const BigRandomSimple &a1)
{
    QBitArray result = DecToBin::operator -(this->m_bitnumber, a1.m_bitnumber);
    return BigRandomSimple(result);
}

bool BigRandomSimple::tableTest(const quint16 table[], const quint8 tableSize) const
{


}

QBitArray DecToBin::operator+(const QBitArray &a1, const QBitArray &a2)
{
    QBitArray result(qMax(a1.size(), a2.size()) + 1);
    const QBitArray *min = a1.size() < a2.size() ? &a1 : &a2;
    quint16 itr = 1;
    bool rest = 0;
    bool nextrest = 0;
    for(; itr <= min->size(); ++itr)
    {
        result.setBit(result.size() - itr, a1.testBit(a1.size() - itr) ^ a2.testBit(a2.size() - itr));
        nextrest = a1.testBit(a1.size() - itr) && a2.testBit(a2.size() - itr);
        nextrest = (result.testBit(result.size() - itr) && rest) || nextrest;
        result.setBit(result.size() - itr, result.testBit(result.size() - itr) ^ rest);
        rest = nextrest;
    }
    const QBitArray *max = a1.size() > a2.size() ? &a1 : &a2;
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
    for(quint16 itr = 0; itr < resultWithLessSize.size(); ++itr)
        resultWithLessSize.setBit(resultWithLessSize.size() - itr, result.testBit(result.size() - itr));
    return resultWithLessSize;
}

QBitArray DecToBin::operator-(const QBitArray &a1, const QBitArray &a2)
{
    QBitArray result(qMax(a1.size(), a2.size()));
    const QBitArray *min = a1.size() < a2.size() ? &a1 : &a2;
    quint16 itr = 1;
    bool rest = 0;
    bool nextrest = 0;
    for(; itr <= min->size(); ++itr)
    {
        result.setBit(result.size() - itr, a1.testBit(a1.size() - itr) ^ a2.testBit(a2.size() - itr));
        nextrest = !a1.testBit(a1.size() - itr) && a2.testBit(a2.size() - itr);
        nextrest = (!result.testBit(result.size() - itr) && rest) || nextrest;
        result.setBit(result.size() - itr, result.testBit(result.size() - itr) ^ rest);
        rest = nextrest;
    }
    const QBitArray *max = a1.size() > a2.size() ? &a1 : &a2;
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
    for(itr = 0; !result.testBit(itr); ++itr)
        --lessResultSize;
    QBitArray resultWithLessSize(lessResultSize);
    for(itr = 0; itr < lessResultSize; ++itr)
        resultWithLessSize.setBit(resultWithLessSize.size() - itr, result.testBit(result.size() - itr));
    return resultWithLessSize;
}

bool DecToBin::operator>(const QBitArray &a1, const QBitArray &a2) //this realization basics on concept that each QBitArray do not contain any zeros (falses) in the begining (0, 1, 2 etc indexes)
{
    if(a1.size() > a2.size() && a1.testBit(0))
        return true;
    if(a1.size() < a2.size() && a2.testBit(0))
        return false;
    for(quint16 itr = 0; itr < a1.size(); ++itr) //now we have QBitArrays with similar size and just need to check the bits from highest to lowest
        if(a1.testBit(itr) < a2.testBit(itr))
            return false;
    return true;
}

bool DecToBin::operator<(const QBitArray &a1, const QBitArray &a2) //this realization basics on concept that each QBitArray do not contain any zeros (falses) in the begining (0, 1, 2 etc indexes)
{
    if(a1.size() < a2.size() && a1.testBit(0))
        return true;
    if(a1.size() > a2.size() && a2.testBit(0))
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
