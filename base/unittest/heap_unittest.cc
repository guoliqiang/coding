// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-31 13:32:06
// File  : heap_unittest.cc
// Brief :
#include "base/public/heap.h"
#include "stdlib.h"
using namespace base;  // NOLINT

int main(int argc, char** argv) {
  Heap<int> heap;
  for (int i = 0; i < 10; i++) {
    heap.Push(random()%100);
  }
  while (heap.Size() > 0) {
    LOG(INFO) << heap.Get();
    heap.Pop();
  }
  HERE(INFO);

  FixSizeHeap<int> fheap(5);
  for (int i = 0; i < 10; i++) {
    fheap.Push(random()%100);
  }
  while (fheap.Size() > 0) {
    LOG(INFO) << fheap.Get();
    fheap.Pop();
  }

  return 0;
}
