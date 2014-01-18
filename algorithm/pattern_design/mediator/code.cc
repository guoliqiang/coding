// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-18 15:03:17
// File  : code.cc
// Brief :

// 当系统中的类较多，且类之间有交互，这样每个类就不得不知道
// 其它类的存在，这些类之间的关系是耦合紧密的不利于扩展
// 可以声明一个中介着，每个类只和中介这交互，中介着协调各个类之间的交互

#include "base/public/common_ojhead.h"

namespace algorithm {
class Colleague;

// 中介者必须知道所有的要交互的类
class Mediator {
 public:
  virtual ~Mediator() {}
 protected:
  std::vector<Colleague *> vec;
};

// 中介着
class ConcreateMediator : public Mediator {
};

// 必须具有中介着指针，以便和中介着交互
class Colleague {
 public:
  Colleague(Mediator * ptr = NULL) : mediator(ptr) {}
 protected:
  Mediator * mediator;
};

// A 不知道B的存在
class ConcreateColleagueA : public Colleague {
 public:
};

class ConcreateColleagueB : public Colleague {
 public:
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
