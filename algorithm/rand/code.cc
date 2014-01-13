// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-02 21:36:55
// File  : code.cc
// Brief :

/*
线性同余法
所谓随机是有seed的随机决定的，seed一般取time(NULL)
http://stackoverflow.com/questions/15038174/generate-random-numbers-without-using-any-external-functions
http://www.choudan.net/2013/07/26/%E9%9A%8F%E6%9C%BA%E6%95%B0%E7%9A%84%E7%AE%80%E5%8D%95%E7%94%9F%E6%88%90.html
*/
#include "base/public/common_ojhead.h"

namespace algorithm {
long long a = 25214903917;   // These Values for a and c are the actual values found
long long c = 11;            // in the implementation of java.util.Random(), see link
long long previous = 0;

void Rseed(long long seed) {
    previous = seed;
}

long long Rand() {
    long long r = a * previous + c;  // or % m
    // Note: typically, one chooses only a couple of bits of this value, see link
    previous = r;
    return r;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
