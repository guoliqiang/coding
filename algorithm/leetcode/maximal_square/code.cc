// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 23:55:35
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

int MaximalSquare(std::vector<int> height) {
  height.push_back(0);
  std::stack<int> stack;
  int i = 0;
  int max = 0;
  while (i < height.size()) {
    if (stack.empty() || height[i] >= height[stack.top()]) {
      stack.push(i++);
    } else {
      int ti = stack.top();
      stack.pop();
      int width = stack.empty() ? i : i - stack.top() - 1;
      if (width >= height[ti]) {
        max = std::max(max, height[ti] * height[ti]);
      }
    }
  }
  return max;
}

int MaximalSquare(const std::vector<std::vector<char> > & v) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  int max = 0;
  std::vector<int> line(n, 0);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (v[i][j] == '0') line[j] = 0;
      else line[j] += 1;
    }
    max = std::max(max, MaximalSquare(line));
  }
  return max;
}

}  // namespace algorithm

namespace NB {
int MaximalSquare(const std::vector<std::vector<char> > & v) {
  int m = v.size();
  if (m == 0) return 0;

  int n = v[0].size();
  int max = 0;
  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  for (int i = 0; i < m; i++) {
    if (v[i][0] == '1') {
      dp[i][0] = 1;
      max = 1;
    }
  }
  for (int i = 0; i < n; i++) {
    if (v[0][i] == '1') {
      dp[0][i] = 1;
      max = 1;
    }
  }
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (v[i][j] == '1') {
        dp[i][j] = std::min(dp[i][j - 1],
                   std::min(dp[i - 1][j - 1], dp[i - 1][j])) + 1;
        max = std::max(max, dp[i][j]);
      }
    }
  }
  return max * max;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<char> > foo(4, std::vector<char>(5, '0'));
  foo[0][0] = '1'; foo[1][0] = '1'; foo[2][0] = '1'; foo[3][0] = '1';
  foo[2][1] = '1';
  foo[0][2] = '1'; foo[1][2] = '1'; foo[2][2] = '1';
  foo[1][3] = '1'; foo[2][3] = '1'; foo[3][3] = '1';
  foo[1][4] = '1'; foo[2][4] = '1';
  for (int i = 0; i < foo.size(); i++) {
    LOG(INFO) << JoinVector(foo[i]);
  }
  LOG(INFO) << MaximalSquare(foo);
  return 0;
}

