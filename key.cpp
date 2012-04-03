#include "key.h"

using namespace std;

Key::Key(const quint32 &length, QObject *parent) :
    QObject(parent)
{
    BigPrime p(length);
    BigPrime q(length, p);
    m_blocklength = (p * q).value();
    mpz_class m = ((p - 1) * (q - 1)).value();
    m_pubkey = BigPrime(m).mutuallyPrime();
    m_privkey = secret(m, m_pubkey);
}

QVector<mpz_class> *Key::crypt(const QString &infString)
{
    Q_ASSERT(infString.length() >= 1);

    QVector<mpz_class> *result = new QVector<mpz_class>;
//    QVector<mpz_class>::iterator itrRes = result->begin();
    quint64 itrRes = 0;

    quint64 itrString = 0;
    while(itrString < infString.size())
    {
        result->append(0);
        //<read next block>
        mpz_class digit = 0x1;
        while(itrString < infString.size() && ((*result)[itrRes] + infString.at(itrString).unicode() * digit) < m_blocklength)
        {
            (*result)[itrRes] += infString.at(itrString).unicode() * digit;
            ++itrString;
            digit *= 0x10000;
        }
//        cout << hex << (*result)[itrRes] << endl << flush;
        //crypting
        mpz_powm(mpz_ptr(&(*result)[itrRes]), mpz_ptr(&(*result)[itrRes]), mpz_ptr(&m_pubkey), mpz_ptr(&m_blocklength));
        itrRes++;
    }
    return result;
}

QString Key::decrypt(const QVector<mpz_class> *inf)
{
    QString result;
    foreach(mpz_class itr, *inf)
    {
        mpz_powm(mpz_ptr(&itr), mpz_ptr(&itr), mpz_ptr(&m_privkey), mpz_ptr(&m_blocklength));
        //TODO: check for empty
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
{
    mpz_class result;
    mpz_powm(mpz_ptr(&result), mpz_ptr(&inf), mpz_ptr(&m_privkey), mpz_ptr(&m_blocklength));
    return result;
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
