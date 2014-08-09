// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "../public/thread_safe_guard.h"
#include "../public/thread.h"
#include "../../third_part/testing/gtest/include/gtest/gtest.h"


void NotSafeFun() {
  THREAD_SAFE_FUN;
  sleep(10);
}

class NotSafeClass {
 public:
  void Do() {
    THREAD_SAFE_PTR(this);
    sleep(10);
  }
};

class Foo : public base::Thread  {
 public:
  Foo() : base::Thread(true) {}
 protected:
  void Run() {
    NotSafeFun();
  }

};

void TiggerCore() {
  Foo foo1;
  Foo foo2;
  foo1.Start();
  foo2.Start();
  foo1.Join();
  foo2.Join();
}

void TiggerNotCore() {
  Foo foo1;
  Foo foo2;
  foo1.Start();
  foo1.Join();
  foo2.Start();
  foo2.Join();
}

NotSafeClass t;

class Bar : public base::Thread  {
 public:
  Bar() : base::Thread(true) {}
 protected:
  void Run() {
    t.Do();
  }

};

void ClassTiggerCore() {
  Bar bar1;
  Bar bar2;
  bar1.Start();
  bar2.Start();
  bar1.Join();
  bar2.Join();
}

void ClassTiggerNotCore() {
  Bar bar1;
  Bar bar2;
  bar1.Start();
  bar1.Join();
  bar2.Start();
  bar2.Join();
}

int main(int argc, char** argv) {
  ClassTiggerCore();
  return 0;
}
