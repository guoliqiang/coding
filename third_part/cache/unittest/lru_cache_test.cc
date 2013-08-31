// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "base/public/logging.h"
#include "../public/lru_cache-inl.h"
#include "../public/threadsafe_lru_cache-inl.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

namespace cache {

TEST(LRUCacheTest, Normal) {
  LRUCache<int, int> cache(5);
  cache.Put(2, new int(2));
  base::shared_ptr<int> ptr(new int(1));
  cache.Put(1, ptr);
  cache.Put(3, base::shared_ptr<int>(new int(3)));
  EXPECT_EQ(1, *cache.Get(1).get());
  EXPECT_EQ(3, *cache.Get(3).get());
  EXPECT_EQ(2, *cache.Get(2).get());
  EXPECT_EQ(1, *cache.Get(1).get());
  EXPECT_EQ(2, *cache.Get(2).get());
  EXPECT_EQ(3, *cache.Get(3).get());
  EXPECT_EQ(NULL, cache.Get(4).get());
}

TEST(LRUCacheTest, SameKey) {
  LRUCache<int, int> cache(4);
  cache.Put(1, new int(1));
  cache.Put(1, new int(2));
  cache.Put(3, new int(3));
  EXPECT_EQ(2, *cache.Get(1).get());
  EXPECT_EQ(3, *cache.Get(3).get());
  EXPECT_EQ(2, *cache.Get(1).get());
}

TEST(LRUCacheTest, Overflow) {
  LRUCache<int, int> cache(3);
  cache.Put(1, new int(1));
  cache.Put(2, new int(2));
  cache.Put(2, new int(1));
  cache.Put(3, new int(3));
  cache.Put(4, new int(4));
  EXPECT_EQ(1, *cache.Get(2).get());
  EXPECT_EQ(3, *cache.Get(3).get());
  EXPECT_EQ(NULL, cache.Get(1).get());
  EXPECT_EQ(4, *cache.Get(4).get());
}

}  // namespace cache
