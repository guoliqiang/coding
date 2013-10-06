// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-06 18:03:34
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

/*
 * 如果想节省内存的话改成用3个队列，每次pop出最小的那个
 * 队列的'首元素' (要在*3 / * 5 / *7 后比较)
 * */

int FindNth(int n) {
  std::vector<int> v;
  v.push_back(3);
  v.push_back(5);
  v.push_back(7);
  int three = 0;
  int five = 0;
  int seven = 0;
  while (v.size() < n) {
    int t3 = v[three] * 3;
    int t5 = v[five] * 5;
    int t7 = v[seven] * 7;
    int min = std::min(t3, std::min(t5, t7));
    if (min == t3) {
      v.push_back(t3);
      three++;
    } else if (min == t5) {
      v.push_back(t5);
      five++;
    } else {
      v.push_back(t7);
      seven++;
    }
  }
  return v[n - 1];
}

}  // namespce algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << FindNth(5);
  return 0;
}
