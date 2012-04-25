#ifndef KEY_H
#define KEY_H

#include <bigprime.h>
#include <QObject>
#include <cstdlib>
#include <QDebug>


class Key : public QObject
{
    Q_OBJECT
public:
    typedef QVector<mpz_class> cryptedmsg;

private:
    enum {
        defaultbase = 10
    };

    mpz_class m_pubkey;
    mpz_class m_privkey;
    mpz_class m_blocklength;
    mpz_class secret(const mpz_class& _a, const mpz_class& _b);
    void matrix_mult(mpz_class a[2][2], const mpz_class &q);
    quint32 keylength;

    cryptedmsg crypt(const QString &infString);
    QString decrypt(const cryptedmsg &inf);
    mpz_class getPrivateKey() const { return m_privkey; }
    mpz_class getPublicKey() const { return m_pubkey; }
    mpz_class getMaxBlockLength() const { return m_blocklength - 1; }
    static QString toQString(const mpz_class &val, int base = 10);

    int m_base;

public:
    explicit Key(const quint32 &length, QObject *parent = 0);
    static QString toQString(const cryptedmsg &val, int base = 10);
    static QString QStringBase(const QString numericstr, int toBase, int fromBase = 10);
    int getBase() const { return m_base; }
    ~Key() {}

signals:
    void PrivateKeyUpdate(QString priv);
    void PublicKeyUpdate(QString pub);
    void MessageCrypted(Key::cryptedmsg msg);
    void MessageCryptedString(QString msg);
    void MessageDecryptedString(QString msg);
    void NewKeyGenerationStarted();
    void NewKeyGenerationFinished();

public slots:
    void NewKey(quint32 length = 0);
    void CryptMessageSlot(const QString &msg, const int OutStringBase);
    void DecryptMessageSlot(const cryptedmsg &msg);
    void KeyLengthChanged(const quint32 &length);
    void BaseChanged(const int &base);

};

#endif // KEY_H
