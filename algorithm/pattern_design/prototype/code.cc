// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 13:45:27
// File  : code.cc
// Brief :

// 适用于实例是在运行时产生的
#include "base/public/common_ojhead.h"

namespace algorithm {

class Prototype {
 public:
  virtual ~Prototype() {}
  virtual Prototype * Clone() = 0;
};

class ConcreatProtoA : public Prototype {
 public:
  Prototype * Clone() {
    return new ConcreatProtoA(*this);
  }
};

class ConcreatProtoB : public Prototype {
 public:
  Prototype * Clone() {
    return new ConcreatProtoB(*this);
  }
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
