// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 17:02:38
// File  : code.cc
// Brief :

// 背包问题的暴力搜索解法
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
// 遍历树的写法，左侧表示不选当前节点，右侧表示选择当前节点
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

namespace other {
// 很容易扩展到对数量有限制的情况, 不允许重复的情况
void TraceBack(std::vector<int> & vec, std::vector<int> & path, std::vector<std::vector<int> > & ans, int idx) {
    ans.push_back(path);

    for (int i = idx; i < vec.size(); i++) {
        path.push_back(vec[i]);
        TraceBack(vec, path, ans, i + 1);
        path.pop_back();
    }
}
}  // namespace other

using namespace algorithm;

namespace iter {
std::vector<std::vector<int > > subsets(std::vector<int>& nums) {
  std::sort(nums.begin(), nums.end());

  int n = nums.size();
  int size = pow(2, n);
  std::vector<std::vector<int> > ans;

  for (int i = 0; i < size; i++) {
      std::vector<int> cur;
      for (int j = 0; j < n; j++) {
          if (i & (1 << j)) cur.push_back(nums[j]);
      }
      ans.push_back(cur);
  }
  return ans;
}
}  // namespace iter

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
