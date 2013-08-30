// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#include "../public/samf_mempool.h"
#include <math.h>
#include "base/public/logging.h"

namespace util {
namespace mempool {

SamfMempool::SamfMempool(size_t item_size,
                         int item_per_block,
                         bool fast_and_wasted,
                         int block_per_alloc /*100*/)
                         : item_size_(item_size),
                           item_per_block_(item_per_block),
                           block_per_alloc_(block_per_alloc),
                           fast_and_wasted_(fast_and_wasted) {
  // 是否处于快速模式
  // (TODO) guoliqiang make item_size to 2(x) and memory align.
  if (fast_and_wasted) {
    size_t real_block_size = item_size * item_per_block_ + kBlockHeaderSize;
    attach_block_size_ =
        pow(2, static_cast<uint64>(log(real_block_size - 1) / log(2)) + 1);
    mask_ = ~(attach_block_size_ - 1);
  } else {
    attach_block_size_ = item_size * item_per_block_ + kBlockHeaderSize;
    mask_ = 0;
  }
  size_per_alloc_ = (block_per_alloc_ + 1) * attach_block_size_;

  itemsize_is_2exp_ = false;
  itemsize_2exp_ = 0;
  for (itemsize_2exp_ = 0; itemsize_2exp_ < 32; ++itemsize_2exp_) {
    if (pow(2, itemsize_2exp_) == item_size_) {
      VLOG(5) << "itemsize is " << itemsize_2exp_ << " of 2";
      itemsize_is_2exp_ = true;
      break;
    }
  }
  VLOG(5) << "item_size:" << item_size_ << ","
          << "item_per_block:" << item_per_block_ << ","
          << "block_per_alloc:" << block_per_alloc_;
  VLOG(5) << "attach_block_size:" << attach_block_size_ << ","
          << "size_per_alloc:" << size_per_alloc_;
}

SamfMempool::~SamfMempool() {
  for (int i = 0; i < raw_mems_.size(); ++i) {
    free(raw_mems_[i]);
  }
}

void SamfMempool::AllocNewBlocks() {
  char* raw_mem = static_cast<char*>(malloc(size_per_alloc_));
  CHECK(raw_mem);
  raw_mems_.push_back(raw_mem);
  char* blocks = raw_mem -
                 reinterpret_cast<uint64>(raw_mem) % attach_block_size_ +
                 attach_block_size_;
  VLOG(5) << "alloc raw_mem:" << reinterpret_cast<uint64>(raw_mem) << ","
          << "real_blocks:" << reinterpret_cast<uint64>(blocks);
  char* block = blocks;
  for (int i = 0; i < block_per_alloc_; ++i, block += attach_block_size_) {
    block_info_.push_back(std::make_pair(FlagSet(), block));
    block_info_.back().first.Init(item_per_block_);
    blocks_.push_back(block);
    BlockHeader* header = GetBlockHeader(block);
    header->magic_num = kSamfMagic;
    header->block_info = &block_info_.back();
  }
}

bool SamfMempool::RecycleBlocks() {
  BlockInfoDeque::iterator it = block_info_.begin();
  BlockInfoDeque::iterator end = block_info_.end();
  for (; it != end; ++it) {
    if (it->first.None()) {
      blocks_.push_back(it->second);
    }
  }
  return !blocks_.empty();
}

void* SamfMempool::FetchBlock() {
  if (blocks_.empty()) {
    if (!RecycleBlocks()) {
      AllocNewBlocks();
    }
  }
  char* block = blocks_.back();
  blocks_.pop_back();
  BlockHeader* header = GetBlockHeader(block);
  DCHECK(header->magic_num == kSamfMagic);
  DCHECK_EQ(header->block_info->first.Count(), 0);
  DCHECK(header->block_info->second == block);
  header->block_info->first.Set();
  return block + kBlockHeaderSize;
}

void SamfMempool::FreeItem(void* item) {
  char* block = NULL;
  char* pitem = reinterpret_cast<char*>(item);
  if (fast_and_wasted_) {
    block = reinterpret_cast<char*>(reinterpret_cast<uint64>(item) & mask_);
  } else {
    block = pitem - reinterpret_cast<uint64>(item) % attach_block_size_;
  }
  BlockHeader* header = GetBlockHeader(block);
  size_t item_idx = 0;
  if (itemsize_is_2exp_) {
    item_idx = (pitem - block - sizeof(BlockHeader)) >> itemsize_2exp_;
  } else {
    item_idx = (pitem - block - sizeof(BlockHeader)) / item_size_;
  }
  DCHECK(header->magic_num == kSamfMagic);
  DCHECK_GT(header->block_info->first.Count(), 0);
  DCHECK_EQ(header->block_info->second, block);
  DCHECK(header->block_info->first.Test(item_idx));
  header->block_info->first.Reset(item_idx);
}

void SamfMempool::FreeItems(void* item, int num_item) {
  char* pitem = reinterpret_cast<char*>(item);
  for (int i = 0; i < num_item; ++i) {
    FreeItem(pitem);
    pitem += item_size_;
  }
}

void SamfMempool::FreeBlock(void* usr_block) {
  char* block = reinterpret_cast<char*>(usr_block) - kBlockHeaderSize;
  BlockHeader* header = GetBlockHeader(block);
  DCHECK(header->magic_num == kSamfMagic);
  DCHECK(header->block_info->second == block);
  DCHECK_GT(header->block_info->first.Count(), 0);
  header->block_info->first.Reset();
}

}  // namespace ymempool
}  // namespace util
