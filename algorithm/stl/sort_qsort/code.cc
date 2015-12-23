// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-26 21:46:50
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

bool Cmpa(const int &x, const int &y) {
  return x < y; // 从小到大排序
}

// 根据C语言的标准，它的原型是int cmp(const void* a, const void* b);函数有两个参数，都是指针，
// 分别指向需要排序的数组中的一个元素，并且两个指针不会指向同一个元素。函数返回int，如果返回值为负，
// 则说明第一个元素应该排在第二个元素的前面；如果返回值为正，则说明第一个元素应该排在第二个元素的后面；
// 如果返回值为零，则不论哪个元素排在前面均可（还记得吗？快速排序是不稳定的排序法，所以相等的两个元素在排序之后的顺序是难以确定的）。
// http://blog.sina.com.cn/s/blog_6fb54b980100n1br.html
int Cmpb(const void *x, const void *y) {
  // return *((int *)x) < *((int *)y); // 同priority_queue，它会从大到小排序(但不用用比较的方法，因为false返回0)
  return *((int *)x) - *((int *)y);
}

void Test() {
  int a[2];
  a[0] = 100;
  a[1] = 10; 
  std::sort(a, a + 2, Cmpa);
  LOG(INFO) << a[0] << " " << a[1];
  std::qsort(a, 2, sizeof(int), Cmpb);
  LOG(INFO) << a[0] << " " << a[1];
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  Test();
  return 0;
}
