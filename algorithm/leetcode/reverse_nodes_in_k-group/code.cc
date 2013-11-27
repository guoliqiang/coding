// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 19:38:44
// File  : code.cc
// Brief :

/*
Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.
If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.
You may not alter the values in the nodes, only nodes itself may be changed.
Only constant memory is allowed.

For example,
Given this linked list: 1->2->3->4->5
For k = 2, you should return: 2->1->4->3->5
For k = 3, you should return: 3->2->1->4->5

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 29/29 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 144 milli secs
 * Progress: 81/81 test cases passed.
 */

#include <vector>
#include <iostream>
#include "base/public/logging.h"

namespace algorithm {

struct ListNode {
  int val;
  ListNode * next;
  ListNode(int x) : val(x), next(NULL) {}
};

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


bool LetterK(ListNode * head, int k) {
  int n = 0;
  while (head != NULL) {
    head = head->next;
    n++;
    if (n >= k) return false;
  }
  return true;
}

// 这个最简单
void ReverseS(ListNode * h) {
  ListNode * pre = NULL;
  while (h != NULL) {
    ListNode * foo = h->next;
    h->next = pre;
    pre = h;
    h = foo;
  }
}

// 这个函数debug了半天，这也是一个题目
void Reverse(ListNode * h, ListNode * t) {
  if (h->next == NULL) return;

  ListNode * pre = NULL;
  while (h != NULL && h->next != NULL) {
    // 偶数时h == NULL
    ListNode * foo = h->next->next;
    ListNode * bar = h->next;
    h->next->next = h;
    h->next = pre;
    pre = bar;
    h = foo;
  }
  // 注意处理奇数末尾结点
  if (h != NULL) h->next = pre;
}

/*
 *   A  ->  B  -> C     E  -> F -> G   H
 *                |     |          |   |
 *               pre    h          t  npre
 * 
 *
 * */

ListNode *reverseKGroup(ListNode *head, int k) {
  if (k <= 1) return head;
  if (LetterK(head, k)) {
    return head;
  }
  ListNode * rs = NULL;
  
  ListNode * pre = NULL;
  ListNode * npre = NULL;
  ListNode * h = NULL;
  ListNode * t = NULL;
        
  t = head;
  h = head;
  pre = NULL;
  npre = NULL;
  
  while(h) {
  // 这个循环至少可以完成一次
    int n = k - 1;
    while (n){
      t = t->next;
      if (t == NULL) {
        pre->next = h;
        return rs;
      }
      n--;
   }
   npre = t->next;
   t->next = NULL;
   Reverse(h, t);
   if (rs == NULL) rs = t;
   if (pre != NULL) pre->next = t;
   pre = h;
   h = npre;
   t= npre;
 }
 return rs;
}


}  // namespace algorithm

using namespace algorithm;

namespace third {

// 截取前k的模板,如果可以提取出k个，其首尾指针存储于rs中，返回true，否则
// 返回false
bool Split(ListNode * & head, int k, std::pair<ListNode *, ListNode *> & rs) {
  rs.first = head;
  ListNode * temp = head;
  
  k -= 1;
  while (k && temp != NULL && temp->next != NULL) {
    temp = temp->next;
    k--;
  }
  if (k > 0 || temp == NULL) return false; // 当输入为1，temp为NULL时用到temp==NULL逻辑
  
  head = temp->next;
  temp->next = NULL;
  rs.second = temp;
  return true;
}

// Reverse 模板
std::pair<ListNode *, ListNode *> Reverse(ListNode * head) {
  ListNode * h = NULL;
  ListNode * t = NULL;
  while (head != NULL) {
    ListNode * temp = head->next;
    if (h == NULL) h = head;
    head->next = t;
    t = head;
    head = temp;
  }
  return std::make_pair(t, h);
}

ListNode * ReverseK(ListNode * head, int k) {
  ListNode * rs = NULL;
  std::pair<ListNode*, ListNode *> split;
  std::pair<ListNode*, ListNode *> list;
  if (Split(head, k, split) == false) return head;
  else rs = split.second;
  
  list = Reverse(split.first);
  ListNode * pre = list.second;
  while (head != NULL && Split(head, k, split)) {
    list = Reverse(split.first);
    pre->next = list.first;
    pre = list.second;
  }
  pre->next = head;
  return rs;
}

}  // namespace third

namespace twice {
using namespace algorithm;

class Solution {
 public:
  ListNode *reverseKGroup(ListNode *head, int k) {
    if (head == NULL) return head;
    ListNode * rs = head;
    ListNode * pre = head;
    // 在list中截取前k个的模板,当k>0时，不够k个
    int tk = k - 1;
    while (tk && pre != NULL && pre->next != NULL) {
      pre = pre->next;
      tk--;
    }
    if (tk == 0) rs = pre;
    pre = NULL;
        
    while (head != NULL) {
      ListNode * t = head;
      tk = k - 1;
      while (tk && t->next != NULL) {
        t = t->next;
        tk--;
      }
      if (tk > 0) {
        if (pre != NULL) pre->next = head;
        break;
      } else {
        ListNode * tpre = NULL;
        while (head != t) {
          ListNode * foo = head->next;
          head->next = tpre;
          tpre = head;
          head = foo;
        }
        head = t->next;
        t->next = tpre;
        if (pre != NULL) pre->next = t;
        else pre = t;
        while (pre->next != NULL) pre = pre->next;
      }
    }
    return rs;
  }
};
}  // namespace twice

int main(int argc, char** argv) {
  std::vector<int> foo;
  
  foo.clear();
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(4);
  foo.push_back(5);
  foo.push_back(6);
  // Out(reverseKGroup(MakeList(foo), 4));
  Out(third::ReverseK(MakeList(foo), 1));
  return 0;
  /*
  foo.clear();
  foo.push_back(1);
  Out(SwapPairs(MakeList(foo)));

  foo.clear();
  foo.push_back(1);
  foo.push_back(23);
  Out(SwapPairs(MakeList(foo)));


  foo.clear();
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(4);
  foo.push_back(5);
  foo.push_back(6);
  foo.push_back(7);
  Out(SwapPairs(MakeList(foo)));
  return 0;
  */
}
