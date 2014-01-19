// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 14:13:12
// File  : code.cc
// Brief :

// 非线程安全

#include "base/public/common_ojhead.h"

namespace algorithm {

template <typename T>
class Singleton {
 public:
  static T * GetInstance() {
    if (v == NULL) v = new T();
    return v;
  }
 private:
  static T * v;
};

template<typename T>
T * Singleton<T>::v = NULL;

class Foo {
 public:
  static Foo * GetInstance() {
    return Singleton<Foo>::GetInstance();
  }

  friend class Singleton<Foo>;
 
 private:  // 构造，拷贝构造，和赋值函数都要私有
  Foo(const Foo &);
  Foo() {}
  void operator = (const Foo &);
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
