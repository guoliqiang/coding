// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "base/public/logging.h"
#include "third_part/mempool/samf_mempool/public/samf_mempool.h"

namespace util {
namespace mempool {
namespace benchmark {

static const int kAllocTimes = 10000;
static const int kItemSize = 16;
static const int kItemPerBlock = 1024;

void RawMallocAndFree(int iteration) {
  vector<void*> blocks;
  blocks.reserve(kAllocTimes * kItemPerBlock);
  for (int iter = 0; iter < iteration; ++iter) {
    for (int i = 0; i < kAllocTimes * kItemPerBlock; ++i) {
      blocks.push_back(malloc(kItemSize));
    }
    size_t size = blocks.size();
    for (int i = 0; i< size; ++i) {
      free(blocks[i]);
    }
    blocks.clear();
  }
}

void SamfMallocAndFree(int iteration) {
  SamfMempool mempool(kItemSize, kItemPerBlock, 10000);
  vector<void*> blocks;
  blocks.reserve(kAllocTimes);
  for (int iter = 0; iter < iteration; ++iter) {
    for (int i = 0; i< kAllocTimes; ++i) {
      blocks.push_back(mempool.FetchBlock());
    }
    size_t size = blocks.size();
    for (int i = 0; i < size; ++i) {
      char *p = reinterpret_cast<char*>(blocks[i]);
      for (int item_idx = 0; item_idx < kItemPerBlock; ++item_idx) {
        mempool.FreeItem(p);
        p += kItemSize;
      }
    }
    blocks.clear();
  }
}

BENCHMARK(SamfMallocAndFree)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(RawMallocAndFree)->ThreadRange(1, testing::NumCPUs());

}  // namespace benchmark
}  // namespace mempool
}  // namespace util

