// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 21:07:51
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 20 milli secs
 * Progress: 108/108 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void Plus(std::vector<int> & v, int i, int k) {
  if (k == 0) return;
  
  int sum = 0;
  if (i == -1) sum = k;
  else sum = v[i] + k;
  
  k = sum / 10;
  sum %= 10;
  if (i == -1) {
    v.insert(v.begin(), sum);
    Plus(v, i, k);
  } else {
    v[i] = sum;
    Plus(v, i - 1, k);
  }
}

std::vector<int> PlusOne(std::vector<int> & v) {
   std::vector<int> rs = v;
   Plus(rs, rs.size() - 1, 1);
   return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(8);
  foo.push_back(3);
  LOG(INFO) << JoinVector(PlusOne(foo));
  return 0;
}
