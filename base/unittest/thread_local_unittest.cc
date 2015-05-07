// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-08-21 22:33:51
// File  : thread_local_unittest.cc
// Brief :

#include "base/public/thread_local_var.h"
#include "base/public/logging.h"
#include "base/public/thread.h"

class Foo {
 public:
  Foo() {
    LOG(INFO) << "Foo()";
    state = 0;
  }
  ~Foo() {
    LOG(INFO) << "~Foo()";
  }
  void Increase() {
    state++;
  }
  int Get() {
    return state;
  }
 private:
  int state;
};

base::ThreadLocalVar<Foo> foo;

class Bar : public base::Thread {
 public:
  Bar() : base::Thread(true) {}
  virtual void Run() {
    while (true) {
      LOG(INFO) << "Bar";
    }
  }
 
};

int main(int argc, char** argv) {
  Bar bar;
  bar.Start();
  bar.Join();
  return 0;
}
