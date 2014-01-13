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

// O(n) 因为每个位置最多进栈一次处栈一次
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

// DP vijos1055
namespace NB {
int MaxRectangle(std::vector<std::vector<char> > & matrix) {
  int m = matrix.size();
  int n = m == 0 ? 0: matrix[0].size();
  std::vector<int> left(n, 0);    // 从左向右数第一个不小于height[i]的数的下表
  std::vector<int> right(n, n);   // 从右向左数第一个不小于height[i]的数的下表
  std::vector<int> height(n, 0);  // hegiht 多有数据为0，此时left right中的数，显然是符合定义的
                                  // 余下的操作就是在height更新时保持left right的性质
  
  int max = 0;
  for (int i = 0; i < m; i++) {
    int l = 0;
    int r = n;
    for (int j = 0; j < n; j++) {
      if (matrix[i][j] == '1') {
        left[j] = std::max(l, left[j]);
        height[j]++;
      } else {
        l = j + 1;
        height[j] = 0;
        // 根据left right的定义显然
        left[j] = 0;
        right[j] = n;
      }
    }
    for (int j = n - 1; j>= 0; j--) {
      if (matrix[i][j] == '1') {
        right[j] = std::min(r, right[j]);
        max = std::max(max, height[j] * (right[j] - left[j]));
      } else {
        r = j;
      }
    }
  }
  return max;
}
}  // namespace NB

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
  LOG(INFO) << NB::MaxRectangle(foo);
  return 0;
}

