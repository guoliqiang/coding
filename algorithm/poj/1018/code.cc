// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-26 21:21:22
// File  : code.cc
// Brief :

// http://blog.163.com/xiangzaihui@126/blog/static/16695574920116131119191/

#include "base/public/common_ojhead.h"

namespace algorithm {
struct Node {
  int b;
  int p;
};

// 按价格从小到大排序
int cmp(const void* a , const void* b) {
  return (*(Node*)a).p > (*(Node*)b).p;
}

int Read() {
  Node a[105][105];
  int t, n, c[105];
  std::cin >> t;
  while (t--) {
    std::cin >> n ;
    int i, j, k, r, minb, sump;
    double bp = 0 ;
    for (i = 1; i <= n; i++ ) {
      std::cin >> c[i];
      for (j = 1; j <= c[i]; j++) {
        std::cin >> a[i][j].b >> a[i][j].p;
      }
      std::qsort(&a[i][1], c[i], sizeof(a[0][0]) ,cmp);
    }
    for (i = 1; i <= n; i++) {
      for (j = 1; j <= c[i]; j++) {
        minb = a[i][j].b;
        sump = a[i][j].p;
        for (k = 1; k <= n; k++) {
          if (k == i) continue;
          for (r = 1; r <= c[k]; r++) {
            if (minb <= a[k][r].b) break;
          }
          if (r > c[k]) break;
          sump += a[k][r].p ;
        }
        if (k <= n) continue;
        if (minb / double(sump) > bp) bp = minb / double(sump);
      }
    }
    printf( "%.3f\n" , bp ) ;
  }
  return 0;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
