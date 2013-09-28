// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 14:41:23
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 28 milli secs
 * Progress: 12/12 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void GrayCode(int n, std::vector<int> & v) {
  if (n == 0) {
    v.push_back(0);
  } else if (n == 1) {
    v.push_back(0);
    v.push_back(1);
  } else {
    GrayCode(n - 1, v);
    int i = v.size() - 1;
    while (i >= 0) {
      v.push_back((v[i] | 1 << (n - 1)));
      i--;
    }
  }
}

std::vector<int> GrayCode(int n) {
  std::vector<int> rs;
  GrayCode(n, rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> rs = GrayCode(4);
  LOG(INFO) << JoinVector(rs);
  return 0;
}
