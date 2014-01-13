// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-05 16:35:18
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
/*
Given an array A of positive integers. Convert it to a sorted array with minimum cost. The only valid operation are: 
1) Decrement with cost = 1 
2) Delete an element completely from the array with cost = value of element

dp[i][j]:
前i个组成的有序数组中，最后一个数为s[j]的min cost
http://www.careercup.com/question?id=9333968
http://stackoverflow.com/questions/8928061/convert-array-to-a-sorted-one-using-only-two-operations
*/
int Dp(std::vector<int> & vec) {
  std::set<int> tset;
  for (int i = 0; i < vec.size(); i++) tset.insert(vec[i]);
  std::vector<int> s(tset.begin(), tset.end());
  int n = vec.size();
  int m = s.size();
  std::vector<std::vector<int> > dp(n ,std::vector<int>(m, 0));
  for (int i = 0; i < m; i++) {
    dp[0][i] = vec[0] - s[i] >= 0 ? vec[0] - s[i] : 0;
  }
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < m; j++) {
      dp[i][j] = INT_MAX;
      int t = vec[i] - s[j];
      if (t < 0) t = vec[i];
      for (int k = 0; k <= j; k++) {
        dp[i][j] = std::min(dp[i][j], dp[i - 1][k] + t);
      }
    }
  }
  int rs = INT_MAX;
  for (int i = 0; i < m; i++) rs = std::min(rs, dp[n - 1][i]);
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::vector<int> vec;
  // 4,3,5,6
  vec.push_back(4);
  vec.push_back(3);
  vec.push_back(5);
  vec.push_back(6);
  LOG(INFO) << Dp(vec);
  vec.clear();
  // 10,3,11,12
  vec.push_back(10);
  vec.push_back(3);
  vec.push_back(12);
  vec.push_back(12);
  LOG(INFO) << Dp(vec);
  return 0;
}
