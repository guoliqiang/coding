// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 21:35:44
// File  : code.cc
// Brief :

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */

class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        if (!head) return head;
        ListNode * v = head;
        ListNode * pre = head;
        ListNode * t = head;
        t = t->next;
        
        while (t != NULL) {
            if (t->val == pre->val) {
                pre = pre->next;
                t = t->next;
            } else {
                v->next = t;
                pre = pre->next;
                t = t->next;
                v = v->next;
            }
        }
        v->next = NULL;
        return head;
    }
};
