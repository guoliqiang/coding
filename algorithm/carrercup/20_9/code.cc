// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 14:20:46
// File  : code.cc
// Brief :

/*
 * Numbers are randomly generated and passed to a method.
 * Write a program to find and maintain the median value as new values are generated.
 * */

#include "base/public/common_head.h"

namespace algorithm {
// 中位数的定义是排序后的
// 中位数：A[n/2]  n  == 偶数
// 中位数：(A[(n-1)/2] + A[n/2])/2  n == 奇数
// 自己实现的是取中间的那个数，没有排序的。
int Median(int v) {
  static std::queue<int> queue;
  static bool flag = false;
  if (!queue.empty()) {
    if (flag) queue.pop();
    flag = !flag;
  }
  queue.push(v);
  return queue.front();
}

double Median2(int v) {
  static std::list<int> queue;
  static bool flag = false;
  double t = v;
  if (!queue.empty()) {
    if (flag) {
      queue.pop_front();
      t = queue.front();
    } else {
      t = queue.front();
      int tmp = t;
      queue.pop_front();
      t += (queue.size() == 0 ? v : queue.front());  // bug fixed
      queue.push_front(tmp);
      t /= 2;
    }
    flag = !flag;
  }
  queue.push_back(v);
  return t;
}

}  // namespace algorithm

namespace NB {

// 插入新元素还是O(logn)的时间，而取中位数只需要O(1)的时间
// NB http://hawstein.com/posts/20.9.html
// http://www.cnblogs.com/flyoung2008/articles/2136485.html
// priority queue是基于heap实现的
std::priority_queue<int, std::vector<int>, std::less<int> > left_heap;  // 最大堆
std::priority_queue<int, std::vector<int>, std::greater<int> > right_heap;  // 最小堆

double Median(int v) {
  if (left_heap.size() == 0) left_heap.push(v);
  else {
    if (v <= left_heap.top()) left_heap.push(v);
    else right_heap.push(v);
  }
  while (left_heap.size() > right_heap.size() + 1) {
    right_heap.push(left_heap.top());
    left_heap.pop();
  }

  while (right_heap.size() > left_heap.size() + 1) {
    left_heap.push(right_heap.top());
    right_heap.pop();
  }
  if (right_heap.size() == left_heap.size()) {
    return (right_heap.top() + left_heap.top()) / 2.0;
  }
  if (right_heap.size() == left_heap.size() + 1) {
    return right_heap.top();
  } else {
    return left_heap.top();
  }
}
}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  for (int i = 10; i >= 0; i--) {
    LOG(INFO) << "i:" << i << " Media:" << NB::Median(i);
  }
  return 0;
}
