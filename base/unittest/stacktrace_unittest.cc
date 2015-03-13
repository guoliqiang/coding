// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-07-09 12:44:42
// File  : time_unittest.cc
// Brief :
#include "base/public/debug_util.h"

using namespace base;

class Foo {
 public:
  void Bar() {
     StackTrace st;
     st.PrintBacktrace();
  }
  void Bar2() {
    Bar();
  }
};

void FooBar() {
  Foo foo;
  foo.Bar2();
}

int main(int argc, char** argv) {
  FooBar();
  return 0;
}
