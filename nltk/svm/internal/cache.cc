// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-07 22:59:15
// File  : cache.cc
// Brief :

#include "../public/cache.h"

namespace nltk {
namespace svm {

int32_t Cache::GetData(int32_t index, int32_t len,
                       base::shared_ptr<CacheNode>* rs) {
  base::shared_ptr<CacheNode> data;
  data = cache_->Get(index);
  if (data.get() != NULL) {
    if (data.get()->len >= len) {
      *rs = data;
      cache_->Put(index, data);
      return len;
    } else {
      double * ptr = new double[len];
      for (int i = 0; i < data.get()->len; i++) {
        ptr[i] = data.get()->data[i];
      }
      CacheNode * node = new CacheNode();
      node->data = ptr;
      node->len = len;
      base::shared_ptr<CacheNode> foo(node);
      *rs = foo;
      Insert(index, foo);
      return data->len;
    }
  } else {
    CacheNode * node = new CacheNode();
    double * ptr = new double[len];
    node->len = len;
    node->data = ptr;
    base::shared_ptr<CacheNode> foo(node);
    Insert(index, foo);
    *rs = foo;
    return 0;
  }
}

void Cache::Insert(int32_t index, base::shared_ptr<CacheNode> & node) {
  if (size_ > node->len) {
    size_ -= node->len;
    cache_->Put(index, node);
  } else {
    while (size_ < node->len && cache_->Size() > 0) {
      CHECK_GT(cache_->Size(), 0);
      cache::LRUCache<int32_t, CacheNode>::iterator back = cache_->back();
      size_ += back->second.get()->len;
      cache_->Erase(back);
    }
    cache_->Put(index, node);
  }
}
}  // namespace svm
}  // namespace nltk
