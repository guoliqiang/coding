// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 10:17:36
// File  : code.cc
// Brief :

/*
Follow up for N-Queens problem.
Now, instead outputting board configurations, return the total number of distinct solutions.

*/
/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 1304 milli secs
 * Progress: 12/12 test cases passed.
 *
 * TODO
 * 扩展 左右上下 和 对角线 对称的算一个
 * 结果数 直接除 4
 * */

#include <vector>
#include <algorithm>
#include <iostream>
#include "base/public/logging.h"

// 普通的回溯方法会T
// 这个方法check的次数会少
namespace algorithm {

bool Check(std::vector<int> & num, int k) {
  for (int i = 0; i < k; i++) {
    if (num[i] == num[k]) return false;
    if (abs(num[i] - num[k]) == abs(i - k)) return false;
  }
  return true;
}

void Trace(std::vector<int> & num, int n,
           int * rs) {
  if (n >= num.size()) {
    (*rs)++;
  } else {
    for (int i = n; i < num.size(); i++) {
      std::swap(num[n], num[i]);
      if (Check(num, n)) Trace(num, n + 1, rs);
      std::swap(num[n], num[i]);
    }
  }
}

int NQueues(int n) {
  std::vector<int> num(n, 0);
  int rs = 0;
  for (int i = 0; i < n; i++) {
    num[i] = i;
  }
  Trace(num, 0, &rs);
  return rs;
}

void Out(std::vector<std::vector<std::string> > & v) {
  for (int i = 0; i< v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++) {
      std::cout << v[i][j] << std::endl;
    }
    std::cout << std::endl;
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  int rs = NQueues(8);
  LOG(INFO) << rs;
  return 0;
}

