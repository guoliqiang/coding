// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 10:17:08
// File  : code.cc
// Brief :

// 根据传入的工厂不同可以在应用内生成不同的对象
#include "base/public/common_ojhead.h"

namespace algorithm {

class AbstractProductA;
class AbstractProductB;

class AbstractFactory {
 public:
  virtual ~AbstractFactory() {}

  virtual AbstractProductA * CreatProductA() {
    return NULL;
  }

  virtual AbstractProductB * CreatProductB() {
    return NULL;
  }
};

class AbstractProductA {
 public:
  virtual ~AbstractProductA() {}
};

class ProductA1 : public AbstractProductA {
};

class ProductA2 : public AbstractProductA {
};

class AbstractProductB {
 public:
  virtual ~AbstractProductB() {}
};

class ProductB1 : public AbstractProductB {
};

class ProductB2 : public AbstractProductB {
};

class ConcreatFactoryA : public AbstractFactory {
 public:
  virtual AbstractProductA * CreatProductA() {
    return new ProductA1();
  }
};

class ConcreatFactoryB : public AbstractFactory {
 public:
  virtual AbstractProductB * CreatProductB() {
    return new ProductB1();
  }
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
