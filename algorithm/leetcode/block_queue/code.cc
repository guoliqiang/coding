// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-25 22:36:54
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class Mutex {
 public:
  void Lock() {}
  void UnLock() {}
};

class CondVar {
 public:
  void Wait(Mutex * p) {}
  void Singnal() {}
};

template <class T>
class BlockQueue {
 public:
  BlockQueue(int len) : capacity(len) {}

  void Push(const T & v) {
    mutex.Lock();
    while (queue.size() == capacity) {
      full.Wait(&mutex);
    }
    queue.push(v);
    empty.Singnal();
    mutex.UnLock();
  }

  T Get() {
    mutex.Lock();
    while (queue.size() == 0) {
      empty.Wait(&mutex);
    }
    T ans = queue.front();
    queue.pop();
    full.Singnal();
    mutex.UnLock();
    return ans;
  }
 private:
  std::queue<T> queue;
  Mutex mutex;
  CondVar full;
  CondVar empty;
  int capacity;
};

template <class T>
class SmartPtr {
 public:
  SmartPtr(T ptr = NULL) : data(ptr) {
    cnt = (ptr == NULL) ? NULL : new int(1);
  }

  ~SmartPtr() {
    if (data != NULL) {
      *cnt -= 1;
      if (*cnt == 0) {
        delete data;
        delete cnt;
      }
    }
  }

  SmartPtr(const SmartPtr<T> & other) {
    data = other.data;
    cnt = other.cnt;
    if (cnt != NULL) *cnt++;
  }

  SmartPtr<T> & operator = (const SmartPtr<T> & other) {
    if (data != other.data) {
      SmartPtr<T> tmp(other);
      Swap(tmp);
    }
    return *this;
  }

 private:
  void Swap(SmartPtr<T> * ptr) {
    std::swap(ptr->data, data);
    std::swap(ptr->cnt, cnt);
  }

 private:
  int * cnt;
  T * data;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
