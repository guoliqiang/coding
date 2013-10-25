// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-25 02:32:13
// File  : code.cc
// Brief : 又名 “稳定婚姻问题”
// 唏嘘～～
// http://www.matrix67.com/blog/archives/2976

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;

struct Men {
  int state;
  std::stack<int>  priority;
  Men() : state(0){}
} men[MAX];

struct Women {
  int hasband;
  std::vector<int> priority;
  Women() : hasband(-1) {
    priority.resize(MAX, -1);
  }
} women[MAX];

void StableMatch() {
  for (int i = 0; i < N; i++) {
    bool flag = false;
    for (int j = 0; j < N; j++) {
      if (men[j].state == 1) continue;
      flag = true;
      int t = men[j].priority.top();
      men[j].priority.pop();
      if (women[t].hasband == -1 ||
          women[t].priority[j] < women[t].priority[women[t].hasband]) {
        men[j].state = 1;
        if (women[t].hasband != -1) men[women[t].hasband].state = 0;
        women[t].hasband = j;
      }
    }
    if (!flag) break;
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}
