#include <iostream>
#include <cstdio>

#include "bigInt.h"
#include "checkPrime.h"

using namespace std;


<<<<<<< HEAD
int main()
{
    bigInt a("359334085968622831041960188598043661065388726959079837"), b, m;
    //while (cin >> a >> b)
    //    cout << a / b << endl<< a % b << endl;

    //freopen("test.txt", "r", stdin);
    freopen("primes.txt", "w", stdout);
=======

int main()
{
    bigInt a("359334085968622831041960188598043661065388726959079837"), b, m;
    //while (cin >> a >> b >> m)
    //    cout << power(a, b, m) << endl;

    //freopen("test.txt", "r", stdin);
    //freopen("primes.txt", "w", stdout);
>>>>>>> master
    //for (a = 3; a < 100; ++a)
    //    cout << a << " " << isPrime(a) << " " << prime(a.toInt()) << endl;

    //a = 2147483647;
<<<<<<< HEAD
    //cout << a << " " << isPrime(a) << endl;
    for (bigInt a = 2; a < 10000; ++a)
        if (isPrime(a))
            cout << a << endl;
    //cout << isPrime(a);
=======
    cout << a << " " << isPrime(a) << endl;

    /*for (bigInt a = 2; a < 10000; ++a)
        if (isPrime(a))
            cout << a << endl;*/

>>>>>>> master
    return 0;
}

