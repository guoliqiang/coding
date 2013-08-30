// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#include "third_part/mempool/masf_mempool/public/masf_mempool.h"
#include <stdlib.h>
#include <algorithm>
#include "base/public/logging.h"
#include "third_part/gtl/public/stl_util-inl.h"

namespace util {
namespace mempool {

SolidMempool::SolidMempool(size_t item_size,
                           int item_per_block,
                           float balance_factor,
                           int  balance_period)
                           : item_size_(item_size),
                             item_per_block_(item_per_block),
                             balance_period_(balance_period),
                             next_balance_time_(balance_period),
                             max_used_per_period_(0) {
  CHECK(balance_factor > 0.0f && balance_factor <= 1.0f);
  inv_balance_factor_ = 1.0f / balance_factor;
  CHECK_GT(balance_period_, 0);
  VLOG(5) << "create solid pool: item_size:" << item_size
          << " item_per_block:" << item_per_block;
  AllocNewBlock();
}

SolidMempool::~SolidMempool() {
  Destroy();
}

void SolidMempool::Destroy() {
  for (int i = 0; i < blocks_.size(); i++) {
    free(blocks_[i]);
  }
  blocks_.clear();
}

void *SolidMempool::Malloc() {
  if (item_idx_ < item_per_block_) {
    return blocks_[block_idx_] + item_size_ * item_idx_++;
  }
  if (blocks_.size()-1 == block_idx_) {
    AllocNewBlock();
  } else {
    block_idx_++;
    item_idx_ = 0;
  }
  return blocks_[block_idx_] + item_size_ * item_idx_++;
}

void SolidMempool::AllocNewBlock() {
  char *block =
      reinterpret_cast<char*>(malloc(item_size_ * item_per_block_));
  CHECK(block);
  blocks_.push_back(block);
  block_idx_ = blocks_.size()-1;
  item_idx_ = 0;
}

void SolidMempool::Recycle() {
  BalancePool();
  item_idx_ = 0;
  block_idx_ = 0;
}

void SolidMempool::BalancePool() {
  int64 block_size = item_per_block_ * item_size_;
  uint64 used_size = block_idx_ * block_size + item_idx_ * item_size_;
  max_used_per_period_ = std::max(max_used_per_period_, used_size);
  if (--next_balance_time_ > 0) {
    return;
  }
  // balance time
  int64 total_size = blocks_.size() * block_size;
  int64 threshold = inv_balance_factor_ * max_used_per_period_;
  while (blocks_.size() > 1 && (total_size - block_size) >= threshold) {
    free(blocks_.back());
    blocks_.pop_back();
    total_size -= block_size;
  }
  // reset variables
  max_used_per_period_ = 0;
  next_balance_time_ = balance_period_;
}

MasfMempool::MasfMempool(int item_per_block,
                         float balance_factor,
                         int balance_period)
                         : item_per_block_(item_per_block),
                           balance_factor_(balance_factor),
                           balance_period_(balance_period) {
  Create();
}

MasfMempool::~MasfMempool() {
  Destroy();
}

void MasfMempool::Create() {
  if (!smp_.empty()) return;
  for (size_t size = 1; size <= kInitMaxSize; size *= 2) {
    SolidMempool* pool = new SolidMempool(size, item_per_block_,
                                          balance_factor_, balance_period_);
    smp_.push_back(pool);
  }
  max_size_ = kInitMaxSize;
}

void MasfMempool::Destroy() {
  gtl::STLDeleteElements(&smp_);
}

void *MasfMempool::Malloc(size_t size) {
  if (size <= max_size_) {
    size_t tmp = size-1;
    int idx = 0;
    if (size > 0) {
      while (tmp) {
        ++idx;
        tmp >>= 1;
      }
    }
    CHECK(idx < smp_.size());
    return smp_[idx]->Malloc();
  } else {
    size_t mp_size = max_size_ * 2;
    for (; mp_size < size; mp_size *= 2) {
      SolidMempool* pool = new SolidMempool(mp_size, item_per_block_,
                                            balance_factor_, balance_period_);
      smp_.push_back(pool);
    }
    // 超过待分配大小的内存池分配
    SolidMempool* pool = new SolidMempool(mp_size, item_per_block_,
                                          balance_factor_, balance_period_);
    smp_.push_back(pool);
    max_size_ = mp_size;
    return smp_.back()->Malloc();
  }
}

void MasfMempool::Recycle() {
  for (int i = 0; i < smp_.size(); ++i) {
    smp_[i]->Recycle();
  }
}

}  // namespace mempool
}  // namespace util
