// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 20:17:00
// File  : code.cc
// Brief :

/*
 *
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 300 milli secs
 * Progress: 30/30 test cases passed.
 * */

#include <vector>
#include <set>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {

void PermuteSub(std::vector<int> & num, int n, std::set<std::vector<int> > * rs) {
  if (n >= num.size()) {
    rs->insert(num);
  } else {
    // 不能剔除干净 如 [2 2 1 1]
    // 所以改用set保存结果了
    for (int i = n; i < num.size(); i++) {
      if(i != n && num[i] == num[n]) {
        continue;
      } else {
        std::swap(num[n], num[i]);
        PermuteSub(num, n + 1, rs);
        std::swap(num[n], num[i]);
      }
    }
  }
}

std::vector<std::vector<int> > Permute(std::vector<int> & num) {
  std::vector<std::vector<int> > rs;
  std::set<std::vector<int> > foo;
  PermuteSub(num, 0, &foo);
  for (std::set<std::vector<int> >::iterator i = foo.begin();
       i != foo.end(); i++) {
    rs.push_back(*i);
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(1);
  num.push_back(1);
  num.push_back(2);
  std::vector<std::vector<int> > foo = Permute(num);
  for (int i = 0; i < foo.size(); i++) {
    LOG(INFO) << JoinVector(foo[i]);
  }
  return 0;
}
