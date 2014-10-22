// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-10-22 14:31:06
// File  : uthread.h
// Brief :

#ifndef  __UTHREAD_H_
#define  __UTHREAD_H_

#include <vector>
#include "base/public/light_singleton.h"
#include "base/public/logging.h"
#include "base/public/light_singleton.h"
#include "third_part/uthread/public/uthread.h"

namespace uthread {

class UThread {
 public:
  UThread() : start_ (false) {}
  virtual ~UThread() {}

  uthread_h utid() const {
    CHECK(start_) << "Join must be called after Start!";
    return fd_;
  }

  void Start();
  void Join();

 protected:
  virtual void Run() = 0;

  static void ThreadRunner(void* arg) {
    UThread *t = reinterpret_cast<UThread*>(arg);
    t->Run();
  }

  bool start_;
  uthread_h fd_;
 private:
  DISALLOW_COPY_AND_ASSIGN(UThread);
};

class UThreadThreadLevelDaemon {
 public:
  UThreadThreadLevelDaemon() {
    uthread_init();
  }

  ~UThreadThreadLevelDaemon() {
    for (int i = 0; i < ev.size(); i++) {
      uthread_close(ev[i]);
    }
    uthread::uthread_exit();
  }

  uthread_h CreateEvent() {
    uthread::uthread_h tmp = uthread_event_create();
    ev.push_back(tmp);
    return tmp;
  }

 private:
  std::vector<uthread_h> ev;
 private:
  DISALLOW_COPY_AND_ASSIGN(UThreadThreadLevelDaemon);
};

}  // namespace uthread

#endif  // __UTHREAD2_H_
