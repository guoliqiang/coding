// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-16 12:34:13
// File  : code.cc
// Brief :

class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        ListNode * odd = NULL;
        ListNode ** odd_pre = &odd;
        ListNode * even = NULL;
        ListNode * * even_pre = &even;

        int cnt = 1;
        while (head != NULL) {
            if (cnt % 2) {
                *odd_pre = head;
                odd_pre = &((*odd_pre)->next);
            } else {
                *even_pre = head;
                even_pre = &((*even_pre)->next);
            }
            cnt++;
            head = head->next;
        }

        *odd_pre = even;
        *even_pre = NULL;
        return odd;
    }
};
