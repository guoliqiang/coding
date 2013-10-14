// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 15:44:31
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 36/36 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 200/200 test cases passed.
 *
 *
 * 直观的方法生成直到底k个时返回
 * */

#include <vector>
#include "base/public/logging.h"

namespace algorithm {

int Factorial(int n) {
  int rs = 1;
  while (n) {
    rs *= n;
    n--;
  }
  return rs;
}

void GetKPermutation(std::vector<char> & v, int b, int k) {
  if (b >= v.size() || k <= 0) return;
  int size = v.size() - b;
  int tmp = Factorial(size - 1);
  
  int i  = k / tmp;
  char foo = v[b + i];
  for (int j = b + i; j > b; j--) v[j] = v[j - 1];
  v[b] = foo;
  
  k %= tmp;
  GetKPermutation(v, b + 1, k);
}

std::string GetPermutation(int n, int k) {
  std::vector<char> v;
  for (int i = 0; i < n; i++)  {
    v.push_back(i + 1 + '0');
  }
  // 程序中下标是从0开始的
  // 对于取模/除以 某个返回的需求 下表必须从0开始
  // Note here***
  GetKPermutation(v, 0, k - 1);
  std::string rs = "";
  for (int i = 0; i < v.size(); i++) {
    rs.append(&v[i], 1);
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << GetPermutation(1, 1);
  LOG(INFO) << GetPermutation(3, 2);
  LOG(INFO) << GetPermutation(3, 5);
  LOG(INFO) << GetPermutation(4, 5);
  LOG(INFO) << GetPermutation(2, 2);
  return 0;
}
