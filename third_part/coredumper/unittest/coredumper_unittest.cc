// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-03-17 23:39:37
// File  : coredumper_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "base/public/thread.h"
#include "third_part/coredumper/include/google/coredumper.h"

class ThreadFoo : public base::Thread {
 public:
  ThreadFoo() : base::Thread(false) {}
  void Run() {
    while (true) {
      double var = 0;
      for (int i = 0; i < 10000; i++) {
        var += i;
      }
    }
  }
};

void CoreDump() {
  WriteCoreDump("./test.core");
}

void TestCoreDump() {
  CoreDump();
}

int main(int argc, char** argv) {
  ThreadFoo foo1;
  ThreadFoo foo2;
  foo1.Start();
  foo2.Start();
  TestCoreDump();
  return 0;
}

