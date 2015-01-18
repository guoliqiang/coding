// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-08 16:02:47
// File  : profile_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "third_part/gperftools/wrapper/public/gperftools.h"

int Fib(int num) {
  if (num == 0 || num == 1) {
    return 1;
  } else {
    return Fib(num - 1) * Fib(num - 2);
  }
}

int Fib2(int num) {
  if (num == 0 || num == 1) {
    return 1;
  } else {
    return Fib2(num - 1) * Fib2(num - 2);
  }
}

// manually control
// profiler::Start("test.prof");
// profiler::Stop();

// or use singal
// env CPUPROFILE=test.prof CPUPROFILESIGNAL=12 ./profile_unittest &
// then begin to profiler use:
// killall -12 profile_unittest
// after a while to stop profiler use:
// killall -12 profile_unittest
//
// http://gperftools.googlecode.com/svn/trunk/doc/cpuprofile.html

int main(int argc, char** argv) {
  while (true) {
    Fib(40);
    Fib2(40);
    sleep(1);
  }
  return 0;
}
