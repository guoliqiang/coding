// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#ifndef UTIL_YMEMPOOL_SAMF_MEMPOOL_FLAG_SET_H_
#define UTIL_YMEMPOOL_SAMF_MEMPOOL_FLAG_SET_H_

#include <stdlib.h>
#include "base/public/logging.h"

namespace util {
namespace mempool {

//TODO(guoliqiang) use bitset instead of array of bool to save memory.

class FlagSet {
 public:
  FlagSet() : flags_(NULL), size_(0) {
  }

  explicit FlagSet(int size) {
    Init(size);
  }

  ~FlagSet() {
    if (flags_) free(flags_);
  }

  void Init(int size) {
    size_ = size;
    // sizeof(bool) == sizeof(char) == 1
    flags_ = reinterpret_cast<bool*>(malloc(size));
    memset(flags_, 0, size_);
  }

  void Set() {
    memset(flags_, 1, size_);
  }

  void Set(int idx) {
    DCHECK(idx < size_);
    flags_[idx] = true;
  }

  void Reset() {
    memset(flags_, 0, size_);
  }

  void Reset(int idx) {
    DCHECK(idx < size_);
    flags_[idx] = false;
  }

  bool Test(int idx) {
    DCHECK(idx < size_);
    return flags_[idx];
  }

  int Count() {
    int count = 0;
    for (int i = 0; i < size_; ++i) {
      count += flags_[i];
    }
    return count;
  }

  bool None() {
    int num_ulong = size_ >> 3;
    int byte_last = size_ & sizeof(uint64);
    uint64* ulong = reinterpret_cast<uint64*>(flags_);
    for (int i = 0; i < num_ulong; ++i) {
      if (ulong[i] != 0) {
        return false;
      }
    }
    for (int i = size_ - byte_last; i < size_; ++i) {
      if (flags_[i]) {
        return false;
      }
    }
    return true;
  }

  bool Any() {
    for (int i = 0; i < size_; ++i) {
      if (flags_[i]) {
        return true;
      }
    }
    return false;
  }

 private:
  bool* flags_;
  int size_;
};

}  // namespace mempool
}  // namespace util

#endif  // UTIL_YMEMPOOL_SAMF_MEMPOOL_FLAG_SET_H_
