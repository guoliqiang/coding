// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 20:17:00
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 88 milli secs
 * Progress: 25/25 test cases passed.
 *
 * */

#include <vector>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {

void PermuteSub(std::vector<int> & num, int n, std::vector<std::vector<int> > * rs) {
  if (n >= num.size()) {
    rs->push_back(num);
  } else {
    for (int i = n; i < num.size(); i++) {
      std::swap(num[n], num[i]);
      PermuteSub(num, n + 1, rs);
      std::swap(num[n], num[i]);
    }
  }
}

std::vector<std::vector<int> > Permute(std::vector<int> & num) {
  std::vector<std::vector<int> > rs;
  PermuteSub(num, 0, &rs);
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(1);
  num.push_back(2);
  num.push_back(3);
  std::vector<std::vector<int> > foo = Permute(num);
  for (int i = 0; i < foo.size(); i++) {
    LOG(INFO) << JoinVector(foo[i]);
  }
  return 0;
}
