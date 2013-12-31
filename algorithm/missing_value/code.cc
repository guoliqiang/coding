// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-29 22:24:33
// File  : code.cc
// Brief :

/*
如果输入是无序的，使用Sum(1 .. n) - vector<int>

如果产生越界的话使用边加边减的方法
*/
#include "base/public/common_ojhead.h"

namespace algorithm {
// 针对输入是排序好的情况可以用log(n)的时间
// 搜索出来
int Missing(std::vector<int> & v) {
  int b = 0;
  int e = v.size() - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (mid == v[mid]) b = mid + 1;
    else e = mid - 1;
  }
  return b;
}
// 1 3 5 7 9
int Missing2(std::vector<int> & v) {
  int b = 0;
  int e = v.size() - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (v[mid] == 2 * mid + 1) b = mid + 1;
    else e = mid - 1;
  }
  return 2 * b + 1;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::vector<int> v;
  v.push_back(0);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  v.push_back(6);
  v.push_back(7);
  v.push_back(8);
  LOG(INFO) << Missing(v);
  v.clear();
  v.push_back(1);
  v.push_back(3);
  // v.push_back(5);
  v.push_back(7);
  v.push_back(9);
  v.push_back(11);
  LOG(INFO) << Missing2(v);
  return 0;
}
