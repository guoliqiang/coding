// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 15:29:02
// File  : code.cc
// Brief :

#include "base/public/logging.h"
#include <vector>
#include <algorithm>
using namespace std;


// time limited

int maxArea(std::vector<int> &height) {
  // Start typing your C/C++ solution below
  // DO NOT write int main() function
  int max_value = -1;
  int max_index = -1;
  for (int i = 1; i < height.size(); i++) {
    int b = 0;
    int e = i;
    LOG(INFO) << "i: " << i << " max_value:" << max_value << " index:" << max_index;
    if (max_value != -1) {
      if(height[i] == min(height[i - 1], height[max_index])) {
        max_value += height[i];
        continue;
      } else {
        if (height[i] > min(height[i - 1], height[max_index])) {
          b = max_index;
        } else {
          e = max_index;
        }
      }
    }
    LOG(INFO) << i << " b :" << b << " e :" << e;
    for (int j = b; j <= e; j++) {
      if (max_value < min(height[i], height[j]) * (i - j)) {
        max_value = min(height[i], height[j]) * (i - j);
        max_index = j;
      }
    }
  }
  return max_value;
}

/*
 *
 * 若 ai ~ aj 是最大值，那么ai的左边一定不存在比ai大的值，同理aj的右边也不可能
 * 存在比aj大的值。
 *
 * 初始a0   ~  an作为候选最大值，假设a0 < an,
 * 那么下一个可能的最大值（如果存在这个值应该是比此时值大的数中的最小数）
 * 方法是 移动a0到ai，ai是第一个大于a0的数，此时假设 ai > an,那么下一个候选
 * 就是移动an到aj，aj是第一个大于an的数。
 *
 * aj~an 与 a0~ai直接组成的候选值一定小于a0~an计算出的候选值。
 *
 * http://blog.csdn.net/a83610312/article/details/8548519
 *  
 * */

int MaxArea(std::vector<int> & h) {
  int b = 1;
  int e = h.size() - 1;
  int max = 0;
  while (b <= e) {
    max = std::max(max, (e - b) * std::min(h[b], h[e]));
    if (h[b] < h[e]) {
      int foo = b;
      while (foo <= e && h[foo] <= h[b]) foo++;
      b = foo;
    } else {
      int foo = e;
      while (foo >= b && h[foo] <= h[e]) foo--;
      e = foo;
    }
  }
  return max;
}

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(4);
  foo.push_back(3);
  LOG(INFO) << maxArea(foo);
  LOG(INFO) << MaxArea(foo);
  return 0;
}
