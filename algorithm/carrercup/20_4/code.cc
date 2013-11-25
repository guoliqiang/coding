// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 20:45:42
// File  : code.cc
// Brief :

/*
 * Write a method to count the number of 2s between 0 and n.
 *
 * */

#include "base/public/logging.h"
#include <math.h>

namespace algorithm {

int Vec2Int(std::vector<int> & v) {
  int rs = 0;
  for (int i = 0; i < v.size(); i++) {
    rs = rs * 10 + v[i];
  }
  return rs;
}

int TwoNum(int n) {
  int rs = 0;
  std::vector<int> l;  // bug fixed: use vector to store the 000 of 1000
  while (n) {
    int t = n % 10;
    int h = n / 10;
    if (t > 2) {
      rs += (h + 1) * pow(10, l.size());
    } else if (t == 2) {
      rs += h * pow(10, l.size()) + Vec2Int(l) + 1;
    } else {
      rs += h * pow(10, l.size());
    }
    n = h;
    l.insert(l.begin(), t);
  }
  return rs;
}

int TwoNumFool(int n) {
  int rs = 0;
  for (int i = 1; i <= n; i++) {
    int t = i;
    while (t) {
      if (t % 10 == 2) rs++;
      t /= 10;
    }
  }
  return rs;
}

}  // namespace algorithm

namespace NB {
int Num(int n, int k) {
  int rs = 0;
  int factor = 1;
  while (n / factor != 0) {
    int h = n / (factor * 10);
    int l = n % factor;
    int t = (n / factor) % 10;
    if (t > k) {
      rs += (h + 1) * factor;
    } else if (t == k) {
      rs += h * factor + l + 1;
    } else {
      rs += h * factor;
    }
    factor *= 10;
  }
  return rs;
}
}  // namepsace NB

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 899;
  LOG(INFO) << TwoNum(n) << " " << TwoNumFool(n) << " " << NB::Num(n, 2);
  return 0;
}
