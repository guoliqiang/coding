// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-21 02:44:39
// File  : code.cc
// Brief :

/*
Design and implement a data structure for Least Recently Used (LRU) cache. It should support the following operations: get and set.

get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
set(key, value) - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the 

nice code：
https://code.google.com/p/concurrentlinkedhashmap/wiki/Design
*/

/*
 * 17 / 17 test cases passed.
 * Status: Accepted
 * Runtime: 844 ms
 * Submitted: 0 minutes ago
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
class LRUCache{
 
 public:
  LRUCache(int capacity) {
    size_ = capacity;
  }
    
  int get(int key) {
    if (key_value_.count(key)) {
      Update(key);
      return key_value_[key];
    } else {
      return -1;
    }
  }
    
  void set(int key, int value) {
    if (key_value_.count(key)) {
      key_value_[key] = value;
      Update(key);
    } else {
      while (keys_.size() >= size_) {
        Delete(keys_.back());
      }
      Insert(key, value);
    }
  }
  
  void Out() {
    HERE(INFO);
    for (std::list<int>::iterator i = keys_.begin(); i != keys_.end(); i++) {
      LOG(INFO) << *i;
    }
  }

 private:
  void Insert(int key, int value) {
    key_value_[key] = value;
    keys_.push_front(key);
    key_iterator_.insert(std::make_pair(key, keys_.begin()));
  }

  void Delete(int key) {
    key_value_.erase(key);
    keys_.erase(key_iterator_[key]);
    key_iterator_.erase(key);
  }

  void Update(int key) {
    keys_.splice(keys_.begin(), keys_, key_iterator_[key]);  // 目的list是第二个参数, 记住
    /* or
    keys_.erase(tmap[key]);
    keys_.push_front(key);
    key_iterator_[key] = keys_.begin();
    */
  }
 

  int size_;
  std::map<int, int> key_value_;
  std::list<int> keys_;
  std::map<int, std::list<int>::iterator > key_iterator_;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LRUCache cache(5);
  for (int i = 0; i < 10; i++) {
    cache.set(i, i);
    cache.Out();
  }
  for (int i = 0; i < 10; i++) {
    LOG(INFO) << i << ":" << cache.get(i);
    cache.Out();
  }
  return 0;
}
