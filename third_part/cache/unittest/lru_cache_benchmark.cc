// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "testing/gtest/include/gtest/gtest.h"
#include "util/random/acm_random.h"
#include "util/cache/lru_cache-inl.h"

using util::ACMRandom;

namespace cache {

void Write(int iteration) {
  LRUCache<int, int> cache(5000);
  ACMRandom random(iteration);
  testing::StopBenchmarkTiming();
  testing::StartBenchmarkTiming();
  for (int i = 0; i < iteration; ++i)
    cache.Put(random.Uniform(iteration), new int(random.Uniform(50000)));
  testing::StopBenchmarkTiming();
}

static LRUCache<int, int> *int_cache = NULL;

void PrepareCache() {
  if (int_cache) return;
  const int kMax = 100000;
  int_cache = new LRUCache<int, int>(kMax);
  ACMRandom random(232);
  for (int i = 0; i < kMax; ++i)
    int_cache->Put(random.Uniform(kMax), new int(random.Uniform(kMax)));
}

void Read(int iteration) {
  PrepareCache();
  ACMRandom random(iteration);
  testing::StopBenchmarkTiming();
  testing::StartBenchmarkTiming();
  for (int i = 0; i < iteration; ++i)
    int_cache->Get(random.Uniform(iteration));
  testing::StopBenchmarkTiming();
}

BENCHMARK(Write)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(Read)->ThreadRange(1, testing::NumCPUs());

}  // namespace cache
