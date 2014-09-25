// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 17:54:48
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct ListNode {
  int val;
  ListNode * pre;
  ListNode * next;
  ListNode(int v = 0) : val(v), pre(this), next(this) {}
};

// trick, 参考stl中list的实现，增加一个end节点组成环形的双向链表
// [begin, end)
class List {
 public:
  List() {
    node = new ListNode();
  }
  ListNode * Begin() {
    return node->next;
  }
  ListNode * End() {
    return node;
  }
  void Insert(ListNode * ite, int k) {
    ListNode * tmp = new ListNode(k);
    tmp->next = ite;
    tmp->pre = ite->pre;
    ite->pre->next = tmp;
    ite->pre = tmp;
  }
  // insert [b, e) befot ite
  // 需要改动6个指针，需要7行code，首先从改掉之后还可以访问到的指针修改
  void Transfer(ListNode * ite, ListNode * b, ListNode * e) {
    e->pre->next = ite;
    b->pre->next = e;
    ite->pre->next = b;
    ListNode * dump = ite->pre;
    ite->pre = e->pre;
    e->pre = b->pre;
    b->pre = dump;
  }
  void Erase(ListNode * ite) {
    ite->pre->next = ite->next;
    ite->next->pre = ite->pre;
    delete ite;
  }
  // 其它很多操作都可以基于insert erase transfer三个函数实现
 private:
  ListNode * node;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
