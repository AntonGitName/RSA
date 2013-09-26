#include "bigInt.h"


const bigInt fastMul(const bigInt&, const bigInt&)
{

}




std::ostream& operator<< (std::ostream &out, const bigInt &x) {return out << x.toString();}

std::istream& operator>> (std::istream &in, bigInt &x) {std::string s; in >> s; x = bigInt(s); return in;}

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
    bigInt c;
    c.sign = a.sign ^ b.sign;
    c.a.assign(a.a.size() + b.a.size(), 0);
    for (size_t i = 0; i < a.a.size(); ++i)
        for (int j = 0, carry = 0; j < (int)b.a.size() || carry; ++j)
        {
            int cur = c.a[i+j] + a.a[i] * (j < (int)b.a.size() ? b.a[j] : 0) + carry;
            c.a[i+j] = int (cur % bigInt::base);
            carry = int (cur / bigInt::base);
        }
    c.deleteNulls();
    return c;
}

inline const bigInt operator/(const bigInt& x, const bigInt& y)
{
    if (x.absLess(y))
        return 0;
    bigInt c, l(1), r;
    r.a.assign((int)x.a.size()-(int)y.a.size() + 1, bigInt::base - 1);
    r.sign = false;
    while ((l + 1) < r)
    {
        c = (l + r) / 2;
        if (x.absLess(y * c))
            r = c - 1;
        else
            l = c;
    }
    c = (r * y <= x) ? r : l;
    c.sign = x.sign ^ y.sign;
    c.deleteNulls();
    return c;
}

const bigInt operator%(const bigInt& x, const bigInt& y)
{
    if (y == 1)
        return 0;
    if (y == 2)
        return x.a[0] & 1;
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
    if (y >= bigInt::base)
        return x / bigInt(y);
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