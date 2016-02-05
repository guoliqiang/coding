// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 23:55:35
// File  : code.cc
// Brief :

/*
Given n non-negative integers representing the histogram's bar height where the width of each bar is 1, find the area of largest rectangle in the histogram.
Above is a histogram where width of each bar is 1, given height = [2,1,5,6,2,3].
The largest rectangle is shown in the shaded area, which has area = 10 unit.
For example,
Given height = [2,1,5,6,2,3],
return 10.

*/

/*
 *  
 *
 *
 *
 *
 *     |  |
 *     |  |
 *     |  |  |  |
 *  |  |  |  |  |  |
 *  1  2  3  4  5  6
 *
 *  最大面积的后选值不可能是以1 2 结尾，因为可以扩展到3
 *  即：连续不减的区域，其产生的最大候选值只可能以最后一个
 *  柱子作为结尾，其开始位置不会小于当前位置（4）且距离当前
 *  位置最近的位置（1），因为这样的开始位置组成的区域可以扩展到
 *  当前位置（4a）
 *
 *  且这种连续不减区域不影响对其后某个柱子结束的最大区域的计算
 *
 *  所以其最末位置如果不是0需要认为添加一个0
 *
 *  http://blog.csdn.net/abcbc/article/details/8943485
 *  http://www.cnblogs.com/lichen782/p/leetcode_Largest_Rectangle_in_Histogram.html
 *
 *
 * */

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 51/51 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 104 milli secs
 * Progress: 94/94 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace best {
using namespace std;
// 4 2 0 3 2 5
int largestRectArea(vector<int> &h) {
    stack<int> p;
    int i = 0, m = 0;
    h.push_back(0);
    // trick 末尾加0，最后一次循环不用写了
    while(i < h.size()) {  // 写成for的形式，要注意在else部分i-- 见poj3494
        if(p.empty() || h[p.top()] <= h[i])
            p.push(i++);
        else {
            int t = p.top();
            p.pop();
            m = max(m, h[t] * (p.empty() ? i : i - p.top() - 1 ));
        }
    }
    return m;
}
}

namespace algorithm {

int LargestRetangleArea(std::vector<int>height) {
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
        // this is bug
        // consider this case 4 2 0 3 2 5
        // int tmax = stack.empty() ? height[ti] * i : height[ti] * (i - ti);
        // the begin index is dicided by its prefore one in this stack,
        // becase may prefore + 1 != i
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


}  // namespace algorithm

using namespace algorithm;

namespace twice {
int Largest(std::vector<int> height) {
  height.push_back(0);
  int max = 0;
  std::stack<int> stack;
  int i = 0;
  while (i < height.size()) {   // bug fixed, 注意写成for的形式有个trick需要注意，见poj3494
    if (stack.empty() || height[i] >= height[stack.top()]) {  // >= . > 都行
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
}  // namespace twice

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(4);
  foo.push_back(2);
  foo.push_back(0);
  foo.push_back(3);
  foo.push_back(2);
  foo.push_back(5);
  foo.push_back(0);
  LOG(INFO) << LargestRetangleArea(foo);
  LOG(INFO) << best::largestRectArea(foo);
  return 0;
}

