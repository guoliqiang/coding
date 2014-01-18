// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-16 20:03:00
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

template<typename T>
class Array {
 public:
 class Iterator {
   public:
    Iterator(Array * p) : ptr(p), idx(0) {};
    
    void First() {
      idx = 0;
    }

    bool IsDone() {
      return idx == ptr->Size();
    }

    void Next() {
      idx++;
    }

    T CurrentValue() {
      return ptr->Get(idx);
    }
   
   private:
    Array * ptr;
    int idx;
 };

 public:
  Array(int n) : size(n) {
    data = new T[size];
  }

  ~Array() {
    delete data;
  }

  int Size() {
    return size;
  }

  T Get(int k) {
    return data[k];
  }

  void Set(const T & t, int k) {
    data[k] = t;
  }

  Iterator GetIte() {
    return Iterator(this);
  }
 private:
  T * data;
  int size;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  Array<int> vec(10);
  for (int i = 0; i < 10; i++) vec.Set(i * 10, i);
  for (Array<int>::Iterator i = vec.GetIte(); !i.IsDone(); i.Next()) {
    LOG(INFO) << i.CurrentValue();
  }
  return 0;
}
