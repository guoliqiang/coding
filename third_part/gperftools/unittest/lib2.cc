// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2015-06-15 19:49:15
// File  : lib1.cc
// Brief :

#include "base/public/logging.h"

extern "C" {
void Foo2(void) {
  LOG(INFO) << "Foo2() in lib 2";
}
}

void Foo(void) __attribute__((alias("Foo2")));
