// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-17 18:12:55
// File  : code.cc
// Brief :

/*
 * Accepted 5996K 47MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1299720;
int vec[MAXN];
std::vector<int> prime;

void Calculate() {
  memset(vec, 0, sizeof(vec));
  vec[0] = vec[1] = -1;
  for (int i = 2; i < MAXN; i++) {
    if (vec[i] == 0) {
      prime.push_back(i);
    }
    for (int j = 0; j < prime.size(); j++) {
      if (prime[j] * i < MAXN) {
        vec[prime[j] * i] = -1;
      } else break;
      if (i % prime[j] == 0) break;
    }
  }
}

int BSearch(int k) {
  int b = 0;
  int e = prime.size();
  while (b < e) {
    int mid = b + (e - b) / 2;
    if (prime[mid] < k) b = mid + 1;
    else e = mid;
  }
  return b;
}

void Read() {
  Calculate();
  int k = 0;
  while (scanf("%d", &k) && k) {
    int t = BSearch(k);
    if (t >= prime.size() || prime[t] == k) {
      printf("0\n");
    } else {
      printf("%d\n", prime[t] - prime[t - 1]);
    }
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
