//bigprime generation Thread
#ifndef BPTHREAD_H
#define BPTHREAD_H

#include <QThread>
#include <bigprime.h>

class BPThread : public QThread
{
    Q_OBJECT

private:
    quint32 m_length;
    void run();

public:
    BPThread(const quint32 length);
    ~BPThread() {}

signals:
    void NewBigPrime(BigPrime number);

};

#endif // BPTHREAD_H
