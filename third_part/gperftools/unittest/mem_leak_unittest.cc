// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-21 19:18:11
// File  : mem_leark_unittest.cc
// Brief :

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "third_part/gperftools/include/gperftools/heap-checker.h"
#include "base/public/logging.h"

int main(int argc, char** argv) {
  for (int i = 0; i < 100; i++) new char;
  HeapLeakChecker::NoGlobalLeaks();
  return 0;
}
