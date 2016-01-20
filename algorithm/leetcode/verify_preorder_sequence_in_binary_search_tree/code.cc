// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 11:54:58
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool Verify(std::vector<int> & vec, int b, int e) {
  if (b >= e) return true;

  int idx = b + 1;
  while (idx <= e && vec[idx] < vec[b]) idx++;
  for (int i = idx; i <= e; i++) {
    if (vec[i] <= vec[b]) return false;
  }
  return Verify(vec, b + 1, idx - 1) && Verify(vec, idx, e);
}

bool Verify(std::vector<int> & vec) {
  int n = vec.size();
  return Verify(vec, 0, n - 1);
}

}  // namespace algorithm

namespace NB {
// https://leetcode.com/discuss/52060/72ms-c-solution-using-one-stack-o-n-time-and-space
bool Verify(std::vector<int> & vec) {
  std::stack<int> stack;
  int pre = INT_MIN;

  for (int i = 0; i < vec.size(); i++) {
    if (stack.empty() || vec[i] < stack.top()) {
      if (vec[i] < pre) return false;
      stack.push(vec[i]);
    } else {
      while (!stack.empty() && stack.top() < vec[i]) {
        pre = stack.top();
        stack.pop();
      }
      stack.push(vec[i]);
    }
  }
  return true;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(1);
  LOG(INFO) << Verify(vec);
  return 0;
}
