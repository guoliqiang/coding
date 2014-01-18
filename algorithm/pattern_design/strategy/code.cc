// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 00:52:30
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

// 策略模式，可以选择同一类型的不同算法,
// 每个算法写在不同的类里面，相互之间保持独立

class Context;
class Strategy {
 public:
  virtual void Work(Context * context);
  // 如果想让strategy不知道contex的存在,
  // 需要将stragety需要的所有输入都以参数输入进来
  virtual ~Strategy() {}
};

class Context {
 public:
  void Do() {
    strategy->Work(this);
  }
 private:
  Strategy * strategy;
 private:  // other information
};

class SimpleStrategy : public Strategy {
 public:
  void Work(Context * context) {
    LOG(INFO) << "simple";
  }
};

class ComplexStrategy : public Strategy {
 public:
  void Work(Context * context) {
    LOG(INFO) << "complex";
  }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
