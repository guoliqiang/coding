// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 15:37:16
// File  : code.cc
// Brief :

/*
Given an array write a function to print all continuous subsequences in the array which sum of 0. 
e.g: 
Input: 
Array = [-1, -3, 4, 5, 4] 
output: 
-1, -3, 4
http://www.careercup.com/question?id=5172027535130624
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
// O(n^2)
void Dp(std::vector<int> & v) {
  int n = v.size();
  std::vector<std::vector<int> > dp(n, std::vector<int>(n, 0));
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      if (k == 0) dp[i][i + k] = v[i];
      else dp[i][i + k] = dp[i][i + k - 1] + v[i + k];
      if (dp[i][i + k] == 0) {
        LOG(INFO) << JoinVector(std::vector<int>(v.begin() + i, v.begin() + i + k + 1));
      }
    }
  }
}
}  // namespace algorithm

// O(n + m), worst O(n^2)
namespace NB {
void Dp(std::vector<int> & v) {
  std::map<int, std::vector<int> > tmap;
  tmap[0] = std::vector<int>(1, -1);
  int cur = 0;
  for (int i = 0; i < v.size(); i++) {
    cur += v[i];
    if (!tmap.count(cur)) tmap[cur] = std::vector<int>();
    tmap[cur].push_back(i);
  }
  for (std::map<int, std::vector<int> >::iterator i = tmap.begin(); i != tmap.end(); i++) {
    std::vector<int> & t = i->second;
    for (int i = 0; i < t.size(); i++) {
      for (int j = i + 1; j < t.size(); j++) {
        int b = std::max(0, t[i]);
        int e = t[j];
        LOG(INFO) << JoinVector(std::vector<int>(v.begin() + b, v.begin() + e + 1));
      }
    }
  }
}
}  // namespace NB
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(-1);
  v.push_back(-3);
  v.push_back(4);
  v.push_back(5);
  v.push_back(-2);
  v.push_back(-4);
  v.push_back(6);
  Dp(v);
  HERE(INFO);
  NB::Dp(v);
  return 0;
}
