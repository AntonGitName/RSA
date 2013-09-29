#include <iostream>
#include <cstdio>

#include "bigInt.h"
#include "checkPrime.h"
#include "generatePrime.h"


using namespace std;

int main()
{
    bigInt a("359334085968622831041960188598043661065388726959079837"), b, m;
    //freopen("test.txt", "r", stdin);
    //freopen("primes.txt", "w", stdout);


    //a = 1000000007;
    //a = 2147483647;
    //
    //while (cin >> a >> b)
    //{
    //    //cout << "a+b=" << a+b << endl;
    //    //cout << "a-b=" << a-b << endl;
    //    cout << "a*b=" << (m = a*b) << endl;
    //    //cout << "a/b=" << a/b << endl;
    //    //cout << "a%b=" << a%b << endl;
    //    //cout << "a^b=" << power(a,b) << endl;
    //}

    a = getPrime();
    cout << a << " " << isPrime(a) << endl;

    
    /*for (bigInt a = 2; a < 10000; ++a)
        if (isPrime(a))
            cout << a << endl;*/
    

    //a = 1024*(1024 + 512 + 128);
    //vector<bool> v = getBits(a);
    //for (int i = 0; i < v.size(); ++i)
    //    cout << v[i];

    return 0;
}

