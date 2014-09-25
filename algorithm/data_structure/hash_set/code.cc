// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-16 15:03:21
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"


namespace algorithm {
// 不适合添加delete操作
template<typename T>
class HashSet {
 struct Node {
   T val;
   int next;
 };
 
 public:
  HashSet(int c = 1500, int p = 997) {
    capacity = c;
    prime = p;
    data = new Node[c];
    head = new int[p];
    memset(head, -1, sizeof(head));
    size = 0;
  }

  int HashElement(int k) {
    return k % prime;
  }
  int HashElement(double k) {
    return HashElement(abs(k));
  }
  int HashElement(const char * ptr) {
    int rs = 0;
    while (ptr != '\0') {
      rs = (rs * 31 + *ptr) % prime;
    }
    return rs % prime;
  }
  int HashElement(const std::string & str) {
    return HashElement(str.c_str());
  }

  bool Insert(const T & k) {
    int idx = HashElement(k);
    for (int i = head[k]; i != -1; i = data[i].next) {
      if (data[i].val == k) return false;
    }
    if (size == capacity) Double();
    data[size].val = k;
    data[size].next = head[idx];
    head[idx] = size++;
  }

  bool Find(const T & k) {
    int idx = HashElement(k);
    for (int i = head[k]; i != -1; i = data[i].next) {
      if (data[i].val == k) return true;
    }
    return false;
  }

  void Double() {
    T * tmp = new T[capacity * 2];
    memcopy(data, tmp, capacity * sizeof(Node));
    delete data;
    data = tmp;
    capacity *= 2;
  }

 private:
  int prime;
  int capacity;
  
  Node* data;
  int * head;
  int size;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  HashSet<std::string> hashset;
  return 0;
}
