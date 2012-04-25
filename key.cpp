#include "key.h"
//#include <mainwindow.h>
#include <bpethread.h>
#include <bpthread.h>

#include <QApplication>

using namespace std;

Key::Key(const quint32 &length, QObject *parent) :
    QObject(parent), m_base(defaultbase)
{
    Q_ASSERT(length >= 8);
    keylength = length;
    BigPrime p(length);
    BigPrime q(length, p);
    m_blocklength = (p * q).value();
    mpz_class m = ((p - 1) * (q - 1)).value();
    m_pubkey = BigPrime(m).mutuallyPrime();
    m_privkey = secret(m, m_pubkey);
    emit PublicKeyUpdate(toQString(m_pubkey, m_base));
    emit PrivateKeyUpdate(toQString(m_privkey, m_base));
}

void Key::NewKey(quint32 length)
{
    emit NewKeyGenerationStarted();
    if(length == 0)
        length = keylength;
    else
        if(length >= 8)
            keylength = length;
    Q_ASSERT(length >= 8);

    BigPrime p(length);
    BigPrime q(length, p);
    m_blocklength = (p * q).value();
    mpz_class m = ((p - 1) * (q - 1)).value();
    m_pubkey = BigPrime(m).mutuallyPrime();
    m_privkey = secret(m, m_pubkey);
    emit PublicKeyUpdate(toQString(m_pubkey, m_base));
    emit PrivateKeyUpdate(toQString(m_privkey, m_base));
    emit NewKeyGenerationFinished();
}

//void Key::NewKey(quint32 length)
//{
//    emit NewKeyGenerationStarted();
//    if(length == 0)
//        length = keylength;
//    else
//        if(length >= 8)
//            keylength = length;
//    Q_ASSERT(length >= 8);

//    qRegisterMetaType<BigPrime>("BigPrime");
//    BPThread *BigPrimeThread = new BPThread(length);
//    connect(BigPrimeThread, SIGNAL(finished()), BigPrimeThread, SLOT(deleteLater()));
//    BPEThread *BigPrimeWithEnemyThread = new BPEThread(length);
//    connect(BigPrimeWithEnemyThread, SIGNAL(finished()), BigPrimeWithEnemyThread, SLOT(deleteLater()));

//    connect(BigPrimeThread, SIGNAL(NewBigPrime(BigPrime)), BigPrimeWithEnemyThread, SLOT(EnemyBigPrimeSlot(BigPrime)));
//    connect(BigPrimeWithEnemyThread, SIGNAL(NewBigPrime(BigPrime,BigPrime)), this, SLOT(ContinueNewKeyGeneration(BigPrime,BigPrime)));
//    BigPrimeWithEnemyThread->start();
//    BigPrimeThread->start();
//}


//void Key::ContinueNewKeyGeneration(BigPrime p, BigPrime q)
//{
//    m_blocklength = (p * q).value();
//    mpz_class m = ((p - 1) * (q - 1)).value();
//    m_pubkey = BigPrime(m).mutuallyPrime();
//    m_privkey = secret(m, m_pubkey);
//    emit PublicKeyUpdate(toQString(m_pubkey, m_base));
//    emit PrivateKeyUpdate(toQString(m_privkey, m_base));
//    emit NewKeyGenerationFinished();
//}

Key::cryptedmsg Key::crypt(const QString &infString)
{
    Q_ASSERT(infString.length() >= 1);

    cryptedmsg result;
    quint64 itrRes = 0;

    quint64 itrString = 0;
    while(itrString < infString.size())
    {
        result.append(0);
        //<read next block>
        mpz_class digit = 0x1;
        //write infString's unicode to result without crypting in REVERSE order
        while(itrString < infString.size() && (result[itrRes] + infString.at(itrString).unicode() * digit) < m_blocklength)
        {
            result[itrRes] += infString.at(itrString).unicode() * digit;
            ++itrString;
            digit *= 0x10000;
        }
        //crypt current block of unicode chars 
        mpz_powm(mpz_ptr(&result[itrRes]), mpz_ptr(&result[itrRes]), mpz_ptr(&m_pubkey), mpz_ptr(&m_blocklength));
        itrRes++;
    }
    return result;
}

//message struct:


//QString Key::crypt(const QString &infString, int base)
//{
//    Q_ASSERT(infString.length() >= 1);

//    QString result;

//    quint64 itrString = 0;
//    while(itrString < infString.size())
//    {
//        //<read next block>
//        mpz_class digit = 0x1;
//        mpz_class current = 0x0;
//        //write infString's unicode to current without crypting in REVERSE order
//        while(itrString < infString.size() && (current + infString.at(itrString).unicode() * digit) < m_blocklength)
//        {
//            current += infString.at(itrString).unicode() * digit;
//            ++itrString;
//            digit *= 0x10000;
//        }
//        //crypt current block of unicode chars
//        mpz_powm(mpz_ptr(&current), mpz_ptr(&current), mpz_ptr(&m_pubkey), mpz_ptr(&m_blocklength));
//        char *currentCharArr;
//        mpz_set_str(mpz_ptr(&current), currentCharArr, base);
//        result.append(QString(currentCharArr).size());
//        result.append(currentCharArr);
//        delete [] currentCharArr;
//    }
//    return result;
//}

