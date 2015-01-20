// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-19 05:17:22
// File  : pipe_monitor_unittest.cc
// Brief :

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/pipe_monitor/public/pipe_monitor.h"

void Foo(const std::string input) {
  LOG(INFO) << input;
}

TEST(pipe_monitor, Normal) {
  pipe_monitor::PipeMonitor pm("/tmp/pmonitor", true);
  base::Closure * callback = base::NewPermanentCallback(Foo,
      std::string("Hello Word"));
  pm.AddCallback('G', callback);
  pm.Start();
  pm.Join();
}
