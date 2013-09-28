// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 11:14:36
// File  : code.cc
// Brief :

/*
 * 链表题目思考三种解决方法 简化编程难度
 * 1. 增加guard节点
 * 2.定义只想指针的指针
*/
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
    ListNode *partition(ListNode *head, int x) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
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
};
