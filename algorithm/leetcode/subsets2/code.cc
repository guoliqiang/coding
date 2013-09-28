// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 15:40:16
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 92 milli secs
 * Progress: 19/19 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

void Subsets(std::vector<int> & s, std::vector<int> & v, int n,
             std::set<std::vector<int> > &rs) {
  if (n == s.size()) {
    std::vector<int> tmp;
    for (int i = 0; i < v.size(); i++) {
      if (v[i] == 1) tmp.push_back(s[i]);
    }
    std::sort(tmp.begin(), tmp.end());
    rs.insert(tmp);
  } else {
    for(int i = 0; i < 2; i++) {
      v[n] = i;
      Subsets(s, v, n + 1, rs);
    }
  }
}

std::vector<std::vector<int> > SubsetWithDup(std::vector<int> & s) {
  std::vector<int> v(s.size(), 0);
  std::set<std::vector<int> > sets;
  Subsets(s, v, 0, sets);
  std::vector<std::vector<int> > rs(sets.begin(), sets.end());
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(2);
  std::vector<std::vector<int> > rs = SubsetWithDup(foo);

  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}
