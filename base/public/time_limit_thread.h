// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-25 22:42:22
// File  : time_limit_thread.h
// Brief :

#ifndef  __TIME_LIMIT_THREAD_H_
#define  __TIME_LIMIT_THREAD_H_

#include <list>
#include <pthread.h>
#include "base/public/basictypes.h"
#include "base/public/mutex.h"
#include "base/public/shared_ptr.h"

// Note: the thread must exit with "return" of Run function. Beacuse
// pthread_cancel and pthread_kill both are possible result to memory leak.
//
// Usage:
// class Foo : public TimeLimitThread {
//  public:
//   Foo(int timeout) : TimeLimitThread(timeout) {}
//  protected:
//   virtual void Run() {
//     do something;
//     TIMEOUTRETURN;
//     do something;
//   }
// };
//
// Note: when timeout, the thread are also running until reatch return.
//       So you must be carefull for data synchronization.

namespace base {

class TimeLimitThread {
 public:
  template <typename Type>
  static shared_ptr<TimeLimitThread> GetInstance(int time_out) {
    shared_ptr<TimeLimitThread> tmp(new Type(time_out));
    MutexLock lock(&mutex);
    for (std::list<shared_ptr<TimeLimitThread> >::iterator i = threads.begin();
         i != threads.end();) {
      if (i->get()->Finish()) i = threads.erase(i);
      else i++;
    }
    threads.push_back(tmp);
    return tmp;
  }

  TimeLimitThread(int time_out)
      : started_(false),
        finished_(false),
        already_time_out_(false),
        time_out_(time_out) { }

  virtual ~TimeLimitThread() { }

  inline pthread_t tid() const {
    return tid_;
  }

  inline bool Finish() {
    MutexLock lock(&mutex_);
    return finished_;
  }

  inline bool AlreadyTimeOut() {
    MutexLock lock(&mutex_);
    return already_time_out_;
  }
  void Start();
  bool Join();

 protected:
  virtual void Run() = 0;
  void Exit();

  static void* ThreadRunner(void* arg) {
    TimeLimitThread *t = reinterpret_cast<TimeLimitThread*>(arg);
    t->Run();
    t->Exit();
    return NULL;
  }

 private:
  pthread_t tid_;
  bool started_;
  bool finished_;
  bool already_time_out_;
  int time_out_;
  Mutex mutex_;
  CondVar cond_var_;

  static std::list<shared_ptr<TimeLimitThread> > threads;
  static Mutex mutex;
  DISALLOW_COPY_AND_ASSIGN(TimeLimitThread);
};

#define TIMEOUTRETURN do { if (AlreadyTimeOut()) return; } while(0)
}  // namespace base

#endif  //__TIME_LIMIT_THREAD_H_
