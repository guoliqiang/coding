// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-04-06 00:45:36
// File  : mutex.h
// Brief :

#ifndef  BASE_MUTEX_H_
#define  BASE_MUTEX_H_
#include "./define.h"
#include <pthread.h>
namespace base {
class Mutex {
 public:
  Mutex();
  ~Mutex();
  void Lock();
  bool TryLock();
  void UnLock();
 private:
  pthread_mutex_t mu_;
 private:
  DISABLE_ASSIGN_COPY(Mutex)
};
//
class MutexWrapper {
 public:
  MutexWrapper();
  ~MutexWrapper();
 private:
  Mutex mutex_;
 private:
  DISABLE_ASSIGN_COPY(MutexWrapper)
};
}  // namespace
#endif  //BASE_MUTEX_H_
