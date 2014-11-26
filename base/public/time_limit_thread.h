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

namespace base {

class TimeLimitThread {
 public:
  template <typename Type>
  static shared_ptr<TimeLimitThread> GetInstance(int time_out) {
    shared_ptr<TimeLimitThread> tmp(new Type(time_out));
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
        time_out_(time_out) { }

  virtual ~TimeLimitThread() { }

  pthread_t tid() const {
    return tid_;
  }
  void Start();
  bool Join();
  bool Finish();

 protected:
  virtual void Run() = 0;
  void Exit();

  static void* ThreadRunner(void* arg) {
    TimeLimitThread *t = reinterpret_cast<TimeLimitThread*>(arg);
    t->Run();
    t->Exit();
    return NULL;
  }

  pthread_t tid_;
  bool started_;
  bool finished_;
  int time_out_;
  Mutex mutex_;
  CondVar cond_var_;
  static std::list<shared_ptr<TimeLimitThread> > threads;
  DISALLOW_COPY_AND_ASSIGN(TimeLimitThread);
};

}  // namespace base

#endif  //__TIME_LIMIT_THREAD_H_
