// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-07 22:23:43
// File  : cache.h
// Brief :

#ifndef  __CACHE_H_
#define  __CACHE_H_

#include "base/public/logging.h"
#include "third_part/cache/public/lru_cache-inl.h"
#include "base/public/scoped_ptr.h"
#include "base/public/shared_ptr.h"

namespace nltk {
namespace svr {

struct CacheNode {
  CacheNode() : len(0), data(NULL) {}
  ~CacheNode() {
    if (data) delete [] data;
  }

  int len;
  double * data;
};

class Cache {
 public:
  Cache(int32_t len, int32_t size) {
    size_ = size;
    cache_.reset(new cache::LRUCache<int32_t, CacheNode>(len));
  }

  int32_t GetData(int32_t index, int32_t len, base::shared_ptr<CacheNode>* rs);
  void Insert(int32_t index, base::shared_ptr<CacheNode> & node);
 
 private:
  int32_t size_;
  scoped_ptr<cache::LRUCache<int32_t, CacheNode> > cache_;
};

}  // namespace svr
}  // namespace nltk

#endif  // __CACHE_H_
