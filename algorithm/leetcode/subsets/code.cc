// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 17:02:38
// File  : code.cc
// Brief :

/*
Given a set of distinct integers, S, return all possible subsets.
Note:
Elements in a subset must be in non-descending order.
The solution set must not contain duplicate subsets.
For example,
If S = [1,2,3], a solution is:

[
  [3],
  [1],
  [2],
  [1,2,3],
  [1,3],
  [2,3],
  [1,2],
  []
]
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 6/6 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 48 milli secs
 * Progress: 10/10 test cases passed.
 *
 * */

#include "base/public/common_head.h"


namespace algorithm {

void SubsetTree(std::vector<int> & path, int i,
                std::vector<int> & v,
                std::vector<std::vector<int> > * rs) {
  if (i == path.size()) {
    std::vector<int> tmp;
    for (int j = 0; j < path.size(); j++) {
      if (path[j]) {
        tmp.push_back(v[j]);
      }
    }
    std::sort(tmp.begin(), tmp.end());
    rs->push_back(tmp);
  } else {
    for (int j = 0; j < 2; j++) {
      path[i] = j;
      SubsetTree(path, i + 1, v, rs);
    }
  }
}

std::vector<std::vector<int> > Subsets(std::vector<int> & v) {
  std::vector<int> path(v.size(), 0);
  std::vector<std::vector<int> > rs;
  SubsetTree(path, 0, v, &rs);
  return rs;
}

}  // namespace algorithm

namespace twice {
using namespace std;

void Trace(std::vector<int> & s, std::vector<int> & path,
           int k, std::vector<std::vector<int> > & rs) {
  if (k == s.size()) {
    rs.push_back(path);
  } else {
    Trace(s, path, k + 1, rs);
    path.push_back(s[k]);
    Trace(s, path, k + 1, rs);
    path.pop_back();
  }
}
class Solution {
 public:
  std::vector<std::vector<int> > subsets(std::vector<int> &S) {
    std::sort(S.begin(), S.end());
    std::vector<std::vector<int> > rs;
    std::vector<int> path;
    Trace(S, path, 0, rs);
    return rs;
  }
};
}  // namespace twice

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> bar;
  bar.push_back(1);
  bar.push_back(2);
  bar.push_back(3);

  std::vector<std::vector<int> > foo = Subsets(bar);
  LOG(INFO) << foo.size();
  LOG(INFO) << JoinMatrix(&foo);
  return 0;
}
