// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-20 11:42:00
// File  : rune_unittest.cc
// Brief :

#include "base/public/rune.h"
#include "base/public/logging.h"

using namespace base;  // NOLINT

int main(int argc, char** argv) {
  char ptr[] = "中国abcd";
  LOG(INFO) << UTF8Length(ptr) << ":" << ptr;
  
  char ptr1[] = "ab中国";
  LOG(INFO) << UTF8Length(ptr1) << ":" << ptr1;
  
  char ptr2[] = "中国";
  LOG(INFO) << UTF8Length(ptr2) << ":" << ptr2;
  
  return 0;
}
