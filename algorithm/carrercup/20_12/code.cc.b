// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 16:12:47
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
int MaxVec(std::vector<int> & v) {
  for (int i = 0; i < v.size(); i++) {
    if (v[i] == -1) return -1;
  }
  std::stack<int> stack;
  int max = -1;
  for (int i = 0; i < v.size(); i++) {
    if (stack.empty()) stack.push(i);
    else {
      if (v[i] >= stack.top()) stack.push(i);
      else {
        int t = 0;
        while (!stack.empty() && stack.top() > v[i]) {
          t = stack.top();
          stack.pop();
        }
        int tmax = stack.empty() ? v[t] * (i - 1) : v[t] * (i - stack.top() - 1);
        max = std::max(max, tmax);
        stack.push(i);
      }
    }
  }

  if (!stack.empty()) {
    int t = 0;
    while (!stack.empty()) {
      t = stack.top();
      stack.pop();
    }
    tmax = v[t] * v.size();
    max = std::max(max, tmax);
  }
  return max;
}

int MaxSub(std::vector<int> & v) {
  int b = 0;
  int e = v.size() - 1;
  while (b < v.size() && v[b] != 0) b++;
  while (e >= 0 && v[e] != 0) e--;
  if (b >= e) return -1;
  int t = b + 1;
  int max = -1;
  while (t <= e) {
    if (v[t] == 0) {
      std::vector<int> tv(v.begin() + b, v.begin() + t);
      max = std::max(max, MaxMatrix(tv));
      b = t;
    }
  }
  return max;
}

int MaxSubSquare(std::vector<std::vector<int> > & matrix) {
  int m = matrix.size();
  int n = (m == 0) ? 0 : matrix[0].size();
  for (int i = m - 1; i >= 1; i--) {
    std::vector<int> foo;
    for (int j = 0; j < n; j++) {
      if (matrix[i + 1][j] == '0')
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
