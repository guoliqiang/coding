// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 17:38:06
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// 适用于如果完全套用面向对象的观点，系统中的对象可能成百上千个
namespace algorithm {

class Flyweight {
 public:
  virtual void Operator(int extension) = 0;
};

class ConcreateFlyweight : public Flyweight {
 public:
  void Operator(int extension) {
    LOG(INFO) << extension;
  }
 private:
  char ch;
};

// ConcreateFlyweight 是共享的，外部信息如何存储是关键
class FlyweightFactory {
 public:
  Flyweight * Get(char key) {
    if (!v.count(key)) {
      v[key] = new ConcreateFlyweight();
    }
    return v[key];
  }
 private:
  std::map<char, Flyweight *> v;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
