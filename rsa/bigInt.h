#ifndef _BIGINT_H_
#define _BIGINT_H_

#include <vector>
#include <string>
#include <fstream>
#include <complex>
#include <algorithm>

typedef std::complex<long double> complex_double;

const double PI = 3.1415926535897932384626433832795028841971;

class bigInt
{
private:
    std::vector<long long> a;
    bool sign;
    static const int base = 10000 * 1000;

    bool bigInt::absLess(const bigInt & x) const
    {
        if (a.size() < x.a.size())
            return true;
        if (a.size() > x.a.size())
            return false;
        for (int i = (int)a.size() - 1; i >= 0; --i)
            if (a[i] != x.a[i])
                return a[i] < x.a[i];
        return false;
    }
    void deleteNulls() {while (a.size() >= 1 && !a.back()) a.pop_back();}

public:
    bigInt(size_t x) : sign(false) {while (x) a.push_back(x % base), x /= base;}
    bigInt(int x) : sign(x < 0) {x = (x > 0)?x:-x;while(x) a.push_back(x % base), x /= base;}
    bigInt(long long x = 0) : sign(x < 0) {x = (x > 0)?x:-x;while(x) a.push_back(x % base), x /= base;}
    bigInt(std::string s) {
        bool sign1 = false;
        *this = 0LL;
        if (!s.empty())
            sign1 = s[0] == '-';
        for (size_t i = int(sign1); i < s.length(); ++i)
            *this = 10LL * *this + long long(s[i] - '0');
        sign = sign1;
    }
    
    std::string toString() const;
    long long toInt() const {long long res = 0; for (int i=int(a.size())-1;i>=0;--i) res = res * bigInt::base + a[i]; return sign?-res:res;}
    unsigned long long toUInt() const {unsigned long long res = 0; for (int i=int(a.size())-1;i>=0;--i) res = res * bigInt::base + a[i]; return res;}
    size_t size() const {return a.size();}

    void changeSign() {sign ^= true;}
    const bigInt operator+() const {return *this;}
    const bigInt operator-() const {bigInt x(*this); x.sign ^= true; return x;}
    bigInt& operator=(const bigInt & x) { a = x.a; sign = x.sign; return *this;}
    bigInt& operator+=(const bigInt & x) { *this = *this + x; return *this;}
    bigInt& operator-=(const bigInt & x) { *this = *this - x; return *this;}
    bigInt& operator*=(const bigInt & x) { *this = *this * x; return *this;}
    bigInt& operator/=(const bigInt & x) { *this = *this / x; return *this;}
    bigInt& operator++() { *this = *this + bigInt(1LL); return *this;}
    const bigInt operator++(int) { bigInt old(*this); *this = *this + bigInt(1LL); return old;}
    bigInt& operator--() { *this = *this - bigInt(1LL); return *this;}
    const bigInt operator--(int) { bigInt old(*this); *this = *this - bigInt(1LL); return old;}
    //operator bool () {return *this != bigInt(0);}

    friend const bigInt power(bigInt, bigInt, const bigInt&);

    friend inline const bigInt operator+(const bigInt&, const bigInt&);
    friend inline const bigInt operator-(const bigInt&, const bigInt&);
    
    friend inline const bigInt operator*(const bigInt&, const bigInt&);
    friend inline const bigInt operator/(const bigInt&, const long long&);
    friend inline const bigInt operator/(const bigInt&, const bigInt&);
    friend const bigInt operator%(const bigInt&, const bigInt&);
    friend inline const bool operator<(const bigInt&, const bigInt&);
    friend const bool operator>(const bigInt&, const bigInt&);
    friend inline const bool operator==(const bigInt&, const bigInt&);
    friend const bool operator!=(const bigInt&, const bigInt&);
    friend inline const bool operator<=(const bigInt&, const bigInt&);
    friend const bool operator>=(const bigInt&, const bigInt&);

    friend const bigInt fastMul(const bigInt&, const bigInt&);
    friend const std::vector<bool> getBits(const bigInt&);

};

std::ostream& operator<< (std::ostream &out, const bigInt &x);
std::istream& operator>> (std::istream &in, bigInt &x);

const bigInt power(bigInt a, int b);
const bigInt power(bigInt a, bigInt b);
const bigInt power(bigInt a, bigInt b, const bigInt& m);

#endif