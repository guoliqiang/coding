// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-10-22 14:31:06
// File  : uthread.cc
// Brief :

#include "third_part/uthread/wrapper/uthread.h"

namespace uthread {

void UThread::Start() {
  fd_ = uthread_create(0, UThread::ThreadRunner, (void *)(this));
  start_ = true;
}

void UThread::Join() {
  CHECK(start_) << "Join must be called after Start!";
  uthread_wait(utid());
}

}  // namespace uthread
