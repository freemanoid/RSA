#include "bpthread.h"

BPThread::BPThread(const quint32 length)
{
    Q_ASSERT_X(length >= 2, "length of new big prime must be >= 2", "length of new big prime must be >= 2");
    m_length = length;
}

void BPThread::run()
{
    emit NewBigPrime(BigPrime(m_length));
}
