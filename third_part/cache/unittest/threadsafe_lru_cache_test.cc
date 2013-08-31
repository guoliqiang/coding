// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "base/logging.h"
#include "base/shared_ptr.h"
#include "base/thread_pool.h"
#include "base/time.h"
#include "base/yr.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "util/cache/threadsafe_lru_cache-inl.h"

namespace cache {

typedef ThreadSafeLRUCache<int, int> Cache;

TEST(ThreadSafeLRUCacheTest, Normal) {
  Cache cache(5);
  cache.Put(2, new int(2));
  cache.Put(1, new int(1));
  cache.Put(3, new int(3));
  shared_ptr<int> value;
  EXPECT_TRUE(cache.Get(1, &value));
  EXPECT_EQ(1, *value);
  EXPECT_TRUE(cache.Get(3, &value));
  EXPECT_EQ(3, *value);
  EXPECT_TRUE(cache.Get(2, &value));
  EXPECT_EQ(2, *value);
  EXPECT_TRUE(cache.Get(1, &value));
  EXPECT_EQ(1, *value);
  EXPECT_TRUE(cache.Get(2, &value));
  EXPECT_EQ(2, *value);
  EXPECT_TRUE(cache.Get(3, &value));
  EXPECT_EQ(3, *value);
  EXPECT_FALSE(cache.Get(4, &value));
}

void SetCache(Cache *cache, int multiplier) {
  for (int i = 2; i < 1000; ++i) {
    shared_ptr<int> value(new int(i * multiplier));
    cache->Put(i, value);
  }
}

void SetThread(Cache *cache) {
  int64 now = GetTimeInMs();
  int multiplier = 1;
  while (GetTimeInMs() - now < 500) {
    SetCache(cache, multiplier);
    ++multiplier;
  }
}

void ReadThread(Cache *cache) {
  int64 now = GetTimeInMs();
  shared_ptr<int> value;
  while (GetTimeInMs() - now < 500) {
    for (int i = 2; i < 1000; ++i) {
      bool r = cache->Get(i, &value);
      if (r)
        ASSERT_EQ(0, *value % i);
    }
  }
}

TEST(ThreadSafeLRUCacheTest, Multithread) {
  Cache cache(900);
  ThreadPool pool(7);
  pool.StartWorkers();
  for (int i = 0; i < 2; ++i) {
    pool.Add(NewOneTimeCallback(SetThread, &cache));
  }
  for (int i = 0; i < 5; ++i) {
    pool.Add(NewOneTimeCallback(ReadThread, &cache));
  }
}

}  // namespace cache
