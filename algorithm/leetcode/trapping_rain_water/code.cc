// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 10:57:18
// File  : code.cc
// Brief :

/*
 * Given n non-negative integers representing an elevation map where the width of each bar
 * is 1, compute how much water it is able to trap after raining.
 *
 * For example, 
 * Given [0,1,0,2,1,0,1,3,2,1,2,1], return 6.
 *
 *
 * The above elevation map is represented by array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of
 * rain water (blue section) are being trapped. Thanks Marcos for contributing this image!
 *
 * */

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 44 milli secs
 * Progress: 314/314 test cases passed.
 *  
 * 如果发现思路变得复杂时要停下脚步，
 * 一定有更好的，更容易实现的方法.
 *
 * */

#include <algorithm>
#include <vector>
#include "base/public/logging.h"

namespace algorithm {
int Calculate(int * a, int left, int right) {
  int rs = 0;
  int foo = std::min(a[left], a[right]);
  for (int i = left; i <= right; i++) {
    if (a[i] < foo) rs += (foo - a[i]);
  }
  return rs;
}

void Merge(std::vector<std::pair<int, int> > & v, int * a) {
  if (v.size() <= 1) return;
  std::vector<std::pair<int, int> > foo;
  bool flag = true;
  while (flag) {
    foo.clear();
    std::pair<int, int> pre(-1, -1);
    flag = false;
    for (int i = 0; i < v.size(); i++) {
      if (pre.first != - 1 && v[i].first <= pre.second &&
          a[pre.first] >= a[pre.second] &&
          a[v[i].second] >= a[v[i].first]) {
          pre.second = v[i].second;
          flag = true;
        } else {
          if (pre.first != -1) {
            foo.push_back(pre);
          }
          pre.first = v[i].first;
          pre.second = v[i].second;
        }
    }
    if (pre.first != -1) {
      foo.push_back(pre);
    }
    v = foo;
  }
}

bool Find(int * a, int i,
          int n, int * o_l, int * o_r) {
  int left = i - 1;
  int right = i + 1;
  if (left >= 0 && a[left] >= a[i] &&
      right < n && a[right] >= a[i]) {
    left--;
    right++;
    while (left >= 0 && a[left] >= a[left + 1]) left--;
    while (right < n && a[right] >= a[right - 1]) right++;
    *o_l = left + 1;
    *o_r = right - 1;
    if (a[*o_l] <= a[i] || a[*o_r] <= a[i]) return false;
    return true;
  } else {
    return false;
  }
}

int Trap(int * a, int n) {
  int rs = 0;
  int i = 0;
  std::vector<std::pair<int, int> > v;
  while (i < n) {
    int left = 0;
    int right = 0;
    if (Find(a, i, n, &left, &right)) {
      v.push_back(std::make_pair(left, right));
      i = right + 1;
    } else {
      i++;
    }
  }
  Merge(v, a);
  for (int i = 0; i < v.size(); i++) {
    rs += Calculate(a, v[i].first, v[i].second);
  }
  return rs;
}

}  // namespace algorithm

/*
 *
 * O(n) solution. for each bar, find the max height bar on the left and right.
 * then for this bar it can hold min(max_left, max_right) - height
 *
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 48 milli secs
 * Progress: 314/314 test cases passed.
 *
 *
 * */
namespace other {
int Trap(int * a, int n) {
  std::vector<int> lmax(n, 0);
  std::vector<int> rmax(n, 0);
  for (int i = 0; i < n; i ++) {
    if (i == 0) {
      lmax[i] = a[i];
      rmax[n - 1 - i] = a[n - 1];
    } else {
      lmax[i] = std::max(lmax[i - 1], a[i]);
      rmax[n - 1 - i] = std::max(rmax[n - i], a[n - 1 - i]);
    }
  }
  int rs = 0;
  for (int i = 0; i < n; i++) {
    rs += std::min(lmax[i], rmax[i]) - a[i];
  }
  return rs;
}
}  // namespace other

using namespace algorithm;

namespace NB {
int trap(std::vector<int>& height) {
  int ans = 0;
  int n = height.size();
  if (n == 0) return ans;

  int b = 0;
  int e = n - 1;
  int leftmax = height[b];
  int rightmax = height[e];

  while (b <= e) {
    if (height[b] < height[e]) {
      ans += std::max(0, leftmax - height[b]);
      leftmax = std::max(leftmax, height[b]);
      b++;
    } else {
      ans += std::max(0, rightmax - height[e]);
      rightmax = std::max(rightmax, height[e]);
      e--;
    }
  }
  return ans;
}
}  // namespace NB

int main(int argc, char** argv) {
  // int A[] = {0,1,0,2,1,0,1,3,2,1,2,1};
  // int A[] = {5,5,1,7,1,1,5,2,7,6};
  // int A[] = {4,2,0,3,2,5};
  int A[] = {4,3,3,9,3,0,9,2,8,3};
  // int A[] = {8,8,1,5,6,2,5,3,3,9};
  // 看这个例子就可以知道为什么得到的结果需要先merge再计算
  int size = sizeof(A) / sizeof(int);
  LOG(INFO) << Trap(A, size);
  LOG(INFO) << other::Trap(A, size);
  return 0;
}
