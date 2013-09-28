// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 19:38:44
// File  : code.cc
// Brief :

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


int main(int argc, char** argv) {
  std::vector<int> foo;
  
  foo.clear();
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(3);
  foo.push_back(4);
  foo.push_back(5);
  foo.push_back(6);
  Out(reverseKGroup(MakeList(foo), 4));
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
