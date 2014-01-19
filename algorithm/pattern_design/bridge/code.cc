// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 09:41:36
// File  : code.cc
// Brief :

// 期望抽象部分可以被其它项目复用
#include "base/public/common_ojhead.h"

namespace algorithm {
// 可复用的抽象部分
class Implementor;
class Abstraction {
 public:
  Abstraction() : imp(NULL) {}
  virtual ~Abstraction() {}
 protected:
  virtual Implementor * GetImplemtor() = 0;
 protected:
  Implementor * imp;
};

class Implementor {
 public:
  virtual ~Implementor() {}
 private:
};

// 系统特定部分
class ConcreatImplementorA : public Implementor {
 public:
 private:
};

class ConcreatImplementorB : public Implementor {
 public:
 private:
};

class RefinedAbstractionA : public Abstraction {
 protected:
  Implementor * GetImplementor() {
    return new ConcreatImplementorA();
  }
};

class RefinedAbstractionB : public Abstraction {
 protected:
  Implementor * GetImplementor() {
    return new ConcreatImplementorB();
  }
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
