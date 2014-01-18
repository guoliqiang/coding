// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 19:15:35
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

// 适用于对象初始化代价比较大，但对象不总是使用到
// lazyinstance 可以保证只有在使用到才初始化

template<typename T>
class LazyInstance {
 public:
  LazyInstance() : ptr(NULL) {};
  virtual ~LazyInstance() {
    if (ptr != NULL) delete ptr;
  }

  T * operator ->() {
    return Load();
  }

  T & operator *() {
    return *Load();
  }

 private:
  T * Load() {
    if (ptr == NULL) ptr = new T();
    return ptr;
  }

 private:
  T * ptr;
};

class Foo {
 public:
  void Do() {
    LOG(INFO) << "Foo";
  }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LazyInstance<Foo> foo;
  foo->Do();
  return 0;
}
