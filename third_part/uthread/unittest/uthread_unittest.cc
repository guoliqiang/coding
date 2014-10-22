// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-10-22 14:54:29
// File  : uthread_unittest.cc
// Brief :

#include <stack>
#include "base/public/thread.h"
#include "third_part/uthread/wrapper/uthread.h"

class Producer : public uthread::UThread {
 public:
  Producer (std::stack<int> * ptr, uthread::uthread_h ev)
      : uthread::UThread(), ptr_(ptr), ev_(ev), cnt_(0) {}
 protected:
  void Run() {
    while (true) {
      if (ptr_->size() == 0) {
        for (int i = 0; i < 1; i++) {
          ptr_->push(cnt_);
          LOG(INFO) << "producer:" << cnt_++;
        }
        uthread::uthread_event_broadcast(ev_);
      } else {
        uthread::uthread_wait(ev_);
      }
    }
  }
 private:
  std::stack<int> * ptr_;
  uthread::uthread_h ev_;
  int cnt_;
};


class Customer : public uthread::UThread {
 public:
  Customer (std::stack<int> * ptr, uthread::uthread_h ev)
      : uthread::UThread(), ptr_(ptr), ev_(ev) {}
 protected:
  void Run() {
    while (true) {
      if (ptr_->size() != 0) {
        while (!ptr_->empty()) {
          int t = ptr_->top();
          ptr_->pop();
          LOG(INFO) << "customer:" << t;
        }
        uthread::uthread_event_broadcast(ev_);
      } else {
        uthread::uthread_wait(ev_);
      }
    }
  }
 private:
  std::stack<int> * ptr_;
  uthread::uthread_h ev_;
};

class NormalThread : public base::Thread {
 public:
  NormalThread() : base::Thread(true) {}
 protected:
  void Run() {
    uthread::UThreadThreadLevelDaemon bar;
    uthread::uthread_h ev = bar.CreateEvent();
    std::stack<int> foo;
    Producer p(&foo, ev);
    Customer c(&foo, ev);
    p.Start();
    c.Start();
    p.Join();
    c.Join();
  }
 private:
};

int main(int argc, char** argv) {
  NormalThread thread_a;
  thread_a.Start();
  thread_a.Join();
  NormalThread thread_b;
  thread_b.Start();
  thread_b.Join();
  return 0;
}

