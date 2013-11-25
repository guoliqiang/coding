// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-23 02:15:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class A {
 public:
  A() {
    HERE(INFO);
  }
  virtual ~A() {
   LOG(INFO) << "~A";
  }
};

// 只要A的析构声明为virtual, B, C的析构
// 自动成为virtual函数
class B : public A {
  public:
   B() {
     HERE(INFO);
   }
   ~B() {
    LOG(INFO) << "~B";
   }
};

class C : public B {
 public:
  C() {
    HERE(INFO);
  }
  ~C() {
    LOG(INFO) << "~C";
  }
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  // B b;  // 及时A的析构不是virtual也会调用，因为可以明确知道b的结构
  // A * ptr = new B();
  // delete ptr;
  A * ptr = new C();
  delete ptr;
  return 0;
}
