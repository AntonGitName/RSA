#include <iostream>
#include <vector>

#include <cstdio>
#include "bigInt.h"

using namespace std;

int main()
{
    bigInt a, b, m;
    while (cin >> a >> b >> m)
        cout << power(a, b, m) << endl;
    return 0;
}

