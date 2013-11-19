// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-18 17:58:25
// File  : code.cc
// Brief :

/*
 * Accepted  132K  125MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {

int ZeroNum(int k) {
  int rs = 0;
  while (k) {
    rs += k / 5;
    k /= 5;
  }
  return rs;
}

void Read() {
  int c;
  scanf("%d", &c);
  getchar();
  for (int i = 0; i < c; i++) {
    int n = 0;
    scanf("%d", &n);
    getchar();
    int t = ZeroNum(n);
    printf("%d\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
