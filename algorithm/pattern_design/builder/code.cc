// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 13:10:18
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

class PartA {
 public:
  virtual ~PartA() {}
};
class PartA1 : public PartA {
};

class PartA2 : public PartA {
};

class PartB {
 public:
  virtual ~PartB() {}
};
class PartB1 : public PartB {
};

class PartB2 : public PartB {
};

// 一个对象由不同零件组成，这样零件组装的算法独立于组成部件
class Build {
 public:
  virtual PartA * BuildA() {return NULL;}
  virtual PartB * BuildB() {return NULL;}
  virtual ~Build() {}
};

// build product1
class BuildProduct1 : public Build {
 public:
  PartA * BuildA() {
    return new PartA1();
  }
  PartB * BuildB() {
    return new PartB1();
  }
};

// build product2
class BuildProduct2 : public Build {
 public:
  PartA * BuildA() {
    return new PartA2();
  }
  PartB * BuildB() {
    return new PartB2();
  }
};

class Director {
 public:
  Director(Build * p) : build(p) {}
  // 组装零件的算法位于此
  void Work() {}
 private:
  Build * build;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
