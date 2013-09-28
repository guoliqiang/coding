// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 16:17:26
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 41/41 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 72 milli secs
 * Progress: 134/134 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool SearchMatrix(std::vector<std::vector<int> > & v, int t) {
  int b = 0;
  int m = v.size();
  int n = (m == 0 ? 0 : v[0].size());

  int e = m * n;
  while (b < e) {
    int mid = b + (e - b) / 2;
    int i = mid / n;
    int j = mid % n;
    if (v[i][j] == t) return true;
    else {
      if (v[i][j] < t) b = mid + 1;
      else e = mid;
    }
  }
  return false;
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > v (1, std::vector<int> (4, 0));
  v[0][0] = 0;
  v[0][1] = 1;
  v[0][2] = 2;
  v[0][3] = 3;
  LOG(INFO) << SearchMatrix(v, 0);
  LOG(INFO) << SearchMatrix(v, 2);
  LOG(INFO) << SearchMatrix(v, 3);
  LOG(INFO) << SearchMatrix(v, -1);
  LOG(INFO) << SearchMatrix(v, 5);
  return 0;
}
