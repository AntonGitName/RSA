#include "checkPrime.h"

static const size_t smallPrimesNum = 168;
static const int smallPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
static const int MAX_POWER_2 = 1024;

bool checkSmall(const bigInt& a)
{
    for (int i = 0; i < smallPrimesNum; ++i)
        if (a == smallPrimes[i])
            return true;
        else if (a % smallPrimes[i] == 0)
            return false;
}

const bigInt abs(const bigInt &a)
{
    return (a < 0) ? (-a) : a;
}

inline bigInt gcd1(const bigInt& a, const bigInt& b)
{
    if (a == 0)
        return b;
    return gcd1(b % a, a);
}

inline bigInt gcd(const bigInt& a, const bigInt& b)
{
    return gcd1(abs(a), abs(b));
}

//! Переводит число n в форму q*2^p
inline void transform_num (bigInt n, bigInt & p, bigInt & q)
{
    /*int l = 0;
    int r = MAX_POWER_2;
    int m = (l + r) / 2;
    bigInt power_2 = power(2, m);
    bigInt t;
    
    p = n / power_2;
    t = p * power_2;

    while (l < r)
    {
        if (n == t)
            l = m;
        else
            r = m - 1;
        m = (l + r + 1) / 2;
        power_2 = power(2, m);
        p = n / power_2;
        t = p * power_2;
    }
    q = l;*/
    q = 0;
    while (n % 2 == 0)
    {
        ++q;
        n /= 2;
    }
    p = n;
}

bool miller_rabin (bigInt n, bigInt b)
{

    // сначала проверяем тривиальные случаи
    if (n == 2)
        return true;
    if (n < 2 || (n % 2 == 0))
        return false;

    // проверяем, что n и b взаимно просты (иначе это приведет к ошибке)
    // если они не взаимно просты, то либо n не просто, либо нужно увеличить b
    if (b < 2)
        b = 2;
    for (bigInt g; (g = gcd (n, b)) != 1; ++b)
        if (n > g)
            return false;

    // разлагаем n-1 = q*2^p
    bigInt n_1 = n;
    --n_1;
    bigInt p, q;
    transform_num (n_1, q, p);

    // вычисляем b^q mod n, если оно равно 1 или n-1, то n простое (или псевдопростое)
    bigInt rem = power (b, q, n);
    if (rem == 1 || rem == n_1)
        return true;

    // теперь вычисляем b^2q, b^4q, ... , b^((n-1)/2)
    // если какое-либо из них равно n-1, то n простое (или псевдопростое)
    for (bigInt i = 1; i < p; i++)
    {
        rem = (rem * rem) % n;
        if (rem == n_1)
            return true;
    }

    return false;

}

inline bool perfect_square(const bigInt &a)
{
    bigInt l = 1;
    bigInt r = a;
    bigInt m;
    while (l < r)
    {
        m = (l + r) / 2;
        if (a > m * m)
            r = m;
        else
            l = m + 1;
    }
    return (a == l * l); 
}

inline int jacobi(bigInt a, bigInt b)
{
    int r = 1;
    if (gcd(a, b) != 1)
        return 0;
    if (a < 0)
    {
        a.changeSign();
        if (b % 4 == 3) r = -r;
    }
    while (a != 0)
    {
        int t = 0;
        while (a % 2 == 0)
        {
            ++t;
            a /= 2;
        }
        if (t & 1)
        {
            int x = (int)(b % 8).toInt();
            if (x == 3 || x == 5) r = -r;
        }
        if( ((a % 4) == 3) && ((b % 4) == 3)) r = -r;
    
        bigInt c = a;
        a = b % c;
        b = c;
    }
    return r;
}

inline unsigned bits_in_number(bigInt a) // for positive only
{
    unsigned bits_count = 0;
    while (a > 0)
    {
        bits_count ++;//= (a % 2).toInt();
        a /= 2;
    }
    return bits_count;
}

inline int test_bit(bigInt a, int k)
{
    while (k--)
        a /= 2;
    return (int)(a % 2).toInt();
}

