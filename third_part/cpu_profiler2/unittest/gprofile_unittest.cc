// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-08 16:02:47
// File  : profile_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "third_part/cpu_profiler2/public/profiler.h"

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

int main(int argc, char** argv) {
  ProfilerStart("./gcpu_profiler.prof");
  for (int i = 0; i < 10; i++) Fib(40);
  ProfilerStop();
  return 0;
}
