// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-01 22:57:15
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// C++ 的list即是双向链表
// map如果换成hashmap速度会更快
// TODO read code:https://code.google.com/p/concurrentlinkedhashmap/wiki/Design
namespace algorithm {
class LRUCache{
public:
    LRUCache(int capacity) {
      size = capacity;
    }
    
    int get(int key) {
      if (!kv.count(key)) return -1;
      return kv[key];
    }
    
    void set(int key, int value) {
      if (kv.count(key)) Update(key, value);
      else {
        if (kv.size() == size) Delete();
        Insert(key, value);
      }
    }
    
    void Update(int key, int value) {
      kv[key] = value;
      list.splice(list.begin(), list, tmap[key]);
      /*
      list.erase(tmap[key]);
      list.push_front(key);
      tmap[key] = list.begin();
      */
    }
    
    void Insert(int key, int value) {
      kv[key] = value;
      list.push_front(key);
      tmap[key] = list.begin();
    }
    
    void Delete() {
      int key = list.back();
      kv.erase(key);
      list.erase(tmap[key]);
      tmap.erase(key);
    }
    
    int size;
    std::map<int, int> kv;
    std::list<int> list;
    std::map<int, std::list<int>::iterator> tmap;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LRUCache cache(10); 
  cache.set(10,13);
  cache.set(3,17);
  cache.set(6,11);
  cache.set(10,5);
  cache.set(9,10);
  cache.get(13);
  cache.set(2,19);
  cache.get(2);
  cache.get(3);
  cache.set(5,25);
  cache.get(8);
  cache.set(9,22);
  cache.set(5,5);
  cache.set(1,30);
  cache.get(11);
  cache.set(9,12);
  cache.get(7);
  cache.get(5);
  cache.get(8);
  cache.get(9);
  cache.set(4,30);
  cache.set(9,3);
  cache.get(9);
  cache.get(10);
  cache.get(10);
  cache.set(6,14);
  cache.set(3,1);
  cache.get(3);
  cache.set(10,11);
  cache.get(8);
  cache.set(2,14);
  cache.get(1);
  cache.get(5);
  cache.get(4);
  cache.set(11,4);
  cache.set(12,24);
  cache.set(5,18);
  cache.get(13);
  cache.set(7,23);
  cache.get(8);
  cache.get(12);
  cache.set(3,27);
  cache.set(2,12);
  cache.get(5);
  cache.set(2,9);
  cache.set(13,4);
  cache.set(8,18);
  cache.set(1,7);
  cache.get(6);
  cache.set(9,29);
  cache.set(8,21);
  cache.get(5);
  cache.set(6,30);
  cache.set(1,12);
  cache.get(10);
  cache.set(4,15);
  cache.set(7,22);
  cache.set(11,26);
  cache.set(8,17);
  cache.set(9,29);
  cache.get(5);
  cache.set(3,4);
  cache.set(11,30);
  cache.get(12);
  cache.set(4,29);
  cache.get(3);
  cache.get(9);
  cache.get(6);
  cache.set(3,4);
  cache.get(1);
  cache.get(10);
  cache.set(3,29);
  cache.set(10,28);
  cache.set(1,20);
  cache.set(11,13);
  cache.get(3);
  cache.set(3,12);
  cache.set(3,8);
  cache.set(10,9);
  cache.set(3,26);
  cache.get(8);
  cache.get(7);
  cache.get(5);
  cache.set(13,17);
  cache.set(2,27);
  cache.set(11,15);
  cache.get(12);
  cache.set(9,19);
  cache.set(2,15);
  cache.set(3,16);
  cache.get(1);
  cache.set(12,17);
  cache.set(9,1);
  cache.set(6,19);
  cache.get(4);
  cache.get(5);
  cache.get(5);
  cache.set(8,1);
  cache.set(11,7);
  cache.set(5,2);
  cache.set(9,28);
  cache.get(1);
  cache.set(2,2);
  cache.set(7,4);
  cache.set(4,22);
  cache.set(7,24);
  cache.set(9,26);
  cache.set(13,28);
  cache.set(11,26);
  return 0;
}
