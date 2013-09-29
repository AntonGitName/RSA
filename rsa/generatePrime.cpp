#include "generatePrime.h"
#include <iostream>
const int firstPrime = 1e9+7;
const bigInt MAXNUM = power(2, 130);

bigInt getBigRandom(std::default_random_engine &generator, bigInt l, bigInt r)
{
    if ((r - l) > std::numeric_limits<int>::max())
    {
        std::uniform_int_distribution<int> distribution(0, std::numeric_limits<int>::max());
        bigInt part = (r - l) / std::numeric_limits<int>::max();
        bigInt newL = l + part * distribution(generator);
        bigInt newR = newL + part;
        return getBigRandom(generator, newL , newR);
    }
    int dist = (r - l).toInt();
    std::uniform_int_distribution<int> distribution(0, dist);
    return l + distribution(generator);
}

bigInt getPrime()
{
    bigInt N = firstPrime;
    bigInt S = 2;
    bigInt R;

    std::default_random_engine generator( time(NULL) );

    while (N * N < MAXNUM)
    { 
        S = N;
        //std::cout << S << std::endl;
        int i = 0;
        do
        {
            R = getBigRandom(generator, S, 4 * S + 3);
            N = S * R + 1;
            //std::cout << R << std::endl;
            ++i;
        } while (!isPrime(N));
        std::cout << i << std::endl;
    }
    return N;
}
