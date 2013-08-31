// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (CHANGE TO YOUR NAME)

#ifndef UTIL_CACHE_THREADSAFE_LRU_CACHE_INL_H_
#define UTIL_CACHE_THREADSAFE_LRU_CACHE_INL_H_

#include "base/public/shared_ptr.h"
#include "threadsafe_lru_cache.h"
#include "lru_cache-inl.h"

namespace cache {

template<typename TypeKey, typename TypeValue>
ThreadSafeLRUCache<TypeKey, TypeValue>::ThreadSafeLRUCache(uint32 max_size) {
  base::MutexLock lock(&mu_);
  cache_.reset(new LRUCache<TypeKey, TypeValue>(max_size));
}


template<typename TypeKey, typename TypeValue>
ThreadSafeLRUCache<TypeKey, TypeValue>::~ThreadSafeLRUCache() {
  base::MutexLock lock(&mu_);
  cache_->Clear();
}

template<typename TypeKey, typename TypeValue>
bool ThreadSafeLRUCache<TypeKey, TypeValue>::Get(
    const TypeKey &key, base::shared_ptr<TypeValue> *value) {
  base::MutexLock lock(&mu_);
  *value = cache_->Get(key);
  return value->get() != NULL;
}

template<typename TypeKey, typename TypeValue>
void ThreadSafeLRUCache<TypeKey, TypeValue>::Put(const TypeKey &key,
                                                 TypeValue *value) {
  base::MutexLock lock(&mu_);
  cache_->Put(key, value);
}

template<typename TypeKey, typename TypeValue>
void ThreadSafeLRUCache<TypeKey, TypeValue>::Put(const TypeKey &key,
     base::shared_ptr<TypeValue> value) {
  base::MutexLock lock(&mu_);
  cache_->Put(key, value);
}

template<typename TypeKey, typename TypeValue>
void ThreadSafeLRUCache<TypeKey, TypeValue>::Clear() {
  base::MutexLock lock(&mu_);
  cache_->Clear();
}

}  // namespace cache

#endif  // UTIL_CACHE_THREADSAFE_LRU_CACHE_INL_H_
