// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-16 15:31:17
// File  : code2.cc
// Brief :

#include "base/public/common_ojhead.h"

// todo增加内存池
namespace algorithm {
template <typename T>
class HashSet {
 
 struct Node {
   Node(const T & v) : val(v) {};
   Node() {};
   T val;
   Node * next;
 };

 public:
  HashSet(int p = 997) {
    prime = std::max(997, p);
    head = new Node*[prime];
    memset(head, 0, prime * sizeof(Node*));
  }

  int Hahs(int v) {
    return v % prime;
  }

  int Hash(const char * p) {
    int t = 0;
    while (*p != '\0') {
      t = (t * 31 + *p) % prime;
      p++;
    }
    return t;
  }

  int Hash(const std::string & str) {
    return Hash(str.c_str());
  }

  bool Insert(const T & v) {
    int idx = Hash(v);
    Node * pre = NULL;
    Node * t = head[idx];
    while (t != NULL) {
      if (t->val == v) return false;
      pre = t;
      t = t->next;
    }
    if (pre == NULL) head[idx] = new Node(v);
    else pre->next = new Node(v);
    size++;
    return true;
  }

  bool Find(const T & v) {
    int idx = Hash(v);
    Node * t = head[idx];
    while (t != NULL) {
      if (t->val == v) return true;
      t = t->next;
    }
    return false;
  }

  void Delete(const T & v) {
    if (Find(v)) {
      int idx = Hash(v);
      Node * pre = NULL;
      Node * t = head[idx];
      while (t->val != v) {
        pre = t;
        t = t->next;
      }
      if (pre == NULL) head[idx] = t->next;
      else pre->next = t->next;
      delete t;
      size--;
    }
  }

 private:
  int size;
  int prime;
  Node * * head;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  HashSet<std::string> hash_set;
  return 0;
}
