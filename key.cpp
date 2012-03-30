#include "key.h"

using namespace std;

Key::Key(const quint32 &length, QObject *parent) :
    QObject(parent)
{
    BigPrime p(length);
    BigPrime q(length, p);
    cout << "p = " << p.value() << endl << flush;
    cout << "q = " << q.value() << endl << flush;
    m_blocklength = (p * q).value();
    cout << "m_blocklength(p * q) = " << m_blocklength << endl << flush;
    mpz_class m = ((p - 1) * (q - 1)).value();
    cout << "m((p - 1) * (q - 1)) = " << m << endl << flush;
    m_pubkey = BigPrime(m).mutuallyPrime();
    cout << "m_pubkey(mutuallyprime to m) = " << m_pubkey << endl << flush;
    m_privkey = secret(m, m_pubkey);
    cout << "m_privkey(secret(m, m_pubkey)) = " << m_privkey << endl << flush;
}

mpz_class Key::crypt(const mpz_class &inf)
{
    Q_ASSERT_X(inf < m_blocklength, "block length crypt", "information block size must be less than max block length");
    mpz_class result;
//    mpz_class a = 3;
//    mpz_class b = 319;
//    mpz_powm(mpz_ptr(&result), mpz_ptr(&inf), mpz_ptr(&a), mpz_ptr(&b));
    mpz_powm(mpz_ptr(&result), mpz_ptr(&inf), mpz_ptr(&m_pubkey), mpz_ptr(&m_blocklength));
    return result;
}

mpz_class Key::decrypt(const mpz_class &inf)
{
    mpz_class result;
//    mpz_class a = -93;
//    mpz_class b = 319;
//    mpz_powm(mpz_ptr(&result), mpz_ptr(&inf), mpz_ptr(&a), mpz_ptr(&b));
    mpz_powm(mpz_ptr(&result), mpz_ptr(&inf), mpz_ptr(&m_privkey), mpz_ptr(&m_blocklength));
    return result;
}

void Key::matrix_mult(mpz_class a[2][2], const mpz_class &q)
{
    mpz_class b[2][2] = {{0, 1}, {1, -q}};
//    mpz_class b[2][2] = {{0, 1}, {1, q}};
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
//        matrix_mult(E, a > b ? a - q : b - q);
        a = b;
        b = r;
    }
    return E[1][1];
}
