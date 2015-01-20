// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-20 00:35:32
// File  : signal_monitor_unittest.cc
// Brief :

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/signal_monitor/public/signal_monitor.h"

void Foo(const std::string input) {
  LOG(INFO) << input;
}

TEST(SignalMonitor, Normal) {
  base::Closure * callback = base::NewPermanentCallback(Foo,
      std::string("Hello Word"));
  signal_monitor::SignalMonitor::GetInstance()->AddCallback(12, callback);
  while (true) sleep(1);
}
