#include <iostream>
#include <cstdio>

#include "bigInt.h"
#include "checkPrime.h"

using namespace std;

int main()
{
    bigInt a("359334085968622831041960188598043661065388726959079837"), b, m;
    //freopen("test.txt", "r", stdin);
    //freopen("primes.txt", "w", stdout);



    //a = 2147483647;
    //cout << a << " " << isPrime(a) << endl;
	
    for (bigInt a = 2; a < 10000; ++a)
        if (isPrime(a))
            cout << a << endl;
    
    return 0;
}

