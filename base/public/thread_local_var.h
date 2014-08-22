// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2014-08-26 18:51:29
// File  : light_sington.h
// Brief : http://blog.csdn.net/lmh12506/article/details/8452700

#ifndef THREAD_LOCAL_VAR_H
#define THREAD_LOCAL_VAR_H

#include <pthread.h>
#include "base/public/logging.h"

namespace base {

template<typename T>
class ThreadLocalVar {
 public:
  ThreadLocalVar() {
    pthread_key_create(&pkey_, &ThreadLocalVar::Release);
    (void)sizeof(T);
  }

  ~ThreadLocalVar() {
    pthread_key_delete(pkey_);
  }

  T & Value() {
    T* thread_value = static_cast<T*>(pthread_getspecific(pkey_));
    if (thread_value == NULL) {
      T* new_obj = new T();
      pthread_setspecific(pkey_, new_obj);
      thread_value = new_obj;
    }
    return *thread_value;
  }

  template <typename A1>
  T & Value(A1 arg1) {
    T* thread_value = static_cast<T*>(pthread_getspecific(pkey_));
    if (!thread_value) {
      T* new_obj = new T(arg1);
      pthread_setspecific(pkey_, new_obj);
      thread_value = new_obj;
    }
    return *thread_value;
  }

  ThreadLocalVar & operator = (const T & v) {
    Value() = v;
    return *this;
  }

  operator T() const {
    T* thread_value = static_cast<T*>(pthread_getspecific(pkey_));
    CHECK(thread_value != NULL);
    return *thread_value;
  }

 private:
  static void Release(void *x) {
    T* obj = static_cast<T*>(x);
    delete obj;
  }

 private:
  pthread_key_t pkey_;
};
} // namespace base 
#endif   // THREAD_THREAD_LOCAL_VAR_H
