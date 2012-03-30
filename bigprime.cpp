#include "bigprime.h"

BigPrime::BigPrime(quint32 length, const BigPrime &enemy, QObject *parent) : QObject(parent)
{
    Q_ASSERT_X(length >= 2, "length of new big prime must be >= 2", "length of new big prime must be >= 2");
    m_length = length;
    m_number = randomator()->get_z_bits(length);
    mpz_setbit(mpz_ptr(&m_number), 0); //no even numbers!
    mpz_setbit(mpz_ptr(&m_number), length - 1); //we want to have at least length bits and must set first bit to be happy
    while(!primalityTest())
        m_number += 2;
    if(enemy.m_number != (mpz_class(0)))
        while(m_number == enemy.m_number)
        {
            m_number = randomator()->get_z_bits(length);
            mpz_setbit(mpz_ptr(&m_number), 0); //no even numbers!
            mpz_setbit(mpz_ptr(&m_number), length - 1); //we want to have at least length bits and must set first bit to be happy
            while(!primalityTest())
                m_number += 2;
        }
}

void BigPrime::show() const
{
    std::cout << m_number << std::endl << std::flush;
}

gmp_randclass *BigPrime::randomator() const
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
//    return tableTest(primes, tableSize) && bool(mpz_probab_prime_p(mpz_ptr(&m_number), m_length));
}

bool BigPrime::tableTest() const
{
    using namespace bp;
    for(quint8 itr = 0; itr < tableSize; ++itr)
        if(this->m_number % primes[itr] == 0)
        {
            if(this->m_number == primes[itr])
                return true;
//            qDebug() << "delitsia na " << primes[itr];
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
        mpz_class witnessOfSimplicity = (randomator()->get_z_range(m_number - 3) + 2);
        mpz_class carry;
        mpz_powm_sec(mpz_ptr(&carry), mpz_ptr(&witnessOfSimplicity), mpz_ptr(&mult), mpz_ptr(&m_number));
        if(carry == 1 || carry == (m_number - 1))
            continue;
        for(mpz_class itr = 1; itr < powerOfTwo; ++itr)
        {
            mpz_powm_ui(mpz_ptr(&carry), mpz_ptr(&carry), 2, mpz_ptr(&m_number));
            if(carry == 1)
            {
//                qDebug() << "at round " << round << " FAILED!";
                return false;
            }
            if(carry == (m_number - 1))
                goto labelnextitr;
        }
//        qDebug() << "at round " << round << " FAILED!";
        return false;
        labelnextitr: ;
    }
    return true;
}
