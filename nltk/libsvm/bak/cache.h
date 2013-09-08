// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-04 19:01:25
// File  : cache.h
// Brief :

#ifndef  __CACHE_H_
#define  __CACHE_H_

#include "common.h"

namespace nltk {

// Kernel Cache
// l is the number of total data items
// size is the cache size limit in bytes
//
class Cache {
 public:
  Cache(int l,long int size);
  ~Cache();

  // request data [0,len)
  // return some position p where [p,len) need to be filled
  // (p >= len if nothing needs to be filled)
  int GetData(const int index, Qfloat **data, int len);
  void SwapIndex(int i, int j);  
 
 private:
  struct head_t {
    head_t *prev_;
    head_t *next_; // a circular list
    Qfloat *data_;
    int len_;     // data[0,len) is cached in this entry
  };
  
 private:
  int l_;
  int64_t size_;
  head_t *head_;
  head_t lru_head_;
 
 private:
  void LruDelete(head_t *h);
  void LruInsert(head_t *h);
};

}  // namespace nltk

#endif  //__CACHE_H_
