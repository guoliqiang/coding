// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-07 16:17:35
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace NB {

void * AlignedMalloc(size_t required, size_t align) {
  int offset = align - 1 + sizeof(void *);
  void * p1 = malloc(offset + required);
  if (p1 == NULL) return NULL;
  void ** p2 = reinterpret_cast<void**>((reinterpret_cast<size_t>(p1) + offset) & ~(align - 1));
  p2[-1] = p1;
  return reinterpret_cast<void*>(p2);
}

void AlignedFree(void * p) {
  free((reinterpret_cast<void **>(p))[-1]);
}
}  // namespace algorithm

using namespace NB;


int main(int argc, char** argv) {
  char * p = (char *)AlignedMalloc(1000, 128);
  p[0] = 'c';
  LOG(INFO) << p[0];
  AlignedFree(p);
  return 0;
}

