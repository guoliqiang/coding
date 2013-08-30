// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_PUBLIC_OBJ_POOL_H_
#define UTIL_PARSE_MASTER2_PUBLIC_OBJ_POOL_H_

#include <string>
#include <vector>

#include "base/public/logging.h"

namespace parse_master {
template<class T>
class ObjPool {
 public:
  ObjPool() : free_begin_(0), max_pool_size_(0) {}
  
  explicit ObjPool(int32 max_pool_size): free_begin_(0),
                                         max_pool_size_(max_pool_size) {
    CHECK_LT(0, max_pool_size);
  }

  ~ObjPool() {
    gtl::STLDeleteElements(&pool_);
  }

  inline T* Get() {
    T* obj = NULL;
    if (free_begin_ >= pool_.size()) {
      obj = new T;
      pool_.push_back(obj);
    } else {
      obj = pool_[free_begin_];
      obj->clear();
    }
    ++free_begin_;
    return obj;
  }

  inline void Clear() {
    if (max_pool_size_ > 0 && pool_.size() > max_pool_size_) {
      for (int i = max_pool_size_; i < pool_.size(); ++i) {
        delete pool_[i];
      }
      pool_.resize(max_pool_size_);
    }
    free_begin_ = 0;
  }
 
 private:
  std::vector<T*> pool_;
  int32 free_begin_;
  int32 max_pool_size_;
  DISALLOW_COPY_AND_ASSIGN(ObjPool);
};
}

#endif  // UTIL_PARSE_MASTER2_PUBLIC_OBJ_POOL_H_
