// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 19:20:03
// File  : stack2queue.cc
// Brief :

#include "base/public/logging.h"
#include "stack2queue.h"
using namespace algorithm;  // NOLINT


int main(int argc, char** argv) {
  MyQueue<int> queue;
  for (int i = 0; i < 5; i++) {
   queue.Push(i);
  }

  LOG(INFO) << queue.Front();
  queue.Pop();
  while (!queue.Empty()) {
    LOG(INFO) << queue.Front();
    queue.Pop();
  }

  for (int i = 5; i < 10; i++) {
   queue.Push(i);
  }
 
  LOG(INFO) << queue.Front();
  queue.Pop();
  while (!queue.Empty()) {
    LOG(INFO) << queue.Front();
    queue.Pop();
  }
  return 0;
}
