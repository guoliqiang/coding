// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 13:18:55
// File  : code.cc
// Brief :

// 回溯法其实就是DFS
/*
Numbers can be regarded as product of its factors. For example,
8 = 2 x 2 x 2;
  = 2 x 4.
Write a function that takes an integer n and return all possible combinations of its factors.

Note:
Each combination’s factors must be sorted ascending, for example: The factors of 2 and 6 is [2, 6], not [6, 2].
You may assume that n is always positive.
Factors should be greater than 1 and less than n.

Examples:

input: 1

output:

[]
input: 37

output:

[]
input: 12

output:

[
  [2, 6],
  [2, 2, 3],
  [3, 4]
]
input: 32

output:

[
  [2, 16],
  [2, 2, 8],
  [2, 2, 2, 4],
  [2, 2, 2, 2, 2],
  [2, 4, 4],
  [4, 8]
]
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
void Get(std::vector<int> & path, int v, std::vector<std::vector<int> > & ans) {
  if (v == 1) {
    ans.push_back(path);
    return;
  }

  for (int i = path.back(); i <= v; i++) {
    if (v % i == 0) {
      path.push_back(i);
      Get(path,  v / i, ans);
      path.pop_back();
    }
  }
}

std::vector<std::vector<int> > GetFactor(int n) {
  std::vector<std::vector<int> > ans;
  std::vector<int> path;
  for (int i = 2; i < n; i++) {
    if (n % i == 0) {
      path.push_back(i);
      Get(path, n / i, ans);
      path.pop_back();
    }
  }
  return ans;
}
}  // namespace algorithm

namespace twice {
void TraceBack(std::vector<int> & path, int idx, int cur, int n, std::vector<std::vector<int> > & ans) {
  if (cur == n) {
    ans.push_back(path);
  }
  if (cur >= n) return;

  for (int i = idx; i < n; i++) {
    if (cur * i <= n) {
      path.push_back(i);
      TraceBack(path, i, cur * i, n, ans);
      path.pop_back();
    }
  }
}

std::vector<std::vector<int> > GetFactor(int n) {
  std::vector<std::vector<int> > ans;
  std::vector<int> path;
  TraceBack(path, 2, 1, n, ans);
  return ans;
}
}  // namespace twice

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > vec;
  vec = twice::GetFactor(32);
  LOG(INFO) << JoinMatrix(&vec);
  LOG(INFO) << vec.size();
  return 0;
}
