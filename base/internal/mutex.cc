// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-04-06 00:58:16
// File  : mutex.cc
// Brief :
#include "../public/mutex.h"
namespace base {
// constructor
Mutex::Mutex() {
  CHECK_EQ(0, pthread_mutex_init(&mu_, NULL));
}
// destructor
Mutex::~Mutex() {
  CHECK_EQ(0, pthread_mutex_destroy(&mu_));
}
// lock
void Mutex::Lock() {
  CHECK_EQ(0, pthread_mutex_lock(&mu_));
}
// unlock
void Mutex::UnLock() {
  CHECK_EQ(0, pthread_mutex_unlock(&mu_));
}
// trylock
bool Mutex::TryLock() {
  return pthread_mutex_trylock(&mu_) == 0;
}

// constructor
MutexWrapper::MutexWrapper() {
  mutex_.Lock();
}
// destructor
MutexWrapper::~MutexWrapper() {
  mutex_.UnLock();
}

}  // namespace
