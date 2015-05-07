// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-03-15 19:43:52
// File  : timer_unittest.cc
// Brief :

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <sys/syscall.h>
#include <list>
#include <string>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/thread.h"

void SetTimer() {
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 1000000 / 1000;
  timer.it_value = timer.it_interval;
  setitimer(ITIMER_PROF, &timer, 0);
}

void SetTimerThread() {
  int rv;
  struct sigevent sevp;
  timer_t timerid;
  struct itimerspec its;
  memset(&sevp, 0, sizeof(sevp));
  sevp.sigev_notify = SIGEV_THREAD_ID;
  sevp._sigev_un._tid = syscall(__NR_gettid);
  const int signal_number = SIGPROF;
  sevp.sigev_signo = signal_number;
  clockid_t clock = CLOCK_THREAD_CPUTIME_ID;
  rv = timer_create(clock, &sevp, &timerid);
  if (rv) {
    LOG(FATAL) << "aborting due to timer_create error:" << strerror(errno);
  }

  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 1000000000 / 100000;
  its.it_value = its.it_interval;
  rv = timer_settime(timerid, 0, &its, 0);
  if (rv) {
    LOG(FATAL) << "aborting due to timer_settime error:" << strerror(errno);
  }
}

void SignalHandler(int sig, siginfo_t* sinfo, void* ucontext) {
  LOG(INFO) << "catch " << sig << " " << syscall(__NR_gettid);
}

void RegisterSignal() {
  struct sigaction sa;
  sa.sa_sigaction = SignalHandler;
  sa.sa_flags = SA_RESTART | SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  const int signal_number = SIGPROF;
  CHECK(sigaction(signal_number, &sa, NULL) == 0) << "sigprof (enable)";
}

class ThreadFoo : public base::Thread {
 public:
  explicit ThreadFoo(bool start_timer) : base::Thread(true) {
    start_timer_ = start_timer;
  };
  void Run() {
    if (start_timer_) SetTimerThread();
    while (true) {
      int foo = 0;
      for (int i = 0; i < 100000; i++) {
        foo += i;
      }
    }
  }
 private:
  bool start_timer_;
};

int main(int argc, char** argv) {
  RegisterSignal();
  SetTimerThread();
  ThreadFoo foo1(true);
  ThreadFoo foo2(false);
  foo1.Start();
  foo2.Start();
  while (true) {
    int foo = 0;
    for (int i = 0; i < 100000; i++) {
      foo += i;
    }
  }
  return 0;
}
