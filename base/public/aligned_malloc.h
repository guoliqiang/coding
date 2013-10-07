// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-07 16:36:41
// File  : align_malloc.h
// Brief :

#ifndef  __ALIGN_MALLOC_H_
#define  __ALIGN_MALLOC_H_

#include <stdlib.h>

namespace base {
// align must be pow of 2
void * AlignedMalloc(size_t required, size_t align) {
  // TODO(guoliqiang) CHECK the input of align
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

}  // namespace base

#endif  // __ALIGN_MALLOC_H_
