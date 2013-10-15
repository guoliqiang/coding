// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 17:02:38
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 10/10 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 64 milli secs
 * Progress: 26/26 test cases passed.
 *
 * */

#include "base/public/common_head.h"


namespace algorithm {

int OneNum(std::vector<int> & path, int i) {
  if (i < 0) return 0;
  int rs = 0;
  for(int j = 0; j <= i; j++) {
    if (path[j]) rs++;
  }
  return rs;
}

void SubsetTree(std::vector<int> & path, int i, int k,
                std::vector<std::vector<int> > * rs) {
  if (i == path.size()) {
    std::vector<int> tmp;
    for (int j = 0; j < path.size(); j++) {
      if (path[j]) tmp.push_back(j + 1);
    }
    rs->push_back(tmp);
  } else {
    int ones = OneNum(path, i - 1);
    if (ones == k) {
      path[i] = 0;
      SubsetTree(path, i + 1, k, rs);
    } else {
      if (path.size() - i + ones == k) {
        path[i] = 1;
        SubsetTree(path, i + 1, k, rs);
      } else {
        for (int j = 0; j < 2; j++) {
          path[i] = j;
          SubsetTree(path, i + 1, k, rs);
        }
      }
    }
  }
}

std::vector<std::vector<int> > Combine(int n, int k) {
  std::vector<int> path(n, 0);
  std::vector<std::vector<int> > rs;
  if (n == 0 || k == 0 || k > n) return rs;
  SubsetTree(path, 0, k, &rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
using namespace std;

void Trace(std::vector<int> & path, int n, int k, int cur, std::vector<std::vector<int> > & rs) {
  if (cur == n) {
      if (path.size() == k) rs.push_back(path);
  } else {
      Trace(path, n, k, cur + 1, rs);
      if (path.size() < k) {
          path.push_back(cur + 1);
          Trace(path, n, k, cur + 1, rs);
          path.pop_back();
      }
  }
}
class Solution {
public:
    vector<vector<int> > combine(int n, int k) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        std::vector<int> path;
        std::vector<std::vector<int> > rs;
        Trace(path, n, k, 0, rs);
        return rs;
    }
};
}  // namespace twice

int main(int argc, char** argv) {
  std::vector<std::vector<int> > foo = Combine(4, 4);
  LOG(INFO) << foo.size();
  LOG(INFO) << JoinMatrix(&foo);
  return 0;
}