QString Key::decrypt(const cryptedmsg &inf)
{
    QString result;
    foreach(mpz_class itr, inf)
    {
        //decrypt current block
        mpz_powm(mpz_ptr(&itr), mpz_ptr(&itr), mpz_ptr(&m_privkey), mpz_ptr(&m_blocklength));
        //TODO: check for empty
        //write current block to result in REVERSE order char by char from unicode
        mpz_class carry = itr % 0x10000;
        result.append(QChar(static_cast<uint>(mpz_get_ui(mpz_ptr(&carry)))));
        while((itr /= 0x10000) > 0)
        {
            carry = itr % 0x10000;
            result.append(QChar(static_cast<uint>(mpz_get_ui(mpz_ptr(&carry)))));
        }
    }
    return result;
}

//QString Key::decrypt(const QString &inf, int base)
//{
//    QString result;
//    quint64 blockCount;/* =*/
//    if(inf.size() % mpz_sizeinbase(mpz_ptr(&m_blocklength), base) > 0)
//        blockCount++;
//    for(quint64 itr = 0; itr < blockCount; itr++)
//    {
//        QStringRef currentString = inf.midRef(mpz_sizeinbase(mpz_ptr(&m_blocklength), base) * itr, mpz_sizeinbase(mpz_ptr(&m_blocklength ), base));
////        qDebug() << "inf: " << inf;
//        qDebug() << "curr: " << currentString;
//        mpz_class currentValue = 0;
//        //geting current block and converting it to numeric value
////        можно в mpz_class обычной функцией своей а не циклом
////        mpz_set_str(mpz_ptr(&currentValue), currentString.to)
//        for(mpz_class itr = 1, digit = 1; itr <= currentString.size(); ++itr)
//        {
//            currentValue += currentString.at(currentString.size() - mpz_get_ui(mpz_ptr(&itr))).digitValue() * digit;
//            digit *= base;
//        }
//        //decrypt current block
//        mpz_powm(mpz_ptr(&currentValue), mpz_ptr(&currentValue), mpz_ptr(&m_privkey), mpz_ptr(&m_blocklength));
//        //write decrypted current block to result in REVERSE order char by char from unicode
//        mpz_class carry = currentValue % 0x10000;
//        result.append(QChar(static_cast<uint>(mpz_get_ui(mpz_ptr(&carry)))));
//        while((currentValue /= 0x10000) > 0)
//        {
//            carry = currentValue % 0x10000;
//            result.append(QChar(static_cast<uint>(mpz_get_ui(mpz_ptr(&carry)))));
//        }
//    }
//    return result;
//}

QString Key::toQString(const mpz_class &val, int base)
{
    Q_ASSERT(base >= 2 && base <= 62);
    return QString(mpz_get_str(NULL, base, mpz_ptr(&val)));
}

QString Key::toQString(const cryptedmsg &val, int base)
{
    Q_ASSERT(base >= 2 && base <= 62);
    QString result;
    foreach(const mpz_class &itr, val)
        result.append(mpz_get_str(NULL, base, mpz_ptr(&itr)));
    return result;
}

QString Key::QStringBase(const QString numericstr, int toBase, int fromBase)
{
    Q_ASSERT(toBase >= 2 && toBase <= 62 && fromBase >= 2 && fromBase <= 62);

    QString result = "";
    mpz_class carry;
    std::string str = numericstr.toStdString();
    char* str_ptr = new char[str.length() + 1];
    strcpy(str_ptr, str.c_str());
    mpz_set_str(mpz_ptr(&carry), str_ptr, fromBase);
    delete [] str_ptr;
    if(carry == 0)
        return result;
    str_ptr = mpz_get_str(NULL, toBase, mpz_ptr(&carry));
    result = QString(str_ptr);
    delete [] str_ptr;
    return result;
}

void Key::CryptMessageSlot(const QString &msg, const int OutStringBase)
{
    cryptedmsg carry = crypt(msg);
    emit MessageCrypted(carry);
    emit MessageCryptedString(toQString(carry, OutStringBase));
}

void Key::DecryptMessageSlot(const Key::cryptedmsg &msg)
{
    emit MessageDecryptedString(decrypt(msg));
}

void Key::KeyLengthChanged(const quint32 &length)
{
    Q_ASSERT(length >= 8);
    keylength = length;
}

void Key::BaseChanged(const int &base)
{
    Q_ASSERT(base >= 2 && base <= 62);
    m_base = base;
}

void Key::matrix_mult(mpz_class a[2][2], const mpz_class &q)
{
    mpz_class b[2][2] = {{0, 1}, {1, -q}};
    mpz_class carry = a[0][0];
    a[0][0] = carry * b[0][0] + a[0][1] * b[1][0];
    a[0][1] = carry * b[0][1] + a[0][1] * b[1][1];
    carry = a[1][0];
    a[1][0] = carry * b[0][0] + a[1][1] * b[1][0];
    a[1][1] = carry * b[0][1] + a[1][1] * b[1][1];
}

mpz_class Key::secret(const mpz_class& _a, const mpz_class& _b)
{
    mpz_class E[2][2] = {{1, 0}, {0, 1}};
    mpz_class r, q, b = _b, a = _a;
    while((r = a % b) != 0)
    {
        q = a / b;
        matrix_mult(E, q);
        a = b;
        b = r;
    }
    return E[1][1];
}
