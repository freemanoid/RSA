#include "bigprime.h"
#include <QApplication>

BigPrime::BigPrime(quint32 length, const BigPrime &enemy)
{
    Q_ASSERT_X(length >= 2, "length of new big prime must be >= 2", "length of new big prime must be >= 2");
    m_length = length;
    m_number = randomator()->get_z_bits(length);
    mpz_setbit(mpz_ptr(&m_number), 0); //no even numbers!
    mpz_setbit(mpz_ptr(&m_number), length - 1); //we want to have at least length bits and must set first bit to be happy
    while(!primalityTest())
    {
        m_number += 2;
        qApp->processEvents();
    }
    mpz_class checker; //check if m_number fits our length (number of bits)
    mpz_ui_pow_ui(mpz_ptr(&checker), 2, ulong(length));
    if(m_number > checker)
    {
        m_number -= 2;
        while(!primalityTest())
        {
            m_number -= 2;
            qApp->processEvents();
        }
    }
    while(m_number == enemy.m_number)
    {
        m_number = randomator()->get_z_bits(length);
        mpz_setbit(mpz_ptr(&m_number), 0); //no even numbers!
        mpz_setbit(mpz_ptr(&m_number), length - 1); //we want to have at least length bits and must set first bit to be happy
        while(!primalityTest())
        {
            m_number += 2;
            qApp->processEvents();
        }
        if(m_number > checker) //check if m_number fits our length (number of bits)
        {
            m_number -= 2;
            while(!primalityTest())
            {
                m_number -= 2;
                qApp->processEvents();
            }
        }
    }
}

BigPrime::BigPrime(quint32 length)
{
    Q_ASSERT_X(length >= 2, "length of new big prime must be >= 2", "length of new big prime must be >= 2");
    m_length = length;
    m_number = randomator()->get_z_bits(length);
    mpz_setbit(mpz_ptr(&m_number), 0); //no even numbers!
    mpz_setbit(mpz_ptr(&m_number), length - 1); //we want to have at least length bits and must set first bit to be happy
    while(!primalityTest())
    {
        m_number += 2;
        qApp->processEvents();
    }
    mpz_class checker; //check if m_number fits our length (number of bits)
    mpz_ui_pow_ui(mpz_ptr(&checker), 2, ulong(length));
    if(m_number > checker)
    {
        m_number -= 2;
        while(!primalityTest())
        {
            m_number -= 2;
            qApp->processEvents();
        }
    }
}

void BigPrime::show() const
{
    std::cout << m_number << std::endl << std::flush;
}

BigPrime operator *(const BigPrime &a1, const BigPrime &a2)
{
    return BigPrime(mpz_class(a1.m_number * a2.m_number));
}

BigPrime operator -(const BigPrime &a1, const quint64 &a2)
{
    return BigPrime(mpz_class(a1.m_number - (unsigned long int)(a2)));
}

BigPrime::BigPrime(const BigPrime &a)
{
    m_length = a.m_length;
    m_number = a.m_number;
}

gmp_randclass *BigPrime::randomator()
{
    static gmp_randclass rnd(gmp_randinit_mt);
    static bool isSeeded = false;
    if(!isSeeded)
    {
        rnd.seed(time(NULL));
        isSeeded = true;
    }
    return &rnd;
}

bool BigPrime::primalityTest() const
{
    return tableTest() && MRtest();
}

BigPrime::BigPrime(mpz_class number) : m_number(number)
{
    m_length = 1;
    while(number / 2 > 0)
    {
        number /= 2;
        ++m_length;
    }
}

bool BigPrime::tableTest() const
{
    using namespace bp;
    for(quint8 itr = 0; itr < tableSize; ++itr)
        if(this->m_number % primes[itr] == 0)
        {
            if(this->m_number == primes[itr])
                return true;
            return false;
        }
    return true;
}

bool BigPrime::MRtest() const
{
    quint32 powerOfTwo = 0;
    mpz_class mult = m_number - 1;
    while(mult % 2 == 0)
    {
        powerOfTwo++;
        mult /= 2;
    }
    for(quint32 round = 1; round <= m_length; ++round)
    {
        qApp->processEvents();
        mpz_class witnessOfSimplicity = (randomator()->get_z_range(m_number - 3) + 2);
        mpz_class carry;
        mpz_powm_sec(mpz_ptr(&carry), mpz_ptr(&witnessOfSimplicity), mpz_ptr(&mult), mpz_ptr(&m_number));
        if(carry == 1 || carry == (m_number - 1))
            continue;
        qApp->processEvents();
        for(mpz_class itr = 1; itr < powerOfTwo; ++itr)
        {
            qApp->processEvents();
            mpz_powm_ui(mpz_ptr(&carry), mpz_ptr(&carry), 2, mpz_ptr(&m_number));
            if(carry == 1)
                return false;
            if(carry == (m_number - 1))
                goto labelnextitr;
            qApp->processEvents();
        }
        return false;
        labelnextitr: ;
    }
    return true;
}

mpz_class BigPrime::mutuallyPrime() const
{
    mpz_class result = randomator()->get_z_bits(this->getLength() - 1) + 5;
    mpz_setbit(mpz_ptr(&result), 0); //no even numbers!
    mpz_setbit(mpz_ptr(&result), mpz_sizeinbase(mpz_ptr(&result), 2) - 1); //we want to have at least length bits and must set first bit to be happy
    mpz_class checker; //check if m_number fits our length (number of bits)
    mpz_ui_pow_ui(mpz_ptr(&checker), 2, ulong(mpz_sizeinbase(mpz_ptr(&result), 2)));
    mpz_class devider = 2;
    do
    {
        result += 2;
        if(result > checker)
            break;
        mpz_class a = this->m_number;
        mpz_class b = result;
        mpz_class rest;
        while(true)
        {
            rest = a % b;
            if(rest == 0)
            {
                devider = b;
                break;
            }
            a = b;
            b = rest;
        }
    } while (devider > 1);
    if(devider <= 1 && result <= checker)
        return result;

    devider = 2;
    do
    {
        result -= 2;
        mpz_class a = this->m_number;
        mpz_class b = result;
        mpz_class rest;
        while(true)
        {
            rest = a % b;
            if(rest == 0)
            {
                devider = b;
                break;
            }
            a = b;
            b = rest;
        }
    } while (devider > 1);
    return result;
}

void BigPrime::operator -=(const quint64 &a)
{
    m_number = m_number - (unsigned long int)(a);
    m_length = 1;
    mpz_class carry = m_number;
    while(carry / 2 > 0)
    {
        carry /= 2;
        ++m_length;
    }
}

bool operator ==(const BigPrime &a1, const BigPrime &a2)
{
    if(a1.m_number == a2.m_number)
        return true;
    return false;
}

