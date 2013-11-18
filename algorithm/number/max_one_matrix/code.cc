// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-29 05:10:45
// File  : code.cc
// Brief :

/*
 *  poj3494
 * */

#include "base/public/common_head.h"

namespace algorithm {

int MaxVec(std::vector<int> v) {
  v.push_back(0);
  int rs = 0;
  std::stack<int> stack;
  for (int i = 0; i < v.size(); i++) {
    if (stack.empty() || stack.top() <= v[i]) {
      stack.push(i);
    } else {
      int t = v[stack.top()];
      stack.pop();
      int foo = stack.empty() ?  t * i : t * (i - stack.top() - 1);
      rs = std::max(rs, foo);
      i--;  // bug fixed
    }
  }
  return rs;
}

int MaxOneMatrix(std::vector<std::vector<int> > & v) {
  int m = v.size();
  if (m == 0) return 0;
  int n = v[0].size();
  int rs = 0;
  std::vector<int> dp(n, 0);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (v[i][j] == 0) dp[j] = 0;
      else dp[j]++;
    }
    rs = std::max(rs, MaxVec(dp));
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int m = 3;
  int n = 3;
  std::vector<std::vector<int> > v(m, std::vector<int>(n, 0));
  v[1][1] = 1;
  v[1][2] = 1;
  v[2][1] = 1;
  LOG(INFO) << MaxOneMatrix(v);
  return 0;
}
