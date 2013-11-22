// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 16:22:32
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 18/18 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 72 milli secs
 * Progress: 129/129 test cases passed.
 * */

#include <vector>
#include <map>
#include <list>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "base/public/common_head.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL) {}
};

void Out(ListNode *);

/*
 * 实现赢着树,类似于堆
 * */

void BuildSucceedTree(std::vector<int> & succeed,
                      std::vector<ListNode * > & lists) {
  succeed.resize(lists.size() * 2 - 1);
  for (int i = lists.size() - 1; i >= 0; i--) {
    succeed[succeed.size() - 1 - (lists.size() - 1 - i)] = i;
  }
  
  // 从后向前两两互为兄弟节点
  for (int i = succeed.size() - 1; i >= 2; i -= 2) {
    int brother = i - 1;
    int parent = i / 2 - 1;
    if (lists[succeed[i]]->val < lists[succeed[brother]]->val) {
      succeed[parent] = succeed[i];
    } else {
      succeed[parent] = succeed[brother];
    }
  }
}

/*
 *   0
 *  1  2
 * 3 4
 * */

void UpdateSucceedTree(std::vector<int> & succeed, int n,
                       std::vector<ListNode *> & lists) {
  int index = lists.size() - 1 + n;
  while (index > 0) {
    int parent = index / 2;
    if (index % 2 == 0) parent--;
    int brother = (2 * parent + 1) == index ? index + 1 : index - 1;
    
    int val_index = 0x7fffffff;
    int val_brother = 0x7fffffff;
    if (lists[succeed[index]] != NULL) val_index = lists[succeed[index]]->val;
    if (lists[succeed[brother]] != NULL) val_brother = lists[succeed[brother]]->val;
    
    if (val_index < val_brother) {
      succeed[parent] = succeed[index];
    } else {
      succeed[parent] = succeed[brother];
    }
    index = parent;
  }
}

bool Terminal(std::vector<ListNode*> & lists, ListNode ** p) {
  *p = NULL;
  for (int i = 0; i <lists.size(); i++) {
    if (lists[i] != NULL) {
      if (*p == NULL) *p = lists[i];
      else return false;
    }
  }
  return true;
}


ListNode * MergeKLists(std::vector<ListNode *> & lists_i) {
  std::vector<ListNode *> lists;
  for (int i = 0; i < lists_i.size(); i++) {
    if (lists_i[i] != NULL) lists.push_back(lists_i[i]);
  }
  if (lists.size() == 0) return NULL;
  if (lists.size() == 1) return lists[0];
  
  std::vector<int> succeed;
  BuildSucceedTree(succeed, lists);
  ListNode * head = NULL;
  ListNode * tail = NULL;
  ListNode * last = NULL;
  while (!Terminal(lists, &last)) {
    int min = succeed[0];
    if (head == NULL) {
      head = lists[min];
      tail = head;
    } else {
      tail->next = lists[min];
      tail = tail->next;
    }
    lists[min] = lists[min]->next;
    tail->next = NULL;
    UpdateSucceedTree(succeed, min, lists);
  }
  if (last != NULL && tail != NULL) tail->next = last;
  return head;
}

// for degug

void Out(ListNode * root) {
  while (root) {
    std::cout << root->val << " ";
    root = root->next;
  }
  std::cout << std::endl;
}

ListNode * MakeList(std::vector<int> & v) {
  ListNode * head = NULL;
  ListNode * tail = NULL;
  for (int i = 0; i < v.size(); i++) {
    if (head == NULL) {
      head = new ListNode(v[i]);
      tail = head;
    } else {
      tail->next = new ListNode(v[i]);
      tail = tail->next;
    }
  }
  return head;
}

}  // namespace algorithm

namespace algorithm {
class Cmp {
 public:
  bool operator ()(const int & a, const int & b) const {
    // return a < b; // 最大堆
    return a > b;  // 最小堆
  }
};

void PriorityQ() {
  std::priority_queue<int, std::vector<int>, Cmp> queue;
  for (int i = 0; i < 10; i++) queue.push(i);
  while (!queue.empty()) {
    int t = queue.top();
    queue.pop();
    LOG(INFO) << t;
  }
}
}  // namespace algorithm

namespace twice {
using namespace std;
using namespace algorithm;
class Cmp{
 public:
  // 这个定义的priority queue居然是最小堆
  // priority queue 的第三个参数需要研究下
  bool operator ()(const ListNode * p1, const ListNode * p2) const {
    return p1->val > p2->val;
  }
};

class Solution {
public:
  ListNode *mergeKLists(vector<ListNode *> &lists) {
    // Note: The Solution object is instantiated only once and is reused by each test case.
    ListNode * root = NULL;
    ListNode ** tail = &root;
    std::priority_queue<ListNode*, std::vector<ListNode*>, Cmp> queue;
    for (int i = 0; i < lists.size(); i++) {
      if (lists[i] != NULL) queue.push(lists[i]);
    }
    while (!queue.empty()) {
      ListNode * t = queue.top();
      queue.pop();
      *tail = t;
      tail = &((*tail)->next);
      if (t->next != NULL) queue.push(t->next);
    }
    return root;
  }
};
}  // namespace twice

using namespace algorithm;

int main(int argc, char** argv) {
  PriorityQ();
  return 0;
  std::vector<ListNode*> foo;
  std::vector<int> v;
  v.clear();
  v.push_back(1);
  foo.push_back(MakeList(v));

  v.clear();
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  foo.push_back(MakeList(v));

  v.clear();
  v.push_back(1);
  v.push_back(2);
  foo.push_back(MakeList(v));
  
  Out(MergeKLists(foo));
  return 0;
}
