// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-20 20:02:07
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// 给一个array比如[4,2,1,3,5],根据这个array现在我们能有了一个新的array => 每数是在原array里,
// 在它左边的所有比它大的number的个数,就是[0,1,2,1,0]. 题目是现在给了这个[0,1,2,1,0]要求原array, 原来array的range是1~n
namespace algorithm {

std::vector<int> Generate(std::vector<int> & vec, int n) {
  std::vector<int> ans;
  for (int i = 0; i < n; i++) {
    ans.push_back(i + 1);
  }
  for (int i = n - 1; i >= 0; i--) {
    int t = ans[i - vec[i]];
    for (int j = i - vec[i]; j < i; j++) ans[j] = ans[j + 1];
    ans[i] = t;
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(0);
  vec.push_back(2);
  vec.push_back(0);
  LOG(INFO) << JoinVector(Generate(vec, 5));
  return 0;
}
