// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "../public/counter.h"
#include "../public/thread_pool.h"
#include "../public/time.h"
#include "../../third_part/testing/gtest/include/gtest/gtest.h"

REGISTER_COUNTER(counter1);
REGISTER_COUNTER(counter2);

namespace base {

TEST(Counter, normal) {
  counter::EnableCounters();

  COUNTER(counter1).Reset();
  COUNTER(counter1).Increment();
  COUNTER(counter1).IncrementBy(4);
  ASSERT_EQ(5, COUNTER(counter1).value());
  COUNTER(counter1).Reset();
  ASSERT_EQ(0, COUNTER(counter1).value());
}

TEST(Counter, kuint64max) {
  counter::EnableCounters();
  COUNTER(counter1).Reset();
  COUNTER(counter1).IncrementBy(kuint64max);
  ASSERT_EQ(kuint64max, COUNTER(counter1).value());
}

void Incre() {
  counter::EnableCounters();
  COUNTER(counter2).Increment();
}

TEST(Counter, Multi) {
  counter::EnableCounters();
  ThreadPool *pool = new ThreadPool(100);
  for (int i = 0; i < 100; ++i)
    pool->Add(NewOneTimeCallback(Incre));
  pool->StartWorkers();

  // wait for all threads' stop
  MilliSleep(100);
  delete pool;

  ASSERT_EQ(100, COUNTER(counter2).value());
}

TEST(Counter, PrintCounters) {
  counter::EnableCounters();
  string counters = counter::PrintCounters();
  LOG(INFO) << "counters: " << counters;
  ASSERT_FALSE(counters.empty());
}

}  // namespace base
