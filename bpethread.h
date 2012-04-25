//bigprime with enemy generation thread
#ifndef BPETHREAD_H
#define BPETHREAD_H

#include <QThread>
#include <bigprime.h>

class BPEThread : public QThread
{
    Q_OBJECT

private:
    BigPrime result;
    quint32 m_length;
    void run();

public:
    BPEThread(const quint32 length) : m_length(length) { }

public slots:
    void EnemyBigPrimeSlot(BigPrime enemy);

signals:
    void NewBigPrime(BigPrime number, BigPrime enemy);

};

#endif // BPETHREAD_H
