// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-07 19:02:51
// File  : code.cc
// Brief :

#include "base/public/common_head.h"
#include "base/public/mutex.h"
namespace algorithm {

template <typename T>
class LightSingleton {
 public:
  static T * GetInstance() {
    if (object == NULL) {
      base::MutexLock l(&mu);
      if (object == NULL) object = new T();
    }
    return object;
  }

  static void Release() {
    if (object != NULL) delete object;
    object = NULL;
  }

 private:
  static T * object;
  static base::Mutex mu;
};

template <typename T>
base::Mutex LightSingleton<T>::mu;

template <typename T>
T * LightSingleton<T>::object = NULL;

/*
 * 如果想让Foo更合理，需要将Foo的构造和拷贝构造赋值私有，
 * 然后声明 LightSingleton<Foo> 为 Foo的friend class
 * */
class Foo {
 public:
  static Foo * GetInstance() {
    return LightSingleton<Foo>::GetInstance();
  }
};

}  // namepsace algorithm


using namespace algorithm;


int main(int argc, char** argv) {
  Foo * t = Foo::GetInstance();
  LOG(INFO) << t;
  return 0;
}
