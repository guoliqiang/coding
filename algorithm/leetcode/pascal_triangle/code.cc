// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 16:43:29
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 6/6 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 15/15 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

int Get(std::vector<int> & v, int i) {
  if (i == 0) return v[0];
  if (i == v.size()) return v[i - 1];
  return v[i - 1] + v[i];
}

std::vector<std::vector<int> > Generate(int num) {
  std::vector<std::vector<int> > rs;
  if (num < 1) return rs;
  rs.push_back(std::vector<int>(1, 1));
  for (int i = 2; i <= num; i++) {
    std::vector<int> foo;
    for (int j = 0; j < i; j++) {
      foo.push_back(Get(rs.back(), j));
    }
    rs.push_back(foo);
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > rs = Generate(5);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}
