#ifndef CRYPT_H
#define CRYPT_H

#include <QObject>
#include <key.h>

class Crypt : public QObject
{
    Q_OBJECT
public:
    explicit Crypt(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // CRYPT_H
