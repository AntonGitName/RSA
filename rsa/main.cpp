#include <iostream>
#include <vector>

#include <cstdio>
#include "bigInt.h"

using namespace std;

int main()
{
    bigInt a, b, m;
    //while (cin >> a >> b >> m)
    //    cout << power(a, b, m) << endl;
	//double x;
	//while (cin >> x)
	//	cout << "up: " << int(x + .5) << "\ndown: " << int(x - .5) << endl;
	freopen("test.txt", "r", stdin);
	freopen("MY_answer.txt", "w", stdout);
	while (cin >> a >> b)
        cout << fastMul(a, b) << endl;
    return 0;
}

