// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-04-06 00:58:16
// File  : mutex.cc
// Brief :
#include "../public/mutex.h"
namespace base {
// constructor
Mutex::Mutex() : need_destroy_(true) {
  CHECK_EQ(0, pthread_mutex_init(&mu_, NULL));
}

Mutex::Mutex(Mutex::LinkerInitialized) : need_destroy_(false) {
  CHECK_EQ(0, pthread_mutex_init(&mu_, NULL));
}
// destructor
Mutex::~Mutex() {
  if (need_destroy_) {
    CHECK_EQ(0, pthread_mutex_destroy(&mu_));
  }
}
// lock
void Mutex::Lock() {
  CHECK_EQ(0, pthread_mutex_lock(&mu_));
}
// unlock
void Mutex::Unlock() {
  CHECK_EQ(0, pthread_mutex_unlock(&mu_));
}
// trylock
bool Mutex::TryLock() {
  return pthread_mutex_trylock(&mu_) == 0;
}

// constructor
MutexLock::MutexLock(Mutex * mutex) : mutex_(mutex){
  mutex_->Lock();
}

// destructor
MutexLock::~MutexLock() {
  mutex_->Unlock();
}

}  // namespace
