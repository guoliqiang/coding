// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-07 18:51:29
// File  : light_sington.h
// Brief :
// 轻量级的单例模式

#ifndef  __LIGHT_SINGLETON_H_
#define  __LIGHT_SINGLETON_H_

#include "mutex.h"

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

#endif  //__LIGHT_SINGLETON_H_
