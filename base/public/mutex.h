// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-04-06 00:45:36
// File  : mutex.h
// Brief :

#ifndef  BASE_PUBLIC_MUTEX_H_
#define  BASE_PUBLIC_MUTEX_H_
#include <pthread.h>
#include "./define.h"
#include "./logging.h"
namespace base {
class Mutex {
 public:
  enum LinkerInitialized {LINKER_INITIALIZED};
 public:
  Mutex();
  Mutex(LinkerInitialized);
  ~Mutex();
  void Lock();
  bool TryLock();
  void Unlock();
 private:
  bool need_destroy_;
  pthread_mutex_t mu_;
 private:
  DISABLE_ASSIGN_COPY(Mutex)
};
//
class MutexLock {
 public:
  explicit MutexLock(Mutex * mutex);
  ~MutexLock();
 private:
  Mutex * mutex_;
 private:
  DISABLE_ASSIGN_COPY(MutexLock)
};
}  // namespace
#endif  // BASE_PUBLIC_MUTEX_H_
