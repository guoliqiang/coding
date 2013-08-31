// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#ifndef UTIL_CACHE_THREADSAFE_LRU_CACHE_H_
#define UTIL_CACHE_THREADSAFE_LRU_CACHE_H_

#include <list>
#include <utility>

#include "base/public/basictypes.h"
#include "base/public/mutex.h"
#include "base/public/scoped_ptr.h"
#include "base/public/shared_ptr.h"
#include "lru_cache.h"

namespace cache {

// This is a thread safe LRU cache.
// It is actually a simple wrapper of LRUCache.
template<typename TypeKey, typename TypeValue>
class ThreadSafeLRUCache {
 public:
  explicit ThreadSafeLRUCache(uint32 max_size);
  ~ThreadSafeLRUCache();

  bool Get(const TypeKey &key, base::shared_ptr<TypeValue> *value);

  // Saves value in cache.
  // If the key already exists, the new value will replace the old one.
  // The cache will own the value and delete it when finish jobs.
  void Put(const TypeKey &key, TypeValue *value);
  void Put(const TypeKey &key, base::shared_ptr<TypeValue> value);

  uint32 Size() const {
    return cache_->Size();
  }

  // Clear all values
  void Clear();

 private:
  scoped_ptr<LRUCache<TypeKey, TypeValue> > cache_;
  base::Mutex mu_;

  DISALLOW_COPY_AND_ASSIGN(ThreadSafeLRUCache);
};

}  // namespace cache

#endif  // UTIL_CACHE_THREADSAFE_LRU_CACHE_H_
