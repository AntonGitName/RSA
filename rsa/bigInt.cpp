#include "bigInt.h"

inline void fft (std::vector<complex_double> & a, bool invert) {
    int n = (int) a.size();
    if (n == 1)  return;
 
    std::vector<complex_double> a0 (n/2),  a1 (n/2);
    for (int i = 0, j = 0; i < n; i += 2, ++j)
    {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }
    fft (a0, invert);
    fft (a1, invert);
 
    double ang = 2*PI/n * (invert ? -1 : 1);
    complex_double w(1),  wn (cos(ang), sin(ang));
    for (int i = 0; i < n / 2; ++i)
    {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert)
            a[i] /= 2,  a[i + n / 2] /= 2;
        w *= wn;
    }
}

const bigInt fastMul(const bigInt& a, const bigInt& b)
{
    bigInt product;
    bool differentSigns = a.sign != b.sign;
    std::vector<complex_double> fa (a.a.begin(), a.a.end()),  fb (b.a.begin(), b.a.end());
    size_t n = 1;
    while (n < std::max (a.size(), b.size()))  n *= 2;
    n <<= 1;
    fa.resize (n),  fb.resize (n);
 
    fft (fa, false),  fft (fb, false);
    for (size_t i=0; i<n; ++i)
        fa[i] *= fb[i];
    fft (fa, true);
 
    product.a.resize(n);
    for (size_t i = 0; i < n; ++i)
    {
        if (fa[i].real() > 0)
            product.a[i] = int (fa[i].real() + 0.5);
        else
            product.a[i] = int (fa[i].real() - 0.5);
    }
    int carry = 0;
    for (size_t i = 0; i < n; ++i)
    {
        product.a[i] += carry;
        carry = product.a[i] / bigInt::base;
        product.a[i] %= bigInt::base;
    }
    product.sign = differentSigns;
    product.deleteNulls();
    return product;
}


std::ostream& operator<< (std::ostream &out, const bigInt &x) {return out << x.toString();}

std::istream& operator>> (std::istream &in, bigInt &x)
{
    std::string s;
    in >> s;
    x = bigInt(s);
    return in;
}

const bigInt power(bigInt a, bigInt b) // a ^ b
{
    if (b == 0) return 1;
    if (b == 1) return a;
    bigInt k = b / 2, t = power(a, k);
    t = (t * t);
    return (b % 2 == 1) ? (a * t) : t;
}

const bigInt power(bigInt a, bigInt b, const bigInt& m) // a ^ b mod m
{
    if (b == 0) return 1;
    if (b == 1) return a;
    if (a > m) a = a % m;
    bigInt k = b / 2, t = power(a, k, m) % m;
    t = (t * t) % m;
    return (b % 2 == 1) ? (a * t) % m : t;
}


std::string bigInt::toString() const
{
    if (a.empty())
        return "0";
    std::string str, tmp;
    if (sign)
        str = '-';
    int log10base = 1, l = 1;
    while ((l *= 10) < bigInt::base)
        ++log10base;
    int y = a.back(), k = 0;
    while (y)
    {
        tmp.push_back(y % 10 + '0');
        y /= 10;
    }
    tmp.assign(tmp.rbegin(), tmp.rend());
    if (tmp.length())
        str += tmp;
    else
        str += '0';
    for (int i = (int)a.size() - 2; i >= 0; --i)
    {
        tmp.assign(log10base, '0');
        int x = a[i], k = 0;
        while (x)
        {
            tmp[log10base - 1 - k++] = x % 10 + '0';
            x /= 10;
        }
        str += tmp;
    }
    return str;
}

//bool bigInt::absLess(const bigInt & x) const
//{
//    if (a.size() < x.a.size())
//        return true;
//    if (a.size() > x.a.size())
//        return false;
//    for (int i = (int)a.size() - 1; i >= 0; --i)
//        if (a[i] != x.a[i])
//            return a[i] < x.a[i];
//    return false;
//}

inline const bigInt operator+(const bigInt& a, const bigInt& b)
{
    if (a.sign ^ b.sign)
    {
        return a - (-b);
    }
    else
    {
        bigInt c;
        c.sign = a.sign;
        int carry = 0;
        size_t n = std::max(a.a.size(), b.a.size());
        for (size_t i = 0; i < n || carry; ++i)
        {
            c.a.push_back(carry + ((i < a.a.size()) ? a.a[i] : 0) + ((i < b.a.size()) ? b.a[i] : 0));
            if (carry = c.a[i] >= bigInt::base)
                c.a[i] -= bigInt::base;
        }
        c.deleteNulls();
        return c;
    }
}

inline const bigInt operator-(const bigInt& a, const bigInt& b)
{
    if (a.sign ^ b.sign)
    {
        return a + (-b);
    }
    else
    {
        if (a.absLess(b))
            return -(b - a);
        bigInt c = a;

        int carry = 0;
        for (size_t i = 0; i < b.a.size() || carry; ++i) {
            c.a[i] = ((i < a.a.size() ? a.a[i] : 0) - carry - (i < b.a.size() ? b.a[i] : 0));
            if (carry = c.a[i] < 0) 
                c.a[i] += bigInt::base;
        }
        c.deleteNulls();
        return c;
    }
}

