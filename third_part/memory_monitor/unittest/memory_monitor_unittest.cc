// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yangzhang@yunrang.com (Sun Yu)

#include <string>
#include <map>

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/memory_monitor/public/memory_monitor.h"

using namespace std; // NOLINT

namespace util {
namespace memory_monitor {

TEST(MemoryMonitor, RssCheck) {
  MemoryMonitor memory_monitor;
  memory_monitor.SetMonitor(kRss, 8400);
  vector<string> messages;
  vector<int*> int_pool;
  for (int i = 0; i <  100000; ++i) {
    int* t = reinterpret_cast<int*>(malloc(1024));
    int_pool.push_back(t);
    messages.clear();
    if (memory_monitor.CheckProcStat(&messages)) {
      LOG(INFO) << "First Index is " << i;
      for (int j = 0; j < messages.size(); ++j) {
        LOG(INFO) << messages[j];
      }
      break;
    }
  }
  int64 rss = memory_monitor.GetRssMemory();
  CHECK_GE(rss, 8400);
  for (int i = 0; i < int_pool.size(); ++i) {
    free(int_pool[i]);
  }
}

TEST(MemoryMonitor, RssIncreaseCheck) {
  MemoryMonitor memory_monitor;
  memory_monitor.SetMonitor(kRss, 8400);
  memory_monitor.SetMonitor(kRssIncrease, 2056);
  vector<string> messages;
  vector<int*> int_pool;
  int inc = 1024 * 1024;
  int i = 0;
  for (; i < 100000; ++i) {
    int* t = reinterpret_cast<int*>(malloc(inc));
    memset(t, 0, inc);
    int_pool.push_back(t);
    messages.clear();
    if (memory_monitor.CheckProcStat(&messages) &&
        messages.size() >= 2) {
      LOG(INFO) << "First Index is " << i;
      for (int j = 0; j < messages.size(); ++j) {
        LOG(INFO) << messages[j];
      }
      break;
    }
    inc *= 2;
  }
  EXPECT_EQ(1, i);

  int64 rss_increse = memory_monitor.GetIncRssMemory();
  CHECK_GE(rss_increse, 2056);
  LOG(INFO) << rss_increse;
  for (int j = 0; j < int_pool.size(); ++j) {
    free(int_pool[j]);
  }
}

TEST(MemoryMonitor, VSizeCheck) {
  MemoryMonitor memory_monitor;
  memory_monitor.SetMonitor(kVSize, 2000000000);
  vector<string> messages;
  vector<int*> int_pool;
  int inc = 100;
  for (int i = 0; i <  100000; ++i) {
    int* t = reinterpret_cast<int*>(malloc(inc));
    int_pool.push_back(t);
    messages.clear();
    if (memory_monitor.CheckProcStat(&messages)) {
      LOG(INFO) << "First Index is " << i;
      for (int j = 0; j < messages.size(); ++j) {
        LOG(INFO) << messages[j];
      }
      break;
    }
    inc *= 2;
  }
  for (int i = 0; i < int_pool.size(); ++i) {
    free(int_pool[i]);
  }
}
}
}
