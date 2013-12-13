// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-12 14:45:25
// File  : code.cc
// Brief :

// Accepted 4768K 1000MS

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1000005;
int flag[MAXN];
std::vector<int> prime;

void Prime() {
  memset(flag, -1, sizeof(flag));
  flag[0] = 0;
  flag[1] = 0;
  for (int i = 2; i <= MAXN; i++) {
    if (flag[i] == -1) prime.push_back(i);
    for (int j = 0; j < prime.size(); j++) {
      if (prime[j] * i <= MAXN) flag[prime[j] * i] = 0;
      else break;
      if (i % prime[j] == 0) break;
    }
  }
}

// 单位为2，3都可以AC，为4时WA猜测是越界了
void Mod(char * ptr, int k) {
  for (int i = 0; i < prime.size(); i++) {
    if (prime[i] >= k) break;
    int len = strlen(ptr);
    int rs = 0;
    for (int j = 0; j < len; j += 3) {
      if (j + 1 == len)
        rs = (rs * 10 + ptr[j] - '0') % prime[i];
      else if (j + 2 == len)
        rs = (rs * 100 + (ptr[j] - '0') * 10 + (ptr[j + 1] - '0')) % prime[i];
      else
        rs = (rs * 1000 + (ptr[j] - '0') * 100 + (ptr[j + 1] - '0') * 10 + (ptr[j + 2] - '0')) % prime[i];
    }
    if (rs == 0) {
      printf("BAD %d\n", prime[i]);
      return;
    }
  }
  printf("GOOD\n");
}

void Read() {
  Prime();
  char data[200];
  int k;
  while (scanf("%s%d", data, &k) != EOF) {
    if (k == 0 && strlen(data) == 1 && data[0] == '0') break;
    Mod(data, k);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
