// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-09 15:08:15
// File  : code.cc
// Brief :

/*
Accepted  3108K 2141MS
如果内存不限制的话,利用dp的思想可以有 n * log(2 ^ n)的算法

因为需要存储2^(n-1)个中间结果，内存明显不够用
所以存储一半，复杂度为2^(n / 2) * log (2 ^ (n / 2))

可以根据能存多少算多少减少点儿时间复杂度

*/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 40;
int N = 0;
LL data[MAXN];
std::vector<std::pair<LL, LL> > vec;
LL rs_min;
LL rs_num;

int BSearch(LL k) {
  int b = 0;
  int e = vec.size() - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (vec[mid].first < k) b = mid + 1;
    else e = mid - 1;
  }
  return b;
}

void Solve() {
  vec.clear();
  int l = N / 2;
  int r = N - l;
  for (int i = 0; i < (1 << l); i++) {
    LL tmp = 0;
    LL num = 0;
    for (int j = 0; j < l; j++) {
      if (i & 1 << j) {
        tmp += data[j];
        num++;
      }
      if ((1 << j) >= i) break;
    }
    vec.push_back(std::make_pair(tmp, num));
  }

  std::sort(vec.begin(), vec.end());
  int k = 0;
  for (int i = 1; i < vec.size(); i++) {
    if (vec[i].first > vec[k].first ||
        (vec[k].second == 0 && vec[i].first == vec[k].first)) {
      vec[++k] = vec[i];
    }
  }
  k++;
  vec.erase(vec.begin() + k, vec.end());
  
  rs_min = 0;
  rs_num = -1;
  for (int i = 0; i < (1 << r); i++) {
    LL tmp = 0;
    LL num = 0;
    for (int j = 0; j < r; j++) {
      if (i & (1 << j)) {
        tmp += data[l + j];
        num++;
      }
      if ((1 << j) >= i) break;
    }
    int m = BSearch(-tmp);
    for (int j = std::max(0, m - 1); j <= std::min((int)vec.size() - 1, m + 1); j++) {
      if (vec[j].second + num > 0) {
        if (rs_num == -1 || rs_min > LLabs(tmp + vec[j].first) ||
            (rs_min == LLabs(tmp + vec[j].first) && rs_num > vec[j].second + num)) {
          rs_min = LLabs(tmp + vec[j].first);
          rs_num = num + vec[j].second;
        }
      }
    }
  }
}


void Read() {
  LL n = 0;
  while (scanf("%lld", &n) != EOF && n != 0) {
    getchar();
    N = n;
    for (int i = 0; i < n; i++) {
      LL t = 0;
      scanf("%lld", &t);
      data[i] = t;
    }
    getchar();
    Solve();
    printf("%lld %lld\n", rs_min, rs_num);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
