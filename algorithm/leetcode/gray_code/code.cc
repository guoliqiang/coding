// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 14:41:23
// File  : code.cc
// Brief :

/*
The gray code is a binary numeral system where two successive values differ in only one bit.
Given a non-negative integer n representing the total number of bits in the code, print the sequence of gray code. A gray code sequence must begin with 0.
For example, given n = 2, return [0,1,3,2]. Its gray code sequence is:

00 - 0
01 - 1
11 - 3
10 - 2
Note:
For a given n, a gray code sequence is not uniquely defined.
For example, [0,2,3,1] is also a valid gray code sequence according to the above definition.
For now, the judge is able to judge based on one instance of gray code sequence. Sorry about that.
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 28 milli secs
 * Progress: 12/12 test cases passed.
 *
 * */

// 当初是如何想到的
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


namespace twice {
// 得到的结果不全
void GrayCode(int n, std::vector<int> & v) {
  v.push_back(0);
  for (int i = 0; i < n; i++) {
    int t = v.back();
    v.push_back((t + 1) | t);
  }
  for (int i = 0; i < n; i++) {
    int t = v.back();
    v.push_back((t - 1) & t);
  }
}
std::vector<int> GrayCode(int n) {
  std::vector<int> rs;
  GrayCode(n, rs);
  return rs;
}
}  // namespace twice

namespace third {

std::vector<int> GrayCode(int n) {
  std::vector<int> rs;
  if (n > 0) {
    rs.push_back(0);
    rs.push_back(1);
    for (int i = 1; i < n; i++) {
      int k = rs.size() - 1;
      while (k >= 0) {
        rs.push_back(rs[k] | 1 << i);
        k--;
      }
    }
  }
  return rs;
}
}  // namespace third


using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> rs = GrayCode(4);
  LOG(INFO) << JoinVector(rs);
  rs = third::GrayCode(4);
  LOG(INFO) << JoinVector(rs);
  return 0;
}
