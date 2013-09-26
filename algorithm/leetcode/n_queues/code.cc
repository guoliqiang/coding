// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 10:17:36
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 9/9 test cases passed.
 *
 * */

#include <vector>
#include <algorithm>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {

bool Check(std::vector<int> & num, int k) {
  for (int i = 0; i < k; i++) {
    if (num[i] == num[k]) return false;
    if (abs(num[i] - num[k]) == abs(i - k)) return false;
  }
  return true;
}

void Trace(std::vector<int> & num, int n,
           std::vector<std::vector<std::string> > * rs) {
  if (n >= num.size()) {
    std::vector<std::string> foo;
    for (int i = 0; i < num.size(); i++) {
      std::string bar;
      for (int j = 0; j < num.size(); j++) {
        if (j == num[i]) bar.push_back('Q');
        else bar.push_back('.');
      }
      foo.push_back(bar);
    }
    rs->push_back(foo);
  } else {
    for (int i = n; i < num.size(); i++) {
      std::swap(num[n], num[i]);
      if (Check(num, n)) Trace(num, n + 1, rs);
      std::swap(num[n], num[i]);
    }
  }
}

std::vector<std::vector<std::string> > NQueues(int n) {
  std::vector<int> num(n, 0);
  std::vector<std::vector<std::string> > rs;
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
  std::vector<std::vector<std::string> > rs = NQueues(8);
  Out(rs);
  LOG(INFO) << JoinMatrix(&rs);
  LOG(INFO) << rs.size();
  return 0;
}

