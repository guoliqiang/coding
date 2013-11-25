// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-23 02:06:02
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class A {
 public:
  virtual void MethodA(int a) {
    HERE(INFO);
  }
  virtual void MethodA(int a, int b) {
    HERE(INFO);
  }
};

class B : public A {
  public:
  void MethodA() {
    HERE(INFO);
  }
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  B b;
  b.MethodA();
  // b.MethodA(3);  // 被隐藏
  // b.MethodA(3, 2);  // 被隐藏
  return 0;
}