inline const bigInt operator*(const bigInt& a, const bigInt& b)
{
    if (b == 2)
    {
        bigInt c = a;
        c.a.push_back(0);
        int carry = 0;
        for (size_t i = 0; i < c.a.size(); ++i)
        {
            c.a[i] = (c.a[i] << 1) + carry;
            if (c.a[i] >= bigInt::base)
            {
                carry = 1;
                c.a[i] -= bigInt::base;
            }
        }
        if (c.a.back() == 0)
            c.a.pop_back();
        return c;
    }
    return fastMul(a, b);
}

inline const bigInt operator/(const bigInt& x, const bigInt& y)
{
    if (x.absLess(y))
        return 0;
<<<<<<< HEAD
    if (y == 2)
    {
        bigInt z = x;
        for (size_t i = z.a.size() - 1; i > 0; --i)
        {
            z.a[i - 1] += bigInt::base * (z.a[i] & 1);
            z.a[i] >>= 1;
        }
        z.a[0] >>= 1;
        if (z.a.back() == 0)
            z.a.pop_back();
        return z;
    }
    bigInt a = x;
    a.sign = false;
    bigInt b = y;
    b.sign = false;
    bigInt m;
    bigInt l = 1;
    bigInt r;
    r.a.assign((int)a.a.size()-(int)b.a.size() + 1, bigInt::base - 1); r.sign = false;
    while (l < r)
    {
        m = (l + r + 1) / 2;
        if (a >= m * b)
            l = m;
        else
            r = m - 1;
    }
    l.sign = x.sign ^ y.sign;
    l.deleteNulls();
    return l;
}

const bigInt operator%(const bigInt& x, const bigInt& y)
{
    if (x.a.empty())
        return 0;
    if (y == 1)
        return 0;
    if (y == 2)
        return x.a[0] & 1;
    if (y == 4)
        return x.a[0] & 3;
    if (y == 8)
        return x.a[0] & 7;
    if (y == 16)
        return x.a[0] & 15; // Dependes on bigInt::base. Change carefully.
    return x - (x / y) * y;
}

inline int numCount(int a)
{
    int x = 1, l = 1;
    while (l *= 10 < a)
        ++x;
    return x;
}

inline const bigInt operator/(const bigInt& x, const int& y)
{
    if (y == 2)
    {
        bigInt z = x;
        for (size_t i = z.a.size() - 1; i > 0; --i)
        {
            z.a[i - 1] += bigInt::base * (z.a[i] & 1);
            z.a[i] >>= 1;
        }
        z.a[0] >>= 1;
        if (z.a.back() == 0)
            z.a.pop_back();
        return z;
    }
    if (y >= bigInt::base)
        return x / bigInt(y);
    if (y == 2)
    {
        bigInt c = x;
        int carry = 0;
        for (size_t i = c.a.size() - 1; i > 0; --i)
        {
            c.a[i - 1] += bigInt::base * (c.a[i] & 1);
            c.a[i] >>= 1;
        }
        c.a[0] >>= 1;
        if (c.a.back() == 0)
            c.a.pop_back();
        return c;
    }    
    bigInt c;
    c.sign = x.sign ^ (y < 0);
    c.a.assign(x.a.size(), 0);
    int carry = 0;
    for (int i = (int) x.a.size() - 1; i >= 0; --i)
    {
        int cur = x.a[i] + carry * bigInt::base;
        c.a[i] = int (cur / y);
        carry = int (cur % y);
    }
    c.deleteNulls();
    return c;
}

inline const bool operator<(const bigInt&x, const bigInt&y)
{
    if (x.sign ^ y.sign)
        return x.sign;
    return x.absLess(y) ^ x.sign;
}

const bool operator>(const bigInt&x, const bigInt&y)
{
    if (x.sign ^ y.sign)
        return y.sign;
    return y.absLess(x) ^ x.sign;
}

inline const bool operator<=(const bigInt&x, const bigInt&y)
{
    return !(y < x);
}

const bool operator>=(const bigInt&x, const bigInt&y)
{
    return !(x < y);
}

inline const bool operator==(const bigInt&x, const bigInt&y)
{
    if (x.sign ^ y.sign || x.a.size() != y.a.size())
        return false;
    for (size_t i = 0; i < x.a.size(); ++i)
        if (x.a[i] != y.a[i])
            return false;
    return true;
}

const bool operator!=(const bigInt&x, const bigInt&y)
{
    if (x.sign ^ y.sign || x.a.size() != y.a.size())
        return true;
    for (size_t i = 0; i < x.a.size(); ++i)
        if (x.a[i] != y.a[i])
            return true;
    return false;
}