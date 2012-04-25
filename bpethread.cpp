#include "bpethread.h"

void BPEThread::run()
{
    result = BigPrime(m_length);
    exec();
}

void BPEThread::EnemyBigPrimeSlot(BigPrime enemy)
{
    while(result == enemy)
        result = BigPrime(m_length);
    emit NewBigPrime(result, enemy);
    exit();
}