bigInt getDD(const bigInt &n) // original
{
    bigInt dd = 5;
    for (bool positiveSign = true;;positiveSign ^= true)
    {
        bigInt g;
        if (positiveSign)
        {
            g = gcd (n, dd);
        }
        else
        {
            dd.changeSign();
            g = gcd (n, dd);
            dd.changeSign();
        }
        if (1 < g && g < n)
            // нашли делитель - d_abs
            return false;
        if (jacobi (dd, n) == -1)
            break;
        dd += (positiveSign ? 2 : -2);
        dd.changeSign();
    }
    return dd;
}

int getDD1(const bigInt &n) // changed
{
    int dd;
	for (int d_abs = 5, d_sign = 1; ; d_sign = -d_sign, ++++d_abs)
	{
		dd = d_abs * d_sign;
		bigInt g = gcd (n, d_abs);
		if (1 < g && g < n)
			// нашли делитель - d_abs
			return false;
		if (jacobi (bigInt(dd), n) == -1)
			break;
	}
    return dd;
}

bool lucas_selfridge (const bigInt & n)
{
    // сначала проверяем тривиальные случаи
    if (n == 2)
        return true;
    if (n < 2 || (n % 2 == 0))
        return false;
    // проверяем, что n не является точным квадратом, иначе алгоритм даст ошибку
    if (perfect_square (n))
        return false;

    // алгоритм Селфриджа: находим первое число d такое, что:
    // jacobi(d,n)=-1 и оно принадлежит ряду { 5,-7,9,-11,13,... }
    
    int dd = getDD1(n);

    // параметры Селфриджа
    bigInt p = 1;
    int q = (1 - dd) / 4;
    
    // разлагаем n+1 = d*2^s
    bigInt n_1 = n + 1;
    bigInt s, d;
    transform_num (n_1, d, s);

    // алгоритм Лукаса
    bigInt
        u = 1,
        v = p,
        u2m = 1,
        v2m = p,
        qm = q,
        qm2 = q*2,
        qkd = q;
    std::vector<bool> bits = getBits(d);
    for (unsigned bit = 1, bitM = bits.size(); bit < bitM; ++bit)////for (unsigned bit = 1, bits = bits_in_number(d); bit < bits; bit++)//
    {
        u2m = (u2m * v2m) % n; // mulmod (u2m, v2m, n);
        v2m = (v2m * v2m) % n; // mulmod (v2m, v2m, n);
        while (v2m < qm2)
            v2m += n;
        v2m -= qm2;
        qm = (qm * qm) % n; // mulmod (qm, qm, n);
        qm2 = qm;
        qm2 *= 2;
        if (test_bit (d, bit))//if(bits[bit]) //
        {
            bigInt t1, t2;
            t1 = u2m;
            t1 = (t1 * v) % n; // mulmod (t1, v, n);
            t2 = v2m;
            t2 = (t2 * u) % n; // mulmod (t2, u, n);
            
            bigInt t3, t4;
            t3 = v2m;
            t3 = (t3 * v) % n; // mulmod (t3, v, n);
            t4 = u2m;
            t4 = (t4 * u) % n; // mulmod (t4, u, n);
            t4 = (t4 * dd) % n; // mulmod (t4, dd, n);

            u = t1 + t2;
            if (u % 2 == 1)
                u += n;
            u /= 2;
            u = u % n;

            v = t3 + t4;
            if (v % 2 == 1)
                v += n;
            v /= 2;
            v = v % n;
            bigInt temp = (qkd * qm);
            bigInt ttttt = temp % n;
            qkd = (qkd * qm) % n; // mulmod (qkd, qm, n);
        }
    }

    // точно простое (или псевдо-простое)
    if (u == 0 || v == 0)
        return true;

    // довычисляем оставшиеся члены
    bigInt qkd2 = qkd;
    qkd2 *= 2;
    for (bigInt r = 1; r < s; ++r)
    {
        v = (v * v) % n; // mulmod (v, v, n);
        v -= qkd2;
        if (v < 0) v += n;
        if (v < 0) v += n;
        if (v >= n) v -= n;
        if (v >= n) v -= n;
        if (v == 0)
            return true;
        if (r < s-1)
        {
            qkd = (qkd * qkd) % n; // mulmod (qkd, qkd, n);
            qkd2 = qkd;
            qkd2 *= 2;
        }
    }

    return false;

}

bool isPrime(const bigInt& a)
{
    if (!checkSmall(a))
        return false;
    if (!miller_rabin(a, 2))
        return false;
    return lucas_selfridge(a);
}