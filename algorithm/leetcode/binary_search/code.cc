// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-20 20:36:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

//知不知道binary search? 但是现在array是unsorted的可是依然看做sorted array来做binary search,
//返回在array里面所有可以这种情况下binary search出来的数, pivot is random
namespace algorithm {

std::vector<int> Get(std::vector<int> & vec) {
  int n = vec.size();
  if (n <= 1) return vec;

  std::vector<int> dp(n, 0);
  std::vector<int> dp2(n, 0);

  for (int i = 0; i < n; i++) {
    if (i == 0) dp[i] = vec[i];
    else dp[i] = std::max(dp[i - 1], vec[i]);
  }

  for (int j = n - 1; j > 0; j--) {
    if (j == n - 1) dp2[j] = vec[j];
    else dp2[j] = std::min(dp2[j + 1], vec[j]);
  }

  std::vector<int> ans;
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      if (dp2[i + 1] > vec[i]) ans.push_back(vec[i]);
    } else if (i == n - 1) {
      if (dp[i - 1] < vec[i]) ans.push_back(vec[i]);
    } else {
      if (dp[i - 1] < vec[i] && dp2[i + 1] > vec[i]) ans.push_back(vec[i]);
    }
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(4);
  vec.push_back(2);
  vec.push_back(1);
  vec.push_back(3);
  vec.push_back(5);

  LOG(INFO) << JoinVector(Get(vec));
  return 0;
}
