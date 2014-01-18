// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 10:03:31
// File  : code.cc
// Brief :

// 适用于递归定义的对象
// 好处：无论是简单对象还是组合对象都有统一的接口

#include "base/public/common_ojhead.h"

namespace algorithm {
class Component {
 public:
  virtual void Do() {}
  virtual bool Add(Component * ptr) { return false;}  // 也可以抛出异常
  virtual bool Remove(Component * ptr) {return false;}
  virtual ~Component() {}  // 基类一定不要忘了将析构声明为virtual
};

class Leaf : public Component {
 public:
  void Do() {}
};

class Composite : public Component {
 public:
  void Do() {
    for (int i = 0; i < vec.size(); i++) vec[i]->Do();
  }

  bool Add(Component * ptr) {
    vec.push_back(ptr);
    return true;
  }

  bool Remove(Component * ptr) {
    for (std::vector<Component *>::iterator i = vec.begin();
         i != vec.end(); i++) {
      if (*i == ptr) {
        vec.erase(i);
        return true;
      }
    }
    return false;
  }

 private:
  std::vector<Component * > vec;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
