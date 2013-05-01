// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-05-02 00:13:24
// File  : lazy_instace_unittest.cc
// Brief :
#include "../public/lazy_instance.h"
#include "../public/logging.h"
#include "../public/at_exit.h"
#include "../public/thread.h"
using namespace base;

class A {
 public:
  A() { LOG(INFO) << "constructor";}
  ~A() { LOG(INFO) << "destructor";}
  void Print() {
    LOG(INFO) << "print";
  }
};


static LazyInstance<A> my_instance = LAZY_INSTANCE_INITIALIZER;

class B : public base::Thread {
 public:
  virtual void Run() {
    for (int i = 0; i< 10; i++) {
     my_instance.Get().Print();
    }
  }
};

void f() {
  my_instance.Get().Print();
}
int main(int argc, char** argv) {
  AtExitManager  exit_manager;
  B b;
  b.SetJoinable(true);
  b.Start();
  b.Join();
  f();
  // sleep(10);
  return 0;
}
