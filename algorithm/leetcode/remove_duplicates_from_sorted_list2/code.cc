// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 21:44:49
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 18/18 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 60 milli secs
 * Progress: 166/166 test cases passed.
 *
 * */

// 感觉已经是相当不错的写法了
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
 public:
   ListNode *deleteDuplicates(ListNode *head) {
     // Start typing your C/C++ solution below
     // DO NOT write int main() function
     ListNode * rs = NULL;
     ListNode ** pre = &rs;
     while (head != NULL) {
       ListNode * t = head->next;
       while (t != NULL && t->val == head->val) t= t->next;
       if (t == head->next) {
         *pre = head;
         pre = &(head->next);
       }
       head = t;
     }
     *pre = NULL;
     return rs;
   }
};

