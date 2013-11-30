// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 11:14:36
// File  : code.cc
// Brief :

/*
Given a linked list and a value x, partition it such that all nodes less than x come before nodes greater than or equal to x.
You should preserve the original relative order of the nodes in each of the two partitions.
For example,
Given 1->4->3->2->5->2 and x = 3,
return 1->2->2->4->3->5.
*/

/*
 * 链表题目思考三种解决方法 简化编程难度
 * 1. 增加guard节点
 * 2.定义只想指针的指针
 *
*/

#include "base/public/common_head.h"

namespace algorithm {
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

// 这个办法当初是怎么想出来的？？？
ListNode *partition(ListNode *head, int x) {
  ListNode * small = NULL;
  ListNode * big = NULL;
  ListNode ** s_ptr = &small;
  ListNode ** g_ptr = &big;
  while (head!= NULL) {
    if (head->val >= x) {
      *g_ptr = head;
      g_ptr = &((*g_ptr)->next);
    } else {
      *s_ptr = head;
      s_ptr = &((*s_ptr)->next);
    }
    head = head->next;
  }
  *s_ptr = big;
  *g_ptr = NULL;
  return small;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}
