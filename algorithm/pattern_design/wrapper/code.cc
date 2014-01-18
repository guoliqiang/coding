// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 11:12:58
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// 统一封装所有的wrapper
// 给每个类动态的增加功能，继承是静态的，会增加不必要的功能

namespace algorithm {

template <typename T, typename R>
class GeneralComponent {
 public:
  virtual R Do(T & t) = 0;
  virtual ~GeneralComponent() {}
};

class FooComponent : public GeneralComponent<std::string &, int> {
  public:
   int Do(std::string & str) {
     return 100;
   }
};

// wrapper和组件具有相同的接口
// 如果组件需要增加很多额外的操作，增加到wrapper中
template <typename T, typename R>
class GeneralWrapper : public GeneralComponent<T, R> {
 public:
  virtual R Do(T & t) {
    return component->Do(t);
  };

  virtual ~GeneralWrapper() {};
  
  GeneralWrapper(GeneralComponent<T, R> * ptr) : component(ptr) {}
 protected:
  GeneralComponent<T, R> * component;
};

class FooWrapper : public GeneralWrapper<std::string &, int> {
  public:
   FooWrapper(GeneralComponent<std::string &, int> * ptr) : GeneralWrapper<std::string &, int>(ptr) {}
   int Do(std::string & str) {
     return component->Do(str);
   }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  GeneralComponent<std::string &, int> * w = new FooWrapper(new FooComponent());
  std::string str = "hello";
  LOG(INFO) << w->Do(str);
  return 0;
}
