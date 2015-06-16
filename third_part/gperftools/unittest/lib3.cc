// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2015-06-15 19:49:15
// File  : lib1.cc
// Brief :

#include "base/public/logging.h"

void Foo();

void Bar() {
  LOG(INFO) << "In Bar call Foo()";
  Foo();
}
