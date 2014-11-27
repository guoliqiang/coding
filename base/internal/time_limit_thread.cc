// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-25 22:42:22
// File  : time_limit_thread.cc
// Brief :

#include "base/public/time_limit_thread.h"
#include "base/public/logging.h"

namespace base {

void TimeLimitThread::Start() {
  pthread_attr_t attr;
  CHECK_EQ(pthread_attr_init(&attr), 0);
  CHECK_EQ(pthread_attr_setdetachstate(
          &attr,
          false ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED), 0);
  int result = pthread_create(&tid_, &attr, &ThreadRunner, this);
  CHECK_EQ(result, 0) << "Could not create thread (" << result << ")";
  CHECK_EQ(pthread_attr_destroy(&attr), 0);
  started_ = true;
}

void TimeLimitThread::Exit() {
  MutexLock lock(&mutex_);
  CHECK(finished_ == false);
  finished_ = true;
  cond_var_.SignalAll();
}

bool TimeLimitThread::Join() {
  CHECK(started_ == true) << "thread not start!";
  base::MutexLock lock(&mutex_);
  if (finished_ == false) {
    if (cond_var_.WaitWithTimeout(&mutex_, time_out_)) {
      already_time_out_ = true;
      return false;
    }
  }
  return true;
}

std::list<shared_ptr<TimeLimitThread> > TimeLimitThread::threads;
Mutex TimeLimitThread::mutex;

}  // namespace base
