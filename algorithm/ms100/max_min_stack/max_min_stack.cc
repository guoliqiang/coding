// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 15:51:51
// File  : max_min_stack.cc
// Brief :

#include "./max_min_stack.h"
#include "base/public/logging.h"
#include "stdlib.h"


using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  NB::MaxMinStack<int> nb;
  MaxMinStack<int> mmstack;
  for (int i = 0; i < 10; i++) {
    int t = random() % 100;
    mmstack.push(t);
    nb.Push(t);
  }

  LOG(INFO) << mmstack.size();
  while (mmstack.size() > 0) {
    LOG(INFO) << mmstack.top() << " " << mmstack.max() << " "
              << mmstack.min();
    mmstack.pop();
    LOG(INFO) << nb.Top() << " " << nb.Max() << " "
              << nb.Min();
    nb.Pop();
  }

  LOG(INFO) << "size:" << nb.Size();
  return 0;
}
