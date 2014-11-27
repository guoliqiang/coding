// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-25 23:29:20
// File  : time_limit_thread_unittest.cc
// Brief :

#include "base/public/time_limit_thread.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

class ThreadFoo : public base::TimeLimitThread {
 public:
  ThreadFoo(int timeout) : base::TimeLimitThread(timeout) { }

 protected:
  virtual void Run() {
    while (true) {
      sleep(1);
      EXPECT_TRUE(AlreadyTimeOut());
      TIMEOUTRETURN;
    }
  }
};

class ThreadBar : public base::TimeLimitThread {
 public:
  ThreadBar(int timeout) : base::TimeLimitThread(timeout) { }

 protected:
  virtual void Run() {
    sleep(1);
    EXPECT_FALSE(AlreadyTimeOut());
    TIMEOUTRETURN;
  }
};

TEST(TimeLimitThread, Normal) {
  base::shared_ptr<base::TimeLimitThread> foo =
      base::TimeLimitThread::GetInstance<ThreadBar>(1200);
  foo->Start();
  EXPECT_TRUE(foo->Join());
}

TEST(TimeLimitThread, Timeout) {
  base::shared_ptr<base::TimeLimitThread> foo =
      base::TimeLimitThread::GetInstance<ThreadFoo>(500);
  foo->Start();
  EXPECT_FALSE(foo->Join());
  sleep(1);
}
