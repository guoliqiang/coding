// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-20 13:26:52
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
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        int carry = 0;
        ListNode * root = NULL;
        ListNode * before = NULL;
        while (l1 != NULL & l2 != NULL) {
            int v = (l1->val + l2->val + carry) % 10;
            carry = (l1->val + l2->val + carry) / 10;
            if (root == NULL) {
                root = new ListNode(v);
                before = root;
            } else {
                before->next = new ListNode(v);
                before = before->next;
            }
            l1 = l1->next; l2 = l2->next;
        }
        
        ListNode * bar = NULL;
        if (l1 == NULL) bar = l2;
        else bar = l1;
        while (bar != NULL) {
            int v = (bar->val + carry) % 10;
            carry = (bar->val + carry) / 10;
            before->next = new ListNode(v);
            before = before->next;
            bar = bar->next;
        }
        
        while (carry > 0) {
            before->next = new ListNode(carry % 10);
            before = before->next;
            carry /= 10;
        }
        return root;
    }
};
