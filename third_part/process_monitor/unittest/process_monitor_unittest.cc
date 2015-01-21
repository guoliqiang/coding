// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-20 00:35:32
// File  : process_monitor_unittest.cc
// Brief :

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/process_monitor/public/process_monitor.h"

int main(int argc, char** argv) {
  char * * iargv = new char *[argc];
  for (int i = 1; i < argc; i++) iargv[i - 1] = argv[i];
  iargv[argc] = NULL;
  process_monitor::ProcessMonitor pm(iargv, false);
  pm.Start();
  pm.Join();
  delete [] iargv;
  return 0;
}
