// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 16:34:45
// File  : rsur.cc
// Brief :

#include "rsur.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::stack<int> s;
  for (int i = 0; i < 10; i++) {
    s.push(i);
  }
  ReverseStack(s);
  while (!s.empty()) {
    LOG(INFO) << s.top();
    s.pop();
  }
  return 0;
}
