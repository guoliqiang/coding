// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-11-09 04:49:13
// File  : code.cc
// Brief :


#include "base/public/common_ojhead.h"
namespace algorithm {

class Foo {
 public:
  Foo() {
    LOG(INFO) << "Foo()";
  }

  ~Foo() {
    LOG(INFO) << "~Foo()";
  }

  Foo(const Foo & foo) {
    LOG(INFO) << "Foo(const Foo & foo)";
  }

  Foo & operator = (const Foo & foo) {
    LOG(INFO) << "operator =";
    return *this;
  }
  int value;
};

// this compiler will use RVO(return value optimize)
Foo Test() {
  Foo foo;
  foo.value = 10;
  LOG(INFO) << &foo;
  return foo;
}

// this compiler can not user RVO
Foo Test2(bool cond) {
  Foo foo;
  Foo bar;
  return cond ? foo : bar;
}

} // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  Foo foo = Test2(true);
  LOG(INFO) << &foo;
  return 0;
}
