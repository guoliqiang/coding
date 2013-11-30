// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 23:55:35
// File  : code.cc
// Brief :

/*
Given a 2D binary matrix filled with 0's and 1's, find the largest rectangle containing all ones and return its area.
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 47/47 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 76 milli secs
 * Progress: 57/57 test cases passed.
 *
 * 难题，在LargestRetangleArea上扩展到了2维上
 * 同理可以扩展到n维上
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

int LargestRetangleArea(const std::vector<int> &height) {
  std::stack<int> stack;
  int i = 0;
  int max = 0;
  while (i < height.size()) {
    if (stack.empty() || height[i] >= height[stack.top()]) {
      stack.push(i++);
    } else {
      while (height[stack.top()] > height[i]) {
        int ti = stack.top();
        stack.pop();
        int tmax = stack.empty() ? height[ti] * i : height[ti] * (i - stack.top() - 1);
        max = std::max(max, tmax);
        if (stack.empty()) break;
      }
      stack.push(i++);
    }
  }
  while (!stack.empty()) {
    int ti = stack.top();
    stack.pop();
    int tmax = stack.empty() ? height[ti] * i : height[ti] * (i - stack.top() - 1);
    max = std::max(max, tmax);
  }
  return max;
}

int MaximalRectangle(const std::vector<std::vector<char> > & v) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  int max = 0;
  std::vector<int> line(n, 0);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (v[i][j] == '0') line[j] = 0;
      else line[j] += 1;
    }
    max = std::max(max, LargestRetangleArea(line));
  }
  return max;
}

}  // namespace algorithm

namespace twice {

int Largest(std::vector<int> height) {
  height.push_back(0);
  int max = 0;
  std::stack<int> stack;
  int i = 0;
  while (i < height.size()) {
    if (stack.empty() || height[i] >= height[stack.top()]) {
      stack.push(i);
      i++;
    } else {
      int t = stack.top();
      stack.pop();
      int tmax = stack.empty() ? height[t] * i : height[t] * (i - stack.top() - 1);
      max = std::max(max, tmax);
    }
  }
  return max;
}

class Solution {
 public:
  int maximalRectangle(std::vector<std::vector<char> > &matrix) {
    int m = matrix.size();
    if (m == 0) return 0;
    int n = matrix[0].size();
        
    int max = 0;
    std::vector<int> lines(n, 0);
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] == '1') lines[j]++;
        else lines[j] = 0;
      }
    max = std::max(max, Largest(lines));
  }
  return max;
}

};
}  // namespace twice
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<char> > foo(4, std::vector<char>(4, '0'));
  foo[0][2] = '1';
  foo[1][3] = '1';
  foo[0][2] = '1';
  foo[2][1] = '1';
  foo[2][2] = '1';
  foo[2][3] = '1';
  foo[3][2] = '1';
  foo[3][3] = '1';
  LOG(INFO) << MaximalRectangle(foo);
  return 0;
}

