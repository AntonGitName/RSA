#ifndef _GENERATEPRIME_H_
#define _GENERATEPRIME_H_

#include <random>
#include <limits>
#include <time.h>

#include "checkPrime.h"
#include "bigInt.h"

bigInt getBigRandom(std::default_random_engine &generator, bigInt l, bigInt r);
bigInt getPrime();

#endif